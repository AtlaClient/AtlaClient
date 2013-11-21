//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdlib.h>
#include "util.h"
#include "orders.h"
#include "markets.h"
#include "unit.h"
#include "EditOrderBuySellFrm.h"
#include "items.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderBuySellForm *EditOrderBuySellForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderBuySellForm::TEditOrderBuySellForm(TComponent* Owner, AOrderBuySell *_ord, AMarkets *marks)
    : TForm(Owner),ord(_ord),markets(marks)
{
 cbCommented->Checked=ord->commented;
 cbRepeating->Checked=ord->repeating;
 if(ord->amount>=0)
   edCount->Text=ord->amount;
 else{
   cbAll->Checked=true;
   edCount->Text=0;
 }
 AnsiString s;
 cbItems->Items->BeginUpdate();
 foreach(markets){
  AMarket *mt=*(AMarket**)iter;
  s=mt->type->names+": "+mt->amount+KeyPhrases->Get(keyMarketAtSign)+mt->price;
  cbItems->Items->Add(s);
  if(mt->type->abr==ord->item){
   cbItems->ItemIndex=cbItems->Items->Count-1;
  }
 }
 cbItems->Items->EndUpdate();
 MoneyChange(0);
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderBuySellForm::bnOkClick(TObject *Sender)
{
 int inditem=cbItems->ItemIndex;
 ord->commented=cbCommented->Checked;
 ord->repeating=cbRepeating->Checked;
 ord->amount=atoi(edCount->Text.c_str());
 if(cbAll->Checked)
   ord->amount=-1;
 if(inditem==-1){
//  ord=0;
//  ModalResult=mrCancel;
  return;
 }


 AMarket *mt=markets->Get(inditem);
 ord->item=mt->type->abr;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderBuySellForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderBuySellForm::edCountExit(TObject *Sender)
{
 int i=atoi(edCount->Text.c_str());
 edCount->Text=i;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderBuySellForm::bnAllClick(TObject *Sender)
{
 int inditem=cbItems->ItemIndex;
 if(inditem<0) return;
 AMarket *mt=markets->Get(inditem);
 edCount->Text=mt->amount;
 MoneyChange(0);
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderBuySellForm::MoneyChange(TObject *Sender)
{
  int count=atoi(edCount->Text.c_str());
  int money=0;
  int have=0;
  int ind=cbItems->ItemIndex;
  if(ind>=0)
  {
    AMarket *mt=markets->Get(ind);
    if(cbAll->Checked)
      money=mt->amount*mt->price;
    else
      money=count*mt->price;
    have=ord->orders->unit->enditems->GetNum(mt->type);
  }
  labPrice->Caption=AnsiString("=$")+money+" have "+have;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderBuySellForm::cbAllClick(TObject *Sender)
{
  bool b=cbAll->Checked;
  if(!cbAll->Enabled)
    return;
  edCount->Visible=!b;
}
//---------------------------------------------------------------------------

