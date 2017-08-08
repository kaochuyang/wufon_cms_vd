#ifndef JUNLI_VD_H
#define JUNLI_VD_H

#include "variable.h"
class junli_vd
{
    public:
        junli_vd();
        virtual ~junli_vd();
        bool parse_junli(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex);
        bool vd_bool_junbo_cms();
        bool vClearMsg(MESSAGEOK *messageIn, unsigned short int msgID);

        void junli_wrong_record(MESSAGEOK messageIn,int junli_length);
    private:
};

#endif // JUNLI_VD_H
