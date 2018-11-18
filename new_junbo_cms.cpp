#include "new_junbo_cms.h"
#include "SMEM.h"
#include "CTIMER.h"
new_junbo_cms::new_junbo_cms()
{
    //ctor
    initial_junbo_control("/dev/ttyS1");
    tty_name="/dev/ttyS1";
    setJunboToCms(&light_off,0xc0,0x0);
    setJunboToCms(&color_red,0xc0,0x1);
    setJunboToCms(&color_green,0xc0,0x2);
    setJunboToCms(&color_yellow,0xc0,0x3);

    for(int i=0; i<=6; i++)
        setJunboToCms(&color[i],0xc0,i);

    for(int i=0; i<=4; i++)
        setJunboToCms(&notice_car[i],0xc1,i);

    for(int i=0; i<100; i++)
        setJunboToCms(&light_timeout[i],0xc2,i);
    setJunboToCms(&flash_off,0xc3,0x0);
    setJunboToCms(&flash_on,0xc3,0x1);
    for(int i=0; i<5; i++)
        setJunboToCms(&query[i],0xc4,i);
    for(int i=0; i<4; i++)
        setJunboToCms(&brightness[i],0xc5,i+1);
    setJunboToCms(&queryCtrlBoard,0xc6,0);

    sendPacketLengh=8;
    ID=1;
}

new_junbo_cms::~new_junbo_cms()
{
    //dtor




}
void new_junbo_cms::setJunboToCms(junbo_to_cms* obj,
                                  BYTE  cmd,
                                  BYTE para1)
{
    obj->ID=0x0;
    obj->command=cmd;
    obj->parameter2=0x0;
    obj->parameter=para1;
}
void new_junbo_cms::junbo_cms_send(junbo_to_cms sendContext)
{
    unsigned char junbo_send_packet[8];
    int ID=1;
    struct tm* currenttime;
    time_t now = time(NULL);
    currenttime = localtime(&now);
    int length=0;
    char cFileTmp[64]= {0};
    sprintf(cFileTmp, "/cct/Data/junborecord/");
    char dateTemp[32]= {0};
    sprintf(dateTemp, "%#04d%#02d%#02d", currenttime->tm_year+1900, currenttime->tm_mon+1, currenttime->tm_mday);
    strcat(cFileTmp,dateTemp);
    strcat(cFileTmp,"_send_junborecord.txt");
    char cTimeHeader[64]= {0};
    sprintf(cTimeHeader, " %#04d/%#02d/%#02d %#02d:%#02d:%#02d  \0", currenttime->tm_year+1900, currenttime->tm_mon+1, currenttime->tm_mday, currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
    for (int i=0; i<300; i++)
        if (cTimeHeader[i]=='\0')
        {
            length=i;
            break;
        }
    FILE *pf=NULL;
    junbo_send_packet[0]=0xaa;
    junbo_send_packet[1]=0xbb;
    junbo_send_packet[2]=smem.GetSequence();
    junbo_send_packet[3]=ID;
    junbo_send_packet[4]=sendContext.command;
    junbo_send_packet[5]=sendContext.parameter;
    junbo_send_packet[6]=sendContext.parameter2;
    printf("\n junbo_command4=%2x parameter5=%2x\n",junbo_send_packet[4],junbo_send_packet[5]);
    junbo_send_packet[7]=0x0;
    for(int i=0; i<7; i++)
    {
        junbo_send_packet[7]^=junbo_send_packet[i];
    }


    junbo_cms_port.Rs232Write(junbo_send_packet,sendPacketLengh,tty_name);//write out

    usleep(30000);
     int count_send=0;
           while(junbo_receive_packet[3]!=(junbo_send_packet[3]-16))
            {

                if(count_send<3)
                {
                 junbo_cms_port.Rs232Write(junbo_send_packet,sendPacketLengh,tty_name);//write out
                     usleep(30000);
                    count_send++;
                }
                else break;
            }

}
bool new_junbo_cms::ParseBlock(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex)
{
    int i,j,k;

    i=0;                         //block[i]
    j=0;
    k=*lastPacketIndex;          //messageIn.packet[k]

    for (i=0; i<receiveBlockLength; i++)
    {
//      printf("_%C", messageIn[j].packet[k]=block[i]);
        if (( block[i] == 0xaa)&&(block[i]==0xbb))
        {
            if((i>0)&&(messageIn[j].success==true))
            {
                junbo_cms_receive(messageIn[j]);    //save log whitch is wrong message
                vClearMsg(messageIn,j);
            }
            messageIn[j].success=true;
            messageIn[j].packetLength = sendPacketLengh;
            k=0;
            messageIn[j].packet[k] = block[i];
            k++;
        }
        else
        {
            if(k<messageIn[j].packetLength-1)
            {
                messageIn[j].packet[k]=block[i];
                k++;
            }
            else if(k==messageIn[j].packetLength-1)
            {
                for(int a=0; a<messageIn[j].packetLength-1; a++)
                    messageIn[j].packet[k]^=messageIn[j].packet[a];

                if(messageIn[j].packet[k]==block[i])
                {
                    messageIn[j].cksStatus=true;
                    junbo_cms_receive(messageIn[j]);    //save log
                    vClearMsg(messageIn,j);
                    k=0;
                }
                else
                {
                    junbo_cms_receive(messageIn[j]);    //save log whitch is wrong message
                    vClearMsg(messageIn,j);
                    k=0;
                };
            }
        }
    }
    *lastPacketIndex=k;
    return true;
}

void new_junbo_cms::brightness_control(int bright_parameter)
{
    try
    {
        BYTE Send_packet[2];
        Send_packet[0]=0x0f;
        Send_packet[1]=0x80;

        if(bright_parameter<4&&bright_parameter>=0)
        {
            printf("\nbrightness_control light\n");
            junbo_cms_send(brightness[bright_parameter]);

            printf("Send junbo cms brightness control.by brightness_control\n");
            writeJob.WritePhysicalOut(Send_packet,2,revAPP);
        }
        else
        {

            Send_packet[1]=0x81;
            writeJob.WritePhysicalOut(Send_packet,2,revAPP);
        }

    }
    catch(...) {}
}

void new_junbo_cms::close_light()
{
    try
    {
        int count_send=0;
        printf("close light\n");
        junbo_cms_send(light_off);
         }
    catch(...) {}
}

void new_junbo_cms::junbo_send_by_VD(int textID)
{
    try
    {
        int count_send=0;

        junbo_cms_send(notice_car[textID]);

    }
    catch(...) {}
}
void new_junbo_cms::junbo_cms_receive(MESSAGEOK messageIn)//just for receive the junbo_cms return message
{
    struct tm* currenttime;
    time_t now = time(NULL);
    currenttime = localtime(&now);
    FILE *pf=NULL;
    int readSelectLength=0;
    char receive_string[25]= {};
    char cReadString[255];
    unsigned char wrongstring[300];
    char cFileTmp[64]= {0};
    sprintf(cFileTmp, "/cct/Data/junborecord/");
    char dateTemp[32]= {0};
    sprintf(dateTemp, "%#04d%#02d%#02d", currenttime->tm_year+1900, currenttime->tm_mon+1, currenttime->tm_mday);
    strcat(cFileTmp,dateTemp);
    strcat(cFileTmp,"_receive_junborecord.txt");
    char cTimeHeader[64]= {0};
    sprintf(cTimeHeader, "%#04d/%#02d/%#02d %#02d:%#02d:%#02d  \0", currenttime->tm_year+1900, currenttime->tm_mon+1, currenttime->tm_mday, currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
    int length=0;
    for (int i=0; i<300; i++)
        if (cTimeHeader[i]=='\0')
        {
            length=i;
            break;
        }
    int ID=0;
    try
    {
        pf=fopen(cFileTmp,"a+");
        for(int i=0; i<sendPacketLengh; i++)junbo_receive_packet[i]=messageIn.packet[i];
        if(messageIn.cksStatus==true)
        {

            if((junbo_receive_packet[4]==0xb1)
                    ||(junbo_receive_packet[4]==0xb3))
            {

                smem.record_light[ID].ID=ID;
                smem.record_light[ID].command=junbo_receive_packet[4];//junbo_receive_packet[2]   I am ID
                smem.record_light[ID].parameter=junbo_receive_packet[5];

                if(smem.record_light[1].command==0xb1&&smem.record_light[1].parameter==0x0)smem.protocol_9F_object.o_CMS_mannager._9f08_cms_off_report();
            }
            else if(junbo_receive_packet[4]==0xb2)
            {

                smem.record_timeout[ID].ID=ID;
                smem.record_timeout[ID].command=junbo_receive_packet[4];//junbo_receive_packet[2]   I am ID
                smem.record_timeout[ID].parameter=junbo_receive_packet[5];
            }
            else if(junbo_receive_packet[4]==0xb5)
            {

                smem.record_brightness[ID].ID=ID;
                smem.record_brightness[ID].command=junbo_receive_packet[4];//junbo_receive_packet[2]   I am ID
                smem.record_brightness[ID].parameter=junbo_receive_packet[5];
            }
            else if(junbo_receive_packet[4]==0xb4)
            {

                smem.record_state[ID][smem.cms_query_count].ID=ID;
                smem.record_state[ID][smem.cms_query_count].command=junbo_receive_packet[4];//junbo_receive_packet[2]   I am ID
                smem.record_state[ID][smem.cms_query_count].parameter=junbo_receive_packet[5];
            }
            else if(junbo_receive_packet[4]==0xb0)
            {
                //color @to do
                smem.record_brightness[ID].ID=ID;
                smem.record_color[ID].command=junbo_receive_packet[4];//junbo_receive_packet[2]   I am ID
                smem.record_color[ID].parameter=junbo_receive_packet[5];

            }


            printf("Receive junbo_protocol=");
            for(int i=0; i<sendPacketLengh; i++)printf("%x ",junbo_receive_packet[i]);
            if(pf!=NULL)
            {
                sprintf(cReadString,"%x %x %x %x %x %x %x %x\n",junbo_receive_packet[0],junbo_receive_packet[1],junbo_receive_packet[2],
                        junbo_receive_packet[3],junbo_receive_packet[4],junbo_receive_packet[5],junbo_receive_packet[6],junbo_receive_packet[7]);
                sprintf(receive_string,"\n Receive junbo_protocol=");
                fwrite(receive_string,sizeof(char),sizeof(receive_string),pf);
                fwrite(cReadString,3*sizeof(char),sendPacketLengh,pf);
            }
            printf("\n");
        }
        else if(messageIn.cksStatus==false)//check the packet always right
        {
            if(pf!=NULL)
            {

                sprintf(receive_string,"wrong junbo_protocol=\n");
                fwrite(receive_string,sizeof(char),sizeof(receive_string),pf);
                sprintf(cReadString,"%x %x %x %x %x %x %x %x\n",junbo_receive_packet[0],junbo_receive_packet[1],junbo_receive_packet[2],
                        junbo_receive_packet[3],junbo_receive_packet[4],junbo_receive_packet[5],junbo_receive_packet[6],junbo_receive_packet[7]);
                fwrite(cReadString,3*sizeof(char),6,pf);
            }
        }

        fwrite( cTimeHeader, length, 1, pf );
        fclose(pf);
        memset(junbo_receive_packet,0,sendPacketLengh);
        memset(cReadString,'\0',sizeof(cReadString));
        memset(wrongstring,'\0',sizeof(wrongstring));
    }
    catch(...) {}
}

void new_junbo_cms::cms_test_function(int text_ID)
{
    try
    {
        BYTE Send_packet[2];
        Send_packet[0]=0x0f;
        Send_packet[1]=0x81;
        if(text_ID>4||text_ID<1)
        {
            writeJob.WritePhysicalOut(Send_packet,2,revAPP);
        }
        else
        {
            junbo_send_by_VD(text_ID);
            printf("cms_test ID=%d\n",text_ID);
            _intervalTimer.set_close_light_timer(smem.light_time.light_flash_time);
            Send_packet[1]=0x80;
            writeJob.WritePhysicalOut(Send_packet,2,revAPP);
        }
    }
    catch(...) {}
}

void new_junbo_cms::light_timeout_control(int control_parameter)
{


    BYTE Send_packet[2];
    Send_packet[0]=0x0f;
    Send_packet[1]=0x80;


    if(control_parameter>99||control_parameter<1)control_parameter=10;

    smem.record_timeout[0].ID=0;
    smem.record_timeout[0].command=0xc2;
    if(control_parameter==99)smem.record_timeout[0].parameter=control_parameter;
    else smem.record_timeout[0].parameter=control_parameter+1;

    printf("\nlight_timeout_control light timeoute=%d sec\n",control_parameter);
    junbo_cms_send(light_timeout[control_parameter]);
}

void new_junbo_cms::report_light_brightness()
{

    unsigned char data[27];

    data[	0	]=0x0F;
    data[	1	]=0xb3;
//    pthread_mutex_lock(&junbo_light_control::_junbo_mutex);
    for(int i=1; i<3; i++)
    {

        data[3*i-1]=ID;
        data[3*i]=smem.record_light[i].command;
        data[3*i+1]=smem.record_light[i].parameter;
    }
    writeJob.WritePhysicalOut(data, 8, revAPP);
//   pthread_mutex_unlock(&junbo_light_control::_junbo_mutex);


}

void new_junbo_cms::report_light_timeout()//for app
{

    unsigned char data[27];

    data[	0	]=0x5F;
    data[	1	]=0xb7;
    pthread_mutex_lock(&junbo_cms::_junbo_mutex);
    for(int i=1; i<3; i++)
    {
        data[3*i-1]=ID;
        data[3*i]=smem.record_timeout[i].command;
        data[3*i+1]=smem.record_timeout[i].parameter;
        printf(" report light timeout ID=%d  timeout=%d\n",data[3*i-1],data[3*i+1]);
    }
    writeJob.WritePhysicalOut(data, 8, revAPP);


    pthread_mutex_unlock(&junbo_cms::_junbo_mutex);


}

void new_junbo_cms::query_modual_state()
{
    try
    {

        if(smem.cms_query_count>4)smem.cms_query_count=0;
        int query_block=smem.cms_query_count;

        printf("\nquery light ID=%d\n",ID);
        junbo_cms_send(query[query_block]);

        if((query_block<=3)&&(query_block>=0))
        {
            smem.cms_query_count++;
            _intervalTimer.set_query_module_timer(2);
        }
        else
        {
            smem.cms_query_count++;
            smem.protocol_9F_object.o_CMS_mannager.o_module.new_9fc2_module_report(ID);
        }//20180321
        /*
        if(query_block==4){report_module_state_to_revapp();
         _intervalTimer.set_query_module_timer(0);
        }*/
        printf("Send junbo light query.by query_modual_state\n");
        /*-----------------*/
    }
    catch(...) {}
}



void new_junbo_cms::report_module_state_to_revapp()
{

    BYTE block_test[7];

    int count=0;

    unsigned char data[32];

    data[	0	]=0x0F;
    data[	1	]=0xb8;
    pthread_mutex_lock(&junbo_cms::_junbo_mutex);
    for(int block=1; block<5; block++)

    {

        block_test[0]=block;
        block_test[1]=(smem.record_state[ID][block].parameter&0x20);
        block_test[2]=(smem.record_state[ID][block].parameter&0x10);
        block_test[3]=(smem.record_state[ID][block].parameter&0x8);
        block_test[4]=(smem.record_state[ID][block].parameter&0x4);
        block_test[5]=(smem.record_state[ID][block].parameter&0x2);
        block_test[6]=(smem.record_state[ID][block].parameter&0x1);

        for(int i=1; i<7; i++)
        {
            if(block_test[i]>0)block_test[i]=1;
            else block_test[i]=0;
        }

        for(int i=0; i<7; i++)data[2+(block-1)*7+(i+1)]=block_test[i];

    }

    printf("report_module_state_to_revapp=");
    for(int i=0; i<30; i++)printf("%x ",data[i]);
    printf("\n");
    writeJob.WritePhysicalOut(data, 30, revAPP);

    pthread_mutex_unlock(&junbo_cms::_junbo_mutex);
}

void new_junbo_cms::color_packet(MESSAGEOK messageIn)
{
  try
    {

        BYTE color[2][3];
        for(int i=0; i<3; i++)
        {
            color[0][i]=messageIn.packet[9+i];
            color[1][i]=messageIn.packet[9+i];
        }

        color_control(color);
        store_color(messageIn);
    }
    catch(...) {}
}
void new_junbo_cms::color_control(BYTE color[2][3])
{
    try
    {


        BYTE packet_error[2]= {0x0f,0x81};

                switch(color[0][0])
                {
                case 0x01:
                junbo_cms_send(color_red);
                                break;
                case 0x02:
                 junbo_cms_send(color_green);

                    break;
                case 0x03:
                     junbo_cms_send(color_yellow);
                    break;
                default:
                    smem.vWriteMsgToDOM("wrong color control number \n");
                    printf("wrong color control ID=%d number= %d\n",1,color[0][0]);
                    writeJob.WritePhysicalOut(packet_error,2,revAPP);
                    break;
                }

        packet_error[1]=0x80;
        writeJob.WritePhysicalOut(packet_error,2,revAPP);

    }
    catch(...) {}
};

