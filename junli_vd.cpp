#include "junli_vd.h"
#include <string.h>
junli_vd::junli_vd()
{
    //ctor
}

junli_vd::~junli_vd()
{
    //dtor
}


bool junli_vd::parse_junli(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex)
{
 int i,j,k;

    i=0;                         //block[i]
    j=0;
    k=*lastPacketIndex;          //messageIn.packet[k]
    BYTE ckstmp=0x0;
    for (i=0; i<receiveBlockLength; i++)
    {
//      printf("_%C", messageIn[j].packet[k]=block[i]);

        if ( block[i] == 0xff)
        {
            if((i>0)&&(messageIn[j].success==true))
            {
              //  junbo_light_receive(messageIn[j]);    //save log whitch is wrong message
                vClearMsg(messageIn,j);
            }

            messageIn[j].success=true;
            k=0;
            messageIn[j].packet[k] = block[i];
            k++;

        }
        else if(k==2)
        {   messageIn[j].packet[k]=block[i];
            messageIn[j].packetLength=messageIn[j].packet[k];//junli's vd Data length not include header,ID,length,cks
        }
        else
        {
             if(k<2)
            {
                messageIn[j].packet[k]=block[i];//product ID
                k++;

            }
            else if(k<messageIn[j].packetLength-1+4)
            {
                messageIn[j].packet[k]=block[i];
                k++;

            }
            else if(k==messageIn[j].packetLength-1+4)
            {
                for(int a=3; a<messageIn[j].packetLength+3; a++)
                        ckstmp+=messageIn[j].packet[a];

                    messageIn[j].packet[k]=ckstmp;
                if(messageIn[j].packet[k]==block[i])
                {
                    messageIn[j].cksStatus=true;
                 //   junbo_light_receive(messageIn[j]);    //save log
                    vClearMsg(messageIn,j);
                    k=0;
                }
                else
                {
                 //  junbo_light_receive(messageIn[j]);    //save log whitch is wrong message
                    vClearMsg(messageIn,j);
                    k=0;
                };

            }
        }
    }

    *lastPacketIndex=k;
    return true;

}

bool junli_vd::vClearMsg(MESSAGEOK *messageIn, unsigned short int msgID)
{
    messageIn[msgID].cksStatus = false;
    messageIn[msgID].success=false;
    messageIn[msgID].UDPorRS232=0;
    messageIn[msgID].ReadorWrite=0;
    messageIn[msgID].bGetDLE = false;
    messageIn[msgID].bGetSOH = false; //the same with STX
    messageIn[msgID].bGetACK = false;
    messageIn[msgID].bGetNAK = false;
    messageIn[msgID].uiPacketTEXTLEN = 65535;

    // messageIn[msgID].usiExternCounter = 0;;

    /*
    for (int b = 0; b < BUFFERSIZE; b++) {
      messageIn[msgID].packet[b]=0;
    }
    */
    bzero(messageIn[msgID].packet, BUFFERSIZE);
}
