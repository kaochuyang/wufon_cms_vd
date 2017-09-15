#include "CTIMER.h"
#include "var.h"
#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "SMEM.h"
#include "DIGITALIO.h"

#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "SCREENModifyDT.h"
#include "SCREENNone.h"
#include "SCREENOnline.h"
#include "SCREENCommStat.h"
#include "SCREENTotalRun.h"
#include "screenCurrentLightStatus.h"
#include "screenChainStatus.h"
#include "SCREENOperStat.h"
#include "screenActuateArwenStatus.h"
#include "screenReverseMenu.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>
#include <signal.h>
#include <time.h>

#include "CSTC.h"
//--------------------------------------------------------------------------
intervalTimer _intervalTimer;

pthread_t intervalTimer::thread_forVDTimer;

timer_t intervalTimer::p_timer;
timer_t intervalTimer::_t1;
timer_t intervalTimer::_t2;
timer_t intervalTimer::_t3;
timer_t intervalTimer::_t4;
timer_t intervalTimer::_t5;
timer_t intervalTimer::_t6;
timer_t intervalTimer::_t7;
timer_t intervalTimer::_t8;
timer_t intervalTimer::_t9;
//OTSS
timer_t intervalTimer::_t10;
timer_t intervalTimer::_tTrafficeLight;



struct itimerspec intervalTimer::p_itimerspec;
struct itimerspec intervalTimer::_it1;
struct itimerspec intervalTimer::_it2;
struct itimerspec intervalTimer::_it3;
struct itimerspec intervalTimer::_it4;
struct itimerspec intervalTimer::_it5;
struct itimerspec intervalTimer::_it6;
struct itimerspec intervalTimer::_it7;
struct itimerspec intervalTimer::_it8;
struct itimerspec intervalTimer::_it9;
//OTSS
struct itimerspec intervalTimer::_it10;
struct itimerspec intervalTimer::_itTrafficeLight;
//struct timer_object power_reboot_group::power_timer;

pthread_mutex_t intervalTimer::_ctimer_mutex = PTHREAD_MUTEX_INITIALIZER;




//intervalTimer-------------------------------------------------------------
intervalTimer::intervalTimer()
{
//  iS0Count = 0;
//  iSKCount = 0;
    usiGreenConflictCount = 0;
}
//--------------------------------------------------------------------------
void intervalTimer::ThreadsGenerate(void)
{
    try
    {
        //OTMARKPRINTF  printf("\nMAIN:                 pid=%d\n",getpid());

        /*
            sigset_t main_mask;
            sigfillset( & main_mask ); //block all signal, Ctrl+C not workable
            sigprocmask( SIG_SETMASK, & main_mask, NULL );
          //OTMARKPRINTF  printf("CSTC::CSTC(): blocking all signals in main!!\n");
        */

        printf("CTimer Starting\n");

        pthread_attr_t attr;
        pthread_attr_init( & attr );
        pthread_attr_setdetachstate( & attr, PTHREAD_CREATE_DETACHED );

        pthread_create( &(intervalTimer::thread_forVDTimer) , & attr, &(intervalTimer::PTime), NULL );

        pthread_attr_destroy( & attr );
    }
    catch (...) {}
}
bool intervalTimer::timer_reboot_create2()
{
    try
    {
             _it4.it_value.tv_sec = 0;
        _it4.it_value.tv_nsec = 0;
        _it4.it_interval.tv_sec = 0;
        _it4.it_interval.tv_nsec = 0;
        if ( timer_settime( _t4, 0, & _it4, NULL ) ) exit( 1 );
    }catch(...){}
}
bool intervalTimer::timer_reboot_create()
{
    try
    {
        FILE *pf=NULL;
        struct sigevent p_event;

        int clock_info[2];
        time_t time_reboot=time(NULL);
        struct tm* clock_time;
        clock_time=localtime(&time_reboot);
        int now_second=0;

        int clock_sec=0;
        int a;


        p_event=event_fillvalue(power_signal_timer,700);
     //   if(p_timer)timer_delete(p_timer);



   /*     if(a=timer_create(CLOCK_REALTIME,&(p_event),&(p_timer)))
        {
            printf("%d\n",a);
            printf("power_reboot_group have some problem\n");
            printf("%d\n",p_timer);
            smem.vWriteMsgToDOM("power_reboot timer() setting error\n");
            exit(1);
        }else printf("p_timer create ok!!");
*/


        if(pf=fopen("/cct/Data/SETTING/power_timer.txt","r"))
        {
            fread(&clock_info,sizeof(clock_info),1,pf);
            fclose(pf);
            int setting_sec=clock_info[0]*3600+clock_info[1]*60;
            now_second=(clock_time->tm_hour)*3600+(clock_time->tm_min)*60;


            if(now_second>setting_sec)clock_sec=now_second-setting_sec;
            else clock_sec=setting_sec-now_second;
            p_itimerspec=timer_spec_fillvalue(clock_sec,0,86400,0);
        }
        else p_itimerspec=timer_spec_fillvalue(0,0,0,0);

        if ( timer_settime( _t4, 0, & (p_itimerspec), NULL ) ) exit( 1 );



printf("timer_reboot_create test \n");

        return true;
    }
    catch(...) {}
}
void intervalTimer::set_close_light_timer(int second)
{
try
{

   _it5.it_value.tv_sec = second-1;
        _it5.it_value.tv_nsec = 0;
        _it5.it_interval.tv_sec = 0;
        _it5.it_interval.tv_nsec = 0;
        if ( timer_settime( _t5, 0, & _it5, NULL ) ) exit( 1 );


}catch(...){}

}
/*
void intervalTimer::close_junbo_cms_light()
{try
{
smem.junbo_object.close_light();
   _it5.it_value.tv_sec = 0;
        _it5.it_value.tv_nsec = 0;
        _it5.it_interval.tv_sec = 0;
        _it5.it_interval.tv_nsec = 0;
        if ( timer_settime( _t5, 0, & _it5, NULL ) ) exit( 1 );


}catch(...){}
}*/

bool intervalTimer::setting_regular_reboot_0F17(int hour,int minute)
{
    try
    {return power_regular_reboot_setting(hour,minute);
    }catch(...){}
}
bool intervalTimer::power_regular_reboot_setting(int hour,int minute)
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

            p_itimerspec=timer_spec_fillvalue(clock_sec,0,86400,0);
            if ( timer_settime( _t4, 0, & p_itimerspec, NULL ) ) exit( 1 );


            return true;
        }
        else
        {
            smem.vWriteMsgToDOM("power_regular_reboot_setting value error");
            return false;
        }

    }
    catch(...) {}

}

bool intervalTimer::power_regular_reboot_resetting_for_outer()
{
    try{power_regular_reboot_resetting();  return true; }catch(...){}
}
bool intervalTimer::power_regular_reboot_resetting()
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

        if(pf=fopen("/cct/Data/SETTING/power_timer.txt","r"))
        {
            fread(&clock_info,sizeof(clock_info),1,pf);
            fclose(pf);
            int setting_sec=clock_info[0]*3600+clock_info[1]*60;
            now_second=(clock_time->tm_hour)*3600+(clock_time->tm_min)*60;


            if(now_second>setting_sec)clock_sec=now_second-setting_sec;
            else clock_sec=setting_sec-now_second;
            p_itimerspec=timer_spec_fillvalue(clock_sec,0,86400,0);
        }
        else p_itimerspec=timer_spec_fillvalue(0,0,0,0);

        if ( timer_settime( _t4, 0, & p_itimerspec, NULL ) ) exit( 1 );

    }
    catch(...) {}

}


sigevent intervalTimer::event_fillvalue(int case_no,int timer_signal)
{
    sigevent fillobject;
    fillobject.sigev_notify = SIGEV_SIGNAL;
    fillobject.sigev_signo = timer_signal;
    fillobject.sigev_value.sival_int = case_no;
    printf("case_no=%d\n",fillobject.sigev_value.sival_int);
    printf("timer_signal=%d\n",fillobject.sigev_signo);
    return fillobject;

}
itimerspec intervalTimer::timer_spec_fillvalue(int start_sec,float start_nsec,int interval_sec,int interval_nsec)
{
    itimerspec fillobject;
    fillobject.it_value.tv_sec=start_sec;
    fillobject.it_value.tv_nsec=start_nsec;
    fillobject.it_interval.tv_sec=interval_sec;
    fillobject.it_interval.tv_nsec=interval_nsec;
    printf("start_sec=%d\n",fillobject.it_value.tv_sec);
    printf("interval_sec=%d\n",fillobject.it_interval.tv_sec);
    return fillobject;
}

//--------------------------------------------------------------------------
//**********************************************************
//****         Creating and Setting Timers
//**********************************************************
//--------------------------------------------------------------------------
void intervalTimer::TimersCreating(void)
{
    try
    {

        struct sigevent _sig1, _sig2, _sig3, _sig4, _sig5, _sig6, _sig7, _sig8, _sig9;
        //OTSS
        struct sigevent _sig10;

        struct sigevent _sigTrafficeLight;


        _sig1.sigev_notify = SIGEV_SIGNAL;
        _sig1.sigev_signo = RTSIGNAL_Timer;
        _sig1.sigev_value.sival_int = 10;
        if ( timer_create( CLOCK_REALTIME, & _sig1, & _t1 ) ) exit( 1 );

        _sig2.sigev_notify = SIGEV_SIGNAL;
        _sig2.sigev_signo = RTSIGNAL_Timer;
        _sig2.sigev_value.sival_int = 11;
        if ( timer_create( CLOCK_REALTIME, & _sig2, & _t2 ) ) exit( 1 );

        _sig3.sigev_notify = SIGEV_SIGNAL;
        _sig3.sigev_signo = RTSIGNAL_Timer;
        _sig3.sigev_value.sival_int = 12;
        if ( timer_create( CLOCK_REALTIME, & _sig3, & _t3 ) ) exit( 1 );

        _sig4.sigev_notify = SIGEV_SIGNAL;
        _sig4.sigev_signo = RTSIGNAL_Timer;
        _sig4.sigev_value.sival_int = 13;
        if ( timer_create( CLOCK_REALTIME, & _sig4, & _t4 ) ) exit( 1 );

        _sig5.sigev_notify = SIGEV_SIGNAL;
        _sig5.sigev_signo = RTSIGNAL_Timer;
        _sig5.sigev_value.sival_int = 14;
        if ( timer_create( CLOCK_REALTIME, & _sig5, & _t5 ) ) exit( 1 );

        _sig6.sigev_notify = SIGEV_SIGNAL;
        _sig6.sigev_signo = RTSIGNAL_Timer;
        _sig6.sigev_value.sival_int = 15;
        if ( timer_create( CLOCK_REALTIME, & _sig6, & _t6 ) ) exit( 1 );

        _sig7.sigev_notify = SIGEV_SIGNAL;
        _sig7.sigev_signo = RTSIGNAL_Timer;
        _sig7.sigev_value.sival_int = 100;
        if ( timer_create( CLOCK_REALTIME, & _sig7, & _t7 ) ) exit( 1 );

        _sig8.sigev_notify = SIGEV_SIGNAL;
        _sig8.sigev_signo = RTSIGNAL_Timer;
        _sig8.sigev_value.sival_int = 101;
        if ( timer_create( CLOCK_REALTIME, & _sig8, & _t8 ) ) exit( 1 );

        _sig9.sigev_notify = SIGEV_SIGNAL;
        _sig9.sigev_signo = RTSIGNAL_Timer;
        _sig9.sigev_value.sival_int = 500;
        if ( timer_create( CLOCK_REALTIME, & _sig9, & _t9 ) ) exit( 1 );

        _sig10.sigev_notify = SIGEV_SIGNAL;
        _sig10.sigev_signo = RTSIGNAL_Timer;
        _sig10.sigev_value.sival_int = 501;
        if ( timer_create( CLOCK_REALTIME, & _sig10, & _t10 ) ) exit( 1 );

        _sigTrafficeLight.sigev_notify = SIGEV_SIGNAL;
        _sigTrafficeLight.sigev_signo = RTSIGNAL_Timer;
        _sigTrafficeLight.sigev_value.sival_int = 600;
        if ( timer_create( CLOCK_REALTIME, & _sigTrafficeLight, & _tTrafficeLight ) ) exit( 1 );

        printf("CTimer Create OK!\n");


    }
    catch (...) {}
}
//--------------------------------------------------------------------------
void intervalTimer::TimersSetting(void)
{
    try
    {
        unsigned char ucTmp;
        int iTmp;

        _it1.it_value.tv_sec = 1;
        _it1.it_value.tv_nsec = 0;
        _it1.it_interval.tv_sec = 1;
        _it1.it_interval.tv_nsec = 0;
        if ( timer_settime( _t1, 0, & _it1, NULL ) ) exit( 1 );

        _it2.it_value.tv_sec = 30;
        _it2.it_value.tv_nsec = 0;
        _it2.it_interval.tv_sec = 1;
        _it2.it_interval.tv_nsec = 0;
        if ( timer_settime( _t2, 0, & _it2, NULL ) ) exit( 1 );

        _it3.it_value.tv_sec = 5;
        _it3.it_value.tv_nsec = 0;
        _it3.it_interval.tv_sec = 8;
        _it3.it_interval.tv_nsec = 0;
        if ( timer_settime( _t3, 0, & _it3, NULL ) ) exit( 1 );

        _it4.it_value.tv_sec = 0;
        _it4.it_value.tv_nsec = 0;
        _it4.it_interval.tv_sec = 0;
        _it4.it_interval.tv_nsec = 0;
        if ( timer_settime( _t4, 0, & _it4, NULL ) ) exit( 1 );

        _it5.it_value.tv_sec = 0;
        _it5.it_value.tv_nsec = 0;
        _it5.it_interval.tv_sec = 0;
        _it5.it_interval.tv_nsec = 0;
        if ( timer_settime( _t5, 0, & _it5, NULL ) ) exit( 1 );

        ucTmp = smem.vGetHWCycleCodeFor_0F14_0FC4();
        switch (ucTmp)
        {
        case(0):
            iTmp = 0;
            break;
        case(1):
            iTmp = 1;
            break;
        case(2):
            iTmp = 2;
            break;
        case(3):
            iTmp = 5;
            break;
        case(4):
            iTmp = 60;
            break;
        case(5):
            iTmp = 300;
            break;
        }
        _it6.it_value.tv_sec = iTmp;
        _it6.it_value.tv_nsec = 0;
        _it6.it_interval.tv_sec = iTmp;
        _it6.it_interval.tv_nsec = 0;
        if ( timer_settime( _t6, 0, & _it6, NULL ) ) exit( 1 );

        _it7.it_value.tv_sec = 2;
        _it7.it_value.tv_nsec = 0;
        _it7.it_interval.tv_sec = 2;
        _it7.it_interval.tv_nsec = 0;
        if ( timer_settime( _t7, 0, & _it7, NULL ) ) exit( 1 );

        _it8.it_value.tv_sec = 0;
        _it8.it_value.tv_nsec = 0;
        _it8.it_interval.tv_sec = 0;
        _it8.it_interval.tv_nsec = 0;
        if ( timer_settime( _t8, 0, & _it8, NULL ) ) exit( 1 );


        _it10.it_value.tv_sec = 60;                                                 //W77E58, only one
        _it10.it_value.tv_nsec = 0;
        _it10.it_interval.tv_sec = 60;
        _it10.it_interval.tv_nsec = 0;                                  // 1/10 sec
        if ( timer_settime( _t10, 0, & _it10, NULL ) ) exit( 1 );

        _itTrafficeLight.it_value.tv_sec = 0;
        _itTrafficeLight.it_value.tv_nsec = 200000000;  //0.2 sec
        _itTrafficeLight.it_interval.tv_sec = 2;
        _itTrafficeLight.it_interval.tv_nsec = 0;
        if ( timer_settime( _tTrafficeLight, 0, & _itTrafficeLight, NULL ) ) exit( 1 );

        printf("CTimer Set OK!\n");


    }
    catch (...) {}
}
//--------------------------------------------------------------------------
void * intervalTimer::PTime(void *arg)
{
    try
    {
        // smem.power_object.power_regular_reboot_resetting();
        int text_ID=1;
        int tempFace=0;

        unsigned char uc6F00[2];
        uc6F00[0] = 0x6F;
        uc6F00[1] = 0x00;

        unsigned char uc0F04[4];
        uc0F04[0] = 0x0F;
        uc0F04[1] = 0x04;
        MESSAGEOK _MSG;

        DATA_Bit DIOByte;

        //OTSS +3
        unsigned char ucXG[4];
        ucXG[0] = 0x58;       // X
        ucXG[1] = 0x47;       // G
        ucXG[2] = 0x0D;       // CR

        //OTMARKPRINTF  printf( "THREAD_VDINFO: pid=%d\n", getpid() );



        TimersCreating();

        TimersSetting();
        timer_reboot_create();
        smem.light_time.read_time();
        smem.light_time.read_textID();
      smem.junbo_object.light_timeout_control(smem.light_time.light_flash_time);
      smem.junbo_object.read_color();
        printf("hello light control\n");
     //   timer_reboot_create();//kaochu 2017 08 17

        int VDsignum = 0;
        int VDrid = 9999;

        int SSInitCount = 0;

        int iCommuncationResetCount = 0;
        int iDBLockCount = 0;

        int iLastAliveStatusCount = 0;

        int iTC92SignalLightStatusVal = 5;
        int iTransmitCycle5F0F_Count = 0;
        iTC92SignalLightStatusVal = smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
        int iTC92SignalStepStatusVal = 5;
        int iTransmitCycle5F03_Count = 0;
        iTC92SignalStepStatusVal = smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);

        time_t _RunSec, _SSLastRun;

        //BT9512260001 START
        char msg[256];
        memset(msg,0,sizeof(msg));
        //BT9512260001 END


        sigset_t Timer_mask;
        sigfillset(& Timer_mask);
        sigprocmask( SIG_SETMASK, & Timer_mask, NULL);
        sigemptyset( & Timer_mask ); //unblock all signal
        sigaddset( & Timer_mask, RTSIGNAL_Timer );    //block VD_SIGNAL
        sigaddset( & Timer_mask, RTSIGNAL_RECODE_SMEM );    //block VD_SIGNAL
        sigaddset(&Timer_mask,power_signal_timer);


        siginfo_t VDTimer_siginfo;
        unsigned short int usiTmp;
        bool bConnectCenterStatus;
        DATA_Bit _ControlStrategy;

        unsigned short int usiCurrentSubphaseStep;
        unsigned short int usiCurrentSubphaseStepN;
        bool bStepChangeOK;

        int iTmp;
        unsigned char ucCounterWDTForLightboardRedcount;

//OT20110526
        int iMinCycleTime;
        unsigned char ucTmp;
        time_t currentTime;
        struct tm *now;
        time_t tmpTime;

        //jacky20151210
        unsigned char RequestKeypad = 0;

        int reset_count=0;//kaochu 20170824
        BYTE Send_packet[2];

        while(1)
        {
            while(smem.vGetTimerMutexRESET() == 1)
            {
//          printf("timer set, ctimer wait!\n");
                usleep(100);
            }
            smem.vSetTimerMutexCTIMER(1);

            VDsignum = sigwaitinfo( & Timer_mask, & VDTimer_siginfo );

            switch ( VDsignum )
            {
            case(power_signal_timer):
                VDrid = VDTimer_siginfo.si_value.sival_int;

                switch (VDrid)
                {
                case (700):
                    smem.power_object.power_reset('C',30);
                    smem.power_object.power_reset('V',30);
                    smem.vWriteMsgToDOM("reboot VD and CMS\n");

                    sleep(30);
                    smem.vWriteMsgToDOM("daily reboot");
                    break;
                default:
                    break;
                }

            case( RTSIGNAL_Timer ):
                VDrid = VDTimer_siginfo.si_value.sival_int;

                switch (VDrid)
                {

                case( 10 ):
                    //WatchDog

                    //OT20110526
                    currentTime = time(NULL);
                    now = localtime(&currentTime);
                    /****************************************************************************/
                    _intervalTimer.vRefreshWatchDog();

                    smem.vSet_DIOByte(digitalIO.vReadDigitalIO());

                    _intervalTimer.GetDoorStatus();
                    /*********************************************************************/
                    _intervalTimer.vCheckSMEM();

                    tempFace=smem.GetcFace();

                    if (tempFace==cMAIN) screenMain.DisplayDateTime();
                    else if (tempFace==cCTLSETUP) screenCtlSetup.DisplayDateTime();
                    else if (tempFace==cMODIFYDT) screenModifyDT.DisplayCurrentDateTime();
                    else if (tempFace==cCOMMSTAT) screenCommStat.UpdateComm();
                    else if (tempFace==cTOTALRUN) screenTotalRun.DisplayTotalTime();
                    else if (tempFace==cCURRENTLIGHTSTATUS)
                    {
                        screenCurrentLightStatus.vRefreshStepSec();
                        screenCurrentLightStatus.DisplayDynSegStatus();
                    }
                    else if (tempFace==cCHAINSTATUS) screenChainStatus.vRefreshChainStatusData();
                    else if (tempFace == cOPERSTAT) screenOperStat.vShowGreenConflict();
                    else if (tempFace == cREVERSETIMTMENU) screenReverseMenu.vRefreshStepSec();
//                                  else if (tempFace == cACTUATEARWENSTATUS) screenActuateArwenStatus.vRefreshEverySec();
                    screenActuateArwenStatus.vRefreshEverySec();


                    if(iLastAliveStatusCount > 300)
                    {
                        smem.vSaveLastAliveStatus();
                        iLastAliveStatusCount = 0;
                    }
                    iLastAliveStatusCount++;

                    smem.vSetSystemTime(currentTime);


                    if(smem.GetRequestKeypad() == 1)
                    {
                        if(RequestKeypad < 5)
                        {
                            SendRequestToKeypad();
                            RequestKeypad++;
                            printf("10 !!!\n");
                        }
                        else if(RequestKeypad >= 5)
                        {
                            smem.SetRequestKeypad(0);
                            RequestKeypad = 0;
                            stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);
                        }
                    }
                    else if(smem.GetRequestKeypad() == 0)
                    {
                        RequestKeypad = 0;
                    }

                    break;
                case( 11 ):
//Remove  _intervalTimer.vCheckAndReSendSS_S0_Status();
                    smem.vCheckConnectStatus();

                    //OT20110526
                    iMinCycleTime = 600;

                    ucTmp = smem.vGetHWCycleCodeFor_0F14_0FC4();
                    switch (ucTmp)
                    {
                    case(0):  //means stop
                        iTmp = 600;
                        break;
                    case(1):
                        iTmp = 1;
                        break;
                    case(2):
                        iTmp = 2;
                        break;
                    case(3):
                        iTmp = 5;
                        break;
                    case(4):
                        iTmp = 60;
                        break;
                    case(5):
                        iTmp = 300;
                        break;
                    }
                    if(iTmp > 0 && iTmp < iMinCycleTime)
                    {
                        iMinCycleTime = iTmp;
                    }

                    iTmp = smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
                    if(iTmp == 0xFF)    //stop send
                    {
                        iTmp = 600;
                    }
                    if(iTmp > 0 && iTmp < iMinCycleTime)
                    {
                        iMinCycleTime = iTmp;
                    }

                    iTmp = smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);
                    if(iTmp == 0xFF)    //stop send
                    {
                        iTmp = 600;
                    }
                    if(iTmp > 0 && iTmp < iMinCycleTime)
                    {
                        iMinCycleTime = iTmp;
                    }
                    iMinCycleTime += 2;  //set delay time.

                    tmpTime = smem.vGetLastGetProtocolTime();
                    if(currentTime >= tmpTime && (currentTime - tmpTime) <= iMinCycleTime)
                    {
                        smem.vSaveCenterConnectStatus(true);
                    }
                    else
                    {
                        smem.vSaveCenterConnectStatus(false);
                    }

                    //OT20110825
                    tmpTime = smem.vGetLastTrafficeLightTime();
//                                  printf("currentTime - tmpTime:%d\n", currentTime - tmpTime);
                    if(tmpTime >= currentTime)    //log time > system time
                    {
                        smem.vSetTrafficLightBoardConnectStatus(true);
                    }
                    else
                    {
                        if( (currentTime - tmpTime) <= 60)
                        {
                            smem.vSetTrafficLightBoardConnectStatus(true);
                        }
                        else
                        {
                            smem.vSetTrafficLightBoardConnectStatus(false);
                        }
                    }


                    break;
                case( 12 ):
//Remove                                   _intervalTimer.vCheckAndReSendSS_SK_Status();
              //      _intervalTimer.vCheckScreenAndDoSomething();

//Remove                                    _SSLastRun = smem.vGetSSCommTime();         //æª¢æ\uFFFDSmartSensor
                 //   _RunSec = time(NULL);

                //    _intervalTimer.vCheckVDUpdateDBAndDoSomething();

                //    if((_RunSec - smem.GetLastKeypadTime()) > 300)    //TEST, 990325 for amegids, don't clear screen
                  //  {
             //           screenNone.DisplayNone();
               //     }

//                                    if (smem.GetKeypadReturn()==false)  SendRequestToKeypad();
//                    SendRequestToKeypad();
//printf("12  !!+n" );
                    break;
                case( 13 ):                                                           //VD SIM
printf("timer test 13\n");

                case( 14 ):

printf("timer test 14\n");
//_intervalTimer.close_junbo_cms_light();
smem.junbo_object.close_light();
                    break;


                case( 15 ):  //0F04, HwStatus AutoReport
                    uc0F04[2] = smem.vGetHardwareStatus(3);
                    uc0F04[3] = smem.vGetHardwareStatus(4);
                    _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc0F04, 4, true);
                    _MSG.InnerOrOutWard = cOutWard;
                    writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);

                    break;

                case( 100 ):


//SendRequestToKeypad();

    /*    Send_packet[0]='$';//STX
       Send_packet[1]='F';
        Send_packet[2]=',';//interval
        Send_packet[3]=0x30+0;//Reset time 0~9
        Send_packet[4]=0x30+5;//0~9
        Send_packet[5]=0x0d;//CR
        Send_packet[6]=0x0a;//LF
printf("hello F\n");

      //  power_control_port.Rs232Write(Send_packet,7,device_name);
         writeJob.WritePhysicalOut(Send_packet,7,DEVICETRAFFICLIGHT);*/
                   /* _intervalTimer.vCommuncationReset(iCommuncationResetCount);                        //default not start
                    iCommuncationResetCount++;
                    if(iCommuncationResetCount >= 3) iCommuncationResetCount = 0;*/
                 //   smem.power_object.power_reset('F',5);//kaochu test test

                   //  text_ID++;
                    //    if(text_ID>4)text_ID=1;

                   //     printf("textID=%d  by test\n",text_ID);
                       // smem.junbo_object.junbo_send_by_VD(2);
                      //  smem.junbo_object.brightness_control(text_ID);
                    //    smem.junbo_object.query_modual_state();
                        //smem.junbo_object.light_timeout_control(text_ID);
                  //      smem.junbo_object.report_module_state_to_revapp();
                    break;

                case( 101 ):
                    _intervalTimer.vDBLockRequest(iDBLockCount);                        //default not start
                    iDBLockCount++;
                    if(iDBLockCount >= 6)                        //TimeOut
                    {
                        iDBLockCount = 0;
                        screenOnline.RequestTimeOut();
                    }
                    if(smem.GetDbOperStat()!=1 || smem.GetcFace()!=cONLINE)
                    {
                        iDBLockCount = 0;
                        _intervalTimer.vDBLockRequest(10);
                    }
                    break;


                case( 500 ):                                      //\uFFFDï¿½ï¿½\uFFFD\uFFFD\uFFFD\uFFFDTOD,

                    break;


                case( 501 ):

                if(reset_count<8)
                {

                    smem.count_vd_alive++;

                    if(smem.count_vd_alive>30)
                    {   printf("count_vd_alive=%d\n",smem.count_vd_alive);
                        smem.power_object.power_reset_all(10);
                        smem.count_vd_alive=0;
                        smem.vWriteMsgToDOM("VD link error");
                        Send_packet[0]=0x6f;
                        Send_packet[1]=0xff;
                        smem._0F80_packet(Send_packet);
                        reset_count++;
                    }

                }
                    break;

                case( 600 ):

                    break;

                default:
                    perror("CSTC TIMERID: error!!");
                    //OTMARKPRINTF  printf("VDrid: \n", VDrid);
                }
                break;


            case( RTSIGNAL_RECODE_SMEM ):
                smem.vSaveShareMemoryDataToDOM();
                break;

            default:
                break;

            }
            smem.vSetTimerMutexCTIMER(0);

        }
    }
    catch (...) {}
}
//--------------------------------------------------------------------------
bool intervalTimer::GetDoorStatus(void)
{
    try
    {

        unsigned char uc0F04[4];
        bool bShouldSendMsg = false;
        bool bCmpRet;

        DATA_Bit tempDIOByte=smem.Get_DIOByte();
        DATA_Bit tempOldDIOByte = smem.Get_OldDIOByte();

        smem.vSetUCData(TC_GreenConflictDetFromDIO, tempDIOByte.DBit);
        bCmpRet = smem.vCmpGreenConflict();

        if(bCmpRet == false )
        {
            usiGreenConflictCount++;
        }
        else
        {
            usiGreenConflictCount = 0;
        }

        if(usiGreenConflictCount > 30000) usiGreenConflictCount = 0;

        if(SignalConflictVersion)
        {
            if(usiGreenConflictCount > 2)
            {
//      bShouldSendMsg = true;
                smem.vSetBOOLData(TC_SignalConflictError, true);

                printf("Lock_to_Set_Control_Strategy by usiGreenConflictCount!!!\n");
                printf("Lock_to_Set_Control_Strategy by usiGreenConflictCount!!!\n");
                printf("Lock_to_Set_Control_Strategy by usiGreenConflictCount!!!\n");


                stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);
            }
        }

        if (usiGreenConflictCount == 3)
        {

            uc0F04[0] = 0x0F;
            uc0F04[1] = 0x04;
            MESSAGEOK _MSG;
            //send HW Status
            uc0F04[2] = smem.vGetHardwareStatus(3);
            uc0F04[3] = smem.vGetHardwareStatus(4);
            _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc0F04, 4, true);
            _MSG.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);

            return true;
        }

        if(smem.vGetBOOLData(TC_SignalConflictError) == true)
        {
            uc0F04[0] = 0x0F;
            uc0F04[1] = 0x04;
            MESSAGEOK _MSG;
            //send HW Status
            uc0F04[2] = smem.vGetHardwareStatus(3);
            uc0F04[3] = smem.vGetHardwareStatus(4);
            _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc0F04, 4, true);
            _MSG.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);

            return true;

        }

        return false;
    }
    catch(...) {}
}

//--------------------------------------------------------------------------
bool intervalTimer::vChildChain(void)
{
    try
    {
        static time_t now;

        static struct tm* basetime;

        static struct tm* currenttime;

        static int iChainSignalNotChange;

        sChildChain sCCTMP;
        long lCycleTime;

        bool bChildChainStatus;
        bool bChildChainStatusOld;
        unsigned char ucMBType;

        DATA_Bit tempDIOByte = smem.Get_DIOByte();
//  printf("DIO:%X\n", tempDIOByte.DBit);
        DATA_Bit tempOldDIOByte = smem.Get_OldDIOByte();

        sCCTMP = smem.vGetChildChainStruct();
        now = time(NULL);
//  printf(" now:%d\n", now);

        ucMBType = smem.vGetMotherBoardType();

        if(ucMBType == MB_ICOP6115)
        {
            bChildChainStatus = tempDIOByte.switchBit.b2;
            bChildChainStatusOld = tempOldDIOByte.switchBit.b2;
        }
        else
        {
            bChildChainStatus = tempDIOByte.switchBit.b1;
            bChildChainStatusOld = tempOldDIOByte.switchBit.b1;
        }

        if(bChildChainStatus != bChildChainStatusOld )                  //if status have change.
        {
            sCCTMP = smem.vGetChildChainStruct();
            now = time(NULL);
            printf(" now:%d\n", now);
            currenttime = localtime(&now);
            if(bChildChainStatus == TC_CHAIN_SIGNAL_IN_START_SUB_PAHSE)          //get StartLock Signal
            {
                printf("GetChildChain Start Time: %2d:%2d:%2d\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
                printf("GetChildChain Start Time: %2d:%2d:%2d\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
                printf("GetChildChain Start Time: %2d:%2d:%2d\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
                sCCTMP.oldStartTime = sCCTMP.newStartTime;
                sCCTMP.newStartTime = now;

                lCycleTime = sCCTMP.newStartTime - sCCTMP.oldStartTime;
                if(sCCTMP.oldStartTime != 0 && lCycleTime > 0 && lCycleTime < 300)        //300 means max cycle for protect
                {
                    sCCTMP.iEndKeepTime = sCCTMP.newStartTime - sCCTMP.newEndTime;
                    sCCTMP.iChainCycle = lCycleTime;
                }
                else                                                                      //error signal cycle
                {
                    sCCTMP.iEndKeepTime = 0;
                }
            }
            else                                                                        //get EndLock Signal (TC_CHAIN_SIGNAL_IN_END_SUB_PAHSE)
            {
                printf("GetChildChain End Time: %2d:%2d:%2d\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
                printf("GetChildChain End Time: %2d:%2d:%2d\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
                printf("GetChildChain End Time: %2d:%2d:%2d\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
                sCCTMP.oldEndTime = sCCTMP.newEndTime;
                sCCTMP.newEndTime = now;

                lCycleTime = sCCTMP.newEndTime - sCCTMP.oldEndTime;
                if(sCCTMP.oldEndTime != 0 && lCycleTime > 0 && lCycleTime < 300)          //300 means max cycle for protect
                {
                    sCCTMP.iStartKeepTime = sCCTMP.newEndTime - sCCTMP.newStartTime;
                    sCCTMP.iChainCycle = lCycleTime;
                }
                else                                                                      //error signal cycle
                {
                    sCCTMP.iStartKeepTime = 0;
                }

            }

            if(sCCTMP.iStartKeepTime != 0 && sCCTMP.iEndKeepTime != 0)                  //Check value reasonable
            {
                sCCTMP.bHaveReasonableChildChainSignal = true;                            //get right chain signal
                printf("GetChildChainbHaveReasonableChildChainSignal\n");
                printf("GetChildChainbHaveReasonableChildChainSignal\n");
                printf("GetChildChainbHaveReasonableChildChainSignal\n");
            }
            else
            {
                sCCTMP.bHaveReasonableChildChainSignal = false;
            }

            smem.vSetChildChainStruct(sCCTMP);
            iChainSignalNotChange = 0;

        }
        else
        {
            iChainSignalNotChange++;
        }

        if(iChainSignalNotChange > 600)
        {
            smem.vClearChildChainStruct();
        }

        return true;
    }
    catch(...) {}
}

//--------------------------------------------------------------------------
bool intervalTimer::vCheckSMEM(void)
{
    try
    {
//    if( smem.vGetSimIntervalTimeChangeStatus() ) { //for VD SimDataSend
//        _it4 = smem.vReadSimIntervalTime();

//      //OTMARKPRINTF  printf("[Message] Change Sim Send Interval:%d\n", _it4.it_interval.tv_sec);

//        if ( timer_settime( _t4, 0, & _it4, NULL ) ) exit( 1 );
//        smem.vSetSimIntervalTimeChangeStatus(false);
//    }
        if( smem.vGet0FHardwareCycleChangeStatus() )
        {
            _it6 = smem.vGet0FHardwareCycle();

            //OTMARKPRINTF  printf("[Message] Change HWStatus Send Interval:%d\n", _it6.it_interval.tv_sec);

            if ( timer_settime( _t6, 0, & _it6, NULL ) ) exit( 1 );
            smem.vSet0FHardwareCycleChangeStatus(false);
        }
        if( smem.vGet0F11CommuncationResetChangeStatus() )
        {
            _it7.it_value.tv_sec = 0;
            _it7.it_value.tv_nsec = 500;
            _it7.it_interval.tv_sec = 3;
            _it7.it_interval.tv_nsec = 0;
            //OTMARKPRINTF  printf("[Message] StartCommuncationReset!\n");
            if ( timer_settime( _t7, 0, & _it7, NULL ) ) exit( 1 );
            smem.vSet0F11CommuncationResetChangeStatus(false);
        }
        if( smem.vGet0F16LockDBRequestStatus() )
        {
            _it8.it_value.tv_sec = 1;
            _it8.it_value.tv_nsec = 0;
            _it8.it_interval.tv_sec = 30;
            _it8.it_interval.tv_nsec = 0;
            //OTMARKPRINTF  printf("[Message] Start Request LockDB!\n");
            if ( timer_settime( _t8, 0, & _it8, NULL ) ) exit( 1 );
            smem.vSet0F16LockDBRequestStatus(false);
        }

        return true;
    }
    catch(...) {}
}

//--------------------------------------------------------------------------
//**********************************************************
//      Read and Reset Timers for Resetting CMOS Time
//**********************************************************
//--------------------------------------------------------------------------
bool intervalTimer::TimersRead_BeforeResetCMOSTime(void)
{
    try
    {

        pthread_mutex_lock(&intervalTimer::_ctimer_mutex);
        struct itimerspec _itZero;

        _itZero.it_value.tv_sec = 0;
        _itZero.it_value.tv_nsec = 0;
        _itZero.it_interval.tv_sec = 0;
        _itZero.it_interval.tv_nsec = 0;

//     while(smem.vGetTimerMutexCTIMER() == true) { }
//     smem.vSetTimerMutexCTIMER(true);
        /*
          if(timer_gettime(_t1,&_it1)) return false;
          if(timer_gettime(_t2,&_it2)) return false;
          if(timer_gettime(_t3,&_it3)) return false;
          if(timer_gettime(_t4,&_it4)) return false;
          if(timer_gettime(_t5,&_it5)) return false;
          if(timer_gettime(_t6,&_it6)) return false;
          if(timer_gettime(_t7,&_it7)) return false;
          if(timer_gettime(_t8,&_it8)) return false;
        */
        if(timer_gettime(_t9,&_it9)) return false;

        timer_settime(_t1, 0, &_itZero, NULL);
        timer_settime(_t2, 0, &_itZero, NULL);
        timer_settime(_t3, 0, &_itZero, NULL);
        timer_settime(_t4, 0, &_itZero, NULL);
        timer_settime(_t5, 0, &_itZero, NULL);
        timer_settime(_t6, 0, &_itZero, NULL);
        timer_settime(_t7, 0, &_itZero, NULL);
        timer_settime(_t8, 0, &_itZero, NULL);
        timer_settime(_t9, 0, &_itZero, NULL);
        timer_settime(_t10, 0, &_itZero, NULL);
        timer_settime(_tTrafficeLight, 0, &_itZero, NULL);

        pthread_mutex_unlock(&intervalTimer::_ctimer_mutex);

        return true;
    }
    catch(...) {}
}
//--------------------------------------------------------------------------
bool intervalTimer::TimersReset_AfterResetCMOSTime(void)
{
    try
    {

        pthread_mutex_lock(&intervalTimer::_ctimer_mutex);

        for(int i = 0; i < 4; i++)
        {

            TimersSetting();
            /*
            if(timer_settime(_t1, 0, &_it1, NULL)) return false;
            if(timer_settime(_t2, 0, &_it2, NULL)) return false;
            if(timer_settime(_t3, 0, &_it3, NULL)) return false;
            if(timer_settime(_t4, 0, &_it4, NULL)) return false;
            if(timer_settime(_t5, 0, &_it5, NULL)) return false;
            if(timer_settime(_t6, 0, &_it6, NULL)) return false;
            if(timer_settime(_t7, 0, &_it7, NULL)) return false;
            if(timer_settime(_t8, 0, &_it8, NULL)) return false;
            */
            if(timer_settime(_t9, 0, &_it9, NULL)) return false;

//    smem.vSetTimerMutexCTIMER(false);

        }

        pthread_mutex_unlock(&intervalTimer::_ctimer_mutex);

        return true;
    }
    catch(...) {}
}

//-------------------------------------------------------------------------
bool intervalTimer::vCheckScreenAndDoSomething(void)
{
    try
    {

        if( smem.GetcFace() == 0)
        {
            return true;
        }
        time_t NowSec = time(NULL);
//  time_t _TmpLastPress =smem.Get???

//  if( (NowSec - _TmpLastPress > 150)
        {
            //change cfase
        }
        return true;

    }
    catch(...) {}
}
//-------------------------------------------------------------------------
bool intervalTimer::vCheckVDUpdateDBAndDoSomething(void)
{
    try
    {

//OT Debug 0410
//change for TC

        if( smem.vGetVDUpdateDB(888) )
        {
            if( smem.GetConnectCenterStatus() )
            {
                unsigned char data[4];
                data[2] = 0;
                data[2] = data[2] + smem.vGetVDUpdateDB(0);
                data[2] = data[2] + smem.vGetVDUpdateDB(1)*2;
                data[2] = data[2] + smem.vGetVDUpdateDB(2)*4;
//OT Debug 951128
                data[2] = data[2] + smem.vGetVDUpdateDB(3)*8;

                data[0] = 0x5F;
                data[1] = 0x0A;

                data[3] = 0xFF;

                MESSAGEOK _MSG;
                _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,false);
                _MSG.InnerOrOutWard = cInner;
                writeJob.WriteWorkByMESSAGEOUT(_MSG);

                for(int i = 0; i < 4; i++)
                    smem.vSetVDUpdateDB(i, false);
            }
            smem.vSetVDUpdateDB(888, false);                                          // send to center OK.

        }
        return true;
    }
    catch(...) {}
}
//--------------------------------------------------------------------------

bool intervalTimer::vCommuncationReset(int iTMP)
{
    try
    {
        unsigned char data[2];
        data[0] = 0x0F;
        data[1] = 0x91;

        switch(iTMP)
        {
        case (0):
            MESSAGEOK _MsgOK;
            _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 2,true);
            _MsgOK.InnerOrOutWard = cOutWard;
            writeJob.WriteWorkByMESSAGEOUT(_MsgOK);

            digitalIO.WriteDigitalIO(0xFF);
            break;
        case (1):
            digitalIO.WriteDigitalIO(0x00);
            smem.vSetTimeInterVal(60, INVAILDVALUE);
//             smem.vSet92VDTransmitType(1, 60);
            break;

        default:
            _it7.it_value.tv_sec = 0;
            _it7.it_value.tv_nsec = 0;
            _it7.it_interval.tv_sec = 0;
            _it7.it_interval.tv_nsec = 0;
            if ( timer_settime( _t7, 0, & _it7, NULL ) ) exit( 1 );

            break;
        }
    }
    catch(...) {}
}

//--------------------------------------------------------------------------
bool intervalTimer::vDBLockRequest(int iTMP)
{
    try
    {
        unsigned char data[2];
        data[0] = 0x0F;
        data[1] = 0x06;
        MESSAGEOK _MsgOK;

        switch(iTMP)
        {
        case (0):
        case (1):
        case (2):
        case (3):
        case (4):
            _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 2,true);
            _MsgOK.InnerOrOutWard = cOutWard;
            writeJob.WriteWorkByMESSAGEOUT(_MsgOK);

            if(smem.GetDbOperStat() != 1)
            {
                _it8.it_value.tv_sec = 0;
                _it8.it_value.tv_nsec = 0;
                _it8.it_interval.tv_sec = 0;
                _it8.it_interval.tv_nsec = 0;
                if ( timer_settime( _t8, 0, & _it8, NULL ) ) exit( 1 );
            }
            break;
        default:
            _it8.it_value.tv_sec = 0;
            _it8.it_value.tv_nsec = 0;
            _it8.it_interval.tv_sec = 0;
            _it8.it_interval.tv_nsec = 0;
            if ( timer_settime( _t8, 0, & _it8, NULL ) ) exit( 1 );

            break;
        }
    }
    catch(...) {}
}

//--------------------------------------------------------------------------
bool intervalTimer::vAllDynamicToTODCount(unsigned short int siTMP)
{
    try
    {
        _it9.it_value.tv_sec = siTMP;
        /* ot add 960802 */
        _it9.it_value.tv_nsec = 0;
        _it9.it_interval.tv_sec = 0;
        _it9.it_interval.tv_nsec = 0;

        if ( timer_settime( _t9, 0, & _it9, NULL ) ) exit( 1 );

    }
    catch(...) {}
}

//----------------------------------------------------------
unsigned short int intervalTimer::vGetEffectTime(void)
{
    try
    {

        //Should be mutex

        unsigned short int time_difference=0;
        timer_gettime(_t9,&_it9);
        time_difference = (_it9.it_value.tv_sec);

        return time_difference;

    }
    catch(...) {}
}


//--------------------------------------------------------------------------
bool intervalTimer::vRefreshWatchDog(void)
{
    try
    {
//  usiWatchDogCount++;
//  printf("usiWatchDogCount:%d\n", usiWatchDogCount);
//  if(usiWatchDogCount >= 1) {
//    usiWatchDogCount = 0;
        digitalIO.vRefresfWatchDog0x444();
//  }

    }
    catch(...) {}
}

//OT970214NEWLCX405
//--------------------------------------------------------------------------
bool intervalTimer::vSendHeartBeatToLCX405(void)
{
    try
    {

        unsigned char ucHeartBeat[22];  //OTCombo0713 SayHelloToCard
        char msg[64];

//  ++usiLCX405WatchDogCount;

//  if(usiLCX405WatchDogCount > 1) {
//    usiLCX405WatchDogCount = 0;

        ucHeartBeat[0] = 0xAA;
        ucHeartBeat[1] = 0xBB;
        ucHeartBeat[2] = 0x13;
        ucHeartBeat[3] = 0x00;
        ucHeartBeat[4] = 0x00;
        ucHeartBeat[5] = 0x00;
        ucHeartBeat[6] = 0x00;
        ucHeartBeat[7] = 0x00;
        ucHeartBeat[8] = 0x00;
        ucHeartBeat[9] = 0x00;
        ucHeartBeat[10] = 0x00;
        ucHeartBeat[11] = 0x00;
        ucHeartBeat[12] = 0x00;
        ucHeartBeat[13] = 0x12;
        ucHeartBeat[14] = 0x00;
        ucHeartBeat[15] = 0x00;
        ucHeartBeat[16] = 0x00;
        ucHeartBeat[17] = 0x00;
        ucHeartBeat[18] = 0xAA;
        ucHeartBeat[19] = 0xCC;
        ucHeartBeat[20] = 0x00;
        for (int i=0; i<20; i++)
        {
            ucHeartBeat[20] ^= ucHeartBeat[i];
        }
        writeJob.WritePhysicalOut(ucHeartBeat, 21, DEVICETRAFFICLIGHT);  //OTCombo0713

//  sprintf(msg,"SendLightHeartBeat");
//  smem.vWriteMsgToDOM(msg);


//  }

        return 0;

    }
    catch(...) {}
}

//----------------------------------------------------------
void intervalTimer::vReportCurrentOperationMode_5F08_inCtimer(void)
{
    try
    {
        unsigned short int data_length = 3;
//mallocFuck  unsigned char *data = (unsigned char *)malloc( data_length*sizeof(unsigned char) );
        unsigned char data[4];
        data[0] = 0x5F;
        data[1] = 0x08;
        data[2] = 0x00;
        data[2] = smem.vGetTC5F08Status();

        printf("[MESSAGE] Ctimer Report 5F08 Current Operation Mode to Center: %d\n", data[2]);

        /*+++++++++++++++++*/

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, data_length, true);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    }
    catch (...) {}
}



