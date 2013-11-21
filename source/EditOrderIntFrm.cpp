//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdlib.h>
#include "util.h"
#include "orders.h"
#include "EditOrderIntFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderIntForm *EditOrderIntForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderIntForm::TEditOrderIntForm(TComponent* Owner, AOrderInt *_ord)
    : TForm(Owner),ord(_ord)
{
 edValue->Text=ord->value;
 cbCommented->Checked=ord->commented;
 cbRepeating->Checked=ord->repeating;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderIntForm::bnOkClick(TObject *Sender)
{
 ord->value=atoi(edValue->Text.c_str());
 ord->commented=cbCommented->Checked;
 ord->repeating=cbRepeating->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderIntForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}

