//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
//#include <map.h>
#include <algorithm>
#include "util.h"
#include "region.h"
#include "BalanceFrm.h"
#include "unit.h"
#include "faction.h"
#include "orders.h"
#include "items.h"
#include "markets.h"
#include "MainFrm.h"
#include "turn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TBalanceForm *BalanceForm;
//---------------------------------------------------------------------------
static char*names[]={
  " by work", //0
  " by entertain", //1
  " by maintenance", //2
  " by tax", //3
  " by produce", //4
  " by buy", //5
  " by sell", //6
  " by study", //7
  " by give to other", //8
  " by cast", //9
  " by build", //10
  " by claim", //11
  " by withdraw", //12
};
class TElem{
public:
  map<int,int> debet,credit; //money by type
  int begturn,endturn;
  int nextbegturn;

//for sort
  int sorttype;
  AItemType *type;

//for output
  AnsiString item;
  int deb,cred;
  AnsiString debstr,credstr;

  TElem():begturn(0),endturn(0),nextbegturn(0){}
};
typedef map<AItemType*,TElem> TElemMap;
int TElemCompare(const TElemMap::iterator&a,const TElemMap::iterator&b){
  int d=a->second.sorttype-b->second.sorttype;
  if(d) return d<0;
  return a->second.type-b->second.type<0;
}
static TElemMap data;
static vector<TElemMap::iterator> indexs;
static AItemType *silvtype=0;
bool TBalanceForm::curregonly=true;
bool TBalanceForm::logging=false;
int TBalanceForm::facdiap=-3;
//---------------------------------------------------------------------------
void LogItem(int logtype, int number, AnsiString item, AUnit *un)
{
  if(!TBalanceForm::logging) return;
  AItemType *itype=ItemTypes->Get(item,4);
  LogItem(logtype,number,itype,un);
}
void LogItem(int logtype, int number, AItemType *itype, AUnit *un)
{
  if(!TBalanceForm::logging) return;
  if(!number) return;
  if(!itype) return;
  if(!inFacDiap(TBalanceForm::facdiap,un->faction))
    return;
  if(number<0)
    data[itype].credit[logtype]-=number;
  else
    data[itype].debet[logtype]+=number;
}
//---------------------------------------------------------------------------
__fastcall TBalanceForm::TBalanceForm(TComponent* Owner)
  : TForm(Owner)
{
  cbCurRegionOnly->Checked=curregonly;
  cbCurRegionOnly->OnClick=cbCurRegionOnlyClick;
}
//---------------------------------------------------------------------------
void TBalanceForm::UpdateGridWidths(bool withnextturn)
{
  if(withnextturn)
    Grid->ColCount=8;
  else
    Grid->ColCount=6;
  int w=Grid->Width-30;
  w-=Grid->DefaultColWidth*(Grid->ColCount-2);
  w/=2;
  Grid->ColWidths[2]=w;
  Grid->ColWidths[3]=w;
}
//---------------------------------------------------------------------------
AnsiString strBorrows="Borrows ";
AnsiString strSilver="silver";
void TBalanceForm::CollectUnit(AUnit * un)
{
  foreach(un->items)
  {
    AItem *it=*(AItem**)iter;
    data[it->type].begturn+=it->count;
  }
  bool gived=!un->endfaction->local;
  foreach(un->enditems)
  {
    AItem *it=*(AItem**)iter;
    if(!it->count) continue;
    if(gived)
      data[it->type].credit[8]+=it->count;
    else
      data[it->type].endturn+=it->count;
  }
}
//---------------------------------------------------------------------------
void TBalanceForm::CollectInRegion(ARegion * reg)
{
  reg->RunOrders(true);
  foreach(reg)
  {
    AObject *obj=*(AObject**)iter;
    foreach(obj->units)
    {
      AUnit *un=*(AUnit**)iter;
      if(!inFacDiap(facdiap,un->faction))
        continue;
      CollectUnit(un);
    }
  }
}
//---------------------------------------------------------------------------
void TBalanceForm::CollectNextUnit(AUnit * un)
{
  foreach(un->items)
  {
    AItem *it=*(AItem**)iter;
    data[it->type].nextbegturn+=it->count;
  }
}
//---------------------------------------------------------------------------
void TBalanceForm::CollectNextInRegion(ARegion * reg)
{
  foreach(reg)
  {
    AObject *obj=*(AObject**)iter;
    foreach(obj->units)
    {
      AUnit *un=*(AUnit**)iter;
      if(!inFacDiap(facdiap,un->faction))
        continue;
      CollectNextUnit(un);
    }
  }
}
//---------------------------------------------------------------------------
void TBalanceForm::CollectData()
{
  data.clear();
  indexs.clear();
  silvtype=ItemTypes->Get("Silv");

  logging=true;
  DisableEvents();
  try{
    if(curregonly)
      CollectInRegion(curRegion);
    else foreach(curRegionList)
    {
      ARegion *reg=*(ARegion**)iter;
      if(!reg->hasInfo) continue;
      CollectInRegion(reg);
    }
    ATurn *turn=0;
    if(curTurnNumber+1<curTurns->count)
      turn=curTurns->Get(curTurnNumber+1);
    if(turn)
    {
      if(curregonly)
      {
        ARegion *reg=turn->RegionList->GetByKey(curRegion->key);
        CollectNextInRegion(reg);
      }else foreach(turn->RegionList)
      {
        ARegion *reg=*(ARegion**)iter;
        if(!reg->hasInfo) continue;
        CollectNextInRegion(reg);
      }
    }
    UpdateGridWidths(turn);
  }catch(...){}
  EnableEvents();
  logging=false;

  int summ;
  AnsiString str;
  AItem it;
  for(TElemMap::iterator i=data.begin();i!=data.end();i++)
  {
	indexs.push_back(i);
	TElem &el=i->second;
	it.type=i->first;
	el.item=i->first->name;
	el.type=i->first;
	el.sorttype=it.GetSortType();

	summ=0;
	str.SetLength(0);
	for(map<int,int>::iterator j=el.debet.begin();j!=el.debet.end();j++)
    {
      summ+=j->second;
      if(str.Length())
        str+="\n";
      str+=j->second;
      str+=names[j->first];
    }
    if(el.debet.size()>1)
      str=AnsiString(summ)+"=\n"+str;
    el.deb=summ;
    el.debstr=str;
    summ=0;
    str.SetLength(0);
    for(map<int,int>::iterator j=el.credit.begin();j!=el.credit.end();j++)
    {
      summ+=j->second;
      if(str.Length())
        str+="\n";
      str+=j->second;
      str+=names[j->first];
    }
    if(el.credit.size()>1)
      str=AnsiString(summ)+"=\n"+str;
    el.cred=summ;
    el.credstr=str;
  }
  sort(indexs.begin().operator ->(),indexs.end().operator ->(),TElemCompare);
}
//---------------------------------------------------------------------------
void __fastcall TBalanceForm::FormShow(TObject *Sender)
{
  MakeFacList();
  CollectData();
  MakeTable();
}
//---------------------------------------------------------------------------
void __fastcall TBalanceForm::GridDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
  AnsiString str;
  if(ARow==0)
  {
    switch(ACol)
    {
      case 0:str="Item";break;
      case 1:str="Begin Turn";break;
      case 2:str="Income";break;
      case 3:str="Expenses";break;
      case 4:str="Change";break;
      case 5:str="End Turn";break;
      case 6:str="Difference";break;
      case 7:str="Next Begin Turn";break;
    }
  }else{
    ARow--;
    if((unsigned)ARow<indexs.size()){
      TElemMap::iterator i=indexs[ARow];
      TElem &el=i->second;
      switch(ACol)
      {
        case 0:str=el.item;break;
        case 1:str=el.begturn;break;
        case 2:
          if(el.deb)
          {
            str=el.debstr;
          }
        break;
        case 3:
          if(el.cred)
          {
            str=el.credstr;
          }
        break;
        case 4:str=el.deb-el.cred;break;
        case 5:
        {
          str=el.endturn;
          int m=el.begturn+el.deb-el.cred;
          if(m!=el.endturn)
          {
            str+=" (";
            str+=m;
            str+=")";
          }
        }
        break;
        case 6:
        {
          int dif=el.nextbegturn-el.endturn;
          if(dif)
            str=dif;
        }
        break;
        case 7:str=el.nextbegturn;break;
      }
    }
  }
  if(!str.Length()) return;
  TRect r=Rect;
  InflateRect(&r,-3,-3);
  DrawText(Grid->Canvas->Handle,str.c_str(),str.Length(),&r,0);
}
//---------------------------------------------------------------------------
void TBalanceForm::MakeTable()
{
  if(!data.size())
  {
    Grid->RowCount=2;
    Grid->Invalidate();
    return;
  }
  Grid->RowCount=data.size()+1;
  int h=Grid->Canvas->TextHeight("W");
  int row=1;
  for(TElemMap::iterator *i=indexs.begin().operator ->();i<indexs.end().operator ->();i++)
  {
    TElem &el=(*i)->second;
    int m1=el.debet.size();
    if(m1>1) m1++;
    int m2=el.credit.size();
    if(m2>1) m2++;
    if(m1<m2) m1=m2;
    if(m1<1) m1=1;
    Grid->RowHeights[row++]=m1*h+6;
  }
  Grid->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TBalanceForm::cbCurRegionOnlyClick(TObject *Sender)
{
  curregonly=cbCurRegionOnly->Checked;
  CollectData();
  MakeTable();
}
//---------------------------------------------------------------------------
void TBalanceForm::MakeFacList()
{
  TStrings *list=cbFacDiap->Items;
  list->BeginUpdate();
  list->Clear();
  list->AddObject(" local",(TObject*)-3);
  foreach(curFactions)
  {
    AFaction *fac=*(AFaction**)iter;
    if(!fac->local) continue;
    list->AddObject(fac->FullName(),(TObject*)fac->number);
  }
  list->EndUpdate();
  int ind=list->IndexOfObject((TObject*)facdiap);
  cbFacDiap->ItemIndex=ind;
}
//---------------------------------------------------------------------------
void __fastcall TBalanceForm::cbFacDiapChange(TObject *Sender)
{
  int ind=cbFacDiap->ItemIndex;
  if(ind<0) return;
  ind=(int)cbFacDiap->Items->Objects[ind];
  if(facdiap==ind) return;
  facdiap=ind;
  CollectData();
  MakeTable();
}
//---------------------------------------------------------------------------
void __fastcall TBalanceForm::bnPrevTurnClick(TObject *Sender)
{
  AtlaForm->bnPrevTurn->Click();
  FormShow(0);
}
//---------------------------------------------------------------------------
void __fastcall TBalanceForm::bnNextTurnClick(TObject *Sender)
{
  AtlaForm->bnNextTurn->Click();
  FormShow(0);
}
//---------------------------------------------------------------------------

