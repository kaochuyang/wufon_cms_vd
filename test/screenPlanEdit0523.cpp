#include "screenPlanEdit0523.h"
//#include "screenPlanMenu.h"
#include "screenRunTableList.h"
#include "SCREENMain.h"
#include "screenWeekDay.h"
#include "LCD240x128.h"
#include "CSTC.h"
//Removed #include "screenPlanEditStep1.h"
#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenPlanEdit0523 screenPlanEdit0523;
//---------------------------------------------------------------------------
ScreenPlanEdit0523::ScreenPlanEdit0523(void)
{
    loadBitmapFromFile();
    initDispWord();
    face=1;
}
//---------------------------------------------------------------------------
ScreenPlanEdit0523::~ScreenPlanEdit0523(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//plan_LU.bit","rb");
    if (bitmap) {
        fread(planBitmap[0],3840,1,bitmap);
        fclose(bitmap);
    }

    bitmap=fopen("//cct//bitmap//backGround//TC5F//plan_LM.bit","rb");
    if (bitmap) {
        fread(planBitmap[1],3840,1,bitmap);
        fclose(bitmap);
    }

    bitmap=fopen("//cct//bitmap//backGround//TC5F//plan_LD.bit","rb");
    if (bitmap) {
        fread(planBitmap[2],3840,1,bitmap);
        fclose(bitmap);
    }

    bitmap=fopen("//cct//bitmap//backGround//TC5F//plan_RU.bit","rb");
    if (bitmap) {
        fread(planBitmap[3],3840,1,bitmap);
        fclose(bitmap);
    }

    bitmap=fopen("//cct//bitmap//backGround//TC5F//plan_RM.bit","rb");
    if (bitmap) {
        fread(planBitmap[4],3840,1,bitmap);
        fclose(bitmap);
    }

    bitmap=fopen("//cct//bitmap//backGround//TC5F//plan_RD.bit","rb");
    if (bitmap) {
        fread(planBitmap[5],3840,1,bitmap);
        fclose(bitmap);
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::DisplayPlan(int plan)
{
try {
    smem.SetcFace(cPLAN);
    face=1;   //���W
    cSecPosition = 0;
    lcd240x128.DISPLAY_GRAPHIC(0,planBitmap[face-1],128,30);
    LoadPlan(plan);
    DisplayPlanTable(face);
    DisplayPlanIdAndPhase();  //DisplayPlanIdAndPhase();
    DisplayCycleAndOffset();  //DisplayCycleAndOffset();

    setCursor8x16(planTable[cSecPosition].X,planTable[cSecPosition].Y+16);

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::initDispWord(void)
{
    for (int i=0;i<4;i++) {
         if (i>=0 && i<=1) planIdAndPhase[i].X=144+i*8;
         else if (i>=2 && i<=3) planIdAndPhase[i].X=208+(i-2)*8;
         planIdAndPhase[i].Y=2;
         planIdAndPhase[i].width=8;
         planIdAndPhase[i].height=16;
    }

    for (int i=0;i<48;i++) {
         if (i>=0 && i<=3) planTable[i].X=64+i*8;
         else if (i>=4 && i<=7) planTable[i].X=104+(i-4)*8;
         else if (i>=8 && i<=11) planTable[i].X=144+(i-8)*8;
         else if (i>=12 && i<=15) planTable[i].X=184+(i-12)*8;
         else if (i>=16 && i<=19) planTable[i].X=64+(i-16)*8;
         else if (i>=20 && i<=23) planTable[i].X=104+(i-20)*8;
         else if (i>=24 && i<=27) planTable[i].X=144+(i-24)*8;
         else if (i>=28 && i<=31) planTable[i].X=184+(i-28)*8;
         else if (i>=32 && i<=35) planTable[i].X=64+(i-32)*8;
         else if (i>=36 && i<=39) planTable[i].X=104+(i-36)*8;
         else if (i>=40 && i<=43) planTable[i].X=144+(i-40)*8;
         else if (i>=44 && i<=47) planTable[i].X=184+(i-44)*8;

         if (i>=0 && i<=15) planTable[i].Y=34;
         else if (i>=16 && i<=31) planTable[i].Y=50;
         else if (i>=32 && i<=47) planTable[i].Y=66;
         planTable[i].width=8;
         planTable[i].height=16;
    }

    for (int i=0;i<8;i++) {
         if (i>=0 && i<=3) cycleAndOffset[i].X=56+i*8;
         else if (i>=4 && i<=7) cycleAndOffset[i].X=56+(i-4)*8;
         if (i>=0 && i<=3) cycleAndOffset[i].Y=94;
         else if (i>=4 && i<=7) cycleAndOffset[i].Y=111;
         cycleAndOffset[i].width=8;
         cycleAndOffset[i].height=16;
    }
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::LoadPlan(int plan)
{
try {
    if (stc.Lock_to_Load_Plan_for_Panel(plan)) {
        planInfo._planid=stc._panel_plan._planid;
        planInfo._dir=stc._panel_plan._dir;
        planInfo._phase_order=stc._panel_plan._phase_order;
        planInfo._subphase_count=stc._panel_plan._subphase_count;
        planInfo._cycle_time=stc._panel_plan._cycle_time;
        planInfo._offset=stc._panel_plan._offset;
        for (int i=0;i<8;i++) {                              //���M��
             planInfo._ptr_subplaninfo[i]._green=0;
             planInfo._ptr_subplaninfo[i]._min_green=0;
             planInfo._ptr_subplaninfo[i]._max_green=0;
             planInfo._ptr_subplaninfo[i]._yellow=0;
             planInfo._ptr_subplaninfo[i]._allred=0;
             planInfo._ptr_subplaninfo[i]._pedgreen_flash=0;
             planInfo._ptr_subplaninfo[i]._pedred=0;
        }
        for (int i=0;i<planInfo._subphase_count;i++) {       //�A���g
             planInfo._ptr_subplaninfo[i]._green=stc._panel_plan._ptr_subplaninfo[i]._green;
             planInfo._ptr_subplaninfo[i]._min_green=stc._panel_plan._ptr_subplaninfo[i]._min_green;
             planInfo._ptr_subplaninfo[i]._max_green=stc._panel_plan._ptr_subplaninfo[i]._max_green;
             planInfo._ptr_subplaninfo[i]._yellow=stc._panel_plan._ptr_subplaninfo[i]._yellow;
             planInfo._ptr_subplaninfo[i]._allred=stc._panel_plan._ptr_subplaninfo[i]._allred;
             planInfo._ptr_subplaninfo[i]._pedgreen_flash=stc._panel_plan._ptr_subplaninfo[i]._pedgreen_flash;
             planInfo._ptr_subplaninfo[i]._pedred=stc._panel_plan._ptr_subplaninfo[i]._pedred;
        }
    } else {
        planInfo._planid=plan;
        planInfo._dir=0;
        planInfo._phase_order=0;
        planInfo._subphase_count=8;
        planInfo._cycle_time=0;
        planInfo._offset=0;
        for (int i=0;i<8;i++) {
             planInfo._ptr_subplaninfo[i]._green=0;
             planInfo._ptr_subplaninfo[i]._min_green=0;
             planInfo._ptr_subplaninfo[i]._max_green=0;
             planInfo._ptr_subplaninfo[i]._yellow=0;
             planInfo._ptr_subplaninfo[i]._allred=0;
             planInfo._ptr_subplaninfo[i]._pedgreen_flash=0;
             planInfo._ptr_subplaninfo[i]._pedred=0;
        }
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::DisplayPlanTable(int page)
{
try {
    DisplayBackGround(page);

    for (int i=0;i<2;i++) {
         if (page==1 || page==4) DisplayGreenAllRedYellow(page);
         else if (page==2 || page==5) DisplayPGFPR(page);
         else if (page==3 || page==6) DisplayMinmaxGreen(page);
    }

    DisplayUpAndDownPage(page); //DisplayUpAndDownPage(page);
    DisplayLeftAndRightPage(page); //DisplayLeftAndRightPage(page);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::SavePlan(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::DisplayPlanIdAndPhase(void)
{
try {
    //PlanId
    lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[0].X,planIdAndPhase[0].Y,word8x16[planInfo._planid/10],planIdAndPhase[0].height,planIdAndPhase[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[1].X,planIdAndPhase[1].Y,word8x16[planInfo._planid%10],planIdAndPhase[1].height,planIdAndPhase[1].width/8);
    //Phase
    lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[2].X,planIdAndPhase[2].Y,word8x16[(planInfo._phase_order>>4)&0xFF],planIdAndPhase[2].height,planIdAndPhase[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[3].X,planIdAndPhase[3].Y,word8x16[planInfo._phase_order-((planInfo._phase_order>>4)&0xFF)*16],planIdAndPhase[3].height,planIdAndPhase[3].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::DisplayCycleAndOffset(void)
{
try {
    //cycle
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[0].X,cycleAndOffset[0].Y,word8x16[planInfo._cycle_time/1000],cycleAndOffset[0].height,cycleAndOffset[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[1].X,cycleAndOffset[1].Y,word8x16[(planInfo._cycle_time/100)%10],cycleAndOffset[1].height,cycleAndOffset[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[2].X,cycleAndOffset[2].Y,word8x16[(planInfo._cycle_time/10)%10],cycleAndOffset[2].height,cycleAndOffset[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[3].X,cycleAndOffset[3].Y,word8x16[planInfo._cycle_time%10],cycleAndOffset[3].height,cycleAndOffset[3].width/8);
    //offset
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[4].X,cycleAndOffset[4].Y,word8x16[planInfo._offset/1000],cycleAndOffset[4].height,cycleAndOffset[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[5].X,cycleAndOffset[5].Y,word8x16[(planInfo._offset/100)%10],cycleAndOffset[5].height,cycleAndOffset[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[6].X,cycleAndOffset[6].Y,word8x16[(planInfo._offset/10)%10],cycleAndOffset[6].height,cycleAndOffset[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[7].X,cycleAndOffset[7].Y,word8x16[planInfo._offset%10],cycleAndOffset[7].height,cycleAndOffset[7].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::DisplayUpAndDownPage(int page)
{
try {
    unsigned char temp[16]={0};
    switch (page) {
        case 1:   //���W
        case 4:   //�k�W
          lcd240x128.DISPLAY_GRAPHIC_XY(224,22,temp,16,1);
          lcd240x128.DISPLAY_GRAPHIC_XY(224,70,arrow8x16[1],16,1);
        break;
        case 2:   //����
        case 5:   //�k��
          lcd240x128.DISPLAY_GRAPHIC_XY(224,22,arrow8x16[0],16,1);
          lcd240x128.DISPLAY_GRAPHIC_XY(224,70,arrow8x16[1],16,1);
        break;
        case 3:   //���U
        case 6:   //�k�U
          lcd240x128.DISPLAY_GRAPHIC_XY(224,22,arrow8x16[0],16,1);
          lcd240x128.DISPLAY_GRAPHIC_XY(224,70,temp,16,1);
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::DisplayLeftAndRightPage(int page)
{
try {
    unsigned char temp[16]={0};
    switch (page) {
        case 1:   //���W
        case 2:   //����
        case 3:   //���U
          lcd240x128.DISPLAY_GRAPHIC_XY(224,38,arrow8x16[3],16,1);
          lcd240x128.DISPLAY_GRAPHIC_XY(224,54,temp,16,1);
        break;
        case 4:   //�k�W
        case 5:   //�k��
        case 6:   //�k�U
          lcd240x128.DISPLAY_GRAPHIC_XY(224,38,temp,16,1);
          lcd240x128.DISPLAY_GRAPHIC_XY(224,54,arrow8x16[2],16,1);
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::DisplayBackGround(int page)
{
try {
    face=page;
    lcd240x128.DISPLAY_GRAPHIC(0,planBitmap[face-1],128,30);
    DisplayPlanIdAndPhase();  DisplayPlanIdAndPhase();
    DisplayCycleAndOffset();  DisplayCycleAndOffset();
  } catch (...){}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyWork(BYTE key)
{
try {
    switch (key) {
        case 0x80:
          doKey0Work();
        break;
        case 0x81:
          doKey1Work();
        break;
        case 0x82:
          doKey2Work();
        break;
        case 0x83:
          doKey3Work();
        break;
        case 0x84:
          doKey4Work();
        break;
        case 0x85:
          doKey5Work();
        break;
        case 0x86:
          doKey6Work();
        break;
        case 0x87:
          doKey7Work();
        break;
        case 0x88:
          doKey8Work();
        break;
        case 0x89:
          doKey9Work();
        break;
        case 0x8A:
          doKeyAWork();
        break;
        case 0x8B:
          doKeyBWork();
        break;
        case 0x8C:
          doKeyCWork();
        break;
        case 0x8D:
          doKeyDWork();
        break;
        case 0x8E:
          doKeyEWork();
        break;
        case 0x8F:
          doKeyFWork();
        break;
        case 0x90:
          doKeyF1Work();
        break;
        case 0x91:
          doKeyF2Work();
        break;
        case 0x92:
          doKeyF3Work();
        break;
        case 0x93:
          doKeyF4Work();
        break;
        case 0x94:
          doKeyUPWork();
        break;
        case 0x95:
          doKeyDOWNWork();
        break;
        case 0x96:
          doKeyLEFTWork();
        break;
        case 0x97:
          doKeyRIGHTWork();
        break;
        case 0x98:
          doKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKey1Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKey2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKey3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyAWork(void)
{

  int iPlanidLess = planInfo._planid - 1;                                   //�W�@��Plan
  int iPlanNextDisplay = 999;

  for(int i = iPlanidLess; i >= 0; i--) {
    if( iPlanNextDisplay == 999  && smem.vGetTCPhasePlanSegTypeData(TC_Plan, i) == true)
      iPlanNextDisplay = i;
  }
  for(int i = 48; i >= iPlanidLess; i--) {
    if(  iPlanNextDisplay == 999 && smem.vGetTCPhasePlanSegTypeData(TC_Plan, i) == true )
      iPlanNextDisplay = i;
  }

  screenPlanEdit0523.DisplayPlan(iPlanNextDisplay);                                 // >0�A����display

}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyEWork(void)
{
  int iPlanidPlus = planInfo._planid + 1;                                   //�U�@��Plan
  int iPlanNextDisplay = 999;

  for(int i = iPlanidPlus; i <= 48; i++) {
  //OTMARKPRINTF  printf("i=%d, iPlanNextDisplay=%d, smem.vGetTCPhasePlanSegTypeData()=%d.\n", i, iPlanNextDisplay, smem.vGetTCPhasePlanSegTypeData(TC_Plan, i));
    if( iPlanNextDisplay == 999  && smem.vGetTCPhasePlanSegTypeData(TC_Plan, i) == true)
      iPlanNextDisplay = i;
  }
  for(int i = 0; i <= iPlanidPlus; i++) {
  //OTMARKPRINTF  printf("i=%d, iPlanNextDisplay=%d, smem.vGetTCPhasePlanSegTypeData()=%d.\n", i, iPlanNextDisplay, smem.vGetTCPhasePlanSegTypeData(TC_Plan, i));
    if(  iPlanNextDisplay == 999 && smem.vGetTCPhasePlanSegTypeData(TC_Plan, i) == true )
      iPlanNextDisplay = i;
  }
  screenPlanEdit0523.DisplayPlan(iPlanNextDisplay);                                 // >0�A����display

}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyF2Work(void)
{
//    screenPlanEditStep1.DisplayplanEditStep1_keyPlanID();
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyF4Work(void)
{
//    screenPlanMenu.DisplayPlanMenu();
    screenRunTableList.DisplayRunTableList();
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyUPWork(void)
{
try {
    if (face!=1 && face!=4) DisplayPlanTable(face-1);

//OTADD
    if(face == 1 || face == 4) {
      int iPlanidLess = planInfo._planid - 1;                                   //�W�@��Plan
      int iPlanNextDisplay = 999;

      for(int i = iPlanidLess; i >= 0; i--) {
      //OTMARKPRINTF  printf("i=%d, iPlanNextDisplay=%d, smem.vGetTCPhasePlanSegTypeData()=%d.\n", i, iPlanNextDisplay, smem.vGetTCPhasePlanSegTypeData(TC_Plan, i));
        if( iPlanNextDisplay == 999  && smem.vGetTCPhasePlanSegTypeData(TC_Plan, i) == true)
          iPlanNextDisplay = i;
      }
      for(int i = 48; i >= iPlanidLess; i--) {
      //OTMARKPRINTF  printf("i=%d, iPlanNextDisplay=%d, smem.vGetTCPhasePlanSegTypeData()=%d.\n", i, iPlanNextDisplay, smem.vGetTCPhasePlanSegTypeData(TC_Plan, i));
        if(  iPlanNextDisplay == 999 && smem.vGetTCPhasePlanSegTypeData(TC_Plan, i) == true )
          iPlanNextDisplay = i;
      }

      screenPlanEdit0523.DisplayPlan(iPlanNextDisplay);                                 // >0�A����display
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyDOWNWork(void)
{
try {
    if (face!=3 && face!=6) DisplayPlanTable(face+1);

//OTADD
/*
    if(face == 3 || face == 6) {
      int iPlanidPlus = planInfo._planid + 1;                                   //�U�@��Plan
      int iPlanNextDisplay = 999;

      for(int i = iPlanidPlus; i <= 48; i++) {
      //OTMARKPRINTF  printf("i=%d, iPlanNextDisplay=%d, smem.vGetTCPhasePlanSegTypeData()=%d.\n", i, iPlanNextDisplay, smem.vGetTCPhasePlanSegTypeData(TC_Plan, i));
        if( iPlanNextDisplay == 999  && smem.vGetTCPhasePlanSegTypeData(TC_Plan, i) == true)
          iPlanNextDisplay = i;
      }
      for(int i = 0; i <= iPlanidPlus; i++) {
      //OTMARKPRINTF  printf("i=%d, iPlanNextDisplay=%d, smem.vGetTCPhasePlanSegTypeData()=%d.\n", i, iPlanNextDisplay, smem.vGetTCPhasePlanSegTypeData(TC_Plan, i));
        if(  iPlanNextDisplay == 999 && smem.vGetTCPhasePlanSegTypeData(TC_Plan, i) == true )
          iPlanNextDisplay = i;
      }

      screenPlanEdit0523.DisplayPlan(iPlanNextDisplay);                                 // >0�A����display
    }
*/

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyLEFTWork(void)
{
try {
    if (face>=4) DisplayPlanTable(face-3);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyRIGHTWork(void)
{
try {
    if (face<=3) DisplayPlanTable(face+3);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::doKeyEnterWork(void)
{
    screenWeekDay.DisplayWeekDay();
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::DisplayGreenAllRedYellow(int page)
{
try {
    int temp=0;
    if (page==1) temp=0;
    else temp=4;
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[0].X,planTable[0].Y,word8x16[planInfo._ptr_subplaninfo[temp]._green/1000],planTable[0].height,planTable[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[1].X,planTable[1].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._green/100)%10],planTable[1].height,planTable[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[2].X,planTable[2].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._green/10)%10],planTable[2].height,planTable[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[3].X,planTable[3].Y,word8x16[planInfo._ptr_subplaninfo[temp]._green%10],planTable[3].height,planTable[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[4].X,planTable[4].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._green/1000],planTable[4].height,planTable[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[5].X,planTable[5].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._green/100)%10],planTable[5].height,planTable[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[6].X,planTable[6].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._green/10)%10],planTable[6].height,planTable[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[7].X,planTable[7].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._green%10],planTable[7].height,planTable[7].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[8].X,planTable[8].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._green/1000],planTable[8].height,planTable[8].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[9].X,planTable[9].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._green/100)%10],planTable[9].height,planTable[9].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[10].X,planTable[10].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._green/10)%10],planTable[10].height,planTable[10].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[11].X,planTable[11].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._green%10],planTable[11].height,planTable[11].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[12].X,planTable[12].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._green/1000],planTable[12].height,planTable[12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[13].X,planTable[13].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._green/100)%10],planTable[13].height,planTable[13].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[14].X,planTable[14].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._green/10)%10],planTable[14].height,planTable[14].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[15].X,planTable[15].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._green%10],planTable[15].height,planTable[15].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[16].X,planTable[16].Y,word8x16[planInfo._ptr_subplaninfo[temp]._allred/1000],planTable[16].height,planTable[16].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[17].X,planTable[17].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._allred/100)%10],planTable[17].height,planTable[17].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[18].X,planTable[18].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._allred/10)%10],planTable[18].height,planTable[18].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[19].X,planTable[19].Y,word8x16[planInfo._ptr_subplaninfo[temp]._allred%10],planTable[19].height,planTable[19].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[20].X,planTable[20].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._allred/1000],planTable[20].height,planTable[20].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[21].X,planTable[21].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._allred/100)%10],planTable[21].height,planTable[21].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[22].X,planTable[22].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._allred/10)%10],planTable[22].height,planTable[22].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[23].X,planTable[23].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._allred%10],planTable[23].height,planTable[23].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[24].X,planTable[24].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._allred/1000],planTable[24].height,planTable[24].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[25].X,planTable[25].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._allred/100)%10],planTable[25].height,planTable[25].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[26].X,planTable[26].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._allred/10)%10],planTable[26].height,planTable[26].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[27].X,planTable[27].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._allred%10],planTable[27].height,planTable[27].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[28].X,planTable[28].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._allred/1000],planTable[28].height,planTable[28].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[29].X,planTable[29].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._allred/100)%10],planTable[29].height,planTable[29].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[30].X,planTable[30].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._allred/10)%10],planTable[30].height,planTable[30].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[31].X,planTable[31].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._allred%10],planTable[31].height,planTable[31].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[32].X,planTable[32].Y,word8x16[planInfo._ptr_subplaninfo[temp]._yellow/1000],planTable[32].height,planTable[32].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[33].X,planTable[33].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._yellow/100)%10],planTable[33].height,planTable[33].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[34].X,planTable[34].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._yellow/10)%10],planTable[34].height,planTable[34].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[35].X,planTable[35].Y,word8x16[planInfo._ptr_subplaninfo[temp]._yellow%10],planTable[35].height,planTable[35].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[36].X,planTable[36].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._yellow/1000],planTable[36].height,planTable[36].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[37].X,planTable[37].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._yellow/100)%10],planTable[37].height,planTable[37].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[38].X,planTable[38].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._yellow/10)%10],planTable[38].height,planTable[38].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[39].X,planTable[39].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._yellow%10],planTable[39].height,planTable[39].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[40].X,planTable[40].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._yellow/1000],planTable[40].height,planTable[40].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[41].X,planTable[41].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._yellow/100)%10],planTable[41].height,planTable[41].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[42].X,planTable[42].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._yellow/10)%10],planTable[42].height,planTable[42].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[43].X,planTable[43].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._yellow%10],planTable[43].height,planTable[43].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[44].X,planTable[44].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._yellow/1000],planTable[44].height,planTable[44].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[45].X,planTable[45].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._yellow/100)%10],planTable[45].height,planTable[45].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[46].X,planTable[46].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._yellow/10)%10],planTable[46].height,planTable[46].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[47].X,planTable[47].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._yellow%10],planTable[47].height,planTable[47].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::DisplayPGFPR(int page)
{
try {
    int temp=0;
    if (page==2) temp=0;
    else temp=4;
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[0].X,planTable[0].Y,word8x16[planInfo._ptr_subplaninfo[temp]._pedgreen_flash/1000],planTable[0].height,planTable[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[1].X,planTable[1].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._pedgreen_flash/100)%10],planTable[1].height,planTable[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[2].X,planTable[2].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._pedgreen_flash/10)%10],planTable[2].height,planTable[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[3].X,planTable[3].Y,word8x16[planInfo._ptr_subplaninfo[temp]._pedgreen_flash%10],planTable[3].height,planTable[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[4].X,planTable[4].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._pedgreen_flash/1000],planTable[4].height,planTable[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[5].X,planTable[5].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._pedgreen_flash/100)%10],planTable[5].height,planTable[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[6].X,planTable[6].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._pedgreen_flash/10)%10],planTable[6].height,planTable[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[7].X,planTable[7].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._pedgreen_flash%10],planTable[7].height,planTable[7].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[8].X,planTable[8].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._pedgreen_flash/1000],planTable[8].height,planTable[8].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[9].X,planTable[9].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._pedgreen_flash/100)%10],planTable[9].height,planTable[9].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[10].X,planTable[10].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._pedgreen_flash/10)%10],planTable[10].height,planTable[10].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[11].X,planTable[11].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._pedgreen_flash%10],planTable[11].height,planTable[11].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[12].X,planTable[12].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._pedgreen_flash/1000],planTable[12].height,planTable[12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[13].X,planTable[13].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._pedgreen_flash/100)%10],planTable[13].height,planTable[13].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[14].X,planTable[14].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._pedgreen_flash/10)%10],planTable[14].height,planTable[14].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[15].X,planTable[15].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._pedgreen_flash%10],planTable[15].height,planTable[15].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[16].X,planTable[16].Y,word8x16[planInfo._ptr_subplaninfo[temp]._pedred/1000],planTable[16].height,planTable[16].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[17].X,planTable[17].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._pedred/100)%10],planTable[17].height,planTable[17].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[18].X,planTable[18].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._pedred/10)%10],planTable[18].height,planTable[18].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[19].X,planTable[19].Y,word8x16[planInfo._ptr_subplaninfo[temp]._pedred%10],planTable[19].height,planTable[19].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[20].X,planTable[20].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._pedred/1000],planTable[20].height,planTable[20].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[21].X,planTable[21].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._pedred/100)%10],planTable[21].height,planTable[21].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[22].X,planTable[22].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._pedred/10)%10],planTable[22].height,planTable[22].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[23].X,planTable[23].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._pedred%10],planTable[23].height,planTable[23].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[24].X,planTable[24].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._pedred/1000],planTable[24].height,planTable[24].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[25].X,planTable[25].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._pedred/100)%10],planTable[25].height,planTable[25].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[26].X,planTable[26].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._pedred/10)%10],planTable[26].height,planTable[26].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[27].X,planTable[27].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._pedred%10],planTable[27].height,planTable[27].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[28].X,planTable[28].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._pedred/1000],planTable[28].height,planTable[28].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[29].X,planTable[29].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._pedred/100)%10],planTable[29].height,planTable[29].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[30].X,planTable[30].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._pedred/10)%10],planTable[30].height,planTable[30].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[31].X,planTable[31].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._pedred%10],planTable[31].height,planTable[31].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEdit0523::DisplayMinmaxGreen(int page)
{
try {
    int temp=0;
    if (page==3) temp=0;
    else temp=4;
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[0].X,planTable[0].Y,word8x16[planInfo._ptr_subplaninfo[temp]._min_green/1000],planTable[0].height,planTable[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[1].X,planTable[1].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._min_green/100)%10],planTable[1].height,planTable[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[2].X,planTable[2].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._min_green/10)%10],planTable[2].height,planTable[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[3].X,planTable[3].Y,word8x16[planInfo._ptr_subplaninfo[temp]._min_green%10],planTable[3].height,planTable[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[4].X,planTable[4].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._min_green/1000],planTable[4].height,planTable[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[5].X,planTable[5].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._min_green/100)%10],planTable[5].height,planTable[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[6].X,planTable[6].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._min_green/10)%10],planTable[6].height,planTable[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[7].X,planTable[7].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._min_green%10],planTable[7].height,planTable[7].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[8].X,planTable[8].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._min_green/1000],planTable[8].height,planTable[8].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[9].X,planTable[9].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._min_green/100)%10],planTable[9].height,planTable[9].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[10].X,planTable[10].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._min_green/10)%10],planTable[10].height,planTable[10].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[11].X,planTable[11].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._min_green%10],planTable[11].height,planTable[11].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[12].X,planTable[12].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._min_green/1000],planTable[12].height,planTable[12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[13].X,planTable[13].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._min_green/100)%10],planTable[13].height,planTable[13].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[14].X,planTable[14].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._min_green/10)%10],planTable[14].height,planTable[14].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[15].X,planTable[15].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._min_green%10],planTable[15].height,planTable[15].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[16].X,planTable[16].Y,word8x16[planInfo._ptr_subplaninfo[temp]._max_green/1000],planTable[16].height,planTable[16].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[17].X,planTable[17].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._max_green/100)%10],planTable[17].height,planTable[17].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[18].X,planTable[18].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._max_green/10)%10],planTable[18].height,planTable[18].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[19].X,planTable[19].Y,word8x16[planInfo._ptr_subplaninfo[temp]._max_green%10],planTable[19].height,planTable[19].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[20].X,planTable[20].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._max_green/1000],planTable[20].height,planTable[20].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[21].X,planTable[21].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._max_green/100)%10],planTable[21].height,planTable[21].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[22].X,planTable[22].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._max_green/10)%10],planTable[22].height,planTable[22].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[23].X,planTable[23].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._max_green%10],planTable[23].height,planTable[23].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[24].X,planTable[24].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._max_green/1000],planTable[24].height,planTable[24].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[25].X,planTable[25].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._max_green/100)%10],planTable[25].height,planTable[25].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[26].X,planTable[26].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._max_green/10)%10],planTable[26].height,planTable[26].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[27].X,planTable[27].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._max_green%10],planTable[27].height,planTable[27].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[28].X,planTable[28].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._max_green/1000],planTable[28].height,planTable[28].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[29].X,planTable[29].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._max_green/100)%10],planTable[29].height,planTable[29].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[30].X,planTable[30].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._max_green/10)%10],planTable[30].height,planTable[30].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[31].X,planTable[31].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._max_green%10],planTable[31].height,planTable[31].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
