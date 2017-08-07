#ifndef JUNLI_VD_H
#define JUNLI_VD_H

#include "variable.h"
class junli_vd
{
    public:
        junli_vd();
        virtual ~junli_vd();
        bool parse_junli(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex);
        bool vClearMsg(MESSAGEOK *messageIn, unsigned short int msgID);
    private:
};

#endif // JUNLI_VD_H
