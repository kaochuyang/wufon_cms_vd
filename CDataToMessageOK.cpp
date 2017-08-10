#include "CDataToMessageOK.h"
#include "SMEM.h"
#include "CHexToInt.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
//----------------------------------------------
// what's in this class
//  1. transfer data to SmartSensorProtocal
//  2. package data to 92 protocal
//----------------------------------------------
CDataToMessageOK oDataToMessageOK;
//-------------------------------------------------------------------
/*
MESSAGEOK CDataToMessageOK::SendSSProtocalSetTimeInterval(int intervalSec)      //設定SmartSensor的IntervalTime
{                                                                               //成功會回應SKSuccess~
try {                                                                           //失敗回應SKFailure~
    int i;
    short int siDataLenth = 0;
    char data[30];
    bzero(data, 30);
    MESSAGEOK _MESSAGEOUT;

    sprintf(data, "SKS00008E0008");
    siDataLenth += 13;

    char cIntervalSec[9];                                                       // 8+1個 \0
    int chkSumTmp = 0;
    char Checksum[5];
    sprintf(cIntervalSec, "%8X", intervalSec);                                  //intervalSec為8位數

    for(i = 0; i < 8; i++) {                                                    //將 0x0 換成 ASCII '0'
        if(cIntervalSec[i] == 0x20)
           cIntervalSec[i] = '0';
        data[siDataLenth+i] = cIntervalSec[i];                                  //順便放到Data裡
    }
    siDataLenth += i;
//---------------

    for(i =2; i < siDataLenth; i++)                                             //計算checkSUM
        chkSumTmp += data[i];

    sprintf(Checksum, "%4X", chkSumTmp);
    for(i = 0; i < 4; i++) {
        if(Checksum[i] == 0x20)
           Checksum[i] = '0';
        data[siDataLenth+i] = Checksum[i];
    }
    siDataLenth  += i;
//---------------
    data[siDataLenth] = 0x0D;
    siDataLenth += 1;

    _MESSAGEOUT.protocol = PROTOCOLSS;
    _MESSAGEOUT.packetLcn = smem.GetAddress();
    _MESSAGEOUT.packetLength = siDataLenth;
    _MESSAGEOUT.cksStatus = true;
    _MESSAGEOUT.success = true;
    _MESSAGEOUT.UDPorRS232 = cRS232;
    _MESSAGEOUT.InnerOrOutWard = cOutWard;
    for(i = 0; i < siDataLenth; i++)
        _MESSAGEOUT.packet[i] = (unsigned char)data[i];

//send
    return _MESSAGEOUT;
  }catch(...){}
}
//-------------------------------------------------------------------
MESSAGEOK CDataToMessageOK::SendSSProtocalSetClassificationLengths(int C1, int C2)
{                                                                               //設定車長類別
try {
    int i;
//    C1 = C1*dCmToFt;
    C1 = C1 / 10;
    if(C1<1) C1=1;

//    C2 = C2*dCmToFt;
    C2 = C2 / 10;
    if(C2 <= C1) C2 = C1+2;


    short int siDataLenth = 0;
    char data[60];
    bzero(data, 60);
    MESSAGEOK _MESSAGEOUT;

    char cInterSec[10];
    int chkSumTmp = 0;
    char fourChar[6];

    sprintf(data, "SKS02000000280000");
    siDataLenth += 17;

//---------------
    sprintf(fourChar, "%4X", C1 );
    for(i = 0; i < 4; i++) {
        if(fourChar[i] == 0x20)
           fourChar[i] = '0';
        data[siDataLenth+i] = fourChar[i];
    }
    siDataLenth += i;
//---------------
    for(i = 0; i < 8; i++) {
        data[siDataLenth+i] = '0';  //Reserved
    }
    siDataLenth += i;
//---------------
    sprintf(fourChar, "%4X", C1+1);
    for(i = 0; i < 4; i++) {
        if(fourChar[i] == 0x20)
           fourChar[i] = '0';
        data[siDataLenth+i] = fourChar[i];
    }
    siDataLenth += i;
//---------------
    sprintf(fourChar, "%4X", C2);
    for(i = 0; i < 4; i++) {
        if(fourChar[i] == 0x20)
           fourChar[i] = '0';
        data[siDataLenth+i] = fourChar[i];
    }
    siDataLenth += i;
//---------------
    for(i = 0; i < 8; i++) {
        data[siDataLenth+i] = '0';  //Reserved
    }
    siDataLenth += i;
//---------------
    sprintf(fourChar, "%4X", C2+1);
    for(i = 0; i < 4; i++) {
        if(fourChar[i] == 0x20)
           fourChar[i] = '0';
        data[siDataLenth+i] = fourChar[i];
    }
    siDataLenth += i;
//---------------
    sprintf(fourChar, "03E8");
    for(i = 0; i < 4; i++) {
        data[siDataLenth+i] = fourChar[i];
    }
    siDataLenth += i;

//---------------
    for(i =2; i < siDataLenth; i++)
        chkSumTmp += data[i];
//---------------
    sprintf(fourChar, "%4X", chkSumTmp);
    for(i = 0; i < 4; i++) {
        if(fourChar[i] == 0x20)
           fourChar[i] = '0';
        data[siDataLenth+i] = fourChar[i];
    }
    siDataLenth += i;

//---------------
    data[siDataLenth] = '~';
    siDataLenth += 1;
    data[siDataLenth] = 0x0D;
    siDataLenth += 1;

    _MESSAGEOUT.protocol = PROTOCOLSS;
    _MESSAGEOUT.packetLcn = smem.GetAddress();
    _MESSAGEOUT.packetLength = siDataLenth;
    _MESSAGEOUT.cksStatus = true;
    _MESSAGEOUT.success = true;
    _MESSAGEOUT.UDPorRS232 = cRS232;
    _MESSAGEOUT.InnerOrOutWard = cOutWard;

    for(i = 0; i < siDataLenth; i++)
        _MESSAGEOUT.packet[i] = (unsigned char)data[i];

    return _MESSAGEOUT;

  } catch(...) {}
}
//-------------------------------------------------------------------
MESSAGEOK CDataToMessageOK::SendSSProtocalSetTime(void)                         //與IPC同步時間
{
try {
    int i;

    short int siDataLenth = 0;
    char data[16];
    bzero(data, 16);
    MESSAGEOK _MESSAGEOUT;

    sprintf(data, "S4");
    siDataLenth += 2;

    char eightChar[9];

    struct tm time_str;
    struct tm *TSP = &time_str;
    time_t now;

    now = time(NULL);
    now = now - TwoKBaseSec;

//---------------
    sprintf(eightChar, "%8X", now);
    for(i = 0; i < 8; i++) {
        if(eightChar[i] == 0x20)
           eightChar[i] = '0';
        data[siDataLenth+i] = eightChar[i];
    }
    siDataLenth += i;
//---------------
    data[siDataLenth] = 0x0D;
    siDataLenth += 1;

    _MESSAGEOUT.protocol = PROTOCOLSS;
    _MESSAGEOUT.packetLcn = smem.GetAddress();
    _MESSAGEOUT.packetLength = siDataLenth;
    _MESSAGEOUT.cksStatus = true;
    _MESSAGEOUT.success = true;
    _MESSAGEOUT.UDPorRS232 = cRS232;
    _MESSAGEOUT.InnerOrOutWard = cOutWard;

    for(i = 0; i < siDataLenth; i++)
        _MESSAGEOUT.packet[i] = (unsigned char)data[i];

    return _MESSAGEOUT;

  } catch(...) {}
}
//-------------------------------------------------------------------
MESSAGEOK CDataToMessageOK::QuerySSProtocalClassificationLengths(void)          //查詢車長設定
{
try {
    int i;

    short int siDataLenth = 0;
    char data[16];
    bzero(data, 16);
    MESSAGEOK _MESSAGEOUT;

    sprintf(data, "SJS0200000028");
    siDataLenth += 13;

    data[13] = 0x0D;
    siDataLenth += 1;

    _MESSAGEOUT.protocol = PROTOCOLSS;
    _MESSAGEOUT.packetLcn = smem.GetAddress();
    _MESSAGEOUT.packetLength = siDataLenth;
    _MESSAGEOUT.cksStatus = true;
    _MESSAGEOUT.success = true;
    _MESSAGEOUT.UDPorRS232 = cRS232;
    _MESSAGEOUT.InnerOrOutWard = cOutWard;

    for(i = 0; i < siDataLenth; i++)
        _MESSAGEOUT.packet[i] = (unsigned char)data[i];

    return _MESSAGEOUT;

  } catch(...) {}
}
*/
//-----------------------------------------------------------------------
MESSAGEOK CDataToMessageOK::vPackageINFOTo92Protocol(unsigned char *cInfoData, int iInfoLength,bool getSeq)
{
try {
    int i, j;
    int len = 10 + iInfoLength;
    int lcn; //error
    char data[16];
    bzero(data, 16);
    MESSAGEOK _MESSAGEOUT;
    _MESSAGEOUT.packetLcn=smem.GetAddress();

    _MESSAGEOUT.packet[0] = 0xAA;
    _MESSAGEOUT.packet[1] = 0xBB;
    if (getSeq)
    _MESSAGEOUT.packet[2] = (BYTE)smem.GetSequence();
    else
    _MESSAGEOUT.packet[2] = 0x00;

    if (smem.vGetAdjudicateReturnAddressBCDorHEX() == cBCD) {                     //Select BCD mode
      _MESSAGEOUT.packet[3]=oHexToInt.vIntToFuckingBCD(1,_MESSAGEOUT.packetLcn );
      _MESSAGEOUT.packet[4]=oHexToInt.vIntToFuckingBCD(0,_MESSAGEOUT.packetLcn );
    }
    else {                                                                      //Normal(HEX) mode
      _MESSAGEOUT.packet[3]=HI(_MESSAGEOUT.packetLcn);
      _MESSAGEOUT.packet[4]=LO(_MESSAGEOUT.packetLcn);
    }

    _MESSAGEOUT.packet[5] = HI(len);
    _MESSAGEOUT.packet[6] = LO(len);

    for (i = 0; i < iInfoLength; i++)
         _MESSAGEOUT.packet[7+i] = cInfoData[i];

    _MESSAGEOUT.packet[7+i] = 0xAA;
    _MESSAGEOUT.packet[8+i] = 0xCC;
    _MESSAGEOUT.packet[9+i] = 0;

    for (j=0;j<9+i;j++) _MESSAGEOUT.packet[9+i]^=_MESSAGEOUT.packet[j];
    _MESSAGEOUT.packetLength=len;                                     //packet Length
    _MESSAGEOUT.protocol = PROTOCOL92;
    _MESSAGEOUT.UDPorRS232 = cRS232;

    return _MESSAGEOUT;

  } catch(...) {}
}

/*
//-----------------------------------------------------------------------
MESSAGEOK CDataToMessageOK::vSet92VDTimeGap_6F14ToMessageOK(int iTimeGap)       //給面版用的
{
try {
    unsigned char cData[3];
    cData[0] = 0x6F;
    cData[1] = 0x14;
    cData[2] = iTimeGap;
    MESSAGEOK _MsgOK;

    _MsgOK = vPackageINFOTo92Protocol(cData, 3,false);
    _MsgOK.InnerOrOutWard = cInner;
    return _MsgOK;
  } catch(...) {}
}
//-----------------------------------------------------------------------
MESSAGEOK CDataToMessageOK::vSet92VDCarLength_6F31ToMessageOK(int iBigCarLength, int iCarLenght)  //For Screen
{
try {
    unsigned char cData[6];
    cData[0] = 0x6F;
    cData[1] = 0x31;
    cData[2] = HI(iBigCarLength);
    cData[3] = LO(iBigCarLength);
    cData[4] = HI(iCarLenght);
    cData[5] = LO(iCarLenght);
    MESSAGEOK _MsgOK;

    _MsgOK = vPackageINFOTo92Protocol(cData, 6,false);
    _MsgOK.InnerOrOutWard = cInner;

    return _MsgOK;
  } catch(...) {}
}
//-----------------------------------------------------------------------
MESSAGEOK CDataToMessageOK::vSet92VDTransmitCycle_6F3FToMessageOK(short int siTransmitCycle, short int TransmitType)
{
try {
    unsigned char cData[4];
    cData[0] = 0x6F;
    cData[1] = 0x3F;
    cData[2] = siTransmitCycle;
    cData[3] = TransmitType;
    MESSAGEOK _MsgOK;

    _MsgOK = vPackageINFOTo92Protocol(cData, 4,false);
    _MsgOK.InnerOrOutWard = cInner;

    return _MsgOK;
  } catch(...) {}
}
//-------------------------------------------------------------------
MESSAGEOK CDataToMessageOK::QuerySSBasicInfo_S0(void)
{
try {
    int i;

    short int siDataLenth = 0;
    char data[16];
    bzero(data, 16);
    MESSAGEOK _MESSAGEOUT;

    sprintf(data, "S0");
    siDataLenth += 2;

    data[siDataLenth] = 0x0D;
    siDataLenth += 1;

    _MESSAGEOUT.protocol = PROTOCOLSS;
    _MESSAGEOUT.packetLcn = smem.GetAddress();
    _MESSAGEOUT.packetLength = siDataLenth;
    _MESSAGEOUT.cksStatus = true;
    _MESSAGEOUT.success = true;
    _MESSAGEOUT.UDPorRS232 = cRS232;
    _MESSAGEOUT.InnerOrOutWard = cOutWard;

    for(i = 0; i < siDataLenth; i++)
        _MESSAGEOUT.packet[i] = (unsigned char)data[i];

    return _MESSAGEOUT;

  }catch(...){}
}
*/
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//OT20110707,
MESSAGEOK CDataToMessageOK::vPackageINFOToVer94RedCount(BYTE byteCMD,           //恆嘉
                                                        unsigned short int usiWay1,
                                                        unsigned short int usiWay2,
                                                        unsigned short int usiWay3,
                                                        unsigned short int usiWay4,
                                                        unsigned short int usiWay5 )
{
try {
  int iPtr = 0;
  int j;
  int lcn; //error
  char data[15];
  bzero(data, 15);
  MESSAGEOK _MESSAGEOUT;
  _MESSAGEOUT.packetLcn=smem.GetAddress();

  for(j = 0; j < 6; j++) {  //null
    _MESSAGEOUT.packet[iPtr] = 0x55;
    iPtr++;
  }

  _MESSAGEOUT.packet[iPtr] = 0xAA; iPtr++;
  _MESSAGEOUT.packet[iPtr] = 0xBB; iPtr++;

  if (smem.vGetAdjudicateReturnAddressBCDorHEX() == cBCD) {                     //Select BCD mode
    _MESSAGEOUT.packet[iPtr]=oHexToInt.vIntToFuckingBCD(1,_MESSAGEOUT.packetLcn ); iPtr++;
    _MESSAGEOUT.packet[iPtr]=oHexToInt.vIntToFuckingBCD(0,_MESSAGEOUT.packetLcn ); iPtr++;
  }
  else {                                                                        //Normal(HEX) mode
    _MESSAGEOUT.packet[iPtr]=HI(_MESSAGEOUT.packetLcn); iPtr++;
    _MESSAGEOUT.packet[iPtr]=LO(_MESSAGEOUT.packetLcn); iPtr++;
  }

  _MESSAGEOUT.packet[iPtr] = byteCMD;  iPtr++;                                  //CMD

  _MESSAGEOUT.packet[iPtr] = usiWay1 & 0xFF; iPtr++;
  _MESSAGEOUT.packet[iPtr] = usiWay2 & 0xFF; iPtr++;
  _MESSAGEOUT.packet[iPtr] = usiWay3 & 0xFF; iPtr++;
  _MESSAGEOUT.packet[iPtr] = usiWay4 & 0xFF; iPtr++;
  _MESSAGEOUT.packet[iPtr] = usiWay5 & 0xFF; iPtr++;

  _MESSAGEOUT.packet[iPtr] = 0xAA; iPtr++;
  _MESSAGEOUT.packet[iPtr] = 0xCC; iPtr++;

  _MESSAGEOUT.packet[iPtr] = 0;

  for (j = 6;j < iPtr; j++) {
    _MESSAGEOUT.packet[iPtr] ^= _MESSAGEOUT.packet[j];
  }
  iPtr++;

  for(j = 0; j < 7; j++) {  //null
    _MESSAGEOUT.packet[iPtr] = 0x55;
    iPtr++;
  }

  _MESSAGEOUT.packetLength = iPtr;                                     //packet Length
  _MESSAGEOUT.protocol = PROTOCOLREDCOUNTVER94;
  _MESSAGEOUT.UDPorRS232 = cRS232;

  return _MESSAGEOUT;

}catch(...){}
}

//cct
//-------------------------------------------------------------------
MESSAGEOK CDataToMessageOK::vPackageINFOToVer94v2RedCount(BYTE byteCMD,           //恆嘉
                                                        unsigned short int usiWay1,
                                                        unsigned short int usiWay2,
                                                        unsigned short int usiWay3,
                                                        unsigned short int usiWay4,
                                                        unsigned short int usiWay5,
                                                        unsigned short int usiWay6,
                                                        unsigned short int usiWay7,
                                                        unsigned short int usiWay8 )
{
try {

  int j;
  int lcn; //error
  char data[15];
  bzero(data, 15);
  MESSAGEOK _MESSAGEOUT;
  _MESSAGEOUT.packetLcn=smem.GetAddress();

  for(j = 0; j < 6; j++) {  //null
    _MESSAGEOUT.packet[j] = 0x55;
  }

  _MESSAGEOUT.packet[6] = 0xAA;
  _MESSAGEOUT.packet[7] = 0xBB;

  if (smem.vGetAdjudicateReturnAddressBCDorHEX() == cBCD) {                     //Select BCD mode
    _MESSAGEOUT.packet[8]=oHexToInt.vIntToFuckingBCD(1,_MESSAGEOUT.packetLcn );
    _MESSAGEOUT.packet[9]=oHexToInt.vIntToFuckingBCD(0,_MESSAGEOUT.packetLcn );
  }
  else {                                                                        //Normal(HEX) mode
    _MESSAGEOUT.packet[8]=HI(_MESSAGEOUT.packetLcn);
    _MESSAGEOUT.packet[9]=LO(_MESSAGEOUT.packetLcn);
  }

  _MESSAGEOUT.packet[10] = byteCMD;                                              //CMD

  _MESSAGEOUT.packet[11] = usiWay1 & 0xFF;
  _MESSAGEOUT.packet[12] = usiWay2 & 0xFF;
  _MESSAGEOUT.packet[13] = usiWay3 & 0xFF;
  _MESSAGEOUT.packet[14] = usiWay4 & 0xFF;
  _MESSAGEOUT.packet[15] = usiWay5 & 0xFF;
  _MESSAGEOUT.packet[16] = usiWay6 & 0xFF;
  _MESSAGEOUT.packet[17] = usiWay7 & 0xFF;
  _MESSAGEOUT.packet[18] = usiWay8 & 0xFF;

  _MESSAGEOUT.packet[19] = 0xAA;
  _MESSAGEOUT.packet[20] = 0xCC;

  _MESSAGEOUT.packet[21] = 0;

  for (j = 6;j < 21; j++) _MESSAGEOUT.packet[21]^=_MESSAGEOUT.packet[j];

  for(j = 0; j < 2; j++) {  //null
    _MESSAGEOUT.packet[22+j] = 0x55;
  }


  _MESSAGEOUT.packetLength = 24;                                     //packet Length
  _MESSAGEOUT.protocol = PROTOCOLREDCOUNTVER94;
  _MESSAGEOUT.UDPorRS232 = cRS232;

  return _MESSAGEOUT;

}catch(...){}
}



//宏凱
//-------------------------------------------------------------------
MESSAGEOK CDataToMessageOK::vPackageINFOToVerHKRedCount(int Data1, int Data2,
                                                        int Data3, int Data4)
{
try{
  MESSAGEOK _MESSAGEOUT;
  _MESSAGEOUT.packetLcn=smem.GetAddress();

  _MESSAGEOUT.packet[0]=0xAA;
  _MESSAGEOUT.packet[1]=0xBB;
  _MESSAGEOUT.packet[2]=HI(_MESSAGEOUT.packetLcn);
  _MESSAGEOUT.packet[3]=LO(_MESSAGEOUT.packetLcn);
  _MESSAGEOUT.packet[4]=0x82;
  _MESSAGEOUT.packet[5]=(BYTE)Data1;
  _MESSAGEOUT.packet[6]=(BYTE)Data2;
  _MESSAGEOUT.packet[7]=(BYTE)Data3;
  _MESSAGEOUT.packet[8]=(BYTE)Data4;
  _MESSAGEOUT.packet[9]=0xAA;
  _MESSAGEOUT.packet[10]=0xCC;
  _MESSAGEOUT.packet[11]=0;
  for (int i=0;i<11;i++) _MESSAGEOUT.packet[11]^=_MESSAGEOUT.packet[i];

  _MESSAGEOUT.packetLength = 12;
  _MESSAGEOUT.protocol = PROTOCOLREDCOUNTVERHK;
  _MESSAGEOUT.UDPorRS232 = cRS232;

  return _MESSAGEOUT;

}catch(...){}
}

//-------------------------------------------------------------------
MESSAGEOK CDataToMessageOK::vPackageINFOToVerHKRedCountComm()
{
try{
  MESSAGEOK _MESSAGEOUT;
  _MESSAGEOUT.packetLcn=smem.GetAddress();

  _MESSAGEOUT.packet[0]=0xAA;
  _MESSAGEOUT.packet[1]=0xBB;
  _MESSAGEOUT.packet[2]=HI(_MESSAGEOUT.packetLcn);
  _MESSAGEOUT.packet[3]=LO(_MESSAGEOUT.packetLcn);
  _MESSAGEOUT.packet[4]=0xFF;
  _MESSAGEOUT.packet[5]=0xAA;
  _MESSAGEOUT.packet[6]=0xCC;
  _MESSAGEOUT.packet[7]=0;

  for (int i=0;i<7;i++) _MESSAGEOUT.packet[7]^=_MESSAGEOUT.packet[i];

  _MESSAGEOUT.packetLength = 8;
  _MESSAGEOUT.protocol = PROTOCOLREDCOUNTVERHK;
  _MESSAGEOUT.UDPorRS232 = cRS232;

  return _MESSAGEOUT;

}catch(...){}
}

//-----------------------------------------------------------------------
MESSAGEOK CDataToMessageOK::vPackageINFOTo92ProtocolSetADDR(unsigned char *cInfoData, int iInfoLength, unsigned char ucSeq, unsigned short int usiAddr)
{
try {
    int i, j;
    int len = 10 + iInfoLength;
    int lcn; //error
    char data[16];
    MESSAGEOK _MESSAGEOUT;

    bzero(data, 16);
    _MESSAGEOUT.packetLcn =usiAddr;

    _MESSAGEOUT.packet[0] = 0xAA;
    _MESSAGEOUT.packet[1] = 0xBB;
    _MESSAGEOUT.packet[2] = ucSeq;

    _MESSAGEOUT.packet[3]=HI(_MESSAGEOUT.packetLcn);
    _MESSAGEOUT.packet[4]=LO(_MESSAGEOUT.packetLcn);

    _MESSAGEOUT.packet[5] = HI(len);
    _MESSAGEOUT.packet[6] = LO(len);

    for (i = 0; i < iInfoLength; i++)
         _MESSAGEOUT.packet[7+i] = cInfoData[i];

    _MESSAGEOUT.packet[7+i] = 0xAA;
    _MESSAGEOUT.packet[8+i] = 0xCC;
    _MESSAGEOUT.packet[9+i] = 0;

    for (j=0;j<9+i;j++) _MESSAGEOUT.packet[9+i]^=_MESSAGEOUT.packet[j];
    _MESSAGEOUT.packetLength=len;                                     //packet Length
    _MESSAGEOUT.protocol = PROTOCOLREDCOUNTVERCCT97;
    _MESSAGEOUT.UDPorRS232 = cRS232;

    return _MESSAGEOUT;

  } catch(...) {}
}

