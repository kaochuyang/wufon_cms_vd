#ifndef SCREENWarningH
#define SCREENWarningH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENWarning:public SCREEN
{
    public:
        SCREENWarning(void);
        ~SCREENWarning(void);

        void DisplayWarning(void);
        void DoKeyWork(BYTE);                                                   //�ھ�KEY���ȧ@��



    private:
        BYTE ucBitmap[3840];                                              //����

        void LoadBitmapFromFile(void);                                          //�N����Load�i�O����

        void DoKeyF1Work();
        void DoKeyF2Work();
        void DoKeyF3Work();
        void DoKeyF4Work();
};
//---------------------------------------------------------------------------
extern SCREENWarning screenWarning;
#endif
