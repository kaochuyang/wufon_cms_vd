#ifndef SHAREMEMORY_H
#define SHAREMEMORY_H
#include <pthread.h>
#include "RS232.h"
#include "UDP.h"
#include "variable.h"
class sharememory
{
    public:
        sharememory();
        virtual ~sharememory();
        int GetAddress(void);
        bool SetAddress(int Lcn);
        int GetSequence(void);
        int vGetAdjudicateReturnAddressBCDorHEX(void);
    protected:
    private:
    int address;
    int sequence;
    int iAdjudicateReturnAddressBCDorHEX;

     pthread_mutex_t mutexSmem;
};
extern sharememory smem;
#endif // SHAREMEMORY_H
