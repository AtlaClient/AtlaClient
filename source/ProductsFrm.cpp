//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <map.h>

#include "util.h"
#include "ProductsFrm.h"
#include "region.h"
#include "markets.h"
#include "unit.h"
#include "faction.h"
#include "orders.h"
#include "skills.h"
#include "AboutFrm.h"
#include "MainFrm.h"
#include "SupportBuildingFrm.h"
#include "turn.h"
#include "RegionListFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TProductsForm *ProductsForm;
AnsiString TProductsForm::Item="STON";
int TProductsForm::DefLevel=3;
static int colwidths[6]={3,1,1,1,2,2};

class TElem{
public:
  ARegion *reg;
  int curprod,maxprod,used;
  map<int,int>producers,idle;

  int curneed,maxneed;
  AnsiString producersstr,idlestr;

  TElem():reg(0),curprod(0),maxprod(0),used(0){}
};
class TSumm{
public:
  int curprod,maxprod;
  int curneed,maxneed;
  int producers,idle;
};
static vector<TElem> data;
static TSumm summ;
//---------------------------------------------------------------------------
AnsiString GetMapStr(map<int,int>&m)
{
  AnsiString str,str2;
  for(map<int,int>::iterator i=m.begin();i!=m.end();i++)
  {
    if(str.Length())
      str.Insert("+",1);
    str2.printf("%dL%d",i->second,i->first);
    str.Insert(str2,1);
  }
  return str;
}
//---------------------------------------------------------------------------
__fastcall TProductsForm::TProductsForm(TComponent* Owner)
  : TForm(Owner),ToolType(0),IType(0)
{
  int w=Grid->Width-30;
  int s=0;
  for(int i=0;i<6;i++)
    s+=colwidths[i];
  w/=s;
  for(int i=0;i<6;i++)
    Grid->ColWidths[i]=w*colwidths[i];

  Grid->Cells[0][0]="Region";
  Grid->Cells[1][0]="Cur/Max";
  Grid->Cells[2][0]="Needed";
  Grid->Cells[3][0]="Used";
  Grid->Cells[4][0]="Producers";
  Grid->Cells[5][0]="Idle";
}
//---------------------------------------------------------------------------
void __fastcall TProductsForm::FormShow(TObject *Sender)
{
  MakeItemsList();
  CollectData();
  MakeTable();
}
//---------------------------------------------------------------------------
void TProductsForm::CollectInRegion(ARegion*reg)
{
  AnsiString userinfo=curTurn->RegUserInfos[reg];
  if(userinfo.Pos("prodignore")) return;
  AMarket *prod=reg->products->GetMarket(IType);
  TElem el;
  int numbuilds=0;
  if(userinfo.Pos("produse"))
    el.reg=reg;
  reg->RunOrders(false);
  foreach(reg)
  {
    AObject *obj=*(AObject**)iter;
    if(obj->num&&HasBonusObject)
    {
      AObjectType *otype=ObjectTypes->Get(obj->type);
      if(otype&&otype->production==IType->abr)
        numbuilds++;
    }
    foreach(obj->units)
    {
      AUnit *un=*(AUnit**)iter;
      if(!un->faction->local) continue;
      if(prod&&un->endguard==GUARD_GUARD)
        el.reg=reg;
      int lev=un->GetSkill(IType->skill);
      if(!lev) continue;
      el.reg=reg;
      AOrder *_ord=un->orders->GetMonthLong();
      int men=un->GetMen();
      if(_ord&&_ord->type==O_PRODUCE)
      {
        AOrderAbr *ord=(AOrderAbr*)_ord;
        if(ord->abr==Item)
          el.producers[lev]+=men;
        continue;
      }
/*
      if(_ord&&_ord->type==O_STUDY)
        lev=un->GetBegSkill(IType->skill);
*/        
      el.idle[lev]+=men;
    }
  }
  if(!el.reg) return;
  el.curprod=prod?prod->amount:0;
  int beg=HasBonusObject?el.curprod:0;
  if(numbuilds) beg=TSupportBuildingForm::CalcBegProd(el.curprod,numbuilds);
  if(beg==0)
    el.maxprod=el.curprod;
  else
    el.maxprod=TSupportBuildingForm::CalcMaxProd(beg);
  el.used=prod?prod->price:0;
  el.producersstr=GetMapStr(el.producers);
  el.idlestr=GetMapStr(el.idle);
  data.push_back(el);
}
//---------------------------------------------------------------------------
void TProductsForm::CollectData()
{
  data.clear();
  DisableEvents();
  try{
    foreach(curRegionList)
    {
      ARegion *reg=*(ARegion**)iter;
      CollectInRegion(reg);
    }
  }catch(...){}
  EnableEvents();
}
//---------------------------------------------------------------------------
void TProductsForm::MakeTable()
{
  int count=data.size();
  if(!count)
  {
    Grid->RowCount=3;
    AnsiString str;
    for(int i=0;i<Grid->ColCount;i++)
    {
      Grid->Cells[i][1]=str;
      Grid->Cells[i][2]=str;
    }
    return;
  }
  UpdateSumm();
  Grid->RowCount=count+2;
  int row=2;
  Grid->Cells[1][1]=AnsiString(summ.curprod)+"/"+summ.maxprod;
  Grid->Cells[2][1]=AnsiString(summ.curneed)+"/"+summ.maxneed;
  Grid->Cells[4][1]=summ.producers;
  Grid->Cells[5][1]=summ.idle;

  int m1,m2;
  for(TElem *el=data.begin().operator ->(),*endel=data.end().operator ->();el<endel;el++,row++)
  {
    Grid->Cells[0][row]=el->reg->FullName(false);
    Grid->Cells[1][row]=AnsiString(el->curprod)+"/"+el->maxprod;
    Grid->Cells[2][row]=AnsiString(el->curneed)+"/"+el->maxneed;
    m1=0;
    if(el->curprod) m1=el->used*100/el->curprod;
    m2=0;
    if(el->maxprod) m2=el->used*100/el->maxprod;
    Grid->Cells[3][row]=AnsiString().sprintf("%d/%d%%/%d%%",el->used,m1,m2);
    Grid->Cells[4][row]=el->producersstr;
    Grid->Cells[5][row]=el->idlestr;
    if(el->reg==curRegion)
      Grid->Row=row;
  }
}
//---------------------------------------------------------------------------
void TProductsForm::MakeItemsList()
{
  TStrings *list=cbItems->Items;
  list->BeginUpdate();
  list->Clear();
  int ind=-1;
  foreach(ItemTypes)
  {
    AItemType *itype=*(AItemType**)iter;
    if(!itype->skill.Length()) continue;
    if(itype->inputs[0].Length()) continue;
    if(itype->abr==Item)
      ind=list->Count;
    list->AddObject(itype->name,(TObject*)itype);
  }
  list->EndUpdate();
  if(ind<0) ind=0;
  cbItems->ItemIndex=ind;
  cbItemsChange(0);
  cbLevel->ItemIndex=DefLevel-1;
}
//---------------------------------------------------------------------------
void __fastcall TProductsForm::cbItemsChange(TObject *Sender)
{
  int ind=cbItems->ItemIndex;
  if(ind<0) return;
  IType=(AItemType*)cbItems->Items->Objects[ind];
  ToolType=0;
  if(IType->helpprodmult&&IType->helpproditem.Length())
  {
    ToolType=ItemTypes->Get(IType->helpproditem,4);
  }
  AnsiString str;
  if(IType->helpprodmult)
  {
    if(ToolType)
      str=ToolType->name;
    str+=" +";
    str+=IType->helpprodmult;
  }
  labTools->Caption=str;
  HasBonusObject=false;
  foreach(ObjectTypes)
  {
    AObjectType *otype=*(AObjectType**)iter;
    if(otype->production!=IType->abr) continue;
    HasBonusObject=true;
    break;
  }
  if(IType->abr==Item) return;
  Item=IType->abr;
  CollectData();
  MakeTable();
}
//---------------------------------------------------------------------------
void __fastcall TProductsForm::GridDblClick(TObject *Sender)
{
  int ind=Grid->Row-2;
  if((unsigned)ind>=data.size()) return;
  TElem &el=data[ind];
  AtlaForm->GotoRegion(el.reg);
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TProductsForm::cbLevelChange(TObject *Sender)
{
  DefLevel=cbLevel->ItemIndex+1;
  MakeTable();
}
//---------------------------------------------------------------------------
void TProductsForm::UpdateSumm()
{
  summ.curprod=0;
  summ.maxprod=0;
  summ.curneed=0;
  summ.maxneed=0;
  summ.producers=0;
  summ.idle=0;
  int div=DefLevel+IType->helpprodmult;
  for(TElem *el=data.begin().operator ->(),*endel=data.end().operator ->();el<endel;el++)
  {
    summ.curprod+=el->curprod;
    summ.maxprod+=el->maxprod;
    int m1=0;
    if(el->curprod) m1=(el->curprod-1)/div+1;
    int m2=0;
    if(el->maxprod) m2=(el->maxprod-1)/div+1;
    el->curneed=m1;
    el->maxneed=m2;
    summ.curneed+=m1;
    summ.maxneed+=m2;
    for(map<int,int>::iterator i=el->producers.begin();i!=el->producers.end();i++)
      summ.producers+=i->second;
    for(map<int,int>::iterator i=el->idle.begin();i!=el->idle.end();i++)
      summ.idle+=i->second;
  }
}
//---------------------------------------------------------------------------
void __fastcall TProductsForm::bnToRegionListClick(TObject *Sender)
{
  RegionListForm->miClearListClick(0);
  for(TElem *el=data.begin().operator ->(),*endel=data.end().operator ->();el<endel;el++)
    RegionListForm->AddRegion(el->reg);
  RegionListForm->Visible=true;  
  AtlaForm->DrawMap();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------

