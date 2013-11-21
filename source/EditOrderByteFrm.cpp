//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "util.h"
#include "orders.h"
#include "EditOrderByteFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderByteForm *EditOrderByteForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderByteForm::TEditOrderByteForm(TComponent* Owner, AOrderByte *_ord, TStringList *list)
    : TForm(Owner),ord(_ord)
{

 cbCommented->Checked=ord->commented;
 cbRepeating->Checked=ord->repeating;
 Height=130+(list->Count-2)*24;
 for(int i=0,y=8;i<list->Count;i++,y+=24){
  TRadioButton *but=new TRadioButton(this);
  but->Parent=this;
  but->Left=8;
  but->Top=y;
  but->Width=154;
  but->Height=17;
  but->Tag=i;
  but->TabOrder=short(i); 
  but->Caption=list->Strings[i];
  if(i==ord->value) but->Checked=true;
 }
// rbOn->Checked=ord->value;
// rbOff->Checked=!ord->value;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderByteForm::bnOkClick(TObject *Sender)
{
 ord->commented=cbCommented->Checked;
 ord->repeating=cbRepeating->Checked;
 for(int i=0;i<ControlCount;i++){
  TRadioButton *but=dynamic_cast<TRadioButton*>(Controls[i]);
  if(!but) continue;
  if(!but->Checked) continue;
  ord->value=(unsigned char)but->Tag;
  return;
 }
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderByteForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}


