//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "ReqFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TReqForm *ReqForm;
//---------------------------------------------------------------------------
__fastcall TReqForm::TReqForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
int __fastcall TReqForm::ShowFrm(AnsiString l, AnsiString s1, AnsiString s2){
  Label1->Caption = l;
  bt1->Caption = s1;
  bt2->Caption = s2;
  return ShowModal();
}
