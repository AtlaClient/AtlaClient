//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <algorithm>
#include "util.h"
#include "eapfile.h"
#include "faction.h"
#include "viewitems.h"
#include "orders.h"
#include "items.h"
#include "turn.h"
#include "region.h"
#include "skills.h"
#include "unit.h"
#include "math.h"

#include "reports.h"
#include "acplugins.h"
#include "plugdoc\plugins.h"
#include "gameparams.h"
#include "exprlang.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
AUnit *curUnit;
AUnit::AUnit(ATurn *t)
 :num(-1),faction(0),turn(t),
//  reveal(REVEAL_NONE),flags(0),info(INFO_NONE),guard(GUARD_NONE)
  globalflags(0),
  weight(0), walkweight(0), ridweight(0), flyweight(0),swimweight(0),
  endflags(0),endguard(0),endreveal(0),endfaction(0),
  baseobj(0),endobj(0),needed(0)
{
 if(hasskills||reserved||hascanstudy){
  globalflags=0;
 }
 orders=new AOrders(this);
 items=new AItems;
 enditems=new AItems;
 skills=new ASkills;
 endskills=new ASkills;
 canstudy=new TStringList;
}
__fastcall AUnit::~AUnit()
{
 if(curUnit==this)
  curUnit=0;
 delete orders;
 delete items;
 delete enditems;
 delete skills;
 delete endskills;
 delete canstudy;
}
void AUnit::Read(TFile & in)
{
 in>>name;
 in>>num;
 int facnum;
 in>>facnum;
 SetFaction(facnum);
 in>>globalflags;
 in>>describe;
 orders->Read(in);
 items->Read(in);
 enditems->Clear();
 if(hasskills)
  skills->Read(in);
 if(hascombatspell)
   if(BaseVer>=32)
     in>>combatspell;
   else
     ReadStringAsInt(in,combatspell);
 if(hascanstudy){
   if(BaseVer>=32)
   {
     AnsiString str;
     in>>str;
     canstudy->Text=str;
   }else
   {
    int kol;
    in>>kol;
    AnsiString str;
    canstudy->Clear();
    canstudy->Capacity=kol;
    for(int i=0;i<kol;i++){
     ReadStringAsInt(in,str);
     canstudy->Add(str);
    }
   }
 }
 if(hasparams)
   in>>params;
}
void AUnit::Write(TFile &out)
{
 out<<name;
 out<<num;
 int facnum=0;
 if(faction) facnum=faction->number;
 out<<facnum;
 reserved=0;
// hascanstudy=0; ////////temp
 hascombatspell=hascombatspell&&combatspell.Length();
 hasparams=params.Length()?1:0;
 out<<globalflags;
/* int dlen=describe.Length();
 if(dlen){
  if(describe[dlen]=='.')describe.SetLength(dlen-1);
 }*/
 out<<describe;
 orders->Write(out);
 items->Write(out);
 if(hasskills)
  skills->Write(out);
 if(hascombatspell)
  out<<combatspell;
 if(hascanstudy){
  out<<canstudy->Text;
 }
 if(hasparams)
   out<<params;
}
void AUnit::CreateNew(AUnit * prvunit)
{
//warning: no copy orders
 name=prvunit->name;
 info=prvunit->info;
 num=prvunit->num;
 faction=prvunit->faction;
 guard=prvunit->guard;
 reveal=prvunit->reveal;
 flags=prvunit->flags;

 describe=prvunit->describe;

// orders->Update(newun->orders);

 items->Clear();
 items->Update(prvunit->items);
 enditems->Clear();
 hasskills=prvunit->hasskills;
 skills->Update(prvunit->skills);
 hascombatspell=prvunit->hascombatspell;
 combatspell=prvunit->combatspell;

 hascanstudy=prvunit->hascanstudy;
 if(hascanstudy){
  canstudy->Assign(prvunit->canstudy);
 }
}
void AUnit::Update(AUnit *newun)
{
//warning: move orders
 if(info>newun->info)return;
 name=newun->name;
 info=newun->info;
 num=newun->num;
 if(!faction||newun->faction->number)
  faction=newun->faction;
 guard=newun->guard;
 reveal=newun->reveal;
 flags=newun->flags;

 describe=newun->describe;

 orders->Update(newun->orders);

 items->Clear();
 items->Update(newun->items);
 enditems->Clear();
 hasskills=newun->hasskills;
 skills->Update(newun->skills);
 hascombatspell=newun->hascombatspell;
 combatspell=newun->combatspell;

 hascanstudy=newun->hascanstudy;
 if(hascanstudy){
  canstudy->Assign(newun->canstudy);
 }
 params=newun->params;
}
void AUnit::UpdateFromPrev(AUnit * prvun)
{
 if(IsLocal()) return;
 if(prvun->orders->count&&!orders->count){
  foreach(prvun->orders){
   AOrder *prvord=*(AOrder**) iter;
   if(!prvord->repeating) continue;
   AnsiString str=prvord->WriteTemplate();
   AOrder *ord=ParseOrder(str);
   if(!ord) continue;
   orders->Add(ord);
  }
 }
 if(prvun->skills->count/*&&!skills->count*/){
  skills->UpdateFromPrev(prvun->skills);
  hasskills=true;
 }
}

bool AUnit::GetFlag(int flag)
{
 return flags&flag;
}
void AUnit::SetFlag(int flag, bool value)
{
 if(value){
  flags|=flag;
 }else{
  flags&=~flag;
 }
}
bool AUnit::GetEndFlag(int flag)
{
 return endflags&flag;
}
void AUnit::SetEndFlag(int flag, bool value)
{
 if(value){
  endflags|=flag;
 }else{
  endflags&=~flag;
 }
}
AnsiString AUnit::FullName(bool fortemplate)
{
 AnsiString s;
 if(num<0){
  AnsiString ld=GetLocalDescr();
  if(ld.Length()){
   s+=ld+" (";
  }else s+=" ";
  if(num<-FACTIONMULT)
  {
    s+="faction ";
    s+=-num/FACTIONMULT;
    s+=" new ";
    s+=-num%FACTIONMULT;
  }else
  {
    s+="new ";
    s+=-num;
  }
  if(ld.Length()){
   s+=")";
  }
 }else{
  if(fortemplate){
   if(name.Length()){
    s+=name;
   }else{
    s+="Unit";
   }
  }else{
   AnsiString ld=GetLocalDescr();
   if(ld.Length()&&optShowLDescFirst)s+=ld;
   else if(name.Length())
    s+=name;
   else if(ld.Length())
    s+=ld;
   else
    s+="Unit";
  }
  s+=" (";
  s+=num;
  s+=")";
 }
 return s;
}
AnsiString AUnit::FullName2()
{
 AnsiString s;
 if(num<0){
  AnsiString ld=GetLocalDescr();
  if(ld.Length()){
   s+=ld+" (";
  }else s+=" ";
  s+="new ";
  s+=-num;
  if(ld.Length()){
   s+=")";
  }
 }else{
  AnsiString ld=GetLocalDescr();
  if(ld.Length()){
   s+=ld;
  }
  if(name.Length()){
   if(ld.Length()) s+=" ";
   s+=name;
  }
  s+=" (";
  s+=num;
  s+=")";
 }
 return s;
}
AnsiString AUnit::GetFlagsStr1()
{
 AnsiString s;
 if(guard==GUARD_GUARD){
  s+=KeyPhrases->Get(keyUnitOnGuard);
 }
 return s;
}
AnsiString AUnit::GetFlagsStr2()
{
 AnsiString s;
 if(guard==GUARD_AVOID){
  s+=KeyPhrases->Get(keyUnitAvoid);
 }
 if(GetFlag(FLAG_BEHIND)){
  s+=KeyPhrases->Get(keyUnitBehind);
 }
 return s;
}
AnsiString AUnit::GetFlagsStr3()
{
 AnsiString s;
 if(reveal==REVEAL_UNIT){
  s+=KeyPhrases->Get(keyUnitRevUnit);
 }
 if(reveal==REVEAL_FACTION){
  s+=KeyPhrases->Get(keyUnitRevFaction);
 }
 if(GetFlag(FLAG_SHARING)){
  s+=KeyPhrases->Get(keyUnitSharing);
 }
 if(GetFlag(FLAG_HOLDING)){
  s+=KeyPhrases->Get(keyUnitHold);
 }
 if(GetFlag(FLAG_AUTOTAX)){
  s+=KeyPhrases->Get(keyUnitTax);
 }
 if(GetFlag(FLAG_NOAID)){
  s+=KeyPhrases->Get(keyUnitNoAid);
 }
 if(GetFlag(FLAG_CONSUMING_UNIT)){
  s+=KeyPhrases->Get(keyUnitConsumUnit);
 }
 if(GetFlag(FLAG_CONSUMING_FACTION)){
  s+=KeyPhrases->Get(keyUnitConsumFaction);
 }
 if(GetFlag(FLAG_NOCROSS_WATER)){
  s+=KeyPhrases->Get(keyUnitNoCross);
 }
 if(GetFlag(FLAG_NOSPOILS)){
  if(optServSpoils)
   s+=KeyPhrases->Get(keyUnitSpoilsNone);
  else
   s+=KeyPhrases->Get(keyUnitNoSpoils);
 }
 if(GetFlag(FLAG_WALKSPOILS)){
  s+=KeyPhrases->Get(keyUnitSpoilsWalk);
 }
 if(GetFlag(FLAG_RIDESPOILS)){
  s+=KeyPhrases->Get(keyUnitSpoilsRide);
 }
 if(GetFlag(FLAG_FLYSPOILS)){
  s+=KeyPhrases->Get(keyUnitSpoilsFly);
 }
 return s;
}
AnsiString AUnit::GetEndFlagsStr(bool fortree)
{
 AnsiString s;
 if(fortree)
 {
   if(endguard==GUARD_GUARD){
    s+=KeyPhrases->Get(keyUnitOnGuard);
    if(guard!=GUARD_GUARD)
      s+="+";
    else
    {
      foreach(orders)
      {
        AOrder *_ord=*(AOrder**)iter;
        if(_ord->commented) continue;
        if(_ord->type!=O_GUARD) continue;
        s+="-+";
        break;
      }
    }
   }else if(guard==GUARD_GUARD&&!faction->local)
   {
    s+=KeyPhrases->Get(keyUnitOnGuard);
    s+="-";
   }
 }
 else
 if(endguard==GUARD_GUARD){
  s+=KeyPhrases->Get(keyUnitOnGuard);
 }
 if(fortree)
 {
   if(faction->number){
    s+=", ";
    s+=faction->FullName();
   }
 }else
 if(endfaction->number){
  s+=", ";
  s+=endfaction->FullName();
 }
 if(endguard==GUARD_AVOID){
  s+=KeyPhrases->Get(keyUnitAvoid);
 }
 if(GetEndFlag(FLAG_BEHIND)){
  s+=KeyPhrases->Get(keyUnitBehind);
 }
 if(endreveal==REVEAL_UNIT){
  s+=KeyPhrases->Get(keyUnitRevUnit);
 }
 if(endreveal==REVEAL_FACTION){
  s+=KeyPhrases->Get(keyUnitRevFaction);
 }
 if(GetFlag(FLAG_SHARING)){
  s+=KeyPhrases->Get(keyUnitSharing);
 }
 if(GetEndFlag(FLAG_HOLDING)){
  s+=KeyPhrases->Get(keyUnitHold);
 }
 if(GetEndFlag(FLAG_AUTOTAX)){
  s+=KeyPhrases->Get(keyUnitTax);
 }
 if(GetEndFlag(FLAG_NOAID)){
  s+=KeyPhrases->Get(keyUnitNoAid);
 }
 if(GetEndFlag(FLAG_CONSUMING_UNIT)){
  s+=KeyPhrases->Get(keyUnitConsumUnit);
 }
 if(GetEndFlag(FLAG_CONSUMING_FACTION)){
  s+=KeyPhrases->Get(keyUnitConsumFaction);
 }
 if(GetEndFlag(FLAG_NOCROSS_WATER)){
  s+=KeyPhrases->Get(keyUnitNoCross);
 }
 if(GetEndFlag(FLAG_NOSPOILS)){
  if(optServSpoils)
   s+=KeyPhrases->Get(keyUnitSpoilsNone);
  else
   s+=KeyPhrases->Get(keyUnitNoSpoils);
 }
 if(GetEndFlag(FLAG_WALKSPOILS)){
  s+=KeyPhrases->Get(keyUnitSpoilsWalk);
 }
 if(GetEndFlag(FLAG_RIDESPOILS)){
  s+=KeyPhrases->Get(keyUnitSpoilsRide);
 }
 if(GetEndFlag(FLAG_FLYSPOILS)){
  s+=KeyPhrases->Get(keyUnitSpoilsFly);
 }
 return s;
}
AnsiString AUnit::UnitHeader(bool fortemplate)
{
 AnsiString s;
 s+=FullName(fortemplate);
 s+=GetFlagsStr1();
 if(faction->number){
  s+=", ";
  s+=faction->FullName();
 }
 s+=GetFlagsStr2();
 s+=GetFlagsStr3();
 return s;
}

AnsiString AUnit::Print()
{
  AnsiString s;
  if(info==INFO_LOCAL)s="* "; else
  if(info==INFO_FACTION) s="- "; else s="--";
  s += UnitHeader();
  AnsiString itemstr=items->Print();
  if(itemstr.Length())
  {
	s += ", ";
	s += itemstr;
  }
  if(describe.Length())s+=/*"; "+*/describe;
  s += ".";
  return s;
}

bool AUnit::WriteReport(TStream *strm,int obs,int truesight,bool detfac,bool autosee)
{
 if(num<0) return true;
 AnsiString s;
 int stealth=GetBegStealth();
 int tmpreveal;
 if(faction->local){
  tmpreveal=reveal;
 }else tmpreveal=REVEAL_FACTION;
 if(obs==-1){
  obs=2;
 }else{
  if(obs<stealth) {
            /* The unit cannot be seen */
   if(tmpreveal==REVEAL_FACTION){
    obs=1;
   }else{
    if(guard==GUARD_GUARD||tmpreveal==REVEAL_UNIT||autosee){
     obs=0;
    }else{
     return true;
    }
   }
  }else{
   if(obs==stealth){
    /* Can see unit, but not Faction */
    if(tmpreveal==REVEAL_FACTION){
     obs=1;
    }else{
     obs=0;
    }
   }else{
    /* Can see unit and Faction */
    obs = 1;
   }
  }
 }
 /* Setup True Sight */
 if(obs==2){
  truesight=1;
 }else{
  if(GetBegSkill(IlluSkill)>truesight){
   truesight=0;
  }else{
   truesight=1;
  }
 }
 if(detfac&&obs!=2) obs=1;

 if(obs==2) s="* ";
 else s="- ";
 s+=FullName(true);
 s+=GetFlagsStr1();
 if(faction->number&&!faction->local){
  s+=", ";
  s+=faction->FullName();
 }
 if(obs>0){
  if(faction->number&&faction->local){
   s+=", ";
   s+=faction->FullName();
  }
  s+=GetFlagsStr2();
 }
 if(obs==2){
  s+=GetFlagsStr3();
 }
 s+=items->WriteReport(obs,truesight,false);
 if(obs==2) {
  s+=KeyPhrases->Get(keyUnitSkills);
  s+=skills->Print();
 }
 if(obs==2) {
  if(params.Length())
  {
    s+=". ";
    s+=params;
  }
  if(combatspell.Length()){
   s+=KeyPhrases->Get(keyUnitCombatSpell);
   s+=SkillTypes->AbrToName(combatspell)+" [";
   s+=combatspell+"]";
  }
  if(canstudy->Count){
   s+=KeyPhrases->Get(keyUnitCanStudy);
   for(int i=0,endi=canstudy->Count;i<endi;i++){
    AnsiString skill=canstudy->Strings[i];
    if(i) s+=", ";
    s+=SkillTypes->AbrToName(skill)+" [";
    s+=skill+"]";
   }
  }
//  temp += MageReport();
 }



 if(describe.Length())
  s+=describe;
 s+=".";
 SaveRepString(strm,s);
 return true;
}
AnsiString AUnit::BattleReport(int obs){
 AnsiString s;
 s=FullName(true);
 if(faction&&faction->number){
   s+=", ";
   s+=faction->FullName();
 }
 if(GetFlag(FLAG_BEHIND)){
  s+=KeyPhrases->Get(keyUnitBehind);
 }
 s+=items->BattleReport(false);
 int lvl;
 lvl=GetBegRealSkill(TactSkill);
 if(lvl){
  s+=", ";
  s+=SkillTypes->AbrToName(TactSkill);
  s+=" ";
  s+=lvl;
 }
 lvl=GetBegRealSkill(CombSkill);
 if(lvl){
  s+=", ";
  s+=SkillTypes->AbrToName(CombSkill);
  s+=" ";
  s+=lvl;
 }
 lvl=GetBegRealSkill(LbowSkill);
 if(lvl){
  s+=", ";
  s+=SkillTypes->AbrToName(LbowSkill);
  s+=" ";
  s+=lvl;
 }
 lvl=GetBegRealSkill(XbowSkill);
 if(lvl){
  s+=", ";
  s+=SkillTypes->AbrToName(XbowSkill);
  s+=" ";
  s+=lvl;
 }
 lvl=GetBegRealSkill(RidiSkill);
 if(lvl){
  s+=", ";
  s+=SkillTypes->AbrToName(RidiSkill);
  s+=" ";
  s+=lvl;
 }
 lvl=GetBegRealSkill(combatspell);
 if(lvl){
  s+=", ";
  s+=combatspell;
  s+=" ";
  s+=lvl;
 }
 if(describe.Length())
  s+=describe;
 s+="."; 
 return s;
}

AnsiString AUnit::PrintForTree()
{
 AnsiString s;
 s=FullName(false);
 s+=GetEndFlagsStr(true);
 return s;
}
void AUnit::MakeView(VListItems * vlis)
{
 AnsiString s;
 VListItem *vli;
 if(info==INFO_LOCAL)s="* ";
 else if(info==INFO_FACTION) s="- ";
 else s="--";
 s+=UnitHeader(true);
 vli=new VListItem(vlis);
 vli->type=litUnitHeader;
 vli->value=num;
 vli->text=s;

 vli=new VListItem(vlis);
 vli->type=litUnitInfo;
// vli->value=num;
 vli->text=items->Print();

 if(hasskills){
  vli=new VListItem(vlis);
  vli->type=litUnitInfo;
// vli->value=num;
  vli->text=skills->Print(1+4+8);
 }
 if(params.Length()){
  vli=new VListItem(vlis);
  vli->type=litUnitInfo;
// vli->value=num;
  vli->text=params;
 }
 if(hascombatspell){
  s="Combat spell: ";
  ASkill *sk=skills->GetSkill(combatspell);
  if(sk)s+=sk->Print(1+4);
  vli=new VListItem(vlis);
  vli->type=litUnitInfo;
  vli->text=s;
 }
 if(hascanstudy){
  s="Can Study: ";
  for(int i=0;i<canstudy->Count;i++){
   ASkillType *sk=SkillTypes->Get(canstudy->Strings[i],2);
   if(sk){
    s+=sk->name;
    s+=" [";
    s+=sk->abr;
    s+="]";  
   }
   s+=", ";
  }
  s.SetLength(s.Length()-2);
  vli=new VListItem(vlis);
  vli->type=litUnitInfo;
  vli->text=s;
 }

 vli=new VListItem(vlis);
 vli->type=litUnitInfo;
// vli->value=num;
 vli->text=describe;

  RunOrders();
  int i = 0;

  foreach(orders)
  {
    AOrder *ord=*(AOrder**) iter;
    vli=new VListItem(vlis);
    vli->type=litUnitOrder;
    vli->value = i;
    s = ord->Print();
    if(ord->type!=O_MESSAGE) s=" "+s;
    vli->text=s;
    if(ord->type == O_TURN)
    {
      AOrderTurn *ordturn = ((AOrderTurn*)ord);
      foreach(ordturn->torders)
      {
        AOrder *ord1 = *(AOrder**) iter;
        vli = new VListItem(vlis);
        vli->type=litUnitOrder;
        vli->value = i;
        s = ord1->Print();
        s = "   " + s;
        vli->text = s;
      }
      vli = new VListItem(vlis);
      vli->type = litUnitOrder;
      vli->value = i;
      vli->text = " endturn";
    }
    i++;
  }

  vli = new VListItem(vlis);
  vli->type = litUnitInfo;

  vli = new VListItem(vlis);
  vli->type = litUnitInfo;
  vli->text = "At end of turn:";

  s = FullName();
  s+= GetEndFlagsStr();
  vli = new VListItem(vlis);
  vli->type = litUnitInfo;
  vli->text = s;

  if(endobj!=baseobj||endobj->endreg!=baseobj->basereg)
  {
    s = "Located in ";
    if(endobj->num)
    {
      s += endobj->FullName();
      s += " in ";
    }
    s += endobj->endreg->FullName(false);

    vli = new VListItem(vlis);
    vli->type = litUnitInfo;
    vli->text = s;
  }

  vli = new VListItem(vlis);
  vli->type = litUnitInfo;
  vli->text = enditems->Print();

  if(endskills->count)
  {
    vli = new VListItem(vlis);
    vli->type = litUnitInfo;
    vli->text = endskills->Print(1+4+8);
  }
  s  = "Weight: ";
  s += weight;
  s += "/";
  s += walkweight;
  if(walkweight < weight) s += "-";
  s += "/";
  s += ridweight;
  if(ridweight < weight) s += "-";
  s += "/";
  s += flyweight;
  if(flyweight < weight) s += "-";
  if(optServNoCross||swimweight)
  {
    s += "/";
    s += swimweight;
    if(swimweight < weight) s += "-";
  }

  vli = new VListItem(vlis);
  vli->type = litUnitInfo;
  vli->text = s;

  vli = new VListItem(vlis);
  vli->type = litUnitInfo;

  AFaction *fac = faction;
  if(!fac) return;
  if(!fac->local) return;
  TStringList *slist = new TStringList;
  FillEventsList(slist);
  if(slist->Count)
  {
    vli = new VListItem(vlis);
    vli->type = litUnitEvents;
    vli->value = -fac->number;
    vli->text = "Events:";

    for(int i=0;i<slist->Count;i++)
    {
      vli = new VListItem(vlis);
      vli->type = litUnitEvent;
      vli->value = -fac->number;
      vli->text = slist->Strings[i];
    }
  }
  delete slist;

}

void AUnit::FillEventsList(TStrings *slist)
{
 AFactions *facs=faction->factions;
 AnsiString findstr;
 findstr.printf("(%d)",num);
 foreach(facs)
 {
   AFaction *fac=*(AFaction**)iter;
   if(!fac->local) continue;
   for(int i=0;i<fac->events->Count;i++){
     AnsiString str=fac->events->Strings[i];
     int pos=str.Pos(findstr);
     if(!pos) continue;
     slist->Add(str);
   }
 }  
 if(baseobj->num>=100&&baseobj->units->IndexOf(this)==0)
 {
   findstr.printf("[%d] ",baseobj->num);
   foreach(facs)
   {
     AFaction *fac=*(AFaction**)iter;
     if(!fac->local) continue;
     for(int i=0;i<fac->events->Count;i++){
       AnsiString str=fac->events->Strings[i];
       int pos=str.Pos(findstr);
       if(!pos) continue;
       slist->Add(str);
     }
   }
 }
}
void AUnit::MakeTree(TTreeNode * parnode, VTreeNodes * vtns)
{
 AnsiString str=PrintForTree();
 VTreeNode *vtn;
 vtn=new VTreeNode(vtns);
 vtn->type=tntUnit;
 vtn->value=num;
 vtn->data=this;
 TTreeNode *node=parnode->Owner->AddChildObject(parnode,str,vtn);
 vtn->node = node;
 TV_ITEM tv;
 tv.hItem = node->ItemId;
 tv.mask = TVIF_STATE;
 tv.stateMask = TVIS_BOLD;
 /*
 if(IsLocal()){
   tv.state=0xffffffff;
 }else{
   tv.state = 0;
 }
 */
 tv.state = 0;
 TreeView_SetItem(node->Handle,&tv);

}
bool AUnit::IsLocal()
{
 return info==INFO_LOCAL;
}

void AUnit::RunOrders()
{
// enditems->Update(items);

/* foreach(orders, AOrder *ord)
  ord->Run(this);
 }*/
 baseobj->basereg->RunOrders(false);
// enditems->DeleteEmpty();
}

AUnits::AUnits(AObject *obj)
 :TList(),object(obj),autodelete(true)
{
}
__fastcall AUnits::~AUnits()
{
 Clear();
}
void __fastcall AUnits::Clear()
{
 if(autodelete){
  int i=Count-1;
  if(i<0) return;
  AUnit **iter=(AUnit**)List+i;
  for(;i>=0;i--,iter--){
   delete *iter;
 /* for(int i=Count-1;i>=0;i--){
   AUnit * u=Get(i);*/
//   TList::Delete(i);
 //  delete u;
  }
 }
 TList::Clear();
}
void AUnits::Read(TFile& in)
{
 Clear();
 int kol;
 in>>kol;
 Capacity=kol;
 for(int i=0;i<kol;i++){
  AUnit *un=new AUnit(object->turn);
  un->Read(in);
  Add(un);
 }
}
void AUnits::Write(TFile & out)
{
 int kol=count;
 out<<kol;
 foreach(this){
  AUnit *un=*(AUnit**)iter;
  un->Write(out);
 }
}
AUnit* AUnits::Get(int index)
{
 return (AUnit*)Items[index];
}
AUnit* AUnits::GetUnit(int num)
{
 foreach(this){
  AUnit *unit=*(AUnit**) iter;
  if(unit->num==num)return unit;
 }
 return 0;
}
bool AUnits::Delete(int num)
{
// for(int i=Count-1;i>=0;i--){
 int i=Count-1;
 AObject **iter=(AObject**)List+i;
 for(;i>=0;i--,iter--){
  AUnit * u=*(AUnit**)iter; //Get(i);
  if(u->num==num){
   TList::Delete(i);
   if(autodelete) delete u;
   return true;
  }
 }
 return false;
}
bool AUnits::DeleteByIndex(int ind)
{
  AUnit *u=Get(ind);
  TList::Delete(ind);
  if(autodelete) delete u;
  return true;
}
void AUnits::Add(AUnit* un)
{
 if(GetUnit(un->num))
  throw Exception("Units::Add: Already Exist");
 if(autodelete)un->baseobj=object;
 TList::Add(un);
}
void AUnits::CreateNew(AUnits* prvunits)
{
 Clear();
//do nothing
}
void AUnits::Update(AUnits *newunits)
{
 AUnit *un;
 foreach(newunits){
  AUnit *newun=*(AUnit**) iter;
  un=GetUnit(newun->num);
  if(!un){
   un=new AUnit(object->turn);
   un->Update(newun);
   Add(un);
  }else{
   un->Update(newun);
  }
 }
}
AnsiString AUnit::GetLocalDescr()
{
 AOrderLocDescr *ord=dynamic_cast<AOrderLocDescr *>(orders->Find(O_LOCALDESCR));
 if(!ord)return "";
 return ord->text;
}
void AUnit::SetLocalDescr(AnsiString descr)
{
 AOrderLocDescr *ord=dynamic_cast<AOrderLocDescr *>(orders->Find(O_LOCALDESCR));
 if(ord)
 {
   ord->text=descr;
   return;
 }
 ord=new AOrderLocDescr;
 ord->text=descr;
 orders->Add(ord);
}
AOrder * AUnit::GetOrder(int num)
{
 if(num<0||num>=orders->count)return 0;
 return orders->Get(num);
}


bool AUnit::SaveTemplate(TFileStream * file)
{
 AnsiString s;
 if(num<0){
  s="form ";
  s+=-num;
  s+="\n";
 }else{
  s="unit ";
  s+=num;
  s+="\n";
 }
 file->Write(s.c_str(),s.Length());
 orders->Compact();
 foreach(orders){
  AOrder *ord=*(AOrder**) iter;
  s=ord->WriteTemplate();
  if(!s.Length())continue;
  s+="\n";
  file->Write(s.c_str(),s.Length());
 }
 if(num<0){
  s="end\n";
  file->Write(s.c_str(),s.Length());
 }

 s="\n";
 file->Write(s.c_str(),s.Length());

 return true;
}


void AUnit::FormNew(AUnit *base)
{
 name="";
 faction=base->faction;
 info=INFO_LOCAL;
 orders->Clear();
 items->Clear();
 guard=GUARD_NONE;
 reveal=REVEAL_NONE;
 flags=0;
}
int AUnit::GetMen(AnsiString type)
{
 return enditems->GetNum(type);
}
int AUnit::GetMen()
{
 int n=0;
 foreach(enditems){
  AItem *i=*(AItem**) iter;
  AItemType *itype=i->type;
  if(!itype)continue;
  if(itype->type&AItemType::IT_MAN)n+=i->count;
 }
 return n;
}
int AUnit::GetBegMen()
{
 int n=0;
 foreach(items){
  AItem *i=*(AItem**) iter;
  AItemType *itype=i->type;
  if(!itype)continue;
  if(itype->type&AItemType::IT_MAN)n+=i->count;
 }
 return n;
}


int AUnit::GetAttitude(ARegion * reg, AUnit * u)
{
 if(faction==u->faction)return A_ALLY;
 if(!faction->number)
  if(u->faction->number)return A_NEUTRAL;
//  else return A_ALLY;
 if(!u->faction->number) return faction->defattitude;
 int att=faction->GetAttitude(u->faction->number);
 if(!faction->local)return att;
 if (att>=A_FRIENDLY && att >= faction->defattitude) return att;
 if (CanSee(reg,u) == 2) {
  return att;
 }else{
  return faction->defattitude;
 }
}
bool AUnit::IsAlive()
{
 foreach(enditems){
  AItem *it=*(AItem**) iter;
  if(!it->count) continue;
//  AItemType *itype=it->type;
  if(it->type->type&(AItemType::IT_MAN|AItemType::IT_MONSTER)) return true;
 }
 return false;
}
int AUnit::CanSee(ARegion * r, AUnit * u)
{
 if(!r) return 2;
 return faction->CanSee(r,u);
}
void AUnit::SetFaction(int facnum)
{
 AFaction *fac=turn->factions->GetFaction(facnum);
 if(fac)
   faction=fac;
}


bool AUnit::HasTaxOrder()
{
 AOrder *ord=orders->Find(O_TAX,2);
 if(ord)return true;
 //autotax ?
 bool b=GetEndFlag(FLAG_AUTOTAX);

 AOrderByte *bord=dynamic_cast<AOrderByte*>(orders->Find(O_AUTOTAX,2));
 if(bord){
  b=bord->value;
 }
 if(!b)return false;
 ord=new AOrderSimple;
 ord->type=O_TAX;
 orders->Add(ord);
 return true;
}
bool AUnit::HasTradeOrder()
{
 foreachr(orders){
  AOrder *ord=*(AOrder**) iter;
  if(ord->IsTradeOrder())
   return true;
 }
 return false;
}
int AUnit::Taxers()
{
 int TotalMen=GetMen();
 if(!TotalMen) return 0;
 if(GetSkill(CombSkill)) return TotalMen;
 int numNoWeapons=TotalMen;
 foreach(enditems){
  AItem *item=*(AItem**) iter;
  AItemType *itype=item->type;
  if(!(itype->type&AItemType::IT_WEAPON)) continue;
  AWeaponType *wtype=WeaponTypes->Get(itype->abr);
  if(!wtype) continue;
  if(wtype->flags&AWeaponType::NEEDSKILL){
   if(wtype->skill1.Length()&&!GetSkill(wtype->skill1))
    if(wtype->skill2.Length()){
     if(!GetSkill(wtype->skill2)){
      continue;
     }
    }else continue;
  }
  numNoWeapons-=item->count;
  if(numNoWeapons<=0) return TotalMen;
 }
 return TotalMen-numNoWeapons;
}
void AUnit::SetMoney(int count)
{
 enditems->SetNum("SILV",count);
}
int AUnit::GetMoney()
{
 return enditems->GetNum("SILV");
}


AnsiString AUnit::Alias()
{
 if(num>=0)return AnsiString(num);
 AnsiString str;
 if(faction&&faction->number){
  str="faction ";
  str+=faction->number;
  str+=" ";
 }
 str+="new ";
 str+=-num;
 return str;
}
int AUnit::GetBegRealSkill(AnsiString type)
{
 ASkill *sk=skills->GetSkill(type);
 if(!sk) return 0;
 return sk->GetLevel();
}
int AUnit::GetBegSkill(AnsiString type)
{
 if(type==ObseSkill) return GetBegObservation();
 if(type==SteaSkill) return GetBegStealth();
 if(type==TactSkill) return GetBegTactics();
 return GetBegRealSkill(type);
}
int AUnit::GetRealSkill(AnsiString type)
{
 ASkill *sk=endskills->GetSkill(type);
 if(!sk)return 0;
 return sk->GetLevel();
}
int AUnit::GetSkill(AnsiString type)
{
 if(type==EnteSkill) return GetEndEntertainment();
 return GetRealSkill(type);
}
int AUnit::GetBegSkillDays(AnsiString type)
{
 ASkill *sk=skills->GetSkill(type);
 if(!sk)return 0;
 return sk->days;
}
int AUnit::GetSkillDays(AnsiString type)
{
 ASkill *sk=endskills->GetSkill(type);
 if(!sk)return 0;
 return sk->days;
}
void AUnit::SetBegSkillDays(AnsiString type, int val)
{
 ASkill *sk=skills->GetSkill(type);
 if(!sk){
  sk=new ASkill;
  sk->SetType(type);
  skills->Add(sk);
 }
 sk->days=val;
}
void AUnit::SetSkillDays(AnsiString type, int val)
{
 ASkill *sk=endskills->GetSkill(type);
 if(!sk){
  sk=new ASkill;
  sk->SetType(type);
  endskills->Add(sk);
 }
 sk->days=val;
}
bool AUnit::IsLeader()
{
 foreach(enditems){
  AItem *it=*(AItem**)iter;
  if(it->type->type&AItemType::IT_LEADER)
    return true;
 }
 return false;
}
bool AUnit::IsNormal()
{
 return GetMen()&&!IsLeader();
}
void AUnit::UpdateWeight()
{
 int w=0,ww=0,rw=0,fw=0,sw=0;
 foreach(enditems){
  AItem *it=*(AItem**) iter;
  AItemType *itype=it->type; //ItemTypes->Get(it->type);
  w+=itype->weight*it->count;
  ww+=itype->walk*it->count;
  rw+=itype->ride*it->count;
  fw+=itype->fly*it->count;
  sw+=itype->swim*it->count;
  if( (itype->abr == "SILV") && (SilverWeight) )
  {
	 w += floor(it->count / SilverWeight);
  }
  if(itype->hitchItem.Length()){
   int hitches=enditems->GetNum(itype->hitchItem);
   int hitched=it->count;
   if(hitched>hitches) hitched=hitches;
   ww+=hitched*itype->hitchwalk;
   rw+=hitched*itype->hitchride;
   fw+=hitched*itype->hitchfly;
   sw+=hitched*itype->hitchswim;
  }
 }
 weight=w;
 walkweight=ww;
 ridweight=rw;
 flyweight=fw;
 swimweight=sw;
}
bool AUnit::CheckDepend(int lev, AnsiString dep, int deplev)
{
 int temp=GetRealSkill(dep);
 if(temp<deplev) return false;
 if(lev>=temp) return false;
 return true;
}

bool AUnit::CanStudy(AnsiString sk)
{
 ASkillType *stype=SkillTypes->Get(sk,2);
 if(!stype) return false;
 int curlev = GetRealSkill(sk);
 if(stype->depends[0].Length()){
  if(!CheckDepend(curlev,stype->depends[0],stype->levels[0]))
   return false;
 }else return true;
 if(stype->depends[1].Length()){
  if(!CheckDepend(curlev,stype->depends[1],stype->levels[1]))
   return false;
 }else return true;
 if(stype->depends[2].Length()){
  if(!CheckDepend(curlev,stype->depends[2],stype->levels[2]))
   return false;
 }else return true;
 return true;
}
bool AUnit::BegCheckDepend(int lev, AnsiString dep, int deplev)
{
 int temp=GetBegRealSkill(dep);
 if(temp<deplev) return false;
 if(lev>=temp) return false;
 return true;
}
bool AUnit::BegCanStudy(AnsiString sk)
{
 ASkillType *stype=SkillTypes->Get(sk,2);
 if(!stype) return false;
 int curlev = GetBegRealSkill(sk);
 if(stype->depends[0].Length()){
  if(!BegCheckDepend(curlev,stype->depends[0],stype->levels[0]))
   return false;
 }else return true;
 if(stype->depends[1].Length()){
  if(!BegCheckDepend(curlev,stype->depends[1],stype->levels[1]))
   return false;
 }else return true;
 if(stype->depends[2].Length()){
  if(!BegCheckDepend(curlev,stype->depends[2],stype->levels[2]))
   return false;
 }else return true;
 return true;
}
int AUnit::MoveType()
{
 int w=weight;
 if(w<=flyweight) return M_FLY;
 if(w<=ridweight) return M_RIDE;
 if(w<=walkweight) return M_WALK;
 return M_NONE;
}


int AUnit::CalcMovePoints(int movetype)
{
 if(movetype<0)
   movetype=MoveType();
 int mp=Plugins.CalcMovePoints(this,movetype);
 if(mp>=0) return mp;
 switch (movetype) {
  case M_NONE:
  return 0;
  case M_WALK:
  return WalkSpeed;
  case M_RIDE:
  return RideSpeed;
  case M_FLY:
   if(SwinSkill.Length()&&GetSkill(SwinSkill)) return WindSpeed;
  return FlySpeed;
  case M_SAIL:
  return SailSpeed;
 }
 return 0;
}
int AUnit::GetBegObservationBonus()
{
 int bonus=0;
 int men=GetMen();
 if(men==1)
  bonus=(GetBegSkill(TrueSkill)+1)/2;
 if(bonus<AmtsObseBonus&&men&&items->GetNum(AmtsItem))
  bonus=AmtsObseBonus;
 return bonus;
}
int AUnit::GetBegObservation()
{
 int retval=GetBegRealSkill(ObseSkill);
 retval+=GetBegObservationBonus();
//todo
 return retval;
}

int AUnit::GetBegStealthBonus()
{
  int mens = GetMen();
  if(!mens) return 0;
  // if (GetFlag(FLAG_INVIS)
  if(mens<=items->GetNum(RingItem))
	return RingSteaBonus;
 return 0;
}
int AUnit::GetBegStealth()
{
 int retval=GetBegRealSkill(SteaSkill);
 retval+=GetBegStealthBonus();
//- getbegstealth
 return retval;
}
int AUnit::GetBegTactics(){
 int retval=GetBegRealSkill(TactSkill);
 foreach(items){
  AItem *it=*(AItem**)iter;
  if(it->type->type&AItemType::IT_MONSTER){
   AMonstrType *mtype=MonstrTypes->Get(it->type->abr);
   if(mtype){
    int temp=mtype->tactics;
    if(temp>retval) retval=temp;
   }
  }
 }
 return retval;
}
int AUnit::GetBuildBonus(AObjectType*otype)
{
 if(otype->helpprodmult){
  int mens=GetMen();
  int bonus=mens;
	if(otype->helpproditem.Length()){
	 bonus=enditems->GetNum(otype->helpproditem);
	}
	if(bonus>mens) bonus=mens;
	return bonus*otype->helpprodmult;
 }
 return 0;
}
int AUnit::GetEndEntertainment(){
 int level=GetRealSkill(EnteSkill);
 if(!PhenSkill.Length())
   return level;
 int level2=5*GetRealSkill(PhenSkill);
 return std::max(level,level2);
}


bool AUnit::IsMage()
{
 if(combatspell.Length()) return true;
 foreach(endskills)
 {
   ASkill *sk=*(ASkill**)iter;
   if(sk->type->flags&ASkillType::MAGIC)
     return true;
 }
 return false;
}

int AUnit::GetSize()
{
 int siz=sizeof(*this);
 siz+=name.Length()+1;
 siz+=describe.Length()+1;
 siz+=orders->GetSize();
 siz+=items->GetSize();
 siz+=enditems->GetSize();
 siz+=skills->GetSize();
 siz+=endskills->GetSize();
 siz+=combatspell.Length()+1;
 siz+=sizeof(TStringList);
 siz+=canstudy->Text.Length()+1;
 return siz;
}
int AUnits::GetSize()
{
 int siz=sizeof(*this);
 siz+=Capacity*4;
 foreach(this){
  AUnit *un=*(AUnit**)iter;
  siz+=un->GetSize();
 }
 return siz;
}


bool AUnit::GetBattleItem(int battype, AnsiString item)
{
 AItemType *itype=ItemTypes->Get(item,4);
 if(!itype) return false;
 if(!(itype->type&battype)) return false;
 if(battype==AItemType::IT_BATTLE)
 {
// Exclude weapons.  They will be handled later.
   if(itype->type&AItemType::IT_WEAPON)
     return false;
 }
 AItem *it=enditems->GetItem(itype);
 if(!it||!it->count) return false;
 it->count--;
 return true;
}
void AUnit::ReturnBattleItem(AnsiString item)
{
 AItemType *itype=ItemTypes->Get(item,4);
 if(!itype) return;
 AItem *it=enditems->GetItem(itype);
 if(!it) return;
 it->count++;
}
int AUnit::GetSoldiers()
{
 int n=0;
 foreach(enditems) {
  AItem *it=*(AItem**)iter;
  if (it->IsSoldier()) n+=it->count;
 }
 return n;
}

AUnit * AUnit::CreateBattleUnit()
{
 AUnit *un=new AUnit(0);
 un->name=name;
// info=prvunit->info;
 un->num=num;
 un->faction=faction;
 un->flags=endflags;

 un->describe=describe;

// orders->Update(newun->orders);

 un->items->Clear();
 un->items->Update(items);
 un->items->DeleteNonCombat();
 un->enditems->Clear();
 un->hasskills=hasskills;
 un->skills->Update(skills);
 un->hascombatspell=hascombatspell;
 un->combatspell=combatspell;
 return un;
}
void AUnit::SetFlagOrders(unsigned _guard,unsigned _reveal,unsigned _flags){
 bool isnew=num<0;

 bool guard=_guard==GUARD_GUARD;
 bool avoid=_guard==GUARD_AVOID;
 int consume=REVEAL_NONE; //it is no error
 if(_flags&FLAG_CONSUMING_UNIT)
  consume=REVEAL_UNIT;
 if(_flags&FLAG_CONSUMING_FACTION)
  consume=REVEAL_FACTION;

 {
  AOrderByte *ord=dynamic_cast<AOrderByte*>(orders->Find(O_GUARD,2));
  if(ord){
   if(endguard!=_guard){
    if(isnew||this->guard!=_guard){
     ord->value=guard;
    }else{
     orders->Delete(ord);
    }
   }
  }else if(isnew||(_guard==GUARD_GUARD&&this->guard!=GUARD_GUARD)
   ||(this->guard==GUARD_GUARD&&_guard==GUARD_NONE)){
   AOrderByte *ord=new AOrderByte;
   ord->type=O_GUARD;
   ord->value=guard;
   orders->Add(ord);
  }
 }
 {
  AOrderByte *ord=dynamic_cast<AOrderByte*>(orders->Find(O_AVOID,2));
  if(ord){
   if(endguard!=_guard){
    if(isnew||this->guard!=_guard){
     ord->value=avoid;
    }else{
     orders->Delete(ord);
    }
   }
  }else if(isnew||(_guard==GUARD_AVOID&&this->guard!=GUARD_AVOID)
   ||(this->guard==GUARD_AVOID&&_guard==GUARD_NONE)){
   AOrderByte *ord=new AOrderByte;
   ord->type=O_AVOID;
   ord->value=avoid;
   orders->Add(ord);
  }
 }
 {
  AOrderByte *ord=dynamic_cast<AOrderByte*>(orders->Find(O_REVEAL,2));
  if(ord){
   if(endreveal!=_reveal){
    if(isnew||reveal!=_reveal){
     ord->value=_reveal;
    }else{
     orders->Delete(ord);
    }
   }
  }else if(isnew||endreveal!=_reveal){
   AOrderByte *ord=new AOrderByte;
   ord->type=O_REVEAL;
   ord->value=_reveal;
   orders->Add(ord);
  }
 }
 {
  AOrderByte *ord=dynamic_cast<AOrderByte*>(orders->Find(O_CONSUME,2));
  if(ord){
   if((endflags&FLAG_CONSUMING)!=(_flags&FLAG_CONSUMING)){
    if(isnew||(flags&FLAG_CONSUMING)!=unsigned(_flags&FLAG_CONSUMING)){
     ord->value=consume;
    }else{
     orders->Delete(ord);
    }
   }
  }else if(isnew||(endflags&FLAG_CONSUMING)!=(_flags&FLAG_CONSUMING)){
   AOrderByte *ord=new AOrderByte;
   ord->type=O_CONSUME;
   ord->value=consume;
   orders->Add(ord);
  }
 }
 int xorflags=flags^_flags;
 int xorendflags=endflags^_flags;
 {
  AOrderByte *ord=dynamic_cast<AOrderByte*>(orders->Find(O_BEHIND,2));
  if(ord){
   if(xorendflags&FLAG_BEHIND){
    if(isnew||(xorflags&FLAG_BEHIND)){
     ord->value=(_flags&FLAG_BEHIND)!=0;
    }else{
     orders->Delete(ord);
    }
   }
  }else if(isnew||(xorendflags&FLAG_BEHIND)){
   AOrderByte *ord=new AOrderByte;
   ord->type=O_BEHIND;
   ord->value=(_flags&FLAG_BEHIND)!=0;
   orders->Add(ord);
  }
 }
 {
  AOrderByte *ord=dynamic_cast<AOrderByte*>(orders->Find(O_AUTOTAX,2));
  if(ord){
   if(xorendflags&FLAG_AUTOTAX){
    if(isnew||(xorflags&FLAG_AUTOTAX)){
     ord->value=(_flags&FLAG_AUTOTAX)!=0;
    }else{
     orders->Delete(ord);
    }
   }
  }else if(isnew||(xorendflags&FLAG_AUTOTAX)){
   AOrderByte *ord=new AOrderByte;
   ord->type=O_AUTOTAX;
   ord->value=(_flags&FLAG_AUTOTAX)!=0;
   orders->Add(ord);
  }
 }
 {
  AOrderByte *ord=dynamic_cast<AOrderByte*>(orders->Find(O_HOLD,2));
  if(ord){
   if(xorendflags&FLAG_HOLDING){
    if(isnew||(xorflags&FLAG_HOLDING)){
     ord->value=(_flags&FLAG_HOLDING)!=0;
    }else{
     orders->Delete(ord);
    }
   }
  }else if(isnew||(xorendflags&FLAG_HOLDING)){
   AOrderByte *ord=new AOrderByte;
   ord->type=O_HOLD;
   ord->value=(_flags&FLAG_HOLDING)!=0;
   orders->Add(ord);
  }
 }
 {
  AOrderByte *ord=dynamic_cast<AOrderByte*>(orders->Find(O_NOAID,2));
  if(ord){
   if(xorendflags&FLAG_NOAID){
    if(isnew||(xorflags&FLAG_NOAID)){
     ord->value=(_flags&FLAG_NOAID)!=0;
    }else{
     orders->Delete(ord);
    }
   }
  }else if(isnew||(xorendflags&FLAG_NOAID)){
   AOrderByte *ord=new AOrderByte;
   ord->type=O_NOAID;
   ord->value=(_flags&FLAG_NOAID)!=0;
   orders->Add(ord);
  }
 }

 if(optServerSharing){
   AOrderByte *ord=dynamic_cast<AOrderByte*>(orders->Find(O_SHARING,2));
   if(ord){
     if(xorendflags&FLAG_SHARING){
       if(isnew||(xorflags&FLAG_SHARING)){
         ord->value = (_flags&FLAG_SHARING) != 0;
       } else {
         orders->Delete(ord);
       }
     }
   } else if(isnew||(xorendflags&FLAG_SHARING)){
       AOrderByte *ord=new AOrderByte;
       ord->type=O_SHARING;
       ord->value=(_flags&FLAG_SHARING) != 0;
       orders->Add(ord);
     }
 }

 if(optServNoCross){
  AOrderByte *ord=dynamic_cast<AOrderByte*>(orders->Find(O_NOCROSS,2));
  if(ord){
   if(xorendflags&FLAG_NOCROSS_WATER){
    if(isnew||(xorflags&FLAG_NOCROSS_WATER)){
     ord->value=(_flags&FLAG_NOCROSS_WATER)!=0;
    }else{
     orders->Delete(ord);
    }
   }
  }else if(isnew||(xorendflags&FLAG_NOCROSS_WATER)){
   AOrderByte *ord=new AOrderByte;
   ord->type=O_NOCROSS;
   ord->value=(_flags&FLAG_NOCROSS_WATER)!=0;
   orders->Add(ord);
  }
 }
 if(optServSpoils){
  int _spoils=GetSpoilsFromFlag(_flags);
  int spoils=GetSpoilsFromFlag(flags);
  int endspoils=GetSpoilsFromFlag(endflags);

  AOrderByte *ord=dynamic_cast<AOrderByte*>(orders->Find(O_SPOILS,2));
  if(ord){
   if(endspoils!=_spoils){
    if(isnew||spoils!=_spoils){
     ord->value=_spoils;
    }else{
     orders->Delete(ord);
    }
   }
  }else if(isnew||endspoils!=_spoils){
   AOrderByte *ord=new AOrderByte;
   ord->type=O_SPOILS;
   ord->value=_spoils;
   orders->Add(ord);
  }
 }else if(optServNoSpoils){
  AOrderByte *ord=dynamic_cast<AOrderByte*>(orders->Find(O_NOSPOILS,2));
  if(ord){
   if(xorendflags&FLAG_NOSPOILS){
    if(isnew||(xorflags&FLAG_NOSPOILS)){
     ord->value=(_flags&FLAG_NOSPOILS)!=0;
    }else{
     orders->Delete(ord);
    }
   }
  }else if(isnew||(xorendflags&FLAG_NOSPOILS)){
   AOrderByte *ord=new AOrderByte;
   ord->type=O_NOSPOILS;
   ord->value=(_flags&FLAG_NOSPOILS)!=0;
   orders->Add(ord);
  }
 }
 endguard=_guard;
 endreveal=_reveal;
 endflags=_flags;
}
bool AUnit::CanFly(){
 if(flyweight>=weight) return true;
 return false;
}
bool AUnit::CanReallySwim(){
 if(swimweight>=weight) return true;
 return false;
}
bool AUnit::CanSwim(){
 if(CanReallySwim())
  return true;
 if(optServFlightOverWater!=WFLIGHT_NONE&&CanFly())
  return true;
 return false;
}
int AUnit::GetMaxLevel(AnsiString skill)
{
 PluginUnit=this;
 int l=Plugins.ProcessSpecEvent(PE_CALCUNITMAXLEVEL,skill.c_str());
 if(l>=0) return l;
 int n=100;
 foreach(enditems){
  AItem *i=*(AItem**) iter;
  AItemType *itype=i->type;
  if(!itype)continue;
  if(!(itype->type&AItemType::IT_MAN)) continue;
  int l=ManTypes->GetLevel(itype->abr,skill);
  if(l<n) n=l;
 }
 return n;
}
int AUnit::GetSpoilsFromFlag(unsigned flags)
{
 if(flags&FLAG_NOSPOILS) return 0;
 if(flags&FLAG_WALKSPOILS) return 1;
 if(flags&FLAG_RIDESPOILS) return 2;
 if(flags&FLAG_FLYSPOILS) return 3;
 return 4;
}
unsigned AUnit::GetFlagFromSpoils(int spoils)
{
 switch(spoils){
  case 0: return FLAG_NOSPOILS;
  case 1: return FLAG_WALKSPOILS;
  case 2: return FLAG_RIDESPOILS;
  case 3: return FLAG_FLYSPOILS;
  default: return 0;
 }
}


int AUnit::CanUseWeapon(AWeaponType * wtype, bool riding)
{
 if(riding){
  if(wtype->flags&AWeaponType::NOMOUNT) return -1;
 }else{
  if(wtype->flags&AWeaponType::NOFOOT) return -1;
 }
 return CanUseWeapon(wtype);
}
int AUnit::CanUseWeapon(AWeaponType * wtype)
{
 if(!(wtype->flags&AWeaponType::NEEDSKILL)) return 0;
 int level1,level2;
 level1=GetBegSkill(wtype->skill1);
 level2=GetBegSkill(wtype->skill2);
 if(level2>level1) level1=level2;
 if(!level1) return -1;
 return level1;
}
void AUnit::PrepareOrders()
{
 endobj=baseobj;
 items->Sort();
 enditems->CreateNew(items);
 endskills->CreateNew(skills);
 UpdateWeight();

 endflags=flags;
 endguard=guard;
 endreveal=reveal;
 endfaction=faction;
 foreachr(orders){
  AOrder *o=*(AOrder**)iter;
  if(o->type!=O_MESSAGE)continue;
  orders->Delete(o);
 }
}
AOrderComment *AUnit::FindCommentOrder(AnsiString str)
{
  str+=" ";
  foreach(orders)
  {
    AOrder *o=*(AOrder**)iter;
    if(o->type!=O_COMMENT)continue;
    AOrderComment *ord=dynamic_cast<AOrderComment*>(o);
    if(!ord) continue;
    if(!strncmpi(ord->text.c_str(),str.c_str(),str.Length()))
      return ord;
  }
  return 0;
}
AnsiString AUnit::GetComment(AnsiString str)
{
  AOrderComment *ord=FindCommentOrder(str);
  if(!ord) return AnsiString();
  return ord->text.SubString(str.Length()+2,ord->text.Length());
}
void AUnit::SetComment(AnsiString str, AnsiString value)
{
  AOrderComment *ord=FindCommentOrder(str);
  if(!ord)
  {
    if(value.Length())
    {
      ord=new AOrderComment;
      ord->text=str+" "+value;
      orders->Add(ord);
      baseobj->basereg->turn->Modified=true;
    }
    return;
  }
  if(value.Length())
  {
    AnsiString text=str+" "+value;
    if(ord->text!=text)
    {
      ord->text=text;
      baseobj->basereg->turn->Modified=true;
    }
  }else
  {
    orders->Delete(ord);
    baseobj->basereg->turn->Modified=true;
  }
}
bool AUnit::CanSpy()
{
  if(endreveal != REVEAL_NONE) return false;
  if(GetBegStealth() < 5) return false;
  return true;
}
int AUnit::CalcNeeded()
{
  if(!IsLocal()) return 0;
  if(!faction->local) return 0;
  PluginUnit=this;
  int need=Plugins.ProcessSpecEvent(PE_CALCMAINTENANCE,0);
  if(need>=0)
    return need;
  if(IsLeader()){
    need=LeaderMaintCost;
  }else need=ManMaintCost;
  need*=GetMen();
  return need;
}
int AUnit::CalcStudyCost(AnsiString abr)
{
  if(!faction->local) return 0;
  PluginUnit=this;
  int cost=Plugins.ProcessSpecEvent(PE_CALCSTUDYCOST,abr.c_str());
  if(cost>=0)
    return cost;
  ASkillType *stype=SkillTypes->Get(abr,2);
  if(!stype) return 0;
  return GetMen()*stype->cost;
}
void AUnit::UnPlug()
{
  foreach(orders)
  {
    AOrder *ord=*(AOrder**)iter;
    if(ord->type!=O_PLUGIN) continue;
    ((AOrderPlugin*)ord)->UnPlug();
  }
}
int AUnit::CalcExpr(AnsiString expr)
{
  if(!expr.Length())
    return 0;
  ExprSetUnitMode(this);
  int res=0;
  try{
    res=ExprProcessExpr(expr);
  }catch (Exception &exception){
    AnsiString str=FullName()+": ";
    exception.Message=str+exception.Message;
    Application->ShowException(&exception);
  }
  return res;
}

