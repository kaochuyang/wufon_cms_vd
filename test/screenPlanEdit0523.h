#ifndef screenPlan0523H
#define screenPlan0523H
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
    struct SubPlanInfo {
           unsigned short int _green;
           unsigned short int _min_green;
           unsigned short int _max_green;
           unsigned short int _yellow;
           unsigned short int _allred;
           unsigned short int _pedgreen_flash;
           unsigned short int _pedred;
    };

    struct PlanInfo {
           unsigned short int _planid;
           unsigned short int _dir;
           unsigned short int _phase_order;
           unsigned short int _subphase_count;
           unsigned short int _cycle_time;
           unsigned short int _offset;
           SubPlanInfo _ptr_subplaninfo[8];
    };
//---------------------------------------------------------------------------
class ScreenPlanEdit0523:public SCREEN
{
    public:
      ScreenPlanEdit0523(void);
      ~ScreenPlanEdit0523(void);

      void DisplayPlan(int);
      void doKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

    private:
      DISP_WORD planIdAndPhase[4];
      DISP_WORD planTable[48];
      DISP_WORD cycleAndOffset[8];

      int face;                                                                 //���� 1:���W 2:���� 3:���U 4:�k�W 5:�k�� 6:�k�U
      int cSecPosition;                                                         //OT Debug 0523
      void LoadPlan(int);                                                       //�q�ǫi���̱o��
      void DisplayPlanTable(int);                                               //��ܦb�e���W
      void SavePlan(void);                                                      //�g�^�ǫi����

      void initDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�
      void DisplayUpAndDownPage(int);
      void DisplayLeftAndRightPage(int);
      void DisplayPlanIdAndPhase(void);
      void DisplayCycleAndOffset(void);

      void DisplayGreenAllRedYellow(int);
      void DisplayPGFPR(int);
      void DisplayMinmaxGreen(int);

      void DisplayBackGround(int);

      PlanInfo planInfo;

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
extern ScreenPlanEdit0523 screenPlan0523;
#endif


