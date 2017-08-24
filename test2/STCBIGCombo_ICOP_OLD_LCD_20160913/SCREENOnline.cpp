//---------------------------------------------------------------------------
#include "SCREENOnline.h"
#include "SCREENLogo.h"
#include "SCREENMain.h"
#include "LCD240x128.h"
#include "SMEM.h"

SCREENOnline screenOnline;
//---------------------------------------------------------------------------
SCREENOnline::SCREENOnline(void)
{
    LoadBitmapFromFile();
}
//---------------------------------------------------------------------------
SCREENOnline::~SCREENOnline(void)
{
}
//---------------------------------------------------------------------------
void SCREENOnline::DoKeyWork(BYTE key)                                            //�w�L�o,�i�Ӫ��O0x80~0x98����,5X5����
{
try {
    switch (key) {
        case 0x90:
             DoKeyF1Work();
        break;
        case 0x98:
             DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOnline::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//2_password_online.bit","rb");
   if (bitmap) {
        fread(onlineBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOnline::DisplayOnline(void)
{
try {
    smem.SetcFace(cONLINE);
    smem.vWriteMsgToDOM("Enter Online Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,onlineBitmap,128,30);
    setSelectPoint16x16(16,25,16,16/8);       //�T�w��
    DisplayDBOperStat();
    DisplayOperMode();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOnline::DisplayDBOperStat(void)
{
try {
    dbOperStat=smem.GetDbOperStat();
    requestResult=INVAILDVALUE;
    if (dbOperStat==0) {                                                        //���\,�i�����i�J
        clearSelectPoint16x16(16,45,16,16/8);
        setSelectPoint16x16(16,65,16,16/8);
        clearSelectPoint16x16(16,85,16,16/8);
    } else if (dbOperStat==1) {                                                 //�����\,�e��w�^�h���߭n�D
            smem.vSet0F16LockDBRequestStatus(true);
            setSelectPoint16x16(16,45,16,16/8);
            clearSelectPoint16x16(16,65,16,16/8);
            clearSelectPoint16x16(16,85,16,16/8);
    } else if (dbOperStat==2) {                                                 //���\�Ȭd��
            clearSelectPoint16x16(16,45,16,16/8);
            clearSelectPoint16x16(16,65,16,16/8);
            setSelectPoint16x16(16,85,16,16/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOnline::DisplayOperMode(void)
{
try {
    if (smem.GetcFace()==cONLINE) {
        //���ݾާ@��1,�{���ާ@��0
        if (smem.GetOperMode()==0) setSelectPoint16x16(16,105,16,16/8);
        else clearSelectPoint16x16(16,105,16,16/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOnline::ClearAll(void)
{
try {
    clearSelectPoint16x16(16,45,16,16/8);
    clearSelectPoint16x16(16,65,16,16/8);
    clearSelectPoint16x16(16,85,16,16/8);
    clearSelectPoint16x16(168,70,16,16/8);
    clearSelectPoint16x16(200,70,16,16/8);
    clearSelectPoint16x16(184,22,16,16/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOnline::RequestAccept(void)
{
    if (smem.GetcFace()==cONLINE) {
        ClearAll();
        dbOperStat=0;
        setSelectPoint16x16(16,65,16,16/8);     //�{���ާ@
        requestResult=0;
        setSelectPoint16x16(200,70,16,16/8);
    }
}
//---------------------------------------------------------------------------
void SCREENOnline::RequestReadOnly(void)
{
    if (smem.GetcFace()==cONLINE) {
        ClearAll();
        dbOperStat=2;
        setSelectPoint16x16(16,85,16,16/8);     //��Ū
        requestResult=1;
        setSelectPoint16x16(200,70,16,16/8);
    }
}
//---------------------------------------------------------------------------
void SCREENOnline::RequestNoAccept(void)
{
    if (smem.GetcFace()==cONLINE) {
        ClearAll();
        dbOperStat=1;
        setSelectPoint16x16(16,45,16,16/8);
        requestResult=2;
        setSelectPoint16x16(184,22,16,16/8);
    }
}
//---------------------------------------------------------------------------
void SCREENOnline::RequestTimeOut(void)
{
    if (smem.GetcFace()==cONLINE) {
        ClearAll();
        dbOperStat=1;
        setSelectPoint16x16(16,45,16,16/8);
        requestResult=3;
        setSelectPoint16x16(168,70,16,16/8);    //TimeOut
    }
}
//---------------------------------------------------------------------------
void SCREENOnline::DoKeyF1Work(void)
{
    screenLogo.DisplayLogo();
}
//---------------------------------------------------------------------------
void SCREENOnline::DoKeyEnterWork(void)
{
try {
//dbOperStat->0:���\�{���ާ@,1:�����\�{���ާ@,2:�u���\�{���d��
//requestResult->0:accept,1:��Ū,2�����\,3:Timeout

    if (smem.GetOperMode()==0) {                                                //����{���ާ@�Ҧ�
        if (smem.vLoadCenterConnectStatus()==false) {                           //�_�u�ɪ����i�J
            screenMain.DisplayMain();
        } else {

             if (dbOperStat==0 || dbOperStat==2) {                              //�p�G�O���\�ާ@�Υu���\�d��
                 screenMain.DisplayMain();
             } else if (dbOperStat==1) {                                        //�T��ާ@,�V����request
                        if (requestResult==0 || requestResult==1)  screenMain.DisplayMain();
             }
        }
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
