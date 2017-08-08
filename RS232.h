#ifndef RS232_H
#define RS232_H

#include "variable.h"
#include <pthread.h>
//---------------------------------------------------------------------------
class RS232
{
    public:

        RS232(void);
        ~RS232(void);

        BYTE block[BUFFERSIZE];                                                 //rs232 port buffer

        MESSAGEOK messageIn[MSGINSIZE];                                         //±µ¦¬¸ÑªR¦¨¥\ªºbuffer

        int maxMessageIndex;                                                    //¦¹¦¸³B²zªºmessageIn«Ê¥]­Ó¼Æ
        int lastPacketIndex;                                                    //¤W¤@¦¸©ñ¨ì³Ì«á¤@­ÓmessageIn.packet[Index]

        int Getfd(void);                                                        //Àò±ofile description
        bool GetPortAlreadyOpen(void);                                          //§PÂ_¦¹°ð¬O§_¤w¶}±Ò
        int GetConnDevice(void);                                                //Àò±o¦¹°ð©Ò³s±µªº³]³Æ
        bool SetConnDevice(int);                                                //³]©w¦¹°ð©Ò³s±µªº³]³Æ
        int OpenRs232Port(char *,int,bool);                                     //¶}³q°T°ð(device name,baudrate,is even parity)
        bool CloseRs232Port(void);                                              //Ãö³q°T°ð
        int Rs232Read(void);                                                    //Åª³q°T°ð(read buffer)
        bool Rs232Write(BYTE *,int,char *);                                     //¼g³q°T°ð(write array,write length,device name)


    private:

        pthread_mutex_t mutexRS232;                                             //«OÅ@°O¾ÐÅé

        int rs232fd;                                                            //file descript
        bool alreadyOpen;                                                       //¦¹port¬O§_¤w¶}±Ò
        int connDevice;                                                         //¦¹°ð³s±µªº³]³Æ
        int connSubDevice;                                                      //¦¹°ð³s±µªº³]³Æ©Ò³s±µªº¤l³]³Æ


};
//---------------------------------------------------------------------------
#endif
