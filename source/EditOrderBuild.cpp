//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "util.h"
#include "orders.h"
#include "region.h"
#include "unit.h"
#include "items.h"
#include "EditOrderBuild.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderBuildForm *EditOrderBuildForm;
struct Buil{
 int needed;
 int prod;
 int have;
 AObjectType *otype;
};
//---------------------------------------------------------------------------
__fastcall TEditOrderBuildForm::TEditOrderBuildForm(TComponent* Owner, AOrderBuild *_ord)
    : TForm(Owner),ord(_ord)
{
  cbRepeating->Checked=ord->repeating;
  cbCommented->Checked=ord->commented;
  AUnit *un=_ord->orders->unit;
  AObject *obj=un->endobj;
  TStringList *list=(TStringList *)cbBuilding->Items;
  Buil *buil;
  AnsiString s;
  if(obj->incomplete>0)
  {
	AObjectType *otype=ObjectTypes->Get(obj->type);
	if(un->GetSkill(otype->skill)>=otype->level)
	{
	  buil =new Buil;
	  buil->needed = -obj->incomplete;
	  buil->otype=otype;
	  s = "Continue building ";
	  s += obj->FullName();
	  list->AddObject(s,(TObject*)buil);
	  if(_ord->object.Length()==0)
	  {
		cbBuilding->ItemIndex=0;
	  }
	}
  } else
  if(obj->IsRoad())
  {
	AObjectType *otype=ObjectTypes->Get(obj->type);
	buil = new Buil;
	buil->needed = -(24+3)/4;
	buil->otype = otype;
	s = "Maintenance road ";
	s += obj->FullName();
	list->AddObject(s,(TObject*)buil);
	if(_ord->object.Length()==0)
	{
	  cbBuilding->ItemIndex = 0;
	}
  }
  foreach(ObjectTypes)
  {
	AObjectType *otype=*(AObjectType**)iter;
	if(otype->skill.Length()==0) continue;
	if(un->GetSkill(otype->skill)<otype->level) continue;
	buil = new Buil;
	buil->needed = otype->cost;
	buil->otype = otype;
	s = otype->name;
	int count = 0;
	foreach(obj->basereg)
	{
	  AObject *_obj=*(AObject**)iter;
	  if(_obj->type==s) count++;
	}
	if(count)
	{
	  s += " (";
	  s += count;
	  s += ")";
	}
	if(otype->helpprodmult)
	{
	  s += " +";
	  s += otype->helpprodmult;
	  if(otype->helpproditem.Length())
	  {
		s += "*"+ItemTypes->AbrToName(otype->helpproditem);
	  }
	}
	list->AddObject(s,(TObject*)buil);
	if(_ord->object==otype->name)
	{
	  cbBuilding->ItemIndex=list->Count-1;
	}
  }
  for(int i=list->Count-1;i>=0;i--)
  {
	Buil *buil=(Buil*)list->Objects[i];
	buil->prod=un->GetSkill(buil->otype->skill)*un->GetMen()+un->GetBuildBonus(buil->otype);
	if(buil->otype->item==WoodOrStoneStr) buil->have = un->enditems->GetNum("WOOD")+un->enditems->GetNum("STON");
	  else buil->have=un->enditems->GetNum(buil->otype->item);
  }
  cbBuildingChange(0);
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderBuildForm::bnOkClick(TObject *Sender)
{
 ord->repeating=cbRepeating->Checked;
 ord->commented=cbCommented->Checked;
 int ind=cbBuilding->ItemIndex;
 if(ind>=0){
  Buil *buil=(Buil*)cbBuilding->Items->Objects[ind];
  ord->object=(buil->needed<0)?AnsiString(""):buil->otype->name;
 }
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderBuildForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}
//---------------------------------------------------------------------------

void __fastcall TEditOrderBuildForm::FormDestroy(TObject *Sender)
{
 TStringList *list=(TStringList *)cbBuilding->Items;
 for(int i=0;i<list->Count;i++){
  Buil *buil=(Buil*)list->Objects[i];
  delete buil;
 }
}
//---------------------------------------------------------------------------

void __fastcall TEditOrderBuildForm::cbBuildingChange(TObject *Sender)
{
 int ind=cbBuilding->ItemIndex;
 if(ind<0){
  AnsiString s;
  labProd->Caption=s;
  labCost->Caption=s;
  labHave->Caption=s;
  labProt->Caption=s;
  labCap->Caption=s;
  labSail->Caption=s;
  return;
 }
 Buil *buil=(Buil*)cbBuilding->Items->Objects[ind];
 AObjectType *otype=buil->otype;
 AnsiString s;
 s="Productivity: ";
 s+=buil->prod;
 labProd->Caption=s;
 s="Needed ";
 if(buil->needed<0) s+=-buil->needed;
 else s+=buil->needed;
 s+=" ";
 if(otype->item==WoodOrStoneStr){
  s+="wood or stone";
 }else s+=ItemTypes->AbrToNames(otype->item);
 labCost->Caption=s;
 s="Have ";
 s+=buil->have;
 labHave->Caption=s;
 if(otype->protect){
  s="Protects ";
  s+=otype->protect;
  s+=" men";
 }else s="";
 labProt->Caption=s;
 if(otype->capacity){
  s="Capacity ";
  s+=otype->capacity;
 }else s="";
 labCap->Caption=s;
 if(otype->sailors){
  s="Sailors ";
  s+=otype->sailors;
 }else s="";
 labSail->Caption=s;
}
//---------------------------------------------------------------------------

