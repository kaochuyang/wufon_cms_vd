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



//initial function, parameter


//while loop, select

//parse

//close function, free memory

}
catch(...){}

}

