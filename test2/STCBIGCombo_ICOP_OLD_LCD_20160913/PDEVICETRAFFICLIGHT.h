//---------------------------------------------------------------------------

#include "var.h"
//---------------------------------------------------------------------------
class PDEVICETRAFFICLIGHT
{
    public:

        PDEVICETRAFFICLIGHT(void);
        ~PDEVICETRAFFICLIGHT(void);
        bool DoWorkViaPDevice(MESSAGEOK);                                       //�ھ�PDevice�Ӫ��ʥ]�@�Ʊ�

    private:
        int iAskW77E58CounterReturn;

        DATA_Bit GC_DATA0;
        DATA_Bit GC_DATA1;
        DATA_Bit GC_DATA2;
        void BRTGreenConflict(BYTE, BYTE, BYTE, BYTE);


};
//---------------------------------------------------------------------------
