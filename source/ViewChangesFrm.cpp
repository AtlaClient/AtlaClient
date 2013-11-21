//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <algorithm>
#include "util.h"
#include "changes.h"
#include "ViewChangesFrm.h"
#include "unit.h"
#include "region.h"
#include "items.h"
#include "faction.h"
#include "MainFrm.h"
#include "turn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TViewChangesForm *ViewChangesForm;

//---------------------------------------------------------------------------
__fastcall TViewChangesForm::TViewChangesForm(TComponent* Owner)
  : TForm(Owner)
{
  cbRegDiap->ItemIndex=2;
  facdiap=-4;
  UpdateMode();
}
//---------------------------------------------------------------------------
void __fastcall TViewChangesForm::FormShow(TObject *Sender)
{
  Caption=AnsiString("Changes for turn ")+Changes.turnnum;
  sgChanged->Height=ClientHeight/2;
  sgAdded->Width=ClientWidth/2;
  MakeFacList();
  ShowAll();
}
//---------------------------------------------------------------------------
void TViewChangesForm::ShowAll()
{
  ShowAdded();
  ShowDeleted();
  ShowChanged();
}
//---------------------------------------------------------------------------
extern int movingdx[NDIRS], movingdy[NDIRS];
void TViewChangesForm::FillRegionLocs(vector<unsigned> &locs)
{
  if(cbRegDiap->ItemIndex==2) return;
  if(!baseReg)
    baseReg=curRegion;
  switch(cbRegDiap->ItemIndex)
  {
    case 0:
      locs.push_back(baseReg->key);
    break;
    case 1:
      locs.push_back(baseReg->key);
      ALocation loc;
      for(int i=0;i<NDIRS;i++)
      {
        loc=*baseReg;
        loc.xloc+=movingdx[i];
        loc.yloc+=movingdy[i];
        locs.push_back(GetXYZKey(loc.xloc,loc.yloc,loc.zloc));
      }
    break;
  }
}
//---------------------------------------------------------------------------
void TViewChangesForm::ShowAdded()
{
  addeds.clear();
  int count=Changes.addcount;
  if(count)
  {
    int row=1;
    vector<unsigned> locs;
    FillRegionLocs(locs);
    for(int i=0,endi=Changes.elems.size();i<endi;i++)
    {
       TChanges::TElem &elem=Changes.elems[i];
       if(elem.type!=TChanges::TElem::ctAdded) continue;
       if(!inFacDiap(facdiap,elem.to->faction))
         continue;
       if(locs.size())
       {
		 unsigned *l = find(locs.begin().operator ->(),locs.end().operator ->(),elem.to->baseobj->basereg->key);
		 if(l==locs.end().operator ->())
           continue;
	   }
       if(sgAdded->RowCount<row+1)
         sgAdded->RowCount=row+1;
       sgAdded->Cells[0][row]=PrintUnit(elem.to);
       row++;
       addeds.push_back(&elem);
    }
    count=row-1;
  }
  sgAdded->Enabled=count;
  sgAdded->Cells[0][0]=AnsiString("Added ")+count;
  if(!count)
  {
    sgAdded->RowCount=2;
    sgAdded->Cells[0][1]="";
    return;
  }
  sgAdded->RowCount=count+1;
}
//---------------------------------------------------------------------------
void TViewChangesForm::ShowDeleted()
{
  deleteds.clear();
  int count=Changes.delcount;
  if(count)
  {
    int row=1;
    vector<unsigned> locs;
    FillRegionLocs(locs);
    for(int i=0,endi=Changes.elems.size();i<endi;i++)
    {
       TChanges::TElem &elem=Changes.elems[i];
       if(elem.type!=TChanges::TElem::ctDeleted) continue;
       if(!inFacDiap(facdiap,elem.from->faction))
         continue;
       if(locs.size())
       {
		 unsigned *l=find(locs.begin().operator ->(),locs.end().operator ->(),elem.from->baseobj->basereg->key);
         if(l==locs.end().operator ->())
           continue;
       }
       if(sgDeleted->RowCount<row+1)
         sgDeleted->RowCount=row+1;
       sgDeleted->Cells[0][row]=PrintUnit(elem.from);
       row++;
       deleteds.push_back(&elem);
    }
    count=row-1;
  }
  sgDeleted->Enabled=count;
  sgDeleted->Cells[0][0]=AnsiString("Deleted ")+count;
  if(!count)
  {
    sgDeleted->RowCount=2;
    sgDeleted->Cells[0][1]="";
    return;
  }
  sgDeleted->RowCount=count+1;
}
//---------------------------------------------------------------------------
void TViewChangesForm::ShowChanged()
{
  changeds.clear();
  int count=Changes.changecount;
  int row=1;
  if(count)
  {
    vector<unsigned> locs;
    FillRegionLocs(locs);
    for(int i=0,endi=Changes.elems.size();i<endi;i++)
    {
     TChanges::TElem &elem=Changes.elems[i];
     if(!(elem.type&mode)) continue;
     if(!inFacDiap(facdiap,elem.from->faction)
       &&!inFacDiap(facdiap,elem.to->faction))
       continue;
     if(locs.size())
     {
	   unsigned *l=find(locs.begin().operator ->(),locs.end().operator ->(),elem.from->baseobj->basereg->key);
	   if(l==locs.end().operator ->())
       {
		 l=find(locs.begin().operator ->(),locs.end().operator ->(),elem.to->baseobj->basereg->key);
         if(l==locs.end().operator ->())
           continue;
       }
     }
     if(sgChanged->RowCount<row+2)
       sgChanged->RowCount=row+2;
     AnsiString basestr,changesstr;
     PrintUnits(elem,basestr,changesstr);
     sgChanged->Cells[0][row++]=basestr;
     sgChanged->Cells[0][row++]=changesstr;
     changeds.push_back(&elem);
    }
  }
  count=(row/2);
  sgChanged->Enabled=count;
  sgChanged->Cells[0][0]=AnsiString("Changes ")+count;
  if(!count)
  {
    sgChanged->RowCount=2;
    sgChanged->Cells[0][1]="";
    return;
  }
  sgChanged->RowCount=row;
}
//---------------------------------------------------------------------------
AnsiString TViewChangesForm::PrintUnit(AUnit * un)
{
  AnsiString str=un->UnitHeader();
  AnsiString itemstr=un->items->Print();
  if(itemstr.Length()){
   str+=", ";
   str+=itemstr;
  }
  if(un->describe.Length()) str+=un->describe;
  str+=". In "+un->baseobj->basereg->FullName(false);
  return str;
}
//---------------------------------------------------------------------------
bool TViewChangesForm::PrintUnits(TChanges::TElem &elem, AnsiString &basestr
  , AnsiString & changesstr)
{
  AUnit *from=elem.from,*to=elem.to;
  basestr=PrintUnit(to);
  AnsiString str=" ";
  if(elem.type&TChanges::TElem::ctChangedItems)
  {
    str+=" Items: ";
    AItems *fromits=from->items;
    AItems *toits=to->items;
    int diff;
    bool first=true;
    foreach(fromits)
    {
      AItem *fit=*(AItem**)iter;
      AItem *tit=toits->GetItem(fit->type);
      if(!tit)
       diff=-fit->count;
      else diff=tit->count-fit->count;
      if(!diff) continue;
      if(first)
        first=false;
      else
        str+=", ";
      if(diff>0)  str+="+";
      str+=diff;
      str+=" "+fit->type->abr;
    }
    foreach(toits)
    {
      AItem *tit=*(AItem**)iter;
      AItem *fit=fromits->GetItem(tit->type);
      if(fit) continue;
      diff=tit->count;
      if(first)
        first=false;
      else
        str+=", ";
      str+="+";
      str+=diff;
      str+=" "+tit->type->abr;
    }
    str+=".";
  }
  if(elem.type&TChanges::TElem::ctChangedFaction)
  {
    str+=" Old faction: "+from->faction->FullName();
  }
  if(elem.type&TChanges::TElem::ctChangedLocation)
  {
    if(!Changes.CompareRegion(from,to))
      str+=" Old location: "+from->baseobj->basereg->FullName(false)+".";
    else
    {
      if(from->baseobj->num)
        str+=" Old object: "+from->baseobj->FullName();
      if(to->baseobj->num)
        str+=" New object: "+to->baseobj->FullName();
    }
  }
  if(elem.turnnum!=Changes.turnnum-1)
  {
    str+=AnsiString(" Turn ")+elem.turnnum;
  }
  changesstr=str;
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TViewChangesForm::sgAddedDblClick(TObject *Sender)
{
  int ind=sgAdded->Row-1;
  if(ind>=(int)addeds.size())
    return;
  TChanges::TElem &elem=*addeds[ind];
  int turnnum=Changes.turnnum;
  if(curTurn->num!=turnnum)
    AtlaForm->ChangeTurn(turnnum-curTurn->num,false);
  if(curTurn->num!=turnnum)
   return;
  AtlaForm->GotoUnit(elem.to);
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TViewChangesForm::sgDeletedDblClick(TObject *Sender)
{
  int ind=sgDeleted->Row-1;
  if(ind>=(int)deleteds.size())
    return;
  TChanges::TElem &elem=*deleteds[ind];
  int turnnum=Changes.turnnum-1;
  if(curTurn->num!=turnnum)
    AtlaForm->ChangeTurn(turnnum-curTurn->num,false);
  if(curTurn->num!=turnnum)
   return;
  AtlaForm->GotoUnit(elem.from);
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TViewChangesForm::sgChangedDblClick(TObject *Sender)
{
  int row=sgChanged->Row;
  int ind=(row-1)/2;
  row%=2;
  if(ind>=(int)changeds.size())
    return;
  TChanges::TElem &elem=*changeds[ind];
  int turnnum=Changes.turnnum;
  if(!row) turnnum=elem.turnnum;
  if(curTurn->num!=turnnum)
    AtlaForm->ChangeTurn(turnnum-curTurn->num,false);
  if(curTurn->num!=turnnum)
   return;
  AtlaForm->GotoUnit(row?elem.to:elem.from);
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void TViewChangesForm::UpdateMode()
{
  mode=0;
  if(miFaction->Checked)
    mode|=TChanges::TElem::ctChangedFaction;
  if(miItems->Checked)
    mode|=TChanges::TElem::ctChangedItems;
  if(miLocation->Checked)
    mode|=TChanges::TElem::ctChangedLocation;
}
//---------------------------------------------------------------------------
void __fastcall TViewChangesForm::miFactionClick(TObject *Sender)
{
  miFaction->Checked=!miFaction->Checked;
  UpdateMode();
  ShowChanged();
}
//---------------------------------------------------------------------------
void __fastcall TViewChangesForm::miItemsClick(TObject *Sender)
{
  miItems->Checked=!miItems->Checked;
  UpdateMode();
  ShowChanged();
}
//---------------------------------------------------------------------------
void __fastcall TViewChangesForm::miLocationClick(TObject *Sender)
{
  miLocation->Checked=!miLocation->Checked;
  UpdateMode();
  ShowChanged();
}
//---------------------------------------------------------------------------
void __fastcall TViewChangesForm::miAddedClick(TObject *Sender)
{
  int numturn=5;
  AnsiString str=numturn;
  if(!InputQuery("Verify added","Enter turns count",str))
    return;
  numturn=atoi(str.c_str());
  if(numturn<2)
    return;
  Changes.VerifyAdded(numturn);
  ShowAll();
}
//---------------------------------------------------------------------------
void __fastcall TViewChangesForm::cbRegDiapChange(TObject *Sender)
{
  ShowAll();
}
//---------------------------------------------------------------------------
void __fastcall TViewChangesForm::bnSetBaseRegClick(TObject *Sender)
{
  baseReg=curRegion;
  ShowAll();
}
//---------------------------------------------------------------------------
void TViewChangesForm::MakeFacList()
{
  TStrings *list=cbFacDiap->Items;
  list->BeginUpdate();
  list->Clear();
  list->AddObject("  nonlocal",(TObject*)-4);
  list->AddObject(" friendly",(TObject*)-5);
  list->AddObject(" nonfriendly",(TObject*)-7);
  list->AddObject(" unfriendly",(TObject*)-6);
  foreach(curFactions)
  {
    AFaction *fac=*(AFaction**)iter;
    list->AddObject(fac->FullName(),(TObject*)fac->number);
  }
  list->EndUpdate();
  int ind=list->IndexOfObject((TObject*)facdiap);
  cbFacDiap->ItemIndex=ind;
}
//---------------------------------------------------------------------------
void __fastcall TViewChangesForm::cbFacDiapChange(TObject *Sender)
{
  int ind=cbFacDiap->ItemIndex;
  if(ind<0) return;
  ind=(int)cbFacDiap->Items->Objects[ind];
  if(facdiap==ind) return;
  facdiap=ind;
  ShowAll();
}
//---------------------------------------------------------------------------
void __fastcall TViewChangesForm::cbFacDiapEnter(TObject *Sender)
{
  facnum_search=0;
}
//---------------------------------------------------------------------------
void __fastcall TViewChangesForm::cbFacDiapKeyPress(TObject *Sender,
      char &Key)
{
  if(Key>='0'&&Key<='9')
  {
    facnum_search*=10;
    facnum_search+=Key-'0';
    Key=0;
    int ind=cbFacDiap->Items->IndexOfObject((TObject*)facnum_search);
    if(ind<0)
      return;
    cbFacDiap->ItemIndex=ind;
    cbFacDiapChange(0);
  }else
  {
    facnum_search=0;
  }
}
//---------------------------------------------------------------------------

