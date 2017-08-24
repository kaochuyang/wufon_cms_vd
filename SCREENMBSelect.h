#ifndef SCREENMBSelectH
#define SCREENMBSelectH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENMBSelect:public SCREEN
{
    public:

      SCREENMBSelect(void);
      ~SCREENMBSelect(void);

      void DisplayMBSelect(void);                                                //���
      void DoKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

    private:

      DISP_WORD cycle;
      DISP_WORD mark[6];                                                        //��ܴ�Ъ���m
      int cSelect;                                                              //�{�b��ܪ��ﶵ

      BYTE ucBitmap[3840];                                                 //����
      void LoadBitmapFromFile(void);                                            //�N����Load�i�O����
      void InitDispWord(void);                                                  //��l�ƪťզ�m

      void DoKey0Work(void);
      void DoKey1Work(void);
      void DoKey2Work(void);
      void DoKey3Work(void);
      void DoKey4Work(void);
      void DoKey5Work(void);

      void DoKeyF1Work(void);
      void DoKeyF3Work(void);
      void DoKeyF4Work(void);

      void DoKeyUPWork(void);
      void DoKeyDOWNWork(void);
      void DoKeyLEFTWork(void);
      void DoKeyRIGHTWork(void);
      void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENMBSelect screenMBSelect;
#endif