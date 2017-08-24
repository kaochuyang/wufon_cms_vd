//---------------------------------------------------------------------------
#include "PARSETRAFFIC.h"
#include "SMEM.h"

#include <stdio.h>
#include <string.h>

#include "screenCurrentCommPacket.h"
//---------------------------------------------------------------------------
PARSETRAFFIC::PARSETRAFFIC(void)
{
}
//---------------------------------------------------------------------------
PARSETRAFFIC::~PARSETRAFFIC(void)
{
}

//---------------------------------------------------------------------------
bool PARSETRAFFIC::ParseBlock(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex,int *maxMessageIndex)
{
try {
    int i,j,k;
    unsigned char tempcks;

    i = 0;                         //block[i]
    j = *maxMessageIndex;          //messageIn[j]
    k = *lastPacketIndex;        //messageIn[j].packet[k]
    if(k < 0) {k = 0;}
    if(messageIn[j].uiPacketTEXTLEN == 0) { messageIn[j].uiPacketTEXTLEN = 65530; }
    if(k >= BUFFERSIZE-1) { vClearMsg(messageIn, j); } //should change buffer size

    for (i = 0; i < receiveBlockLength; i++) {

//        printf("%X  ", block[i]);
//        printf("messageIn[%d].cksStatus:%d\n", j, messageIn[j].cksStatus);
//        printf("messageIn[%d].success:%d\n", j, messageIn[j].success);

        messageIn[j].packet[k] = block[i];

        if(k == 0) {
            if(messageIn[j].packet[k] == 0xAA) {
                messageIn[j].bGetDLE = true;
                k++;
            } else {
                if(j > 0) {
                    //special special special case!!
                    messageIn[j] = messageIn[j-1];
                    messageIn[j].cksStatus = false;
                    k = messageIn[j].uiPacketTEXTLEN;
                    messageIn[j].uiPacketTEXTLEN = 65530;
                    messageIn[j].packet[k] = block[i];
                    k++;

                    // printf("\n%X %X\n", messageIn[j].packet[k-1], messageIn[j].packet[k]);

                } else {
                  vClearMsg(messageIn, j);
                  k = 0;
                }

            }
        }
        else if(k == 1) {  //bGetDLE must be true
            if(messageIn[j].packet[k] == 0xBB) {
                messageIn[j].bGetSOH = true;
                k++;
            } else if(messageIn[j].packet[k] == 0xDD) {
                messageIn[j].bGetACK = true;
                k++;
                messageIn[j].uiPacketTEXTLEN = 5;  //set default to 77
                // messageIn[j].uiPacketTEXTLEN = 8;  //set default to 92
            } else if(messageIn[j].packet[k] == 0xEE) {
                messageIn[j].bGetNAK = true;
                k++;
                messageIn[j].uiPacketTEXTLEN = 6;  //set default to 77
                // messageIn[j].uiPacketTEXTLEN = 9;  //set default to 92
            } else {
                vClearMsg(messageIn, j);
                k = 0;
            }
        } else {  //other byte
            if(messageIn[j].packet[k-1] == 0xAA) {
                messageIn[j].bGetDLE = true;
            }
            if(messageIn[j].bGetSOH == true) {  //now in 0xAA 0xBB status
                if(messageIn[j].bGetDLE == true && messageIn[j].packet[k] == 0xCC) {
                    messageIn[j].uiPacketTEXTLEN = k+2;
                }
            }
            k++;
        }
        messageIn[j].bGetDLE = false;  //force clean every time

//        printf("messageIn[%d].uiPacketTEXTLEN:%d\n", j, messageIn[j].uiPacketTEXTLEN);

        if(k == messageIn[j].uiPacketTEXTLEN) {  //check ok or fail by checksum

            for(int n = 0; n < k-2; n++) {
                if(messageIn[j].packet[n] == 0xAA &&
                   (messageIn[j].packet[n+1] == 0xBB ||
                   messageIn[j].packet[n+1] == 0xDD ||
                   messageIn[j].packet[n+1] == 0xEE)   ) {  //find start

                        tempcks = 0;
                        for(int m = n; m < k - 1; m++) {
                           tempcks ^= messageIn[j].packet[m];
                        }
                        // printf("cks:%2X, %d-%d\n", tempcks, n, k);
                        if (messageIn[j].packet[ messageIn[j].uiPacketTEXTLEN-1 ] == tempcks) {
                            messageIn[j].cksStatus = true;
                            //OTTEST messageIn[j].success = true;  //OTTEST

                            if( n != 0) {  //move packet
                                int iMovePtr = 0;
                                for(int m = n; m < k; m++) {
                                    messageIn[j].packet[iMovePtr] = messageIn[j].packet[m];
                                    iMovePtr++;
                                }
                                messageIn[j].uiPacketTEXTLEN -= n;
                            }
                            break;
                        }
                   }
            }

            if(messageIn[j].cksStatus == false) {
                //extern
                if(messageIn[j].bGetSOH == true) {
                    messageIn[j].uiPacketTEXTLEN = 65530;
                    // messageIn[j].usiExternCounter++;
                }
                if(messageIn[j].bGetACK == true) {
                    if(messageIn[j].uiPacketTEXTLEN == 5) {
                      messageIn[j].uiPacketTEXTLEN = 8;  //set to 92
                      // messageIn[j].usiExternCounter++;
                    } else if(messageIn[j].uiPacketTEXTLEN == 8) {  //fail
                        vClearMsg(messageIn, j);
                        k = 0;
                    }
                }
                if(messageIn[j].bGetNAK == true) {
                    if(messageIn[j].uiPacketTEXTLEN == 6) {
                      messageIn[j].uiPacketTEXTLEN = 9;  //set to 92
                      // messageIn[j].usiExternCounter++;
                    } else if(messageIn[j].uiPacketTEXTLEN == 9) {  //fail
                        vClearMsg(messageIn, j);
                        k = 0;
                    }
                }
            } else {
               messageIn[j].packetLength = messageIn[j].uiPacketTEXTLEN;
               j++;

               //20110922
               if(j >= MSGINSIZE) {
                 j--;
               }

               vClearMsg(messageIn, j);
               k = 0;
            }
        }
    }

    *maxMessageIndex = j;
    *lastPacketIndex = k;


    return true;
  } catch(...) {}
}

//---------------------------------------------------------------------------
bool PARSETRAFFIC::vClearMsg(MESSAGEOK *messageIn, unsigned short int msgID)
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


//---------------------------------------------------------------------------
bool PARSETRAFFIC::DecideProtocol(int *maxMessageIndex,MESSAGEOK *messageIn,int connDevice)    //�������ʥ]��w�N�X
{
try {
    if (*maxMessageIndex >= 0) {
        for (int i=0;i<=*maxMessageIndex;i++) {                                 //�ˬd�C�@��messageIn[i]
//        printf("[OTMsg] Go DecideProtocol. C:%d S:%d \n", messageIn[i].cksStatus, messageIn[i].success);
             if (messageIn[i].cksStatus==true) {                                //�q�LCheckSum�˴�

/*Debug Code
             if ((messageIn[i].packet[0]==(const BYTE)0xAA) && (messageIn[i].packet[1]==(const BYTE)0xDD)) {
               printf("printfMsgAADD3 cksStatus:%d, success:%d, SEQ:%d\n",
               messageIn[i].cksStatus, messageIn[i].success, messageIn[i].packet[2]);
             }
*/

                 switch (connDevice) {
                         case DEVICEUNKNOW:                                     //�����D�O���F�F
                              messageIn[i].protocol=PROTOCOLUNKNOW;
                         break;     //�H��A�ԲӳB�z
                         case DEVICEMUTILPROTOCOL:                              //�i�঳�h�ب�w�q�o��port�i��
                              messageIn[i].protocol=PROTOCOLUNKNOW;
                         break;     //�H��A�ԲӳB�z

                         case DEVICECENTER77:                                   //77�~���汱����
                              messageIn[i].protocol=PROTOCOL77;
                         break;
                         case DEVICECENTER87:                                   //87�~���汱����
                              messageIn[i].protocol=PROTOCOL87;
                         break;
                         case DEVICECENTER92:                                   //92�~���汱����
                              messageIn[i].protocol=PROTOCOL92;
                         break;
                         case DEVICETESTER77:                                   //77�~���{���ⴣ���վ�
                              messageIn[i].protocol=PROTOCOL77;
                         break;
                         case DEVICETESTER87:                                   //87�~���{���ⴣ���վ�
                              messageIn[i].protocol=PROTOCOL87;
                         break;
                         case DEVICETESTER92:                                   //92�~���{���ⴣ���վ�
                              messageIn[i].protocol=PROTOCOL92;
                         break;
                         case DEVICE77TC:                                       //77�~�����x���
                              messageIn[i].protocol=PROTOCOL77;
                         break;
                         case DEVICE87TC:                                       //87�~�����x���
                              messageIn[i].protocol=PROTOCOL87;
                         break;
                         case DEVICE87CMS:                                      //87�~����T�i�ܱ��
                              messageIn[i].protocol=PROTOCOL87;
                         break;
                         case DEVICE87VD:                                       //87�~�����������
                              messageIn[i].protocol=PROTOCOL87;
                         break;
                         case DEVICE92TC:                                       //92�~�����x���
                              messageIn[i].protocol=PROTOCOL92;
                         break;
                         case DEVICE92CMS:                                      //92�~����T�i�ܱ��
                              messageIn[i].protocol=PROTOCOL92;
                         break;
                         case DEVICE92VD:                                       //92�~�����������
                              messageIn[i].protocol=PROTOCOL92;
                         break;
                         case DEVICEKEYPAD:                                     //�F�ͭ��O������L
                              messageIn[i].protocol=PROTOCOLKEYPAD;
                         break;
                         case DEVICEOWNWAY:                                     //�������O�˼�
                              messageIn[i].protocol=PROTOCOLOWNWAY;
                         break;
                         case DEVICEEOI:                                        //���Ŭ��O�˼�
                              messageIn[i].protocol=PROTOCOLEOI;
                         break;
                         case DEVICELISCMS:                                     //�x�_�õaCMS
                              messageIn[i].protocol=PROTOCOLLISCMS;
                         break;
                         /*OTCombo0713*/
                         case DEVICETRAFFICLIGHT:                               //�ص{RS232����O���O
                              messageIn[i].protocol=PROTOCOLTRAFFICLIGHT;
                         break;


                         case DEVICERTMS:                                       //RTMS
                         case DEVICESS:                                         //SMART SENSOR
                         case DEVICECENTERNTCIP:                                //NTCIP�~���汱����
                         case DEVICETESTERNTCIP:                                //NTCIP�~���{���ⴣ���վ�
                         case DEVICENTCIPTC:                                    //NTCIP���x���
                         case DEVICENTCIPCMS:                                   //NTCIP��T�i�ܱ��
                         case DEVICENTCIPVD:                                    //NTCIP���������
                              messageIn[i].protocol=PROTOCOLUNKNOW;
                         break;                                                 //�o�O���i��X�{�bPARSETRAFFIC�̭��� �u��AA BB,AA DD,AA EE

                         case DEVICEREDCOUNTVER94:
                              messageIn[i].protocol = PROTOCOLREDCOUNTVER94;    //�s�����O��ƾ�(����)
                         break;

                         case DEVICEREDCOUNTVERHK:
                              messageIn[i].protocol = PROTOCOLREDCOUNTVERHK;    //�ª����O��ƾ�(����)
                         break;

                         case DEVICEREDCOUNTVERCCT97:
                              messageIn[i].protocol = PROTOCOLREDCOUNTVERCCT97;
                         break;


                         case DEVICECCJDYNCTL:
                              messageIn[i].protocol=PROTOCOLCCJ;
                         break;

                         case DEVICEREVMANUALPAD:
                              messageIn[i].protocol = PROTOCOLREVMANUALPAD;
                         break;

                         case DEVICEREVSYNC:
                              messageIn[i].protocol = PROTOCOLREVSYNC;
                         break;

                         case DEVICEAMEGIDS:
                              messageIn[i].protocol = PROTOCOLAMEGIDS;
                         break;




                         default:
                         break;
                 }

             }

        }
    }

    return true;

  } catch (...) {}
}

//---------------------------------------------------------------------------
bool PARSETRAFFIC::CheckReasonable(int *maxMessageIndex,MESSAGEOK *messageIn)   //�ˬd�ʥ]�X�z��
{
try {
    if (*maxMessageIndex >= 0) {
        for (int i=0;i<=*maxMessageIndex;i++) {                                 //�ˬd�C�@��messageIn[i]
             if (messageIn[i].cksStatus==true) {                                //�q�LCheckSum�˴�

/*Debug Code
             if ((messageIn[i].packet[0]==(const BYTE)0xAA) && (messageIn[i].packet[1]==(const BYTE)0xDD)) {
               printf("printfMsgAADD4 cksStatus:%d, success:%d, SEQ:%d\n",
               messageIn[i].cksStatus, messageIn[i].success, messageIn[i].packet[2]);
             }
*/

                 switch (messageIn[i].protocol) {
                         case PROTOCOLUNKNOW :                                  //���M������w
                              messageIn[i].success=false;
                         break;
                         case PROTOCOL77:                                       //��q��77�~���q�T��w
                              if (messageIn[i].packet[0]==(const BYTE)0xAA) {
                                  if (messageIn[i].packet[1]==(const BYTE)0xBB) {
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else if (messageIn[i].packet[1]==(const BYTE)0xDD) {
                                      if (messageIn[i].packetLength==5) messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else if (messageIn[i].packet[1]==(const BYTE)0xEE) {
                                      if (messageIn[i].packetLength==6) messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else messageIn[i].success=false;
                              } else messageIn[i].success=false;
                         break;
                         case PROTOCOL87:                                       //��q��87�~���q�T��w
                              messageIn[i].success=true;
                         break;  //�H��A�ԲӳB�z
                         case PROTOCOL92:                                       //��q��92�~���q�T��w
                              if (messageIn[i].packet[0]==(const BYTE)0xAA) {
                                  if (messageIn[i].packet[1]==(const BYTE)0xBB) {
                                      if (messageIn[i].packetLength>=12 && messageIn[i].packetLength==((int)messageIn[i].packet[5]*256+(int)messageIn[i].packet[6]))
                                          messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else if (messageIn[i].packet[1]==(const BYTE)0xDD) {
                                      if (messageIn[i].packetLength==8) messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else if (messageIn[i].packet[1]==(const BYTE)0xEE) {
                                      if (messageIn[i].packetLength==9) messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else messageIn[i].success=false;
                              } else messageIn[i].success=false;
                         break;
                         case PROTOCOLKEYPAD:                                   //�F�ͭ��O������L�q�T��w
                         case PROTOCOLTRAFFICLIGHT:
                              if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                              else messageIn[i].success=false;

                         break;
                         case PROTOCOLOWNWAY:                                   //�������O�˼Ƴq�T��w
                              messageIn[i].success=false;
                         break;  //�������O�˼�:���|�Ө�o��,�]���O��V��(����ܬݪO)

                         case PROTOCOLREDCOUNTVER94:
                              if (messageIn[i].packet[4] == 0x82 ||
                                  messageIn[i].packet[4] == 0x83 ||
                                  messageIn[i].packet[4] == 0x84    )
                                messageIn[i].success = true;
                              else
                                messageIn[i].success = false;
                         break;

                         case PROTOCOLREDCOUNTVERCCT97:                                       //��q��92�~���q�T��w
                              if (messageIn[i].packet[0]==(const BYTE)0xAA) {
                                  if (messageIn[i].packet[1]==(const BYTE)0xBB) {
                                      if (messageIn[i].packetLength>=12 && messageIn[i].packetLength==((int)messageIn[i].packet[5]*256+(int)messageIn[i].packet[6]))
                                          messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else if (messageIn[i].packet[1]==(const BYTE)0xDD) {
                                      if (messageIn[i].packetLength==8) messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else if (messageIn[i].packet[1]==(const BYTE)0xEE) {
                                      if (messageIn[i].packetLength==9) messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else messageIn[i].success=false;
                              } else messageIn[i].success=false;
                         break;


                         case PROTOCOLEOI:                                      //���Ŭ��O�˼Ƴq�T��w
                              messageIn[i].success=true;
                         break;  //�H��A�ԲӳB�z

                         case PROTOCOLLISCMS:                                   //�x�_�õaCMS�q�T��w
                              messageIn[i].success=true;
                         break;  //�H��A�ԲӳB�z

                         case PROTOCOLNTCIP:                                    //��q��NTCIP�q�T��w
                         case PROTOCOLRTMS:                                     //RTMS�q�T��w
                         case PROTOCOLSS:                                       //SMART SENSOR�q�T��w
                              messageIn[i].success=false;
                         break;                                                 //������w���|�b�o�̳B�z

                         case PROTOCOLCCJ:
                              messageIn[i].success=true;
                         break;

                         case PROTOCOLREVMANUALPAD:
                         case PROTOCOLREVSYNC:
                         case PROTOCOLAMEGIDS:
                              messageIn[i].success = true;
                         break;

                         default:
                         break;
                 }
             }
        }
    }

    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
bool PARSETRAFFIC::AssignLcn(int *maxMessageIndex,MESSAGEOK *messageIn)         //�ھګʥ]�p��LCN
{
try {
    if (*maxMessageIndex >= 0) {
        for (int i=0;i<=*maxMessageIndex;i++) {                                 //�ˬd�C�@��messageIn[i]
//        printf("[OTMsg] Go AssignLcn. C:%d S:%d \n", messageIn[i].cksStatus, messageIn[i].success);
             if (messageIn[i].cksStatus==true) {                                //�q�LCheckSum�˴�
                 if (messageIn[i].success==true) {                              //�q�L�ˬd�ʥ]�X�z��

                     switch (messageIn[i].protocol) {
                             case PROTOCOL77:                                   //��q��77�~���q�T��w
                             case PROTOCOL87:                                   //��q��87�~���q�T��w
                                  if (smem.vGetAdjudicateReturnAddressBCDorHEX()==cBCD) {  //BCD code�s�X
                                      bcdSwitchHi.bcdCode=messageIn[i].packet[2];
                                      bcdSwitchLo.bcdCode=messageIn[i].packet[3];
                                      messageIn[i].packetLcn=bcdSwitchHi.bcdHiLo.b2*1000+bcdSwitchHi.bcdHiLo.b1*100+bcdSwitchLo.bcdHiLo.b2*10+bcdSwitchLo.bcdHiLo.b1;
                                  } else {                                                 //16�i��s�X
                                      messageIn[i].packetLcn=(int)messageIn[i].packet[2]*256+(int)messageIn[i].packet[3];
                                  }
                             break;
                             case PROTOCOL92:                                   //��q��92�~���q�T��w
                             case PROTOCOLCCJ:
                             case PROTOCOLREVMANUALPAD:
                             case PROTOCOLREVSYNC:
                             case PROTOCOLAMEGIDS:
                                  if (smem.vGetAdjudicateReturnAddressBCDorHEX()==cBCD) {  //BCD code�s�X
                                      bcdSwitchHi.bcdCode=messageIn[i].packet[3];
                                      bcdSwitchLo.bcdCode=messageIn[i].packet[4];
                                      messageIn[i].packetLcn=bcdSwitchHi.bcdHiLo.b2*1000+bcdSwitchHi.bcdHiLo.b1*100+bcdSwitchLo.bcdHiLo.b2*10+bcdSwitchLo.bcdHiLo.b1;
                                  } else {                                                 //16�i��s�X
                                      messageIn[i].packetLcn=(int)messageIn[i].packet[3]*256+(int)messageIn[i].packet[4];
                                  }
                             break;
                             case PROTOCOLUNKNOW:                               //���M������w
                             case PROTOCOLNTCIP:                                //��q��NTCIP�q�T��w
                             case PROTOCOLKEYPAD:                               //�F�ͭ��O������L�q�T��w
                             case PROTOCOLRTMS:                                 //RTMS�q�T��w
                             case PROTOCOLSS:                                   //SMART SENSOR�q�T��w
                             case PROTOCOLOWNWAY:                               //�������O�˼Ƴq�T��w
                             case PROTOCOLEOI:                                  //���Ŭ��O�˼Ƴq�T��w
                             case PROTOCOLLISCMS:                               //�x�_�õaCMS�q�T��w
                                  messageIn[i].packetLcn=0;
                             break;     //LCD�b���ä��A��
                             default:
                             break;
                     }
                 }
             }
        }
    }

    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
bool PARSETRAFFIC::EchoToGUI(int *maxMessageIndex,MESSAGEOK *messageIn,char *deviceName)      //�@����w�۹諸�Ʊ�
{
try {
    if (*maxMessageIndex>=0) {                                                  //�T�O
        for (int i=0;i<=*maxMessageIndex;i++) {
             if (messageIn[i].cksStatus==true) {                                //�q�LCheckSum�˴�
                 if (messageIn[i].success==true) {                              //�q�L�ˬd�ʥ]�t�X�z��

                 //OTADD for packet display to screen
                   screenCurrentCommPacket.vRefreshCurrentScreenPacket(messageIn[i].packet, messageIn[i].packetLength, "0");

                     char tempBuff[256],buff[2048]="";
                     char portName[200]="[RECEIVE] ";
                     char temp[6]=" --";

                     strcat(portName,deviceName);
                     strcat(portName,temp);

                     strcat(buff,portName);

                     for (int j=0;j<messageIn[i].packetLength;j++) {
                          sprintf(tempBuff,"%3X",messageIn[i].packet[j]);
                          strcat(buff,tempBuff);
                     }


                     printf("%s\n",buff);

                 }
             }
        }  //end for (int i=0 ; i < maxMessageIndex ; i++)
    }  //end if (maxMessageIndex >= 0)

    return true;

  } catch(...) {}
}

//---------------------------------------------------------------------------
bool PARSETRAFFIC::MoveLastData(int *maxMessageIndex,int *lastPacketIndex,MESSAGEOK *messageIn)
{
try {

    if(messageIn[*maxMessageIndex].cksStatus == true) {
        for(int j = 0; j <= *maxMessageIndex; j++) {
//            printf("messageIn[%d].cks:%d\n", *maxMessageIndex, messageIn[*maxMessageIndex].cksStatus);
            vClearMsg(messageIn, j);
        }
        *maxMessageIndex = 0;
        *lastPacketIndex = 0;
    } else if(*maxMessageIndex > 0) {

        messageIn[0] = messageIn[*maxMessageIndex];

        for(int j = 1; j <= *maxMessageIndex; j++) {
            vClearMsg(messageIn, j);
        }
        *maxMessageIndex = 0;
//        printf("messageIn[%d].cks:%d\n", 0, messageIn[0].cksStatus);
    }

    return true;

  } catch(...) {}
}


//---------------------------------------------------------------------------

/*
//---------------------------------------------------------------------------
//bool PARSETRAFFIC::decide77Or87(int messageInIdx,BYTE controlCode,int length)
bool PARSETRAFFIC::decide77Or87(MESSAGEOK *messageIn)
{
try {
    switch(messageIn.packet[4]) {      //77��87�n�ѱ���X�P�_
         //���x����e�^����
         case 0xE7:          //77�~�����x���:�{��B���P�Ѿl��������
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=12) messageIn.success=true;
         break;
         case 0xE2:          //77�~�����x���:�{��ާ@�Ҧ�����
         case 0xD2:          //32�ɬq����ϥ�
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0xEC:          //77�~�����x���:����B�@���A����
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x0E:          //77�~�����x���:for ����
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x0F:          //77�~�����x���:
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x0C:          //77�~�����x���:Plan2�ǰe���\
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0xEF:          //77�~�����x���:�S����ɬq���A����
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=27) messageIn.success=true;
         break;
         case 0xEB:          //77�~�����x���:�̪�@���c���}�Ҥ������ɶ�
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=15) messageIn.success=true;
         break;
         case 0xE3:          //77�~�����x���:�{��ɬq�_�l�ɶ�����
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=12) messageIn.success=true;
         break;
         case 0xE9:          //77�~�����x���:�ɬq����
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=10) messageIn.success=true;
         break;
         case 0xEA:          //77�~�����x���:�ɬq���ɨ�p������
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=10) messageIn.success=true;
         break;
         case 0xE0:          //77�~�����x���:�Y�ɮɶ�����
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=10) messageIn.success=true;
         break;
         case 0xE1:          //77�~�����x���:����������
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=11) messageIn.success=true;
         break;
         case 0xE4:          //77�~�����x���:�Ĥ@���T������
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=14) messageIn.success=true;
         break;
         case 0xE5:          //77�~�����x���:�ĤG���T������
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=14) messageIn.success=true;
         break;
         case 0xE8:          //77�~�����x���:�g����ɬq���A����
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=12) messageIn.success=true;
         break;

         //���߰e�ܸ��x���


         //��T�i�ܨt�α���e�^����
         case 0x00:          //87�~����T�i�ܨt�α��:�ǰe���\
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=9) messageIn.success=true;
         break;
         case 0x01:          //87�~����T�i�ܨt�α��:�ǰe����
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=9) messageIn.success=true;
         break;
         case 0x15:          //87�~����T�i�ܨt�α��:�w�魫�m�^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x19:          //87�~����T�i�ܨt�α��:�n�魫�m�^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x13:          //87�~����T�i�ܨt�α��:��ɻ~�t�L�j�n�D���߹��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=9) messageIn.success=true;
         break;
         case 0x14:          //87�~����T�i�ܨt�α��:�{���]�Ƥ���ɶ��^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=13) messageIn.success=true;
         break;
         case 0x2F:          //87�~����T�i�ܨt�α��:�{���]�ƶǿ�g���^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x1D:          //87�~����T�i�ܨt�α��:�{���]�Ƶw�骬�A�^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x1E:          //87�~����T�i�ܨt�α��:�q�����_�^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=11) messageIn.success=true;
         break;
         case 0x25:          //87�~����T�i�ܨt�α��:�����������^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=12) messageIn.success=true;
         break;
         case 0x26:          //87�~����T�i�ܨt�α��:�������޲z�^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=12) messageIn.success=true;
         break;
         case 0x2A:          //87�~����T�i�ܨt�α��:����}�l�P�����^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x2E:          //87�~����T�i�ܨt�α��:�j������
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x2D:          //87�~����T�i�ܨt�α��:�n�D���ߤU�Ǳ�����
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x76:          //87�~����T�i�ܨt�α��:����T���^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=38) messageIn.success=true;
         break;
         case 0x86:          //87�~����T�i�ܨt�α��:�����m�^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=22) messageIn.success=true;
         break;
         case 0xA8:          //87�~����T�i�ܨt�α��:����ɶ��^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=20) messageIn.success=true;
         break;
         case 0x52:          //87�~����T�i�ܨt�α��:��ܬ��ƻP���Z�^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=10) messageIn.success=true;
         break;
         case 0x7F:          //87�~����T�i�ܨt�α��:���I���յ��G�^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=16) messageIn.success=true;
         break;

         //77,87����
         case 0x10:
              if (messageIn.packetLength==12) {
                     messageIn.protocol = PROTOCOL77;
                     messageIn.success=true;
              } else {
                  messageIn.protocol = PROTOCOL87;
                  if (messageIn.packetLength>=6) messageIn.success=true;
              }
         break;
         case 0x29:          //87�~����T�i�ܨt�α��:�{���]�ƽs���^��
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;

         case 0x11:
              if (messageIn.packetLength==11) {
                     messageIn.protocol = PROTOCOL77;
                     messageIn.success=true;
              } else {
                  messageIn.protocol = PROTOCOL87;
                  if (messageIn.packetLength>=6) messageIn.success=true;
              }
         break;
         case 0x21:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x22:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x24:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x27:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x28:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x2C:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;



         //���߰e�ܸ�T�i�ܨt�α��
         case 0x12:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x2B:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x16:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x17:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x18:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x1C:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x1F:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;

         case 0x74:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x77:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x75:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x85:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x79:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x7C:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x7E:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0xA5:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0xA7:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0xA6:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x50:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x51:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x55:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x56:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x59:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;

         default: //�D�n���F���߰e��77�~�����f�������w
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=7) messageIn.success=true;
         break;
    } //end switch
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PARSETRAFFIC::decideProtocol92Normal(MESSAGEOK *messageIn)
{
try {
    //�@�ΰT�� 0F+controlCode
    switch (messageIn.packet[8]) {
        case 0x80:       //�^�����O����       �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=13) messageIn.success=true;
        break;
        case 0x81:       //�^�����O�L��       �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=15) messageIn.success=true;
        break;
        case 0x8F:       //�{���]�ƥN�ǫ��O�W��    �׺ݱ��->�ϰ챱�->�����
             if (messageIn.packetLength>=16) messageIn.success=true;
        break;
        case 0x8E:       //�U���N�ǫ��O�ܸ��f�]��  �����->�ϰ챱�->�׺ݱ��
             if (messageIn.packetLength>=16) messageIn.success=true;
        break;
        case 0x10:       //���]�w�{���]��(�w�魫�m)   �����->�׺ݱ��
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x90:       //�^���{���]�ƭ��]�w �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x40:       //�d�߲{���]�ƽs��   �����->�׺ݱ��
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0xC0:       //�^���{���]�ƽs��   �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=14) messageIn.success=true;
        break;
        case 0x00:       //�^���e���q�����_(�]�ƭ��s�Ұ�)  �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=15) messageIn.success=true;
        break;
        case 0x11:       //���]�{���]�Ƥ��q�T(�n�魫�m)    �����->�׺ݱ��
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0x91:       //�^���q�T���]�w    �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0x41:       //�d�߲{���]�ƪ��A     �����->�׺ݱ��
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0xC1:       //�^���d�߲{���]�ƪ��A     �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x12:       //�]�w�{���]�Ƥ�����ɶ�    �����->�׺ݱ��
             if (messageIn.packetLength>=18) messageIn.success=true;
        break;
        case 0x92:       //��ɻ~�t�^���έn�D���߹��   �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x42:       //�d�߲{���]�Ƥ�����ɶ�    �����->�׺ݱ��
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0xC2:       //�^���{���]�Ƥ�����ɶ�    �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=18) messageIn.success=true;
        break;
        case 0x02:       //�^���׺ݳ]�Ʋ{����ʧ��ɶ�  �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0x13:       //�]�w�{���]�ƫ��O����    �����->�׺ݱ��
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x43:       //�d�߰b��N����������Ϋ��O����   �����->�׺ݱ��
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0xC3:       //�^���b��N����������Ϋ��O����   �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=17) messageIn.success=true;
        break;
        case 0x14:       //�]�w�{���]�Ʀ^���w�骬�A���g��   �����->�׺ݱ��
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x44:       //�d�߲{���]�Ʀ^�����p���ǿ�g��   �����->�׺ݱ��
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0xC4:       //�^���{���]�Ʀ^�����p���ǿ�g��   �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x04:       //�{���]�Ʀ^�����A    �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x15:       //�]�w�{���]�ƾާ@�K�X     �����->�׺ݱ��
             if (messageIn.packetLength>=17) messageIn.success=true;
        break;
        case 0x45:       //�d�߲{���]�ƾާ@��w�K�X    �����->�׺ݱ��
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0xC5:       //�^�в{���]�ƾާ@�K�X    �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=17) messageIn.success=true;
        break;
        case 0x16:       //�]�w,�Ѱ��{����Ʈw�ާ@��w   �����->�׺ݱ��
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x46:       //�d�߲{����Ʈw��w   �����->�׺ݱ��
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0xC6:       //�^���{����Ʈw��w   �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x47:       //�d�߳q�T��w�^�����O   �����->�׺ݱ��
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0xC7:       //�^���q�T��w�^�����O   �׺ݱ��->�ϰ챱�(�汱����)
             if (messageIn.packetLength>=13) messageIn.success=true;
        break;
        default:
        break;
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PARSETRAFFIC::decideProtocol92Tc(MESSAGEOK *messageIn)
{
try {
    //���x����T�� 0x5F+controlCode
    messageIn.success=true;
    switch (messageIn.packet[8]) {
        case 0x10:       //�]�w�{�汱��������e
        break;
        case 0x40:       //�d�߲{�汱������]�w���e
        break;
        case 0xC0:       //�^���{�汱��������e
        break;
        case 0x00:       //�۰ʦ^����������{����椺�e
        break;
        case 0x11:       //�]�w�@��騮�D�ռ�����Ѽ�
        break;
        case 0x41:       //�d�ߤ@��騮�D�ռ�����ѼƤ��]�w���e
        break;
        case 0xC1:       //�^���@��騮�D�ռ�����ѼƤ��]�w���e
        break;
        case 0x01:       //�۰ʦ^���@�먮�D�ռ�����_��
        break;
        case 0x12:       //�]�w�S���騮�D�ռ����Ѽ�
        break;
        case 0x42:       //�d�߯S���騮�D�ռ�����ѼƤ��]�w���e
        break;
        case 0xC2:       //�^���S���騮�D�ռ�����ѼƤ��]�w���e
        break;
        case 0x02:       //�۰ʦ^���S���騮�D�ռ�����_��
        break;
        case 0x13:       //�]�w���x����ɬ۱ƦC
        break;
        case 0x43:       //�d�߸��x����ɬ۱ƦC�����
        break;
        case 0xC3:       //�^�����x����ɬ۱ƦC�����
        break;
        case 0x03:       //�D�ʦ^�����x����B���ഫ�����
        break;
        case 0x14:       //�]�w���f�ɨ�p�����򥻰Ѽ�
        break;
        case 0x44:       //�d�߸��f�ɨ�p�����򥻰Ѽ�
        break;
        case 0xC4:       //�^�����f�ɨ�p�e���򥻰Ѽ�
        break;
        case 0x15:       //�]�w�ɨ�p�����e
        break;
        case 0x45:       //�d�߮ɨ�p�����]�w���e
        break;
        case 0xC5:       //�^���ɨ�p�����]�w���e
        break;
        case 0x16:       //�]�w�@���ɬq���A
        break;
        case 0x46:       //�d�ߤ@���ɬq���A���]�w���e
        break;
        case 0xC6:       //�^���@���ɬq���A���]�w���e
        break;
        case 0x17:       //�]�w�S����ɬq���A
        break;
        case 0x47:       //�d�߯S����ɬq���A���]�w���e
        break;
        case 0xC7:       //�^���S����ɬq���A���]�w���e
        break;
        case 0x18:       //��ܰ��椧�ɨ�p��
        break;
        case 0x48:       //�d�ߥثe�ɨ�p�����e
        break;
        case 0xC8:       //�^���ثe�ɨ�p�����e
        break;
        case 0x19:       //�]�wĲ�ʱ���պA
        break;
        case 0x49:       //�d��Ĳ�ʱ���պA���e
        break;
        case 0xC9:       //�^��Ĳ�ʱ���պA���e
        break;
        case 0x09:       //Ĳ�ʫ�^��Ĳ�ʱ���i��
        break;
        case 0x1A:       //�]�w�@���Ĳ�ʮɬq���A
        break;
        case 0x4A:       //�d�ߤ@���Ĳ�ʫ��A���]�w���e
        break;
        case 0xCA:       //�^���@���Ĳ�ʮɬq���A���]�w���e
        break;
        case 0x1B:       //�]�w�S�����Ĳ�ʮɬq��A
        break;
        case 0x4B:       //�d�߯S�����Ĳ�ʮɬq���A���]�w���e
        break;
        case 0xCB:       //�^���S����Ĳ�ʮɬq���A���]�w���e
        break;
        case 0x1C:       //�ɬ۩ΨB���ܴ�����
        break;
        case 0x4C:       //�d�߸��x����{��ɬۤΨB��
        break;
        case 0xCC:       //�^�����x����{��ɬۤΨB���P�������
        break;
        case 0x0C:       //�D�ʦ^�����x����{��ɬۤΨB��
        break;
        case 0x1D:       //�{�ɸ��f����
        break;
        case 0x1E:       //�]�w�S�Ը��u����Ѽ�
        break;
        case 0x4E:       //�d�߯S�Ը��u����ѼƤ��]�w���e
        break;
        case 0xCE:       //�^���S�Ը��u����ѼƤ��]�w���e
        break;
        case 0x2F:       //�]�w�����ɬ۱ƦC
        break;
        case 0x5F:       //�d�߸����ɬ۱ƦC�����
        break;
        case 0xDF:       //�^�������ɬ۱ƦC�����
        break;
        case 0x31:       //�]�w�����s���X�պA
        break;
        case 0x61:       //�d�ߥ����s���X�պA
        break;
        case 0xE1:       //�^�������s���X�պA
        break;
        case 0x32:       //�]�w�l���s��]�w�պA
        break;
        case 0x62:       //�d�ߤl���s��]�w�պA
        break;
        case 0xE2:       //�^���l���s��]�w�պA
        break;
        case 0x33:       //�]�w�l������պA
        break;
        case 0x63:       //�d�ߤl������պA���]�w���e
        break;
        case 0xE3:       //�^���l������պA���]�w���e
        break;
        case 0x08:       //�^�����x����{���ާ@
        break;
        case 0x0A:       //�^���{����Ƨ��
        break;
        case 0x0B:       //�n�D����ߤU�Ǹ�Ʈw���
        break;
        case 0x3F:       //�]�w��Ʀ^���ǿ�g��
        break;
        case 0x6F:       //�d�߲{���]�ƥ�q�ѼƤ��ǿ�g��
        break;
        case 0xEF:       //�d�߫�^���{���O�A�ΨB���ഫ��Ƥ��^���ǿ�g��
        break;
        case 0x0F:       //���жǿ���f�����O�A���A
        break;
        case 0x3E:       //�]�w�{���]�ƴ������A
        break;
        case 0x6E:       //�d�߲{���]�ƴ���]�w
        break;
        case 0xEE:       //�^���{���]�ƴ������A
        break;
        case 0x0E:       //�^���Ƶ{�������_��
        break;
        default:
        break;
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PARSETRAFFIC::decideProtocol92Vd(MESSAGEOK *messageIn)
{
try {
    //�����������T�� 0x6F+controlCode
    switch (messageIn.packet[8]) {
        default:
            messageIn.success=true;
        break;
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PARSETRAFFIC::decideProtocol92Cms(MESSAGEOK *messageIn)
{
try {
    //��T�i�ܼлx�T�� 0xAF+controlCode
    switch (messageIn.packet[8]) {
        default:
            messageIn.success=true;
        break;
    }
  } catch(...) {}
}
*/

//---------------------------------------------------------------------------
bool PARSETRAFFIC::vJudgeProtocolAndCheckLength(int *maxMessageIndex,MESSAGEOK *messageIn)
{
try {
//0:�L�s�� 1:�汱���� 2:��L�}����J�����d 3:RTMS 4:�N�Ǹ��f77�~���x��� 5:�N�Ǹ��f92�~���x��� 6.�N�Ǹ��f87�~��T�i�ܱ�� 7.�N�Ǹ��f92�~��T�i�ܱ�� 8.�������O�˼� 9.���Ŭ��O�˼�
      //�H�W�з�77,87,92�q�T��w,�Ҭ�AA BB.....AA CC CKS �� AA DD..... �� AA EE.....


      if (*maxMessageIndex >= 0) {
          for (int i=0;i<=*maxMessageIndex;i++) {                                 //�ˬd�C�@��messageIn[i]
               if (messageIn[i].cksStatus==true) {                                //�q�LCheckSum�˴�

/*Debug Code
               if ((messageIn[i].packet[0]==(const BYTE)0xAA) && (messageIn[i].packet[1]==(const BYTE)0xDD)) {
                 printf("printfMsgAADD2 cksStatus:%d, success:%d, SEQ:%d\n",
                 messageIn[i].cksStatus, messageIn[i].success, messageIn[i].packet[2]);
               }
*/


            if (messageIn[i].packet[0]==(const BYTE)0xAA) {
                if (messageIn[i].packet[1]==(const BYTE)0xBB) {

                    if (messageIn[i].packetLength>=12) {                      //���ץ����n>12�~���i�ର92�~������ aa bb 0 hiLcn loLcn hiLen loLen eqpCode cmdCode aa cc cks
                        if (messageIn[i].packetLength==((int)messageIn[i].packet[5]*256+(int)messageIn[i].packet[6])) {    //92�~���q�T��w
                            messageIn[i].protocol = PROTOCOL92;                          //92�~���q�T��w

                            //OT Debug 0706 fuck change
                            messageIn[i].success=true;

                            if (messageIn[i].packet[7]==0x0F) {               //92�~���@�ΰT��
//                                decideProtocol92Normal(messageIn[i]);
//================================================================
                                //�@�ΰT�� 0F+controlCode
                                switch (messageIn[i].packet[8]) {
                                    case 0x80:       //�^�����O����       �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=13) messageIn[i].success=true;
                                    break;
                                    case 0x81:       //�^�����O�L��       �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=15) messageIn[i].success=true;
                                    break;
                                    case 0x8F:       //�{���]�ƥN�ǫ��O�W��    �׺ݱ��->�ϰ챱�->�����
                                         if (messageIn[i].packetLength>=16) messageIn[i].success=true;
                                    break;
                                    case 0x8E:       //�U���N�ǫ��O�ܸ��f�]��  �����->�ϰ챱�->�׺ݱ��
                                         if (messageIn[i].packetLength>=16) messageIn[i].success=true;
                                    break;
                                    case 0x10:       //���]�w�{���]��(�w�魫�m)   �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x90:       //�^���{���]�ƭ��]�w �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x40:       //�d�߲{���]�ƽs��   �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0xC0:       //�^���{���]�ƽs��   �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=14) messageIn[i].success=true;
                                    break;
                                    case 0x00:       //�^���e���q�����_(�]�ƭ��s�Ұ�)  �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=15) messageIn[i].success=true;
                                    break;
                                    case 0x11:       //���]�{���]�Ƥ��q�T(�n�魫�m)    �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0x91:       //�^���q�T���]�w    �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0x41:       //�d�߲{���]�ƪ��A     �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0xC1:       //�^���d�߲{���]�ƪ��A     �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x12:       //�]�w�{���]�Ƥ�����ɶ�    �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=18) messageIn[i].success=true;
                                    break;
                                    case 0x92:       //��ɻ~�t�^���έn�D���߹��   �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x42:       //�d�߲{���]�Ƥ�����ɶ�    �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0xC2:       //�^���{���]�Ƥ�����ɶ�    �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=18) messageIn[i].success=true;
                                    break;
                                    case 0x02:       //�^���׺ݳ]�Ʋ{����ʧ��ɶ�  �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0x13:       //�]�w�{���]�ƫ��O����    �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x43:       //�d�߰b��N����������Ϋ��O����   �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0xC3:       //�^���b��N����������Ϋ��O����   �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=17) messageIn[i].success=true;
                                    break;
                                    case 0x14:       //�]�w�{���]�Ʀ^���w�骬�A���g��   �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x44:       //�d�߲{���]�Ʀ^�����p���ǿ�g��   �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0xC4:       //�^���{���]�Ʀ^�����p���ǿ�g��   �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x04:       //�{���]�Ʀ^�����A    �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x15:       //�]�w�{���]�ƾާ@�K�X     �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=17) messageIn[i].success=true;
                                    break;
                                    case 0x45:       //�d�߲{���]�ƾާ@��w�K�X    �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0xC5:       //�^�в{���]�ƾާ@�K�X    �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=17) messageIn[i].success=true;
                                    break;
                                    case 0x16:       //�]�w,�Ѱ��{����Ʈw�ާ@��w   �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x46:       //�d�߲{����Ʈw��w   �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0xC6:       //�^���{����Ʈw��w   �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x47:       //�d�߳q�T��w�^�����O   �����->�׺ݱ��
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0xC7:       //�^���q�T��w�^�����O   �׺ݱ��->�ϰ챱�(�汱����)
                                         if (messageIn[i].packetLength>=13) messageIn[i].success=true;
                                    break;
                                    default:
                                    break;
                                }
//================================================================

                            } else if (messageIn[i].packet[7]==0x5F) {        //92�~�����x����T��
//                                decideProtocol92Tc(messageIn[i]);
//================================================================
                                //���x����T�� 0x5F+controlCode
                                messageIn[i].success=true;
                                switch (messageIn[i].packet[8]) {
                                    case 0x10:       //�]�w�{�汱��������e
                                    break;
                                    case 0x40:       //�d�߲{�汱������]�w���e
                                    break;
                                    case 0xC0:       //�^���{�汱��������e
                                    break;
                                    case 0x00:       //�۰ʦ^����������{����椺�e
                                    break;
                                    case 0x11:       //�]�w�@��騮�D�ռ�����Ѽ�
                                    break;
                                    case 0x41:       //�d�ߤ@��騮�D�ռ�����ѼƤ��]�w���e
                                    break;
                                    case 0xC1:       //�^���@��騮�D�ռ�����ѼƤ��]�w���e
                                    break;
                                    case 0x01:       //�۰ʦ^���@�먮�D�ռ�����_��
                                    break;
                                    case 0x12:       //�]�w�S���騮�D�ռ����Ѽ�
                                    break;
                                    case 0x42:       //�d�߯S���騮�D�ռ�����ѼƤ��]�w���e
                                    break;
                                    case 0xC2:       //�^���S���騮�D�ռ�����ѼƤ��]�w���e
                                    break;
                                    case 0x02:       //�۰ʦ^���S���騮�D�ռ�����_��
                                    break;
                                    case 0x13:       //�]�w���x����ɬ۱ƦC
                                    break;
                                    case 0x43:       //�d�߸��x����ɬ۱ƦC�����
                                    break;
                                    case 0xC3:       //�^�����x����ɬ۱ƦC�����
                                    break;
                                    case 0x03:       //�D�ʦ^�����x����B���ഫ�����
                                    break;
                                    case 0x14:       //�]�w���f�ɨ�p�����򥻰Ѽ�
                                    break;
                                    case 0x44:       //�d�߸��f�ɨ�p�����򥻰Ѽ�
                                    break;
                                    case 0xC4:       //�^�����f�ɨ�p�e���򥻰Ѽ�
                                    break;
                                    case 0x15:       //�]�w�ɨ�p�����e
                                    break;
                                    case 0x45:       //�d�߮ɨ�p�����]�w���e
                                    break;
                                    case 0xC5:       //�^���ɨ�p�����]�w���e
                                    break;
                                    case 0x16:       //�]�w�@���ɬq���A
                                    break;
                                    case 0x46:       //�d�ߤ@���ɬq���A���]�w���e
                                    break;
                                    case 0xC6:       //�^���@���ɬq���A���]�w���e
                                    break;
                                    case 0x17:       //�]�w�S����ɬq���A
                                    break;
                                    case 0x47:       //�d�߯S����ɬq���A���]�w���e
                                    break;
                                    case 0xC7:       //�^���S����ɬq���A���]�w���e
                                    break;
                                    case 0x18:       //��ܰ��椧�ɨ�p��
                                    break;
                                    case 0x48:       //�d�ߥثe�ɨ�p�����e
                                    break;
                                    case 0xC8:       //�^���ثe�ɨ�p�����e
                                    break;
                                    case 0x19:       //�]�wĲ�ʱ���պA
                                    break;
                                    case 0x49:       //�d��Ĳ�ʱ���պA���e
                                    break;
                                    case 0xC9:       //�^��Ĳ�ʱ���պA���e
                                    break;
                                    case 0x09:       //Ĳ�ʫ�^��Ĳ�ʱ���i��
                                    break;
                                    case 0x1A:       //�]�w�@���Ĳ�ʮɬq���A
                                    break;
                                    case 0x4A:       //�d�ߤ@���Ĳ�ʫ��A���]�w���e
                                    break;
                                    case 0xCA:       //�^���@���Ĳ�ʮɬq���A���]�w���e
                                    break;
                                    case 0x1B:       //�]�w�S�����Ĳ�ʮɬq��A
                                    break;
                                    case 0x4B:       //�d�߯S�����Ĳ�ʮɬq���A���]�w���e
                                    break;
                                    case 0xCB:       //�^���S����Ĳ�ʮɬq���A���]�w���e
                                    break;
                                    case 0x1C:       //�ɬ۩ΨB���ܴ�����
                                    break;
                                    case 0x4C:       //�d�߸��x����{��ɬۤΨB��
                                    break;
                                    case 0xCC:       //�^�����x����{��ɬۤΨB���P�������
                                    break;
                                    case 0x0C:       //�D�ʦ^�����x����{��ɬۤΨB��
                                    break;
                                    case 0x1D:       //�{�ɸ��f����
                                    break;
                                    case 0x1E:       //�]�w�S�Ը��u����Ѽ�
                                    break;
                                    case 0x4E:       //�d�߯S�Ը��u����ѼƤ��]�w���e
                                    break;
                                    case 0xCE:       //�^���S�Ը��u����ѼƤ��]�w���e
                                    break;
                                    case 0x2F:       //�]�w�����ɬ۱ƦC
                                    break;
                                    case 0x5F:       //�d�߸����ɬ۱ƦC�����
                                    break;
                                    case 0xDF:       //�^�������ɬ۱ƦC�����
                                    break;
                                    case 0x31:       //�]�w�����s���X�պA
                                    break;
                                    case 0x61:       //�d�ߥ����s���X�պA
                                    break;
                                    case 0xE1:       //�^�������s���X�պA
                                    break;
                                    case 0x32:       //�]�w�l���s��]�w�պA
                                    break;
                                    case 0x62:       //�d�ߤl���s��]�w�պA
                                    break;
                                    case 0xE2:       //�^���l���s��]�w�պA
                                    break;
                                    case 0x33:       //�]�w�l������պA
                                    break;
                                    case 0x63:       //�d�ߤl������պA���]�w���e
                                    break;
                                    case 0xE3:       //�^���l������պA���]�w���e
                                    break;
                                    case 0x08:       //�^�����x����{���ާ@
                                    break;
                                    case 0x0A:       //�^���{����Ƨ��
                                    break;
                                    case 0x0B:       //�n�D����ߤU�Ǹ�Ʈw���
                                    break;
                                    case 0x3F:       //�]�w��Ʀ^���ǿ�g��
                                    break;
                                    case 0x6F:       //�d�߲{���]�ƥ�q�ѼƤ��ǿ�g��
                                    break;
                                    case 0xEF:       //�d�߫�^���{���O�A�ΨB���ഫ��Ƥ��^���ǿ�g��
                                    break;
                                    case 0x0F:       //���жǿ���f�����O�A���A
                                    break;
                                    case 0x3E:       //�]�w�{���]�ƴ������A
                                    break;
                                    case 0x6E:       //�d�߲{���]�ƴ���]�w
                                    break;
                                    case 0xEE:       //�^���{���]�ƴ������A
                                    break;
                                    case 0x0E:       //�^���Ƶ{�������_��
                                    break;
                                    default:
                                    break;
                                }

                            } else if (messageIn[i].packet[7]==0x6F) {        //92�~�������������T��
//                                decideProtocol92Vd(messageIn[i]);
//================================================================
                                //�����������T�� 0x6F+controlCode
                                switch (messageIn[i].packet[8]) {
                                    default:
                                        messageIn[i].success=true;
                                    break;
                                }

                            } else if (messageIn[i].packet[7]==0xAF) {        //92�~����T�i�ܱ���T��
//                                decideProtocol92Cms(messageIn[i]);
//================================================================
                                switch (messageIn[i].packet[8]) {
                                    default:
                                        messageIn[i].success=true;
                                    break;
                                }

                            }

                        } else {                                                         //77��87�~���q�T��w
//                            decide77Or87(messageIn[i]);
//================================================================
                            switch(messageIn[i].packet[4]) {      //77��87�n�ѱ���X�P�_
                                 //���x����e�^����
                                 case 0xE7:          //77�~�����x���:�{��B���P�Ѿl��������
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0xE2:          //77�~�����x���:�{��ާ@�Ҧ�����
                                 case 0xD2:          //32�ɬq����ϥ�
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0xEC:          //77�~�����x���:����B�@���A����
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x0E:          //77�~�����x���:for ����
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x0F:          //77�~�����x���:
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x0C:          //77�~�����x���:Plan2�ǰe���\
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0xEF:          //77�~�����x���:�S����ɬq���A����
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=27) messageIn[i].success=true;
                                 break;
                                 case 0xEB:          //77�~�����x���:�̪�@���c���}�Ҥ������ɶ�
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=15) messageIn[i].success=true;
                                 break;
                                 case 0xE3:          //77�~�����x���:�{��ɬq�_�l�ɶ�����
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0xE9:          //77�~�����x���:�ɬq����
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0xEA:          //77�~�����x���:�ɬq���ɨ�p������
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0xE0:          //77�~�����x���:�Y�ɮɶ�����
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0xE1:          //77�~�����x���:����������
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                 break;
                                 case 0xE4:          //77�~�����x���:�Ĥ@���T������
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=14) messageIn[i].success=true;
                                 break;
                                 case 0xE5:          //77�~�����x���:�ĤG���T������
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=14) messageIn[i].success=true;
                                 break;
                                 case 0xE8:          //77�~�����x���:�g����ɬq���A����
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;

                                 //���߰e�ܸ��x���


                                 //��T�i�ܨt�α���e�^����
                                 case 0x00:          //87�~����T�i�ܨt�α��:�ǰe���\
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=9) messageIn[i].success=true;
                                 break;
                                 case 0x01:          //87�~����T�i�ܨt�α��:�ǰe����
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=9) messageIn[i].success=true;
                                 break;
                                 case 0x15:          //87�~����T�i�ܨt�α��:�w�魫�m�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x19:          //87�~����T�i�ܨt�α��:�n�魫�m�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x13:          //87�~����T�i�ܨt�α��:��ɻ~�t�L�j�n�D���߹��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=9) messageIn[i].success=true;
                                 break;
                                 case 0x14:          //87�~����T�i�ܨt�α��:�{���]�Ƥ���ɶ��^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=13) messageIn[i].success=true;
                                 break;
                                 case 0x2F:          //87�~����T�i�ܨt�α��:�{���]�ƶǿ�g���^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x1D:          //87�~����T�i�ܨt�α��:�{���]�Ƶw�骬�A�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x1E:          //87�~����T�i�ܨt�α��:�q�����_�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                 break;
                                 case 0x25:          //87�~����T�i�ܨt�α��:�����������^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0x26:          //87�~����T�i�ܨt�α��:�������޲z�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0x2A:          //87�~����T�i�ܨt�α��:����}�l�P�����^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x2E:          //87�~����T�i�ܨt�α��:�j������
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x2D:          //87�~����T�i�ܨt�α��:�n�D���ߤU�Ǳ�����
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x76:          //87�~����T�i�ܨt�α��:����T���^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=38) messageIn[i].success=true;
                                 break;
                                 case 0x86:          //87�~����T�i�ܨt�α��:�����m�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=22) messageIn[i].success=true;
                                 break;
                                 case 0xA8:          //87�~����T�i�ܨt�α��:����ɶ��^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=20) messageIn[i].success=true;
                                 break;
                                 case 0x52:          //87�~����T�i�ܨt�α��:��ܬ��ƻP���Z�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0x7F:          //87�~����T�i�ܨt�α��:���I���յ��G�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=16) messageIn[i].success=true;
                                 break;

                                 //77,87����
                                 case 0x10:
                                      if (messageIn[i].packetLength==12) {
                                             messageIn[i].protocol = PROTOCOL77;
                                             messageIn[i].success=true;
                                      } else {
                                          messageIn[i].protocol = PROTOCOL87;
                                          if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                      }
                                 break;
                                 case 0x29:          //87�~����T�i�ܨt�α��:�{���]�ƽs���^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;

                                 case 0x11:
                                      if (messageIn[i].packetLength==11) {
                                             messageIn[i].protocol = PROTOCOL77;
                                             messageIn[i].success=true;
                                      } else {
                                          messageIn[i].protocol = PROTOCOL87;
                                          if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                      }
                                 break;
                                 case 0x21:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x22:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x24:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x27:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x28:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x2C:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;



                                 //���߰e�ܸ�T�i�ܨt�α��
                                 case 0x12:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x2B:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x16:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x17:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x18:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x1C:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x1F:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;

                                 case 0x74:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x77:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x75:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x85:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x79:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x7C:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x7E:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0xA5:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0xA7:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0xA6:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x50:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x51:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x55:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x56:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x59:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;

                                 default: //�D�n���F���߰e��77�~�����f�������w
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=7) messageIn[i].success=true;
                                 break;
                            } //end switch

                        }

                    } else if (messageIn[i].packetLength>=8 && messageIn[i].packetLength<12) {      //����77�~��87�~����,���i�ର92�~��
//                         decide77Or87(messageIn[i]);
//================================================================
                            switch(messageIn[i].packet[4]) {      //77��87�n�ѱ���X�P�_
                                 //���x����e�^����
                                 case 0xE7:          //77�~�����x���:�{��B���P�Ѿl��������
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0xE2:          //77�~�����x���:�{��ާ@�Ҧ�����
                                 case 0xD2:          //32�ɬq����ϥ�
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0xEC:          //77�~�����x���:����B�@���A����
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x0E:          //77�~�����x���:for ����
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x0F:          //77�~�����x���:
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x0C:          //77�~�����x���:Plan2�ǰe���\
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0xEF:          //77�~�����x���:�S����ɬq���A����
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=27) messageIn[i].success=true;
                                 break;
                                 case 0xEB:          //77�~�����x���:�̪�@���c���}�Ҥ������ɶ�
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=15) messageIn[i].success=true;
                                 break;
                                 case 0xE3:          //77�~�����x���:�{��ɬq�_�l�ɶ�����
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0xE9:          //77�~�����x���:�ɬq����
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0xEA:          //77�~�����x���:�ɬq���ɨ�p������
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0xE0:          //77�~�����x���:�Y�ɮɶ�����
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0xE1:          //77�~�����x���:����������
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                 break;
                                 case 0xE4:          //77�~�����x���:�Ĥ@���T������
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=14) messageIn[i].success=true;
                                 break;
                                 case 0xE5:          //77�~�����x���:�ĤG���T������
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=14) messageIn[i].success=true;
                                 break;
                                 case 0xE8:          //77�~�����x���:�g����ɬq���A����
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;

                                 //���߰e�ܸ��x���


                                 //��T�i�ܨt�α���e�^����
                                 case 0x00:          //87�~����T�i�ܨt�α��:�ǰe���\
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=9) messageIn[i].success=true;
                                 break;
                                 case 0x01:          //87�~����T�i�ܨt�α��:�ǰe����
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=9) messageIn[i].success=true;
                                 break;
                                 case 0x15:          //87�~����T�i�ܨt�α��:�w�魫�m�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x19:          //87�~����T�i�ܨt�α��:�n�魫�m�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x13:          //87�~����T�i�ܨt�α��:��ɻ~�t�L�j�n�D���߹��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=9) messageIn[i].success=true;
                                 break;
                                 case 0x14:          //87�~����T�i�ܨt�α��:�{���]�Ƥ���ɶ��^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=13) messageIn[i].success=true;
                                 break;
                                 case 0x2F:          //87�~����T�i�ܨt�α��:�{���]�ƶǿ�g���^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x1D:          //87�~����T�i�ܨt�α��:�{���]�Ƶw�骬�A�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x1E:          //87�~����T�i�ܨt�α��:�q�����_�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                 break;
                                 case 0x25:          //87�~����T�i�ܨt�α��:�����������^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0x26:          //87�~����T�i�ܨt�α��:�������޲z�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0x2A:          //87�~����T�i�ܨt�α��:����}�l�P�����^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x2E:          //87�~����T�i�ܨt�α��:�j������
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x2D:          //87�~����T�i�ܨt�α��:�n�D���ߤU�Ǳ�����
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x76:          //87�~����T�i�ܨt�α��:����T���^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=38) messageIn[i].success=true;
                                 break;
                                 case 0x86:          //87�~����T�i�ܨt�α��:�����m�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=22) messageIn[i].success=true;
                                 break;
                                 case 0xA8:          //87�~����T�i�ܨt�α��:����ɶ��^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=20) messageIn[i].success=true;
                                 break;
                                 case 0x52:          //87�~����T�i�ܨt�α��:��ܬ��ƻP���Z�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0x7F:          //87�~����T�i�ܨt�α��:���I���յ��G�^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=16) messageIn[i].success=true;
                                 break;

                                 //77,87����
                                 case 0x10:
                                      if (messageIn[i].packetLength==12) {
                                             messageIn[i].protocol = PROTOCOL77;
                                             messageIn[i].success=true;
                                      } else {
                                          messageIn[i].protocol = PROTOCOL87;
                                          if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                      }
                                 break;
                                 case 0x29:          //87�~����T�i�ܨt�α��:�{���]�ƽs���^��
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;

                                 case 0x11:
                                      if (messageIn[i].packetLength==11) {
                                             messageIn[i].protocol = PROTOCOL77;
                                             messageIn[i].success=true;
                                      } else {
                                          messageIn[i].protocol = PROTOCOL87;
                                          if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                      }
                                 break;
                                 case 0x21:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x22:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x24:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x27:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x28:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x2C:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;



                                 //���߰e�ܸ�T�i�ܨt�α��
                                 case 0x12:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x2B:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x16:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x17:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x18:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x1C:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x1F:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;

                                 case 0x74:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x77:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x75:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x85:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x79:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x7C:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x7E:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0xA5:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0xA7:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0xA6:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x50:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x51:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x55:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x56:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x59:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;

                                 default: //�D�n���F���߰e��77�~�����f�������w
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=7) messageIn[i].success=true;
                                 break;
                            } //end switch

                    }

                } else if (messageIn[i].packet[1]==(const BYTE)0xDD){         //���{���X
                    if (messageIn[i].packetLength==5) {
                        messageIn[i].protocol = PROTOCOL77;                              //77�~�����{���X
                        messageIn[i].success=true;
                    } else if (messageIn[i].packetLength==8) {
                               messageIn[i].protocol = PROTOCOL92;                       //92�~�����{���X
                               messageIn[i].success=true;
                    }

                } else if (messageIn[i].packet[1]==(const BYTE)0xEE){         //�t�{���X
                    if (messageIn[i].packetLength==6) {
                        messageIn[i].protocol = PROTOCOL77;                              //77�~���t�{���X
                        messageIn[i].success=true;
                    } else if (messageIn[i].packetLength==9) {
                               messageIn[i].protocol = PROTOCOL92;                       //92�~���t�{���X
                               messageIn[i].success=true;
                    }

                }
            }

        }
          }
      }
  } catch(...) {}
}

//---------------------------------------------------------------------------
bool PARSETRAFFIC::CheckSum(int *maxMessageIndex,MESSAGEOK *messageIn)          //�ֹ��ˬd�X
{
try {
    if (*maxMessageIndex >= 0) {                      //�T�O
        for (int i=0;i<=*maxMessageIndex;i++) {       //�ˬd�C�@��messageIn[i]

/*Debug Code
        if ((messageIn[i].packet[0]==(const BYTE)0xAA) && (messageIn[i].packet[1]==(const BYTE)0xDD)) {
          printf("printfMsgAADD1 cksStatus:%d, success:%d, SEQ:%d\n",
          messageIn[i].cksStatus, messageIn[i].success, messageIn[i].packet[2]);
        }
*/


//    if(messageIn[i].packet[7] == (const BYTE)0x6F) printf("printfMsg  this is 6f!\n");
//    for(int ii = 0; ii < 12; ii++)
//      printf("%x ",messageIn[i].packet[ii]);
//    printf("\n");

             int j=0;                                 //�p��messageIn[].packet[j];
             BYTE tempcks=0;                          //�Ȧs�n��諸checkSum;
             messageIn[i].cksStatus=false;            //default��false

             if (messageIn[i].packetLength>4) {        //���ʥ]���צܤ֭n��5(AA DD 00 01 cks-77�~,AA DD 00 00 01 00 08 cks-92�~)

                 if ((messageIn[i].packet[0]==(const BYTE)0xAA) &&
                     (messageIn[i].packet[1]==(const BYTE)0xDD || messageIn[i].packet[1]==(const BYTE)0xEE) &&
                     (messageIn[i].packetLength==5 || messageIn[i].packetLength==6 || messageIn[i].packetLength==8 || messageIn[i].packetLength==9)) {

                       printf("p2\n");

                      for (int k=0;k<messageIn[i].packetLength-1;k++)           //AA DD(EE) HI LO CKS
                           tempcks^=messageIn[i].packet[k];                     //��packetIn��CheckSum

                           if (messageIn[i].packet[messageIn[i].packetLength-1]==tempcks)
                               messageIn[i].cksStatus=true;

                 } else {

                      for (j=4;j<messageIn[i].packetLength;j++) {

//                           if ( (messageIn[i].packet[j-3]!=(const BYTE)0xAA && messageIn[i].packet[j-2]==(const BYTE)0xAA && messageIn[i].packet[j-1]==(const BYTE)0xCC) ||     //�ĤG�P�_:aa cc�@�w�۳s//�קKAA AA CC�]��AA�O�w�]���W��byte
                           if ( (messageIn[i].packet[j-4]!=(const BYTE)0xAA && messageIn[i].packet[j-3]!=(const BYTE)0xBB && messageIn[i].packet[j-2]==(const BYTE)0xAA && messageIn[i].packet[j-1]==(const BYTE)0xCC) ||     //�ĤG�P�_:aa cc�@�w�۳s//�קKAA AA CC�]��AA�O�w�]���W��byte
                                ((messageIn[i].packet[4]==(const BYTE)0x55 || messageIn[i].packet[4]==(const BYTE)0x56) && messageIn[i].packet[j-2]==(const BYTE)0xAA && messageIn[i].packet[j-1]==(const BYTE)0xCC) )     //��w�X��56�����i��ǰeAA BB XX XX 56 ...... AA AA CC Cks����w�L��
                              {

//OTDebug                                 printf("p4\n");

                                 for (int k=0 ; k<j ; k++)                      //j=packetInLength-1=cks�Ҧb��[Index]
                                      tempcks^=messageIn[i].packet[k];          //��packetIn��CheckSum

                                 if (messageIn[i].packet[j]==tempcks)  {        //�ĤT�P�_:aa cc�᪺cks
                                                                                //�@�w�M���}�C�۲�..
                                     messageIn[i].packetLength=j+1;             //�p�G�T�ӧP�_����,���}�C�����״N�Oj+1(���B�������sassign)
                                     messageIn[i].cksStatus=true;               //�N���}�C�]�����T��
//OTDebug                                     printf("cks OK, i:%d\n", i);
                                     break;                                     //�p�G�T�ӧP�_�����T�h���X���j��

                                 }  //endIf
                           }

                      }  //end for (j=3 ; j <  messageIn[i].packetLength ; j++)
                 }
             }  //end if (messageIn[i].packetLength > 4)
        }  //end for (int i=0 ; i < maxMessageIndex ; i++)
    }  //end if (maxMessageIndex >= 0)

    return true;


  } catch(...) {}
}