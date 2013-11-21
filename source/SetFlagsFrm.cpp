//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "SetFlagsFrm.h"
#include "unit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TSetFlagsForm *SetFlagsForm;
extern AnsiString keySpoils[5]; //from orders.cpp
//---------------------------------------------------------------------------
__fastcall TSetFlagsForm::TSetFlagsForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSetFlagsForm::FormShow(TObject *Sender)
{
 cbGuard->Checked=(curUnit->endguard==GUARD_GUARD);
 cbAvoid->Checked=(curUnit->endguard==GUARD_AVOID);
 if(curUnit->endreveal==REVEAL_FACTION){
  rgReveal->ItemIndex=2;
 }else if(curUnit->endreveal==REVEAL_UNIT){
  rgReveal->ItemIndex=1;
 }else{
  rgReveal->ItemIndex=0;
 }
 cbBehind->Checked=(curUnit->GetEndFlag(FLAG_BEHIND));
 cbAutoTax->Checked=(curUnit->GetEndFlag(FLAG_AUTOTAX));
 cbHold->Checked=(curUnit->GetEndFlag(FLAG_HOLDING));
 cbNoAid->Checked=(curUnit->GetEndFlag(FLAG_NOAID));
 cbNoCross->Checked=(curUnit->GetEndFlag(FLAG_NOCROSS_WATER));
 cbNoSpoils->Checked=(curUnit->GetEndFlag(FLAG_NOSPOILS));
 cbSharing->Checked = (curUnit->GetEndFlag(FLAG_SHARING));

 cbSharing->Visible = optServerSharing;
 cbNoCross->Visible = optServNoCross;

 if(optServSpoils){
  for(int i=0;i<5;i++){
   rgSpoils->Items->Add(keySpoils[i]);
  }
  rgSpoils->ItemIndex=AUnit::GetSpoilsFromFlag(curUnit->endflags);
  rgSpoils->Visible=true;
 }else
  cbNoSpoils->Visible=optServNoSpoils;

 if(curUnit->GetEndFlag(FLAG_CONSUMING_FACTION)){
  rgConsume->ItemIndex=2;
 }else if(curUnit->GetEndFlag(FLAG_CONSUMING_UNIT)){
  rgConsume->ItemIndex=1;
 }else{
  rgConsume->ItemIndex=0;
 }
}
void __fastcall TSetFlagsForm::bnOkClick(TObject *Sender)
{
 unsigned _guard=GUARD_NONE;
 if(cbGuard->Checked) _guard=GUARD_GUARD;
 else if(cbAvoid->Checked) _guard=GUARD_AVOID;
 unsigned _flags=0;
 switch(rgConsume->ItemIndex){
  case REVEAL_UNIT: _flags|=FLAG_CONSUMING_UNIT;break;
  case REVEAL_FACTION: _flags|=FLAG_CONSUMING_FACTION;break;
 }
 if(cbNoAid->Checked) _flags|=FLAG_NOAID;
 if(cbHold->Checked) _flags|=FLAG_HOLDING;
 if(cbAutoTax->Checked) _flags|=FLAG_AUTOTAX;
 if(cbBehind->Checked) _flags|=FLAG_BEHIND;
 if(cbSharing->Visible)
  if(cbSharing->Checked) _flags|=FLAG_SHARING;
 if(cbNoCross->Visible)
  if(cbNoCross->Checked) _flags|=FLAG_NOCROSS_WATER;
 if(cbNoSpoils->Visible)
  if(cbNoSpoils->Checked) _flags|=FLAG_NOSPOILS;
 if(rgSpoils->Visible)
  _flags|=AUnit::GetFlagFromSpoils(rgSpoils->ItemIndex);
 curUnit->SetFlagOrders(_guard,rgReveal->ItemIndex,_flags);
}
//---------------------------------------------------------------------------
void __fastcall TSetFlagsForm::cbGuardClick(TObject *Sender)
{
 if(cbGuard->Checked)
  cbAvoid->Checked=false;
}
void __fastcall TSetFlagsForm::cbAvoidClick(TObject *Sender)
{
 if(cbAvoid->Checked)
  cbGuard->Checked=false;
}
//---------------------------------------------------------------------------
