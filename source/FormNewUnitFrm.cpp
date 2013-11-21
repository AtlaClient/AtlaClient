//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <algorithm>

#include "util.h"
#include "unit.h"
#include "FormNewUnitFrm.h"
#include "region.h"
#include "orders.h"
#include "markets.h"
#include "items.h"
#include "faction.h"
#include "gameparams.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TFormNewUnitForm *FormNewUnitForm;

extern AnsiString keySpoils[5]; //from orders.cpp
//---------------------------------------------------------------------------
__fastcall TFormNewUnitForm::TFormNewUnitForm(TComponent* Owner)
  : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TFormNewUnitForm::edAliasKeyPress(TObject *Sender,
      char &Key)
{
 if(Key==8) return;
 if(Key<'0'||Key>'9') Key=0; 
}
//---------------------------------------------------------------------------
void __fastcall TFormNewUnitForm::cbGuardClick(TObject *Sender)
{
 if(cbGuard->Checked)
  cbAvoid->Checked=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormNewUnitForm::cbAvoidClick(TObject *Sender)
{
 if(cbAvoid->Checked)
  cbGuard->Checked=false;
}
//---------------------------------------------------------------------------


void __fastcall TFormNewUnitForm::FormShow(TObject *Sender)
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

  cbSharing->Enabled = optServerSharing;
  cbNoCross->Enabled = optServNoCross;

  if(optServSpoils && !(rgSpoils->Items->Count))
  {
	for(int i=0;i<5;i++)
	{
	  rgSpoils->Items->Add(keySpoils[i]);
	}
	rgSpoils->ItemIndex=AUnit::GetSpoilsFromFlag(curUnit->endflags);
	rgSpoils->Enabled = true;
  } else cbNoSpoils->Enabled = optServNoSpoils;

  if(curUnit->GetEndFlag(FLAG_CONSUMING_FACTION))
  {
	rgConsume->ItemIndex=2;
  } else
  if(curUnit->GetEndFlag(FLAG_CONSUMING_UNIT))
  {
	rgConsume->ItemIndex=1;
  } else rgConsume->ItemIndex=0;

  int nn = -curRegion->GetNewUnitNum();
  edAlias->Text = nn;
  edAlias->SetFocus();
  edAlias->SelectAll();
}
//---------------------------------------------------------------------------

struct SortUnItem{
 int num;
 AnsiString name;
 AUnit *unit;
};
int SortUnItemCompare(const SortUnItem &a,const SortUnItem &b){
 if(a.num>b.num) return true;
 if(a.num<b.num) return false;
 return a.name<b.name;
}
//---------------------------------------------------------------------------

void __fastcall TFormNewUnitForm::bnOkClick(TObject *Sender)
{
 int num=atoi(edAlias->Text.c_str());
 if(num<=0||num>FACTIONMULT){
  Application->MessageBox("Bad alias","Error creating unit", MB_OK);
  ModalResult=mrNone;
 }
}
//---------------------------------------------------------------------------

void __fastcall TFormNewUnitForm::FormCreate(TObject *Sender)
{
  PrepareReg();
}
//---------------------------------------------------------------------------

void __fastcall TFormNewUnitForm::PrepareReg()
{
  if(!curRegion) return;
  AnsiString s;

  cbItemsB->Clear();
  foreach(curRegion->saleds)
  {
	AMarket *mt = *(AMarket**)iter;
	if(mt->type->type&AItemType::IT_MAN)
	{
	  s = mt->type->names+": " + mt->amount + KeyPhrases->Get(keyMarketAtSign) + mt->price;
	  cbItemsB->Items->AddObject(s, (TObject*)mt);
	}
  }
  if(cbItemsB->Items->Count) cbItemsB->ItemIndex = 0;

  cbListS->Clear();
  foreach(SkillTypes)
  {
	ASkillType *stype=*(ASkillType**)iter;
	if(stype->flags&ASkillType::MAGIC)
	  if(!stype->flags&ASkillType::FOUNDATION) continue;
	if(stype->depends[0].Length()) continue;
	s = stype->name;
	if(stype->flags&ASkillType::FOUNDATION) s += " (foundation)";
	cbListS->Items->AddObject(s, (TObject*)stype);
  }
  cbListS->Sorted = true;
  if(cbListS->Items->Count) cbListS->ItemIndex = 0;

  // Get Silver
  TStrings *List = cbTargetsU->Items;
  List->BeginUpdate();
  List->Clear();
  if(curRegion)
  {
	vector<SortUnItem> list;
	curRegion->RunOrders(true,rsAfterGive);
    int nums;
    foreach(curRegion)
    {
      AObject *obj=*(AObject**) iter;
      foreach(obj->units)
      {
        AUnit *un = *(AUnit**) iter;
        if(!un->IsLocal()) continue;
        nums = un->enditems->GetNum("SILV");
        if(!nums) continue;

        SortUnItem si;
		si.unit = un;
        si.name = un->FullName();
        si.num  = nums;
        list.push_back(si);
      } // for each units
    } // for objects

	sort(list.begin(), list.end(), SortUnItemCompare);

    for(int i=0,endi=list.size();i<endi;i++)
    {
      SortUnItem &si = list[i];
      AUnit *un = si.unit;
      s = si.name;
      if(un->faction->number) { s += ", "; s += un->faction->FullName(); }
      s += " (max ";
      s += si.num;
      s += ")";
      List->AddObject(s, (TObject*) un);
    }
  } // If cur Region
  List->EndUpdate();
  if(cbTargetsU->Items->Count) cbTargetsU->ItemIndex = 0;
  cbTargetsUChange(0);

  CalcMoney();
}
//---------------------------------------------------------------------------

void __fastcall TFormNewUnitForm::cbTargetsUChange(TObject *Sender)
{
  if(cbTargetsU->ItemIndex < 0) return;
  AUnit *un = ((AUnit*)cbTargetsU->Items->Objects[cbTargetsU->ItemIndex]);
  edReceive->MaxValue = un->enditems->GetNum("SILV");
}
//---------------------------------------------------------------------------

void __fastcall TFormNewUnitForm::btAllSClick(TObject *Sender)
{
  if(cbTargetsU->ItemIndex < 0) { edReceive->Value = 0; return; }
  btAllNeedS->Down = false;
  AUnit *un = ((AUnit*)cbTargetsU->Items->Objects[cbTargetsU->ItemIndex]);
  edReceive->Value = un->enditems->GetNum("SILV");
}
//---------------------------------------------------------------------------

void __fastcall TFormNewUnitForm::bnAllBClick(TObject *Sender)
{
  if(cbItemsB->ItemIndex < 0) { edCountB->Value = 0; return; }
  AMarket *mt = ((AMarket*)cbItemsB->Items->Objects[cbItemsB->ItemIndex]);
  int maxbuy = edReceive->Value / mt->price;
  if(maxbuy > mt->amount) maxbuy = mt->amount;
  edCountB->Value = maxbuy;
  CalcMoney();
}
//---------------------------------------------------------------------------

void __fastcall TFormNewUnitForm::cbAllBClick(TObject *Sender)
{
  edCountB->Enabled = (!cbAllB->Checked);
  if(cbAllB->Checked) bnAllBClick(0);
}
//---------------------------------------------------------------------------

int TFormNewUnitForm::CalcMoney()
{
  int money = 0, m;
  lbNeedM->Caption = "0";
  // Buy
  if((edCountB->Value > 0) && (cbItemsB->ItemIndex>=0) && (!cbCommB->Checked))
  {
	AMarket *mt = ((AMarket*)cbItemsB->Items->Objects[cbItemsB->ItemIndex]);
	money = edCountB->Value * mt->price;
	lbNeedB->Caption = money;
	// Maintenance
	if(edMonth->Value > 0)
	{
	  if(mt->type->type&AItemType::IT_LEADER) m = LeaderMaintCost; else m = ManMaintCost;
	  m = m * edCountB->Value * edMonth->Value;
	  lbNeedM->Caption = m;
	  money += m;
	}
  } else lbNeedB->Caption = "0";

  // Study
  if(cbStudy->Checked && (cbListS->ItemIndex>=0) && (!cbCommS->Checked)){
	ASkillType *stype = ((ASkillType*)cbListS->Items->Objects[cbListS->ItemIndex]);
	m = stype->cost * edMonS->Value * edCountB->Value;
	lbNeedS->Caption = m;
	money += m;
  } else lbNeedS->Caption = "0";

  lbNeedAll->Caption = money;

  if(btAllNeedS->Down) edReceive->Value = money;
  if(money > edReceive->Value) lbNeedAll->Font->Color = clRed;
	else lbNeedAll->Font->Color = clGreen;
  return money;
}
//---------------------------------------------------------------------------


void __fastcall TFormNewUnitForm::ChangeInfo(TObject *Sender)
{
  if(edCountB->Text != "")
  CalcMoney();
}
//---------------------------------------------------------------------------


void __fastcall TFormNewUnitForm::cbListSChange(TObject *Sender)
{
  cbStudy->Checked = true;
  CalcMoney();
}
//---------------------------------------------------------------------------

void __fastcall TFormNewUnitForm::btAllNeedSClick(TObject *Sender)
{
  if(btAllNeedS->Down) CalcMoney();
}
//---------------------------------------------------------------------------

