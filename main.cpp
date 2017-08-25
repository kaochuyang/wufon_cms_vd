//---------------------------------------------------------------------------
#pragma hdrstop

#include "var.h"
#include "SMEM.h"
#include "PARSETRAFFIC.h"
#include "PARSERTMS.h"
#include "PARSENMEA.h"
#include "PARSESS.h"

#include "READJOB.h"
#include "WRITEJOB.h"

#include "LCD240x128.h"
#include "DIGITALIO.h"

#include "CTIMER.h"
#include "CDataToMessageOK.h"

#include "SCREENLogo.h"

#include "STORAGE.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

/*Combo++*/
#include "CSTC.h"
#include "CSTORINI.h"
/*Combo--*/
#include "PTRAFFIC92TC.h"
#include "PTRAFFIC92COMM.h"
//---------------------------------------------------------------------------
//void *timerHandle (void *);                                                     //¥t¤@­Óthread,±Mªù±±¨îTimer
//---------------------------------------------------------------------------
void SendRequestToKeypad(void)
{
    try
    {

        MESSAGEOK messageOut;
        messageOut.protocol=PROTOCOLKEYPAD;
        messageOut.packetLength=6;
        messageOut.cksStatus=true;
        messageOut.success=true;
        messageOut.ReadorWrite=cWRITE;

        messageOut.packet[0]=0xAA;
        messageOut.packet[1]=0xBB;
        messageOut.packet[2]=0x03;
        messageOut.packet[3]=0xAA;
        messageOut.packet[4]=0xCC;
        messageOut.packet[5]=0;
        for (int i=0; i<5; i++) messageOut.packet[5]^=messageOut.packet[i];

        writeJob.WriteWorkByMESSAGEOUT(messageOut);

    }
    catch (...) {}
}

//---------------------------------------------------------------------------
#pragma argsused
int main(int argc, char* argv[])
{
    try
    {
        bool notexit=true;                                                          //while loop ¥Ã¤£Â÷¶}
        unsigned char ucTmp;

        int iFWYearTmp;
        int iFWMonthTmp;
        int iFWDayTmp;
        char cTMP[128];

        int rec=-1;                                                                 //Select Result
        fd_set readfs;                                                              //File Descriptor Set
        int maxport=0,tempmax=0;                                                    //maxnum file desciptor used
        struct timeval timeout;                                                     //°²¨Ï­Y¤z¬íselect³£¨S¦³¸ê®Æ¶i¨Ó,´N°µtimeout­n§@ªº¨Æ±¡
        int readSelectLength=0;                                                     //§PÂ_Åª¨ìªºªø«×

        PARSETRAFFIC parseAABB;                                                     //¸ÑªR©Ò¦³AABB¶}ÀYAACCµ²§Àªºª«¥ó
        PARSERTMS parseRtms;                                                        //¸ÑªRRTMSªºª«¥ó
        PARSESS parseSS;                                                            //¸ÑªRSmartSensorªºª«¥ó
        PARSENMEA parseNMEA;
        tsUDPMappingLCN _sUDPMappingTMP;
        tsUDPInfo _sUDPMappingInfoTMP;

        int iCom2Type;
        iCom2Type = smem.vGetINTData(Com2_TYPE);

        //remove tc reset file
        system("rm -rf /tmp/resetTCmyself.txt");
        system("sync");

        int iTmp;

        smem.junli_object.open_port_process("/dev/ttyS0");
        smem.junbo_object.initial_junbo_control("/dev/ttyS1");


        /*
          ³q°T°ð¤À§G

          RS232,422,485 ³q°T°ð
          /dev/ttyS0(RS232)         ¥æ³q±±¨î¤¤¤ß-GPRS MODEM-(9600 N 8 1)
          /dev/ttyS1(RS232)         Resrever
          /dev/ttyS2(RS232)         KEYPAD-(9600 N 8 1)
          /dev/ttyS3(RS232)         ¿OÂI±±¨î(9600 N 8 1)
          ??????????(RS485)         RTMS-(9600 N 8 1)

          UDP SOCKET ³q°T°ð
          192.168.1.101:6003        ¥æ³q±±¨î¤¤¤ß
          192.168.1.101:6789        «O¯d´ú¸Õ¥Î
          192.168.1.102:6003        ²{³õ´ú¸Õ¾÷


          IO ³q°T°ð
          LPT1  LCD(240x128)  ¦ì§}-0x378
          DGIO  Light         ¦ì§}-In:0x200 Out:0x208
        */


        //¶}±ÒRS232,422,485 ³q°T°ð
        //¶}±Ò¥æ³q±±¨î¤¤¤ß³q°T°ð
/*
        ucTmp = smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status);
        printf("smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status):%d\n", ucTmp);

*/

        if (smem.lightPort.SetConnDevice(DEVICETRAFFICLIGHT))
            if ((tempmax=smem.lightPort.OpenRs232Port("/dev/ttyS3",9600,false))>0)
            {
                if (tempmax>maxport)  maxport=tempmax;
                printf("open TrafficLightPort-/dev/ttyS3 (fdValue:%d) Success!!\n",tempmax);
            }
            else printf("open LightPort Fail!!\n");


        int localPort1=smem.GetLocalIP1(5);
        int distPort0=smem.GetdistIp0(5);
        int distPort=smem.GetDistIP(5);

        char localIP1[20]= {0},distIP0[20]= {0},distIP[20]= {0};
        char tmp[5]= {0};

        for (int ip=1; ip<=4; ip++)
        {
            sprintf(tmp,"%d",smem.GetLocalIP1(ip));
            strcat(localIP1,tmp);
            if (ip<4) strcat(localIP1,".");
            sprintf(tmp,"%d",smem.GetdistIp0(ip));
            strcat(distIP0,tmp);
            if (ip<4) strcat(distIP0,".");
            sprintf(tmp,"%d",smem.GetDistIP(ip));
            strcat(distIP,tmp);
            if (ip<4) strcat(distIP,".");
        }


        printf("\nIP1 : %s:%d\n",localIP1,localPort1);
        printf("DISTIP0 : %s:%d\n",distIP0, distPort0);
        printf("DISTIP : %s:%d\n\n",distIP,distPort);


        //¶}±ÒUDP SOCKET ³q°T°ð
        //¶}±Ò»P¥æ³q±±¨î¤¤¤ß³q°TªºUDP SOCKET


        if (smem.centerSocket.SetConnDevice(DEVICECENTER92))
            if ((tempmax=smem.centerSocket.OpenUdpSocket(localIP1,localPort1,distIP0,distPort0))>0)
            {
                if (tempmax>maxport)  maxport=tempmax;
                printf("open CenterSocket-%s:%d (fdValue:%d) Success!!\n",localIP1,localPort1,tempmax);
            }
            else printf("open CenterSocket-%s:%d Fail!!\n",localIP1,localPort1);



////-----------------------------------------------------------------------------------------------------//
        system("ifconfig eth1 192.168.2.1");//for VSX-6156
        if ( smem.revAPP_socket.SetConnDevice(1000))
            if ((tempmax= smem.revAPP_socket.OpenUdpSocket("192.168.2.1",7002,"192.168.2.2",7002))>0)
            {
                if (tempmax>maxport)  maxport=tempmax;
                printf("open rev_socket-%s:%d (fdValue:%d) Success!!\n","192.168.2.1",7002,tempmax);
            }
            else printf("open CenterSocket-%s:%d Fail!!\n","192.168.2.2",7002);
        PTRAFFIC92TC oRev_protocol_5F;
        PTRAFFIC92COMM oRev_protocol_0F;
        MESSAGEOK revAPP_messagein;
////-----------------------------------------------------------------------------------------------------//



        maxport=maxport+1;                                                          //important


        //¶}±ÒIO ³q°T°ð
        //LPT1(0x378) LCD­±ªO
                                       //±o¨ìkernalÅv­­,¥i¥H±±¨îLPT1
        if (lcd240x128.GetAuthority(0x378))                                         //±o¨ìkernalÅv­­,¥i¥H±±¨îLPT1
        {
            lcd240x128.GRAPHIC_MODE();                                              //±NLcd³]©w¬°Graphicµe­±
            //OTMARKPRINTF  printf("LCD GetKernalAuthority Successful!!\n");
        }
        else printf("LCD GetKernalAuthority Fail!!\n");

        //Digital Input Output
        if (digitalIO.GetAuthority(0x200,0x208))
        {
            //OTMARKPRINTF  printf("MotherBoard Digital I/O GetKernalAuthority Successful!!\n");
        }
        else printf("MotherBoard Digital I/O GetKernalAuthority Failure!!\n");

        screenLogo.DisplayLogo();                                                   //Åã¥ÜLogo­¶

        printf("Main 1 OK! \n");

        _intervalTimer.ThreadsGenerate();                                           //°_Interval Timer Thread

//    stc.ThreadsGenerate();

        //OT Fix 950727   LCN0000
//   SendRequestToKeypad();                                                      //°Ýkeypad¥Ø«e­±ªO³]©w

        printf("Main 2 OK! \n");

        bzero(cTMP, 128);
        iFWYearTmp = smem.vGetFirmwareInfo_0FC3(0);
        iFWMonthTmp = smem.vGetFirmwareInfo_0FC3(1);
        iFWDayTmp = smem.vGetFirmwareInfo_0FC3(2);
        sprintf(cTMP, "TC Booting, FW Ver:%02d-%02d-%02d", iFWYearTmp, iFWMonthTmp, iFWDayTmp);
        smem.vWriteMsgToDOM(cTMP);

        while (notexit)
        {

            FD_ZERO(&readfs);

             if(smem.revAPP_socket.GetPortAlreadyOpen())FD_SET(smem.revAPP_socket.Getfd(),&readfs);//for revAPP

            if (smem.lightPort.GetPortAlreadyOpen()) FD_SET(smem.lightPort.Getfd(),&readfs);

            if (smem.centerSocket.GetPortAlreadyOpen()) FD_SET(smem.centerSocket.Getfd(),&readfs);

            if(smem.junli_object.junli_port.GetPortAlreadyOpen())FD_SET(smem.junli_object.junli_port.Getfd(),&readfs);
            if(smem.junbo_object.junbo_cms_port.GetPortAlreadyOpen())FD_SET(smem.junbo_object.junbo_cms_port.Getfd(),&readfs);

            timeout.tv_sec=60;                                                      //timeout¬í¼Æ
            timeout.tv_usec=0;                                                      //³o­Ó¬O²@¬í,¼È¤£¨Ï¥Î

            rec=select(maxport,&readfs,NULL,NULL,&timeout);                         //wait block for read

            if (rec<0)                                                              //Select Error
            {

            }
            else if (rec==0)                                                        //Select Time Out
            {
                //OTMARKPRINTF  printf("Select Time Out!!\n");
            }
            else
            {


              /* if (smem.lightPort.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.lightPort.Getfd(),&readfs))
                    {
                        readSelectLength=smem.lightPort.Rs232Read();
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)    //OT20110728
                            {
                                parseAABB.ParseBlock(readSelectLength,smem.lightPort.block,smem.lightPort.messageIn,&smem.lightPort.lastPacketIndex,&smem.lightPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.lightPort.maxMessageIndex,smem.lightPort.messageIn);
                                parseAABB.DecideProtocol(&smem.lightPort.maxMessageIndex,smem.lightPort.messageIn,smem.lightPort.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.lightPort.maxMessageIndex,smem.lightPort.messageIn);
                                parseAABB.AssignLcn(&smem.lightPort.maxMessageIndex,smem.lightPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.lightPort.maxMessageIndex,smem.lightPort.messageIn,cRS232);
                                readJob.DoWorkByMESSAGEIN(&smem.lightPort.maxMessageIndex,smem.lightPort.messageIn);
                                parseAABB.EchoToGUI(&smem.lightPort.maxMessageIndex,smem.lightPort.messageIn,"/dev/ttyS3");
                                parseAABB.MoveLastData(&smem.lightPort.maxMessageIndex,&smem.lightPort.lastPacketIndex,smem.lightPort.messageIn);



                                smem.vSetBOOLData(TC_SIGNAL_DRIVER_UNIT, true);
                            }
                        }
                    }
                }*/

                if (smem.junli_object.junli_port.GetPortAlreadyOpen())
                {
                    if (smem.junli_object.junli_port.Getfd(),&readfs)
                    {
                        readSelectLength=smem.junli_object.junli_port.Rs232Read();
                        if (readSelectLength>0)
                        {

                            smem.junli_object.parse_junli(readSelectLength,smem.junli_object.junli_port.block,smem.junli_object.junli_port.messageIn,&smem.junli_object.junli_port.lastPacketIndex);
                        }
                    }
                }


                if(smem.junbo_object.junbo_cms_port.GetPortAlreadyOpen())
                {
                    if(FD_ISSET(smem.junbo_object.junbo_cms_port.Getfd(),&readfs))
                    {
                        readSelectLength=smem.junbo_object.junbo_cms_port.Rs232Read();
                        if(readSelectLength>0)
                        {
                            /*  for(int i =0; i<readSelectLength; i++)
                                  printf("%x ",smem.junbo_object.junbo_cms_port.block[i]);*/

                            //  printf("\n\n");

                            smem.junbo_object.ParseBlock(readSelectLength,smem.junbo_object.junbo_cms_port.block,smem.junbo_object.junbo_cms_port.messageIn,
                                                         &smem.junbo_object.junbo_cms_port.lastPacketIndex);

                        }
                    }

                }

  /*****************************************************************/
                if(smem.revAPP_socket.GetPortAlreadyOpen())
                {
                    if(FD_ISSET(smem.revAPP_socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.revAPP_socket.UdpRead();

                        printf("receive messgae REVAPP~~~~~~~~~~~\n");
                        if(readSelectLength>0)
                        {
                            //printf("readSelectLength=%d\n",readSelectLength);
                            if(1)//smem.vGetCommEnable()==true)
                            {
                                int rev_select=0;
                                revAPP_messagein=smem.o_Junbo_light.revAPP_packet(readSelectLength,smem.revAPP_socket.block);
                                for(int g=0;g<revAPP_messagein.packetLength;g++)printf("%x ",revAPP_messagein.packet[g]);
                                        printf("\n");


                                if(revAPP_messagein.packet[0]==0x5F)
                                {
                                    switch(revAPP_messagein.packet[1])
                                    {
                                    case (0x11):
                                        oRev_protocol_5F.vWriteReverseTimeData5F11_revAPP(revAPP_messagein);
                                        break;
                                    case (0x41):
                                        oRev_protocol_5F.vQueryReverseTimeData5F41_revAPP(revAPP_messagein);
                                        break;
                                    case (0x12):
                                        oRev_protocol_5F.vWriteReverseTimeData5F12_revAPP(revAPP_messagein);
                                        break;
                                    case (0x42):
                                        oRev_protocol_5F.vQueryReverseTimeData5F42_revAPP(revAPP_messagein);
                                        break;
                                    case (0xc1):
                                        smem.o_Junbo_light.light_timeout_control(revAPP_messagein.packet[2]);
                                        break;
                                    case (0xc3):
                                        smem.o_Junbo_light.report_light_timeout();
                                        break;
                                    case (0xc5):
                                        smem.o_Junbo_light.brightness_control(revAPP_messagein.packet[2]);
                                        break;
                                    case (0xc6):
                                        smem.o_Junbo_light.report_light_brightness();
                                    case (0xc7):
                                        smem.o_Junbo_light.report_module_state_to_revapp();
                                        break;

                                    default:
                                        printf("wrong revAPP 5F protocol!!\n");
                                        break;
                                    }
                                }
                                else if(revAPP_messagein.packet[0]==0x0F)
                                {
                                    switch(revAPP_messagein.packet[1])
                                    {
                                        case(0x0):
                                        smem.junbo_object.cms_test_function(revAPP_messagein.packet[2]);
                                        smem.vWriteMsgToDOM("cms_test_function");

                                        break;
                                    case(0xb7):
                                        smem.power_object.report_power_reboot();
                                        smem.vWriteMsgToDOM("report_power_reboot by app");


                                        break;
                                    case (0x5a):
                                        smem.junbo_object.query_modual_state();
                                        smem.vWriteMsgToDOM("query_modual_state by app");


                                    case (0x16):


                                        smem.power_object.power_reset(revAPP_messagein.packet[2],(int)revAPP_messagein.packet[3]);
                                        smem.vWriteMsgToDOM("power_reset by app");
                                        break;

                                    case (0x17):
                                        oRev_protocol_0F.VD_CMS_reboot_0F17(revAPP_messagein.packet[2]);
                                        smem.vWriteMsgToDOM("write VD_CMS_reboot_0F17 by app");
                                        break;
                                    case (0x19):
                                        smem.junbo_object.brightness_control(revAPP_messagein.packet[2]);
                                        smem.vWriteMsgToDOM("write brightness_control by app");
                                        break;
                                    case(0x29):
                                        smem.junbo_object.report_light_brightness();
                                        smem.vWriteMsgToDOM("report_light_brightness by app");
                                        break;

                                        break;
                                    case(0x1b):
                                        smem.junbo_object.report_module_state_to_revapp();
                                        smem.vWriteMsgToDOM("report_module_state_to_revapp by app");
                                        break;

                                    case (0x1c):
                                        smem.junbo_object.light_timeout_control(revAPP_messagein.packet[2]);
                                        smem.vWriteMsgToDOM("light_timeout_control by app");
                                        break;
                                    case(0x1d):
                                        smem.junbo_object.report_light_timeout();
                                        smem.vWriteMsgToDOM("report_light_timeout by app");
                                        break;

                                    case (0x10):
                                        smem.vWriteMsgToDOM("vRebootIPC_0F10_revAPP by app");
                                        oRev_protocol_0F.vRebootIPC_0F10_revAPP(revAPP_messagein);
                                        break;

                                    case (0x4a):
                                        smem.vWriteMsgToDOM("WriteNetworkSetConfirm_RevAPP by app");
                                        smem.disk.WriteNetworkSetConfirm_RevAPP(revAPP_messagein);
                                        break;
                                    case (0x18):
                                        smem.vWriteMsgToDOM("dispalyLCN_NetworkIP_to_RevAPP by app");
                                        smem.disk.dispalyLCN_NetworkIP_to_RevAPP();
                                        break;
                                    case(0xc1):
                                        smem.vWriteMsgToDOM("vReportIPCTime_0FC2_revAPP by app");
                                        oRev_protocol_0F.vReportIPCTime_0FC2_revAPP();
                                        printf("hello\n\n\n");
                                        break;
                                    case(0x01):
                                        smem.vWriteMsgToDOM("check link state by app");
                                        if((revAPP_messagein.packet[2]==0x02)&&(revAPP_messagein.packet[3]=0x03))
                                            oRev_protocol_0F.check_link_revAPP();
                                        else printf("check link packet miss\n");

                                        break;
                                    default:
                                        smem.vWriteMsgToDOM("wrong revAPP 0F protocol!! by app");
                                        printf("wrong revAPP 0F protocol!!\n");
                                        break;
                                    }
                                }
                                else if(revAPP_messagein.packet[0]==0xab)oRev_protocol_5F.revseg_send_to_revAPP();
                                memset(revAPP_messagein.packet,0x0,sizeof(revAPP_messagein.packet));
                            }
                        }
                    }
                }
 /********************************************************************/


                if (smem.centerSocket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.centerSocket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.centerSocket.UdpRead();
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)
                            {
                                //OT20110526
                                smem.vSetLastGetProtocolTime();

                                parseAABB.ParseBlock(readSelectLength,smem.centerSocket.block,smem.centerSocket.messageIn,&smem.centerSocket.lastPacketIndex,&smem.centerSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                //enable vJudgeProtocolAndCheckLength
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn,smem.centerSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                parseAABB.AssignLcn(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn,cUDP);
                                readJob.vCheckIfBcastingForwardToUDP(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                readJob.CheckLcn(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                readJob.SetCenterComm(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn,"192.168.1.101:6003");
                                parseAABB.MoveLastData(&smem.centerSocket.maxMessageIndex,&smem.centerSocket.lastPacketIndex,smem.centerSocket.messageIn);
                            }
                        }
                    }
                }


            }

        }

        //pthread_join(tidTimerHandle,NULL);                                          //µ²§ô¥t¤@­Óthread


        //Ãö³¬RS232,422,485 ³q°T°ð

        if (smem.centerSocket.CloseUdpSocket()) printf("Close CenterSocket Successful!!\n");


        //Ãö³¬IO ³q°T°ð
        lcd240x128.ReleaseAuthority();
        digitalIO.ReleaseAuthority();


        return 0;

    }
    catch (...) {}
}
//---------------------------------------------------------------------------
void *timerHandle (void *)
{
}
//---------------------------------------------------------------------------
