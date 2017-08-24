#ifndef SCREENUpdateDBH
#define SCREENUpdateDBH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENUpdateDB:public SCREEN
{
    public:
        SCREENUpdateDB(void);
        ~SCREENUpdateDB(void);

        void DisplayUpdateDB(void);                                             //���
        void DoKeyWork(BYTE);                                                   //�ھ�KEY���ȧ@��

    private:
        BYTE updateDBBitmap[3840];                                              //����

        void LoadBitmapFromFile(void);                                          //�N����Load�i�O����
        void DoKeyF1Work();
        void DoKeyF4Work();
        void DoKeyEnterWork();
};
//---------------------------------------------------------------------------
extern SCREENUpdateDB screenUpdateDB;
#endif

