//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <inifiles.hpp>

#include "EventsFrm.h"
#include "util.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEventsForm *EventsForm;
static AnsiString IniSect="EventsWin";
extern TPopupMenu *TreeViewPopupMenu;
//---------------------------------------------------------------------------
__fastcall TEventsForm::TEventsForm(TComponent* Owner)
    : TForm(Owner),begin(false)
    ,Disabled(false),nobegincount(0)
{
 Left=Screen->Width-Width;
 TIniFile *ini=new TIniFile(IniName);
 Left=atoi(ini->ReadString(IniSect,"Left",Left).c_str());
 Top=atoi(ini->ReadString(IniSect,"Top",Top).c_str());
 Width=atoi(ini->ReadString(IniSect,"Width",Width).c_str());
 Height=atoi(ini->ReadString(IniSect,"Height",Height).c_str());
 delete ini;
}
//---------------------------------------------------------------------------
void TEventsForm::AddEvent(AnsiString ev)
{
 if(Disabled) return;
 if(begin&&optClearEventsOnRunRegionOrders)
 {
   if(nobegincount)
     nobegincount--;
   else
     Memo->Clear();
 }
 if(Memo->Lines->Count>350){
  Memo->Lines->BeginUpdate();
  while(Memo->Lines->Count>300){
   Memo->Lines->Delete(0);
  }
  Memo->Lines->EndUpdate();
 }
 if(begin)
  Memo->Lines->Add(optRunRegionOrdersEventPrefix);
 Memo->Lines->Add(ev);
 begin=false;
 if(!Visible){
  if(optShowEvents){
   Show();
  }
 }else{
  BringToFront();
//  Application->ProcessMessages();
 }
}


void TEventsForm::ClearEvents()
{
 Memo->Lines->Clear();
}
void __fastcall TEventsForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 Memo->Lines->Clear();    
}
//---------------------------------------------------------------------------

void __fastcall TEventsForm::FormDestroy(TObject *Sender)
{
 TIniFile *ini=new TIniFile(IniName);
 ini->WriteString(IniSect,"Left",Left);
 ini->WriteString(IniSect,"Top",Top);
 ini->WriteString(IniSect,"Width",Width);
 ini->WriteString(IniSect,"Height",Height);
 delete ini;

}
//---------------------------------------------------------------------------

void __fastcall TEventsForm::MemoDblClick(TObject *Sender)
{
 if(!Memo->SelLength) return;
 AnsiString str=Memo->SelText.Trim();
 if(!str.Length()) return;
 if(str[1]!='(') return;
 int pos=str.Pos(")");
 if(!pos) return;
 AtlaForm->edFindUnit->Text=str.SubString(2,pos-2);
 char Key=13;
 AtlaForm->edFindUnitKeyPress(0,Key);
}
//---------------------------------------------------------------------------
void TEventsForm::Begin()
{
 begin=true;
}
//---------------------------------------------------------------------------
void __fastcall TEventsForm::FormShortCut(TWMKey &Msg, bool &Handled)
{
  if(!TreeViewPopupMenu) return;
  Handled=TreeViewPopupMenu->IsShortCut(Msg);
}
//---------------------------------------------------------------------------
void TEventsForm::AddNoClearOnBegin()
{
  nobegincount++;
}
//---------------------------------------------------------------------------

