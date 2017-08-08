#ifndef JUNBO_CMS_H
#define JUNBO_CMS_H

#include "variable.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include "RS232.h"
class junbo_cms
{
    public:
        junbo_cms();
        virtual ~junbo_cms();

    void initial_junbo_control(char *text_record_location,char *output_tty_name);
    void junbo_cms_send(unsigned char junbo_send_packet[6]);
    void junbo_cms_receive(MESSAGEOK messageIn);
    //void junbo_light_send_reference_step(unsigned short int junbo_step);


    void query_modual_state(int query_block);
    void brightness_control(int bright_parameter);
    void light_timeout_control(int control_parameter);
    void report_light_brightness();
    void report_light_timeout();//for app

    void delete_record_before_15day();

    void report_module_state_to_revapp();


    bool vClearMsg(MESSAGEOK *messageIn, unsigned short int msgID);
    bool ParseBlock(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex);

    typedef  struct junbo_to_cms
    {
        unsigned char ID;
        unsigned char command;
        unsigned char parameter;
    };

RS232 junbo_cms_port;


    private:
        char *tty_name;
        char input_string[25];
        char filename[80];//record file director
        unsigned char junbo_send_packet[6];

        unsigned char junbo_receive_packet[6];

        junbo_to_cms notice_car[4],flash_on,flash_off,light_off,query[4],brightness[4],light_timeout[11];
};

#endif // JUNBO_CMS_H
