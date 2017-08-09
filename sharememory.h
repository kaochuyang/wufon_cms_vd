#ifndef SHAREMEMORY_H
#define SHAREMEMORY_H
#include <pthread.h>
#include "RS232.h"
#include "UDP.h"
#include "variable.h"
#include "junli_vd.h"
#include "junbo_cms.h"

class sharememory
{
    public:


        UDP centerSocket;
        sharememory();
        virtual ~sharememory();
        junli_vd junli_object;
        junbo_cms junbo_object;

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
