#ifndef SCREENNoneH
#define SCREENNoneH

#include "SCREEN.h"
#include "var.h"
//---------------------------------------------------------------------------
class SCREENNone:public SCREEN
{
    public:
        SCREENNone(void);
        ~SCREENNone(void);

        void DisplayNone(void);                                                 //��ܪťխ�
        void DoKeyWork(BYTE);                                                   //�ھ�KEY���ȧ@��

    private:
        BYTE noneBitmap[3840];                                                  //�ťթ���

        void DoKeyDefaultWork();                                                //����L��ɧ@����
};
//---------------------------------------------------------------------------
extern SCREENNone screenNone;
#endif
