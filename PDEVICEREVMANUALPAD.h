//---------------------------------------------------------------------------

#ifndef PDEVICEREVMANUALPADH
#define PDEVICEREVMANUALPADH

#include "var.h"
//---------------------------------------------------------------------------
class PDEVICEREVMANUALPAD
{
    public:

        PDEVICEREVMANUALPAD(void);
        ~PDEVICEREVMANUALPAD(void);
        bool DoWorkViaPDevice(MESSAGEOK);                                       //�ھ�PDevice�Ӫ��ʥ]�@�Ʊ�

        bool vSetRevStatus(MESSAGEOK);
        bool vAckTCConn(MESSAGEOK, bool);

    private:

};
//---------------------------------------------------------------------------
#endif
