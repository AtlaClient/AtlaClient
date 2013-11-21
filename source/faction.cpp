//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <comctrls.hpp>

#include "util.h"
#include "faction.h"
#include "eapfile.h"
#include "turn.h"
#include "viewitems.h"
#include "region.h"
#include "unit.h"
#include "orders.h"
#include "reports.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
char *AttitudeStrs[] ={
  "Hostile",
  "Unfriendly",
  "Neutral",
  "Friendly",
  "Ally"
};

struct EventStr{
  EventTypes index;
  char capt[28];
  char key[28];
}

EvetnsStrs[]={
   {EVT_ADDRESS, "Adress:", "The address of "},
   {EVT_PREFFS, "Preffereds:", "): Preferred "},
   {EVT_GIVE, "Gives", "): Gives "},
   {EVT_TAX, "Taxes:", "): Collects $"},
   {EVT_SELL, "Sells:", "): Sells "},
   {EVT_BUY, "Buys:", "): Buys "},
   {EVT_SAIL, "Sails:", "] sails "},
   {EVT_WALK, "Walks:", "): Walks "},
   {EVT_RIDE, "Rides:", "): Rides "},
   {EVT_FLY, "Flies:", "): Flies "},
   {EVT_TEACH, "Teach:", "): Teaches "},
   {EVT_STUDY, "Study:", "): Studies "},
   {EVT_EARN, "Earn:", "): Earns "},
   {EVT_PROD, "Produce:", "): Produces "},
   {EVT_BUILD, "Build:",  "): Performs construction"},
   {EVT_BORROW, "Borrows:", "): Borrows "},
   {EVT_BORROW1, "Borrows:", ") borrows "},
};

int AttitudeStrToInt(AnsiString str){
 for(int i=0;i<5;i++){
  if(!AnsiCompareText(Trim(str),
    Trim(AnsiString(AttitudeStrs[i])))) return i;
  //if(!strncmpi(str, AttitudeStrs[i], str.Length())) return i;
  //if(str.Pos(AttitudeStrs[i])==1)return i;
 }
 return -1;
}

void Attitude::CreateNew(Attitude * prevat)
{
 if(!prevat)throw(Exception("Can't create attitude from NULL"));
 attitude=prevat->attitude;
 facnum=prevat->facnum;
}
AFaction*curFaction;
AFaction::AFaction(AFactions *facs)
 :TList(),defattitude(A_HOSTILE),number(0),local(false),unclaimed(0),
 warpts(0),warmax(0),warused(0),
 tradepts(0),trademax(0),tradeused(0),
 magepts(0),magemax(0),mageused(0),
 factions(facs),globalflags(0)
{
  orders=new AOrders((AUnit*)this);
  errors=new TStringList;
  battles=new TStringList;
  events=new TStringList;
  unkfacstatus=new TStringList;
  unkfacmoneys=new TStringList;
  color = clWindowText;
}
__fastcall AFaction::~AFaction()
{
// Clear();
 if(curFaction==this) curFaction=0;
 delete errors;errors=0;
 delete battles;battles=0;
 delete events;events=0;
 delete orders;orders=0;
 delete unkfacstatus;unkfacstatus=0;
 delete unkfacmoneys;unkfacmoneys=0;
}
void __fastcall AFaction::Clear()
{
 for(int i=Count-1;i>=0;i--){
  Attitude*t=(Attitude*)Items[i];
  delete t;
 }
 if(orders) orders->Clear();
 if(errors) errors->Clear();
 if(battles) battles->Clear();
 if(events) events->Clear();
 if(unkfacstatus) unkfacstatus->Clear();
 if(unkfacmoneys) unkfacmoneys->Clear();
 TList::Clear();
}
void AFaction::Read(TFile & in)
{
 Clear();
 in>>number>>name>>local;
 if(local){
  in>>unclaimed;
  in>>address>>password;
  in>>defattitude;
  in>>warpts>>warmax>>warused;
  in>>tradepts>>trademax>>tradeused;
  in>>magepts>>magemax>>mageused;

  int size;
  in>>size;
  Capacity=size;
  for(int i=0;i<size;i++){
   int facnum,attitude;
   in>>facnum>>attitude;
   SetAttitude(facnum,attitude);
  }
 }else{
  in>>address;
 }
 in>>globalflags;
 orders->Read(in);
 ReadStringList(in,errors);
 ReadStringList(in,battles);
 ReadStringList(in,events);
 ReadStringList(in,unkfacstatus);
 if(BaseVer>=31)
  ReadStringList(in,unkfacmoneys);
 int Cl;
 if(BaseVer>=33){
  in>>Cl; color = (TColor)Cl;
 }
}

void AFaction::Write(TFile& out)
{
 out<<number<<name<<local;
 if(local){
  out<<unclaimed;
  out<<address<<password;
  out<<defattitude;
  out<<warpts<<warmax<<warused;
  out<<tradepts<<trademax<<tradeused;
  out<<magepts<<magemax<<mageused;

  out<<count;
  for(int i=0;i<count;i++){
   Attitude*at=(Attitude*)Items[i];
   out<<(at->facnum)<<(at->attitude);
  }
 }else{
  out<<address;
 }
 out<<(globalflags&0xffff);
 orders->Write(out);
 WriteStringList(out,errors);
 WriteStringList(out,battles);
 WriteStringList(out,events);
 WriteStringList(out,unkfacstatus);
 WriteStringList(out,unkfacmoneys);
 out<<(int)color;
 
}
int AFaction::GetAttitude(int facnum, bool maydef)
{
 if(facnum==number)return A_ALLY;
 Attitude*a=Get(facnum);
 if(a) return a->attitude;
 if(maydef) return 5;
 return defattitude;
}
void AFaction::SetAttitude(int facnum, int att)
{
 Attitude*a=Get(facnum);
 if(att==-1){Remove(a); delete a;return;}
 if(!a){
  a=new Attitude;
  a->facnum=facnum;
  Add(a);
 }
 a->attitude=att;
}
Attitude* AFaction::Get(int facnum)
{

 for(int i=0;i<count;i++){
  Attitude*a=(Attitude*)Items[i];
  if(a->facnum==facnum) return a;
 }
 return 0;
}
void AFaction::CreateNew(AFaction *prevfac)
{
 if(!prevfac)
  throw(Exception("Can't create faction from NULL"));
 number=prevfac->number;
 name=prevfac->name;
 local=prevfac->local;
 unclaimed=0;//prevfac->unclaimed;
 address=prevfac->address;
 password=prevfac->password;
 defattitude=prevfac->defattitude;
 warpts=prevfac->warpts;
 warmax=prevfac->warmax;
 warused=0;//prevfac->warused;
 tradepts=prevfac->tradepts;
 trademax=prevfac->trademax;
 tradeused=0;//prevfac->tradeused;
 magepts=prevfac->magepts;
 magemax=prevfac->magemax;
 mageused=0;//prevfac->mageused;
 globalflags=prevfac->globalflags&0xffff;
 Clear();
 for(int i=0;i<prevfac->count;i++){
  Attitude*prevat=(Attitude*)prevfac->Items[i];
  Attitude*at=new Attitude;
  at->CreateNew(prevat);
  Add(at);
 }
}
AnsiString AFaction::FullName()
{
 return name+" ("+number+")";
}
AnsiString AFaction::ReportHeader()
{
 AnsiString end="\r\n";
 AnsiString s;
 s+=KeyPhrases->Get(keyAtlRepHead)+end;
 s+=FullName();
 if(!optConquest)
 {
   s+=" (";
   bool first=true;
   if(warpts){
    first=false;
    s+=KeyPhrases->Get(keyWarSign)+" "+warpts;
   }
   if(tradepts){
    if(!first) s+=", ";
    first=false;
    s+=KeyPhrases->Get(keyTradeSign)+" "+tradepts;
   }
   if(magepts){
    if(!first) s+=", ";
    s+=KeyPhrases->Get(keyMagicSign)+" "+magepts;
   }
   s+=")";
 }
 s+=end;
 s+=TurnNumberToString(curTurn->num)+end+end;
 s+=curTurns->ver1+end;
 s+=curTurns->ver2+end+end;
 s+=KeyPhrases->Get(keyFacStat)+end;
 if(!optConquest)
 {
   s+=KeyPhrases->Get(keyTaxStatSign)+warused+" ("+warmax+")"+end;
   s+=KeyPhrases->Get(keyTradeStatSign)+tradeused+" ("+trademax+")"+end;
 }  
 s+=KeyPhrases->Get(keyMagesStatSign)+mageused+" ("+magemax+")"+end;
 for(int i=0;i<unkfacstatus->Count;i++){
  s+=unkfacstatus->Strings[i]+end;
 }
 s+=end;
 return s;
}
void AFaction::MakeView(VListItems * vlis)
{
 VListItem *vli;
 AnsiString s;
 s=FullName();
 if(!optConquest)
 {
   s+=" (";
   s+=KeyPhrases->Get(keyWarSign)+" "+warpts+", ";
   s+=KeyPhrases->Get(keyTradeSign)+" "+tradepts+", ";
   s+=KeyPhrases->Get(keyMagicSign)+" "+magepts+")";
 }

 vli=new VListItem(vlis);
 vli->type=litFactHeader;
 vli->value=number;
 vli->text=s;

 s=KeyPhrases->Get(keyFacStat);
 vli=new VListItem(vlis);
 vli->type=litFactHeader;
 vli->value=number;
 vli->text=s;

 if(!optConquest)
 {
   s=KeyPhrases->Get(keyTaxStatSign)+warused+" : "+CountTaxRegions()+" ("+warmax+")";
   vli=new VListItem(vlis);
   vli->type=litFactHeader;
   vli->value=number;
   vli->text=s;

   s=KeyPhrases->Get(keyTradeStatSign)+" "+tradeused+" : "+CountTradeRegions()+" ("+trademax+")";
   vli=new VListItem(vlis);
   vli->type=litFactHeader;
   vli->value=number;
   vli->text=s;
 }
 s=KeyPhrases->Get(keyMagesStatSign)+" "+mageused+" : "+CountMages()+" ("+magemax+")";
 vli=new VListItem(vlis);
 vli->type=litFactHeader;
 vli->value=number;
 vli->text=s;

 for(int i=0;i<unkfacstatus->Count;i++){
  s=unkfacstatus->Strings[i];
  vli=new VListItem(vlis);
  vli->type=litFactHeader;
  vli->value=number;
  vli->text=s;
 }

 vli=new VListItem(vlis);
 vli->type=litFactHeader;
 vli->value=number;

 s=KeyPhrases->Get(keyAttitudes)+AttitudeStrs[defattitude]+")";
 vli=new VListItem(vlis);
 vli->type=litFactHeader;
 vli->value=number;
 vli->text=s;

 for(int i=0;i<NATTITUDES;i++){
  s=AttitudeStrs[i];
  s+=" : ";
  bool found=false;
  for(int j=0;j<count;j++){
   Attitude *t=(Attitude*)Items[j];
   if(t->attitude==i){
    AFaction *fac=curFactions->GetFaction(t->facnum);
    if(!fac)continue;
    if(found){
     s+=", ";
    }else{
     found=true;
    }
    s+=fac->FullName();
   }
  }
  if(!found)s+="none";
  vli=new VListItem(vlis);
  vli->type=litFactHeader;
  vli->value=number;
  vli->text=s;
 }
 vli=new VListItem(vlis);
 vli->type=litFactHeader;
 vli->value=number;

 s=KeyPhrases->Get(keyUnclaimed)+unclaimed;
 int claims=0;
 foreach(factions->turn->RegionList){
   ARegion *reg=*(ARegion**)iter;
   claims+=reg->claims[number];
 }
 if(claims)
 {
   s+=" (";
   s+=unclaimed-claims;
   s+=")";
 }
 vli=new VListItem(vlis);
 vli->type=litFactHeader;
 vli->value=number;
 vli->text=s;
 for(int i=0;i<unkfacmoneys->Count;i++){
  s=unkfacmoneys->Strings[i];
  vli=new VListItem(vlis);
  vli->type=litFactHeader;
  vli->value=number;
  vli->text=s;
 }

 vli=new VListItem(vlis);
 vli->type=litFactHeader;
 vli->value=number;

 int kol;
 kol=errors->Count;
 if(kol){
  s="Errors: ";
  s+=kol;
  vli=new VListItem(vlis);
  vli->type=litFactErrors;
  vli->value=number;
  vli->text=s;
 }
 kol=battles->Count;
 if(kol){
  s="Battles: ";
  if(battleindexs.size())
  {
    s+=battleindexs.size();
    s+=" (";
    s+=kol;
    s+=")";
  }else
    s+=kol;
  vli=new VListItem(vlis);
  vli->type=litFactBattles;
  vli->value=number;
  vli->text=s;
 }
 kol=events->Count;
 if(kol){
  s="Events: ";
  s+=kol;
  vli=new VListItem(vlis);
  vli->type=litFactEvents;
  vli->value=number;
  vli->text=s;
 }
 vli=new VListItem(vlis);
 vli->type=litFactHeader;
 vli->value=number;
 if(deleted){
  vli=new VListItem(vlis);
  vli->type=litFactHeader;
  vli->value=number;
  vli->text="Deleted";
 }
}
void AFaction::MakeViewErrors(VListItems * vlis)
{
 AnsiString s;
 VListItem *vli;
 
 s=KeyPhrases->Get(keyAtlErrHead);
 vli=new VListItem(vlis);
 vli->type=litFactErrorsHeader;
 vli->value=number;
 vli->text=s;

 for(int i=0;i<errors->Count;i++){
  vli=new VListItem(vlis);
  vli->type=litFactError;
  vli->value=number;
  vli->text=errors->Strings[i];
 }
}
void AFaction::MakeViewBattles(VListItems * vlis)
{
 AnsiString s;
 VListItem *vli;

 s=KeyPhrases->Get(keyAtlBatHead);
 vli=new VListItem(vlis);
 vli->type=litFactBattlesHeader;
 vli->value=number;
 vli->text=s;

 for(int i=0;i<(int)battleindexs.size();i++){
  vli=new VListItem(vlis);
  vli->type=litFactBattleHeader;
  vli->value=number;
  vli->text=battles->Strings[battleindexs[i].ind];
 }
 if(battleindexs.size()) return;
 for(int i=0;i<battles->Count;i++){
  vli=new VListItem(vlis);
  vli->type=litFactBattle;
  vli->value=number;
  vli->text=battles->Strings[i];
 }
}
void AFaction::MakeViewBattle(VListItems * vlis, int index)
{
 AnsiString s;
 VListItem *vli;

 s.printf("Battle %d:",index+1);
 vli=new VListItem(vlis);
 vli->type=litFactBattlesHeader;
 vli->value=number;
 vli->text=s;

 int begind=battleindexs[index].ind;
 int endind=battles->Count;
 if(index+1<(int)battleindexs.size())
   endind=battleindexs[index+1].ind;
 for(int i=begind;i<endind;i++){
  vli=new VListItem(vlis);
  vli->type=litFactBattle;
  vli->value=number;
  vli->text=battles->Strings[i];
 }
}
void AFaction::MakeViewEvents(VListItems * vlis)
{
 AnsiString s;
 VListItem *vli;

 s=KeyPhrases->Get(keyAtlEvnHead);
 vli=new VListItem(vlis);
 vli->type=litFactEventsHeader;
 vli->value=number;
 vli->text=s;

 for(int i=0;i<events->Count;i++){
  vli=new VListItem(vlis);
  vli->type=litFactEvent;
  vli->value=number;
  vli->text=events->Strings[i];
 }
}
void AFaction::MakeTree(TTreeNode * parnode, VTreeNodes * vtns)
{
 AnsiString str=FullName();
 if(deleted){
  str+=" (-)";
 }
 VTreeNode *vtn;
 vtn=new VTreeNode(vtns);
 vtn->type=tntFaction;
 vtn->value=number;
 TTreeNode *node=parnode->Owner->AddChildObject(parnode,str,vtn);
 vtn->node=node;
 TTreeNode *node2,*node3;
 if(errors->Count){
  vtn=new VTreeNode(vtns);
  vtn->type=tntFactionErrors;
  vtn->value=number;
  node2=parnode->Owner->AddChildObject(node,"Errors",vtn);
  vtn->node=node2;
 }
 if(battles->Count){
  if(!battleindexs.size()||!fullparsedbattle) ParseBattles();
  vtn=new VTreeNode(vtns);
  vtn->type=tntFactionBattles;
  vtn->value=number;
  node2=parnode->Owner->AddChildObject(node,"Battles",vtn);
  vtn->node=node2;
  for(int i=0;i<(int)battleindexs.size();i++)
  {
	bool presentthisfaction=battleindexs[i].presentthisfaction;
	vtn=new VTreeNode(vtns);
	vtn->type=tntFactionBattle;
	vtn->value=i+number*10000;
	vtn->data=(void*)presentthisfaction;
	node3=node2->Owner->AddChildObject(node2,battles->Strings[battleindexs[i].ind],vtn);
	vtn->node=node3;
    if(!presentthisfaction) continue;
    TV_ITEM tv;
    tv.hItem=node3->ItemId;
    tv.mask=TVIF_STATE;
    tv.stateMask=TVIS_BOLD;
    tv.state=0xffffffff;
    TreeView_SetItem(node3->Handle,&tv);
  }
 }
 if(events->Count){
   vtn = new VTreeNode(vtns);
   vtn->type = tntFactionEvents;
   vtn->value = number;
   node2 = parnode->Owner->AddChildObject(node,"Events",vtn);
   vtn->node = node2;
   // спробуем разбить евенты на категории...
   for(int i=sizeof(EvetnsStrs)/sizeof(EvetnsStrs[0])-1;i>=0;i--)
   {
	 EventStr &es = EvetnsStrs[i];
	 VTreeNode vtn1 = new VTreeNode(vtns);
	 vtn1->type = tntFactionEvents;
	 vtn1->value = es.index;
	 node2 = vtn->AddChildObject(node,es.capt,vtn);
	 vtn1->node = node2;
   }

   
 }
 node->Expand(false);
}

AFactions*curFactions;
AFactions::AFactions(ATurn *t)
 :TList(),turn(t)
{
}
__fastcall AFactions::~AFactions()
{
// Clear();
 if(curFactions==this) curFactions=0;
}
void __fastcall AFactions::Clear()
{
 for(int i=Count-1;i>=0;i--){
  AFaction * t=Get(i);
//  TList::Delete(i);
  delete t;
 }
 TList::Clear();
}
AFaction * AFactions::Get(int index)
{
 return (AFaction*)Items[index];
}
void AFactions::Read(TFile& in)
{
 Clear();
 int size;
 in>>size;
 Capacity=size;
 for(int i=0;i<size;i++){
  AFaction*fac=new AFaction(this);
  fac->Read(in);
  Add(fac);
 }
}
void AFactions::Write(TFile& out)
{
 out<<count;
 for(int i=0;i<count;i++){
  AFaction *fac=Get(i);
  fac->Write(out);
 }
}
AFaction* AFactions::GetFaction(int facnum)
{
// if(facnum==0) return 0; //temp

 for(int i=0;i<count;i++){
  AFaction*fac=Get(i);
  if(fac->number==facnum)return fac;
 }
 return 0;
}
AFaction* AFactions::GetFaction(AnsiString name)
{
 for(int i=0;i<count;i++){
  AFaction*fac=Get(i);
  if(fac->name==name)return fac;
 }
 return 0;
}
void AFactions::Add(AFaction* fac)
{
 if(GetFaction(fac->number))
  throw Exception(AnsiString("Error AFactions::Add(): number ")+fac->number+" already present");
 TList::Add(fac);
}
bool AFactions::SetCurFaction(int facnum)
{
  if(curFactions != this)return false;
  if(facnum == 0){ curFaction = 0; return true; }
  AFaction *fac = GetFaction(facnum);
  if(curFaction == fac) return true;
  if(!fac)return false;
  if(!fac->local)return false;
  curFaction = fac;
  AddEvent("Faction "+fac->FullName()+" activated");
  AtlaForm->miCF->Tag = facnum;
  for(int i=0; i<AtlaForm->miCF->Count; i++)
    if(AtlaForm->miCF->Items[i]->Tag == facnum) AtlaForm->miCF->Items[i]->Checked = true;
  AtlaForm->OnSetCurRegion();

  return true;
}

void AFactions::CreateNew(AFactions *prevfacs)
{
 Clear();
 if(!prevfacs){
  AFaction *fac;
  fac=new AFaction(this);
  fac->number=0;
  fac->name="Undetected";
  Add(fac);
  fac=new AFaction(this);
  fac->number=1;
  fac->name="The Guardsmen";
  Add(fac);
  fac=new AFaction(this);
  fac->number=2;
  fac->name="Creatures";
  Add(fac);
  return;
 }
 for(int i=0;i<prevfacs->count;i++){
  AFaction *prevfac=prevfacs->Get(i);
  if(prevfac->deleted) continue;
  AFaction *fac=new AFaction(this);
  fac->CreateNew(prevfac);
  Add(fac);
 }
}
//0 - not found, 1 - present, 2 - name, not num, 3 - num, not name
int AFactions::VerFaction(AnsiString name, int num)
{
 AFaction*fac;
 fac=GetFaction(num);
 if(fac){
  if(fac->name==name)return 1;
  return 3;
 }
 fac=GetFaction(name);
 if(fac)return 2;
 return 0;
}
void AFactions::Delete(int facnum)
{
 AFaction *fac=GetFaction(facnum);
 if(fac) TList::Remove(fac);
 delete fac;
}
void AFactions::MakeTree(TTreeNode * parnode, VTreeNodes * vtns)
{
 for(int i=0;i<count;i++){
  AFaction*fac=Get(i);
  if(!fac->local)continue;
  fac->MakeTree(parnode,vtns);
 }
}
bool AFaction::SaveTemplate(TFileStream * file)
{
 AnsiString s;
 s=KeyPhrases->Get(keyTemplateBegin);
 s+=number;
 s+=" \"";
 s+=password;
 s+="\"\n\n";
 file->Write(s.c_str(),s.Length());
 foreach(curRegionList){
  ARegion *reg=*(ARegion**)iter;
  reg->runned=false;
  bool hasany=false;
  foreach(reg){
   AObject *obj=*(AObject**)iter;
   bool newpresent=false;
   foreach(obj->units){
    AUnit *unit=*(AUnit**)iter;
    if(unit->faction!=this)continue;
    hasany=true;
    if(unit->num<0){newpresent=true;continue;}
    if(!unit->SaveTemplate(file))return false;
   }
   if(newpresent)foreach(obj->units){
    AUnit *unit=*(AUnit**)iter;
    if(unit->num>0)continue;
    if(unit->faction!=this)continue;
    if(!unit->SaveTemplate(file))return false;
   }
  }
  if(hasany)
    reg->runned=false;
 }
 s="#end\n\n";
 file->Write(s.c_str(),s.Length());

 return true;
}
int AFaction::CanSee(ARegion * r, AUnit * u)
{
 int detfac=0;
 if(u->faction==this) return 2;
 if(u->reveal==REVEAL_FACTION) return 2;
 int retval=0;
 if(u->reveal==REVEAL_UNIT) retval=1;
 foreach(r){
  AObject *obj=*(AObject**)iter;
  bool dummy=false;
  if(obj->num==0) dummy=true;
  foreach(obj->units){
   AUnit *temp=*(AUnit**)iter;
   if(u==temp&&dummy==false) retval=1;
   if(temp->faction==this){
//    if (temp->GetSkill(S_OBSERVATION) > u->GetSkill(S_STEALTH)) {
     return 2;
/*
    }else{
    if (temp->GetSkill(S_OBSERVATION) ==  u->GetSkill(S_STEALTH))
     retval=1;
    }*/
   }
  }
 }
 if (retval==1&&detfac) return 2;
 return retval;
}
int AFaction::CountTaxRegions()
{
 int count=0;
 foreach(factions->turn->RegionList){
	ARegion *reg=*(ARegion**)iter;
	if(reg->IsTaxRegion(number))
		count++;
 }
 return count;
}
int AFaction::CountTradeRegions()
{
 int count=0;
 foreach(factions->turn->RegionList){
	ARegion *reg=*(ARegion**)iter;
	if(reg->IsTradeRegion(number))
		count++;
 }
 return count;
}
int AFaction::CountMages()
{
 int count=0;
 foreach(factions->turn->RegionList){
	ARegion *reg=*(ARegion**)iter;
	foreach(reg){
	 AObject *obj=*(AObject**)iter;
	 foreach(obj->units){
		AUnit *u=*(AUnit**)iter;
		if(u->endfaction!=this)continue;
		if(!u->IsMage())continue;
		count++;
	 }
	}
 }
 return count;
}

bool AFaction::SaveReport(TFileStream * file)
{
 AnsiString s;
 s=ReportHeader();
// file->Write(s.c_str(),s.Length());
 SaveRepString(file,s);
/* {
  TStringList *list=new TStringList;
  list->Text=s;
  SaveRepStringList(file,list);
  delete list;
 }*/
 if(errors->Count){
  SaveRepString(file,KeyPhrases->Get(keyAtlErrHead));
  SaveRepStringList(file,errors);
  SaveRepString(file,"");
 }
 if(battles->Count){
  SaveRepString(file,KeyPhrases->Get(keyAtlBatHead));
  SaveRepStringList(file,battles);
  SaveRepString(file,"");
 }
 {
  SaveRepString(file,KeyPhrases->Get(keyAtlEvnHead));
  SaveRepStringList(file,events);
  SaveRepString(file,"");
 }
 {
  s=KeyPhrases->Get(keyAttitudes)+AttitudeStrs[defattitude]+"):";
  SaveRepString(file,s);

  for(int i=0;i<NATTITUDES;i++){
   s=AttitudeStrs[i];
   s+=" : ";
   bool found=false;
   for(int j=0;j<count;j++){
    Attitude *t=(Attitude*)Items[j];
    if(t->attitude==i){
     AFaction *fac=curFactions->GetFaction(t->facnum);
     if(!fac)continue;
     if(found){
      s+=", ";
     }else{
      found=true;
     }
     s+=fac->FullName();
    }
   }
   if(!found)s+="none";
   s+=".";
   SaveRepString(file,s);
  }
  SaveRepString(file,"");
 }
 {
  s=KeyPhrases->Get(keyUnclaimed)+unclaimed+".";
  SaveRepString(file,s);
  for(int i=0;i<unkfacmoneys->Count;i++){
    s=unkfacmoneys->Strings[i];
    SaveRepString(file,s);
  }
  SaveRepString(file,"");
 }
 foreach(factions->turn->RegionList){
  ARegion *reg=*(ARegion**)iter;
  if(reg->lastviewfact!=number&&!reg->PresentFaction(this))
   continue;
  if(!reg->WriteReport(file,roNormalReport)) return false;
 }
 SaveRepString(file,"");
 
 s=KeyPhrases->Get(keyTemplateSign);
 s+="(Short Format):";
 SaveRepString(file,s);
 SaveRepString(file,"");

 return SaveTemplate(file);
}



int AFaction::GetSize()
{
 int siz=sizeof(*this);
 siz+=Capacity*4;
 siz+=count*sizeof(Attitude);
 siz+=3*sizeof(TStringList);
 siz+=errors->Text.Length()+1;
 siz+=battles->Text.Length()+1;
 siz+=events->Text.Length()+1;
 siz+=orders->GetSize();
 return siz;
}
int AFactions::GetSize()
{
 int siz=sizeof(*this);
 siz+=Capacity*4;
 foreach(this){
  AFaction *fac=*(AFaction**) iter;
  siz+=fac->GetSize();
 }
 return siz;
}
//---------------------------------------------------------------------------
bool AFaction::VerifyEventsWarnings()
{
  if(!events->Count) return false;
  if(!EventsWarnings->Count) return false;
  int count=0;
  AnsiString str="Event: ";
  for(int i=0;i<events->Count;i++)
  {
    AnsiString ev=events->Strings[i];
    for(int j=0;j<EventsWarnings->Count;j++)
    {
      int pos=ev.Pos(EventsWarnings->Strings[j]);
      if(!pos) continue;
      count++;
      AddEvent(str+ev);
      break;
    }
  }
  return count;
}
//---------------------------------------------------------------------------
void AFaction::ParseBattles()
{
  ATurn *prevturn=curTurns->Get(curTurn->num-1);
  if(!prevturn&&battleindexs.size()&&fullparsedbattle==false)
    return;
  ARegionList *rl=0;
  if(prevturn) rl=prevturn->RegionList;
  battleindexs.clear();
  if(!battles->Count)
    return;
  for(int i=0;i<battles->Count;i++)
  {
    AnsiString str=battles->Strings[i];
    char *s=str.c_str();
    if(!*s) continue;
    int bt=IsBattleReport(s);
    if(!bt) continue;
    battleindexs.push_back(ABattleInfo(i));
    if(!rl)
    {
      fullparsedbattle=false;
      continue;
    }
    if(bt==3)//ass info
    {
      s=str.c_str();
      int num;
      if(ExtractBattleUnitName(s,num,0))
      {
        AUnit *un=rl->GetUnit(num);
        if(un&&un->faction->number==number)
          (battleindexs.end()-1)->presentthisfaction=true;
      }
      continue;
    }
    int stage=0; //1-attackers,2-defenders  
    for(int j=i+2;j<battles->Count;j++)
    {
      str=battles->Strings[j];
      if(!str.Length())
      {
        if(stage==2) break;
        else continue;
      }
      s=str.c_str();
      if(!MyCompare(s,KeyPhrases->Get(keyBattleAttackers)))
      {
        stage=1;
        continue;
      }
      if(!MyCompare(s,KeyPhrases->Get(keyBattleDefenders)))
      {
        stage=2;
        continue;
      }
      if(stage==0) break;
      int num;
      if(!ExtractBattleUnitName(s,num,0))
       break;
      AUnit *un=rl->GetUnit(num);
      if(!un) continue;
      if(un->faction->number!=number) continue;
      (battleindexs.end()-1)->presentthisfaction=true;
      break;
    }
    fullparsedbattle=true;
  }
}
//---------------------------------------------------------------------------
void AFaction::DeleteBattle(int index)
{
  if(unsigned(index)>=battleindexs.size())
    return;
  int begind=battleindexs[index].ind;
  int endind=battles->Count;
  if(index+1<(int)battleindexs.size())
    endind=battleindexs[index+1].ind;
  endind-=begind;
  for(int i=0;i<endind;i++)
    battles->Delete(begind);
  battleindexs.erase(battleindexs.begin()+index);
  for(int i=index;i<(int)battleindexs.size();i++)
    battleindexs[i].ind-=endind;
  curTurn->Modified=true;
}
//---------------------------------------------------------------------------

