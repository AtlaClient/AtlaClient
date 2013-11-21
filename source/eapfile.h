//TInputOutput and TFile declaration
//Last modified: 29.06.2000
//---------------------------------------------------------------------------

#ifndef _file_H
#define _file_H

#include <stdio.h>
namespace EAP{
class TInputOutput{
public:
    TInputOutput(){};
    virtual ~TInputOutput(){};
    virtual bool Opened()=0;
    virtual bool ReadBlock(void * buf, int size)=0;
    virtual bool WriteBlock(void * buf, int size)=0;

    TInputOutput&operator<<(int v);
    TInputOutput&operator<<(short v);
    TInputOutput&operator<<(AnsiString v);
    TInputOutput&operator<<(char* v);
    TInputOutput&operator<<(bool v);
    TInputOutput&operator<<(unsigned char v);
    TInputOutput&operator<<(float v);
    TInputOutput&operator<<(double v);
    TInputOutput&operator<<(WORD v);

    TInputOutput&operator>>(int&v);
    TInputOutput&operator>>(short&v);
    TInputOutput&operator>>(AnsiString &v);
    TInputOutput&operator>>(bool&v);
    TInputOutput&operator>>(unsigned char&v);
    TInputOutput&operator>>(float&v);
    TInputOutput&operator>>(double&v);
    TInputOutput&operator>>(WORD&v);
};
class TFile:public TInputOutput{
protected:
    FILE *f;
public:
    TFile();
    virtual ~TFile();

    virtual bool Opened(){return f!=0;};
    virtual bool ReadBlock(void * buf, int size);
    virtual bool WriteBlock(void * buf, int size);
    void Open(AnsiString name,bool rewrite=false);
    void Close();

};
};
using namespace EAP;
//---------------------------------------------------------------------------
#endif
