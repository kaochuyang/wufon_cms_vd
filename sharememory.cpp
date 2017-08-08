#include "sharememory.h"

sharememory::sharememory()
{
    //ctor
}

sharememory::~sharememory()
{
    //dtor
}

sharememory smem;

int sharememory::GetAddress()
{
try {
    int tempAddress=0;
    pthread_mutex_lock(&mutexSmem);
    tempAddress=address;
    pthread_mutex_unlock(&mutexSmem);
    return tempAddress;
  } catch (...) {}
}

int sharememory::GetSequence(void)
{
try {
    int tempSequence;
    pthread_mutex_lock(&mutexSmem);
    tempSequence=sequence;
    sequence++;                                          //
    if (sequence>=256) sequence=0;                       //
    pthread_mutex_unlock(&mutexSmem);
    return tempSequence;
  } catch (...) {}
}

int sharememory::vGetAdjudicateReturnAddressBCDorHEX(void)
{
try {
  int iTMP=0;
  pthread_mutex_lock(&mutexSmem);
  iTMP=iAdjudicateReturnAddressBCDorHEX;
  pthread_mutex_unlock(&mutexSmem);
  return iTMP;
} catch (...) {}
}
bool sharememory::SetAddress(int lcn)
{
try {
    pthread_mutex_lock(&mutexSmem);
    //store......
    address=lcn;                                                           //From Control panel
    pthread_mutex_unlock(&mutexSmem);
    return true;
  } catch (...) {}
}
