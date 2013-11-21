//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "util.h"
#include "orders.h"
#include "EditOrderCommentFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderCommentForm *EditOrderCommentForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderCommentForm::TEditOrderCommentForm(TComponent* Owner, AOrderComment *_ord)
    : TForm(Owner),ord(_ord)
{
 edText->Text=ord->text;
 cbRepeating->Checked=ord->repeating;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderCommentForm::bnOkClick(TObject *Sender)
{
 ord->text=edText->Text;
 ord->repeating=cbRepeating->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderCommentForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderCommentForm::bnConvertLocalClick(TObject *Sender)
{
 bnOkClick(this);
 AOrderLocDescr *neword=new AOrderLocDescr;
 neword->text=ord->text;

 ord=(AOrderComment*)neword;
 ModalResult=mbOK;
}
//---------------------------------------------------------------------------

void __fastcall TEditOrderCommentForm::bnConvertCustomClick(
      TObject *Sender)
{
 bnOkClick(this);
 AOrderCustom *neword=new AOrderCustom;
 neword->text=ord->text;
 neword->repeating=ord->repeating;
 neword->commented=true;

 ord=(AOrderComment*)neword;
 ModalResult=mbOK;
}
//---------------------------------------------------------------------------


