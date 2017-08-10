#ifndef SCREENOperModeH
#define SCREENOperModeH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENOperMode:public SCREEN
{
    public:
        SCREENOperMode(void);
        ~SCREENOperMode(void);

        void DisplayOperMode(void);
        void DoKeyWork(BYTE);                                                   //�ھ�KEY���ȧ@��

        void UpdateOperMode(void);

    private:
        BYTE operModeBitmap[3840];                                              //����

        void LoadBitmapFromFile(void);                                          //�N����Load�i�O����

        void DoKeyF1Work();
        void DoKeyF4Work();
};
//---------------------------------------------------------------------------
extern SCREENOperMode screenOperMode;
#endif
