//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "util.h"
#include "orders.h"
#include "EditOrderCastFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TEditOrderCastForm *EditOrderCastForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderCastForm::TEditOrderCastForm(TComponent* Owner, AOrderCast *_ord,TStringList *names, TStringList *abrs)
    : TForm(Owner),ord(_ord),namelist(names),abrlist(abrs)
{
 cbCommented->Checked=ord->commented;
 cbRepeating->Checked=ord->repeating;
 cbList->Items->Assign(names);
 cbList->ItemIndex=abrlist->IndexOf(ord->abr);
 edParams->Text=_ord->params; 
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderCastForm::bnOkClick(TObject *Sender)
{
 int ind=cbList->ItemIndex;
 ord->commented=cbCommented->Checked;
 ord->repeating=cbRepeating->Checked;
 ord->params=edParams->Text;
 if(ind!=-1){
  ord->abr=abrlist->Strings[ind];
 }
 if(!ord->abr.Length())
  ord=0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderCastForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}

