#ifndef screenPlanEditF2H
#define screenPlanEditF2H
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------

    struct SubPlanInfoEditF2 {
           unsigned short int _green;
           unsigned short int _min_green;
           unsigned short int _max_green;
           unsigned short int _yellow;
           unsigned short int _allred;
           unsigned short int _pedgreen_flash;
           unsigned short int _pedred;
    };

    struct PlanInfoEditF2 {
           unsigned short int _planid;
           unsigned short int _dir;
           unsigned short int _phase_order;
           unsigned short int _subphase_count;
           unsigned short int _cycle_time;
           unsigned short int _offset;
           SubPlanInfoEditF2 _ptr_subplaninfo[8];
    };

//---------------------------------------------------------------------------
class ScreenPlanEditF2:public SCREEN
{
    public:
      ScreenPlanEditF2(void);
      ~ScreenPlanEditF2(void);

      void DisplayPlan(int);
      void doKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

      void vDisplayPlanNoReload(int);                                           //for PhaseSel
      bool vGoToPhasePreview(void);

    private:
      DISP_WORD planIdAndPhase[4];
      DISP_WORD planTable[48];
      DISP_WORD cycleAndOffset[8];

//OT Debug 0523
      BYTE planTableTemp[48];
      BYTE OffsetTemp[4];
      BYTE ucPhaseTemp[2];

      void vTmpArrayToData();
      bool vSetSubphaseCount(void);

      int face;                                                                 //���� 1:���W 2:���� 3:���U 4:�k�W 5:�k�� 6:�k�U
      int cSelPosition;                                                         //OT Debug 0523
      int cPosition;                                                         //OT Debug 0523
      void LoadPlan(int);                                                       //�q�ǫi���̱o��
      void vLoadPlanOnlyFromSTCPanel(int);
      void DisplayPlanTable(int);                                               //��ܦb�e���W
      void SavePlan(bool);                                                      //�g�^�ǫi����
      bool vAppendPlan(void);

      void initDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�
      void DisplayUpAndDownPage(int);
      void DisplayLeftAndRightPage(int);
      void DisplayPlanIdAndPhase(void);
      void DisplayCycleAndOffset(void);

      void DisplayGreenAllRedYellow(int);
      void DisplayPGFPR(int);
      void DisplayMinmaxGreen(int);

      void DisplayBackGround(int);

      PlanInfoEditF2 planInfo;

      BYTE planBitmap[6][3840];                                                 //����
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
};
//---------------------------------------------------------------------------
extern ScreenPlanEditF2 screenPlanEditF2;
#endif


