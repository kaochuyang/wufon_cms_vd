#ifndef screenReverseMenuH
#define screenReverseMenuH
//---------------------------------------------------------------------------
#include "SCREEN.h"
#include "CReverseTime.h"
//---------------------------------------------------------------------------
class ScreenReverseMenu:public SCREEN
{
    public:
      ScreenReverseMenu(void);
      ~ScreenReverseMenu(void);

      void DisplayReverseMenu(void);
      void doKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��
      void vRefreshStepSec(void);

    private:
      DISP_WORD mark[10];
      void initDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�
      int cSelect;                                                              //�{�b��ܪ��ﶵ

      BYTE reverseMenuBitmap[3840];
      void loadBitmapFromFile(void);                                            //�N����Load�i�O����

      DISP_WORD markStepID[2];
      DISP_WORD markStepSecCount[5];

      unsigned short int usiLocalRevStep;
      CReverseTimeInfo _LocalRev;

      void doKey0Work(void);
      void doKey1Work(void);
      void doKey2Work(void);
      void doKey3Work(void);
      void doKey4Work(void);
      void doKey5Work(void);
      void doKey6Work(void);
      void doKey7Work(void);
      void doKey8Work(void);
      void doKey9Work(void);
      void doKeyAWork(void);
      void doKeyBWork(void);
      void doKeyCWork(void);
      void doKeyDWork(void);
      void doKeyEWork(void);
      void doKeyFWork(void);
      void doKeyF1Work(void);
      void doKeyF2Work(void);
      void doKeyF3Work(void);
      void doKeyF4Work(void);
      void doKeyUPWork(void);
      void doKeyDOWNWork(void);
      void doKeyLEFTWork(void);
      void doKeyRIGHTWork(void);
      void doKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern ScreenReverseMenu screenReverseMenu;
#endif