//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "util.h"
#include "orders.h"
#include "EditOrderCustomFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderCustomForm *EditOrderCustomForm;
extern AnsiString keyCust;
//---------------------------------------------------------------------------
__fastcall TEditOrderCustomForm::TEditOrderCustomForm(TComponent* Owner, AOrderCustom *_ord)
    : TForm(Owner),ord(_ord)
{
 edText->Text=ord->text;
 cbCommented->Checked=ord->commented;
 cbRepeating->Checked=ord->repeating;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderCustomForm::bnOkClick(TObject *Sender)
{
 ord->text=edText->Text;
 ord->commented=cbCommented->Checked;
 ord->repeating=cbRepeating->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderCustomForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderCustomForm::bnConvertCommentClick(
      TObject *Sender)
{
 bnOkClick(this);
 AOrderComment *neword=new AOrderComment;
 neword->text=ord->text;
 neword->repeating=ord->repeating;

 ord=(AOrderCustom*)neword;
 ModalResult=mbOK;
}

void __fastcall TEditOrderCustomForm::bnConvertOrderClick(TObject *Sender)
{
 bnOkClick(this);
 AnsiString str=ord->WriteTemplate();
 int pos=str.Pos(keyCust);
 if(pos>0)
 {
   str.SetLength(pos-1);
   if(!str.Length()) return;
 }
 AOrder *neword=ParseOrder(str);
 if(!neword) return;
 if(neword->type==O_CUSTOMORDER){ //dynamic_cast<AOrderCustom*>(neword)){
  delete neword;
  return;
 }

 ord=(AOrderCustom*)neword;
 ModalResult=mbOK;
}
//---------------------------------------------------------------------------


