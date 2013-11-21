//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdlib.h>
#include "util.h"
#include "orders.h"
#include "EditOrderAutoGiveFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderAutoGiveForm *EditOrderAutoGiveForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderAutoGiveForm::TEditOrderAutoGiveForm(TComponent* Owner, AOrderAutoGive *_ord)
    : TForm(Owner),ord(_ord)
{
 cbItems->Items->BeginUpdate();
 cbItems->Items->Clear();
 foreach(ItemTypes){
  AItemType *itype=*(AItemType**)iter;
  if(itype->flags&AItemType::CANTGIVE) continue;
  if(itype->names.Length())
   cbItems->Items->Add(itype->names);
  else
   cbItems->Items->Add(itype->name);
 }
 cbItems->Items->EndUpdate();
 int ind=cbItems->Items->IndexOf(ItemTypes->AbrToNames(_ord->item));
 cbItems->ItemIndex=ind;

 int num=_ord->number;
 if(num==0){
  rbAll->Checked=true;
 }else if(num<0){
  rbExcept->Checked=true;
  edNumber->Text=-num;
 }else{
  rbNumber->Checked=true;
  edNumber->Text=num;
 }
 edBegStr->Text=_ord->begstr;
 edSubStr->Text=_ord->substr;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoGiveForm::bnOkClick(TObject *Sender)
{
 int ind=cbItems->ItemIndex;
 if(ind!=-1){
  ord->item=ItemTypes->NameToAbr(cbItems->Items->Strings[ind]);
 }
 int num=atoi(edNumber->Text.c_str());
 if(rbAll->Checked) num=0;
 if(rbExcept->Checked) num=-num;
 ord->number=num;
 ord->begstr=edBegStr->Text;
 ord->substr=edSubStr->Text;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoGiveForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}





void __fastcall TEditOrderAutoGiveForm::rbExceptClick(TObject *Sender)
{
 edNumber->Enabled=true;
 if(Visible)edNumber->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TEditOrderAutoGiveForm::rbAllClick(TObject *Sender)
{
 edNumber->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TEditOrderAutoGiveForm::rbNumberClick(TObject *Sender)
{
 edNumber->Enabled=true;
 if(Visible)edNumber->SetFocus();
}
//---------------------------------------------------------------------------


