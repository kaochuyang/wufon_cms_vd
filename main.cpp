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
 int maxport=0;                                                             //maxnum file desciptor used
 struct timeval timeout;                                                     //假使若干秒select都沒有資料進來,就做timeout要作的事情
 int readSelectLength=0;                                                     //判斷讀到的長度



//open a lot of device port

maxport=smem.junli_object.open_port_process("/dev/ttyS0");
maxport=smem.junbo_object.open_port_process("/dev/ttyS1");
maxport++;

while(1)
{//initial function, parameter
FD_ZERO(&readfs);

  if(smem.junli_object.junli_port.GetPortAlreadyOpen())FD_SET(smem.junli_object.junli_port.Getfd(),&readfs);
  if(smem.junbo_object.junbo_cms_port.GetPortAlreadyOpen())FD_SET(smem.junbo_object.junbo_cms_port.Getfd(),&readfs);

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

                if (smem.centerSocket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.centerSocket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.centerSocket.UdpRead();
                        if (readSelectLength>0)
                        {


                        }
                    }
                }




                if (smem.junli_object.junli_port.GetPortAlreadyOpen())
                {
                    if (smem.junli_object.junli_port.Getfd(),&readfs)
                    {
                        readSelectLength=smem.junli_object.junli_port.Rs232Read();
                        if (readSelectLength>0)
                        {
                        smem.junli_object.parse_junli(readSelectLength,smem.junli_object.junli_port.block,smem.junli_object.junli_port.messageIn,&smem.junli_object.junli_port.lastPacketIndex);
                        }
                    }
                }

            }

        if (smem.junli_object.junli_port.CloseRs232Port()) printf("Close junli_port Successful!!\n");

        if (smem.junbo_object.junbo_cms_port.CloseRs232Port()) printf("Close junbo_cms_port Successful!!\n");

//close function, free memory
}
}
catch(...){}

}

