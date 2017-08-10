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


smem.junli_object.open_port_process("/dev/ttyUSB0");
smem.junbo_object.open_port_process("/dev/ttyS1");


        int iCom2Type;
        iCom2Type = smem.vGetINTData(Com2_TYPE);

        //remove tc reset file
        system("rm -rf /tmp/resetTCmyself.txt");
        system("sync");

        int iTmp;

        /*
          ³q°T°ð¤À§G

          RS232,422,485 ³q°T°ð
          /dev/ttyS10(RS232)         ¥æ³q±±¨î¤¤¤ß-GPRS MODEM-(9600 N 8 1)
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


        if( 1 == ucTmp )
        {


        }
        else
        {
            if (smem.centerPort.SetConnDevice(DEVICECENTER92))
                if ((tempmax=smem.centerPort.OpenRs232Port("/dev/ttyS10",9600, false))>0)
                {
                    if (tempmax>maxport)
                    {
                        maxport = tempmax;
                    }
                    printf("open CenterPort-/dev/ttyS10 (fdValue:%d) Success!!\n",tempmax);
                }
                else printf("open CenterPort Fail!!\n");
        }


        //¶}±ÒKEYPAD³q°T°ð
        if (smem.keypadPort.SetConnDevice(DEVICEKEYPAD))
            if ((tempmax=smem.keypadPort.OpenRs232Port("/dev/ttyS2",9600,false))>0)
            {
                if (tempmax>maxport)  maxport=tempmax;
                printf("open KeypadPort-/dev/ttyS2 (fdValue:%d) Success!!\n",tempmax);
            }
            else printf("open KeypadPort Fail!!\n");





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

int newmax=0;
newmax=maxport;
printf("maxport=%d\n",maxport);
        if (smem.centerSocket.SetConnDevice(DEVICECENTER92))
            if ((tempmax=smem.centerSocket.OpenUdpSocket(localIP1,localPort1,distIP0,distPort0))>0)
            {   printf("tempmax=%d\n",tempmax);
                if (tempmax>maxport)  maxport=tempmax;
                printf("open CenterSocket-%s:%d (fdValue:%d) Success!!\n",localIP1,localPort1,tempmax);
            }
            else printf("open CenterSocket-%s:%d Fail!!\n",localIP1,localPort1);
printf("maxport=%d\n",maxport);



        maxport=maxport+newmax+1;                                                          //important

printf("maxport=%d\n",maxport);
        //¶}±ÒIO ³q°T°ð
        //LPT1(0x378) LCD­±ªO
//ICOPBUG
//    if (lcd240x128.GetAuthority(0x278)) {                                       //±o¨ìkernalÅv­­,¥i¥H±±¨îLPT1
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



        //OT Fix 950727   LCN0000
        SendRequestToKeypad();                                                      //°Ýkeypad¥Ø«e­±ªO³]©w

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

            //join rs232 fd_set
            if (smem.centerPort.GetPortAlreadyOpen()) FD_SET(smem.centerPort.Getfd(),&readfs);


            if (smem.keypadPort.GetPortAlreadyOpen()) FD_SET(smem.keypadPort.Getfd(),&readfs);


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


                if (smem.centerPort.GetPortAlreadyOpen())                           //port¬O§_¤w¶}±Ò
                {
                    if (FD_ISSET(smem.centerPort.Getfd(),&readfs))                  //port¬O§_¦³¸ê®Æ
                    {
                        readSelectLength=smem.centerPort.Rs232Read();               //±Nrs232¸ê®Æq1`Åª¶ibuffer¤¤
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)
                            {
                                //OT20110526
                                smem.vSetLastGetProtocolTime();
                                parseAABB.ParseBlock(readSelectLength,smem.centerPort.block,smem.centerPort.messageIn,&smem.centerPort.lastPacketIndex,&smem.centerPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                parseAABB.DecideProtocol(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn,smem.centerPort.GetConnDevice());
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                parseAABB.CheckReasonable(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                parseAABB.AssignLcn(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn,cRS232);
                                readJob.vCheckIfBcastingForwardToUDP(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                readJob.CheckLcn(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                readJob.SetCenterComm(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                parseAABB.EchoToGUI(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn,"/dev/ttyS10");
                                parseAABB.MoveLastData(&smem.centerPort.maxMessageIndex,&smem.centerPort.lastPacketIndex,smem.centerPort.messageIn);
                            }
                        }
                    }
                }

                if (smem.keypadPort.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.keypadPort.Getfd(),&readfs))
                    {
                        readSelectLength=smem.keypadPort.Rs232Read();
                        if (readSelectLength>0)
                        {

//OT20111116
                            if(smem.vGetCommEnable() == true)
                            {
                                parseAABB.ParseBlock(readSelectLength,smem.keypadPort.block,smem.keypadPort.messageIn,&smem.keypadPort.lastPacketIndex,&smem.keypadPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn);
                                parseAABB.DecideProtocol(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn,smem.keypadPort.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn);
                                parseAABB.AssignLcn(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn,cRS232);
                                readJob.SetKeypadComm(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn);
                                parseAABB.EchoToGUI(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn,"/dev/ttyS2");
                                parseAABB.MoveLastData(&smem.keypadPort.maxMessageIndex,&smem.keypadPort.lastPacketIndex,smem.keypadPort.messageIn);
                            }
                        }
                    }
                }



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
                            for(int i =0; i<readSelectLength; i++)
                                printf("%x ",smem.junbo_object.junbo_cms_port.block[i]);

                            printf("\n\n");

                            smem.junbo_object.ParseBlock(readSelectLength,smem.junbo_object.junbo_cms_port.block,smem.junbo_object.junbo_cms_port.messageIn,
                                                          &smem.junbo_object.junbo_cms_port.lastPacketIndex);

                        }
                    }

                }

                if(smem.revAPP_socket.GetPortAlreadyOpen())
                {
                    if(FD_ISSET(smem.revAPP_socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.revAPP_socket.UdpRead();

                        printf("receive messgae REVAPP~~~~~~~~~~~\n");
                        if(readSelectLength>0)
                        {
                            //printf("readSelectLength=%d\n",readSelectLength);
                            if(smem.vGetCommEnable()==true)
                            {
                                int rev_select=0;
                                revAPP_messagein=smem.o_Junbo_light.revAPP_packet(readSelectLength,smem.revAPP_socket.block);
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
                                    case (0x10):
                                        oRev_protocol_0F.vRebootIPC_0F10_revAPP(revAPP_messagein);
                                        break;
                                    case (0x4a):
                                        smem.disk.WriteNetworkSetConfirm_RevAPP(revAPP_messagein);
                                        break;
                                    case (0x18):
                                        smem.disk.dispalyLCN_NetworkIP_to_RevAPP();
                                        break;
                                    case(0xc1):
                                        oRev_protocol_0F.vReportIPCTime_0FC2_revAPP();
                                        printf("hello\n\n\n");
                                        break;
                                    case(0x01):
                                        if((revAPP_messagein.packet[2]==0x02)&&(revAPP_messagein.packet[3]=0x03))
                                            oRev_protocol_0F.check_link_revAPP();
                                        else printf("check link packet miss\n");

                                        break;
                                    default:
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

            }

        }


        //Ãö³¬RS232,422,485 ³q°T°ð
        if (smem.centerPort.CloseRs232Port()) printf("Close CenterPort Successful!!\n");

        if (smem.keypadPort.CloseRs232Port()) printf("Close KeypadPort Successful!!\n");

        if (smem.centerSocket.CloseUdpSocket()) printf("Close CenterSocket Successful!!\n");

        //Ãö³¬IO ³q°T°ð
        lcd240x128.ReleaseAuthority();
        digitalIO.ReleaseAuthority();


        return 0;

    }
    catch (...) {}
}