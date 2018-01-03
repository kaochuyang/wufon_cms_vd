#include "junbo_cms.h"
#include "SMEM.h"
#include "WRITEJOB.h"
#include "CDataToMessageOK.h"
#include "CTIMER.h"
pthread_mutex_t junbo_cms::_junbo_mutex=PTHREAD_MUTEX_INITIALIZER;
junbo_cms::junbo_cms()
{
    //ctor
    initial_module_state(&module_state_object);
}

junbo_cms::~junbo_cms()
{
    //dtor
}

bool junbo_cms::initial_module_state(module_state_struct *object)
{
    try
    {
        object->bit_block_ID=0;
        object->bit_0=0;
        object->bit_1=0;
        object->bit_2=0;
        object->bit_3=0;
        return true;
    }
    catch(...)
    {

    }
}

void junbo_cms::initial_junbo_control(char *output_tty_name)
{
    tty_name=output_tty_name;
        pthread_mutex_init(&_junbo_mutex,NULL);


    if (junbo_cms_port.OpenRs232Port(tty_name, 9600, false)>0)
    {
        printf("open REV_light_port-%s  Success!!\n",tty_name);
    }
    else printf("open REV_light_port Fail!!\n");


    light_off.command=0xc0;
    light_off.parameter=0x0;

    for(int i=1; i<=4; i++)
    {
        notice_car[i].command=0xc1;
        notice_car[i].parameter=i;
        printf("notice_car[%d]=%x,parameter=%x\n",i,notice_car[i].command,notice_car[i].parameter);
    }

    for(int i=0; i<100; i++)
    {
        light_timeout[i].command=0xc2;
        light_timeout[i].parameter=i;
    }

    flash_on.command=0xc3;
    flash_on.parameter=0x01;

    flash_off.command=0xc3;
    flash_off.parameter=0x0;

    for(int i=0; i<4; i++)
    {
        query[i].command=0xc4;
        query[i].parameter=i;
    }


    for(int i=0; i<4; i++)
    {
        brightness[i].command=0xc5;
        brightness[i].parameter=i+1;
    }

    color_red.command=0xc6;
    color_red.parameter=0x10;
    color_green.command=0xc6;
    color_green.parameter=0x20;
    color_yellow.command=0xc6;
    color_yellow.parameter=0x30;


    // filename[80]='0';
//    sprintf(filename,text_record_location);



}

void junbo_cms::junbo_cms_send(unsigned char junbo_send_packet[6])
{
    int ID=0;
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

    printf("\n junbo_command=%2x parameter=%2x\n",junbo_send_packet[3],junbo_send_packet[4]);
    junbo_send_packet[5]=0x0;
    for(int i=0; i<5; i++)
    {
        junbo_send_packet[5]^=junbo_send_packet[i];
    }

    /*  if((junbo_send_packet[3]==0xc1)&&(junbo_send_packet[3]==0xc3)&&(junbo_send_packet[3]==0xc0))
      {
          ID=junbo_send_packet[2];
          smem.record_light[ID].ID=0;
          smem.record_light[ID].command=0;//junbo_receive_packet[2]   I am ID
          smem.record_light[ID].parameter=0;
      }*/
    junbo_cms_port.Rs232Write(junbo_send_packet,6,tty_name);//write out
    memset(input_string,'0',sizeof(input_string));
    pf=fopen(cFileTmp,"a+");
    if(pf!=NULL)
    {

        sprintf(input_string,"\n    Send junbo_protocol=");
        fwrite(input_string,sizeof(char),sizeof(input_string),pf);
        for(int i=0; i<6; i++)
        {
            sprintf(input_string," %x ",junbo_send_packet[i]);
            fwrite(input_string,3*sizeof(char),sizeof(char),pf);
        }
        fwrite( cTimeHeader, length, 1, pf );
        fclose(pf);
    }
    else printf("pf=NULL!!\n");

    usleep(30000);

}
void junbo_cms::junbo_send_by_VD(int textID)
{
    try
    {
        int count_send=0;
        for(int i=0; i<2; i++)
        {
            junbo_send_packet[0]=0xaa;
            junbo_send_packet[3]=notice_car[textID].command;
            junbo_send_packet[4]=notice_car[textID].parameter;
            printf("RECEIVE THE VD MESSAGE\n");
            for(int i=1; i<3; i++)
            {
                junbo_send_packet[1]=smem.GetSequence();
                junbo_send_packet[2]=0x0+i;
                junbo_send_packet[5]=0x0;
                for(int i=0; i<5; i++)
                {
                    junbo_send_packet[5]^=junbo_send_packet[i];
                }
                junbo_cms_send(junbo_send_packet);
                while(smem.record_light[i].command!=0xb1)
                {

                    if(count_send<3)
                    {
                        junbo_cms_send(junbo_send_packet);
                        count_send++;
                    }
                    else break;
                }

            }
        }
    }
    catch(...) {}
}
void junbo_cms::close_light()
{
    try
    {
        int count_send=0;
        junbo_send_packet[0]=0xaa;
        junbo_send_packet[3]=light_off.command;
        junbo_send_packet[4]=light_off.parameter;
        printf("close light\n");
        for(int i=1; i<3; i++)
        {
            junbo_send_packet[1]=smem.GetSequence();
            junbo_send_packet[2]=0x0+i;
            junbo_send_packet[5]=0x0;
            for(int i=0; i<5; i++)
            {
                junbo_send_packet[5]^=junbo_send_packet[i];
            }
            junbo_cms_send(junbo_send_packet);
            while(smem.record_light[i].command!=0xb0)
            {

                if(count_send<3)
                {
                    junbo_cms_send(junbo_send_packet);
                    count_send++;
                }
                else break;
            }
        }
    }
    catch(...) {}
}

void junbo_cms::junbo_cms_receive(MESSAGEOK messageIn)//just for receive the junbo_cms return message
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
        for(int i=0; i<6; i++)junbo_receive_packet[i]=messageIn.packet[i];
        if(messageIn.cksStatus==true)
        {


            if((junbo_receive_packet[3]==0xb1)
                    ||(junbo_receive_packet[3]==0xb0)
                    ||(junbo_receive_packet[3]==0xb3))
            {
                ID=junbo_receive_packet[2];
                smem.record_light[ID].ID=junbo_receive_packet[2];
                smem.record_light[ID].command=junbo_receive_packet[3];//junbo_receive_packet[2]   I am ID
                smem.record_light[ID].parameter=junbo_receive_packet[4];

                if(smem.record_light[1].command==0xb0&&smem.record_light[2].command==0xb0)smem.protocol_9F_object.o_CMS_mannager._9f08_cms_off_report();
            }
            else if(junbo_receive_packet[3]==0xb2)
            {
                ID=junbo_receive_packet[2];
                smem.record_timeout[ID].ID=junbo_receive_packet[2];
                smem.record_timeout[ID].command=junbo_receive_packet[3];//junbo_receive_packet[2]   I am ID
                smem.record_timeout[ID].parameter=junbo_receive_packet[4];
            }
            else if(junbo_receive_packet[3]==0xb5)
            {
                ID=junbo_receive_packet[2];
                smem.record_brightness[ID].ID=junbo_receive_packet[2];
                smem.record_brightness[ID].command=junbo_receive_packet[3];//junbo_receive_packet[2]   I am ID
                smem.record_brightness[ID].parameter=junbo_receive_packet[4];
            }
            else if(junbo_receive_packet[3]==0xb4)
            {
                //module_state_object.bit_block_ID=((junbo_receive_packet[4]>>4)&0xff);
                smem.record_state[ID][smem.cms_query_count].ID=junbo_receive_packet[2];
                smem.record_state[ID][smem.cms_query_count].command=junbo_receive_packet[3];//junbo_receive_packet[2]   I am ID
                smem.record_state[ID][smem.cms_query_count].parameter=junbo_receive_packet[4];
            }








            printf("Receive junbo_protocol=");
            for(int i=0; i<6; i++)printf("%x ",junbo_receive_packet[i]);
            if(pf!=NULL)
            {
                sprintf(cReadString,"%x %x %x %x %x %x\n",junbo_receive_packet[0],junbo_receive_packet[1],junbo_receive_packet[2],
                        junbo_receive_packet[3],junbo_receive_packet[4],junbo_receive_packet[5]);
                sprintf(receive_string,"\n Receive junbo_protocol=");
                fwrite(receive_string,sizeof(char),sizeof(receive_string),pf);
                fwrite(cReadString,3*sizeof(char),6,pf);
            }
            printf("\n");
        }
        else if(messageIn.cksStatus==false)//check the packet always right
        {
            if(pf!=NULL)
            {

                sprintf(receive_string,"wrong junbo_protocol=\n");
                fwrite(receive_string,sizeof(char),sizeof(receive_string),pf);
                sprintf(cReadString,"%x %x %x %x %x %x\n",junbo_receive_packet[0],junbo_receive_packet[1],junbo_receive_packet[2],
                        junbo_receive_packet[3],junbo_receive_packet[4],junbo_receive_packet[5]);
                fwrite(cReadString,3*sizeof(char),6,pf);
            }
        }

        fwrite( cTimeHeader, length, 1, pf );
        fclose(pf);
        memset(junbo_receive_packet,0,6);
        memset(cReadString,'\0',sizeof(cReadString));
        memset(wrongstring,'\0',sizeof(wrongstring));
    }
    catch(...) {}
}
void junbo_cms::delete_record_before_15day()
{
    try
    {


        struct tm* currenttime;
        time_t now = time(NULL);
        localtime(&now);
        now=now-1296000;
        currenttime = localtime(&now);
        char buf[256];
        buf[255] = '\0';
        char dateTemp[32]= {0};
        sprintf(dateTemp, "%#04d%#02d%#02d*", currenttime->tm_year+1900, currenttime->tm_mon+1, currenttime->tm_mday);
        if (snprintf(buf, 255, "rm /cct/Data/junborecord/%s",dateTemp ) != -1)
        {

            printf("%s",buf);
            system(buf);
        }
    }
    catch(...) {}



}

bool junbo_cms::vClearMsg(MESSAGEOK *messageIn, unsigned short int msgID)
{
    messageIn[msgID].cksStatus = false;
    messageIn[msgID].success=false;
    messageIn[msgID].UDPorRS232=0;
    messageIn[msgID].ReadorWrite=0;
    messageIn[msgID].bGetDLE = false;
    messageIn[msgID].bGetSOH = false; //the same with STX
    messageIn[msgID].bGetACK = false;
    messageIn[msgID].bGetNAK = false;
    messageIn[msgID].uiPacketTEXTLEN = 65535;

    // messageIn[msgID].usiExternCounter = 0;;

    /*
    for (int b = 0; b < BUFFERSIZE; b++) {
      messageIn[msgID].packet[b]=0;
    }
    */
    bzero(messageIn[msgID].packet, BUFFERSIZE);
}

bool junbo_cms::ParseBlock(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex)
{
    int i,j,k;

    i=0;                         //block[i]
    j=0;
    k=*lastPacketIndex;          //messageIn.packet[k]

    for (i=0; i<receiveBlockLength; i++)
    {
//      printf("_%C", messageIn[j].packet[k]=block[i]);

        if ( block[i] == 0xaa)
        {
            if((i>0)&&(messageIn[j].success==true))
            {
                junbo_cms_receive(messageIn[j]);    //save log whitch is wrong message
                vClearMsg(messageIn,j);
            }

            messageIn[j].success=true;
            messageIn[j].packetLength = 6;

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

int junbo_cms::open_port_process(char* tty_name)
{
    try
    {
        int tempmax=0;
        if (tempmax=junbo_cms_port.OpenRs232Port(tty_name, 9600, false)>0)
        {
            printf("open junbo_cms_port-%s  Success!!\n",tty_name);
        }
        else printf("open junbo_cms_port Fail!!\n");

        return tempmax;
    }
    catch(...) {}
}


void junbo_cms::carflow_calculate_interval()
{
    try
    {
        static time_t now;
        static struct tm* currenttime;
        int timeOffset;

        now = time(NULL);
        currenttime = localtime(&now);
        timeOffset=(currenttime->tm_hour*3600+currenttime->tm_min*60+currenttime->tm_sec)-(smem.flow_hour*3600+smem.flow_min*60+smem.flow_sec);
        smem.flow_hour=currenttime->tm_hour;
        smem.flow_min=currenttime->tm_min;
        smem.flow_sec=currenttime->tm_sec;
        if(timeOffset>3)
        smem.car_flow_count++;

printf("CAR FLOW  CLACULATE=%d\n\n",smem.car_flow_count);

    }
    catch(...) {}
}
int junbo_cms::clear_calculate_carflow()
{
    try
    {

printf("SENT CAR FLOW  CLACULATE=%d\n\n",smem.car_flow_count);
       int car_flow=smem.car_flow_count;
    smem.car_flow_count=0;

 return car_flow;

    }catch(...){}
}
void junbo_cms::CMS_boot_time()
{
    try
    {
pthread_mutex_lock(&_junbo_mutex);
struct tm* currenttime;
    time_t now = time(NULL);
    currenttime = localtime(&now);

    if(smem.CMS_boot_flag==false)
    {
        smem.cms_boot_hour=currenttime->tm_hour;
        smem.cms_boot_min=currenttime->tm_min;
        smem.cms_boot_sec=currenttime->tm_sec;
        smem.CMS_boot_flag=true;
    }

pthread_mutex_unlock(&_junbo_mutex);

    }catch(...){}
}

void junbo_cms::brightness_control(int bright_parameter)
{
    try

    {
        BYTE Send_packet[2];
        Send_packet[0]=0x0f;
        Send_packet[1]=0x80;

        unsigned char ucSendTMP[6];
        if(bright_parameter<4&&bright_parameter>=0)
        {
            for(int ID=1; ID<3; ID++ )


            {
                ucSendTMP[0] = 0xAA;//head
                ucSendTMP[1] = smem.GetSequence();
                ucSendTMP[2] = ID;//ID
                ucSendTMP[3] = brightness[bright_parameter].command;
                ucSendTMP[4] = brightness[bright_parameter].parameter;
                ucSendTMP[5] = 0x0;//cks
                for (int a=0; a<5; a++)
                    ucSendTMP[5]^=ucSendTMP[a];
                printf("\nbrightness_control light ID=%d\n",ID);
                junbo_cms_send(ucSendTMP);
            }

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

void junbo_cms::report_light_brightness()
{

    unsigned char data[27];

    data[	0	]=0x0F;
    data[	1	]=0xb3;
//    pthread_mutex_lock(&junbo_light_control::_junbo_mutex);
    for(int i=1; i<3; i++)
    {

        data[3*i-1]=smem.record_light[i].ID;
        data[3*i]=smem.record_light[i].command;
        data[3*i+1]=smem.record_light[i].parameter;
    }
    writeJob.WritePhysicalOut(data, 8, revAPP);
//   pthread_mutex_unlock(&junbo_light_control::_junbo_mutex);


}



void junbo_cms::query_modual_state()
{
    try
    {

        if(smem.cms_query_count>=4)smem.cms_query_count=0;
        int query_block=smem.cms_query_count;


//if(query_block==0)initial_module_state(&module_state_object);

        unsigned char ucSendTMP[6];
        for(int ID=1; ID<3; ID++)
        {




                smem.record_state[ID][query_block].ID=0;
                smem.record_state[ID][query_block].command=0;
                smem.record_state[ID][query_block].parameter=0;
                ucSendTMP[0] = 0xAA;//head
                ucSendTMP[1] =smem.GetSequence();
                ucSendTMP[2] = ID;
                ucSendTMP[3] = query[query_block].command;
                ucSendTMP[4] = query[query_block].parameter;
                ucSendTMP[5] = 0x0;//cks
                for (int a=0; a<5; a++)
                    ucSendTMP[5]^=ucSendTMP[a];
                printf("\nquery light ID=%d\n",ID);
                junbo_cms_send(ucSendTMP);



        }




if((query_block<3)&&(query_block>=0))
{
    smem.cms_query_count++;
    _intervalTimer.set_query_module_timer(2);
}

if(query_block==4){report_module_state_to_revapp();
 _intervalTimer.set_query_module_timer(0);
}
        printf("Send junbo light query.by query_modual_state\n");
        /*-----------------*/
    }
    catch(...) {}
}


void junbo_cms::report_module_state_to_revapp()
{

    BYTE block_test[5];

    int count=0;

    unsigned char data[32];

    data[	0	]=0x0F;
    data[	1	]=0xb8;
    pthread_mutex_lock(&junbo_cms::_junbo_mutex);
    for(int ID=1; ID<3; ID++)
    {
        for(int block=1; block<4; block++)

        {

            block_test[0]=block+(ID-1)*3;
            block_test[1]=(smem.record_state[ID][block].parameter&0x08);
            block_test[2]=(smem.record_state[ID][block].parameter&0x04);
            block_test[3]=(smem.record_state[ID][block].parameter&0x02);
            block_test[4]=(smem.record_state[ID][block].parameter&0x01);

            for(int i=1; i<5; i++)
            {
                if(block_test[i]>0)block_test[i]=1;
                else block_test[i]=0;
            }

            for(int i=0; i<5; i++)data[2+(block-1)*5+(ID-1)*15+i]=block_test[i];

        }


    }
    printf("report_module_state_to_revapp=");
    for(int i=0; i<32; i++)printf("%x ",data[i]);
    printf("\n");
    writeJob.WritePhysicalOut(data, 32, revAPP);

    pthread_mutex_unlock(&junbo_cms::_junbo_mutex);
}
void junbo_cms::report_module_state_to_center()
{
    try
    {
        BYTE block_test[5];

        int count=0;

        unsigned char data[32];

        data[	0	]=0x0F;
        data[	1	]=0xb8;
        pthread_mutex_lock(&junbo_cms::_junbo_mutex);
        for(int ID=1; ID<3; ID++)
        {
            for(int block=1; block<4; block++)

            {

                block_test[0]=block+(ID-1)*3;
                block_test[1]=(smem.record_state[ID][block].parameter&0x08);
                block_test[2]=(smem.record_state[ID][block].parameter&0x04);
                block_test[3]=(smem.record_state[ID][block].parameter&0x02);
                block_test[4]=(smem.record_state[ID][block].parameter&0x01);

                for(int i=1; i<5; i++)
                {
                    if(block_test[i]>0)block_test[i]=1;
                    else block_test[i]=0;
                }

                for(int i=0; i<5; i++)data[2+(block-1)*5+(ID-1)*15+i]=block_test[i];

            }

        }


        MESSAGEOK _MsgOK;

        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 32,true);
        _MsgOK.InnerOrOutWard = cOutWard;




        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);



        pthread_mutex_unlock(&junbo_cms::_junbo_mutex);
    }
    catch(...) {}
}





void junbo_cms::report_light_timeout()//for app
{

    unsigned char data[27];

    data[	0	]=0x5F;
    data[	1	]=0xb7;
    pthread_mutex_lock(&junbo_cms::_junbo_mutex);
    for(int i=1; i<3; i++)
    {
        data[3*i-1]=i;
        data[3*i]=smem.record_timeout[i].command;
        data[3*i+1]=smem.record_timeout[i].parameter;
        printf(" report light timeout ID=%d  timeout=%d\n",data[3*i-1],data[3*i+1]);
    }
    writeJob.WritePhysicalOut(data, 8, revAPP);


    pthread_mutex_unlock(&junbo_cms::_junbo_mutex);


}



void junbo_cms::light_timeout_control(int control_parameter)
{


    BYTE Send_packet[2];
    Send_packet[0]=0x0f;
    Send_packet[1]=0x80;
    pthread_mutex_lock(&junbo_cms::_junbo_mutex);

    if(control_parameter>99||control_parameter<1)control_parameter=10;
    unsigned char ucSendTMP[6];

    for(int i=1; i<3; i++)
    {
        smem.record_timeout[i].ID=i;
        smem.record_timeout[i].command=0xc2;
        if(control_parameter==99)smem.record_timeout[i].parameter=control_parameter;
        else smem.record_timeout[i].parameter=control_parameter+1;
        //int i=4;
        ucSendTMP[0] = 0xAA;//head
        ucSendTMP[1] =smem.GetSequence();
        ucSendTMP[2] = i;//ID
        ucSendTMP[3] = light_timeout[control_parameter].command;
        ucSendTMP[4] = light_timeout[control_parameter].parameter;
        ucSendTMP[5] = 0x0;//cks
        for (int a=0; a<5; a++)
            ucSendTMP[5]^=ucSendTMP[a];
        printf("\nlight_timeout_control light ID=%d  timeoute=%d sec\n",i,ucSendTMP[4]);
        junbo_cms_send(ucSendTMP);
    }


    pthread_mutex_unlock(&junbo_cms::_junbo_mutex);
    smem.light_time.store_time(control_parameter);
    writeJob.WritePhysicalOut(Send_packet,2,revAPP);




    printf("Send junbo light light_timeout_control.by light_timeout_control\n");
}

void junbo_cms::cms_test_function(int text_ID)
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

void junbo_cms::color_control(BYTE color[2][3])
{
    try
    {


        BYTE Send_packet[6];
        Send_packet[0]=0xaa;
        BYTE packet_error[2]= {0x0f,0x81};
        BYTE bit= {1};

        for(int j=0; j<2; j++)
        {
            for(int i=0; i<3; i++)
            {
                switch(color[j][i])
                {
                case 0x01:
                    Send_packet[4]=color_red.parameter+(bit<<i);
                    break;
                case 0x02:
                    Send_packet[4]=color_green.parameter+(bit<<i);
                    break;
                case 0x03:
                    Send_packet[4]=color_yellow.parameter+(bit<<i);
                    break;
                default:
                    smem.vWriteMsgToDOM("wrong color control number \n");
                    printf("wrong color control ID=%d number= %d\n",j+1,i+1);
                    writeJob.WritePhysicalOut(packet_error,2,revAPP);
                    break;

                }

                Send_packet[1]=smem.GetSequence();//sequence
                Send_packet[2]=1+j;//ID
                Send_packet[3]=0xc6;//command
                Send_packet[5]=0x0;//cks
                for (int a=0; a<5; a++)
                    Send_packet[5]^=Send_packet[a];
                bit=1;

                junbo_cms_send(Send_packet);
            }
        }

        packet_error[1]=0x80;
        writeJob.WritePhysicalOut(packet_error,2,revAPP);

    }
    catch(...) {}
};

void junbo_cms::color_packet(MESSAGEOK messageIn)
{
    try
    {

        BYTE color[2][3];
        for(int i=0; i<3; i++)
        {
            color[0][i]=messageIn.packet[9+i];
            color[1][i]=messageIn.packet[12+i];
        }

        color_control(color);
        store_color(messageIn);
    }
    catch(...) {}
};

void junbo_cms::store_color(MESSAGEOK messageIn)
{
    try
    {
        FILE *pf=NULL;
        char filename[256]="/cct/Data/SETTING/color_set.bin";
        BYTE color[6];
        for(int i=0; i<6; i++)
        {


            if(messageIn.packet[9+i]>0&&messageIn.packet[9+i]<4) color[i]=messageIn.packet[9+i];
            else color[i]=0x3;
        }
        pf=fopen(filename,"w+");
        if(pf!=NULL)
        {


            fwrite(&color,sizeof(color),6,pf);

            printf("color= ");
            for(int i=0; i<6; i++)printf("%x",color[i]);
            printf(" \n ");
        }
        else
        {
            smem.vWriteMsgToDOM("store color error\n");
            printf("store_color error\n");
        };
        fclose(pf);

    }
    catch(...) {}

}
void junbo_cms::read_color()
{
    try
    {
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

        BYTE color_M[2][3];
        for(int i=0; i<3; i++)
        {
            color_M[0][i]=color[i];
            color_M[1][i]=color[3+i];
        }
        color_control(color_M);

    }
    catch(...) {}

}
