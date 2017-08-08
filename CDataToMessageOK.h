#ifndef CDATATOMESSAGEOK_H
#define CDATATOMESSAGEOK_H

#include "variable.h"
#include "sharememory.h"
class CDataToMessageOK
{
    public:
        CDataToMessageOK();
        virtual ~CDataToMessageOK();
         MESSAGEOK vPackageINFOTo92Protocol(unsigned char *cInfoData, int iInfoLength,bool getSeq);
    protected:
    private:
};

#endif // CDATATOMESSAGEOK_H
