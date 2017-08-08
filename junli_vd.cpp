#include "junli_vd.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
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

                vClearMsg(messageIn,j);
            }

            messageIn[j].success=true;
            k=0;
            messageIn[j].packet[k] = block[i];
            k++;

        }
        else if(k==1)
        {
            messageIn[j].packet[k]=block[i];//product_ID
            k++;
        }
        else if(k==2)//data length
        {
            messageIn[j].packet[k]=block[i];
            messageIn[j].packetLength=messageIn[j].packet[k];//junli's vd Data length not include header,ID,length,cks
            k++;
        }
        else
        {
            if(k<messageIn[j].packetLength-1+4)
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

                    if(messageIn[j].packet[1]==0xed) {} //make cms light_on
                    else if(messageIn[j].packet[1]==0x1c) {} //record the VD still alive
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
void junli_wrong_record(MESSAGEOK messageIn,int junli_length)
{
    try
    {
        char input_string[25];
        char record_file[80];
        struct tm* currenttime;
        time_t now = time(NULL);
        currenttime = localtime(&now);
        int length=0;
        FILE *pf=NULL;
        char cFileTmp[64]= {0};
        sprintf(cFileTmp, "/cct/Data/junli_record/");

        char dateTemp[32]= {0};
        sprintf(dateTemp, "%#04d%#02d%#02d", currenttime->tm_year+1900, currenttime->tm_mon+1, currenttime->tm_mday);

        strcat(cFileTmp,dateTemp);
        strcat(cFileTmp,"_junli_vd_record.txt");
        char cTimeHeader[64]= {0};
        sprintf(cTimeHeader, " %#04d/%#02d/%#02d %#02d:%#02d:%#02d\n\0", currenttime->tm_year+1900, currenttime->tm_mon+1, currenttime->tm_mday, currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
         for (int i=0; i<300; i++)
            if (cTimeHeader[i]=='\0')
            {
                length=i;
                break;
            }
        memset(input_string,'0',sizeof(input_string));
        pf=fopen(cFileTmp,"a+");
        if(pf!=NULL)
        {
            sprintf(input_string,"receive junli_protocol=");
            fwrite(input_string,sizeof(char),sizeof(input_string),pf);
            for(int i=0; i<junli_length; i++)
            {
                sprintf(input_string," %x ",messageIn.packet[i]);
                fwrite(input_string,3*sizeof(char),sizeof(char),pf);
            }
            fwrite( cTimeHeader, length, 1, pf );
            fclose(pf);
        }
        else printf("pf=NULL!!\n");
        usleep(30000);
    }
    catch(...) {}
}
/*
bool junli_vd::vd_bool_junbo_cms(MESSAGEOK messageIn)
{



}*/

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


