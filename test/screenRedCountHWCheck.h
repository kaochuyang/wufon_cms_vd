#ifndef screenRedCountHWCheckH
#define screenRedCountHWCheckH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenRedCountHWCheck:public SCREEN
{
    public:
      ScreenRedCountHWCheck(void);
      ~ScreenRedCountHWCheck(void);

      void DisplayRedCountHWCheck(unsigned short int);
      void doKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

      void vRefreshRedCountHWCheck(unsigned char, unsigned char, unsigned char);

    private:
      DISP_WORD markLeftWord[7];
      DISP_WORD markRightWord[7];

      void initDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�
      int cSelect;                                                              //�{�b��ܪ��ﶵ

      unsigned char ucID;

      BYTE ucRedCountHWCheckBitmap[3840];
      void loadBitmapFromFile(void);                                            //�N����Load�i�O����

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
extern ScreenRedCountHWCheck screenRedCountHWCheck;
#endif
