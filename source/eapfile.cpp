//TInputOutput and TFile implementation
//Last modified: 29.06.2000
//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "eapfile.h"
//---------------------------------------------------------------------------
namespace EAP{
char *ExcNotOpened="InputOutput not opened";
char *ExcReadError="InputOutput read error";
char *ExcWriteError="InputOutput write error";

TInputOutput&TInputOutput::operator<<(int v){
 if(!Opened()) throw ExcNotOpened;
 if(!WriteBlock(&v,sizeof(v))) throw ExcWriteError;
 return *this;
}
TInputOutput&TInputOutput::operator<<(short v){
 if(!Opened()) throw ExcNotOpened;
 if(!WriteBlock(&v,sizeof(v))) throw ExcWriteError;
 return *this;
}
TInputOutput&TInputOutput::operator<<(AnsiString v){
 if(!Opened()) throw ExcNotOpened;
 short len=short(v.Length());
 if(!WriteBlock(&len,sizeof(len))) throw ExcWriteError;
 if(len)
  if(!WriteBlock(v.c_str(),len)) throw ExcWriteError;
 return *this;
}
TInputOutput&TInputOutput::operator<<(char*v){
 if(!Opened()) throw ExcNotOpened;
 short len=short(strlen(v));
 if(!WriteBlock(&len,sizeof(len))) throw ExcWriteError;
 if(len)
  if(!WriteBlock(v,len)) throw ExcWriteError;
 return *this;
}
TInputOutput&TInputOutput::operator<<(bool v){
 if(!Opened()) throw ExcNotOpened;
 if(!WriteBlock(&v,sizeof(v))) throw ExcWriteError;
 return *this;
}
TInputOutput&TInputOutput::operator<<(unsigned char v){
 if(!Opened()) throw ExcNotOpened;
 if(!WriteBlock(&v,sizeof(v))) throw ExcWriteError;
 return *this;
}
TInputOutput&TInputOutput::operator<<(float v){
 if(!Opened()) throw ExcNotOpened;
 if(!WriteBlock(&v,sizeof(v))) throw ExcWriteError;
 return *this;
}
TInputOutput&TInputOutput::operator<<(double v){
 if(!Opened()) throw ExcNotOpened;
 if(!WriteBlock(&v,sizeof(v))) throw ExcWriteError;
 return *this;
}
TInputOutput&TInputOutput::operator<<(WORD v){
 if(!Opened()) throw ExcNotOpened;
 if(!WriteBlock(&v,sizeof(v))) throw ExcWriteError;
 return *this;
}
TInputOutput&TInputOutput::operator>>(int&v){
 if(!Opened()) throw ExcNotOpened;
 int t;
 if(!ReadBlock(&t,sizeof(t))) throw ExcReadError;
 v=t;
 return *this;
}
TInputOutput&TInputOutput::operator>>(short&v){
 if(!Opened()) throw ExcNotOpened;
 short t;
 if(!ReadBlock(&t,sizeof(t))) throw ExcReadError;
 v=t;
 return *this;
}
TInputOutput&TInputOutput::operator>>(AnsiString&v){
 if(!Opened()) throw ExcNotOpened;
 short len;
 char buf[5000];
 buf[0]=0;
 if(!ReadBlock(&len,sizeof(len))) throw ExcReadError;
 if(len>=sizeof(buf))return *this;
 if(len)
  if(!ReadBlock(buf,len)) throw ExcReadError;
 buf[len]=0;
 v=buf;
 return *this;
}
TInputOutput&TInputOutput::operator>>(bool&v){
 if(!Opened()) throw ExcNotOpened;
 bool t;
 if(!ReadBlock(&t,sizeof(t))) throw ExcReadError;
 v=t;
 return *this;
}
TInputOutput&TInputOutput::operator>>(unsigned char&v){
 if(!Opened()) throw ExcNotOpened;
 unsigned char t;
 if(!ReadBlock(&t,sizeof(t))) throw ExcReadError;
 v=t;
 return *this;
}
TInputOutput&TInputOutput::operator>>(float&v){
 if(!Opened()) throw ExcNotOpened;
 float t;
 if(!ReadBlock(&t,sizeof(t))) throw ExcReadError;
 v=t;
 return *this;
}
TInputOutput&TInputOutput::operator>>(double&v){
 if(!Opened()) throw ExcNotOpened;
 double t;
 if(!ReadBlock(&t,sizeof(t))) throw ExcReadError;
 v=t;
 return *this;
}
TInputOutput&TInputOutput::operator>>(WORD&v){
 if(!Opened()) throw ExcNotOpened;
 WORD t;
 if(!ReadBlock(&t,sizeof(t))) throw ExcReadError;
 v=t;
 return *this;
}
TFile::TFile():f(0)
{
}


TFile::~TFile()
{
 Close();
}
void TFile::Open(AnsiString name,bool rewrite)
{
 Close();
 char*attr=rewrite?"w+b":"r+b";
 f=fopen(name.c_str(),attr);
}
void TFile::Close()
{
 if(f){fclose(f);f=0;}
}
bool TFile::ReadBlock(void * buf, int size){
 return fread(buf,size,1,f)==1;

}
bool TFile::WriteBlock(void * buf, int size){
 return fwrite(buf,size,1,f)==1;
}

};

