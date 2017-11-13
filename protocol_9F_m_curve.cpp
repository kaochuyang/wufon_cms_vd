#include "protocol_9F_m_curve.h"
#include "SMEM.h"
#include "PTRAFFIC92COMM.h"
protocol_9F_m_curve _9f_object;
protocol_9F_m_curve::protocol_9F_m_curve()
{
    //ctor

}

protocol_9F_m_curve::~protocol_9F_m_curve()
{
    //dtor
}

//ACK------------------------------------------------------------------------
int protocol_9F_m_curve::vReturnToCenterACK(unsigned char ucDevCode,
                                       unsigned char ucControlCode)
{
    try
    {
        unsigned char data[4];

        data[0] = 0x0F;
        data[1] = 0x80;
        data[2] = ucDevCode;
        data[3] = ucControlCode;

        MESSAGEOK _MsgOK;

        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,true);
        _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        return 0;
    }
    catch (...) {}
}
//---------------------------------------------------------------------------
int protocol_9F_m_curve::vReturnToCenterNACK(unsigned char ucDevCode,
                                        unsigned char ucControlCode,
                                        unsigned char ErrorCode,
                                        unsigned char ParameterNumber)
{
    try
    {
        unsigned char data[6];

        data[0] = 0x0F;
        data[1] = 0x81;
        data[2] = ucDevCode;
        data[3] = ucControlCode;
        data[4] = ErrorCode;
        data[5] = ParameterNumber;

        MESSAGEOK _MsgOK;

        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 6,true);
        _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        return 0;
    }
    catch (...) {}
}
//---------------------------------------------------------------------------
bool protocol_9F_m_curve::tc_manager::textID_manager::_9f10_settextID(byte textID)
{
   try
    {

if(textID>4||textID<1)_9f_object.vReturnToCenterNACK(0x9f,0x10,0x8,textID);
else
{smem.light_time.text_ID_control(textID);
smem.vWriteMsgToDOM("store_text_ID by center \n");
_9f_object.vReturnToCenterACK(0x9f,0x10);
}
        return true;


    }
    catch(...) {}
}
bool protocol_9F_m_curve::tc_manager::textID_manager::_9f40_textID_query()
{

}
