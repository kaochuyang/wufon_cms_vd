//---------------------------------------------------------------------------

#ifndef PTRAFFICREVSYNCH
#define PTRAFFICREVSYNCH

#include "var.h"
//---------------------------------------------------------------------------
class PTRAFFICREVSYNC
{
    public:

        PTRAFFICREVSYNC(void);
        ~PTRAFFICREVSYNC(void);
        bool DoWorkViaPTraffic(MESSAGEOK);                                       //�ھ�PDevice�Ӫ��ʥ]�@�Ʊ�

        bool vSyncStatus(MESSAGEOK);
        bool vAckTCConn(MESSAGEOK, bool);

    private:

};
//---------------------------------------------------------------------------
#endif
