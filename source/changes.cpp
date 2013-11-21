//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "util.h"
#include "changes.h"
#include "unit.h"
#include "turn.h"
#include "region.h"
#include "ViewChangesFrm.h"
#include "faction.h"
#include "items.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TChanges Changes;

//---------------------------------------------------------------------------
void TChanges::Clear()
{
  addcount=delcount=changecount=0;
  elems.clear();
}
//---------------------------------------------------------------------------
bool TChanges::Collect()
{
  ATurn *toturn=curTurn;
  turnnum=toturn->num;
  if(!turnnum) return false;
  curTurns->PreloadTurnIfNeed(turnnum-1,true);
  ATurn *fromturn=curTurns->Get(turnnum-1);
  if(!fromturn) return false;
  Clear();
  AUnits *from=new AUnits(0);
  from->autodelete=false;
  AUnits *to=new AUnits(0);
  to->autodelete=false;
//prepare
  foreach(fromturn->RegionList)
  {
    ARegion *reg=*(ARegion**)iter;
    foreach(reg)
    {
      AObject *obj=*(AObject**)iter;
      foreach(obj->units)
      {
        AUnit *un=*(AUnit**)iter;
        if(un->faction->local) continue;
        if(un->num<0) continue;
        from->Add(un);
      }
    }
  }
  foreach(toturn->RegionList)
  {
    ARegion *reg=*(ARegion**)iter;
    foreach(reg)
    {
      AObject *obj=*(AObject**)iter;
      foreach(obj->units)
      {
        AUnit *un=*(AUnit**)iter;
        if(un->faction->local) continue;
        if(un->num<0) continue;
        to->Add(un);
      }
    }
  }
//ctDeleted
  for(int i=0;i<from->count;i++)
  {
    AUnit *un=from->Get(i);
    if(to->GetUnit(un->num)) continue;
    from->DeleteByIndex(i);
    i--;
    TElem elem;
    elem.type=TElem::ctDeleted;
    elem.from=un;
    elem.turnnum=turnnum-1;
    elems.push_back(elem);
    delcount++;
  }
//ctAdded
  for(int i=0;i<to->count;i++)
  {
    AUnit *un=to->Get(i);
    if(from->GetUnit(un->num)) continue;
    to->DeleteByIndex(i);
    i--;
    TElem elem;
    elem.type=TElem::ctAdded;
    elem.to=un;
    elems.push_back(elem);
    addcount++;
  }
  for(int i=0;i<to->count;i++)
  {
    AUnit *toun=to->Get(i);
    AUnit *fromun=from->GetUnit(toun->num);
    int type=Compare(fromun,toun);
    if(!type) continue;
    TElem elem;
    elem.type=type;
    elem.from=fromun;
    elem.turnnum=turnnum-1;
    elem.to=toun;
    elems.push_back(elem);
    changecount++;
  }
  delete from;
  delete to;
  return true;
}
//---------------------------------------------------------------------------
bool TChanges::Edit()
{
  ViewChangesForm->ShowModal();
  return true;
}
//---------------------------------------------------------------------------
bool TChanges::CompareFaction(AUnit * from, AUnit * to)
{
  return from->faction->number==to->faction->number;
}
//---------------------------------------------------------------------------
bool TChanges::CompareItems(AUnit * from, AUnit * to)
{
  AItems *fromits=from->items;
  AItems *toits=to->items;
  if(fromits->count!=toits->count) return false;
  foreach(fromits)
  {
    AItem *fit=*(AItem**)iter;
    AItem *tit=toits->GetItem(fit->type);
    if(!tit) return false;
    if(fit->count!=tit->count) return false;
  }
  foreach(toits)
  {
    AItem *tit=*(AItem**)iter;
    AItem *fit=fromits->GetItem(tit->type);
    if(!fit) return false;
  }
  return true;
}
//---------------------------------------------------------------------------
bool TChanges::CompareLocation(AUnit * from, AUnit * to)
{
  if(from->baseobj->num!=to->baseobj->num)
    return false;
  return CompareRegion(from,to);
}
//---------------------------------------------------------------------------
bool TChanges::CompareRegion(AUnit * from, AUnit * to)
{
  ARegion *fromreg=from->baseobj->basereg;
  ARegion *toreg=to->baseobj->basereg;
  if(fromreg->xloc!=toreg->xloc)
    return false;
  if(fromreg->yloc!=toreg->yloc)
    return false;
  if(fromreg->zloc!=toreg->zloc)
    return false;
  return true;
}
//---------------------------------------------------------------------------
int TChanges::Compare(AUnit * from, AUnit * to)
{
  int type=0;
  if(!CompareFaction(from,to))
    type|=TElem::ctChangedFaction;
  if(!CompareItems(from,to))
    type|=TElem::ctChangedItems;
  if(!CompareLocation(from,to))
    type|=TElem::ctChangedLocation;
  return type;
}
//---------------------------------------------------------------------------
bool TChanges::VerifyAdded(int turncount)
{
  if(!elems.size()) return false;
  if(!turnnum) return false;
  for(int endt=turnnum,t=endt-turncount;t<endt;t++)
    curTurns->PreloadTurnIfNeed(t,true);

  AUnits *to=new AUnits(0);
  to->autodelete=false;
  for(int i=elems.size()-1;i>=0;i--)
  {
     TElem *elem=elems.begin().operator ->()+i;
     if(elem->type!=TElem::ctAdded) continue;
     to->Add(elem->to);
     elems.erase(elem);
  }
  for(int t=turnnum-2,endt=t-turncount;t>=endt;t--)
  {
    ATurn *turn=curTurns->Get(t);
    if(!turn) continue;
    ARegionList *rl=turn->RegionList;
    for(int i=to->count-1;i>=0;i--)
    {
      AUnit *toun=to->Get(i);
      AUnit *fromun=rl->GetUnit(toun->num);
      if(!fromun) continue;
      to->DeleteByIndex(i);
      int type=Compare(fromun,toun);
      if(!type) continue;
      TElem elem;
      elem.type=type;
      elem.from=fromun;
      elem.turnnum=turn->num;
      elem.to=toun;
      elems.push_back(elem);
      changecount++;
    }
  }
//ctAdded
  addcount=0;
  for(int i=0;i<to->count;i++)
  {
    AUnit *un=to->Get(i);
    to->DeleteByIndex(i);
    i--;
    TElem elem;
    elem.type=TElem::ctAdded;
    elem.to=un;
    elems.push_back(elem);
    addcount++;
  }
  delete to;
  return true;
}
//---------------------------------------------------------------------------

