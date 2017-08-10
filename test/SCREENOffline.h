#ifndef SCREENOfflineH
#define SCREENOfflineH

#include "SCREEN.h"
#include "var.h"
//---------------------------------------------------------------------------
class SCREENOffline:public SCREEN
{
    public:
        SCREENOffline(void);
        ~SCREENOffline(void);

        void DisplayOffline(void);
        void DoKeyWork(BYTE);                                                   //�ھ�KEY���ȧ@��
        void DisplayOperMode(void);

    private:
        BYTE offlineBitmap[3840];                                                  //�ťթ���

        void LoadBitmapFromFile(void);

        void DoKeyF1Work(void);
        void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENOffline screenOffline;
#endif
