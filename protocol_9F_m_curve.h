#ifndef PROTOCOL_9F_M_CURVE_H
#define PROTOCOL_9F_M_CURVE_H
#include "var.h"

class protocol_9F_m_curve
{
public:
    protocol_9F_m_curve();
    virtual ~protocol_9F_m_curve();
    void send_to_center_3(BYTE head,BYTE type,BYTE value);
    void send_to_center_2(BYTE head,BYTE type);//for act report

    bool DoWorkViaPTraffic92(MESSAGEOK message);        //switch for select

    //----------TC manager-------------------------//
    class tc_manager
    {
    public:
        bool _9f09_tc_link_report();

        class textID_manager
        {

        public:
            bool _9f10_settextID(BYTE textID);
            bool _9f40_textID_query();
            bool _9fc0_textID_report();

        };

        class power_manager
        {public:
            bool _9f07_power_reboot(BYTE device,BYTE second);//device=cms or vd

        };
        //    private:
        textID_manager o_text_ID;
        power_manager o_power;
    };
    //--------------//
    //-------cms brightness manager-------//
    class cms_manager
    {
    public:
        bool _9f08_cms_off_report();

        class brightness_manager
        {
        public:
            bool _9f11_brightness_set(BYTE brightness);
            bool _9f41_brightness_query();
            bool _9fc1_brightness_report();
        };

//-----------cms module manager------------------//
        class module_manager
        {
        public:
            bool _9f42_module_query();
            bool _9fc2_module_report();
            bool _9f02_moduel_act_report();

        };
        class display_time_manager
        {
        public:
            bool _9f13_display_time_set(BYTE _time_);
            bool _9f43_display_time_query();
            bool _9fc3_display_time_report();
        };

        class color_manager
        {
        public:
            bool _9f14_color_set(MESSAGEOK messageIn);
            bool _9f44_color_query();
            bool _9fc4_color_report();
        };
//private:
        brightness_manager o_brightness;
        module_manager   o_module;
        display_time_manager o_display_time;
        color_manager o_color;

    };
    //---------------//
    class vd_manager
    {
    public:
        bool _9f05_vd_state_act_report();
        bool _9f07_vd_trigger_report();


    };

    cms_manager o_CMS_mannager;
    vd_manager o_VD_manager;
    tc_manager o_TC_manager;
protected:
private:




    static  BYTE module_err_quan;
    int vReturnToCenterACK(unsigned char , unsigned char);
    int vReturnToCenterNACK(unsigned char, unsigned char, unsigned char, unsigned char);
};

#endif // PROTOCOL_9F_M_CURVE_H
