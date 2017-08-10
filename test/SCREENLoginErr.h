#ifndef SCREENLoginErrH
#define SCREENLoginErrH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENLoginErr:public SCREEN
{
    public:
      SCREENLoginErr(void);
      ~SCREENLoginErr(void);

      void DisplayLoginErr(void);                                               //��ܵn�J���~�e��
      void DoKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

    private:
      BYTE loginErrBitmap[3840];                                                //����
      void LoadBitmapFromFile(void);                                            //�N����Load�i�O����
      void DoKeyEscWork();                                                      //��ESC��ɧ@����
      void DoKeyEnterWork();                                                    //��Enter��ɧ@����
};
//---------------------------------------------------------------------------
extern SCREENLoginErr screenLoginErr;
#endif
