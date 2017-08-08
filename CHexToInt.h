#ifndef CHEXTOINT_H
#define CHEXTOINT_H


class CHexToInt
{
    public:
        CHexToInt();
        virtual ~CHexToInt();
        long hchartoi (char, int);             /* converts a hex char to decimal knowing its 0 based place value */
        long htoi (char *, int);               /* converts a string of hex bits to integer ... */
        char vIntToFuckingBCD(int, int);                  //convert Int To special BCD code (max only to million)
    protected:
    private:
};
extern CHexToInt oHexToInt;
#endif // CHEXTOINT_H
