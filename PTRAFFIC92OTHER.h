//---------------------------------------------------------------------------

#ifndef PTRAFFIC92OTHERH
#define PTRAFFIC92OTHERH

#include "var.h"
#include "SMEM.h"

//---------------------------------------------------------------------------
class PTRAFFIC92OTHER
{
    public:

        PTRAFFIC92OTHER(void);
        ~PTRAFFIC92OTHER(void);

        bool DoWorkViaPTraffic92(MESSAGEOK);                                    //�ھ�PTraffic92�Ӫ��T���@�Ʊ�

    private:


        int vReturnToCenterACK(unsigned char , unsigned char);
        int vReturnToCenterNACK(unsigned char, unsigned char, unsigned char, unsigned char);



};
//---------------------------------------------------------------------------
#endif
