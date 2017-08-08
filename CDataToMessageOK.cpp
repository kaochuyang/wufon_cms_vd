#include "CDataToMessageOK.h"
#include "CHexToInt.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
CDataToMessageOK::CDataToMessageOK()
{
    //ctor
}

CDataToMessageOK::~CDataToMessageOK()
{
    //dtor
}
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
