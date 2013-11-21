//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <vector.h>
#include <algorithm>
#include <Math.h>
//---------------------------------------------------------------------------
#include "util.h"
#include "orders.h"
#include "unit.h"
#include "region.h"
#include "items.h"
#include "EditOrderReceiveFrm.h"
#include "faction.h"
#include "turn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderReceiveForm *EditOrderReceiveForm;
static AnsiString lastitem;
//---------------------------------------------------------------------------
__fastcall TEditOrderReceiveForm::TEditOrderReceiveForm(TComponent* Owner)
  : TForm(Owner),items(new TStringList),targets(new AUnits(curRegion->GetDummy())),
	added(false)
{
  targets->autodelete = false;
  if(!curUnit||!curRegion) return;
  curRegion->RunOrders(true, rsAfterGive);
  curUnit->UpdateWeight();
  foreach(curRegion)
  {
	AObject *obj=*(AObject**) iter;
	foreach(obj->units)
	{
	  AUnit *un = *(AUnit**) iter;
	  if(un == curUnit) continue;
	  if(!un->IsLocal()) continue;
	  targets->Add(un);
	}
  }
  Init();
}
//---------------------------------------------------------------------------
void TEditOrderReceiveForm::Init()
{
  AnsiString s;
  curtarget = 0;
  curitem = "";
  items->Clear();
  foreach(targets)
  {
	AUnit *un = *(AUnit**)iter;
	un->enditems->DeleteEmpty();
	foreach(un->enditems)
	{
	  AItem *it=*(AItem**) iter;
	  if(items->IndexOf(it->type->abr)==-1)
	  {
		items->Add(it->type->abr);
	  }
	}
  }

  for(int i=items->Count-1;i>=0;i--)
  {
	s = items->Strings[i];
	AItemType *itype=ItemTypes->Get(s);
	if(!itype||(itype->flags&AItemType::CANTGIVE))
	{
	  items->Delete(i);
	} else {
	  items->Objects[i]=(TObject*) itype;
	}
  }
  cbItemsChange(0);
  cbTargetsChange(0);
  if(items->IndexOf(lastitem) >= 0)
  {
	cbItems->ItemIndex = cbItems->Items->IndexOf(lastitem);
	curitem = lastitem;
  }
  MakeItemsList();
  MakeTargetsList();
  UpdateCapacity();
}
//---------------------------------------------------------------------------
__fastcall TEditOrderReceiveForm::~TEditOrderReceiveForm()
{
  delete items;
  delete targets;
}
//---------------------------------------------------------------------------
struct SortUnItem{
  int num;
  AnsiString name;
  AUnit *unit;
};
//---------------------------------------------------------------------------
int SortUnItemCompare(const SortUnItem &a,const SortUnItem &b){
  if(a.num>b.num) return true;
  if(a.num<b.num) return false;
  return a.name<b.name;
}
//---------------------------------------------------------------------------
void TEditOrderReceiveForm::MakeTargetsList()
{
  AnsiString s;
  vector<SortUnItem> list;
  foreach(targets)
  {
	AUnit *un=*(AUnit**)iter;
	SortUnItem si;
	si.unit = un;
	si.name = un->FullName();
	si.num = 0;
	if(cbItems->ItemIndex < 6)
	{
	  // Typed Items
	  foreach(un->enditems)
	  {
		AItem *it = *(AItem**)iter;
		// it->
		switch (cbItems->ItemIndex)
		{
		  case 0 : // Advanced
			if(it->type->type&AItemType::IT_ADVANCED) si.num += it->count;
			break;
		  case 1 : // Armor
			if(it->type->type&AItemType::IT_ARMOR) si.num += it->count;
			break;
		  case 2 : // Food
			if(it->type->type&AItemType::IT_FOOD) si.num += it->count;
			break;
		  case 3 : // Mounts
			if(it->type->type&AItemType::IT_MOUNT) si.num += it->count;
			break;
		  case 4 : // Trade
			if(it->type->type&AItemType::IT_TRADE) si.num += it->count;
			break;
		  case 5 : // Weapon
			if(it->type->type&AItemType::IT_WEAPON) si.num += it->count;
		}
	  }
	} else
	if(curitem.Length()) si.num = un->enditems->GetNum(curitem);
	list.push_back(si);
  }
  sort(list.begin(),list.end(),SortUnItemCompare);

  TStrings *List = cbTargets->Items;
  bool bb = (cbTargets->ItemIndex == 0);
  List->BeginUpdate();
  List->Clear();
  List->Add("From All units");

  for(int i=0,endi=list.size();i<endi;i++)
  {
	SortUnItem &si=list[i];
	AUnit *un = si.unit;
	s = si.name;
	if(un->faction->number)
	{
	  s += ", ";
	  s += un->faction->FullName();
	}
	if( (cbItems->ItemIndex >= 0) && (cbItems->ItemIndex < 6))
	{
	  if(!si.num) continue;
	  s += " (max ";
	  s += si.num;
	  s += ")";
	} else
	if(curitem.Length())
	{
	  if(!si.num) continue;
	  s += " (max ";
	  s += si.num;
	  s += ")";
	}
	List->AddObject(s,(TObject*) un);
	if(un == curtarget) cbTargets->ItemIndex = List->Count-1;
  }
  if(bb) cbTargets->ItemIndex = 0;
  List->EndUpdate();
}
//---------------------------------------------------------------------------
struct SortInItem{
  int num;
  AnsiString name;
  AItemType *itype;
};
//---------------------------------------------------------------------------
int SortInItemCompare(const SortInItem &a,const SortInItem &b){
  if(a.num>b.num) return true;
  if(a.num<b.num) return false;
  return a.name<b.name;
}
//---------------------------------------------------------------------------
void TEditOrderReceiveForm::MakeItemsList()
{
  int ind = cbItems->ItemIndex;
  vector<SortInItem> lst;
  for(int i=0,endi=items->Count;i<endi;i++)
  {
	SortInItem si;
	si.itype=(AItemType*) items->Objects[i];
	si.name=si.itype->name;
	if(curtarget) si.num = curtarget->enditems->GetNum(si.itype->abr); else si.num = 0;
   lst.push_back(si);
  }
  sort(lst.begin(),lst.end(),SortInItemCompare);

  TStrings *list = cbItems->Items;
  AnsiString s;
  list->BeginUpdate();
  list->Clear();
  list->Add("...All Advanced Items");
  list->Add("...All Armor Items");
  list->Add("...All Food Items");
  list->Add("...All Mount Items");
  list->Add("...All Trade Items");
  list->Add("...All Weapon Items");
  if(ind >= 0 && ind < 6) cbItems->ItemIndex = ind;
  for(int i=0,endi=lst.size();i<endi;i++)
  {
	SortInItem &si=lst[i];
	s = si.name;
	if(curtarget)
	{
	  s+=" (max ";
	  s+=si.num;
	  s+=")";
	}
	list->AddObject(s,(TObject*)si.itype);
	if(si.itype->abr == curitem) cbItems->ItemIndex = list->Count-1;
  }
  list->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderReceiveForm::cbItemsChange(TObject *Sender)
{
  int ind = cbItems->ItemIndex;
  int ind2 = cbTargets->ItemIndex;
  edCount->Visible = (ind>=0)&&(ind2>0);
  bnAll->Visible = (ind>=0)&&(ind2>0);
  if(ind < 0) curitem = ""; else
  if(ind < 6) curitem = cbItems->Text; else
  {
	AItemType *itype=(AItemType*) cbItems->Items->Objects[ind];
	curitem = itype->abr;
  }
  if(ind < 0) return;
  MakeTargetsList();
  UpdateCapacity();
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderReceiveForm::cbTargetsChange(TObject *Sender)
{
  int ind = cbTargets->ItemIndex;
  int ind2 = cbItems->ItemIndex;
  edCount->Visible = (ind>0)&&(ind2>5);
  bnAll->Visible = (ind>0)&&(ind2>5);
  if(ind < 0){ curtarget=0; return; }
  if(ind > 0)
  {
	AUnit *un=(AUnit*) cbTargets->Items->Objects[ind];
	curtarget = un;
  } else curtarget = 0;
  MakeItemsList();
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderReceiveForm::FormPaint(TObject *Sender)
{
  static bool first=true;
  if(first)
  {
	first = true;
	if(!curUnit||!curRegion) ModalResult=mrCancel;
  }
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderReceiveForm::bnAllClick(TObject *Sender)
{
  edCount->Text = curtarget->enditems->GetNum(curitem);
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderReceiveForm::bnOkClick(TObject *Sender)
{
  if((cbItems->ItemIndex >= 0) && (cbTargets->ItemIndex == 0))
  {
	// Âçÿòü ÂÑÅ àéòåìû ñî ÂÑÅÕ þíèòîâ!
	bool need;
	foreach(targets)
	{
	  AUnit *un = *(AUnit**)iter;
	  if(un == curUnit) continue;
	  foreach(un->enditems)
	  {
		AItem *it = *(AItem**)iter;
		switch (cbItems->ItemIndex)
		{
		  case 0 : // Advanced
			need = (it->type->type&AItemType::IT_ADVANCED);
			break;
		  case 1 : // Armor
			need = (it->type->type&AItemType::IT_ARMOR);
			break;
		  case 2 : // Food
			need = (it->type->type&AItemType::IT_FOOD);
			break;
		  case 3 : // Mounts
			need = (it->type->type&AItemType::IT_MOUNT);
			break;
		  case 4 : // Trade
			need = (it->type->type&AItemType::IT_TRADE);
			break;
		  case 5 : // Weapon
			need = (it->type->type&AItemType::IT_WEAPON);
			break;
		  default : need = (it->type->abr == curitem);
		}
		if(need)
		{
		  AOrderGive *ord = new AOrderGive;
		  ord->tarnum = curUnit->num;
		  ord->target = curUnit;
		  ord->item = it->type->abr;
		  ord->count = -2;//it->count;
		  ord->repeating = cbRepeating->Checked;
		  un->orders->Add(ord);
		  curTurn->Modified = true;
		}

	  }


	}

	if(Sender == bnOk)
	{
	  ModalResult = mrOk;
	  return;
	}
	added = true;
	curRegion->RunOrders(true, rsAfterGive);
	curUnit->UpdateWeight();
	SetFocus();
	Init();
	return;
  }
  if(!curtarget) return;
  if(!curitem.Length()) return;
  if( (cbItems->ItemIndex >= 0) && (cbItems->ItemIndex < 6))
  {
	// All Typed Items
	foreach(curtarget->items)
	{
	  bool need = false;
	  AItem *it = *(AItem**)iter;
	  switch (cbItems->ItemIndex)
	  {
		case 0 : // Advanced
		  need = (it->type->type&AItemType::IT_ADVANCED);
		  break;
		case 1 : // Armor
		  need = (it->type->type&AItemType::IT_ARMOR);
		  break;
		case 2 : // Food
		  need = (it->type->type&AItemType::IT_FOOD);
		  break;
		case 3 : // Mounts
		  need = (it->type->type&AItemType::IT_MOUNT);
		  break;
		case 4 : // Trade
		  need = (it->type->type&AItemType::IT_TRADE);
		  break;
		case 5 : // Weapon
		  need = (it->type->type&AItemType::IT_WEAPON);
	  }
	  if(need)
	  {
		AOrderGive *ord = new AOrderGive;
		ord->tarnum = curUnit->num;
		ord->target = curUnit;
		ord->item = it->type->abr;
		ord->count = -2;//it->count;
		ord->repeating = cbRepeating->Checked;
		curtarget->orders->Add(ord);
		curTurn->Modified = true;
	  }
	}
  } else
  {
	int count = atoi(edCount->Text.c_str());
	if(count <= 0) return;
	lastitem = curitem;
	AOrderGive *ord = new AOrderGive;
	ord->tarnum = curUnit->num;
	ord->target = curUnit;
	ord->item = curitem;
	ord->count = count;
	ord->repeating = cbRepeating->Checked;
	curtarget->orders->Add(ord);
	curTurn->Modified = true;
  }

  if(Sender == bnOk)
  {
	ModalResult=mrOk;
	return;
  }
  added = true;
  curRegion->RunOrders(true, rsAfterGive);
  curUnit->UpdateWeight();
  SetFocus();
  Init();
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderReceiveForm::bnCancelClick(TObject *Sender)
{
  if(added) ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void TEditOrderReceiveForm::UpdateCapacity()
{
  AnsiString str = "";
  lbNeed->Caption = "";
  if(!curitem.Length() || cbItems->ItemIndex<6)
  {
	labCapacity->Caption = str;
	return;
  }

  AItemType *itype = ItemTypes->Get(curitem, 4);
  //int RealW = itype->weight;
  //if( (itype->abr == "SILV") && (SilverWeight) )
  {
	 //RealW += floor(it->count / SilverWeight);
  }

  //curUnit->RunOrders();
  //curUnit->UpdateWeight();
  
  int walkcapac = itype->weight - itype->walk;
  int ridecapac = itype->weight - itype->ride;
  int flycapac  = itype->weight - itype->fly;
  int swimcapac = itype->weight - itype->swim;

  int wc = curUnit->walkweight - curUnit->weight;
  int rc = curUnit->ridweight  - curUnit->weight;
  int fc = curUnit->flyweight  - curUnit->weight;
  int sc = curUnit->swimweight - curUnit->weight;

  if(walkcapac <= 0) str +="inf";
  else
  {
	wc = wc / walkcapac;
	if(wc < 0) wc = 0;
	str += wc;
  }
  str+="/";
  if(ridecapac<=0)
	str+="inf";
  else
  {
	rc=rc/ridecapac;
	if(rc<0) rc=0;
	str+=rc;
  }
  str+="/";
  if(flycapac<=0)
    str+="inf";
  else
  {
    fc=fc/flycapac;
    if(fc<0) fc=0;
    str+=fc;
  }
  if(optServNoCross||curUnit->swimweight)
  {
    str+="/";
    if(swimcapac<=0)
      str+="inf";
	else
    {
      sc=sc/swimcapac;
      if(sc<0) sc=0;
      str+=sc;
    }
  }
  labCapacity->Caption = str;
  str = "";
  float w1;
  int w2;

  if( (itype->walk - itype->weight > 0) && (curUnit->weight - curUnit->walkweight > 0) )
  {
	 // Need for Walk
	 str = " walk: ";
	 w1 = (curUnit->weight - curUnit->walkweight);
	 w1 = w1 / (itype->walk - itype->weight);
	 w2 = ceil ( w1 );
	 str += w2;
  }

  if( (itype->ride - itype->weight > 0) && (curUnit->weight - curUnit->ridweight > 0) )
  {
	 // Need for Ride
	 if(str != "") str += ";";
	 str += " ride: ";
	 w1 = (curUnit->weight - curUnit->ridweight);
	 w1 = w1 / (itype->ride - itype->weight);
	 w2 = ceil ( w1 );
	 str += w2;
  }

  if( (itype->fly - itype->weight > 0) && (curUnit->weight - curUnit->flyweight > 0) )
  {
	 // Need for Fly
	 if(str != "") str += ";";
	 str += " fly: ";
	 w1 = (curUnit->weight - curUnit->flyweight);
	 w1 = w1 / (itype->fly - itype->weight);
	 w2 = ceil ( w1 );
	 str += w2;
  }

  if( (itype->swim - itype->weight > 0) && (curUnit->weight - curUnit->swimweight > 0) )
  {
	 // Need for Swim
	 if(str != "") str += ";";
	 str += " swim: ";
	 w1 = (curUnit->weight - curUnit->swimweight);
	 w1 = w1 / (itype->swim - itype->weight);
	 w2 = ceil ( w1 );
	 str += w2;
  }

  if(str.Length() > 0)
  {
	 str = "Need for " + str;
	 lbNeed->Caption = str;
  }

}
//---------------------------------------------------------------------------

