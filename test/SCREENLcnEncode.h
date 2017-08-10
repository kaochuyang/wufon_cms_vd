#ifndef SCREENLcnEncodeH
#define SCREENLcnEncodeH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENLcnEncode:public SCREEN
{
    public:
        SCREENLcnEncode(void);
        ~SCREENLcnEncode(void);

        void DisplayLcnEncode(void);
        void DoKeyWork(BYTE);                                                   //�ھ�KEY���ȧ@��

    private:
        BYTE lcnEncodeBitmap[3840];                                              //����

        DISP_WORD select[2];
        int cSelect;
        DISP_WORD lcnCode;

        void LoadBitmapFromFile(void);                                          //�N����Load�i�O����
        void InitDispWord(void);                                                //��l�Ʀ��e�����y�е��Ѽ�

        void DoKey1Work(void);
        void DoKey2Work(void);
        void DoKeyF1Work(void);
        void DoKeyF2Work(void);
        void DoKeyF4Work(void);
        void DoKeyUpWork(void);
        void DoKeyDownWork(void);
        void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENLcnEncode screenLcnEncode;
#endif

