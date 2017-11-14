#include "protocol_9F_m_curve.h"
#include "SMEM.h"
#include "PTRAFFIC92COMM.h"
byte protocol_9F_m_curve::module_err_quan;
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
        {
            smem.light_time.text_ID_control(textID);
            smem.vWriteMsgToDOM("store_text_ID by center \n");
            _9f_object.vReturnToCenterACK(0x9f,0x10);
        }
        return true;


    }
    catch(...) {}
}
bool protocol_9F_m_curve::tc_manager::textID_manager::_9f40_textID_query()
{
    try
    {

        printf("9f40 success!t");

        smem.vWriteMsgToDOM("9f40 read text ID success\n");
        _9f_object.vReturnToCenterACK(0x9f,0x40);
        _9fc0_textID_report();



        return true;
    }
    catch(...) {}
}
bool protocol_9F_m_curve::tc_manager::textID_manager::_9fc0_textID_report()
{
    try
    {
        FILE *pf=NULL;
        char filename[256]="/cct/Data/SETTING/TextID.txt";
        byte packet[3];
        packet[0]=0x9f;
        packet[1]=0xc0;
        byte textID=0xff;
        MESSAGEOK _MSG;
        pf=fopen(filename,"r+");
        if(pf!=NULL)
        {

            fread(&textID,sizeof(byte),1,pf);
            printf("9fc0 success!textID=%d\n",textID);
            smem.vWriteMsgToDOM("9f40 read text ID success\n");

            packet[2]=textID;
            _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(packet, 3, true);
            _MSG.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);
            fclose(pf);
            _9f_object.vReturnToCenterACK(0x9f,0xc0);

        }
        else
        {
            _9f_object.vReturnToCenterNACK(0x9f,0xc0,0x2,0x3);
            fclose(pf);
        }


        return true;
    }
    catch(...) {}
}

bool protocol_9F_m_curve::cms_manager::brightness_manager::_9f11_brightness_set(byte brightness)
{

    try
    {

        cms_mark pack;

        if(0<brightness&&brightness<5)
        {
            smem.junbo_object.brightness_control(brightness);
            pack.brightness=brightness;
            if(smem.light_time.write_cms_mark_object(pack))
                _9f_object.vReturnToCenterACK(0x9f,0x11);
            else _9f_object.vReturnToCenterNACK(0x9f,0x11,0x01,0);

        }
        else _9f_object.vReturnToCenterNACK(0x9f,0x11,0x2,0x3);

        return true;
    }
    catch(...) {}
}

bool protocol_9F_m_curve::cms_manager::brightness_manager::_9f41_brightness_query()
{

    try
    {

        printf("9f41 success!");

        smem.vWriteMsgToDOM("9f41 brightness query success\n");
        _9f_object.vReturnToCenterACK(0x9f,0x41);
        _9fc1_brightness_report();



        return true;
    }
    catch(...) {}
}

bool protocol_9F_m_curve::cms_manager::brightness_manager::_9fc1_brightness_report()
{
    try
    {
        cms_mark object;
        MESSAGEOK _MSG;
        unsigned char data3[3];
        data3[0]  = 0x9f;
        data3[1]  = 0xc1;



        object=smem.light_time.get_cms_record();
        _9f_object.vReturnToCenterACK(0x9f,0xc1);
        smem.vWriteMsgToDOM("9fc1 brightness report success\n");


        data3[2]  = object.brightness;

        _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(data3, 3, true);
        _MSG.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);


        return true;
    }
    catch(...) {}
}

bool protocol_9F_m_curve::cms_manager::module_manager::_9f42_module_query()
{
    try
    {
        smem.junbo_object.query_modual_state();
        printf("9f42 success!");

        smem.vWriteMsgToDOM("9f42 module query success\n");
        _9f_object.vReturnToCenterACK(0x9f,0x42);
        _9f42_module_query();



        return true;
    }
    catch(...) {}
}

bool protocol_9F_m_curve::cms_manager::module_manager::_9fc2_module_report()
{
    try
    {


module_err_quan=0;
        BYTE block_test[5];

        for(int ID=1; ID<3; ID++)//lazy to modify old code
        {
            for(int block=1; block<4; block++)

            {

                block_test[1]=(smem.record_state[ID][block].parameter&0x08);
                block_test[2]=(smem.record_state[ID][block].parameter&0x04);
                block_test[3]=(smem.record_state[ID][block].parameter&0x02);
                block_test[4]=(smem.record_state[ID][block].parameter&0x01);

                for(int i=1; i<5; i++)
                {
                    if(block_test[i]>0)
                        module_err_quan++;

                }
            }
            MESSAGEOK _MSG;
            unsigned char data3[3];
            data3[0]  = 0x9F;
            data3[1]  = 0xc2;
            data3[2]  =module_err_quan;
            _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(data3, 3, true);
            _MSG.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);




            _9f_object.vReturnToCenterACK(0x9f,0xc2);
            smem.vWriteMsgToDOM("9fc2 cms_module state report success\n");

            return true;
        }
        }catch(...) {}
    }

bool protocol_9F_m_curve::cms_manager::module_manager::_9f02_moduel_act_report()
{
    try
    {
               MESSAGEOK _MSG;
            unsigned char data3[3];
            data3[0]  = 0x9F;
            data3[1]  = 0x02;
            data3[2]  = module_err_quan;

            _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(data3, 3, true);
            _MSG.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);
    _9f_object.vReturnToCenterACK(0x9f,0x2);

    }catch(...){}



}

