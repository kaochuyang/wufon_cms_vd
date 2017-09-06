#ifndef JUNLI_VD_H
#define JUNLI_VD_H
#include "RS232.h"
#include "var.h"
class junli_vd
{
    public:


        junli_vd();
        virtual ~junli_vd();

        RS232 junli_port;

        bool parse_junli(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex);
        bool vd_bool_junbo_cms();
        bool vClearMsg(MESSAGEOK *messageIn, unsigned short int msgID);
        int  open_port_process(char* tty_name);
        void junli_wrong_record(MESSAGEOK messageIn,int junli_length);
        void report_to_center_VD_alive();

        bool test_function_C_and_F();
    private:
        int alive_count;
        bool vd_alive_mark;

};

#endif // JUNLI_VD_H
