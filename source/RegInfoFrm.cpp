//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <inifiles.hpp>
#include <stdlib.h>
#include "RegInfoFrm.h"
#include "util.h"
#include "region.h"
#include "markets.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRegInfoForm *RegInfoForm;
static AnsiString IniSect="RegInfoWin";

//---------------------------------------------------------------------------
__fastcall TRegInfoForm::TRegInfoForm(TComponent* Owner)
        : TForm(Owner),oxloc(-1),moving(false)
{
 TIniFile *ini=new TIniFile(IniName);
 Left=atoi(ini->ReadString(IniSect,"Left",Left).c_str());
 Top=atoi(ini->ReadString(IniSect,"Top",Top).c_str());
 Width=atoi(ini->ReadString(IniSect,"Width",Width).c_str());
 Height=atoi(ini->ReadString(IniSect,"Height",Height).c_str());
 delete ini;
}
//---------------------------------------------------------------------------
void TRegInfoForm::UpdateRegionInfo(int xloc,int yloc, int zloc)
{
 if(!ShowRegInfo) return;
 if(oxloc==xloc&&oyloc==yloc&&ozloc==zloc) return;
 oxloc=xloc;oyloc=yloc;ozloc=zloc;
 ARegion *reg=curRegionList->Get(xloc,yloc,zloc);
 if(!reg){
  Memo->Lines->Clear();
  Memo->Height=10;Memo->Width=10;
  return;
 }
 Memo->Lines->BeginUpdate();
 Memo->Lines->Clear();
 int h=0,w=0;
 AnsiString s;


 s=reg->FullName(false);
 if(!reg->hasInfo||reg->oldinfoturnnum!=curTurnNumber){
  s="?"+s;
 }
 AddLine(s,w,h);

 if(reg->hasInfo){
  s="";
  s+=reg->population;
  s+=" peasants";
  if(!optConquest&&reg->population)
    s+=" ("+reg->race+")";
  AddLine(s,w,h);
  s="Tax: $"; s+=reg->money;
  AddLine(s,w,h);
  s="Wages: $"; s+=reg->wages;
  if(reg->wages){
   s+=" (Max: $"; s+=reg->maxwages; s+=")";
  }
  AddLine(s,w,h);
  s="Entertainment: $"; s+=reg->entertainment;
  AddLine(s,w,h);
 }else
 {
  s="unknown peasants";
  AddLine(s,w,h);
  s="Tax: ?";
  AddLine(s,w,h);
  s="Wages: ?";
  AddLine(s,w,h);
  s="Entertainment: ?";
  AddLine(s,w,h);
 }
 if(!optConquest)
 {
  s="";
  AnsiString weat=WeatherRegions->GetWeather(xloc,yloc,zloc,curTurnNumber);
  AnsiString nxtweat=WeatherRegions->GetWeather(xloc,yloc,zloc,curTurnNumber+1);
  if(reg->hasInfo&&reg->oldinfoturnnum==curTurnNumber){
   if(weat!=reg->weather) s+="!";
   s+=WeatherTypes->Get(reg->weather)->curmonthname;
   AddLine(s,w,h);
   s=" ";
   if(nxtweat!=reg->nxtweather) s+="!";
   s+=WeatherTypes->Get(reg->nxtweather)->nextmonthname;
  }else{
   s+=WeatherTypes->Get(weat)->curmonthname;
   AddLine(s,w,h);
   s=" "+WeatherTypes->Get(nxtweat)->nextmonthname;
  }
  int sl=WeatherRegions->GetSeasonLength(xloc,yloc,zloc);
  if(sl!=12)
  {
    s+=" (";
    s+=sl;
    s+=" months)";
  }
  AddLine(s,w,h);
 }
 if(reg->hasInfo){
  if(reg->gate){
   s=KeyPhrases->Get(keyGatesSign);
   s+=reg->gate;
   s+=KeyPhrases->Get(keyGatesSign2);
   s+=curRegionList->numberofgates;
   s+=").";
  }else
   s=" ";
  AddLine(s,w,h);
  s="Products:";
  AddLine(s,w,h);
  foreach(reg->products){
   AMarket *mark=*(AMarket**) iter;
   s=" "+mark->Print(3);
   AddLine(s,w,h);
  }
 }

 Memo->Lines->EndUpdate();
 Memo->Width=w+5;
 Memo->Height=h+5;
 if(!Visible) Show();
}


void __fastcall TRegInfoForm::SetShowRegInfo(bool value)
{
 if(FShowRegInfo == value) return;
 FShowRegInfo = value;
 if(value){
  if(curRegion){
   UpdateRegionInfo(curRegion->xloc,curRegion->yloc,curRegion->zloc);
  }
 }else{
  oxloc=-1;
  Hide();
 }
}
void __fastcall TRegInfoForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 AtlaForm->actShowRegInfo->Checked=false;
 ShowRegInfo=false;
 Action=caHide;
}
//---------------------------------------------------------------------------
void TRegInfoForm::AddLine(const AnsiString & str, int & w, int & h)
{
 TSize sz=Canvas->TextExtent(str);
 h+=sz.cy;
 if(w<sz.cx) w=sz.cx;
 Memo->Lines->Add(str);
}
void __fastcall TRegInfoForm::FormDestroy(TObject *Sender)
{
 TIniFile *ini=new TIniFile(IniName);
 ini->WriteString(IniSect,"Left",Left);
 ini->WriteString(IniSect,"Top",Top);
 ini->WriteString(IniSect,"Width",Width);
 ini->WriteString(IniSect,"Height",Height);
 delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TRegInfoForm::CreateParams(Controls::TCreateParams &Params)
{
  TCustomForm::CreateParams(Params);
  Params.Style|=WS_THICKFRAME;
}
//---------------------------------------------------------------------------
void __fastcall TRegInfoForm::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if(Button==mbRight)
 {
   moving=true;
   movestart=ClientToScreen(TPoint(X,Y));
   Mouse->Capture=Handle;
   return;
 }
}
//---------------------------------------------------------------------------
void __fastcall TRegInfoForm::FormMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(Button!=mbRight||!moving) return;
  FormMouseMove(Sender,Shift,X,Y);
  moving=false;
  Mouse->Capture=0;
}
//---------------------------------------------------------------------------
void __fastcall TRegInfoForm::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
 if(moving)
 {
   TPoint p=ClientToScreen(TPoint(X,Y));
   Left=Left+p.x-movestart.x;
   Top=Top+p.y-movestart.y;
   movestart=p;
   return;
 }
}
//---------------------------------------------------------------------------

