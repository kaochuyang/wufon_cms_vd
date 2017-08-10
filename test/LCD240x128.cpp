//---------------------------------------------------------------------------

//#define SAYNO_LCD
#define TOSHIBA_LCD

#include "LCD240x128.h"

#include <stdio.h>
#include <sys/perm.h>
#include <sys/io.h>
#include<unistd.h>
//OT20140314
#include "SMEM.h"

//for sanyo
#define E_1   outb(temp &=0x0d,CONTROL_PORT);
#define E_0   outb(temp |=0x02,CONTROL_PORT);
#define RS_1  outb(temp &=0x0e,CONTROL_PORT);
#define RS_0  outb(temp |=0x01,CONTROL_PORT);

//for toshiba
#define WR_H  outb(temp &=0x0d,CONTROL_PORT);  //old pin is E  //for LC7981
#define WR_L  outb(temp |=0x02,CONTROL_PORT);
#define CD_H  outb(temp &=0x0e,CONTROL_PORT);  //old pin is RS for LC7981
#define CD_L  outb(temp |=0x01,CONTROL_PORT);
#define RD_H  outb(temp &=0x0b,CONTROL_PORT);  //
#define RD_L  outb(temp |=0x04,CONTROL_PORT);
#define CE_H  outb(temp &=0x07,CONTROL_PORT);  //
#define CE_L  outb(temp |=0x08,CONTROL_PORT);

LCD240x128 lcd240x128;

//---------------------------------------------------------------------------
LCD240x128::LCD240x128(void)
{
    try
    {

        pthread_mutex_init(&mutexLcd,NULL);

        DATA_PORT=0x378;                      //Default IO��m:DATA PORT
        CONTROL_PORT=0x37A;                   //Default IO��m:CONTROL PORT
// #ICOPBUG
//    DATA_PORT=0x278;                      //Default IO��m:DATA PORT
//    CONTROL_PORT=0x27A;                   //Default IO��m:CONTROL PORT

        lcdWidth=240;                         //Lcd�e��pixel
        lcdHeight=128;                        //Lcd����pixel
        haveGetPower=false;                   //�|���o�챱��LPT���v��
        defaultSignal=0;
        temp = 0;

    }
    catch(...) {}
}
//---------------------------------------------------------------------------
LCD240x128::~LCD240x128 (void)
{
}

//---------------------------------------------------------------------------
bool LCD240x128::ReleaseAuthority(void)
{
    try
    {
        if (haveGetPower) outb(defaultSignal,CONTROL_PORT);                         //�٭�CONTROL_PORT Default Status
        if (ioperm(DATA_PORT,3,0)==0)         //���񱱨��v
        {
            haveGetPower=false;               //turn off the flag
            return true;
        }
        else return false;
    }
    catch(...) {}
}

//---------------------------------------------------------------------------
unsigned char LCD240x128::Invert(unsigned char data)
{
    try
    {
        return (
                   ((data & 0x80) >> 7 )  /* bit7 --> bit0 */
                   +((data & 0x40) >> 5 )  /* bit6 --> bit1 */
                   +((data & 0x20) >> 3 )  /* bit5 --> bit2 */
                   +((data & 0x10) >> 1 )  /* bit4 --> bit3 */
                   +((data & 0x08) << 1 )  /* bit3 --> bit4 */
                   +((data & 0x04) << 3 )  /* bit2 --> bit5 */
                   +((data & 0x02) << 5 )  /* bit1 --> bit6 */
                   +((data & 0x01) << 7 )  /* bit0 --> bit7 */
               );
    }
    catch(...) {}
}
//---------------------------------------------------------------------------
//�p�G�n��ܤ@��16*16���r�N�O heightBit=16,widthByte=2
//�p�G�n��ܤ@��24*24���r�N�O heightBit=24,widthByte=3
//�p�G�n��ܾ��240*128���e���N�O heightBit=128,widthByte=30
//�ù��W���@�I�}�l���_����
//�p�G�n�q��(0,0)�I���_ram_address=0
//�p�G�n�q��(X,Y)�I���_ram_address=X/8+30*Y
//�p�G�n�q��(16,0)�I���_ram_address=16/8+30*0=2
//�p�G�n�q��(24,0)�I���_ram_address=24/8+30*0=3
//�p�G�n�q��(0,16)�I���_ram_address=0/8+30*16=480
//�p�G�n�q��(0,24)�I���_ram_address=0/8+30*24=720

//#ifdef SAYNO_LCD
#ifdef TOSHIBA_LCD
//---------------------------------------------------------------------------
bool LCD240x128::GetAuthority(unsigned long ioAddress)
{
    try
    {
        unsigned char ucLCDSwitch = 1;
//    ucLCDSwitch = smem.vGetLCDVersion();
        if(ucLCDSwitch == 0)
        {
            if (ioperm(ioAddress,3,1)==0)         //�o�챱���v
            {
                DATA_PORT=ioAddress;              //BASEPORT
                CONTROL_PORT=ioAddress+2;         //CONTROL_PORT=BASEPORT+2
                haveGetPower=true;                //turn on the flag
                defaultSignal=inb(CONTROL_PORT);  //�o��CONTROL_PORT Default Status,�������٭�
                outb(3,CONTROL_PORT);             //Set Bit0=1,Bit1=1
                return true;
            }
            else return false;

        }
        else if (ucLCDSwitch == 1)
        {
            if (ioperm(ioAddress,3,1)==0)         //�o�챱���v
            {
                DATA_PORT=ioAddress;              //BASEPORT
                CONTROL_PORT=ioAddress+2;         //CONTROL_PORT=BASEPORT+2
                haveGetPower=true;                //turn on the flag
                defaultSignal=inb(CONTROL_PORT);  //�o��CONTROL_PORT Default Status,�������٭�

                // printf("defaultSignal:%0X\n", defaultSignal);


                // outb(3,CONTROL_PORT);             //Set Bit0=1,Bit1=1

                GRAPHIC_MODE();

                return true;
            }
            else return false;
        }
    }
    catch(...) {}
}

//---------------------------------------------------------------------------
void LCD240x128::WRITE_PORT_SANYO(unsigned char C,unsigned char D)
{
    try
    {
        RS_1;                                 //outb(temp &=0x0e,CONTROL_PORT);
        outb(C,DATA_PORT);                    /* output code */
        E_1;                                  //outb(temp &=0x0d,CONTROL_PORT);
        E_0;                                  //outb(temp |=0x02,CONTROL_PORT);
        //while(0);
        RS_0;                                 //outb(temp |=0x01,CONTROL_PORT);
        outb(D,DATA_PORT);                    /* output data */
        E_1;                                  //outb(temp &=0x0d,CONTROL_PORT);
        E_0;                                  //outb(temp |=0x02,CONTROL_PORT);
    }
    catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::CURSOR_SANYO(unsigned short int ram_address) //�w��:��Function����
{
    try
    {
        unsigned char LSB=ram_address & 0x00ff;
        unsigned char MSB=(ram_address>>8) & 0x00ff;
        WRITE_PORT_SANYO(10,LSB);
        WRITE_PORT_SANYO(11,MSB);
    }
    catch(...) {}
}
//---------------------------------------------------------------------------
bool LCD240x128::CLEAR_SANYO(void)                //call��function�N�|�M�����
{
    try
    {
        if (haveGetPower)
        {
            CURSOR_SANYO(0);
            for (int i=0; i<0x2000; i++)
                WRITE_PORT_SANYO(12,0);
            return true;
        }
        else
        {
            GetAuthority(DATA_PORT);
            return false;
        }
    }
    catch(...) {}
}
//---------------------------------------------------------------------------
bool LCD240x128::GRAPHIC_MODE(void)       //call��function�N�����ܹϫ��Ҧ�
{
    try
    {
        unsigned char ucLCDSwitch = 1;
//    ucLCDSwitch = smem.vGetLCDVersion();
        if(ucLCDSwitch == 0)
        {
            if (haveGetPower)
            {
                WRITE_PORT_SANYO(0,0x12);               //�Ҧ�����
                WRITE_PORT_SANYO(1,0x77);               //�r�����Z
                WRITE_PORT_SANYO(2,29);                 //�r����
                WRITE_PORT_SANYO(3,127);                //�ɶ����μ�
                WRITE_PORT_SANYO(4,0x7);                //��Ц�m
                WRITE_PORT_SANYO(0,0x32);               //�Ҧ�����
                WRITE_PORT_SANYO(8,0);                  //�C�ҩl��m
                WRITE_PORT_SANYO(9,0);                  //���ҩl��m
                WRITE_PORT_SANYO(10,0);                 //�CRAM��m
                WRITE_PORT_SANYO(11,0);                 //��RAM��m
                return true;
            }
            else
            {
                GetAuthority(DATA_PORT);
                return false;
            }

        }
        else if(ucLCDSwitch == 1)
        {
            if (haveGetPower)
            {
                //set text address
                WRITE_DATA2_TOSHIBA(0x00, 0x00);
                WRITE_CMD_TOSHIBA(0x40);

                //set graphic address
                WRITE_DATA2_TOSHIBA(0x00, 0x40);  //0x1000
                WRITE_CMD_TOSHIBA(0x42);

                //text area colume set
                WRITE_DATA2_TOSHIBA(30, 0x00);
                WRITE_CMD_TOSHIBA(0x41);

                //graphics area colume set
                WRITE_DATA2_TOSHIBA(30, 0x00);
                WRITE_CMD_TOSHIBA(0x43);

                //mode set (set display mode, graphic)
                WRITE_CMD_TOSHIBA(0x80);
                WRITE_CMD_TOSHIBA(0x98);

                // FULLON();

                // printf("get authority ok\n");
                return true;
            }
            else
            {
                GetAuthority(DATA_PORT);
                return false;
            }
        }
    }
    catch(...) {}
}
//---------------------------------------------------------------------------
bool LCD240x128::CHARACTER_MODE_SANYO(void)     //call��function�N�����ܤ�r�Ҧ�
{
    try
    {
        if (haveGetPower)
        {
            WRITE_PORT_SANYO(0,0x1C);               //�Ҧ�����
            WRITE_PORT_SANYO(1,0x77);               //�r�����Z
            WRITE_PORT_SANYO(2,39);                 //�r����
            WRITE_PORT_SANYO(3,127);                //�ɶ����μ�
            WRITE_PORT_SANYO(4,0x7);                //��Ц�m
            WRITE_PORT_SANYO(0,0x3C);               //�Ҧ�����
            WRITE_PORT_SANYO(8,0);                  //�C�ҩl��m
            WRITE_PORT_SANYO(9,0);                  //���ҩl��m
            WRITE_PORT_SANYO(10,0);                 //�CRAM��m
            WRITE_PORT_SANYO(11,0);                 //��RAM��m
            return true;
        }
        else
        {
            GetAuthority(DATA_PORT);
            return false;
        }
    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool LCD240x128::DISPLAY_GRAPHIC(unsigned int ram_address,unsigned char * font_address,int heightBit,int widthByte)
{
    //�q�ù��W���@�I�}�l���_,��ܪ�bitmap       ,�n��ܪ��ϰ��X��bit,�n��ܪ��ϼe�X��byte
    try
    {
        unsigned char *font_addressInput;
        unsigned int ram_addressInput;
        unsigned char ucTmp;

        unsigned char ucLCDSwitch = 1;
//    ucLCDSwitch = smem.vGetLCDVersion();
        if(ucLCDSwitch == 0)
        {

            pthread_mutex_lock(&mutexLcd);

            GRAPHIC_MODE();

            CLEAR_SANYO();                     //����O����M��

            for(int iLoop = 0; iLoop < 2; iLoop++)    //ot add for ICOP
            {
                font_addressInput = font_address;
                ram_addressInput = ram_address;

                CURSOR_SANYO(ram_address);
                for (int i=0; i<heightBit; i++)                         //heightBit,�n��ܪ��ϰ��X��bit
                {
                    for (int j=0; j<widthByte; j++)                    //widthByte,�n��ܪ��ϼe�X��byte
                    {
                        WRITE_PORT_SANYO(12,Invert(*font_addressInput));
                        font_addressInput++;
                    }
                    CURSOR_SANYO(ram_addressInput=ram_addressInput+30);
                }
            }

            pthread_mutex_unlock(&mutexLcd);

        }
        else if(ucLCDSwitch == 1)
        {
            if(haveGetPower)
            {

                pthread_mutex_lock(&mutexLcd);

                GRAPHIC_MODE();

                CLEAR_TOSHIBA();                     //����O����M��

                ram_address += 0x4000;  //in this system, default graphic address is 0x4000.//20170317

                font_addressInput = font_address;
                ram_addressInput = ram_address;

                CURSOR_TOSHIBA(ram_addressInput);

                WRITE_CMD_TOSHIBA(0xB0);  //auto write start

                for (int i=0; i<heightBit; i++)                         //heightBit,�n��ܪ��ϰ��X��bit
                {

//        WRITE_CMD(0xB0);  //auto write start
                    for (int j=0; j<widthByte; j++)                    //widthByte,�n��ܪ��ϼe�X��byte
                    {
                        //? ucTmp = Invert(*font_addressInput);
                        ucTmp = (*font_addressInput);
                        WRITE_DATA_TOSHIBA(ucTmp);

                        // printf("ucTmp:%0x, font_addressInput:%0x, add:%0000x\n", ucTmp, *font_addressInput, ram_addressInput);
                        font_addressInput++;

                        ram_addressInput++;
                        // getchar();
                    }
//        WRITE_CMD(0xB2);  //auto write start
//        CURSOR(ram_addressInput=ram_addressInput+30);
                }
                WRITE_CMD_TOSHIBA(0xB2);  //auto write stop


                pthread_mutex_unlock(&mutexLcd);

            }
        }

        return true;
    }
    catch(...) {}
}
//---------------------------------------------------------------------------
bool LCD240x128::DISPLAY_GRAPHIC_XY(int X,int Y,unsigned char * font_address,int heightBit,int widthByte)
{
    try
    {
        int ram_address;
        unsigned char *font_addressInput;
        unsigned char ucTmp;

        unsigned char ucLCDSwitch = 1;
//    ucLCDSwitch = smem.vGetLCDVersion();
        if(ucLCDSwitch == 0)
        {

            GRAPHIC_MODE();  //For ICOP

            pthread_mutex_lock(&mutexLcd);

            for(int iLoop = 0; iLoop < 3; iLoop++)    //ot add for ICOP
            {
                font_addressInput = font_address;

                ram_address=X/8+30*Y;
                CURSOR_SANYO(ram_address);
                for (int i=0; i<heightBit; i++)
                {
                    for (int j=0; j<widthByte; j++)
                    {
                        WRITE_PORT_SANYO(12,Invert(*font_addressInput));
                        font_addressInput++;
                    }
                    CURSOR_SANYO(ram_address=ram_address+30);
                }
            }

            pthread_mutex_unlock(&mutexLcd);

        }
        else if(ucLCDSwitch == 1)
        {

            if(haveGetPower)
            {

                // GRAPHIC_MODE();  //For ICOP

                pthread_mutex_lock(&mutexLcd);

                font_addressInput = font_address;
                ram_address = X/8+30*Y;
                // printf("ram_address:%x\n", ram_address);
                 ram_address += 0x4000; //base address is 00x4000//20170317

                CURSOR_TOSHIBA(ram_address);

                for (int i=0; i<heightBit; i++)
                {

                    // WRITE_CMD_TOSHIBA(0xB0);
                    for (int j=0; j<widthByte; j++)
                    {

                        // ucTmp = Invert(*font_addressInput);
                        ucTmp = *font_addressInput;
                        WRITE_DATA_TOSHIBA(ucTmp);
                        WRITE_CMD_TOSHIBA(0xC0);

                        font_addressInput++;
                    }
                    // WRITE_CMD_TOSHIBA(0xB2);
                    ram_address=ram_address+30;
                    CURSOR_TOSHIBA(ram_address);
                }

                pthread_mutex_unlock(&mutexLcd);

            }

        }

        return true;

    }
    catch(...) {}
}

//---------------------------------------------------------------------------
void LCD240x128::DISPLAY_GRAPHICFULL_SPECIAL1_SANYO(unsigned char * font_address)   //����ĪG
{
    try
    {
        pthread_mutex_lock(&mutexLcd);
        CLEAR_SANYO();
        for (int ram_address=30; ram_address>=0; ram_address--)
            SPECIAL1_SANYO(ram_address,font_address);
        pthread_mutex_unlock(&mutexLcd);
    }
    catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::SPECIAL1_SANYO(int ram_address,unsigned char * font_address)
{
    try
    {
        int temp_ram_address=ram_address;
        CURSOR_SANYO(ram_address);
        for (int i=0; i<128; i++)
        {
            for (int j=0; j<30-temp_ram_address; j++)
            {
                WRITE_PORT_SANYO(12,Invert(*font_address));
                font_address++;
            }
            CURSOR_SANYO(ram_address=ram_address+30);
        }
    }
    catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::DISPLAY_GRAPHICFULL_SPECIAL2_SANYO(unsigned char * font_address)   //�ѥk�ܥ���ܮĪG
{
    try
    {
        pthread_mutex_lock(&mutexLcd);
        CLEAR_SANYO();
        for (int ram_address=30; ram_address>=0; ram_address--)
            SPECIAL2_SANYO(ram_address,font_address);
        pthread_mutex_unlock(&mutexLcd);
    }
    catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::SPECIAL2_SANYO(int ram_address,unsigned char * font_address)
{
    try
    {
        int temp_ram_address=ram_address;
        CURSOR_SANYO(ram_address);
        for (int i=0; i<128; i++)
        {
            for (int j=0; j<30; j++)
            {
                if(j>=temp_ram_address)
                    WRITE_PORT_SANYO(12,Invert(*font_address));
                font_address++;
            }
            CURSOR_SANYO(ram_address=ram_address+30);
        }
    }
    catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::DISPLAY_GRAPHICFULL_SPECIAL3_SANYO(unsigned char * font_address)   //�ѥk�ܥ����i�ĪG
{
    try
    {
        pthread_mutex_lock(&mutexLcd);
        CLEAR_SANYO();
        for (int ram_address=30; ram_address>=0; ram_address--)
            SPECIAL3_SANYO(ram_address,font_address);
        pthread_mutex_unlock(&mutexLcd);
    }
    catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::SPECIAL3_SANYO(int ram_address,unsigned char * font_address)
{
    try
    {
        int temp_ram_address=30-ram_address;
        int count=0;
        CURSOR_SANYO(ram_address);
        for (int i=0; i<128; i++)
        {
            for (int j=0; j<30; j++)
            {
                count++;
                if (count%30<=temp_ram_address)
                    WRITE_PORT_SANYO(12,Invert(*font_address));
                font_address++;
            }
            CURSOR_SANYO(ram_address=ram_address+30);
        }
    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool LCD240x128::CLEAR_TOSHIBA(void)                //call��function�N�|�M�����
{
    try
    {
        if (haveGetPower)
        {
            WRITE_DATA2_TOSHIBA(0x00, 0x40);
            WRITE_CMD_TOSHIBA(0x24);
            WRITE_CMD_TOSHIBA(0xB0);
            for(int i = 0; i < 128; i++)
            {
                for(int j = 0; j < 30; j++)
                {
                    WRITE_DATA_TOSHIBA(0);
                }
            }
            WRITE_CMD_TOSHIBA(0xB2);
        }
        else
        {
            GetAuthority(DATA_PORT);
            return false;
        }
    }
    catch(...) {}
}


//---------------------------------------------------------------------------
void LCD240x128::CURSOR_TOSHIBA(unsigned short int ram_address)  //for toshiba
{
    try
    {

        unsigned char lo, hi;
        unsigned short int usiTmp;

        if(haveGetPower)
        {

            usiTmp = (ram_address >> 8);
            hi = usiTmp & 0xFF;
//    printf("usiTmp:%x, hi:%x\n", usiTmp, hi);
            usiTmp = (ram_address);
            lo = usiTmp & 0xFF;
//    printf("usiTmp:%x, lo:%x\n", usiTmp, lo);
            WRITE_DATA2_TOSHIBA(lo, hi);
            WRITE_CMD_TOSHIBA(0x24);  //set Address

        }
    }
    catch(...) {}
}

//---------------------------------------------------------------------------
void LCD240x128::WRITE_DATA2_TOSHIBA(unsigned char L,unsigned char H)
{
    try
    {
        WRITE_DATA_TOSHIBA(L);
        WRITE_DATA_TOSHIBA(H);
    }
    catch(...) {}
}

//---------------------------------------------------------------------------
void LCD240x128::WRITE_DATA_TOSHIBA(unsigned char C)
{
    try
    {

        CD_L;
        //20170306  outb(temp,CONTROL_PORT);
        CE_L;
        WR_L;
        // usleep(1);
        // nanosleep(1);

        outb(C,DATA_PORT);                    // output code
// nanosleep(80);//20170203
        // for(int i = 0; i < delay_cycle; i++);
        CE_H;
        WR_H;
     //   for(int i = 0; i < 10000; i++);
        //  usleep(10);
// nanosleep(80);//20170203
        // for(int i = 0; i < delay_cycle; i++);

    }
    catch(...) {}
}

//---------------------------------------------------------------------------
void LCD240x128::WRITE_CMD_TOSHIBA(unsigned char C)
{
    try
    {
        CD_H;
        CE_L;
        WR_L;
        outb(C,DATA_PORT);                    // output code
        // for(int i = 0; i < delay_cycle; i++);
//    nanosleep(80);//20170203
        CE_H;
        WR_H;

       // for(int i = 0; i < 10000; i++);


        //  usleep(10);
        //   nanosleep(80);//20170203
        // for(int i = 0; i < delay_cycle; i++);
    }
    catch(...) {}
}


#endif

//---------------------------------------------------------------------------
