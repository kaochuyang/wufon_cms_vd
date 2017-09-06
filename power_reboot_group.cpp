#include "power_reboot_group.h"
#include <stdio.h>
#include "SMEM.h"
#include "WRITEJOB.h"
//#include "CTIMER.h"
//struct timer_object power_reboot_group::power_timer;
#include "CDataToMessageOK.h"


power_reboot_group::power_reboot_group()
{



    //ctor
}

power_reboot_group::~power_reboot_group()
{
    //dtor
}


bool power_reboot_group::power_reset(BYTE device,int second)//select device=F C V ; F=flash light , V=VD  ,   C=CMS
{
    try
    {
        printf("power reset sec=%d\n",second);


        BYTE Send_packet[7];
        char cTMP[256];



        Send_packet[0]='$';//STX
 /*       if((device!='C')&&(device!='F')&&(device!='V'))
        {
            printf("The device name is not correct!\n");
            Send_packet[1]=0x0;
        }
        else*/
         Send_packet[1]=device;
        Send_packet[2]=',';//interval
        Send_packet[3]=0x30+(second/10);//Reset time 0~9
        Send_packet[4]=0x30+(second%10);//0~9
        Send_packet[5]=0x0d;//CR
        Send_packet[6]=0x0a;//LF
        char device_name[10];
        sprintf(device_name,"%C",device);
        sprintf(cTMP, "Device %C, second=%d", device, second);
        smem.vWriteMsgToDOM(cTMP);
      //  power_control_port.Rs232Write(Send_packet,7,device_name);
       //  writeJob.WritePhysicalOut(Send_packet,7,DEVICETRAFFICLIGHT);

        printf("power_reset %C %d\n",device,second);
         smem.lightPort.Rs232Write(Send_packet,7,"/dev/ttyS3");
      //   writeJob.WritePhysicalOut(Send_packet, 7, revAPP);


        return true;
    }
    catch(...)
    {

    }


}

int power_reboot_group::open_port_process(char* tty_name)
{
    try
    {
   /*     int tempmax=0;
        if (tempmax=power_control_port.OpenRs232Port(tty_name, 9600, false)>0)
        {
            printf("open power_control_port-%s  Success!!\n",tty_name);
        }
        else printf("open power_control_port Fail!!\n");

        return tempmax;*/
    }
    catch(...) {}
}
/*
bool power_reboot_group::timer_reboot_create()
{
    try
    {
        FILE *pf=NULL;


        int clock_info[2];
        time_t time_reboot=time(NULL);
        struct tm* clock_time;
        clock_time=localtime(&time_reboot);
        int now_second=0;

        int clock_sec=0;
        int a;

        initial_timer(power_timer);
        struct sigevent _sigp;

        _sigp=event_fillvalue(power_signal_timer,700);;

        power_timer.event=event_fillvalue(power_signal_timer,700);
        if(power_timer.timer)timer_delete(power_timer.timer);



        if(a=timer_create(CLOCK_REALTIME,&(power_timer.event),&(power_timer.timer)))
        {
            printf("%d\n",a);
            printf("power_reboot_group have some problem\n");
            printf("%d\n",power_timer.timer);
            smem.vWriteMsgToDOM("power_reboot timer() setting error\n");
            //exit(1);
        }



        if(pf=fopen("/cct/Data/SETTING/power_timer.txt","r"))
        {
            fread(&clock_info,sizeof(clock_info),1,pf);
            fclose(pf);
            int setting_sec=clock_info[0]*3600+clock_info[1]*60;
            now_second=(clock_time->tm_hour)*3600+(clock_time->tm_min)*60;


            if(now_second>setting_sec)clock_sec=now_second-setting_sec;
            else clock_sec=setting_sec-now_second;
            power_timer._itimerspec=timer_spec_fillvalue(clock_sec,0,86400,0);
        }
        else power_timer._itimerspec=timer_spec_fillvalue(0,0,0,0);

        if ( timer_settime( power_timer.timer, 0, & (power_timer._itimerspec), NULL ) ) exit( 1 );






        return true;
    }
    catch(...) {}
}

bool power_reboot_group::power_regular_reboot_setting(int hour,int minute)
{
    try
    {

        if((hour>=0)&&(hour<=23)&&(minute>=0)&&(minute<=59))
        {
            FILE *pf=NULL;
            int clock_info[2];
            time_t time_reboot=time(NULL);
            struct tm* clock_time;
            clock_time=localtime(&time_reboot);
            clock_time->tm_hour=clock_info[0]=hour;
            clock_time->tm_min=clock_info[1]=minute;
            int now_second=0;
            int setting_sec=hour*3600+minute*60;
            int clock_sec=0;

            if(pf=fopen("/cct/Data/SETTING/power_timer.txt","w+"))
            {
                fwrite(&clock_info,sizeof(clock_info),1,pf);
                fclose(pf);
            }

            now_second=(clock_time->tm_hour)*3600+(clock_time->tm_min)*60;

            if(now_second>setting_sec)clock_sec=now_second-setting_sec;
            else clock_sec=setting_sec-now_second;

            power_timer._itimerspec=timer_spec_fillvalue(clock_sec,0,86400,0);
            if ( timer_settime( power_timer.timer, 0, & power_timer._itimerspec, NULL ) ) exit( 1 );


            return true;
        }
        else
        {
            smem.vWriteMsgToDOM("power_regular_reboot_setting value error");
            return false;
        }

    }
    catch(...) {}

}*/
bool power_reboot_group::report_power_reboot()
{
    try
    {

        FILE *pf=NULL;


        int clock_info[2];

        if(pf=fopen("/cct/Data/SETTING/power_timer.txt","r"))
        {
            fread(&clock_info,sizeof(clock_info),1,pf);
            fclose(pf);
        }else{memset(clock_info,'0',sizeof(clock_info));}



        BYTE data[4];
        data[0]=0x0f;
        data[1]=0x5b;
        data[2]=clock_info[0];
        data[3]=clock_info[1];

        writeJob.WritePhysicalOut(data,4,revAPP);
        for(int i=0;i<2;i++)
        printf("clock_info=%d",clock_info[i]);
        printf("\n");




    }
    catch(...) {}
}

bool power_reboot_group::report_power_reboot_to_center()
{
    try
    {

        FILE *pf=NULL;


        int clock_info[2];

        if(pf=fopen("/cct/Data/SETTING/power_timer.txt","r"))
        {
            fread(&clock_info,sizeof(clock_info),1,pf);
            fclose(pf);
        }



        BYTE data[4];
        data[0]=0x0f;
        data[1]=0x5b;
        data[2]=clock_info[0];
        data[3]=clock_info[1];

        MESSAGEOK _MsgOK;

        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,true);
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);


    }
    catch(...) {}
}

/*


bool power_reboot_group::power_regular_reboot_resetting()
{
    try
    {
        FILE *pf=NULL;

        power_timer.event=event_fillvalue(power_signal_timer,700);
        int clock_info[2];
        time_t time_reboot=time(NULL);
        struct tm* clock_time;
        clock_time=localtime(&time_reboot);
        int now_second=0;

        int clock_sec=0;

        if(pf=fopen("/cct/Data/SETTING/power_timer.txt","r"))
        {
            fread(&clock_info,sizeof(clock_info),1,pf);
            fclose(pf);
            int setting_sec=clock_info[0]*3600+clock_info[1]*60;
            now_second=(clock_time->tm_hour)*3600+(clock_time->tm_min)*60;


            if(now_second>setting_sec)clock_sec=now_second-setting_sec;
            else clock_sec=setting_sec-now_second;
            power_timer._itimerspec=timer_spec_fillvalue(clock_sec,0,86400,0);
        }
        else power_timer._itimerspec=timer_spec_fillvalue(0,0,0,0);

        if ( timer_settime( power_timer.timer, 0, & power_timer._itimerspec, NULL ) ) exit( 1 );

    }
    catch(...) {}

}


sigevent power_reboot_group::event_fillvalue(int case_no,int timer_signal)
{
    sigevent fillobject;
    fillobject.sigev_notify = SIGEV_SIGNAL;
    fillobject.sigev_signo = timer_signal;
    fillobject.sigev_value.sival_int = case_no;
    return fillobject;

}
itimerspec power_reboot_group::timer_spec_fillvalue(int start_sec,float start_nsec,int interval_sec,int interval_nsec)
{
    itimerspec fillobject;
    fillobject.it_value.tv_sec=start_sec;
    fillobject.it_value.tv_nsec=start_nsec;
    fillobject.it_interval.tv_sec=interval_sec;
    fillobject.it_interval.tv_nsec=interval_nsec;
    return fillobject;
}
bool power_reboot_group::initial_timer(timer_object timer)
{
    try
    {
        timer.event=event_fillvalue(0,0);

//        timer.timer_switch=false;
        timer._itimerspec=timer_spec_fillvalue(0,0,0,0);
    }
    catch(...) {}
}
*/

bool power_reboot_group::power_reset_all(int second)
{
    try
    {
        power_reset('C',second);
        power_reset('V',second);

        return true;
    }
    catch(...) {}
}


