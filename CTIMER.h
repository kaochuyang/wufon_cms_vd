#ifndef CTIMERH
#define CTIMERH
#include "power_reboot_group.h"
#include <pthread.h>
//--------------------------------------------------------------------------
class intervalTimer
{
    public:
    static void * PTime(void *);


        intervalTimer();
        void ThreadsGenerate(void);
//        bool vSSInit(int);

        bool GetDoorStatus(void);
        //DATA_Bit _DByte;
        bool vCheckSMEM(void);

        bool TimersRead_BeforeResetCMOSTime();
        bool TimersReset_AfterResetCMOSTime();


        bool vCheckScreenAndDoSomething(void);
        bool vCheckVDUpdateDBAndDoSomething(void);

        bool vCommuncationReset(int);
        bool vDBLockRequest(int);
//        bool vSend92VD_6F02(void);
        bool vAllDynamicToTODCount(unsigned short int);

        unsigned short int vGetEffectTime(void);

        unsigned short int usiGreenConflictCount;

        bool vChildChain(void);

        bool vRefreshWatchDog(void);
        unsigned short int usiWatchDogCount;
        unsigned short int usiLCX405WatchDogCount;

        bool vSendHeartBeatToLCX405(void);
        void vReportCurrentOperationMode_5F08_inCtimer(void);

bool setting_regular_reboot_0F17(int hour,int minute);
bool power_regular_reboot_resetting_for_outer();
    bool power_regular_reboot_resetting();
    bool power_regular_reboot_setting(int hour,int minute);

 static  sigevent event_fillvalue(int case_no,int timer_signal);
 static  itimerspec timer_spec_fillvalue(int start_sec,float start_nsec,int interval_sec,int interval_nsec);

static  bool timer_reboot_create();

static bool timer_reboot_create2();

static struct itimerspec p_itimerspec;
  //  struct sigevent p_event;
//void close_junbo_cms_light();//timer_settime
void set_close_light_timer(int second);
   private:






    static pthread_mutex_t _ctimer_mutex;

    static void TimersCreating(void);
    static void TimersSetting(void);
    static pthread_t thread_forVDTimer;




    static timer_t _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9,p_timer;
//OTSS++
    static timer_t _t10;                                                        // For SS XG
    static timer_t _tTrafficeLight;
    static struct itimerspec _it1, _it2, _it3, _it4, _it5, _it6, _it7, _it8, _it9;
    static struct itimerspec _it10;                                             // For SS XG
    static struct itimerspec _itTrafficeLight;

//OTSS--

};
//--------------------------------------------------------------------------
extern intervalTimer _intervalTimer;
#endif
