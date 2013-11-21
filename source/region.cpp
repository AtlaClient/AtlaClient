//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <memory>
#include <algorithm>
#include "util.h"
#include "region.h"
#include "eapfile.h"
#include "turn.h"
#include "unit.h"
#include "viewitems.h"
#include "items.h"
#include "markets.h"
#include "faction.h"
#include "orders.h"
#include "skills.h"
#include "plugdoc\plugins.h"
#include "acplugins.h"
#include "gameparams.h"
#include "reports.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
unsigned GetXYZKey(unsigned x,unsigned y,unsigned z)
{
  return (z<<24)+(y<<12)+x;
}

void ALocation::Read(TFile & in)
{
 ReadIntAsShort(in,xloc);
 if(xloc!=-1){
  ReadIntAsShort(in,yloc);
  ReadIntAsShort(in,zloc);
 }
}
void ALocation::Write(TFile & out)
{
 WriteIntAsShort(out,xloc);
 if(xloc!=-1){
  WriteIntAsShort(out,yloc);
  WriteIntAsShort(out,zloc);
 }
}
void ALocation::operator=(const ARegion &reg)
{
  xloc=reg.xloc;
  yloc=reg.yloc;
  zloc=reg.zloc;
}
int ALocation::operator==(const ARegion &reg)
{
  return xloc==reg.xloc
         &&yloc==reg.yloc
         &&zloc==reg.zloc;
}

ARegion*curRegion;
ARegion::ARegion(ATurn *t)
 :key(0),
  hasInfo(false),
  xloc(-1),yloc(-1),zloc(-1),
  population(0),money(0),wages(0),maxwages(0),entertainment(0),
  gate(0),lastviewfact(0),taxincome(0),curtaxprod(0),taxers(0),
  workincome(0),workers(0),entincome(0),entattempt(0),
  turn(t),
  FRegionStatus(0)
  ,runned(false)
{
 wanteds=new AMarkets;
 saleds=new AMarkets;
 products=new AMarkets;
 CreateDummy();
}
_fastcall ARegion::~ARegion()
{
 Clear();
 if(curRegion==this)curRegion=0;
 delete products;products=0;
 delete wanteds;wanteds=0;
 delete saleds;saleds=0;
 ClearRegionStatus();
}
AObject * ARegion::Get(int num)
{
 return (AObject*)Items[num];
}
AObject *ARegion::GetObject(int objnum)
{
 foreach(this){
  AObject *obj=*(AObject**) iter;
  if(obj->num==objnum)return obj;
 }
 return 0;
}
AUnit * ARegion::GetUnit(int num)
{
 AUnit *unit;
 foreach(this){
  AObject *obj=*(AObject**) iter;
  unit=obj->GetUnit(num);
  if(unit) return unit;
 }
 return 0;
}

void ARegion::Add(AObject * obj)
{
 AObject *curobj=GetObject(obj->num);
 if(curobj)
   throw Exception("Error ARegion::Add(): object already present");
 TList::Add(obj);
 obj->basereg=this;
}
void __fastcall ARegion::Clear()
{
 int i=Count-1;
 AObject **iter=(AObject**)List+i;
 for(;i>=0;i--,iter--){
  delete *iter;
//  Delete(i);
 }
 TList::Clear();
}
void ARegion::Read(TFile& in)
{
 Clear();
 BYTE _type;
 WORD _name;
 in>>_type;
 type=curTurns->tertypes->Strings[_type];
 in>>_name;
 name=curTurns->provincies->Strings[_name];
 ReadIntAsShort(in,xloc);
 ReadIntAsShort(in,yloc);
 ReadIntAsShort(in,zloc);
 UpdateKey();

 in>>hasInfo;
 BYTE _towntype;
 WORD _townname;
 in>>_towntype;
 towntype=curTurns->towntypes->Strings[_towntype];
 if(towntype.Length()){
  in>>_townname;
  townname=curTurns->towns->Strings[_townname];
 }
 ALocation inner;
 if(BaseVer<27)
   inner.Read(in);
 in>>oldinfoturnnum;
 if(hasInfo){
  if(BaseVer>=32)
    in>>race;
  else
    ReadStringAsInt(in,race);
  race=ItemTypes->AbrToNames(race);
  in>>population>>money;
  in>>taxincome;
  in>>workincome;
  in>>entincome;
  in>>lastviewfact;
  in>>weather>>nxtweather;
  in>>wages>>maxwages;
  in>>entertainment;
  for(int i=0;i<NDIRS;i++){
   neighbors[i].Read(in);
  }
  in>>gate;
  wanteds->Read(in);
  saleds->Read(in);
  products->Read(in);
  in>>descr;
  int numreg;
  in>>numreg;
  Capacity=numreg;
  for(int i=0;i<numreg;i++){
   AObject *obj=new AObject(turn);
   obj->Read(in);
   Add(obj);
   if(BaseVer<27)
   {
     if(inner.xloc!=-1&&obj->inner)
     {
       obj->innerloc=inner;
       inner.xloc=-1;
     }
   }
  }
 }
 if(!count) CreateDummy();
}
void ARegion::Write(TFile& out)
{
 BYTE _type=curTurns->Tertypes(type);
 WORD _name=curTurns->Provincies(name);
 BYTE _towntype=curTurns->Towntypes(towntype);
 WORD _townname=curTurns->Towns(townname);
 out<<_type<<_name;
 WriteIntAsShort(out,xloc);
 WriteIntAsShort(out,yloc);
 WriteIntAsShort(out,zloc);

 int turnnum=turn->num;

 bool curhasInfo=hasInfo;
 out<<curhasInfo;
 out<<_towntype;
 if(towntype.Length()){
  out<<_townname;
 }
 out<<oldinfoturnnum;
 if(curhasInfo){
  AnsiString raceabr=ItemTypes->NameToAbr(race);
  out<<raceabr;
  out<<population<<money;
  out<<taxincome;
  out<<workincome;
  out<<entincome;

  out<<lastviewfact;
  if(oldinfoturnnum==turnnum)
   out<<weather<<nxtweather;
  else{
   AnsiString str;
   out<<str<<str;
  }
  out<<wages<<maxwages;
  out<<entertainment;
  for(int i=0;i<NDIRS;i++){
   neighbors[i].Write(out);
  }
  out<<gate;
  wanteds->Write(out);
  saleds->Write(out);
  products->Write(out);
  out<<descr;

  out<<count;
  foreach(this){
   AObject *obj=*(AObject**) iter;
   obj->Write(out);
  }
 }
}
void ARegion::CreateNew(ARegion * prevreg)
{
 if(!prevreg){
  type="nexus";
  xloc=0;
  yloc=0;
  zloc=0;
  UpdateKey();
  name="Unnamed";
  return;
 }
 type=prevreg->type;
 xloc=prevreg->xloc;
 yloc=prevreg->yloc;
 zloc=prevreg->zloc;
 UpdateKey();

 name=prevreg->name;
 towntype=prevreg->towntype;
 townname=prevreg->townname;

 taxincome=prevreg->taxincome;
 workincome=prevreg->workincome;
 entincome=prevreg->entincome;
 UpdateFromPrev(prevreg);
}

AnsiString ARegion::GetLocation()
{
  AnsiString s=" (";
  s += xloc;
  s += ",";
  s += yloc;
  if(!curRegionList)throw Exception("Error: curRegionList not defined");
  ARegionArray *ra = curRegionList->GetRegionArray(zloc);
  if(!ra)throw Exception("Error: level not found");
  if(ra->name.Length())
  {
	s += ",";
	s += ra->name;
  }
  s += ")";
  return s;
}

AnsiString ARegion::ShortName()
{
 AnsiString s=type+" (";
 s+=xloc;
 s+=",";
 s+=yloc;
 if(!curRegionList)throw Exception("Error: curRegionList not defined");
 ARegionArray *ra=curRegionList->GetRegionArray(zloc);
 if(!ra)throw Exception("Error: level not found");
 if(ra->name.Length()){
  s+=",";
  s+=ra->name;
 }
 s+=")"+KeyPhrases->Get(keyInSign);
 s+=name;
 return s;
}
AnsiString ARegion::FullName(bool fortemplate)
{
 AnsiString s;
 if(!fortemplate){
  AnsiString d=descr.TrimRight();
  if(d.Length()){
   s="("+d+") ";
  }
 }
 s+=ShortName();
 if(towntype.Length()){
  s+=KeyPhrases->Get(keyContains);
  s+=townname;
  s+=" ["+towntype+"]";
 }
 return s;
}
bool ARegion::WriteReport(TStream *strm,int mapoptions)
{
 if(!hasInfo) return true; //??
 AnsiString s=FullName();
 if(population){
  s+=", ";
  s+=population;
  s+=KeyPhrases->Get(keyPeasants);
  if(!optConquest)
    s+=race+")";
  s+=KeyPhrases->Get(keySilvSign)+money;
 }
 s+=".";
 SaveRepString(strm,s);
 {
   s=";";
   int curnum=curTurn->num;
   s+=curnum;
   if(curnum!=oldinfoturnnum)
   {
     s+="-";
     s+=oldinfoturnnum;
   }
   int len=60-s.Length();
   if(len<10) len=10;
   s=AnsiString::StringOfChar('-',len)+s+"\n";
   strm->Write(s.c_str(),s.Length());
 }
 ReportTab+=2;
 if(mapoptions&roRegionInfo){
  if(!optConquest)
  {
    s=WeatherTypes->Get(weather)->curmonthname;
    s+=WeatherTypes->Get(nxtweather)->nextmonthname;
    SaveRepString(strm,s);
  }
  s = ' ';
  s+= KeyPhrases->Get(keyWages);
  s+= wages;
  if(wages){
   s+=KeyPhrases->Get(keyMaxSign);
   s+=maxwages;
   s+=")";
  }
  s+=".";
  SaveRepString(strm,s);

  s = ' ';
  s+= KeyPhrases->Get(keyWantedSign);
  s+= wanteds->Print()+".";
  SaveRepString(strm,s);

  s = ' ';
  s+= KeyPhrases->Get(keySaledSign);
  s+= saleds->Print()+".";
  SaveRepString(strm,s);

  if(entertainment){
   s = ' ';
   s+= KeyPhrases->Get(keyEnteSign);
   s+= entertainment;
   s+= ".";
   SaveRepString(strm,s);
  }
  s = ' ';
  s+= KeyPhrases->Get(keyProdSign);
  int mode = 7;
  if(!(mapoptions&roAdvProducts))
   mode|=16;
  s+=products->Print(mode)+".";
  SaveRepString(strm,s);
 }
 SaveRepString(strm,"");

 ReportTab-=2;
 SaveRepString(strm,KeyPhrases->Get(keyExitSign));
 ReportTab+=2;
 for(int i=0;i<NDIRS;i++){
  ARegion *reg=curRegionList->Get(neighbors[i].xloc,neighbors[i].yloc,neighbors[i].zloc);
  if(!reg)continue;
  s=DirTypes->Get(i)->name+" : ";
  s+=reg->FullName()+".";
  SaveRepString(strm,s);
 }
 ReportTab-=2;
 SaveRepString(strm,"");
 if(mapoptions&roRegionInfo&&gate){
  s=KeyPhrases->Get(keyGatesSign);
  s+=gate;
  s+=KeyPhrases->Get(keyGatesSign2);
  s+=curRegionList->numberofgates;
  s+=").";
  SaveRepString(strm,s);
  SaveRepString(strm,"");
 }
 int obs = 100;
 int truesight = 0;
 bool detfac = false;
 if(!(mapoptions&(roLocalAsNonlocalUnit/*|mapoptions&roLocalUnit*/)))
// if( S_MIND_READING != -1 )
 {
  obs = GetObservation(curFaction);
  truesight = GetTrueSight(curFaction);
  if(MindSkill.Length())foreach(this){
   AObject *obj=*(AObject **) iter;
   foreach(obj->units){
    if(detfac) break;
    AUnit *un=*(AUnit**)iter;
    if(un->faction!=curFaction) continue;
    if(un->GetSkill(MindSkill) > 2){
     detfac = true;
     break;
    }
   }
  }
 }
 foreach(this){
  AObject *obj=*(AObject **) iter;
  if(!obj->WriteReport(strm,obs,truesight,detfac,mapoptions)) return false;
 }
// }
 SaveRepString(strm,"");

 return true;
}
AnsiString ARegion::Print()
{
 return "ERROR";
}
void ARegion::MakeView(VListItems * vlis)
{
 VListItem *vli;
 AnsiString s=FullName(false);
 int turnnum=turn->num;
 if(!hasInfo||oldinfoturnnum!=turnnum){
  s="?"+s;
 }
 if(hasInfo){
  RunOrders(false);
  if(population){
   s+=", ";
   s+=population;
   s+=" peasants";
   if(!optConquest)
     s+="("+race+")";
   vli=new VListItem(vlis);
   vli->type=litRegHeader;
   vli->value=-1;
   vli->text=s;
   s="Tax: $";
   s+=money;
   s+="/";
   int taxinc=taxincome;
   if(taxincome)s+=taxincome;
   else{ s+="auto";taxinc=TaxEffect;}
   s+=" Taxers: ";
   s+=taxers;
   s+="/";
   s+=money/taxinc;
  }
 }
 vli=new VListItem(vlis);
 vli->type=litRegHeader;
 vli->value=-1;
 vli->text=s;

 vli=new VListItem(vlis);
 vli->text="---------------------------------------------";
 if(!optConquest)
 {
   s="  ";
   AnsiString weat=WeatherRegions->GetWeather(xloc,yloc,zloc,curTurnNumber);
   AnsiString nxtweat=WeatherRegions->GetWeather(xloc,yloc,zloc,curTurnNumber+1);
   if(/*turn->num&&*/hasInfo&&oldinfoturnnum==turn->num){
    if(weat!=weather) s+="!";
    s+=WeatherTypes->Get(weather)->curmonthname;
    if(nxtweat!=nxtweather) s+="!";
    s+=WeatherTypes->Get(nxtweather)->nextmonthname;
   }else{
    s+=WeatherTypes->Get(weat)->curmonthname;
    s+=WeatherTypes->Get(nxtweat)->nextmonthname;
   }
   int sl=WeatherRegions->GetSeasonLength(xloc,yloc,zloc);
   if(sl!=12)
   {
     s+=" (";
     s+=sl;
     s+=" months)";
   }
   vli=new VListItem(vlis);
   vli->type=litRegWeather;
   vli->value=-1;
   vli->text=s;
 }
 if(hasInfo){
  s="  Wages: $";
  s+=wages;
  if(wages){
   s+="/";
   int wi=workincome;
   if(wi){ s+=wi;
   }else{ s+="auto"; wi=wages;}
   s+=" (Max: $";
   s+=maxwages;
   s+=")";
   s+=" Workers: ";
   s+=workers;
   s+="/";
   s+=maxwages/wi;
  }
  vli=new VListItem(vlis);
  vli->type=litRegWages;
  vli->value=-1;
  vli->text=s;
  s="  Wanted: ";
  s+=wanteds->Print();
  vli=new VListItem(vlis);
  vli->type=litRegWanted;
  vli->value=-1;
  vli->text=s;
  s="  For Sale: ";
  s+=saleds->Print();
  s+=".";
  vli=new VListItem(vlis);
  vli->type=litRegSaled;
  vli->value=-1;
  vli->text=s;
  s="  Entertainment available: $";
  s+=entertainment;
  s+="/";
  int ei=entincome;
  if(ei){ s+=ei;
  }else{ s+="auto";}
  s+=" Attempted: $";
  s+=entattempt;

  vli=new VListItem(vlis);
  vli->type=litRegEnte;
  vli->value=-1;
  vli->text=s;
  s="  Products: ";
  vli=new VListItem(vlis);
  vli->value=-1;
  vli->text=s;

  foreach(products){
   AMarket *mark=*(AMarket**) iter;
   s="    "+mark->Print(3);
   if(mark->price){
    s+=" Attempted: ";
    s+=mark->price;
   }
   vli=new VListItem(vlis);
   vli->type=litRegProd;
   vli->value=-1;
   vli->text=s;
  }
 new VListItem(vlis);

  s="";
  s+="Exits:";
  vli=new VListItem(vlis);
  vli->type=litRegExit;
  vli->value=-1;
  vli->text=s;
  for(int i=0;i<NDIRS;i++){
   ARegion *reg=curRegionList->Get(neighbors[i].xloc,neighbors[i].yloc,neighbors[i].zloc);
   if(!reg)continue;
   s="  "+DirTypes->Get(i)->name+" : ";
   s+=reg->FullName(false)+".";
   vli=new VListItem(vlis);
   vli->type=litRegExit;
   vli->value=i;
   vli->text=s;
  }
 new VListItem(vlis);
  if(gate){
   s="";
   s+=KeyPhrases->Get(keyGatesSign);
   s+=gate;
   s+=KeyPhrases->Get(keyGatesSign2);
   s+=curRegionList->numberofgates;
   s+=").";
   vli=new VListItem(vlis);
   vli->type=litRegGate;
   vli->value=gate;
   vli->text=s;

   new VListItem(vlis);
  }
  {
    AnsiString s1,s2;
    s1=curTurn->RegUserInfos[this];
    int pos=s1.Pos("-");
    if(pos)
    {
      s2=s1.SubString(pos,s1.Length());
      s1.SetLength(pos-1);
    }
    bool has=false;
    if(s1.Length())
    {
      vli=new VListItem(vlis);
      vli->value=-1;
      vli->text="Const User Info: "+s1;
      has=true;
    }
    if(s2.Length()>1)
    {
      vli=new VListItem(vlis);
      vli->value=-1;
      vli->text="Temp User Info: "+s2;
      has=true;
    }
    if(has) new VListItem(vlis); // vli=new VListItem(vlis);
  }
  for(TMapII::iterator i=claims.begin();i!=claims.end();i++)
  {
    AFaction *fac=curFactions->GetFaction(i->first);
    if(!fac) continue;
    if(!i->second) continue;
    s=fac->FullName()+" claims and withdraws $";
    s+=i->second;
    vli=new VListItem(vlis);
    vli->value=-1;
    vli->text=s;
  }
  if(claims.size()) new VListItem(vlis);
  //vli=new VListItem(vlis);
  
  foreach(this){
   AObject *obj=*(AObject **) iter;
   if(obj->num==0)continue;
   if(obj->units->count)s="++";
   else s="+ ";
   s+=obj->ObjectHeader(false);
   vli=new VListItem(vlis);
   vli->type=litRegObj;
   vli->value=obj->num;
   vli->text=s;
  }
  new VListItem(vlis);

  AObject *obj=GetDummy();
  obj->MakeView(vlis);
 }
}
//---------------------------------------------------------------------------
bool ARegion::HasBattles()
{
  foreach(curFactions)
  {
	AFaction *fac = *(AFaction**)iter;
	if(fac->local)
	{
	   if(fac->battles->Count){
		 for(int i=0;i<(int)fac->battleindexs.size();i++)
		 {
			AnsiString str = fac->battles->Strings[fac->battleindexs[i].ind];
			ALocation loc;
			if(!IsBattleReport(str.c_str(),&loc)) continue;
			if( (loc.xloc == xloc) && (loc.yloc == yloc) && (loc.yloc == yloc) ) return true;
		 }
	   }
	}
  }
  return false;
}
//---------------------------------------------------------------------------
void ARegion::MakeTreeBattle(TTreeNode * parnode, VTreeNodes * vtns)
{
  VTreeNode *vtn;
  TTreeNode *node3;
  foreach(curFactions)
  {
	AFaction *fac = *(AFaction**)iter;
	if(fac->local)
	{
	   if(fac->battles->Count){
		 for(int i=0;i<(int)fac->battleindexs.size();i++)
		 {
			AnsiString str = fac->battles->Strings[fac->battleindexs[i].ind];
			ALocation loc;
			if(!IsBattleReport(str.c_str(),&loc)) continue;
			if( (loc.xloc == xloc) && (loc.yloc == yloc) && (loc.yloc == yloc) )
			{
				// Битва в нужном хексе!
				bool presentthisfaction = fac->battleindexs[i].presentthisfaction;
				vtn = new VTreeNode(vtns);
				vtn->type = tntFactionBattle;
				vtn->value = i + fac->number*10000;
				vtn->data = (void*)presentthisfaction;
				node3 = parnode->Owner->AddChildObject(parnode,fac->battles->Strings[fac->battleindexs[i].ind],vtn);
				vtn->node = node3;
				if(!presentthisfaction) continue;
				TV_ITEM tv;
				tv.hItem=node3->ItemId;
				tv.mask=TVIF_STATE;
				tv.stateMask=TVIS_BOLD;
				tv.state=0xffffffff;
				TreeView_SetItem(node3->Handle,&tv);
            }
		 }
	   }
	}
  }

}
//---------------------------------------------------------------------------
void ARegion::MakeTree(TTreeNode * parnode, VTreeNodes * vtns)
{
  AnsiString str = FullName(false);
  VTreeNode *vtn;
  vtn = new VTreeNode(vtns);
  vtn->type = tntRegion;
  vtn->value = -1;
  TTreeNode *node = parnode->Owner->AddChildObject(parnode,str,vtn);
  vtn->node = node;

  if(HasBattles())
  {
	TTreeNode *node2;
	vtn = new VTreeNode(vtns);
	vtn->type = tntFactionBattles;
	vtn->value = curFaction->number;
	node2 = node->Owner->AddChildObject(node, "Battles", vtn);
	vtn->node = node2;
	node2->Expand(false);
	MakeTreeBattle(node2, vtns);
  }

  foreach(this)
  {
	AObject *obj=*(AObject **) iter;
	obj->MakeTree(node,vtns);
  }
}

void ARegion::Update(ARegion * newreg)
{
 type=newreg->type;
 name=newreg->name;
 towntype=newreg->towntype;
 townname=newreg->townname;
 if(newreg->lastviewfact)
  lastviewfact=newreg->lastviewfact;
 if(!newreg->hasInfo) return;
  population=newreg->population;
  race=newreg->race;
  money=newreg->money;
  weather=newreg->weather;
  nxtweather=newreg->nxtweather;
  wages=newreg->wages;
  maxwages=newreg->maxwages;
  entertainment=newreg->entertainment;
  if(oldinfoturnnum!=turn->num&&newreg->oldinfoturnnum==turn->num)
  {
    wanteds->Clear();
    saleds->Clear();
    products->Clear();
  }
  wanteds->Update(newreg->wanteds);
  saleds->Update(newreg->saleds);
  products->Update(newreg->products);
  for(int i=0;i<NDIRS;i++) neighbors[i]=newreg->neighbors[i];
  if(newreg->gate) gate=newreg->gate;
  if(!hasInfo||oldinfoturnnum!=turn->num){
    if(newreg->count>1)
    {
//verify destroyed
      foreachr(this){
       AObject *obj=*(AObject **) iter;
       if(!obj->num) continue;
       AObject *newobj=newreg->GetObject(obj->num);
       if(!newobj&&obj->Destroyable()){
        TList::Remove(obj);
        delete obj;
       }
      }
    }
  }
  oldinfoturnnum=newreg->oldinfoturnnum;
  hasInfo=true;

//verify present
  foreach(newreg){
   AObject *newobj=*(AObject **) iter;
   AObject *obj=GetObject(newobj->num);
   if(obj){
    obj->Update(newobj);
   }else{
    obj=new AObject(turn);
    obj->Update(newobj);
    Add(obj);
   }
  }
 ClearRegionStatus(); 
}
void ARegion::UpdateFromPrev(ARegion *prvreg)
{
 if(!prvreg->hasInfo) return;
 if(prvreg->gate){
  gate=prvreg->gate;
 }
 if(!hasInfo||oldinfoturnnum<=prvreg->oldinfoturnnum){
  hasInfo=true;
  oldinfoturnnum=prvreg->oldinfoturnnum;

  race=prvreg->race;
  population=prvreg->population;
  money=prvreg->money;
  wages=prvreg->wages;
  maxwages=prvreg->maxwages;
  entertainment=prvreg->entertainment;
  for(int i=0;i<NDIRS;i++)neighbors[i]=prvreg->neighbors[i];
  Clear();
  foreach(prvreg){
   AObject *prvobj=*(AObject **)iter;
   if(prvobj->num==0){
    CreateDummy();
    continue;
   }
   AObjectType *ot=ObjectTypes->Get(prvobj->type);
   if(!ot) continue;
   if(ot->sailors&&prvobj->incomplete==0) continue;
   AObject *obj;
   obj=new AObject(turn);
   obj->CreateNew(prvobj);
   Add(obj);
  }
  wanteds->CreateNew(prvreg->wanteds);
  saleds->CreateNew(prvreg->saleds);
  products->CreateNew(prvreg->products);
 }else
 {
//products update in TAtlaForm::miAdvProdsClick, no hidden adv saled
  wanteds->UpdateFromPrev(prvreg->wanteds);
 }
 foreach(prvreg){
  AObject *prvobj=*(AObject**)iter;
  foreach(prvobj->units){
   AUnit *prvun=*(AUnit**)iter;
   if(prvun->IsLocal()) continue;
   AUnit *un=GetUnit(prvun->num);
   if(!un) continue;
//    if(un->IsLocal()) continue;
   un->UpdateFromPrev(prvun);
  }
 }
 foreach(products)
 {
   AMarket *mark=*(AMarket**)iter;
   mark->price=0;
 }
 if(oldinfoturnnum==turn->num&&!descr.Length()){
  if(prvreg->descr.Length()) descr=prvreg->descr;
 }else if(oldinfoturnnum==prvreg->oldinfoturnnum){
  descr=prvreg->descr;
 }
}
int ARegion::GetDistance(int x, int y, int z)
{
 if( zloc != z )
 {
  return( 10000000 );
 }

 ARegionArray *pArr = curRegionList->GetRegionArray(z);
 int maxy;
 maxy = yloc - y;
 if (maxy < 0) maxy = -maxy;

 int maxx = xloc - x;
 if (maxx < 0) maxx = -maxx;

 int max2 = xloc + pArr->x - x;
 if (max2 < 0) max2 = -max2;
 if (max2 < maxx) maxx = max2;

 max2 = xloc - (x + pArr->x);
 if (max2 < 0) max2 = -max2;
 if (max2 < maxx) maxx = max2;

 if (maxy > maxx) return (maxx + maxy) / 2;
 return maxx;
}

ARegionArray*curRegionArray;
ARegionArray::ARegionArray()
 :x(1),y(1),levelnum(-1)
{
}
ARegionArray::~ARegionArray()
{
 if(curRegionArray==this)curRegionArray=0;
}
ARegion* ARegionArray::Get(int x, int y)
{
 return curRegionList->Get(x,y,levelnum);
}
void ARegionArray::Read(TFile& in)
{
 in>>levelnum>>name;
 in>>x>>y;
}
void ARegionArray::Write(TFile& out)
{
 out<<levelnum<<name;
 out<<x<<y;
}
void ARegionArray::CreateNew(ARegionArray* prevra)
{
 if(!prevra)throw Exception("Can't create RegionArray from NULL");
 levelnum=prevra->levelnum;
 name=prevra->name;
 x=prevra->x;
 y=prevra->y;
}
void ARegionArray::NormCoord(int & xloc, int & yloc)
{
 int xx=x;
 if(xx&1) xx++;
 while(xloc<0) xloc+=xx;
 while(xloc>=xx) xloc-=xx;

 if(yloc<0) yloc=0;
 if((xloc+yloc)&1) yloc++;
 if(yloc>=y) yloc=y-1;
 if((xloc+yloc)&1) yloc--;
 if(yloc<0){ yloc++;xloc--; }
}
void ARegionArray::NormCoord2(int & xloc, int & yloc)
{
 if(xloc<=-x) xloc=-x+1;
 if(xloc>=2*x-1) xloc=2*x-2;
 if(yloc<0) yloc=0;
 if(yloc>=y) yloc=y-1;
}

void ARegionArray::UpdateFromPrev(ARegionArray * prvar)
{
 if(x<prvar->x) x=prvar->x;
 if(y<prvar->y) y=prvar->y;
}
void ARegionArray::VerifySize(int xloc, int yloc)
{
 if(!xloc&&!yloc) return;
 if(x<=xloc) x=xloc+1;
 if(y<=yloc) y=yloc+1;
/* if(x/8<=xloc/8) x=(xloc/8+1)*8;
 if(y/8<=yloc/8) y=(yloc/8+1)*8;*/
}

ARegionArrays::ARegionArrays()
{
}
__fastcall ARegionArrays::~ARegionArrays()
{
 Clear();
}
void __fastcall ARegionArrays::Clear()
{
 int i=Count-1;
 if(i<0) return;
 ARegionArray **iter=(ARegionArray**)List+i;
 for(;i>=0;i--,iter--){
  delete *iter;
//  Delete(i);
 }
 TList::Clear();
}
void ARegionArrays::Read(TFile& in)
{
 Clear();
 int size;
 in>>size;
 Capacity=size;
 for(int i=0;i<size;i++){
  ARegionArray *ar=new ARegionArray;
  ar->Read(in);
  Add(ar);
 }
}
void ARegionArrays::Write(TFile& out)
{
 out<<count;
 foreach(this){
  ARegionArray*ar=*(ARegionArray**) iter;
  ar->Write(out);
 }
}
void ARegionArrays::Add(ARegionArray* ar)
{
 if(ar->levelnum==-1)ar->levelnum=count;
 if(ar->levelnum!=count)
   throw Exception("Error ARegionArrays::Add(): number not match");
 TList::Add(ar);
}
ARegionArray* ARegionArrays::Get(int levelnum)
{
 return (ARegionArray*)Items[levelnum];
}
ARegionArray* ARegionArrays::Get(AnsiString levelname)
{
 foreach(this){
  ARegionArray *ra=*(ARegionArray**)iter;
  if(ra->name==levelname)return ra;
 }
 return 0;
}
int ARegionArrays::GetLevelNum(AnsiString levelname)
{
 ARegionArray *ra=Get(levelname);
 if(!ra)return -1;
 return ra->levelnum;
}
void ARegionArrays::CreateNew(ARegionArrays * prevras)
{
 Clear();
 if(!prevras){
  ARegionArray *ra;
  ra=new ARegionArray;
  ra->levelnum=0;
  ra->name="nexus";
  Add(ra);
  ra=new ARegionArray;
  ra->levelnum=1;
  Add(ra);
  return;
 }
 foreach(prevras){
  ARegionArray*prevra=*(ARegionArray**)iter;
  ARegionArray *ra=new ARegionArray;
  ra->CreateNew(prevra);
  Add(ra);
 }
}
void ARegionArrays::UpdateFromPrev(ARegionArrays * prvars)
{
 foreach(prvars){
  ARegionArray *prvar=*(ARegionArray**)iter;
  ARegionArray *ar=Get(prvar->name);
  if(!ar){
   ar=new ARegionArray;
   ar->CreateNew(prvar);
   Add(ar);
   continue;
  }
  ar->UpdateFromPrev(prvar);
 }
}
ARegionList*curRegionList;
ARegionList::ARegionList(ATurn *t)
 :TList(),numberofgates(0),turn(t),RegionArrays(new ARegionArrays)
{
}
__fastcall ARegionList::~ARegionList()
{
 Clear();
 delete RegionArrays;
 if(curRegionList==this)curRegionList=0;
}
void __fastcall ARegionList::Clear()
{
 int i=Count-1;
 if(i<0) return;
 ARegion **iter=(ARegion**)List+i;
 for(;i>=0;i--,iter--){
  delete *iter;
//  Delete(i);
 }
 TList::Clear();
}
void ARegionList::Read(TFile& in)
{
 Clear();
 in>>numberofgates;
 RegionArrays->Read(in);
 int size;
 in>>size;
 Capacity=size;
 for(int i=0;i<size;i++){
  ARegion *reg=new ARegion(turn);
  reg->Read(in);
  Add(reg);
 }
}
void ARegionList::Write(TFile& out)
{
 out<<numberofgates;
 RegionArrays->Write(out);
 int s=0;
 foreach(this){
  ARegion*reg=*(ARegion**)iter;
  if(!reg->IsSaveRegion())
   continue;
  s++;
 }
 out<<s;
 foreach(this){
  ARegion*reg=*(ARegion**)iter;
  if(!reg->IsSaveRegion())
   continue;
  reg->Write(out);
 }
}
bool inline RegionCompare(const ARegion*a,const unsigned key)
{
  return a->key<key;
}
void ARegionList::Add(ARegion * reg)
{
  reg->UpdateKey();
  if(!count)
  {
    TList::Add(reg);
    return;
  }
  unsigned key=reg->key;
  ARegion **begin=(ARegion**)List,**end=begin+count;
  ARegion **reg2=std::lower_bound(begin,end,key,RegionCompare);
  if(reg2!=end)
  {
    if(reg->key==(*reg2)->key)
      throw Exception("Error ARegionList::Add(): region already exists for turn "+AnsiString(turn->num));
  }
  TList::Insert(reg2-begin,reg);
}
ARegion* ARegionList::Get(int regnum)
{
 return (ARegion*)Items[regnum];
}
ARegion* ARegionList::GetByKey(unsigned key)
{
  ARegion **begin=(ARegion**)List,**end=begin+count;
  ARegion **reg2=std::lower_bound(begin,end,key,RegionCompare);
  if(reg2==end)
    return 0;
  if((*reg2)->key==key)
    return *reg2;
  return 0;
}
ARegion* ARegionList::Get(int x, int y, int z)
{
  if(x<0) return 0;
  if((x+y)&1) return 0;
  unsigned key=GetXYZKey(x,y,z);
  return GetByKey(key);
}
ARegion * ARegionList::GetNearestRegion(int x, int y, int z)
{
 int nearnum=-1,neardist=1000000,cur;
 foreach(this){
  ARegion *reg=*(ARegion**)iter;
  if(reg->zloc!=z)continue;
  cur=reg->GetDistance(x,y,z);
//if(cur==0) 
  if(cur<neardist){neardist=cur;nearnum=TList::IndexOf(reg);}
 }
 if(nearnum==-1)return 0;
 return Get(nearnum);
}

ARegionArray* ARegionList::GetRegionArray(int level)
{
 return RegionArrays->Get(level);
}
bool ARegionList::SetCurRegionArray(int levelnum)
{
 if(curRegionList!=this)return false;
 ARegionArray *ra=GetRegionArray(levelnum);
 if(!ra)return false;
 curRegionArray=ra;
 return true;
}
bool ARegionList::SetCurRegion(int x, int y, int z)
{
 if(!SetCurRegionArray(z))return false;
 ARegion *reg=Get(x,y,z);
 if(!reg)return false;
 curRegion=reg;
 return true;
}
void ARegionList::CreateNew(ARegionList* prevrl)
{
 Clear();
 if(!prevrl){
  numberofgates=0;
  RegionArrays->CreateNew(0);
  ARegion *reg=new ARegion(turn);
  reg->turn=turn;
  reg->CreateNew(0);
  Add(reg);
  return;
 }
 numberofgates=prevrl->numberofgates;
 RegionArrays->CreateNew(prevrl->RegionArrays);
 foreach(prevrl){
  ARegion*prevreg=*(ARegion**)iter;
  ARegion*reg=new ARegion(turn);
  reg->turn=turn;
  reg->CreateNew(prevreg);
  Add(reg);
 }
}
void ARegionList::UpdateFromPrev(ARegionList * prvrl)
{
//static int number=0;
 if(numberofgates<prvrl->numberofgates)
  numberofgates=prvrl->numberofgates;
 RegionArrays->UpdateFromPrev(prvrl->RegionArrays);
 foreach(prvrl){
  ARegion *prvreg=*(ARegion**)iter;
  ARegion *reg=Get(prvreg->xloc,prvreg->yloc,prvreg->zloc);
  if(!reg){
   reg=new ARegion(turn);
   reg->turn=turn;
   reg->CreateNew(prvreg);
   Add(reg);
   continue;
  }
  reg->UpdateFromPrev(prvreg);
 }
 if(prvrl->turn->Modified){
  turn->Modified=true;
 }
 turn->FixShafts(0,0);
}
bool ARegionList::UpdateAdvProdsFromPrev(ARegionList * prvrl,bool withmessages)
{
 AnsiString mes;
 bool added=false;
 int numreg=0;
 foreach(this){
	ARegion *reg=*(ARegion**)iter;
	if(!reg->hasInfo||reg->oldinfoturnnum!=reg->turn->num)
//  if(!(reg->infoStatus&isExitsInfo) ||(reg->oldexitsinfo!=curTurnNumber))
	 continue;
	numreg++;
	ARegion *prvreg=prvrl->GetByKey(reg->key);
  ATerrainType *ttype=TerrainTypes->Get(reg->type);
  foreach(ItemTypes){
   AItemType *itype=*(AItemType**)iter;
   if(!ttype->CanProduceAdv(itype->abr)) continue;
   if(!(itype->type&AItemType::IT_ADVANCED))continue;
   if(!itype->skill.Length())continue;
   if(itype->inputs[0].Length())continue;
   if(reg->products->GetMarket(itype->abr))continue;
   bool canmake=false;
   foreach(curFactions){
    AFaction *fac=*(AFaction**)iter;
    if(!fac->local) continue;
    if(!reg->CanMakeAdv(fac,itype->abr)) continue;
    canmake=true;
    break;
   }
   if(!canmake)
   {
     if(prvreg)
     {
       AMarket *prvmark=prvreg->products->GetMarket(itype->abr);
       if(prvmark)
       {
         AMarket *mark=new AMarket;
         mark->CreateNew(prvmark);
         reg->products->Add(mark);
         added=true;
       }
     }
     continue;
   }
   added=true;
   AMarket *mark=new AMarket;
   mark->type=itype;
   reg->products->Add(mark);
   if(prvreg)
   {
     AMarket *prvmark=prvreg->products->GetMarket(itype->abr);
     if(prvmark)
       continue;
	 }
	 if(withmessages)
	 {
		 mes="Advanced product \"";
		 mes+=itype->name;
		 mes+="\" added in ";
		 mes+=reg->FullName();
		 AddEvent(mes);
	 }
  }
 }
 if(added)
	turn->Modified=true;
 if(withmessages)
 {
	 mes="Advanced products analyze ended. Scaned ";
	 mes+=numreg;
	 mes+=" regions.";
	 AddEvent(mes);
 }
 return added;
}
/*void ARegionList::UpdateFromNext(ARegionList * nxtrl)
{
//static int number=0;
 if(numberofgates<nxtrl->numberofgates)
  numberofgates=nxtrl->numberofgates;
 RegionArrays->UpdateFromPrev(nxtrl->RegionArrays);
 foreach(nxtrl){
  ARegion *nxtreg=*(ARegion**)iter;
  if(!nxtreg->hasInfo) continue;
  if(nxtreg->oldinfoturnnum>=turn->num) continue;
  ARegion *reg=Get(nxtreg->xloc,nxtreg->yloc,nxtreg->zloc);
  if(!reg){
   reg=new ARegion(turn);
   reg->turn=turn;
   reg->CreateNew(nxtreg);
   reg->num=-1;
   Add(reg);
   continue;
  }
  if(reg->hasInfo)
   continue;
  reg->UpdateFromPrev(nxtreg);
 }
}*/
void ARegion::CreateDummy()
{
 AObject *obj=new AObject(turn);
 obj->num=0;
 Add(obj);
}
AObject * ARegion::GetDummy()
{
 AObject *obj=Get(0);
 if(obj->num==0)return obj;
 return GetObject(0);
}
int ARegion::GetNewUnitNum(){
 int minunitnum=0;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   if(u->num<minunitnum) minunitnum=u->num;
  }
 }
 return minunitnum-1;
}
int ARegionList::GetNewUnitNum(){
 int minunitnum=0;
 foreach(this){
   ARegion *reg=*(ARegion**)iter;
   int num=reg->GetNewUnitNum();
   if(num<minunitnum) minunitnum=num;
 }
 return minunitnum;
}

AObject *curObject;
AObject::AObject(ATurn *t)
 :num(-1),incomplete(0),runes(0),basereg(0),inner(false),
  turn(t),endreg(0),owner(0)
{
 units=new AUnits(this);
}
__fastcall AObject::~AObject()
{
 Clear();
 if(curObject==this)curObject=0;
 delete units;units=0;
}
void __fastcall AObject::Clear()
{
 if(units){delete units; units=0;}
}
AUnit * AObject::GetUnit(int num)
{
 return units->GetUnit(num);
}

AnsiString AObject::FullName()
{
 AnsiString s;
 s=name;
 s+=" [";
 s+=num;
 s+=KeyPhrases->Get(keyObjNameEndSign);
 s+=type;
 return s;
}
AnsiString AObject::ObjectHeader(bool isreport)
{
 if(num==0)return "";
 AnsiString s,desc2;
// s="+ ";
 s+=FullName();
 if(incomplete>0){
  s+=KeyPhrases->Get(keyObjNeedSign);
  if(incomplete==-1)s+="??";
  else{
   s+=incomplete;
   if(!isreport&&endincomplete!=incomplete){
    s+=" (";
    s+=endincomplete;
    s+=")";
   }
  }
 }
 if(IsRoad()){
  if(incomplete==-1){
   s+=KeyPhrases->Get(keyObjDecayWarning);
  }else if(incomplete==-2){
   s+=KeyPhrases->Get(keyObjMaintenanceWarning);
  }
 }
 if(inner){
  s+=KeyPhrases->Get(keyObjContainsSign);
  if(!isreport){
   if(innerloc.xloc==-1){
    s+=" (not linked)";
   }else{
    s+=" (";
    ARegion *reg=curRegionList->Get(innerloc);
    s+=reg->FullName(false)+")";
   }
  }else{
   if(innerloc.xloc!=-1){
    ARegion *reg=curRegionList->Get(innerloc);
    desc2=";!"+reg->FullName();
   }
  }
 }
 if(runes){
  s+=KeyPhrases->Get(keyObjEngravedSign);
 }
 if(describe.Length()){
  s+="; ";
  s+=describe;
 }else if(desc2.Length()){
  s+="; ";
  s+=desc2;
 }
 AObjectType *ot=ObjectTypes->Get(type);
 if(ot&&ot->canenter==0){
  s+=KeyPhrases->Get(keyObjClosedSign);
 }
// s+=".";
 return s;
}
bool AObject::WriteReport(TStream *strm,int obs,int truesight,bool detfac,int mapoptions)
{
 AnsiString s;
 if(num){
  bool isuser=Destroyable();
  if(isuser){
   if(!(mapoptions&roUserObject))
    return true;
  }else{
   if(!(mapoptions&roNonuserObject))
    return true;
  }
  s="+ "+ObjectHeader(true)+".";
  SaveRepString(strm,s);
 }
 if(!(mapoptions&(roLocalUnit|roLocalAsNonlocalUnit|roNonlocalUnit))){
  SaveRepString(strm,"");
  return true;
 }

 if(num!=0) ReportTab+=2;
 foreach(units){
  AUnit *un=*(AUnit**)iter;
  if(un->faction->local){
   if(mapoptions&roLocalUnit){
    if(curFaction){
     if(un->faction==curFaction){
      if(!un->WriteReport(strm,-1,1,1,true)) return false;
      continue;
     }
    }else{
     if(un->faction->local){
      if(!un->WriteReport(strm,-1,1,1,true)) return false;
      continue;
     }
    }
   }else{
    if(!(mapoptions&roLocalAsNonlocalUnit))
     continue;
   }
  }else{
   if(!(mapoptions&roNonlocalUnit))
    continue;
  }
  if(!un->WriteReport(strm,obs,truesight,detfac,num!=0)) return false;
 }
 SaveRepString(strm,"");
 if(num!=0) ReportTab-=2;
 return true;
}
AnsiString AObject::Print()
{
 AnsiString s;
/* if(num)s="+ "+ObjectHeader(false)+"\r\n";
 if(num==0)return s;
 AObjectType *ot=ObjectTypes->Get(type);
 if(!ot||ot->canenter)return s;
 AnsiString tab="  ";
 foreach(units){
  AUnit *un=*(AUnit**)iter;
  s+=tab+un->Print()+"\r\n";
 }
 s+="\r\n";*/
 return s;
}
void AObject::MakeView(VListItems * vlis)
{
 AnsiString s;
 VListItem *vli;
 if(num){
  if(units->count)s="++";
  else s="+ ";
  s+=ObjectHeader(false);
  vli=new VListItem(vlis);
  vli->type=litObjHeader;
  vli->value=num;
  vli->text=s;

  int protect=Protect();
	int capacity=Capacity();
	int protect2=0;
	int cap2=0;
	if(protect||capacity)	foreach(basereg)
	{
		AObject *obj=*(AObject**)iter;
		foreach(obj->units){
			AUnit *un=*(AUnit**)iter;
			if(un->endobj!=this) continue;
			protect2+=un->GetMen();
			cap2+=un->weight;
		}
	}

	if(protect){
	 s="Protected: ";
	 s+=protect2;
	 s+="/";
	 s+=protect;
	 vli=new VListItem(vlis);
	 vli->type=litObjHeader;
	 vli->value=num;
	 vli->text=s;
	}
	if(capacity){
	 s="Capacity: ";
	 s+=cap2;
	 s+="/";
	 s+=capacity;
   vli=new VListItem(vlis);
   vli->type=litObjHeader;
   vli->value=num;
   vli->text=s;
  }
  int sailors=Sailors();
  if(sailors){
   s="Sailors: ";
   s+=cursailors;
   s+="/";
   s+=sailors;
   vli=new VListItem(vlis);
   vli->type=litObjHeader;
   vli->value=num;
   vli->text=s;
  }
  if(owner)
  {
    s="Owner: ";
    s+=owner->FullName(false);
    vli=new VListItem(vlis);
    vli->type=litObjUnit;
    vli->value=owner->num;
    vli->text=s;
  }
 }
 AnsiString tab;
 if(num)tab="  ";
 foreach(units){
  AUnit *un=*(AUnit**)iter;
  s=tab+un->Print();
  vli=new VListItem(vlis);
  vli->type=litObjUnit;
  vli->value=un->num;
  vli->text=s;
 }
}

void AObject::MakeTree(TTreeNode * parnode, VTreeNodes * vtns)
{
  AnsiString str;
  VTreeNode *vtn;
  TTreeNode *node;
  if(num!=0)
  {
	str = ObjectHeader(false);
	vtn = new VTreeNode(vtns);
	vtn->type = tntObject;
	vtn->value = num;
	node = parnode->Owner->AddChildObject(parnode,str,vtn);
	vtn->node = node;
  } else {
	node = parnode;
  }
  TV_ITEM tv;
  tv.hItem = node->ItemId;
  tv.mask = TVIF_STATE;
  tv.stateMask = TVIS_BOLD;
  tv.state = 0;
  TreeView_SetItem(node->Handle,&tv);

  foreach(units)
  {
	AUnit *unit=*(AUnit**)iter;
	if(AtlaForm->NeedUnitInTree(unit)) unit->MakeTree(node,vtns);
  }
}

void AObject::CreateNew(AObject * prvobj)
{
 if(!prvobj){
  return;
 }
 UpdateFromPrev(prvobj);
}
void AObject::Update(AObject * newobj)
{
 name=newobj->name;
 describe=newobj->describe;
 inner=newobj->inner;
 if(inner)
   if(newobj->innerloc.xloc!=-1)
     innerloc=newobj->innerloc;
 num=newobj->num;
 type=newobj->type;
 incomplete=newobj->incomplete;
 runes=newobj->runes;
 units->Update(newobj->units);
}
void AObject::UpdateFromPrev(AObject * prvobj)
{
 name=prvobj->name;
 describe=prvobj->describe;
 inner=prvobj->inner;
 if(inner)
   if(innerloc.xloc==-1)
     innerloc=prvobj->innerloc;
 num=prvobj->num;
 type=prvobj->type;
 if(prvobj->incomplete)incomplete=-1;
 else incomplete=0;
 runes=prvobj->runes;
}
void AObject::Read(TFile & in)
{
 in>>num;
 if(num){
  in>>name;
  in>>describe;
  in>>inner;
  if(BaseVer>=27)
  {
    if(inner)
      innerloc.Read(in);
  }
  in>>type;
  in>>incomplete;
  in>>runes;
 }
 units->Read(in);
}
void AObject::Write(TFile & out)
{
 out<<num;
 if(num){
  out<<name;
/*  int dlen=describe.Length();
  if(dlen){
   if(describe[dlen]=='.')describe.SetLength(dlen-1);
  }*/
  out<<describe;
  out<<inner;
  if(inner)
    innerloc.Write(out);
  out<<type;
  out<<incomplete;
  out<<runes;
 }
 units->Write(out);
}


bool AObject::IsTower()
{
 if(num==0) return false;
 AObjectType *otype=ObjectTypes->Get(type);
 if(otype->protect) return true;
/* if(!otype->canenter) return false;
 if(otype->sailors) return false;
 if(!otype->production.IsEmpty()) return false;*/
 return false;
}
bool AObject::IsBuilding()
{
 if(num==0) return false;
 AObjectType *otype=ObjectTypes->Get(type);
 if(otype->protect) return false;
 if(!otype->canenter) return false;
 if(otype->sailors) return false;
 if(otype->road) return false;
 if(inner) return false;
 return true;
}
bool AObject::IsLair()
{
 if(num==0) return false;
 AObjectType *otype=ObjectTypes->Get(type);
 return !otype->canenter;
}
bool AObject::IsShip()
{
 if(num==0) return false;
 AObjectType *otype=ObjectTypes->Get(type);
 return otype->sailors;
}
int AObject::IsRoad()
{
 if(num==0) return false;
 AObjectType *otype=ObjectTypes->Get(type);
 return otype->road;
}
bool AObject::Destroyable()
{
 if(num==0) return false;
 AObjectType *otype=ObjectTypes->Get(type);
 return otype->cost!=-1;
}


void AObject::AddUnit(AUnit* unit)
{
 units->Add(unit);
}
AUnit * ARegionList::GetUnit(int num)
{
 foreach(this){
  ARegion *reg=*(ARegion**)iter;
  AUnit *un=reg->GetUnit(num);
  if(un)return un;
 }
 return 0;
}
bool AObject::PresentLocals()
{
 foreach(units){
  AUnit *un=*(AUnit**)iter;
  bool isloc=un->IsLocal();
  if(isloc)return true;
 }
 return false;
}
bool ARegion::PresentLocals()
{
 foreach(this){
  AObject *obj=*(AObject **) iter;
  bool presloc=obj->PresentLocals();
  if(presloc)return true;
 }
 return false;
}
bool ARegion::CanTax(AUnit * u)
{
 foreach(this){
  AObject *obj=*(AObject **) iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(un->endguard==GUARD_GUARD && un->IsAlive())
    if(un->GetAttitude(this,u)<=A_NEUTRAL)
     return false;
  }
 }
 return true;
}
void ARegion::RunOrders(bool immed,RunStop rs)
{
 if(rs!=rsNoStop)
   immed=true;
 if(immed==false&&runned) return;
 runned=false;
 BeginEvents();

//begin order processing
 if(!PrepareOrders()) return; //if no units
 PrepareOrders2();
 RunPluginOrders(PO_RUNBEFOREFLAGS);
 RunFlagOrders();
 RunEnterLeaveOrders();
 if(rs==rsAfterEnter) return;
 RunPromoteOrders();

 RunAttackOrders();
 if(!RunStealOrders()) return;
 RunPluginOrders(PO_RUNBEFOREGIVE);
 if(!RunGiveOrders())return;
 if(rs==rsAfterGive) return;
 RunPluginOrders(PO_RUNAFTERGIVE);
// RunDestroyOrders(); //verify
 if(!RunPillageOrders())
   RunTaxOrders();
 RunPluginOrders(PO_RUNAFTERTAX);
 RunGuard1Orders();
 RunCastOrders();
 RunPluginOrders(PO_RUNAFTERCAST);
 VerifyBuySellOrders();
 if(!RunSellOrders())return;
 if(!RunBuyOrders())return;
 if(rs==rsAfterBuy) return;
 RunPluginOrders(PO_RUNAFTERBUY);
 if(!RunForgetOrders())return;
 RunPluginOrders(PO_RUNAFTERFORGET);
 RunWithdrawOrders();
 RunPluginOrders(PO_RUNAFTERWITHDRAW);
 PrepareConsume();
 if(rs==rsAfterPrepareConsume) return;
 RunSailOrders();
 RunPluginOrders(PO_RUNAFTERSAIL);
 RunMoveOrders();
 RunPluginOrders(PO_RUNAFTERMOVE);
 if(!RunTeachOrders())return;
 RunStudyOrders();
 RunWorkOrders();
 RunEntertainOrders();
 RunProduceOrdersA();
 RunProduceOrders();
 if(rs==rsBeforeBuild) return;
 RunBuildOrders();
 RunPluginOrders(PO_RUNAFTERMONTHLONG);


// RunTeleportOrders();
 RunConsumeOrders();
//end order processing
 foreach(this){
  AObject *obj=*(AObject **) iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   un->enditems->DeleteEmpty();
   un->endskills->DeleteEmpty();
   un->enditems->Sort();
   un->UpdateWeight();
  }
 }
 RunPluginOrders(PO_RUNAFTERRUN);
 runned=true;
}
bool ARegion::RunPillageOrders()
{
 AnsiString mes;
 int t=0;
 AOrders *ords=new AOrders(0);
 ords->autodelete=false;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   AOrder *ord=u->orders->Find(O_PILLAGE,2);
   if(!ord)continue;
   if(money<1){
    mes="Error: PILLAGE: ";
    mes+=u->FullName();
    mes+=" no money";
    AddEvent(mes);
    return false;
   }
   if(wages<11){
    mes="Error: PILLAGE: ";
    mes+=u->FullName();
    mes+=" poor wages";
    AddEvent(mes);
    return false;
   }
   if(!CanTax(u)){
    AddEvent("Error: "+u->FullName()+" PILLAGE: A unit is on guard.");
   }else{
    int men=u->Taxers();
    if(men){
     t+=men;
     ords->Add(ord);
    }else{
     mes="Error: PILLAGE: ";
     mes+=u->FullName();
     mes+=" cannot tax";
     AddEvent(mes);
    }
   }
  }
 }
 if(!t){ delete ords;return false;}
 if(t<money/2/TaxEffect){
  mes=ords->Get(0)->orders->unit->FullName();
  mes+=" PILLAGE: Not enough men to pillage (";
  mes+=t;
  mes+="/";
  mes+=money/2/TaxEffect;
  mes+=")";
  AddEvent(mes);
  delete ords;
  return false;
 }
 ords->Distribute(2*money,-t);

 delete ords;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   AOrder *ord=u->orders->Find(O_TAX,2);
   if(!ord)continue;
   mes="Error: TAX: ";
   mes+=u->FullName();
   mes+=" region pillaged";
   AddEvent(mes);
  }
 }
 return true;
}

void ARegion::RunTaxOrders()
{
 AnsiString mes;
 int t=0;
 AOrders *ords=new AOrders(0);
 ords->autodelete=false;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   AOrder *ord=u->orders->Find(O_TAX,2);
   if(!ord)continue;
   if(!CanTax(u)){
    if(u->IsLocal()) AddEvent("Error: "+u->FullName()+" TAX: A unit is on guard.");
   }else{
    int men=u->Taxers();
    if(men){
     t+=men;
     ords->Add(ord);
    }else{
      mes="Error: TAX: ";
      mes+=u->FullName();
      mes+=" cannot tax.";
      AddEvent(mes);
    }
   }
  }
 }
 taxers=t;
 if(!taxers){ delete ords;return;}
 int curincome=taxincome;
 if(!curincome) curincome=TaxEffect;
 curtaxprod=curincome;
 {
  int inc=money/taxers;
  if(inc<curincome&&!(optDontShowEvents&dseIncome)){
   mes="Warning: tax income = ";
   mes+=inc;
   mes+=" in "+FullName();
   AddEvent(mes);
  }
 }
 ords->Distribute(money,taxers*curincome);
 delete ords;
}
bool ARegion::HasLocalTaxers()
{
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   if(!u->IsLocal())continue;
   if(u->HasTaxOrder())return true;
  }
 }
 return false;
}
bool ARegion::HasLocalTraders()
{
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   if(!u->IsLocal())continue;
   if(u->HasTradeOrder())return true;
  }
 }
 return false;
}
void ARegion::RunStudyOrders()
{
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   if(!u->IsLocal())continue;
   AOrderAbr *ord=dynamic_cast<AOrderAbr*>(u->orders->Find(O_STUDY,2));
   if(!ord)continue;
   ord->Run(0);
  }
 }
}
void ARegion::RunAttackOrders()
{
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   foreach(u->orders){
    AOrder *o=*(AOrder**)iter;
    if(o->commented) continue;
    if(o->type!=O_ATTACK) continue;
    o->Run(0);
   }
  }
 }
}
bool ARegion::RunStealOrders()
{
 bool notedited=true;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   foreachr(u->orders){
    AOrder *o=*(AOrder**)iter;
    if(o->commented) continue;
    if(o->type!=O_STEAL&&o->type!=O_ASSASSINATE) continue;
    if(o->isedited)
      notedited=false;
    else
      o->Run(0);
    break;
   }
  }
 }
 return notedited;
}
bool ARegion::RunGiveOrdersForUnit(AUnit *un)
{
  foreach(un->orders){
   AOrder *o=*(AOrder**)iter;
   if(o->type!=O_GIVE)continue;
   if(o->isedited)
     return false;
   AOrderGive *ord=dynamic_cast<AOrderGive*>(o);
   ord->Run(0);
  }
  return true;
}
bool ARegion::RunGiveOrders()
{
 AnsiString mes;
 std::auto_ptr<AUnits> uns(new AUnits(0));
 uns->autodelete=false;
 //поиск перемещенных юнитов и запись их в uns
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   if(u->endobj==o) continue;
   uns->Add(u);
  }
 }
 foreach(this){
  AObject *o=*(AObject **) iter;
  //обработка неперемещенных юнитов
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   if(u->endobj!=o) continue;
   if(!RunGiveOrdersForUnit(u))
     return false;
  }
  //обработка перемещенных юнитов
  for(int i=0;i<uns->count;i++)
  {
   AUnit *u=uns->Get(i);
   if(u->endobj!=o) continue;
   if(!RunGiveOrdersForUnit(u))
     return false;
   uns->DeleteByIndex(i);
   i--;
  }
 }
 if(uns->count)
   AddEvent("INTERNAL ERROR in GIVE: please contact to author");
 return true;
}


bool ARegion::PrepareOrders()
{
 runned=false;
 AnsiString mes;
 bool hasunit=false;
// hasLocal=false;
// hasLocalTaxers=false;
/* if(!hasInfo){  //not needed
// if(!(infoStatus&isWeatherInfo)){*/
 {
  int turnnum=turn->num;
  if(!weather.Length())
   weather=WeatherRegions->GetWeather(xloc,yloc,zloc,turnnum);
  if(!nxtweather.Length())
   nxtweather=WeatherRegions->GetWeather(xloc,yloc,zloc,turnnum+1);
 }
 foreach(this){
  AObject *o=*(AObject **) iter;
  o->endreg=this;//o->basereg;
  o->endincomplete=o->incomplete;
  if(o->units->count)
    o->owner=o->units->Get(0);
  else
    o->owner=0;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   u->PrepareOrders();
   hasunit=true;
//   if(u->IsLocal()) hasLocal=true;
  }
 }
 claims.clear();
 return hasunit;
}
void ARegion::PrepareOrders2()
{
 AnsiString mes;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   u->HasTaxOrder();
   AOrder *monthlong=0;
   AOrder *castorder=0;
   AOrder *spyorder=0;
   bool warned=false;
   foreach(u->orders){
    AOrder *ord=*(AOrder**)iter;
    if(ord->commented) continue;
    if(ord->type==O_CAST){
     if(castorder){
      mes="Error: too many CAST orders for ";
      mes+=u->FullName();
      AddEvent(mes);
      castorder->commented=true;
     }
     castorder=ord;
    }
    if(ord->type==O_STEAL||ord->type==O_ASSASSINATE){
     if(spyorder){
      mes="Error: too many spy orders for ";
      mes+=u->FullName();
      AddEvent(mes);
      spyorder->commented=true;
     }
     spyorder=ord;
    }
    if(ord->IsMonthLong()){
     if(monthlong){
      if(ord->type==O_TEACH&&monthlong->type==O_TEACH){
//      }else if(o->type==O_MOVE&&monthlong->type==O_MOVE){
//      }else if(o->type==O_ADVANCE&&monthlong->type==O_ADVANCE){
//      }else if(o->type==O_SAIL&&monthlong->type==O_SAIL){
      }else{
       if(!warned){
        mes="Error: too many monthlong orders for ";
        mes+=u->FullName();
        AddEvent(mes);
        warned=true;
       }
       if(monthlong->type!=O_NOP)
        monthlong->commented=true;
      }
     }
     monthlong=ord;
    }
    ord->Prepare();
/*    if(o->type==O_STUDY){
     AOrderAbr *ord=dynamic_cast<AOrderAbr*>(o);
     ord->number=30;
    }
//    if(o->type!=O_GIVE)continue;
//    if(o->isedited)continue;
    //throw Exception("Bad give orders");
    AOrderGive *ord=dynamic_cast<AOrderGive*>(o);
//    if(!ord)continue;
    if(!ord->target){
     if(ord->tarnum)ord->target=GetUnit(ord->tarnum);
     if(ord->commented)continue;
     if(!ord->target){
      mes="Error ";
      mes+=u->FullName();
      mes+=" GIVE: unit ";
      AUnit *un=new AUnit(0);
      un->num=ord->tarnum;
      mes+=un->Alias();
      delete un;
      mes+=" not found";
      AddEvent(mes);
      continue;
     }
    }
    if(ord->ordrec){
     if(ord->commented){
      ord->DeleteOrdRec();
     }
     continue;
    }
    if(!ord->commented)
       ord->SetMessage();

//     ord->target->orders->Add(new AOrderReceived(ord));*/
   }
   if(!monthlong&&u->IsLocal()&&u->faction->local){
    if(!(optDontShowEvents&dseNoMonth)&&
       u->GetLocalDescr()!="empty"){
     mes="Error: no monthlong orders by ";
     mes+=u->FullName();
     AddEvent(mes);
    }
   }
  }
 }
}
void ARegion::RunCastOrders(){
 AnsiString mes;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   AOrder *ord=u->orders->Find(O_CAST,2);
   if(!ord) continue;
   ord->Run(0);
  }
 }
}
void ARegion::VerifyBuySellOrders(){
 AnsiString mes;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   foreach(u->orders){
    AOrder *_ord=*(AOrder**)iter;
    if(_ord->commented) continue;
    AOrderBuySell *ord=dynamic_cast<AOrderBuySell*>(_ord);
    if(!ord) continue;
    if(!ord->item.Length()) continue;
    AMarkets *marks=ord->type==O_BUY?saleds:wanteds;
    AMarket *mark=marks->GetMarket(ord->item);
    if(mark) continue;
    mes="Error: ";
    mes+=u->FullName();
    mes+=" attempt ";
    mes+=ord->type==O_BUY?"BUY":"SELL";
    mes+=" ";
    mes+=ItemTypes->AbrToName(ord->item);
    mes+=" with no market";
    AddEvent(mes);
   }
  }
 }
}

bool ARegion::RunSellOrders()
{
 bool notedited=true;
 foreach(wanteds){
  AMarket *mark=*(AMarket**)iter;
  if(!RunBuySellOrders(O_SELL,mark))
   notedited=false;
 }
 return notedited;
}
bool ARegion::RunBuyOrders()
{
 bool notedited=true;
 foreach(saleds){
  AMarket *mark=*(AMarket**)iter;
  if(!RunBuySellOrders(O_BUY,mark))
   notedited=false;
 }
 return notedited;
}
bool ARegion::RunBuySellOrders(int type,AMarket *mark)
{
 AnsiString mes;
 int attempted=0;
 AOrders *ords=new AOrders(0);
 ords->autodelete=false;
 AnsiString item=mark->type->abr;
 int price=mark->price;
 bool notedited=true;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   foreach(u->orders){
    AOrder *_ord=*(AOrder**)iter;
    if(_ord->type!=type)continue;
    if(_ord->commented)continue;

    AOrderBuySell *ord=dynamic_cast<AOrderBuySell*>(_ord);
    if(ord->item!=item)continue;
    if(ord->isedited){
     notedited=false;
     break;
    }
    int mod=ord->amount;
    if(type==O_BUY){
     int money=u->GetMoney();
     if(mod<0)
     {
			 mod=money/price;
       if(mod>mark->amount)
         mod=mark->amount;
     }
     if(price*mod>money){
      mes="Warning: ";
      mes+=u->FullName();
      mes+=" attempts to buy too many items : needed $";
      mes+=mod*price-money;
      AddEvent(mes);
      mod=money/price;
     }
    }else{
     int kolvo=u->enditems->GetNum(item);
     if(mod<0)
     {
       mod=kolvo;
       if(mod>mark->amount)
         mod=mark->amount;
     }
     if(kolvo<mod){
      mes="Warning: ";
      mes+=u->FullName();
      mes+=" attempts to sell too many items : needed ";
      mes+=mod-kolvo;
      mes+=" ";
      mes+=ItemTypes->AbrToName(item);
      AddEvent(mes);
      mod=kolvo;
     }
    }
    ord->number=mod;
    attempted+=mod;
    ords->Add(ord);
    break;
   }
  }
 }
 if(!attempted){ delete ords;return notedited;}
 if(attempted>mark->amount){
  if(type==O_BUY)
   mes="Warning: attempts to buy too many ";
  else
   mes="Warning: attempts to sell too many ";
  mes+=mark->type->names;//ItemTypes->AbrToNames(mark->type);
  mes+=" in ";
  mes+=FullName(false);
  mes+=" (";
  mes+=attempted;
  mes+="/";
  mes+=mark->amount;
  mes+=")";
  AddEvent(mes);
 }
// bool b=
 ords->Distribute(mark->amount,attempted);
 delete ords;
 return notedited;
}
void ARegion::RunEntertainOrders()
{
 AnsiString mes;
 int t=0;
 AOrders *ords=new AOrders(0);
 ords->autodelete=false;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   AOrder *ord=u->orders->Find(O_ENTERTAIN,2);
   if(!ord)continue;
   int prod=ord->Productivity();
   if(prod){
    t+=prod;
    ords->Add(ord);
   }
  }
 }
 entattempt=t;
 if(!t){ delete ords;return;}
 if(entertainment<t&&!(optDontShowEvents&dseIncome)){
  mes="Warning: entertain ";
  mes+=t;
  mes+="/";
  mes+=entertainment;
  mes+=" in "+FullName(false);
  AddEvent(mes);
 }
 ords->Distribute(entertainment,t);
 delete ords;
}
void ARegion::RunWorkOrders()
{
 AnsiString mes;
 int t=0;
 AOrders *ords=new AOrders(0);
 ords->autodelete=false;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   AOrder *ord=u->orders->Find(O_WORK,2);
   if(!ord)continue;
   int prod=ord->Productivity();
   if(prod){
    t+=prod;
    ords->Add(ord);
   }
  }
 }
 workers=0;
 if(!t){ delete ords;return;}
 int wi=workincome;
 if(wi==0) wi=wages;
 workers=t/wi;
 if(maxwages<t&&!(optDontShowEvents&dseIncome)){
  mes="Warning: work ";
  mes+=maxwages*wi/t;
  mes+="/";
  mes+=wages;
  mes+=" in "+FullName(false);
  AddEvent(mes);
 }
 ords->Distribute(maxwages,t);
 delete ords;
}
void ARegion::RunFlagOrders()
{
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   foreach(u->orders){
    AOrder *_ord=*(AOrder**)iter;
    if(_ord->commented) continue;
    if(_ord->type==O_CLAIM){
     _ord->Run(0);
    }else if(_ord->type==O_WITHDRAW){
     _ord->Run(1);
    }else{
     AOrderByte *ord=dynamic_cast<AOrderByte *>(_ord);
     if(!ord) continue;
//     if(ord->type==O_GUARD&&ord->value)continue;
     ord->Run(0);
    }
   }
  }
 }
}
void ARegion::PrepareConsume()
{
 foreach(this){
  AObject *o=*(AObject **) iter;
  o->cursailors=0;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   u->HasTaxOrder();
   u->UpdateWeight();
   u->movepoints=0;
   if(!u->IsLocal())continue;
   if(!u->faction->local) continue;
   u->needed=u->CalcNeeded();
  }
 }
}
void ARegion::CheckUnitMaintenanceItem(AUnits *units,AnsiString item, int value, bool consume)
{
 foreach(units){
  AUnit *u=*(AUnit**)iter;
  if(!u->needed)continue;
  if(consume&&!u->GetEndFlag(FLAG_CONSUMING))continue;
  int amount=u->enditems->GetNum(item);
  if(amount){
   u->needed-=amount*value;
   if(u->needed<0){
    u->enditems->SetNum(item,-(u->needed/value));
    LogItem(2,-(u->needed/value)-amount,item,u);
    u->needed=0;
   }else{
    u->enditems->SetNum(item,0);
    LogItem(2,-amount,item,u);
   }
  }
 }
}

int ARegion::CheckFactionMaintenanceItem(AUnits *units,AnsiString item, int value, bool consume)
{
 AnsiString namein,nameout,s,itemname=ItemTypes->AbrToName(item),itemnames=ItemTypes->AbrToNames(item);
 int borrowed=0,num;
 AOrderMessage *mes;

 foreach(units){
  AUnit *u=*(AUnit**)iter;
  if(!u->needed)continue;
  if(u->endobj->endreg!=this) continue;
  if(consume&&!u->GetEndFlag(FLAG_CONSUMING_FACTION))continue;
  namein=u->FullName();
  foreach(units){
   AUnit *u2=*(AUnit**)iter;
   if(u==u2||u->endfaction!=u2->endfaction)continue;
   if(u2->endobj->endreg!=this) continue;
   int amount=u2->enditems->GetNum(item);
   if(amount){
    nameout=u2->FullName();
    if(borrowed==0) borrowed=u->num;
    u->needed-=amount*value;
    num=amount;
    if(u->needed<0){
     num+=(u->needed/value);
     u2->enditems->SetNum(item,-(u->needed/value));
     LogItem(2,-num,item,u2);
     u->needed=0;
    }else{
     u2->enditems->SetNum(item,0);
     LogItem(2,-num,item,u2);
    }
    mes=new AOrderMessage(0);
    s=namein+" borrows ";
    if(num!=1){
     s+=num;
     s+=" "+itemnames;
    }else{
     s+=itemname;
    }
    s+=" for maintenance";
    mes->text=s;
    mes->target=u;
    mes->subtype=OM_BORROWSFROM;
    mes->svalue1=item;
    mes->ivalue1=num;
    u2->orders->Add(mes);

    mes=new AOrderMessage(0);
    s="Borrows ";
    if(num!=1){
     s+=num;
     s+=" "+itemnames;
    }else{
     s+=itemname;
    }
    s+=" from "+nameout;
    s+=" for maintenance";
    mes->text=s;
    mes->target=u2;
    mes->subtype=OM_BORROWSTO;
    mes->svalue1=item;
    mes->ivalue1=num;
    u->orders->Add(mes);
   }
   if(u->needed==0)break;
  }
 }
 return borrowed;
}

int ARegion::CheckFactionSharedItems(AUnit *tekU, AnsiString item, int value, int typeS)
{
  AnsiString nameout, s, itemname=ItemTypes->AbrToName(item), itemnames=ItemTypes->AbrToNames(item);
  AnsiString textMes;
  AnsiString namein = tekU->FullName();

  switch(typeS){
    case 0: textMes = " for studying "; break;
    case 1: textMes = " for production "; break;
  }

  int num, needed = value;
  AOrderMessage *mes;

  foreach(this){
    if(needed == 0)break;
    AObject *o=*(AObject **) iter;
    foreach(o->units){
      AUnit *un=*(AUnit**)iter;
      if(!un->IsLocal()) continue;
      if(!un->faction->local) continue;
      if(un == tekU) continue;
	  if(!(un->endflags&FLAG_SHARING)) continue;
	  if(un->endobj->endreg != un->baseobj->basereg) continue;

	  int existn = un->enditems->GetNum(item);

      if(existn > 0){
        // Есть у него такие....
        nameout = un->FullName();
        if(existn < needed){
          // есть немного....
          needed -= existn;
          num = existn;
          tekU->enditems->SetNum(item, 0); // все забрали...
          LogItem(2, -num, item, un); // ????
        } else { // Хватает с гаком
          num = needed;
          un->enditems->SetNum(item, existn-num);
          LogItem(2, -num, item, un);
          needed = 0;
        }

        mes = new AOrderMessage(0);
        s = namein + " borrows ";
        if(num !=1 ){
          s += num;
          s += " " + itemnames;
        } else {
          s += itemname;
        }
        s += textMes;
        mes->text = s;
        mes->target = tekU;
        mes->subtype = OM_BORROWSFROM;
        mes->svalue1 = item;
        mes->ivalue1 = num;
        un->orders->Add(mes);

        mes = new AOrderMessage(0);
        s = "Borrows ";
        if(num != 1){
          s += num;
          s += " " + itemnames;
        } else {
          s += itemname;
        }
        s += " from " + nameout;
        s += textMes;
        mes->text = s;
        mes->target = un;
        mes->subtype = OM_BORROWSTO;
        mes->svalue1 = item;
        mes->ivalue1 = num;
        tekU->orders->Add(mes);
      }
      if(needed == 0)break;
    }
  }
  return (value - needed);
}

void ARegion::RunConsumeOrders()
{
 AnsiString mes;
 AUnits *uns=new AUnits(0);
 uns->autodelete=false;

 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   if(!u->IsLocal())continue;
   if(!u->faction->local)continue;
   int mens=u->GetSoldiers();
   if(!mens){
    if(u->GetLocalDescr()!="empty"){
     mes="Warning: Unit ";
     mes+=u->FullName();
     mes+=" is empty";
     AddEvent(mes);
    }
   }
   uns->Add(u);
  }                 
 }
 foreach(ItemTypes){
  AItemType *itype=*(AItemType**)iter;
  if(!(itype->type&AItemType::IT_FOOD))continue;
  CheckUnitMaintenanceItem(uns,itype->abr,FoodMaintKoeff,true);
 }
 int borrowed=0,bor2;
 foreach(ItemTypes){
  AItemType *itype=*(AItemType**)iter;
  if(!(itype->type&AItemType::IT_FOOD))continue;
  bor2=CheckFactionMaintenanceItem(uns,itype->abr,FoodMaintKoeff,true);
  if(borrowed==0)borrowed=bor2;
 }

 CheckUnitMaintenanceItem(uns,"SILV",1,false);
 bor2=CheckFactionMaintenanceItem(uns,"SILV",1,false);
 if(borrowed==0) borrowed=bor2;

 foreach(ItemTypes){
  AItemType *itype=*(AItemType**)iter;
  if(!(itype->type&AItemType::IT_FOOD))continue;
  CheckUnitMaintenanceItem(uns,itype->abr,FoodMaintKoeff,false);
 }

 foreach(ItemTypes){
  AItemType *itype=*(AItemType**)iter;
  if(!(itype->type&AItemType::IT_FOOD))continue;
  bor2=CheckFactionMaintenanceItem(uns,itype->abr,10,false);
  if(borrowed==0) borrowed=bor2;
 }


 foreach(uns){
  AUnit *u=*(AUnit**)iter;
  if(!u->needed)continue;
  mes="Warning: Unit ";
  mes+=u->FullName();
  mes+=" needs ";
  mes+=u->needed;
  mes+=" silver for maintenance";
  AddEvent(mes);
  borrowed=0;
 }
 delete uns;
 if(borrowed&&!(optDontShowEvents&dseBorrows)){
  mes="Borrows ";
  AUnit *un=GetUnit(borrowed);
  if(un){
   mes+=un->FullName();
  }else{
   mes+=GetAlias(borrowed,0);
  }
  mes+=" in ";
  mes+=FullName(false);
  AddEvent(mes);
 }
}


bool ARegion::RunForgetOrders()
{
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   foreach(u->orders){
    AOrder *ord=*(AOrder**)iter;
    if(ord->type!=O_FORGET)continue;
    if(ord->isedited)return false;
    ord->Run(0);
   }
  }
 }
 return true;
}
void ARegion::RunWithdrawOrders()
{
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   foreach(u->orders){
    AOrder *ord=*(AOrder**)iter;
    if(ord->type!=O_WITHDRAW)continue;
    ord->Run(0);
   }
  }
 }
}


bool ARegion::RunTeachOrders()
{
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   AOrderAbr *or=dynamic_cast<AOrderAbr*>(u->orders->Find(O_STUDY,2));
   if(!or)continue;
   or->number=StudyAddDays*u->GetMen();
  }
 }
 bool notfound=true;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   AOrderTeach *ot=new AOrderTeach();
   ot->orders=u->orders;
   ot->bases=new AOrders(u);
   ot->bases->autodelete=false;
   foreach(u->orders){
    AOrder *o=*(AOrder**)iter;
    if(o->type!=O_TEACH)continue;
    if(o->isedited){
      notfound=false;
      continue;
    }
    if(o->commented)continue;
    AOrderTeach *ord=dynamic_cast<AOrderTeach*>(o);
    int kol=ord->targets->Count;
    if(kol){
     ot->bases->Add(o);
    }
    for(int i=0;i<kol;i++){
     AUnitPtr *ptr=ord->GetTarget(i);
     ot->AddTarget(ptr->num,ptr->num<0?0:ptr->ptr);
    }
   }
   if(ot->targets->Count)
    ot->Run(0);
   delete ot;
  }
 }
 return notfound;
}


void ARegion::RunProduceOrdersA()
{
 foreach(products){
  AMarket *mark=*(AMarket**)iter;
  RunProduceOrdersA(mark);
 }
}
void ARegion::RunProduceOrdersA(AMarket *mark)
{
 AnsiString mes;
 AOrders *ords=new AOrders(0);
 ords->autodelete=false;
 int attempted=0;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
	 AUnit *u=*(AUnit**)iter;
	 if(u->endobj->endreg!=this) continue;
   AOrderAbr *or=dynamic_cast<AOrderAbr*>(u->orders->Find(O_PRODUCE,2));
   if(!or)continue;
   if(or->abr!=mark->type->abr)continue;
   int att=or->Productivity();
   if(att>0){
    attempted+=att;
    ords->Add(or);
   }else if(att==0){
    mes=u->FullName();
    mes+=" PRODUCE: Unit isn't skilled enough";
    AddEvent(mes);
   }
  }
 }
 mark->price=attempted; 
 if(attempted)
 if(mark->amount){
  ords->Distribute(mark->amount,attempted);
 }else{
  mes="Can't produce ";
  mes+=mark->type->name;//ItemTypes->AbrToName(mark->type);
  mes+=" in ";
  mes+=FullName(false);
  AddEvent(mes);
 }
 delete ords;
}


void ARegion::RunProduceOrders()
{
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   AOrderAbr *or=dynamic_cast<AOrderAbr*>(u->orders->Find(O_PRODUCE,2));
   if(!or)continue;
   or->Run(-1);
  }
 }
}


bool ARegion::CanMakeAdv(AFaction * fac, AnsiString item)
{
 AItemType *itype=ItemTypes->Get(item,4);
 if(!itype->skill.Length()) return true;
 AnsiString skill=itype->skill;
 int level=itype->level;
 foreach(this){
  AObject *obj=*(AObject **) iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(un->endfaction!=fac)continue;
   if(un->GetBegSkill(skill)>=level)return true;
  }
 }
 return false;
}




void ARegion::RunEnterLeaveOrders()
{
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   foreach(u->orders){
    AOrder *_ord=*(AOrder**)iter;
    if(_ord->commented) continue;
    if(_ord->type==O_LEAVE||_ord->type==O_ENTER){
     _ord->Run(0);
    }
   }
  }
  if(o->owner) continue;
  foreach(this){
   AObject *o2=*(AObject **) iter;
   foreach(o2->units){
    AUnit *u=*(AUnit**)iter;
    if(u->endobj!=o) continue;
    o->owner=u;
    break;
   }
   if(o->owner) break;
   if(o2==o) break;
  }
 }
}
void ARegion::RunPromoteOrders()
{
 foreach(this){
  AObject *o=*(AObject **) iter;
  AUnit *un=o->GetOwner();
  if(!un) continue;
  AOrder *ord=un->orders->Find(O_PROMOTE,2);
  if(!ord) continue;
  ord->Run(0);
 }
}

bool ARegion::IsCoastal()
{
 if(TerrainTypes->IsOcean(type)) return 1;
 bool noexits=true;
 for(int i=0;i<NDIRS;i++){
  if(neighbors[i].xloc==-1) continue;
  noexits=false;
  ARegion *reg=turn->RegionList->Get(neighbors[i]);
  if(!reg) continue;
  if(TerrainTypes->IsOcean(reg->type)) return true;
 }
 return noexits;
}


int ARegion::MoveCost(int movetype)
{
 int mult=2;
 if(optConquest)
   mult=1;
 else
 {
   AWeatherType *wtype=WeatherTypes->Get(NxtWeather());
   if(wtype) mult=wtype->movemult;
 }
 if(movetype==M_SAIL)
   return mult;
 if(movetype==M_WALK||movetype==M_RIDE){
  ATerrainType *ttype=TerrainTypes->Get(type);
  return ttype->movepoints*mult;
 }
 return mult;
}


AnsiString ARegion::NxtWeather()
{
 if(hasInfo&&oldinfoturnnum==turn->num) return nxtweather;
 return WeatherRegions->GetWeather(xloc,yloc,zloc,curTurnNumber+1);
}
void ARegion::RunGuard1Orders()
{
 AnsiString mes;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   if(u->endguard!=GUARD_GUARD&&u->endguard!=GUARD_SET) continue;

   if(u->IsLocal()&&u->Taxers()==0){
    u->endguard=GUARD_NONE;
    mes="Error ";
    mes+=u->FullName();
    mes+=": Must be combat ready to be on guard";
    AddEvent(mes);
    continue;
   }
   u->endguard=GUARD_GUARD;
  }
 }
}
void ARegion::RunSailOrders()
{
 AnsiString mes;
 foreach(this){
  AObject *o=*(AObject **) iter;
  if(o->IsShip()){
   AUnit *u=o->GetOwner();
   AOrder *ord=0;
   if(u&&u->endobj==o){
    ord=u->orders->Find(O_SAIL,2);
   }
   if(ord){
    foreach(u->orders){
     AOrder *o=*(AOrder **)iter;
     if(o->commented) continue;
     if(o->type!=O_SAIL) continue;
     o->Run(-1);
    }
//    ord->Run(-1);
    continue;
   }
  }
  foreach(o->units){
   AUnit *un=*(AUnit**) iter;
   if(un->endobj->owner==un) continue;
   AOrder *ord=un->orders->Find(O_SAIL,2);
   if(ord){
    un->movepoints=0;
    mes="Error SAIL: ";
    mes+=un->FullName();
    mes+=" must be ship owner";
    AddEvent(mes);
   }
  }
 }
}
void ARegion::RunMoveOrders()
{
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   foreach(u->orders){
    AOrder *ord=*(AOrder**)iter;
    if(ord->commented) continue;
    if(ord->type!=O_MOVE&&ord->type!=O_ADVANCE) continue;
    ord->Run(0);
    if(ord->isedited) return;
   }
  }
 }
}
void ARegion::RunPluginOrders(int runtime)
{
 bool firstrun=runtime==PO_RUNBEFOREFLAGS;
 bool needsecondpass=false;
 int issecondpass=(runtime&PO_DOUBLEPASS)!=0;

 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   foreach(u->orders){
    AOrder *ord=*(AOrder**)iter;
    if(ord->type!=O_PLUGIN) continue;
    if(ord->commented) continue;
    AOrderPlugin *_ord=(AOrderPlugin*)ord;
    if(_ord->OrderType!=PO_NORMAL) continue;
    bool doublepass=false;
    if(firstrun)
    {
      _ord->runtime=0;
      if(_ord->VerifyPlugin())
      {
        int v=Plugins.GetOrderPluginFlag(_ord->ModuleNumber,_ord->OrderNumber);
        if((v&PO_TYPEMASK)==PO_NORMAL)
        _ord->runtime=(v&PO_RUNMASK);
        if(v&PO_DOUBLEPASS)
          doublepass=true;
      }
    }
    if(_ord->runtime!=runtime) continue;
    _ord->Run(issecondpass);
    if(issecondpass) continue;
    if(doublepass)
    {
      _ord->runtime|=PO_DOUBLEPASS;
      needsecondpass=true;
    }
   }
  }
 }
 if(needsecondpass)
   RunPluginOrders(runtime|PO_DOUBLEPASS);
}
void ARegion::RunPluginAutoOrders(int runtime)
{
 bool firstrun=runtime==PO_RUNBEFOREFLAGS;
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   foreach(u->orders){
    AOrder *ord=*(AOrder**)iter;
    if(ord->type!=O_PLUGIN) continue;
    if(ord->commented) continue;
    AOrderPlugin *_ord=(AOrderPlugin*)ord;
    if(_ord->OrderType!=PO_AUTO) continue;
    if(firstrun)
    {
      _ord->runtime=0;
      if(_ord->VerifyPlugin())
      {
        int v=Plugins.GetOrderPluginFlag(_ord->ModuleNumber,_ord->OrderNumber);
        if((v&PO_TYPEMASK)==PO_AUTO)
          _ord->runtime=(v&PO_RUNMASK);
      }
    }
    if(_ord->runtime!=runtime) continue;
    _ord->Run(0);
   }
  }
 }
}

int AObject::Protect()
{
 if(num==0) return 0;
 if(incomplete) return 0;
 AObjectType *ot=ObjectTypes->Get(type);
/* if(!ot)
  return 0;*/
 return ot->protect;
}
int AObject::Capacity()
{
 if(num==0) return 0;
 if(incomplete) return 0;
 AObjectType *ot=ObjectTypes->Get(type);
/* if(!ot)
  return 0;*/
 return ot->capacity;
}
int AObject::Sailors()
{
 if(num==0) return 0;
 if(incomplete) return 0;
 AObjectType *ot=ObjectTypes->Get(type);
/* if(!ot)
  return 0;*/
 return ot->sailors;
}


void ARegion::RunAutoOrders1()
{
 AOrders *ords=new AOrders(0);
 ords->autodelete=false;
 AnsiString mes;
 PrepareOrders();
 PrepareOrders2();
 RunPluginAutoOrders(PO_RUNBEFOREFLAGS);
 RunFlagOrders();
 RunEnterLeaveOrders();
// RunPromoteOrders();
 RunPluginAutoOrders(PO_RUNBEFOREGIVE);

 foreach(this){
  AObject *obj=*(AObject **) iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(!un->IsLocal()) continue;
   foreach(un->orders){
    AOrder *o=*(AOrder**)iter;
    if(o->type==O_AUTOWARNING)
    {
      o->Run(0);
      continue;
	}
	if(o->type!=O_GIVE)continue;
//    if(o->isedited)return false;
    AOrderGive *ord=dynamic_cast<AOrderGive*>(o);
    ord->Run(0);
   }
//autogive
   for(int i=0;i<un->orders->count;i++){
    AOrder *_ord=un->orders->Get(i);
	if(_ord->type==O_AUTOGIVE){
	 AOrderAutoGive *ord=dynamic_cast<AOrderAutoGive*>(_ord);
     int have=un->enditems->GetNum(ord->item);
     if(have==0) continue;
     int gived=0;
     foreach(this){
      AObject *obj2=*(AObject **) iter;
      if(!have) break;
      foreach(obj2->units){
       AUnit *un2=*(AUnit**)iter;
       if(un2==un) continue;
       if(!gived){
        have=un->enditems->GetNum(ord->item);
        if(ord->number==0){
         gived=have;
        }else if(ord->number<0){
         gived=have+ord->number;
         if(gived<=0){
          have=0;break;
         }
        }else{
         gived=ord->number;
         if(gived>have) gived=have;
        }
       }

       if(!gived) break;
       AnsiString name=un2->FullName2();
       if(ord->begstr.Length()){
        if(MyCompare(name.c_str(),ord->begstr)) continue;
       }
       if(ord->substr.Length()){
        if(name.Pos(ord->substr)==0) continue;
       }
       bool AddOrder=true;
       foreach(un->orders){
        AOrder *_ordg=*(AOrder**)iter;
        if(_ordg->type!=O_GIVE) continue;
        AOrderGive *ordg=dynamic_cast<AOrderGive*>(_ordg);
        if(ordg->target==un2&&ordg->item==ord->item){
         AddOrder=false;
         break;
        }
       }
       if(!AddOrder) continue;
       AOrderGive *ordg=new AOrderGive();
       ordg->target=un2;
       ordg->tarnum=un2->num;
       ordg->count=gived;
       ordg->item=ord->item;
       un->orders->Add(ordg);
       ordg->Run(0);
//       have=un->enditems->GetNum(ord->item)-gived;
//       un->enditems->SetNum(ord->item,have);
       mes=un->FullName();
       mes+=": AUTOGIVE give ";
       mes+=gived;
       mes+=" "+ItemTypes->AbrToNames(ord->item);
       mes+=" to "+un2->FullName();
       AddEvent(mes);
       have-=gived;
       gived=0;
      }
     }
	}
	if(_ord->type == O_AUTOFOLLOW)
	{
		// Auto Follow Order
		AOrderAutoFollow *ord=dynamic_cast<AOrderAutoFollow*>(_ord);
		ord->RunFollow();
		continue;
	}

   }
   {//AUTOMOVE
    ords->Clear();
    bool hasmove=un->orders->GetMonthLong();
    AOrderAutoMoving *curord=0;
    foreach(un->orders){
     AOrder* ord=*(AOrder**) iter;
     if(ord->commented) continue;
/*     if(ord->type==O_MOVE||ord->type==O_ADVANCE||ord->type==O_SAIL){
      hasmove=true;
      break;
     }*/
     if(ord->type!=O_AUTOMOVE) continue;
     ords->Add(ord);
     if(!curord&&ord->repeating)
      curord=dynamic_cast<AOrderAutoMoving*>(ord);
    }
    if(hasmove||!ords->count) continue;
    if(!curord){
     mes="AUTOMOVE Warning: ";
     mes+=un->FullName();
     mes+=" has no current automove order";
     AddEvent(mes);
     continue;
    }
    ALocation loc(xloc,yloc,zloc);
    int ind=curord->GetIndex(loc);
    if(ind<0){
     mes="AUTOMOVE Error: ";
     mes+=un->FullName();
     mes+=" has bad current automove order";
     AddEvent(mes);
     continue;
    }
    if(ind==curord->count){
     int i=ords->IndexOf(curord),endi=ords->count;
     curord->repeating=false;
     curord=0;
     for(int j=i+1;j!=i;j++){
      if(j>=endi) j=0;
      if(j==i) break;
      AOrderAutoMoving *ord=(AOrderAutoMoving*)ords->Get(j);
      ALocation bloc=ord->GetLoc(0);
      if(bloc.xloc!=loc.xloc||bloc.yloc!=loc.yloc) continue;
      curord=ord;
      break;
     }
     if(!curord){
      mes="AUTOMOVE Error: ";
      mes+=un->FullName();
      mes+=" has no valid next automove order";
      AddEvent(mes);
      continue;
     }
     curord->repeating=true;
     ind=0;
    }
    AOrderMoving *moveord=curord->CreateMoveOrder(ind);
    un->orders->Add(moveord);
    mes="AUTOMOVE: order add to ";
    mes+=un->FullName();
    AddEvent(mes);
   }
  }
 }
 RunPluginAutoOrders(PO_RUNAFTERRUN);
 delete ords;
}
void ARegion::RunBuildOrders()
{
 foreach(this){
  AObject *o=*(AObject **) iter;
  foreach(o->units){
   AUnit *u=*(AUnit**)iter;
   AOrderBuild *or=dynamic_cast<AOrderBuild*>(u->orders->Find(O_BUILD,2));
   if(!or)continue;
   or->Run(-1);
  }
 }
}
AUnit * AObject::GetOwner()
{
 return owner;
}
ARegionStatus* __fastcall ARegion::GetRegionStatus()
{
 if(!FRegionStatus) UpdateRegionStatus();     
 return FRegionStatus;
}
void ARegion::ClearRegionStatus(){
 delete FRegionStatus;
 FRegionStatus=0;
}
void ARegion::UpdateRegionStatus(){
 delete FRegionStatus;
 FRegionStatus=new ARegionStatus;
 ARegionStatus *rs=FRegionStatus;
 foreach(this){
  AObject *obj=*(AObject **) iter;
  if(obj->IsTower()) rs->HasTower=true;
  if(obj->IsBuilding()) rs->HasBuilding=true;
  if(obj->inner) rs->HasShaft=true;
  if(obj->IsLair()) rs->HasLair=true;
  if(obj->IsShip()) rs->HasShip=true;
  rs->HasRoad|=obj->IsRoad();
  if(obj->incomplete<=0)
    rs->HasGoodRoad|=obj->IsRoad();
 }
}
ARegionStatus::ARegionStatus()
 :HasTower(0),HasBuilding(0),HasShaft(0),HasLair(0),HasShip(0),HasRoad(0)
 ,HasGoodRoad(0)
{}
bool ARegion::PresentFaction(AFaction *fac){
 if(!fac) return false;
 foreach(this){
  AObject *obj=*(AObject **) iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(un->faction==fac) return true;
  }
 }
 return false;
}


int ARegion::GetObservation(AFaction * fac)
{
 int obs=0;
 foreach(this){
  AObject *obj=*(AObject **) iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(un->faction!=fac) continue;
   int temp=un->GetBegObservation();
   if(temp>obs) obs=temp;
  }
 }
 return obs;
}
int ARegion::GetTrueSight(AFaction * fac)
{
 int truesight=0;
//    if( S_TRUE_SEEING != -1 )
 foreach(this){
  AObject *obj=*(AObject **) iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(un->faction!=fac) continue;
   int temp=un->GetBegSkill(TrueSkill);
   if(temp>truesight) truesight=temp;
  }
 }
 return truesight;
}
bool ARegion::HasExitRoad(int dir)
{
 int mask=1<<dir;
 foreach(this){
  AObject *obj=*(AObject **) iter;
  if(!obj->num) continue;
  if(obj->incomplete>0) continue;
  AObjectType *otype=ObjectTypes->Get(obj->type);
  if(otype->road&mask)
   return true;
 }
 return false;
}

bool ARegion::HasConnectingRoad(int dir)
{
 int dir2=(dir+3)%6;
 return HasExitRoad(dir2);
}
bool ARegion::IsTaxRegion(int facnum)
{
	foreach(this){
	 AObject *obj=*(AObject**)iter;
	 foreach(obj->units){
		AUnit *u=*(AUnit**)iter;
		if(u->endfaction->number!=facnum)continue;
		if(!u->HasTaxOrder())continue;
		if(!CanTax(u))continue;
		return true;
	 }
	}
	return false;
}
bool ARegion::IsTradeRegion(int facnum)
{
	foreach(this){
	 AObject *obj=*(AObject**)iter;
	 foreach(obj->units){
		AUnit *u=*(AUnit**)iter;
		if(u->endfaction->number!=facnum)continue;
		if(!u->HasTradeOrder())continue;
		return true;
	 }
	}
	return false;
}


int AObject::GetSize()
{
 int siz=sizeof(*this);
 siz+=name.Length();
 siz+=describe.Length();
 siz+=type.Length();
 siz+=units->GetSize();
 return siz;
}
int ARegion::GetSize()
{
 int siz=sizeof(*this);
/* if(!hasInfo||oldinfoturnnum<turn->num)
  return 50;*/
 siz+=Capacity*4;
 siz+=type.Length();
 siz+=name.Length();
 siz+=towntype.Length();
 siz+=townname.Length();
 siz+=race.Length();
 siz+=weather.Length();
 siz+=nxtweather.Length();
 siz+=wanteds->GetSize();
 siz+=saleds->GetSize();
 siz+=products->GetSize();
 if(FRegionStatus)
  siz+=sizeof(*FRegionStatus);
 foreach(this){
  AObject *obj=*(AObject**)iter;
  siz+=obj->GetSize();
 }
 return siz;
}


int ARegionArray::GetSize()
{
 int siz=sizeof(*this);
 siz+=name.Length();
 return siz;
}
int ARegionArrays::GetSize()
{
 int siz=sizeof(*this);
 siz+=Capacity*4;
 foreach(this){
  ARegionArray *ra=*(ARegionArray**)iter;
  siz+=ra->GetSize();
 }
 return siz;
}
int ARegionList::GetSize()
{
 int siz=sizeof(*this);
 siz+=Capacity*4;
 siz+=RegionArrays->GetSize();
 foreach(this){
  ARegion *reg=*(ARegion**)iter;
  siz+=reg->GetSize();
 }
 return siz;
}
bool ARegion::IsSaveRegion()
{
 if(!hasInfo) return true;
 if(!(turn->num%curTurns->keyturn)){
  if(oldinfoturnnum) return true;
 }
 return (oldinfoturnnum==turn->num);
}
extern int movingdx[NDIRS];
extern int movingdy[NDIRS];
void ARegion::CreateExits(int exits){
 if(!exits) return;
 ARegionArray *ar=curRegionList->GetRegionArray(zloc);
 if(!ar) return;
 ar->VerifySize(xloc,yloc);
 int xsize=ar->x;
 int ysize=ar->y;
 for(int i=0;i<6;i++){
  if(!(exits&(1<<i))) continue;
  int x=xloc+movingdx[i];
  int y=yloc+movingdy[i];
  if(y<0||y>=ysize) continue;
  if(x<0) x+=xsize;
  if(x>=xsize) x-=xsize;
  neighbors[i].xloc=x;
  neighbors[i].yloc=y;
  neighbors[i].zloc=zloc;
 }
}
void ARegionList::Delete(ARegion *reg)
{
  int regnum=TList::IndexOf(reg);
  if(regnum<0) return;
  TList::Delete(regnum);
  delete reg;
}
AnsiString ARegionArray::GetName()
{
  if(!name.Length()) return "SURFACE";
  return name;
}
void AObject::SetNextOwner()
{
  foreach(units)
  {
    AUnit *un=*(AUnit**)iter;
    if(owner)
    {
      if(un==owner)
        owner=0;
      continue;
    }
    if(un->endobj!=this) continue;
    owner=un;
    break;
  }
}

