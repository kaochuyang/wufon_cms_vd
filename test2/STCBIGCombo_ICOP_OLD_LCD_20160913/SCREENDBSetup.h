#ifndef SCREENDBSetupH
#define SCREENDBSetupH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENDBSetup:public SCREEN
{
    public:

      SCREENDBSetup(void);
      ~SCREENDBSetup(void);

      void DisplayDBSetup(void);                                                //���
      void DoKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

    private:

      DISP_WORD db;
      DISP_WORD mark[3];                                                        //��ܴ�Ъ���m
      int cSelect;                                                              //�{�b��ܪ��ﶵ

      BYTE dbSetupBitmap[3840];                                                 //����
      void LoadBitmapFromFile(void);                                            //�N����Load�i�O����
      void InitDispWord(void);                                                  //��l�ƪťզ�m

      void DoKey0Work(void);
      void DoKey1Work(void);
      void DoKey2Work(void);

      void DoKeyF1Work(void);
      void DoKeyF4Work(void);
      void DoKeyUPWork(void);
      void DoKeyDOWNWork(void);
      void DoKeyLEFTWork(void);
      void DoKeyRIGHTWork(void);
      void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENDBSetup screenDBSetup;
#endif


