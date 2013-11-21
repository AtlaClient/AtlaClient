//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "util.h"
#include "orders.h"
#include "EditOrderNameDescribeFrm.h"
#include "unit.h"
#include "faction.h"
#include "region.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TEditOrderNameDescribeForm::TEditOrderNameDescribeForm(TComponent* Owner, AOrderNameDescribe *_ord)
    : TForm(Owner),ord(_ord)
{
 cbCommented->Checked=ord->commented;
 RadioGroup->ItemIndex=_ord->subtype+(_ord->type==O_NAME?0:4);
 edValue->Text=_ord->value;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderNameDescribeForm::bnOkClick(TObject *Sender)
{
 ord->commented=cbCommented->Checked;
 int st=RadioGroup->ItemIndex;
 if(st<4)
 {
   ord->type=O_NAME;
   ord->subtype=st;
 }
 else
 {
   ord->type=O_DESCRIBE;
   ord->subtype=st-4;
 }
 ord->value=edValue->Text;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderNameDescribeForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderNameDescribeForm::bnGetClick(TObject *Sender)
{
  AnsiString str;
  switch(RadioGroup->ItemIndex)
  {
    case 0:
      str=ord->orders->unit->name;
    break;
    case 1:
      str=ord->orders->unit->faction->name;
    break;
    case 2:
      str=ord->orders->unit->baseobj->name;
    break;
    case 3:
      str=ord->orders->unit->baseobj->basereg->townname;
    break;
    case 4:
      str=ord->orders->unit->describe;
    break;
    case 5:
    case 6:
    case 7:
      str=ord->orders->unit->baseobj->describe;
    break;
  }
  if(!str.Length()) return;
  if(RadioGroup->ItemIndex==4&&str.Length()>1)
    str.Delete(1,2);
  edValue->Text=str;
}
//---------------------------------------------------------------------------

