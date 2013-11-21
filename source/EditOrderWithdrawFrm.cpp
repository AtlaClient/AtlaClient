//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdlib.h>
#include "util.h"
#include "orders.h"
#include "EditOrderWithdrawFrm.h"
#include "unit.h"
#include "faction.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderWithdrawForm *EditOrderWithdrawForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderWithdrawForm::TEditOrderWithdrawForm(TComponent* Owner, AOrderWithdraw *_ord)
    : TForm(Owner),ord(_ord)
{
 if(_ord->clientonly)
 {
   cbCommented->Visible=false;
   cbRepeating->Visible=false;
   labUnclaimed->Visible=false;
   labAttempted->Visible=false;
 }else
 {
   cbCommented->Checked=ord->commented;
   cbRepeating->Checked=ord->repeating;
   int uncl=ord->orders->unit->faction->unclaimed;
   labUnclaimed->Caption=KeyPhrases->Get(keyUnclaimed)+uncl;
 }
 cbItems->Items->BeginUpdate();
 cbItems->Items->Clear();
 foreach(ItemTypes){
  AItemType *itype=*(AItemType**)iter;
  if(itype->flags&AItemType::CANTGIVE) continue;
  if(!_ord->clientonly&&itype->withdrawcost==0) continue;
  if(itype->names.Length())
   cbItems->Items->Add(itype->names);
  else
   cbItems->Items->Add(itype->name);
 }
 cbItems->Items->EndUpdate();
 int ind=cbItems->Items->IndexOf(ItemTypes->AbrToNames(_ord->item));
 cbItems->ItemIndex=ind;
 edNumber->Text=_ord->count;
 edNumberChange(0);
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderWithdrawForm::bnOkClick(TObject *Sender)
{
 int ind=cbItems->ItemIndex;
 if(ind!=-1){
  ord->item=ItemTypes->NameToAbr(cbItems->Items->Strings[ind]);
 }
 int num=atoi(edNumber->Text.c_str());
 ord->count=num;
 if(!ord->clientonly)
 {
   ord->commented=cbCommented->Checked;
   ord->repeating=cbRepeating->Checked;
 }
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderWithdrawForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderWithdrawForm::edNumberChange(TObject *Sender)
{
 int ind=cbItems->ItemIndex;
 int num=0;
 if(ind!=-1){
   AItemType *itype=ItemTypes->Get(cbItems->Items->Strings[ind]);
   if(itype){
     num=atoi(edNumber->Text.c_str());
     num*=itype->withdrawcost;
   }
 }
 labAttempted->Caption=AnsiString().sprintf("Cost: %d",num);
}
//---------------------------------------------------------------------------

