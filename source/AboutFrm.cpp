//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "AboutFrm.h"
#include "util.h"
#include "turn.h"
#include "region.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutForm *AboutForm;
AnsiString ACVersionStr;
//---------------------------------------------------------------------------
__fastcall TAboutForm::TAboutForm(TComponent* Owner)
  : TForm(Owner)
{
 AnsiString clname="AtlaClient";
 if(ACVersionStr.Length())
 {
   clname+=" v";
   clname+=ACVersionStr;
 }
 labName->Caption=clname;
 if(curTurn)
 {
   int count=0;
   foreach(curTurn->RegionList)
   {
     ARegion *reg=*(ARegion**)iter;
     if(reg->hasInfo)
       count++;
   }
   labRegCount->Caption=AnsiString(count)+" regions";
 }
}
//---------------------------------------------------------------------------
void __fastcall TAboutForm::labHomePageClick(TObject *Sender)
{
 AnsiString s="http://"+labHomePage->Caption;
 ShellExecute(0,"open",s.c_str(),0,0,SW_SHOW);
}
//---------------------------------------------------------------------------
void __fastcall TAboutForm::labEMailClick(TObject *Sender)
{
 AnsiString s="mailto:"+labEMail->Caption+"?subject=AtlaClient";
 ShellExecute(0,"open",s.c_str(),0,0,SW_SHOW);
}
//---------------------------------------------------------------------------
void TAboutForm::UpdateVersionString()
{
 DWORD h;
 DWORD Size=GetFileVersionInfoSize(Application->ExeName.c_str(), &h);
 if(Size){
  char *buf=new char[Size];
  if(GetFileVersionInfo(Application->ExeName.c_str(),h,Size,buf)!=0){
   char *ValueBuf;
   UINT Len;
   VerQueryValue(buf, "\\VarFileInfo\\Translation",&(void*)ValueBuf,&Len);
   if(Len>=4){
    int cs=(int)(int(*((WORD*)(ValueBuf+2)))|(int(*((WORD*)(ValueBuf)))<<16));
    AnsiString CharSet=IntToHex(cs, 8);
/*    if(VerQueryValue(buf,AnsiString("\\StringFileInfo\\"+CharSet+"\\ProductName").c_str(),&(void*)ValueBuf,&Len)!=0)
     ProductName->Caption=ValueBuf;*/
    if(VerQueryValue(buf,AnsiString("\\StringFileInfo\\"+CharSet+"\\FileVersion").c_str(),&(void*)ValueBuf,&Len)!=0){
     ACVersionStr=ValueBuf;
    }
/*    if(VerQueryValue(buf,AnsiString("\\StringFileInfo\\"+CharSet+"\\LegalCopyright").c_str(),&(void*)ValueBuf,&Len)!=0)
     Copyright->Caption=ValueBuf;
    if(VerQueryValue(buf,AnsiString("\\StringFileInfo\\"+CharSet+"\\CompanyName").c_str(),&(void*)ValueBuf,&Len)!=0)
     Company->Caption=ValueBuf;*/
   }
  }
  delete buf;
 }
}
//---------------------------------------------------------------------------

