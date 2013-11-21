//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "util.h"
#include "orders.h"
#include "EditOrderSimpleFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderSimpleForm *EditOrderSimpleForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderSimpleForm::TEditOrderSimpleForm(TComponent* Owner, AOrderSimple *_ord)
    : TForm(Owner),ord(_ord)
{
 cbCommented->Checked=ord->commented;
 cbRepeating->Checked=ord->repeating;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderSimpleForm::bnOkClick(TObject *Sender)
{
 ord->commented=cbCommented->Checked;
 ord->repeating=cbRepeating->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderSimpleForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}

