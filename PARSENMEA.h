//---------------------------------------------------------------------------

#ifndef PARSENMEAH
#define PARSENMEAH

#include "var.h"
#include "PARSE.h"
//---------------------------------------------------------------------------
class PARSENMEA:public PARSE
{
    public:

        PARSENMEA(void);
        ~PARSENMEA(void);

        bool ParseNMEABlock(int,BYTE *,MESSAGEOK *,int *,int *);                //�ѪRblock
        bool vFindGPRMC(int *,MESSAGEOK *);
        bool vAdjTimeByGPS(int *,MESSAGEOK *);
        bool EchoToGUI(int *,MESSAGEOK *,char *);                               //��ܦܵe��
        bool MoveLastData(int *,int *,MESSAGEOK *);                             //�h���Ѿl�����

    private:

};
//---------------------------------------------------------------------------
#endif




