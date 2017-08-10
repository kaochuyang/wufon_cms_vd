#ifndef screenLightEditH
#define screenLightEditH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class ScreenLightEdit:public SCREEN
{
    public:
      ScreenLightEdit(void);
      ~ScreenLightEdit(void);

      void DisplayLightEdit(int, unsigned short int);
      void doKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

    private:
      DISP_WORD Light[6][8];   //cSelPosition 0
      DISP_WORD PhaseID[2];    //cSelPosition 1
      DISP_WORD SubPhaseID[2]; //cSelPosition 2
      DISP_WORD StepID[2];     //cSelPosition 3
      DISP_WORD SignalCount;   //cSelPosition 4

      BYTE BLight[6][8];
      BYTE BPhaseID[2];
      BYTE BSubPhaseID;
      BYTE BStepID;

      unsigned short int usiLight[8][5][8];
      unsigned char ucPhaseID;
      unsigned char ucSignalMap;
      unsigned char ucSignalCount;
      unsigned char ucSubphaseCount;
      unsigned char ucStepCount[8];

      unsigned short int usiCurrentSubphaseNo;
      unsigned short int usiCurrentStepNo;

      unsigned short int usiFromWhereInLight;

      void vTmpArrayToData();
      int iPlanReturn;


      int face;                                                                 //���� 1:���W 2:���� 3:���U 4:�k�W 5:�k�� 6:�k�U
      int cSelPosition;                                                         //OT Debug 0523
      int cPosition;                                                            //OT Debug 0523
      void LoadLight(int);                                                       //�q�ǫi���̱o��
      void SavePlan(void);                                                      //�g�^�ǫi����

      void initDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�

      BYTE lightEditBitmap[3840];                                                 //����
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
      void doKeyDefaultWork(void);

      void DisplayLight(void);
};
//---------------------------------------------------------------------------
extern ScreenLightEdit screenLightEdit;
#endif


