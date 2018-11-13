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

    /*  if((junbo_send_packet[3]==0xc1)&&(junbo_send_packet[3]==0xc3)&&(junbo_send_packet[3]==0xc0))
      {
          ID=junbo_send_packet[2];
          smem.record_light[ID].ID=0;
          smem.record_light[ID].command=0;//junbo_receive_packet[2]   I am ID
          smem.record_light[ID].parameter=0;
      }*/
    junbo_cms_port.Rs232Write(junbo_send_packet,sendPacketLengh,tty_name);//write out
   /* memset(input_string,'0',sizeof(input_string));
    pf=fopen(cFileTmp,"a+");
    if(pf!=NULL)
    {

        sprintf(input_string,"\n    Send junbo_protocol=");
        fwrite(input_string,sizeof(char),sizeof(input_string),pf);
        for(int i=0; i<sendPacketLengh; i++)
        {
            sprintf(input_string," %x ",junbo_send_packet[i]);
            fwrite(input_string,3*sizeof(char),sizeof(char),pf);
        }
        fwrite( cTimeHeader, length, 1, pf );
        fclose(pf);
    }
    else printf("pf=NULL!!\n");*/

    usleep(30000);

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
            while(smem.record_light[0].command!=0xb0)
            {

                if(count_send<3)
                {
                    junbo_cms_send(light_off);
                    count_send++;
                }
                else break;
            }

    }
    catch(...) {}
}

void new_junbo_cms::njunbo_send_by_VD(int textID)
{
    try
    {
        int count_send=0;

                junbo_cms_send(notice_car[textID]);
                while(smem.record_light[0].command!=0xb1)
                {

                    if(count_send<3)
                    {
                        junbo_cms_send(notice_car[textID]);
                        count_send++;
                    }
                    else break;
                }
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
                ID=junbo_receive_packet[3];
                smem.record_light[ID].ID=junbo_receive_packet[3];
                smem.record_light[ID].command=junbo_receive_packet[4];//junbo_receive_packet[2]   I am ID
                smem.record_light[ID].parameter=junbo_receive_packet[5];

                if(smem.record_light[1].command==0xb1&&smem.record_light[1].parameter==0x0)smem.protocol_9F_object.o_CMS_mannager._9f08_cms_off_report();
            }
            else if(junbo_receive_packet[4]==0xb2)
            {
                ID=junbo_receive_packet[3];
                smem.record_timeout[ID].ID=junbo_receive_packet[3];
                smem.record_timeout[ID].command=junbo_receive_packet[4];//junbo_receive_packet[2]   I am ID
                smem.record_timeout[ID].parameter=junbo_receive_packet[5];
            }
            else if(junbo_receive_packet[4]==0xb5)
            {
                ID=junbo_receive_packet[3];
                smem.record_brightness[ID].ID=junbo_receive_packet[3];
                smem.record_brightness[ID].command=junbo_receive_packet[4];//junbo_receive_packet[2]   I am ID
                smem.record_brightness[ID].parameter=junbo_receive_packet[5];
            }
            else if(junbo_receive_packet[4]==0xb4)
            {
                ID=junbo_receive_packet[3];
                //module_state_object.bit_block_ID=((junbo_receive_packet[4]>>4)&0xff);
                smem.record_state[ID][smem.cms_query_count].ID=junbo_receive_packet[3];
                smem.record_state[ID][smem.cms_query_count].command=junbo_receive_packet[4];//junbo_receive_packet[2]   I am ID
                smem.record_state[ID][smem.cms_query_count].parameter=junbo_receive_packet[5];
            }
else if(junbo_receive_packet[4]==0xb0)
            {
          //color @to do
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








