#ifndef JUNBO_LIGHT_CONTROL_H
#define JUNBO_LIGHT_CONTROL_H
#include "RS232.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <signal.h>
#include <pthread.h>

#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "var.h"
using namespace std;
class junbo_light_control
{
public:
    static pthread_mutex_t _junbo_mutex;
    junbo_light_control();
    ~junbo_light_control();


    void initial_junbo_control(char *text_record_location,char *output_tty_name);

    void junbo_light_send(unsigned char junbo_send_packet[6]);
    void junbo_light_receive(MESSAGEOK messageIn);
    void junbo_light_send_reference_step(unsigned short int junbo_step);


    void junbol_light_5F7A();//query
    void junbol_light_5F7B();//report for 5F7B
    void junbol_light_5F7C();//auto report
    void junbol_light_5F7D(unsigned char input_switch_button);//control junbol light by manual
    void junbol_light_5F7E();//report for 5F7D

    void query_modual_state();
    void brightness_control(int bright_parameter);
    void light_timeout_control(int control_parameter);
    void report_light_brightness();
    void report_light_timeout();
    void report_module_state_to_revapp();

    MESSAGEOK revAPP_packet(int readSelectLength,BYTE block[1024]);

    RS232 REV_light_port;

    itimerspec timer_spec_fillvalue(int start_sec,float start_nsec,int interval_sec,int interval_nsec);
    sigevent event_fillvalue(int case_no);

    void junbo_manual_timer_group();
    struct itimerspec spec_junbo_manual;
    timer_t timer_junbo_manual;
    unsigned char junbo_switch_button;

    unsigned char default_dir_mark;

    typedef  struct junbo_to_light
    {
        unsigned char command;
        unsigned char parameter;
    };
    bool vClearMsg(MESSAGEOK *messageIn, unsigned short int msgID);
    bool ParseBlock(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex);

private:

    junbo_to_light record_light[9];

    junbo_to_light light_ID[9];
    int count_for_receive;

    unsigned char junbo_send_packet[6];
    unsigned char junbo_receive_packet[6];

    unsigned char cks_by_check;

    char input_string[25];

    char filename[80];//record file director


    char *tty_name;
    junbo_to_light green,red,flash_on,flash_off,light_off,query,green_right,brightness[4],light_timeout[11];

};


#endif // JUNBO_LIGHT_CONTROL_H
