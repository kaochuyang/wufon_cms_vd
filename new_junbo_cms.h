#ifndef NEW_JUNBO_CMS_H
#define NEW_JUNBO_CMS_H
#include "junbo_cms.h"

class new_junbo_cms : public junbo_cms
{
public:
    new_junbo_cms();

    virtual ~new_junbo_cms();
    void setJunboToCms(junbo_to_cms* obj,BYTE Command,BYTE  parameter1);
    void junbo_cms_send(junbo_to_cms sendContext);
    void brightness_control(int bright_parameter);
    bool ParseBlock(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex);

    void close_light();
    void junbo_send_by_VD(int textID);
    void junbo_cms_receive(MESSAGEOK messageIn);
    void cms_test_function(int text_ID);
    void light_timeout_control(int control_parameter);
    void report_light_timeout();//for app
    void report_light_brightness();//for app
    void query_modual_state();
    void report_module_state_to_revapp();//for app
      void color_packet(MESSAGEOK messageIn);
  void color_control(BYTE color[2][3]);
void CalModuleErr(BYTE paramater,int blockNum);
int getModuleErr();
protected:
private:
    char *tty_name;
    char input_string[25];
    char filename[80];//record file director

    unsigned char junbo_receive_packet[8];
    junbo_to_cms notice_car[5];
    junbo_to_cms flash_on;
    junbo_to_cms flash_off;
    junbo_to_cms light_off;
    junbo_to_cms query[5];
    junbo_to_cms brightness[4];
    junbo_to_cms light_timeout[100];
    junbo_to_cms color_red;
    junbo_to_cms color_green;
    junbo_to_cms color_yellow;
    junbo_to_cms queryCtrlBoard;
    junbo_to_cms color[7];//test
    int ID;
    int sendPacketLengh;
    int moduleErr;
    int moduleErr_[5];
};

#endif // NEW_JUNBO_CMS_H
