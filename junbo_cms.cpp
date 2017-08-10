#include "junbo_cms.h"
#include "SMEM.h"
junbo_cms::junbo_cms()
{
    //ctor
}

junbo_cms::~junbo_cms()
{
    //dtor
}



void junbo_cms::initial_junbo_control(char *text_record_location,char *output_tty_name)
{
    tty_name=output_tty_name;

    if (junbo_cms_port.OpenRs232Port(tty_name, 9600, false)>0)
    {
        printf("open REV_light_port-%s  Success!!\n",tty_name);
    }
    else printf("open REV_light_port Fail!!\n");


    light_off.command=0xc0;
    light_off.parameter=0x0;

    for(int i=1;i<=4;i++)
    {
        notice_car[i].command=0xc1;
        notice_car[i].parameter=i;
    }

    for(int i=0; i<11; i++)
    {
        light_timeout[i].command=0xc2;
        light_timeout[i].parameter=i;
    }

    flash_on.command=0xc3;
    flash_on.parameter=0x01;

    flash_off.command=0xc3;
    flash_off.parameter=0x0;

    for(int i=0;i<4;i++)
    {
    query[i].command=0xc4;
    query[i].parameter=i;
    }


    for(int i=0; i<4; i++)
    {
        brightness[i].command=0xc5;
        brightness[i].parameter=i+1;
    }

    filename[80]='0';
    sprintf(filename,text_record_location);



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
         junbo_send_packet[0]=0xaa;
junbo_send_packet[1]=smem.GetSequence();
junbo_send_packet[2]=0x1;
junbo_send_packet[3]=notice_car[textID].command;
junbo_send_packet[4]=notice_car[textID].parameter;




printf("VD TEST MESSAGE ,I RECEIVE THE VD MESSAGE\n");
junbo_cms_send(junbo_send_packet);
     }catch(...){}
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

               /* if((junbo_receive_packet[3]==0xb1)
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
                }*/


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


void junbo_cms::brightness_control(int bright_parameter)
{
try

   {

    unsigned char ucSendTMP[6];


        int i=0;
        ucSendTMP[0] = 0xAA;//head
        ucSendTMP[1] = 0x2;//sequence
        ucSendTMP[2] = i;//ID
        ucSendTMP[3] = brightness[bright_parameter].command;
        ucSendTMP[4] = brightness[bright_parameter].parameter;
        ucSendTMP[5] = 0x0;//cks
        for (int a=0; a<5; a++)
            ucSendTMP[5]^=ucSendTMP[a];
        printf("\nbrightness_control light ID=%d\n",i);
        junbo_cms_send(ucSendTMP);


    printf("Send junbo cms brightness control.by brightness_control\n");

   }catch(...){}
}
void junbo_cms::query_modual_state(int query_block)
{
  try
    {unsigned char ucSendTMP[6];

       /* smem.record_state[i].ID=0;
        smem.record_state[i].command=0;
        smem.record_state[i].parameter=0;*/
        int i=0;
        ucSendTMP[0] = 0xAA;//head
        ucSendTMP[1] = 0x1;//sequence
        ucSendTMP[2] = i;//ID
        ucSendTMP[3] = query[query_block].command;
        ucSendTMP[4] = query[query_block].parameter;
        ucSendTMP[5] = 0x0;//cks
        for (int a=0; a<5; a++)
            ucSendTMP[5]^=ucSendTMP[a];
        printf("\nquery light ID=%d\n",i);
        junbo_cms_send(ucSendTMP);


    printf("Send junbo light query.by query_modual_state\n");

    /*-----------------*/
    }catch(...){}
}
/*
void junbo_cms::report_light_timeout()//for app
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


}*/

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
    catch(...){}



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
{try{int tempmax=0;
     if (tempmax=junbo_cms_port.OpenRs232Port(tty_name, 9600, false)>0)
    {
        printf("open junbo_cms_port-%s  Success!!\n",tty_name);
    }
    else printf("open junbo_cms_port Fail!!\n");

    return tempmax;
}catch(...){}
}
