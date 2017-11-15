#include "protocol_9F_m_curve.h"
#include "SMEM.h"
#include "PTRAFFIC92COMM.h"
BYTE protocol_9F_m_curve::module_err_quan;
protocol_9F_m_curve _9f_object;
protocol_9F_m_curve::protocol_9F_m_curve()
{
    //ctor

}

protocol_9F_m_curve::~protocol_9F_m_curve()
{
    //dtor
}


bool protocol_9F_m_curve::DoWorkViaPTraffic92(MESSAGEOK mes)
{

    try
    {


        switch(mes.packet[8])
        {
        case 0x10:
            o_TC_manager.o_text_ID._9f10_settextID(mes.packet[9]);

            break;
        case 0x40:
            o_TC_manager.o_text_ID._9f40_textID_query();
            break;


        case 0x11:
            o_CMS_mannager.o_brightness._9f11_brightness_set(mes.packet[9]);
            break;

        case 0x41:
            o_CMS_mannager.o_brightness._9f41_brightness_query();
            break;

        case 0x42:
            o_CMS_mannager.o_module._9f42_module_query();
            break;

        case 0x13:
            o_CMS_mannager.o_display_time._9f13_display_time_set(mes.packet[9]);
            break;

        case 0x43:
            o_CMS_mannager.o_display_time._9f43_display_time_query();
            break;

        case 0x14:
            o_CMS_mannager.o_color._9f14_color_set(mes);

            break;


        case 0x44:
            o_CMS_mannager.o_color._9f44_color_query();
            break;

        case 0x16:
            o_TC_manager.o_power._9f07_power_reboot(mes.packet[9],mes.packet[10]);

            break;


        default:

            _9f_object.vReturnToCenterNACK(0x9f,mes.packet[8],0,0);
            break;
        }


    }
    catch(...) {}


}

void protocol_9F_m_curve::send_to_center_2(BYTE head,BYTE type)
{
    try
    {

        MESSAGEOK _MSG;
        unsigned char data3[2];
        data3[0]  = head;
        data3[1]  =type;

        _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(data3, 2, true);
        _MSG.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);



    }
    catch(...) {}
}
void protocol_9F_m_curve::send_to_center_3(BYTE head,BYTE type,BYTE value)
{
    try
    {

        MESSAGEOK _MSG;
        unsigned char data3[3];
        data3[0]  = head;
        data3[1]  =type;
        data3[2]  =value;
        _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(data3, 3, true);
        _MSG.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);




    }
    catch(...) {}
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
bool protocol_9F_m_curve::tc_manager::textID_manager::_9f10_settextID(BYTE textID)
{
    try
    {

        if(textID>4||textID<1)_9f_object.vReturnToCenterNACK(0x9f,0x10,0x4,1);
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
        BYTE packet[3];
        packet[0]=0x9f;
        packet[1]=0xc0;
        BYTE textID=0xff;
        MESSAGEOK _MSG;
        pf=fopen(filename,"r+");
        if(pf!=NULL)
        {

            fread(&textID,sizeof(BYTE),1,pf);
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
            _9f_object.vReturnToCenterNACK(0x9f,0xc0,0x8,0x1);
            fclose(pf);
        }


        return true;
    }
    catch(...) {}
}

bool protocol_9F_m_curve::cms_manager::brightness_manager::_9f11_brightness_set(BYTE brightness)
{

    try
    {

        cms_mark pack;

        if(0<brightness&&brightness<5)
        {
            smem.junbo_object.brightness_control(brightness-1);
            pack.brightness=brightness;
            if(smem.light_time.write_cms_mark_object(pack))
                _9f_object.vReturnToCenterACK(0x9f,0x11);
            else _9f_object.vReturnToCenterNACK(0x9f,0x11,0x02,0);

        }
        else _9f_object.vReturnToCenterNACK(0x9f,0x11,0x4,0x1);

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

        }

        _9f_object.send_to_center_3(0x9f,0xc2,module_err_quan);
        _9f_object.vReturnToCenterACK(0x9f,0xc2);
        smem.vWriteMsgToDOM("9fc2 cms_module state report success\n");

        return true;

    }
    catch(...) {}
}

bool protocol_9F_m_curve::cms_manager::module_manager::_9f02_moduel_act_report()
{
    try
    {
        _9f_object.send_to_center_3(0x9f,0x02,module_err_quan);
        _9f_object.vReturnToCenterACK(0x9f,0x2);
        return true;
    }
    catch(...) {}
}

bool protocol_9F_m_curve::cms_manager::display_time_manager::_9f13_display_time_set(BYTE _time_)
{
    try
    {
        int int_time=_time_;
        if(0<int_time&&int_time<101)
        {
            smem.junbo_object.light_timeout_control(int_time);
            _9f_object.vReturnToCenterACK(0x9f,0x13);
            printf("9f 13 display time set %d second\n",int_time);
            smem.vWriteMsgToDOM("9f13 set display time success by center\n");
        }
        else _9f_object.vReturnToCenterNACK(0x9f,0x13,0x4,0x1);

        return true;
    }
    catch(...) {}
}
bool protocol_9F_m_curve::cms_manager::display_time_manager::_9f43_display_time_query()
{
    try
    {
        _9f_object.vReturnToCenterACK(0x9f,0x43);
        printf("9f43 display time query success\n");
        smem.vWriteMsgToDOM("9f43 display time query by center\n");
        _9fc3_display_time_report();
        return true;
    }
    catch(...) {}
}
bool protocol_9F_m_curve::cms_manager::display_time_manager::_9fc3_display_time_report()
{
    try
    {
        _9f_object.vReturnToCenterACK(0x9f,0xc3);
        _9f_object.send_to_center_3(0x9f,0xc3,smem.light_time.light_flash_time);
        printf("_9fc3_display_time_report\n");
        smem.vWriteMsgToDOM("_9fc3_display_time_report\n");

        return true;
    }
    catch(...) {}
}


bool protocol_9F_m_curve::cms_manager::color_manager::_9f14_color_set(MESSAGEOK messageIn)
{
    try
    {

        _9f_object.vReturnToCenterACK(0x9f,0x14);

        smem.junbo_object.color_packet(messageIn);
        printf("_9f14_color_set");
        smem.vWriteMsgToDOM("_9f14_color_set\n");


    }
    catch(...) {}
}

bool protocol_9F_m_curve::cms_manager::color_manager::_9f44_color_query()
{
    try
    {

        _9f_object.vReturnToCenterACK(0x9f,0x44);
        smem.vWriteMsgToDOM("_9f44_color_query\n");
        printf("_9f44_color_query\n");
        _9fc4_color_report();

    }
    catch(...) {}
}



bool protocol_9F_m_curve::cms_manager::color_manager::_9fc4_color_report()
{
    try
    {
        printf("_9fc4_color_report\n");
        _9f_object.vReturnToCenterACK(0x9f,0xc4);
        smem.vWriteMsgToDOM("_9fc4_color_report\n");


        FILE *pf=NULL;
        char filename[256]="/cct/Data/SETTING/color_set.bin";
        BYTE color[6];

        pf=fopen(filename,"r");



        if(pf!=NULL)
        {
            printf("read color success\n");

            fread(&color,sizeof(BYTE),6,pf);

            printf("color= ");
            for(int i=0; i<6; i++)printf("%x",color[i]);
            printf(" \n ");
        }
        else
        {
            smem.vWriteMsgToDOM("read color error\n");
            printf("read color error\n");
            for(int i=0; i<6; i++)color[i]=0x3;
        };
        fclose(pf);



        MESSAGEOK _MSG;
        unsigned char data3[8];
        data3[0]  = 0x9F;
        data3[1]  = 0xc4;
        for(int i=0; i<6; i++)data3[2+i]=color[i];

        _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(data3, 8, true);
        _MSG.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);


    }
    catch(...) {}
}

bool protocol_9F_m_curve::vd_manager::_9f05_vd_state_act_report()
{
    if(smem.count_vd_alive>30)_9f_object.send_to_center_3(0x9f,0x05,0x1);//vd was dead
    else _9f_object.send_to_center_3(0x9f,0x05,0x0);    //vd still alive
}
bool protocol_9F_m_curve::vd_manager::_9f07_vd_trigger_report()
{
    try
    {

        _9f_object.send_to_center_2(0x9f,0x07);//vd was trigger

    }
    catch(...) {}
}


bool protocol_9F_m_curve::tc_manager::power_manager::_9f07_power_reboot(BYTE device,BYTE second)
{

    try
    {

        if(device==0x43||device==0x56)
        {
            if(second<=99&&second>=0)
            {
                _9f_object.vReturnToCenterACK(0x9f,0x07);//0x56=vd 0x43==cms
                smem.power_object.power_reset(device,second);
            }
            else
                _9f_object.vReturnToCenterNACK(0x9f,0x7,0x4,0x2);
        }
        else _9f_object.vReturnToCenterNACK(0x9f,0x7,0x4,0x1);

        return true;
    }
    catch(...) {}

}

bool protocol_9F_m_curve::cms_manager::_9f08_cms_off_report()
{
    try
    {
        _9f_object.send_to_center_2(0x9f,0x08);
    }
    catch(...) {}
}

bool protocol_9F_m_curve::tc_manager::_9f09_tc_link_report()
{
    try
    {
        _9f_object.send_to_center_2(0x9f,0x09);
    }
    catch(...) {}
}







