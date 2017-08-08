#include <stdio.h>

#include "sharememory.h"
#include "PARSEGROUP.h"
#include "readjob.h"
#include "writejob.h"
int main(int argc, char* argv[])
{

try
{


 int rec=-1;                                                                 //Select Result
 fd_set readfs;                                                              //File Descriptor Set
 int maxport=0,tempmax=0;                                                    //maxnum file desciptor used
 struct timeval timeout;                                                     //假使若干秒select都沒有資料進來,就做timeout要作的事情
 int readSelectLength=0;                                                     //判斷讀到的長度



//open a lot of device port


maxport++;

while(1)
{//initial function, parameter
FD_ZERO(&readfs);



//while loop, select

            timeout.tv_sec=60;                                                      //timeout秒數
            timeout.tv_usec=0;                                                      //這個是毫秒,暫不使用

            rec=select(maxport,&readfs,NULL,NULL,&timeout);                         //wait block for read

            if (rec<0)                                                              //Select Error
            {

            }
            else if (rec==0)                                                        //Select Time Out
            {
               printf("Select Time Out!!\n");
            }
            else//parse
            {
               /* if (smem.DeviceSocket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.DeviceSocket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.DeviceSocket.UdpRead();
                        if (readSelectLength>0)
                        {

                        }
                    }
                }*/

            }



//close function, free memory
}
}
catch(...){}

}

