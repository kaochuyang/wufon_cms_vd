//---------------------------------------------------------------------------

#ifndef PTRAFFIC92H
#define PTRAFFIC92H

#include "var.h"
#include "PTRAFFIC92COMM.h"
#include "PTRAFFIC92TC.h"
#include "PTRAFFIC92CMS.h"
#include "PTRAFFIC92VD.h"

//OT Debug 0706
#include "PTRAFFIC92OTHER.h"
//---------------------------------------------------------------------------
class PTRAFFIC92
{
    public:

        PTRAFFIC92(void);
        ~PTRAFFIC92(void);
        bool DoWorkViaPTraffic(MESSAGEOK);                                      //�ھ�pTraffic�ǨӪ�Structure�@�Ʊ�

    private:

        PTRAFFIC92COMM pTraffic92Comm;
        PTRAFFIC92TC pTraffic92Tc;
        PTRAFFIC92CMS pTraffic92Cms;
        PTRAFFIC92VD pTraffic92Vd;
        PTRAFFIC92OTHER pTraffic92Other;

};
//---------------------------------------------------------------------------
#endif



