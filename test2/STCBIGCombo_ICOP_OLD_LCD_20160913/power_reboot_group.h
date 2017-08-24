#ifndef POWER_REBOOT_GROUP_H
#define POWER_REBOOT_GROUP_H
//#include "RS232.h"
#include "var.h"
#include <time.h>
#include <signal.h>
#include <pthread.h>

class power_reboot_group
{
public:
    power_reboot_group();
    virtual ~power_reboot_group();

 //   RS232 power_control_port;
    bool power_reset(BYTE device,int second);

    bool power_reset_all(int second);
    int open_port_process(char* tty_name);


    bool report_power_reboot();
    bool report_power_reboot_to_center();


/*
    typedef struct timer_object
    {
        timer_t timer;
        struct itimerspec _itimerspec;
        sigevent event;
       // bool timer_switch;
    };

    timer_object power_timer;*/

  /*  bool timer_reboot_create();
    bool power_regular_reboot_setting(int hour,int minute);
    bool power_regular_reboot_resetting();
    bool initial_timer(timer_object timer);
    sigevent event_fillvalue(int case_no,int timer_signal);
    itimerspec timer_spec_fillvalue(int start_sec,float start_nsec,int interval_sec,int interval_nsec);
*/
private:

};

#endif // POWER_REBOOT_GROUP_H
