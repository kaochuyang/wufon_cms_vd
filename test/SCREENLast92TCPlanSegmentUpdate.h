#ifndef SCREENLast92TCPlanSegmentUpdateH
#define SCREENLast92TCPlanSegmentUpdateH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENLast92TCPlanSegmentUpdate:public SCREEN
{
    public:

      SCREENLast92TCPlanSegmentUpdate(void);
      ~SCREENLast92TCPlanSegmentUpdate(void);

      void DisplayLastUpdate(void);                                               //����_�q�ɶ���
      void DoKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��
      void DisplayPlanUpdate(void);
      void DisplaySegmentUpdate(void);


    private:

      DISP_WORD DLastTime[12];                                                    //�������ťճB,�w�q�y��
      DISP_WORD DPlanID[2];
      DISP_WORD DSegmentType[2];
      unsigned short int usiSegmentType;
      unsigned short int usiPlanID;

      BYTE lastDownBitmap[3840];                                                //����

      void LoadBitmapFromFile(void);                                            //�N����Load�i�O����
      void InitDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�

      void DoKeyF1Work(void);
      void DoKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern SCREENLast92TCPlanSegmentUpdate screenLast92TCPlanSegmentUpdate;
#endif
