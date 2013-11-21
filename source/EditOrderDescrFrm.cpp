//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "util.h"
#include "orders.h"
#include "EditOrderDescrFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderDescrForm *EditOrderDescrForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderDescrForm::TEditOrderDescrForm(TComponent* Owner, AOrderLocDescr *_ord)
    : TForm(Owner),ord(_ord)
{
 edText->Text=ord->text;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderDescrForm::bnOkClick(TObject *Sender)
{
 int pos=edText->Text.Pos("(");
 if(pos)
 {
	 ShowMessage("( or ) not allowed");
	 ModalResult=mrNone;
 }
 ord->text=edText->Text;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderDescrForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderDescrForm::bnConvertCommentClick(TObject *Sender)
{
 bnOkClick(this);
 AOrderComment *neword=new AOrderComment;
 neword->text=ord->text;
 neword->repeating=true;

 ord=(AOrderLocDescr*)neword;
 ModalResult=mbOK;
}
//---------------------------------------------------------------------------


