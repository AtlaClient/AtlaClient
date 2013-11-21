//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "EditOrdersFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TEditOrdersForm::TEditOrdersForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TEditOrdersForm::FormShow(TObject *Sender)
{
 int len=Editor->Lines->Text.Length();
 Editor->SelStart=len;
 Editor->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TEditOrdersForm::OkActionExecute(TObject *Sender)
{
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TEditOrdersForm::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key == VK_ESCAPE) bnCancel->Click();
}
//---------------------------------------------------------------------------

