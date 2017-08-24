#ifndef SCREENCtlNoH
#define SCREENCtlNoH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENCtlNo:public SCREEN
{
    public:

      SCREENCtlNo(void);
      ~SCREENCtlNo(void);

      void DisplayCtlNo(void);                                                  //��ܳ]�ƽs����
      void DoKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

      void DisplayNum(void);

    private:

      DISP_WORD lcn[5];                                                         //�������ťճB,�w�q�y��

      BYTE ctlNoBitmap[3840];                                                   //����

      void LoadBitmapFromFile(void);                                            //�N����Load�i�O����
      void InitDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�


      void DoKeyF1Work(void);
      void DoKeyF2Work(void);
      void DoKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern SCREENCtlNo screenCtlNo;
#endif
