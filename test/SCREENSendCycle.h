#ifndef SCREENSendCycleH
#define SCREENSendCycleH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENSendCycle:public SCREEN
{
    public:

      SCREENSendCycle(void);
      ~SCREENSendCycle(void);

      void DisplaySendCycle(void);                                              //���
      void DoKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

    private:

      DISP_WORD current;
      DISP_WORD data[4];
      DISP_WORD select[7];
      int cycleData[2];

      BYTE sendCycleBitmap[3840];                                               //����
      void LoadBitmapFromFile(void);                                            //�N����Load�i�O����
      void InitDispWord(void);                                                  //��l�ƪťզ�m

      int cSelect;
      void DisplayCycle(int);

      void DoKey0Work(void);
      void DoKey1Work(void);
      void DoKey2Work(void);
      void DoKey3Work(void);
      void DoKey4Work(void);

      void DoKeyAWork(void);
      void DoKeyBWork(void);
      void DoKeyCWork(void);
      void DoKeyDWork(void);
      void DoKeyEWork(void);
      void DoKeyFWork(void);

      void DoKeyF1Work(void);
      void DoKeyF4Work(void);

      void DoKeyUPWork(void);
      void DoKeyDOWNWork(void);
      void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENSendCycle screenSendCycle;
#endif
