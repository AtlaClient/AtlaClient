//---------------------------------------------------------------------------

#pragma hdrstop
#include <vcl.h>

#include "ShowWeatherRegionsFrm.h"
#include "util.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TShowWeatherRegionsForm *ShowWeatherRegionsForm;
//---------------------------------------------------------------------------
__fastcall TShowWeatherRegionsForm::TShowWeatherRegionsForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TShowWeatherRegionsForm::FormShow(TObject *Sender)
{
 if(!WeatherRegions){
  ModalResult=mrCancel;
  return;
 }
 if(WeatherRegions->count!=3){
  ModalResult=mrCancel;
  return;
 }
 AWeatherRegion *wr=WeatherRegions->Get(2);
 edX->Text=wr->rect.right+1;
 edY->Text=wr->rect.bottom+1;
}
//---------------------------------------------------------------------------
void __fastcall TShowWeatherRegionsForm::bnOkClick(TObject *Sender)
{
 int x=atoi(edX->Text.c_str());
 int y=atoi(edY->Text.c_str());
 if(x<8||y<8) return;
 AWeatherRegion *wr;
 TRect r;
 x--;
 r.left=0;
 r.right=x;
 wr=WeatherRegions->Get(0);
 r.top=0;
 r.bottom=y/3-1;
 wr->rect=r;
 wr=WeatherRegions->Get(1);
 r.top=r.bottom+1;
 r.bottom=2*y/3-1;
 wr->rect=r;
 wr=WeatherRegions->Get(2);
 r.top=r.bottom+1;
 r.bottom=y-1;
 wr->rect=r;
 ModalResult=mrOk;
}
//---------------------------------------------------------------------------
