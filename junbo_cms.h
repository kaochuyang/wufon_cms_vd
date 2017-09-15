#ifndef JUNBO_CMS_H
#define JUNBO_CMS_H

#include "var.h"
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
static pthread_mutex_t _junbo_mutex;
    junbo_cms();
    virtual ~junbo_cms();

    RS232 junbo_cms_port;
    int open_port_process(char* tty_name);


    void initial_junbo_control(char *output_tty_name);
    void junbo_cms_send(unsigned char junbo_send_packet[6]);
    void junbo_cms_receive(MESSAGEOK messageIn);
    void junbo_send_by_VD(int textID);
    void delete_record_before_15day();
    bool vClearMsg(MESSAGEOK *messageIn, unsigned short int msgID);
    bool ParseBlock(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex);
    void close_light();

    void store_color(MESSAGEOK messageIn);
    void read_color();

    void cms_test_function(int text_ID);


    void brightness_control(int bright_parameter);
    void report_light_brightness();

    void color_control(BYTE color[2][3]);
    void color_packet(MESSAGEOK messageIn);
    void light_timeout_control(int control_parameter);
    void report_light_timeout();//for app



    typedef  struct junbo_to_cms
    {
        unsigned char ID;
        unsigned char command;
        unsigned char parameter;
    };


    typedef struct module_state_struct
    {
        BYTE bit_block_ID:4;
        BYTE bit_0:1;
        BYTE bit_1:1;
        BYTE bit_2:1;
        BYTE bit_3:1;
    };


    module_state_struct module_state_object;
    void query_modual_state();
    void report_module_state_to_revapp();
    bool initial_module_state(module_state_struct *object);
    void report_module_state_to_center();




//private:
    char *tty_name;
    char input_string[25];
    char filename[80];//record file director
    unsigned char junbo_send_packet[6];

    unsigned char junbo_receive_packet[6];

    junbo_to_cms notice_car[5],flash_on,flash_off,light_off,query[4],brightness[4],light_timeout[100],color_red,color_green,color_yellow;
};

#endif // JUNBO_CMS_H
