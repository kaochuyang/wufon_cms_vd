#ifndef SCREENHWResetH
#define SCREENHWResetH

#include "SCREEN.h"
#include "var.h"
//---------------------------------------------------------------------------
class SCREENHWReset:public SCREEN
{
    public:
        SCREENHWReset(void);
        ~SCREENHWReset(void);

        void DisplayHWReset(void);
        void DoKeyWork(BYTE);                                                   //�ھ�KEY���ȧ@��

    private:

        BYTE hwResetBitmap[3840];
        DISP_WORD dateWord[8];                                                  //��ܤ������m
        DISP_WORD timeWord[6];                                                  //��ܮɶ�����m

        void LoadBitmapFromFile(void);                                          //�N����Load�i�O����
        void InitDispWord(void);                                                //��l�Ʀ��e�����y�е��Ѽ�
        void DisplayDateTime(void);
};
//---------------------------------------------------------------------------
extern SCREENHWReset screenHWReset;
#endif