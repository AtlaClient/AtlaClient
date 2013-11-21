//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <vector.h>
#include <algorithm>
#include "ViewTaxRegionsFrm.h"
#include "region.h"
#include "unit.h"
#include "orders.h"
#include "faction.h"
#include "MainFrm.h"
#include "turn.h"
#include "gameparams.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TViewTaxRegionsForm *ViewTaxRegionsForm;
//---------------------------------------------------------------------------
struct RegionInfo{
 ARegion *reg;
 int taxers;
 bool blocked;
 RegionInfo():reg(0),taxers(0),blocked(false){}
 int operator<(const RegionInfo&b) const;
};
int RegionInfo::operator<(const RegionInfo&b)const{
 return reg->money>b.reg->money; 
}
struct FactionInfo{
 AFaction *fac;
 int numreg;
 FactionInfo():fac(0),numreg(0){}
};
struct TaxInfo{
 ARegion *reg;
 AFaction *fac;
 int attempt;
 int cantax;
 TaxInfo():reg(0),fac(0),attempt(0),cantax(0){}
};
static class Regions{
private:
 vector<RegionInfo> regions;
public:
 void Clear(){regions.clear();}
 int Size(){return regions.size();}
 int IndexOf(ARegion*reg);
 RegionInfo& Get(ARegion*reg);
 RegionInfo& Get(int index){ return regions[index];}
 void Sort(){ stable_sort(regions.begin(),regions.end());}
} regions;
static class Factions{
private:
 vector<FactionInfo> factions;
public:
 void Clear(){factions.clear();}
 int Size(){return factions.size();}
 FactionInfo&Get(AFaction*fac);
 FactionInfo&Get(int index){ return factions[index];}
} factions;
static class TaxInfos{
private:
 vector<TaxInfo> taxs;
public:
 void Clear(){taxs.clear();}
 TaxInfo& Get(ARegion*reg,AFaction *fac);
} taxinfos;

int Regions::IndexOf(ARegion *reg){
 for(RegionInfo *i=regions.begin().operator ->(),*endi=regions.end().operator ->();i<endi;i++){
  if(i->reg==reg)
   return i-regions.begin().operator ->();
 }
 return -1;
}
RegionInfo& Regions::Get(ARegion *reg){
 for(RegionInfo *i=regions.begin().operator ->(),*endi=regions.end().operator ->();i<endi;i++){
  if(i->reg==reg)
   return *i;
 }
 RegionInfo ri;
 ri.reg=reg;
 regions.push_back(ri);
 return *(regions.end()-1);
}
FactionInfo& Factions::Get(AFaction*fac){
 for(FactionInfo *i=factions.begin().operator ->(),*endi=factions.end().operator ->();i<endi;i++){
  if(i->fac==fac)
   return *i;
 }
 FactionInfo fi;
 fi.fac=fac;
 factions.push_back(fi);
 return *(factions.end()-1);
}
TaxInfo& TaxInfos::Get(ARegion*reg,AFaction *fac){
 for(TaxInfo *i=taxs.begin().operator ->(),*endi=taxs.end().operator ->();i<endi;i++){
  if(i->reg==reg&&i->fac==fac)
   return *i;
 }
 TaxInfo ti;
 ti.reg=reg;
 ti.fac=fac;
 taxs.push_back(ti);
 return *(taxs.end()-1);
}
__fastcall TViewTaxRegionsForm::TViewTaxRegionsForm(TComponent* Owner)
    : TForm(Owner)
{
 Grid->ColWidths[0]=150;
 Grid->ColWidths[1]=100;

 Init();
}
__fastcall TViewTaxRegionsForm::~TViewTaxRegionsForm()
{
 DisableEvents();
 for(int j=0;j<regions.Size();j++){
  RegionInfo& ri=regions.Get(j);
  ri.reg->RunOrders(false);
 }
 EnableEvents();
 regions.Clear();
 factions.Clear();
 taxinfos.Clear();
}
void TViewTaxRegionsForm::Init()
{
 regions.Clear();
 factions.Clear();
 taxinfos.Clear();
 DisableEvents();
 foreach(curRegionList){
  ARegion *reg=*(ARegion**)iter;
  reg->RunOrders(true,rsAfterGive);
  bool foundtaxers=false;
  int attempttax=0;
  bool blocked=false;
  foreach(reg){
   AObject *obj=*(AObject**)iter;
   foreach(obj->units){
    AUnit *un=*(AUnit**) iter;
    if(!un->endfaction->local) continue;
    int taxers=un->Taxers();
    if(!taxers) continue;
    if(!blocked)
     if(!reg->CanTax(un))
      blocked=true;
    foundtaxers=true;
    factions.Get(un->endfaction);
    TaxInfo& ti=taxinfos.Get(reg,un->endfaction);
    ti.cantax+=taxers;
    if(un->HasTaxOrder()){
     ti.attempt+=taxers;
     attempttax+=taxers;
    }
   }
  }
  if(!foundtaxers) continue;
  RegionInfo& ri=regions.Get(reg);
  ri.taxers=attempttax;
  ri.blocked=blocked;
  if(!reg->money)
   ri.blocked=true;
 }
 EnableEvents();
 regions.Sort();
 for(int i=0;i<factions.Size();i++){
  FactionInfo& fi=factions.Get(i);
  int numreg=0;
  for(int j=0;j<regions.Size();j++){
   RegionInfo& ri=regions.Get(j);
   TaxInfo& ti=taxinfos.Get(ri.reg,fi.fac);
   if(ti.attempt)
    numreg++;
  }
  fi.numreg=numreg;
 }

 if(!factions.Size()||!regions.Size())
   PostMessage(Handle, WM_CLOSE, 0, 0);
 else
   MakeGrid();
}
void TViewTaxRegionsForm::MakeGrid()
{
 int colcount=factions.Size()+2;
 if(colcount<3) colcount=3;
 int rowcount=regions.Size()+2;
 if(rowcount<3) rowcount=3;
 Grid->ColCount=colcount;
 Grid->RowCount=rowcount;
 Grid->Invalidate();
}
void __fastcall TViewTaxRegionsForm::GridDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
 if(!factions.Size()||!regions.Size()) return;
 TCanvas *Canv=Grid->Canvas;
 AnsiString str;
 if(ACol<2){
  if(ARow<2){
   if(ACol==1){
    if(ARow==1)
     str="Money\\TaxRegions";
    else
     str="Faction";
   }else if(ARow==1)
    str="Region";
   Canv->TextRect(Rect, Rect.Left+2, Rect.Top+2, str);
   return;
  }
  RegionInfo& ri=regions.Get(ARow-2);
  if(ACol==0){
   str=ri.reg->FullName(false);
  }else{
   if(ri.blocked)
    str="- ";
   int money=ri.reg->money;
   int cantaxers=money?(ri.reg->money-11)/TaxEffect+1:0;
   str+=money;
   str+="/";
   str+=cantaxers;
   str+="/";
   str+=ri.taxers;
   if(cantaxers<=ri.taxers)
    Canv->Font->Color=clYellow;
  }
  Canv->TextRect(Rect, Rect.Left+2, Rect.Top+2, str);
 }else if(ARow<2){
  FactionInfo& fi=factions.Get(ACol-2);
  if(ARow==0){
   str=fi.fac->FullName();
  }else{
   str=fi.numreg;
   str+="/";
   str+=fi.fac->warmax;
   if(fi.numreg>fi.fac->warmax)
    Canv->Font->Color=clRed;
  }
  Canv->TextRect(Rect, Rect.Left+2, Rect.Top+2, str);
 }else{
  RegionInfo& ri=regions.Get(ARow-2);
  FactionInfo& fi=factions.Get(ACol-2);
  TaxInfo& ti=taxinfos.Get(ri.reg,fi.fac);
  if(ti.cantax){
   str=ti.attempt;
   str+="/";
   str+=ti.cantax;
   if(!ti.attempt)
    Canv->Brush->Color=clYellow;
   else if(ti.attempt<ti.cantax)
    Canv->Brush->Color=clRed;
  }
  if(ri.blocked)
   Canv->Brush->Color=clBtnFace;
  Canv->TextRect(Rect, Rect.Left+2, Rect.Top+2, str);
 }
}
void __fastcall TViewTaxRegionsForm::GridDblClick(TObject *Sender)
{
 TPoint p=Grid->ScreenToClient(Mouse->CursorPos);
 int ACol,ARow;
 Grid->MouseToCell(p.x,p.y,ACol,ARow);
 if(ARow<0) return;
 if(ACol==0){
  if(ARow<2) return;
  RegionInfo& ri=regions.Get(ARow-2);
  AtlaForm->GotoRegion(ri.reg);
  ModalResult=mrOk;
  return;
 }
 if(ACol<2||ARow<2) return;
 RegionInfo&ri=regions.Get(ARow-2);
 FactionInfo&fi=factions.Get(ACol-2);
 TaxInfo& ti=taxinfos.Get(ri.reg,fi.fac);
 if(!ti.cantax) return;
 if(ri.blocked&&!ti.attempt) return;
 foreach(ri.reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**) iter;
   if(un->endfaction!=fi.fac) continue;
   int taxers=un->Taxers();
   if(!taxers) continue;
   if(ti.attempt){ //disable all tax order
    {
     AOrderSimple *ord=(AOrderSimple*)un->orders->Find(O_TAX,2);
     if(ord){
      if(ord->repeating)
       ord->commented=true;
      else
       un->orders->Delete(ord);
     }
    }
    if(un->GetEndFlag(FLAG_AUTOTAX)){
     AOrderByte *ord=(AOrderByte*)un->orders->Find(O_AUTOTAX,2);
     if(!ord){
      ord=new AOrderByte;
      ord->type=O_AUTOTAX;
      un->orders->Add(ord);
     }
     ord->value=0;
    }
   }else{ //enable all tax orders
    {
     AOrderSimple *ord=(AOrderSimple*)un->orders->Find(O_TAX,3);
     if(ord){
      ord->commented=false;
     }else{
      ord=new AOrderSimple;
      ord->type=O_TAX;
      un->orders->Add(ord);
     }
    }
    if(un->GetFlag(FLAG_AUTOTAX)&&!un->GetEndFlag(FLAG_AUTOTAX)){
     AOrderByte *ord=(AOrderByte*)un->orders->Find(O_AUTOTAX,2);
     if(ord){
      un->orders->Delete(ord);
     }
    }
   }
  }
 }
 ri.reg->runned=false;
 curTurn->Modified=true;
 Init();
}

void __fastcall TViewTaxRegionsForm::FormShow(TObject *Sender)
{
 int ind=regions.IndexOf(curRegion);
 if(ind>=0)
   Grid->Row=ind+2;
}
//---------------------------------------------------------------------------

