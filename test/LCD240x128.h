/*
    LCD�Ҳ� �ϥ�LPT �ؤo:240x128 pixel
    1.�g�@����30*128=3840��BYTE (�@�C240pixel��30��BYTE,�@128�C)
*/
//----------------------------------------------------------------------
#ifndef LCD240x128H
#define LCD240x128H

#include "pthread.h"
//----------------------------------------------------------------------
class LCD240x128
{
    public:
      LCD240x128 (void);
      ~LCD240x128 (void);

      bool GetAuthority(unsigned long);                                         //�o�챱���v
      bool ReleaseAuthority(void);                                              //���񱱨��v
      bool CLEAR_SANYO(void);                                                         //�M�����
      bool GRAPHIC_MODE(void);                                                  //�����ܹϫ��Ҧ�
      bool CHARACTER_MODE_SANYO(void);                                                //�����ܤ�r�Ҧ�

      bool DISPLAY_GRAPHIC(unsigned int,unsigned char *,int,int);               //���GRAPHIC(���`���)
      bool DISPLAY_GRAPHIC_XY(int,int,unsigned char *,int,int);                 //���GRAPHIC(���`���)
      void DISPLAY_GRAPHICFULL_SPECIAL1_SANYO(unsigned char *);                       //���GRAPHIC(�㭱:����ĪG)
      void DISPLAY_GRAPHICFULL_SPECIAL2_SANYO(unsigned char *);                       //���GRAPHIC(�㭱:�ѥk�ܥ���ܮĪG)
      void DISPLAY_GRAPHICFULL_SPECIAL3_SANYO(unsigned char *);                       //���GRAPHIC(�㭱:�ѥk�ܥ����i�ĪG)

      //TOSHIBA
      bool CLEAR_TOSHIBA(void);                                                         //�M�����

    private:

      pthread_mutex_t mutexLcd;                                                 //�O�@�O����
      int lcdWidth;                                                             //LCD���O���e
      int lcdHeight;                                                            //LCD���O����
      unsigned long DATA_PORT;                                                  //IO��m:DATA PORT
      unsigned long CONTROL_PORT;                                               //IO��m:CONTROL PORT
      bool haveGetPower;                                                        //�O�_��o����LPT���v��
      unsigned char defaultSignal;                                              //��LPT�W���T��
      unsigned char temp;

      void WRITE_PORT_SANYO(unsigned char,unsigned char);                             //�gPort
      void CURSOR_SANYO(unsigned short int);                                                //�w��
      unsigned char Invert(unsigned char);                                      //�NBitmap�ഫ���O�I

      void SPECIAL1_SANYO(int,unsigned char *);                                       //���GRAPHIC(�㭱:����ĪG)
      void SPECIAL2_SANYO(int,unsigned char *);                                       //���GRAPHIC(�㭱:�ѥk�ܥ���ܮĪG)
      void SPECIAL3_SANYO(int,unsigned char *);

      //for toshiba
      void CURSOR_TOSHIBA(unsigned short int);
      void WRITE_DATA2_TOSHIBA(unsigned char,unsigned char);
      void WRITE_DATA_TOSHIBA(unsigned char);
      void WRITE_CMD_TOSHIBA(unsigned char);                                       //���GRAPHIC(�㭱:�ѥk�ܥ����i�ĪG)
};
//-------------------------------------------------------------------
extern LCD240x128 lcd240x128;
#endif
