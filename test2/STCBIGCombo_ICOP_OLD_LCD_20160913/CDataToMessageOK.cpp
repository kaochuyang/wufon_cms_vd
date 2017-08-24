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

MESSAGEOK CDataToMessageOK::vPackageINFOToVer94RedCount(BYTE byteCMD,           //«í¹Å
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
MESSAGEOK CDataToMessageOK::vPackageINFOToVer94v2RedCount(BYTE byteCMD,           //«í¹Å
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



//§»³Í
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

