#include "junbo_light_control.h"
#include "SMEM.h" //for object smem
pthread_mutex_t junbo_light_control::_junbo_mutex=PTHREAD_MUTEX_INITIALIZER;
junbo_light_control::junbo_light_control()
{
    //ctor
}

junbo_light_control::~junbo_light_control()
{
    //dtor
}

void junbo_light_control::initial_junbo_control(char *text_record_location,char *output_tty_name)
{
    tty_name=output_tty_name;

    if (REV_light_port.OpenRs232Port(tty_name, 9600, false)>0)
    {
        printf("open REV_light_port-%s  Success!!\n",tty_name);
    }
    else printf("open REV_light_port Fail!!\n");


    light_off.command=0xc0;
    light_off.parameter=0x0;

    green.command=0xc1;
    green.parameter=0x02;

    red.command=0xc1;
    red.parameter=0x01;

    flash_on.command=0xc3;
    flash_on.parameter=0x01;

    flash_off.command=0xc3;
    flash_off.parameter=0x0;

    query.command=0xc4;
    query.parameter=0x00;

    green_right.command=0xc1;
    green_right.parameter=0x03;

    for(int i=0; i<4; i++)
    {
        brightness[i].command=0xc5;
        brightness[i].parameter=i+1;
    }
    for(int i=0; i<11; i++)
    {
        light_timeout[i].command=0xc2;
        light_timeout[i].parameter=i;
    }

    filename[60]='0';
    sprintf(filename,text_record_location);

    memset(record_light,0x0,sizeof(record_light));

    junbo_switch_button=0x0;

    int count_for_receive=0;

    default_dir_mark=smem.vGetUCData(revDefaultVehWay);;


}


MESSAGEOK junbo_light_control::revAPP_packet(int readSelectLength,BYTE block[1024])
{
    MESSAGEOK messagein;

    for(int i=0; i<readSelectLength; i++)
    {
        messagein.packet[i]=block[i];
    }
    messagein.packetLength=readSelectLength;

    return messagein;
}


void junbo_light_control::junbo_manual_timer_group()
{

    if(junbo_switch_button)
    {
        struct sigevent event_junbo_manual;
        event_junbo_manual=event_fillvalue(0);
        if ( timer_create( CLOCK_REALTIME, & event_junbo_manual, & timer_junbo_manual ) ) exit( 1 );
        spec_junbo_manual=timer_spec_fillvalue(0,3,3,0);
        timer_settime(timer_junbo_manual,0,&spec_junbo_manual,NULL);
    }
    else
    {
        timer_delete(timer_junbo_manual);
    }

}

sigevent junbo_light_control::event_fillvalue(int case_no)
{
    sigevent fillobject;
    fillobject.sigev_notify = SIGEV_SIGNAL;
    fillobject.sigev_signo = rev_manual_timer_signal;
    fillobject.sigev_value.sival_int = case_no;
    return fillobject;

}
itimerspec junbo_light_control::timer_spec_fillvalue(int start_sec,float start_nsec,int interval_sec,int interval_nsec)
{
    itimerspec fillobject;
    fillobject.it_value.tv_sec=start_sec;
    fillobject.it_value.tv_nsec=start_nsec;
    fillobject.it_interval.tv_sec=interval_sec;
    fillobject.it_interval.tv_nsec=interval_nsec;
    return fillobject;
}


void junbo_light_control::junbol_light_5F7E()
{

    unsigned char    data[3];
    data[0]=0x5F;
    data[1]=0x7E;
    data[2]=junbo_switch_button;

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data,3, true);
    _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

}
void junbo_light_control::junbol_light_5F7D(unsigned char input_switch_button)
{
    junbo_switch_button=input_switch_button;

    unsigned char data[6];

    MESSAGEOK _MsgOK;

    int iReturnCommandSet = smem.vGet0FCommandSet();                              //Get Oper Level
    if(iReturnCommandSet <= 1)
    {

        data[0] = 0x0F;
        data[1] = 0x81;
        data[2] = 0x5F;
        data[3] = 0x7D;
        data[4] = 0x80;
        data[5] = 0x00;


        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 6,true);
        _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    }
    else
    {



        data[0] = 0x0F;
        data[1] = 0x80;
        data[2] = 0x5F;
        data[3] = 0X7D;

        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,true);
        _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);


    }



    junbol_light_5F7E();


}
void junbo_light_control::junbol_light_5F7C()
{

    unsigned char data[27];
    unsigned char num_light=0;
    int countID[8];

    for(int query_count=0; query_count<2; query_count++)
    {

        memset(countID,0,sizeof(int));
        num_light=0;


        for(int i=1; i<9; i++)
        {
            if(smem.record_light[i].ID==i)
            {
                countID[num_light]=i;
                num_light++;

            }
        }
        data[2]=num_light;
        data[	0	]=0x5F;
        data[	1	]=0x7C;
//   data[2]=0x8;//
        for(int i=1; i<num_light+1; i++)
        {
            if(query_count==0)
            {
                data[i*3]=smem.record_state[countID[i-1]].ID;
                data[i*3+1]=smem.record_state[countID[i-1]].command;
                data[i*3+2]=smem.record_state[countID[i-1]].parameter;
            }
            else
            {
                data[i*3]=smem.record_light[countID[i-1]].ID;
                data[i*3+1]=smem.record_light[countID[i-1]].command;
                data[i*3+2]=smem.record_light[countID[i-1]].parameter;
            }
        }

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, (3+3*num_light), true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        usleep(50000);
    }

}

void junbo_light_control::junbol_light_5F7B()
{
    unsigned char data[27];
    unsigned char num_light=0;
    int countID[8];

    for(int query_count=0; query_count<2; query_count++)
    {

        memset(countID,0,sizeof(int));
        num_light=0;


        for(int i=1; i<9; i++)
        {
            if(smem.record_light[i].ID==i)
            {
                countID[num_light]=i;
                num_light++;

            }
        }
        data[2]=num_light;
        data[	0	]=0x5F;
        data[	1	]=0x7B;
//   data[2]=0x8;//
        for(int i=1; i<num_light+1; i++)
        {
            if(query_count==0)
            {
                data[i*3]=smem.record_state[countID[i-1]].ID;
                data[i*3+1]=smem.record_state[countID[i-1]].command;
                data[i*3+2]=smem.record_state[countID[i-1]].parameter;
            }
            else
            {
                data[i*3]=smem.record_light[countID[i-1]].ID;
                data[i*3+1]=smem.record_light[countID[i-1]].command;
                data[i*3+2]=smem.record_light[countID[i-1]].parameter;
            }
        }

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, (3+3*num_light), true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        usleep(50000);
    }

}

void junbo_light_control::junbol_light_5F7A()//query
{


    query_modual_state();

    junbol_light_5F7B();
}


void junbo_light_control::junbo_light_send_reference_step(unsigned short int junbo_step)
{
    try
    {
        pthread_mutex_lock(&junbo_light_control::_junbo_mutex);

        unsigned char ucSendTMP[6];
        //memset();
        switch(junbo_step)
        {
        case(0):
            for(int i=1; i<9; i++)
                light_ID[i]=light_off;
            break;
        case(1):
            for(int i=1; i<9; i++)
                light_ID[i]=light_off;
            break;
        case(2):

            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;
            light_ID[3]=red;
            light_ID[4]=green;
            break;

        case(3):
            ucSendTMP[0] = 0xAA;//head
            ucSendTMP[1] = 0x0;//sequence
            ucSendTMP[2] = 4;//ID
            ucSendTMP[3] = green.command;
            ucSendTMP[4] =green.parameter;
            ucSendTMP[5] = 0x0;//cks
            for (int a=0; a<5; a++)
                ucSendTMP[5]^=ucSendTMP[a];
            junbo_light_send(ucSendTMP);
            printf("green_right\n");
            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;
            light_ID[3]=red;
            light_ID[4]=flash_on;

            break;
        case(4):

            /*    ucSendTMP[0] = 0xAA;//head
                ucSendTMP[1] = 0x0;//sequence
                ucSendTMP[2] = 4;//ID
                ucSendTMP[3] = flash_off.command;
                ucSendTMP[4] =flash_off.parameter;
                ucSendTMP[5] = 0x0;//cks
                for (int a=0; a<5; a++)
                    ucSendTMP[5]^=ucSendTMP[a];
                junbo_light_send(ucSendTMP);
                printf("flash off\n");*/


            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;
            light_ID[3]=red;
            light_ID[4]=red;
            break;

        case(5):
            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;
            light_ID[3]=green;
            light_ID[4]=red;
            break;

        case(6)://when junbolight want to flash, it should be light on before flash.
            ucSendTMP[0] = 0xAA;//head
            ucSendTMP[1] = 0x0;//sequence
            ucSendTMP[2] = 3;//ID
            ucSendTMP[3] = green.command;
            ucSendTMP[4] =green.parameter;
            ucSendTMP[5] = 0x0;//cks
            for (int a=0; a<5; a++)
                ucSendTMP[5]^=ucSendTMP[a];
            junbo_light_send(ucSendTMP);
            printf("green_right\n");


            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;
            light_ID[3]=flash_on;
            light_ID[4]=red;

            break;
        case(7):
            /*
                        ucSendTMP[0] = 0xAA;//head
                        ucSendTMP[1] = 0x0;//sequence
                        ucSendTMP[2] = 3;//ID
                        ucSendTMP[3] = flash_off.command;
                        ucSendTMP[4] =flash_off.parameter;
                        ucSendTMP[5] = 0x0;//cks
                        for (int a=0; a<5; a++)
                            ucSendTMP[5]^=ucSendTMP[a];
                        junbo_light_send(ucSendTMP);
                        printf("flash off\n");
            */




            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;
            light_ID[3]=red;
            light_ID[4]=red;

            break;

        case(8):
            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;
            light_ID[3]=red;
            light_ID[4]=green;

            break;

        case(11):
            for(int i=1; i<9; i++)
                light_ID[i]=light_off;

            break;
        case(12):
            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;
            light_ID[3]=green;
            light_ID[4]=red;


        case(13)://when junbolight want to flash, it should be light on before flash.
            ucSendTMP[0] = 0xAA;//head
            ucSendTMP[1] = 0x0;//sequence
            ucSendTMP[2] = 3;//ID
            ucSendTMP[3] = green.command;
            ucSendTMP[4] =green.parameter;
            ucSendTMP[5] = 0x0;//cks
            for (int a=0; a<5; a++)
                ucSendTMP[5]^=ucSendTMP[a];
            junbo_light_send(ucSendTMP);
            printf("green_right\n");


            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;


            light_ID[3]=flash_on;
            light_ID[4]=red;

            break;

        case(14):
            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;

            /*     ucSendTMP[0] = 0xAA;//head
                 ucSendTMP[1] = 0x0;//sequence
                 ucSendTMP[2] = 3;//ID
                 ucSendTMP[3] = flash_off.command;
                 ucSendTMP[4] =flash_off.parameter;
                 ucSendTMP[5] = 0x0;//cks
                 for (int a=0; a<5; a++)
                     ucSendTMP[5]^=ucSendTMP[a];
                 printf("flash off\n");
                 junbo_light_send(ucSendTMP);*/



            light_ID[3]=red;
            light_ID[4]=red;

            break;
        case(15):

            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;
            light_ID[3]=red;
            light_ID[4]=green;

            break;
        case(16):

            ucSendTMP[0] = 0xAA;//head
            ucSendTMP[1] = 0x0;//sequence
            ucSendTMP[2] = 4;//ID
            ucSendTMP[3] = green.command;
            ucSendTMP[4] =green.parameter;
            ucSendTMP[5] = 0x0;//cks
            for (int a=0; a<5; a++)
                ucSendTMP[5]^=ucSendTMP[a];
            junbo_light_send(ucSendTMP);
            printf("green_right\n");

            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;



            light_ID[3]=red;
            light_ID[4]=flash_on;

            break;
        case(17):
            /*   ucSendTMP[0] = 0xAA;//head
               ucSendTMP[1] = 0x0;//sequence
               ucSendTMP[2] = 4;//ID
               ucSendTMP[3] = flash_off.command;
               ucSendTMP[4] =flash_off.parameter;
               ucSendTMP[5] = 0x0;//cks
               for (int a=0; a<5; a++)
                   ucSendTMP[5]^=ucSendTMP[a];
               printf("flash off\n");
               junbo_light_send(ucSendTMP);*/

            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;
            light_ID[3]=red;
            light_ID[4]=red;

            break;
        case(18):
            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;
            light_ID[3]=green;
            light_ID[4]=red;

            break;

        default:
            for(int i=0; i<8; i++)
                light_ID[i]=light_off;

            break;

        }



        if((default_dir_mark!=0)&&((junbo_step==0)||(junbo_step==1)||(junbo_step==11)))
        {
            switch (default_dir_mark)
         {

            case (1):
            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;
            light_ID[3]=green;
            light_ID[4]=red;
            break;
        case (2):
            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;
            light_ID[3]=red;
            light_ID[4]=green;

            break;
        case (3):
            light_ID[1]=light_ID[6]=light_ID[8]=red;
            light_ID[2]=light_ID[5]=light_ID[7]=green;
            light_ID[3]=red;
            light_ID[4]=red;
            break;
        default:
            for(int i=0; i<8; i++)
                light_ID[i]=light_off;
            break;
        }
        }
        else ;

        for(int i=1; i<9; i++)
        {
            //int i=4;
            ucSendTMP[0] = 0xAA;//head
            ucSendTMP[1] = 0x0;//sequence
            ucSendTMP[2] = i;//ID
            ucSendTMP[3] = light_ID[i].command;
            ucSendTMP[4] = light_ID[i].parameter;
            ucSendTMP[5] = 0x0;//cks
            for (int a=0; a<5; a++)
                ucSendTMP[5]^=ucSendTMP[a];
            printf("\nlight ID=%d\n",i);
            junbo_light_send(ucSendTMP);
        }

        pthread_mutex_unlock(&junbo_light_control::_junbo_mutex);
        printf("rev_step=%d\n",junbo_step);
        printf("Send junbo light.by vLightRevSingal (ReverseTime)\n");

        /*-----------------*/

    }
    catch(...) {}
}



void junbo_light_control::junbo_light_send(unsigned char junbo_send_packet[6])
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



    junbo_send_packet[1]=0;//adjust seq
    FILE *pf=NULL;

    printf("\n junbo_command=%2x parameter=%2x\n",junbo_send_packet[3],junbo_send_packet[4]);
    junbo_send_packet[5]=0x0;
    for(int i=0; i<5; i++)
    {
        junbo_send_packet[5]^=junbo_send_packet[i];
    }

    if((junbo_send_packet[3]==0xc1)&&(junbo_send_packet[3]==0xc3)&&(junbo_send_packet[3]==0xc0))
    {
        ID=junbo_send_packet[2];
        smem.record_light[ID].ID=0;
        smem.record_light[ID].command=0;//junbo_receive_packet[2]   I am ID
        smem.record_light[ID].parameter=0;
    }
    REV_light_port.Rs232Write(junbo_send_packet,6,tty_name);//write out
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
//    sleep(1);//can't be too fast to avoid to lose return message.
    // junbo_light_receive(junbo_send_packet[2]);
}

void junbo_light_control::junbo_light_receive(MESSAGEOK messageIn)//just for receive the junbo_light return message
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


            if(junbo_receive_packet[2]>=1&&junbo_receive_packet[2]<=8)
            {
                if((junbo_receive_packet[3]==0xb1)
                        ||(junbo_receive_packet[3]==0xb0)
                        ||(junbo_receive_packet[3]==0xb3))
                {
                    ID=junbo_receive_packet[2];
                    smem.record_light[ID].ID=junbo_receive_packet[2];
                    smem.record_light[ID].command=junbo_receive_packet[3];//junbo_receive_packet[2]   I am ID
                    smem.record_light[ID].parameter=junbo_receive_packet[4];
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
                else
                {
                    ID=junbo_receive_packet[2];
                    smem.record_state[ID].ID=junbo_receive_packet[2];
                    smem.record_state[ID].command=junbo_receive_packet[3];//junbo_receive_packet[2]   I am ID
                    smem.record_state[ID].parameter=junbo_receive_packet[4];
                }

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

bool junbo_light_control::ParseBlock(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex)
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
                junbo_light_receive(messageIn[j]);    //save log whitch is wrong message
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
                    junbo_light_receive(messageIn[j]);    //save log
                    vClearMsg(messageIn,j);
                    k=0;
                }
                else
                {
                    junbo_light_receive(messageIn[j]);    //save log whitch is wrong message
                    vClearMsg(messageIn,j);
                    k=0;
                };

            }
        }
    }

    *lastPacketIndex=k;
    return true;
}

bool junbo_light_control::vClearMsg(MESSAGEOK *messageIn, unsigned short int msgID)
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


void junbo_light_control::query_modual_state()
{
    pthread_mutex_lock(&junbo_light_control::_junbo_mutex);

    unsigned char ucSendTMP[6];


    for(int i=1; i<9; i++)
    {
        smem.record_state[i].ID=0;
        smem.record_state[i].command=0;
        smem.record_state[i].parameter=0;
        //int i=4;
        ucSendTMP[0] = 0xAA;//head
        ucSendTMP[1] = 0x1;//sequence
        ucSendTMP[2] = i;//ID
        ucSendTMP[3] = query.command;
        ucSendTMP[4] = query.parameter;
        ucSendTMP[5] = 0x0;//cks
        for (int a=0; a<5; a++)
            ucSendTMP[5]^=ucSendTMP[a];
        printf("\nquery light ID=%d\n",i);
        junbo_light_send(ucSendTMP);
    }

    pthread_mutex_unlock(&junbo_light_control::_junbo_mutex);

    printf("Send junbo light query.by query_modual_state\n");

    /*-----------------*/

}
void junbo_light_control::brightness_control(int bright_parameter)
{
    pthread_mutex_lock(&junbo_light_control::_junbo_mutex);

    unsigned char ucSendTMP[6];


    for(int i=1; i<9; i++)
    {
        smem.record_brightness[i].ID=0;
        smem.record_brightness[i].command=0;
        smem.record_brightness[i].parameter=0;
        //int i=4;
        ucSendTMP[0] = 0xAA;//head
        ucSendTMP[1] = 0x2;//sequence
        ucSendTMP[2] = i;//ID
        ucSendTMP[3] = brightness[bright_parameter].command;
        ucSendTMP[4] = brightness[bright_parameter].parameter;
        ucSendTMP[5] = 0x0;//cks
        for (int a=0; a<5; a++)
            ucSendTMP[5]^=ucSendTMP[a];
        printf("\nbrightness_control light ID=%d\n",i);
        junbo_light_send(ucSendTMP);
    }

    pthread_mutex_unlock(&junbo_light_control::_junbo_mutex);

    printf("Send junbo light bright ness control.by brightness_control\n");
}

void junbo_light_control::light_timeout_control(int control_parameter)
{
    pthread_mutex_lock(&junbo_light_control::_junbo_mutex);

    unsigned char ucSendTMP[6];


    for(int i=1; i<9; i++)
    {
        smem.record_timeout[i].ID=11;
        smem.record_timeout[i].command=11;
        smem.record_timeout[i].parameter=11;
        //int i=4;
        ucSendTMP[0] = 0xAA;//head
        ucSendTMP[1] = 0x2;//sequence
        ucSendTMP[2] = i;//ID
        ucSendTMP[3] = light_timeout[control_parameter].command;
        ucSendTMP[4] = light_timeout[control_parameter].parameter;
        ucSendTMP[5] = 0x0;//cks
        for (int a=0; a<5; a++)
            ucSendTMP[5]^=ucSendTMP[a];
        printf("\nlight_timeout_control light ID=%d\n",i);
        junbo_light_send(ucSendTMP);
    }

    pthread_mutex_unlock(&junbo_light_control::_junbo_mutex);

    printf("Send junbo light light_timeout_control.by light_timeout_control\n");
}

void junbo_light_control::report_light_brightness()
{

    unsigned char data[27];

    data[	0	]=0x5F;
    data[	1	]=0xb3;
    pthread_mutex_lock(&junbo_light_control::_junbo_mutex);
    for(int i=1; i<9; i++)
    {

        data[3*i-1]=smem.record_light[i].ID;
        data[3*i]=smem.record_light[i].command;
        data[3*i+1]=smem.record_light[i].parameter;
    }
    writeJob.WritePhysicalOut(data, 26, revAPP);
    pthread_mutex_unlock(&junbo_light_control::_junbo_mutex);


}


void junbo_light_control::report_light_timeout()
{

    unsigned char data[27];

    data[	0	]=0x5F;
    data[	1	]=0xb6;
    pthread_mutex_lock(&junbo_light_control::_junbo_mutex);
    for(int i=1; i<9; i++)
    {
        data[3*i-1]=smem.record_timeout[i].ID;
        data[3*i]=smem.record_timeout[i].command;
        data[3*i+1]=smem.record_timeout[i].parameter;
    }
    writeJob.WritePhysicalOut(data, 26, revAPP);
    pthread_mutex_unlock(&junbo_light_control::_junbo_mutex);


}
void junbo_light_control::report_module_state_to_revapp()
{
    unsigned char data[27];

    data[	0	]=0x5F;
    data[	1	]=0xb7;
    pthread_mutex_lock(&junbo_light_control::_junbo_mutex);
    for(int i=1; i<9; i++)
    {
        data[3*i-1]=smem.record_state[i].ID;
        data[3*i]=smem.record_state[i].command;
        data[3*i+1]=smem.record_state[i].parameter;
    }
    writeJob.WritePhysicalOut(data, 26, revAPP);
    pthread_mutex_unlock(&junbo_light_control::_junbo_mutex);
}
