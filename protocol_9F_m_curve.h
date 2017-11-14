#ifndef PROTOCOL_9F_M_CURVE_H
#define PROTOCOL_9F_M_CURVE_H
#include "var.h"
#define byte BYTE
class protocol_9F_m_curve
{
    public:
        protocol_9F_m_curve();
        virtual ~protocol_9F_m_curve();


    //----------TC manager-------------------------//
    class tc_manager
    {
        bool _9f09_tc_link_report();

        class textID_manager
        {


    bool _9f10_settextID(byte textID);
    bool _9f40_textID_query();
    bool _9fc0_textID_report();
        };

        class power_manager
        {
            bool power_reboot(byte device,byte second);//device=cms or vd

        };


    };
    //--------------//
    //-------cms brightness manager-------//
    class cms_manager
    {

        bool _9f08_cms_off_report();

   class brightness_manager
    {
        bool _9f11_brightness_set(byte brightness);
        bool _9f41_brightness_query();
        bool _9fc1_brightness_report();
    };

//-----------cms module manager------------------//
   class module_manager
   {
       bool _9f42_module_query();
       bool _9fc2_module_report();
       bool _9f02_moduel_act_report();

   };
   class display_time_manager
   {
       bool _9f13_display_time_set(byte _time_);
       bool _9f43_display_time_query();
       bool _9fc3_display_time_report();
   };

   class color_manager
   {

     bool _9f14_color_set(byte color[6]);
     bool _9f44_color_query();
     bool _9fc4_color_report();
   };

    };
    //---------------//
   class vd_manager
   {

     bool _9f05_vd_state_act_report();
     bool _9f07_vd_trigger_report();


   };


    protected:
    private:
 static  byte module_err_quan;
int vReturnToCenterACK(unsigned char , unsigned char);
        int vReturnToCenterNACK(unsigned char, unsigned char, unsigned char, unsigned char);
};

#endif // PROTOCOL_9F_M_CURVE_H
