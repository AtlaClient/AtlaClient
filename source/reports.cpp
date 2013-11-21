//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "reports.h"
#include "region.h"
#include "faction.h"
#include "turn.h"
#include "editskilltypefrm.h"
#include "edititemtypefrm.h"
#include "unit.h"
#include "orders.h"
#include "turn.h"
#include "items.h"
#include "markets.h"
#include "skills.h"
#include "mapfrm.h"
#include "battlerep.h"
#include "acplugins.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
extern AnsiString keySpoils[5]; //from orders.cpp
static enum{rumQuery=0,rumAlways,rumNotUpdate
} RegionUpdateMode=rumQuery;

static int RegionTurnNum=-1;
static int UseRegionTurnNum=0; //-1-query,0-ignore,1-use
static int MinTurnNum;

bool ProcessReport(AnsiString fName)
{
  UseRegionTurnNum = -1;
  int turnnum = MinTurnNum = curTurn->num;
  TMemoryStream*mem = LoadFile(fName);
  char*s = (char*)mem->Memory;
  if(!ProcessHeaderReport(s))return false;
  if(!ProcessErrorReport(s))return false;
  if(!ProcessBattlesReport(s))return false;
  if(!ProcessEventReport(s))return false;
  if(!ProcessSkillReport(s))return false;
  if(!ProcessItemReport(s))return false;
  if(!ProcessObjectReport(s))return false;
  if(!ProcessFactionReport(s))return false;
  if(!ProcessRegionsReport(s))return false;
  if(!ProcessOrdersReport(s))return false;
  delete mem;
  if(!curFaction) curTurns->SetCurTurn(turnnum);
  int prvnum=MinTurnNum-1;
  if(prvnum<curTurns->curbeginturn) prvnum=0;
  curTurn->Modified=true;
  ATurn *turn=curTurns->Get(prvnum);
  turn->VerifyNextTurn();
  curTurn->PrepareOrders();
  AddEvent("Report processed");
  MapForm->ResetMap();
  return true;
}
//---------------------------------------------------------------------------
bool ProcessOrder(AnsiString fName){
 TMemoryStream*mem=LoadFile(fName);
 char*memory=(char*)mem->Memory,*s=memory;
 if(!ProcessOrdersReport(s))return false;
 delete mem;
 curTurn->PrepareOrders();
 curTurn->Modified=true;
 AddEvent("Order processed");
 return true;
}
//---------------------------------------------------------------------------
bool ProcessHeaderReport(char*&s){
 SkipEmptyLines(s);
 if(MyCompare(s,KeyPhrases->Get(keyAtlRepHead))){
  int ret=Application->MessageBox("No header found. Activate turn 0?","Warning",MB_YESNOCANCEL);
  if(ret==IDCANCEL) return false;
  if(ret==IDYES){
   AddEvent("No header found. Activate turn 0.");
   curTurns->SetCurTurn(0);
  }
  curFactions->SetCurFaction(0);
  curTurns->lastfac=0;
  RegionUpdateMode=rumQuery;
  return true;
 }
 RegionUpdateMode=rumAlways;
 AnsiString str;
 GetNextString(s);
 AnsiString factionstr=GetNextString(s);
 AnsiString turnstr=GetNextString(s);
 int turn=GetTurnNumberFromString(turnstr);
 char*ss=factionstr.c_str();
 AFaction *faction=new AFaction(curFactions);
 faction->name=GetNextFactionName(ss);
 if(faction->name.Length()==0)throw Exception("Error Process Header: faction not found");
 faction->number=atoi(ss);
 faction->local=true;
 AddEvent(AnsiString("Header found. Turn ")+
  turn+" for faction "+faction->FullName());
 if(curTurn&&curTurn->num)
 {
   AFaction *fac=curTurn->factions->GetFaction(faction->number);
   AnsiString str="Attempt to load report either with inappropriate turn number\n"
				  "or with new faction. Maybe this report belongs other game.";
   bool warning=false;
   if(turn>curTurns->count)
   {
	 str+="\nReport turn number: ";
	 str+=turn;
	 str+="\nLast turn number: ";
	 str+=curTurn->num;
	 warning=true;
   }
   if(!fac)
   {
	 str+="\nNot found faction with number ";
	 str+=faction->number;
	 warning=true;
   }else if(fac->name!=faction->name)
   {
	 str+="\nReport faction name: ";
	 str+=faction->name;
	 str+="\nGame faction name: ";
	 str+=fac->name;
	 warning=true;
   }
   if(warning)
   {
	 str+="\nContinue?";
	 if(Application->MessageBox(str.c_str(),"",MB_YESNO)!=IDYES)
	   return false;
   }
 }
 curTurns->PrepareTurn(turn);
 if(!curTurn||curTurn->num!=turn)
  return false;
 if(!ProcessUpdateFaction(faction))return false;
 curFactions->SetCurFaction(faction->number);
 delete faction;
 curFaction->Clear(); ///
 int pos;
 pos=factionstr.Pos(KeyPhrases->Get(keyWarSign));
 if(pos==0) curFaction->warpts=0;
 else{
  curFaction->warpts=atoi(factionstr.c_str()+pos
   +KeyPhrases->Get(keyWarSign).Length());
 }
 pos=factionstr.Pos(KeyPhrases->Get(keyTradeSign));
 if(pos==0) curFaction->tradepts=0;
 else{
  curFaction->tradepts=atoi(factionstr.c_str()+pos
   +KeyPhrases->Get(keyTradeSign).Length());
 }
 pos=factionstr.Pos(KeyPhrases->Get(keyMagicSign));
 if(pos==0) curFaction->magepts=0;
 else{
  curFaction->magepts=atoi(factionstr.c_str()+pos
   +KeyPhrases->Get(keyMagicSign).Length());
 }
 if(*s!='\n')return false;
 GetNextString(s);
 curTurns->ver1=GetNextString(s);
 curTurns->ver2=GetNextString(s);
 do{
  GetNextString(s);
 }while(*s&&MyCompare(s,KeyPhrases->Get(keyFacStat)));
 GetNextString(s);
 while(*s!='\n'){
  if(!MyCompare(s,KeyPhrases->Get(keyTaxStatSign))){
   s+=KeyPhrases->Get(keyTaxStatSign).Length();
   curFaction->warused=atoi(GetNextToken(s).c_str());
   if(*s!=' '||s[1]!='(')return false;
   s+=2;
   curFaction->warmax=atoi(GetNextToken(s).c_str());
   GetNextString(s);
  }else if(!MyCompare(s,KeyPhrases->Get(keyTradeStatSign))){
   s+=KeyPhrases->Get(keyTradeStatSign).Length();
   curFaction->tradeused=atoi(GetNextToken(s).c_str());
   if(*s!=' '||s[1]!='(')return false;
   s+=2;
   curFaction->trademax=atoi(GetNextToken(s).c_str());
   GetNextString(s);
  }else if(!MyCompare(s,KeyPhrases->Get(keyMagesStatSign))){
   s+=KeyPhrases->Get(keyMagesStatSign).Length();
   curFaction->mageused=atoi(GetNextToken(s).c_str());
   if(*s!=' '||s[1]!='(')return false;
   s+=2;
   curFaction->magemax=atoi(GetNextToken(s).c_str());
   GetNextString(s);
  }else{
   AnsiString str=GetNextString(s);
   int pos=str.Pos(':');
   if(!pos) return false;
   curFaction->unkfacstatus->Add(str);
  }
 }
 GetNextString(s);
 while(*s=='\n') s++;
 AddEvent(curFaction->ReportHeader());
 return true;
}
//---------------------------------------------------------------------------
bool ProcessErrorReport(char*&s){
 if(MyCompare(s,KeyPhrases->Get(keyAtlErrHead)))return true;
 AddEvent(KeyPhrases->Get(keyAtlErrHead));
 AnsiString str=GetNextString(s);
 TStringList *l=curFaction->errors;
 l->Clear();
 do{
  str=GetNextString(s,1);
  if(!str.Length())break;
  l->Add(str);
//  AddEvent(" "+str);
 }while(*s);
 return true;
}
//---------------------------------------------------------------------------
bool ProcessBattlesReport(char*&s){
 if(MyCompare(s,KeyPhrases->Get(keyAtlBatHead)))return true;
 AddEvent(KeyPhrases->Get(keyAtlBatHead));
 AnsiString str=GetNextString(s);
 TStringList *l=curFaction->battles;
 l->Clear();
 while(*s&&!IsEventReport(s)){
  str=GetNextString(s,1);
  if(IsEventReport(s)) break;
  l->Add(str);
//  AddEvent(" "+str);
 }
 return true;
}
//---------------------------------------------------------------------------
bool IsEventReport(const char*s){
 return !MyCompare(s,KeyPhrases->Get(keyAtlEvnHead));
}
//---------------------------------------------------------------------------
bool ProcessEventReport(char*&s){
 if(!IsEventReport(s))return true;
 AddEvent(KeyPhrases->Get(keyAtlEvnHead));
 AnsiString str=GetNextString(s);
 TStringList *l=curFaction->events;
 l->Clear();
 do{
  str=GetNextString(s,1);
  if(!str.Length())break;
  l->Add(str);
//  AddEvent(" "+str);
 }while(*s);
 curFaction->VerifyEventsWarnings();
 return true;
}
//---------------------------------------------------------------------------
bool IsSkillReport(const char*s){
 return !MyCompare(s,KeyPhrases->Get(keySkillHead));
}
//---------------------------------------------------------------------------
bool ProcessSkillReport(char*&s){
 if(!IsSkillReport(s))return true;
 GetNextString(s);
 ASkillInfo *si;
 AnsiString str;
 while(1){
  while(*s=='\n')s++;
  AnsiString skill=GetNextSkillName(s);
  if(!skill.Length())return true;
  AnsiString skillabr=GetNextToken(s);
  if(*s!=']')throw Exception("Error Process Skill Report: abridgement not found");
  si=new ASkillInfo;
  si->skill=skillabr;
  ASkillType *sk=new  ASkillType;
  sk->name=skill;
  sk->abr=skillabr;
  if(!ProcessUpdateSkill(sk))return false;
  delete sk;
  s++;
  while(*s==' ')s++;
  str=GetNextToken(s);
  si->level=str.ToInt();
  while(*s==':'||*s==' ') s++;
//  s+=2;
  AnsiString str=GetNextString(s,1);
  si->text=str;
  if(!ProcessUpdateSkillInfo(si))return false;
  delete si;
 }
}
//---------------------------------------------------------------------------
bool ProcessItemReport(char*&s){
 if(MyCompare(s,KeyPhrases->Get(keyItemHead)))return true;
 GetNextString(s);
 AItemType *it;
 AnsiString str,str2;
 char* ss;
 AItObjInfo *ioi;
 while(1){
  while(*s=='\n') s++;
  ss=s;
  str=GetNextString(s,1);
  int pos=str.Pos('[');
  if(pos==0){
   s=ss;
   break;
  }
  ss=str.c_str()+pos;
  AnsiString abr=GetNextToken(ss);
  if(abr.Length()&&*ss==']')
  {
    ioi=new AItObjInfo;
    ioi->abr=abr;
    ioi->type=AItObjInfo::ITEM;
    ioi->text=str;
    if(!ProcessUpdateItObjInfo(ioi))return false;
    delete ioi;
  }

  it=new AItemType;
  ss=str.c_str();
  bool illus=false;
  if(!MyCompare(ss,KeyPhrases->Get(keyIllusoryItemSign))){
   ss+=KeyPhrases->Get(keyIllusoryItemSign).Length();
   illus=true;
  }
  it->name = GetNextItemName(ss);
  if(!it->name.Length())return false;
//  it->names=it->name;
  if(illus){
   if(*ss=='I') ss++;
  }
  it->abr=GetNextToken(ss);
  if( (!MyCompare(ss, "]. This is a ship")) || (!MyCompare(ss, "]. This is a flying 'ship")) )
  {
	  // Here Ship ! Not Object !!!
	  AItObjInfo *ioi;
	  ioi = new AItObjInfo;
	  ioi->abr = it->name;
	  ioi->type = AItObjInfo::OBJECT;
	  ioi->text = str;
	  if(!ProcessUpdateObjectType(it->name,2)) return false;
	  if(!ProcessUpdateItObjInfo(ioi)) return false;
	  delete ioi;
	  it->name = "unfinished " + it->name;
	  if(!ProcessUpdateItem(it,str))return false;
	  delete it;
	  continue;
  }
  if(MyCompare(ss,KeyPhrases->Get(keyWeightSign)))return true;
  ss+=KeyPhrases->Get(keyWeightSign).Length();
  it->weight=GetNextToken(ss).ToInt();
  if(KeyPhrases->Get(keyFlyCapacSign).Length()){
	if(!MyCompare(ss,KeyPhrases->Get(keyCanWalk))){
	 ss+=KeyPhrases->Get(keyCanWalk).Length();
	 it->walk=it->weight;
	}else if(!MyCompare(ss,KeyPhrases->Get(keyWalkCapacSign))){
	 ss+=KeyPhrases->Get(keyWalkCapacSign).Length();
	 int value=GetNextToken(ss).ToInt();
	 if(!MyCompare(ss,KeyPhrases->Get(keyHitchedSign))){
	   //-hitchwalk
	   ss+=KeyPhrases->Get(keyHitchedSign).Length();
	   while(*ss&&*ss!=','&&*ss!='.') ss++;
	 }else
	  it->walk=it->weight+value;
	}
	if(!MyCompare(ss,KeyPhrases->Get(keyCanRide))){
	 ss+=KeyPhrases->Get(keyCanRide).Length();
	 it->ride=it->weight;
	}else if(!MyCompare(ss,KeyPhrases->Get(keyRideCapacSign))){
	 ss+=KeyPhrases->Get(keyRideCapacSign).Length();
	 int value=GetNextToken(ss).ToInt();
	 it->ride=it->weight+value;
	}
	if(!MyCompare(ss,KeyPhrases->Get(keyCanSwim))){
	 ss+=KeyPhrases->Get(keyCanSwim).Length();
	 it->swim=it->weight;
	}else if(!MyCompare(ss,KeyPhrases->Get(keySwimCapacSign))){
	 ss+=KeyPhrases->Get(keySwimCapacSign).Length();
	 int value=GetNextToken(ss).ToInt();
	 it->swim=it->weight+value;
	}
	if(!MyCompare(ss,KeyPhrases->Get(keyCanFly))){
	 ss+=KeyPhrases->Get(keyCanFly).Length();
	 it->fly=it->weight;
	}else if(!MyCompare(ss,KeyPhrases->Get(keyFlyCapacSign))){
	 ss+=KeyPhrases->Get(keyFlyCapacSign).Length();
	 int value=GetNextToken(ss).ToInt();
	 it->fly=it->weight+value;
	}
	if(str.Pos(KeyPhrases->Get(keyIsMan))){
	 it->type|=AItemType::IT_MAN;
	}
	if(str.Pos(KeyPhrases->Get(keyIsMon))){
	 it->type|=AItemType::IT_MONSTER;
	}
	if(str.Pos(KeyPhrases->Get(keyIsWeapon))){
	 it->type|=AItemType::IT_WEAPON;
	}
	if(str.Pos(KeyPhrases->Get(keyIsArmor))){
	 it->type|=AItemType::IT_ARMOR;
	}
	if(str.Pos(KeyPhrases->Get(keyIsTrade))){
	 it->type|=AItemType::IT_TRADE;
	}
	if(str.Pos(KeyPhrases->Get(keyIsMount))){
	 it->type|=AItemType::IT_MOUNT;
	}
	if(str.Pos(KeyPhrases->Get(keyIsBattle))){
	 it->type|=AItemType::IT_BATTLE;
	}
	if(str.Pos(KeyPhrases->Get(keyIsCantGive))){
	 it->flags|=AItemType::CANTGIVE;
	}
  }else{ //v4.0.6
	if(!MyCompare(ss,KeyPhrases->Get(keyCanWalk))){
	 ss+=KeyPhrases->Get(keyCanWalk).Length();
	 it->walk=it->weight;
	}
	if(!MyCompare(ss,KeyPhrases->Get(keyWalkCapacSign))){
	 ss+=KeyPhrases->Get(keyWalkCapacSign).Length();
	 it->walk=it->weight+GetNextToken(ss).ToInt();
	}
	if(!MyCompare(ss,KeyPhrases->Get(keyCanRide))){
	 ss+=KeyPhrases->Get(keyCanRide).Length();
	 it->ride=it->walk;
	}
	if(!MyCompare(ss,KeyPhrases->Get(keyCanFly))){
	 ss+=KeyPhrases->Get(keyCanFly).Length();
	 it->ride=it->walk;
	 it->fly=it->walk;
	}
	if(!MyCompare(ss,KeyPhrases->Get(keyIsMan))){
	 ss+=KeyPhrases->Get(keyIsMan).Length();
	 it->type|=AItemType::IT_MAN;
	}
	if(!MyCompare(ss,KeyPhrases->Get(keyIsMon))){
	 ss+=KeyPhrases->Get(keyIsMon).Length();
	 it->type|=AItemType::IT_MONSTER;
	}
  }
  if(illus){
   it->abr=AItemTypes::NormalAbrToIllusion(it->abr);
  }
  if(!ProcessUpdateItem(it,str))return false;
  delete it;
 }
 return true;
}
//---------------------------------------------------------------------------
AnsiString GetNextObjName(char*&buf){
 char* s=buf;
 while(*s!=':'&&*s!='\n'&&*s)s++;
 if(*s!=':')return "";
 AnsiString str(buf,s-buf);
 buf=s;
 return str;
}
//---------------------------------------------------------------------------
bool ProcessObjectReport(char*&s){
 if(MyCompare(s,KeyPhrases->Get(keyObjHead)))return true;
 GetNextString(s);
 //AItemType *it;
 AItObjInfo *ioi;
 AnsiString str,str2;
 char* ss,*ss2;
 while(1){
  while(*s=='\n') s++;
  ss=s;
  str=GetNextString(s,1);
  ss2=str.c_str();
  AnsiString abr=GetNextObjName(ss2);
  if(abr.Length()&&*ss2==':')
  {
	ioi=new AItObjInfo;
	ioi->abr=abr;
	ioi->type=AItObjInfo::OBJECT;
	ioi->text=str;
	if(!ProcessUpdateObjectType(abr,2))return false;
	if(!ProcessUpdateItObjInfo(ioi))return false;
	delete ioi;
  }

//  int ship=false;
  if(str.Pos(KeyPhrases->Get(keyIsObjectBuild))!=0){
  }else if(str.Pos(KeyPhrases->Get(keyIsObjectShip))!=0){
//   ship=true;
  }else{
   s=ss;
   break;
  }
  //- objectreport
 }
 return true;
}
//---------------------------------------------------------------------------
bool ProcessFactionReport(char*&s){
 if(MyCompare(s,KeyPhrases->Get(keyAttitudes)))return true;
 s+=KeyPhrases->Get(keyAttitudes).Length();

 AnsiString defattstr=GetNextToken(s);
 GetNextString(s);
 int defatt=AttitudeStrToInt(defattstr);
 if(defatt==-1)return false;
 if(defatt!=curFaction->defattitude){
  curFaction->defattitude=defatt;
  AddEvent("Default attitude changed to "+defattstr);
 }
 AFaction *oldfac=0;
 AFactions *oldfacs=0;
 {
  int oldnum=curTurn->num;
  if(oldnum>0&&oldnum<curTurns->count){
   ATurn *oldturn=curTurns->Get(oldnum-1);
   if(oldturn){
    oldfacs=oldturn->factions;
    oldfac=oldfacs->GetFaction(curFaction->number);
   }
  }
 }
 for(int curatt=A_HOSTILE;curatt<=A_ALLY;curatt++){
  if(MyCompare(s,AnsiString(AttitudeStrs[curatt])))return false;
  s+=strlen(AttitudeStrs[curatt]);
  if(*s==' ')s++;
  if(*s==':')s++;
  {
   AnsiString atts=GetNextString(s,1);
   char *ss=atts.c_str();
   do{
    if(*ss==',')ss++;
    if(*ss==' ')ss++;
    AnsiString facname=GetNextFactionName(ss);
    if(facname.Length()==0)break;
    int facnum=GetNextToken(ss).ToInt();
    if(*ss!=')')return false;
    AFaction *fac=new AFaction(curFactions);
    fac->name=facname;
    fac->number=facnum;
    if(!ProcessUpdateFaction(fac))return false;
    if(!oldfac&&curFaction->GetAttitude(facnum)!=curatt){
     AddEvent("Attitude to "+fac->FullName()+" changed to "+AttitudeStrs[curatt]);
    }
    delete fac;
    curFaction->SetAttitude(facnum,curatt);
    ss++;
   }while(*ss==',');
  }
 }
//compare attitudes
 if(oldfac){
  foreach(oldfac){
   Attitude *att=*(Attitude**)iter;
   Attitude *newatt=curFaction->Get(att->facnum);
   if(!newatt){
    AFaction *fac=oldfacs->GetFaction(att->facnum);
    if(fac)
     AddEvent("Attitude to "+fac->FullName()+" changed to DEFAULT");
   }
  }
  foreach(curFaction){
   Attitude *att=*(Attitude**)iter;
   Attitude *oldatt=oldfac->Get(att->facnum);
   if(!oldatt||att->attitude!=oldatt->attitude){
    AFaction *fac=oldfacs->GetFaction(att->facnum);
    if(fac)
     AddEvent("Attitude to "+fac->FullName()+" changed to "+AttitudeStrs[att->attitude]);
   }
  }
 }

 GetNextString(s);
 if(MyCompare(s,KeyPhrases->Get(keyUnclaimed)))return false;
 s+=KeyPhrases->Get(keyUnclaimed).Length();
 curFaction->unclaimed=atoi(s);
 AddEvent(KeyPhrases->Get(keyUnclaimed)+curFaction->unclaimed);
 GetNextString(s);
 while(*s!='\n')
 {
   AnsiString str=GetNextString(s);
   curFaction->unkfacmoneys->Add(str);
 }
 while(*s=='\n') s++;
 return true;
}
//---------------------------------------------------------------------------
bool IsRegionReport(const char *s){
 if(*s==0||*s=='\n') return false;
 char*ss=const_cast<char*>(s);
 AnsiString terrain=GetNextToken(ss);
 if(TerrainTypes->Get(terrain))return true;
 return false;
}
//---------------------------------------------------------------------------
bool ProcessRegionsReport(char*&s){
/* while(1){
  if(*s==0||IsOrderTemplate(s))break;
  if(IsRegionReport(s)){
   if(!ProcessRegionReport(s))return false;
  }else GetNextString(s);
 }*/
 int regcount=0;
 while(IsRegionReport(s)){
  regcount++;
  if(!ProcessRegionReport(s))return false;
 }
 if(!regcount) return false;
 if(*s=='\n')s++;
 return true;
}
//---------------------------------------------------------------------------
ARegion* ProcessRegionHeader(AnsiString header){
 ARegion *reg=new ARegion(curTurn);
// reg->turn=curTurnNumber;
 char* s=header.c_str();
 reg->type=GetNextToken(s);
 if(*s!=' ')return 0;
 s++;
 ALocation *loc=GetNextLocation(s);
 if(!loc)return 0;
 reg->xloc=loc->xloc;
 reg->yloc=loc->yloc;
 reg->zloc=loc->zloc;
 reg->UpdateKey();
 delete loc;
 if(MyCompare(s,KeyPhrases->Get(keyInSign)))return 0;
 s+=KeyPhrases->Get(keyInSign).Length();
 char*ss=s;
 while(*ss!='.'&&*ss!='!'&&*ss!=','&&*ss!='\n') ss++;
 if(*ss=='\n')return 0;
 reg->name=AnsiString(s,ss-s);
 s=ss;
 if(!MyCompare(s,KeyPhrases->Get(keyContains))){
  s+=KeyPhrases->Get(keyContains).Length();
  reg->townname=GetNextItemName(s);
  reg->towntype=GetNextToken(s);
  if(*s!=']')return 0;
  s++;
  if(!ProcessUpdateTownType(reg->towntype))return 0;
 }
 if(*s==','&&s[1]==' '){
  s+=2;
  reg->hasInfo=true;
  reg->oldinfoturnnum=RegionTurnNum==-1?curTurnNumber:RegionTurnNum;
  reg->population=GetNextToken(s).ToInt();
  if(MyCompare(s,KeyPhrases->Get(keyPeasants)))return 0;
  s+=KeyPhrases->Get(keyPeasants).Length();
  if(!optConquest)
  {
    ss=s;
    while(*ss!=')'&&*ss!='\n')ss++;
    if(*ss!=')')return 0;
    reg->race=AnsiString(s,ss-s);
    s=ss+1;
  }
  if(!MyCompare(s,KeyPhrases->Get(keySilvSign))){
   s+=KeyPhrases->Get(keySilvSign).Length();
   reg->money=GetNextToken(s).ToInt();
  }
 }
 if(*s!='.'&&*s!='!')return 0; //! - for battle
 return reg;
}
//---------------------------------------------------------------------------
bool ProcessWeatherInfo(ARegion *reg,char*&buf){
 char*ss=buf;
 while(*ss==' ')ss++;
 char* s=ss;
 while(*s!=';'&&*s!='\n')s++;
 if(*s!=';')return false;
 if(s[1]!=' ')return false;
 s+=2;
 AnsiString curweat(ss,s-ss);
 AnsiString nextweat=GetNextString(s,3);
 AnsiString curname=WeatherTypes->FindCurWeather(curweat);
 if(!curname.Length())return false;
 AnsiString nxtname=WeatherTypes->FindNextWeather(nextweat);
 if(!nxtname.Length())return false;
 reg->weather=curname;
 reg->nxtweather=nxtname;
 reg->hasInfo=true;
 reg->oldinfoturnnum=RegionTurnNum==-1?curTurnNumber:RegionTurnNum;
 buf=s;
 return true;
};
//---------------------------------------------------------------------------
bool IsWagesReport(char *s){
 return !MyCompare(s,KeyPhrases->Get(keyWages));
}
//---------------------------------------------------------------------------
bool ProcessItemsList(char *&s, AItems *list){
 if(!MyCompare(s,KeyPhrases->Get(keyNoneSign))){
  s+=KeyPhrases->Get(keyNoneSign).Length();return true;
 }
 while(1){
  int count=1;
  if(*s>='0'&&*s<='9'){
   count=atoi(GetNextToken(s).c_str());
   if(*s!=' ')return false;
   s++;
  }
  AnsiString itname=GetNextItemName(s);
  if(!itname.Length())return false;
  AnsiString itabr=GetNextToken(s);
  if(*s!=']') return false;
  s++;
  AItemType *itype=new AItemType;
  if(count==1)itype->name=itname;
  else itype->names=itname;
  bool isillus=false;
  if(!MyCompare(s,KeyPhrases->Get(keyItemIllusionSign))){
   s+=KeyPhrases->Get(keyItemIllusionSign).Length();
   isillus=true;
  }
  if(!isillus && list->GetItem(itabr)){
   AnsiString mes;
   mes="Possible illusion item detected \""+itabr+"\"";
   AddEvent(mes);
   isillus=true;
  }
  if(isillus){
   itype->abr=ItemTypes->NormalAbrToIllusion(itabr);
  }else{
   itype->abr=itabr;
  }
  itype->weight=-1;
  if(!ProcessUpdateItem(itype))return false;
  itabr=itype->abr;
  delete itype;
  AItem *it=new AItem;
  it->count = count;
  it->SetType(itabr);
  if(it->type->type == AItemType::IT_UNFINISHED)
  {
	 s++;
	 while (!(*s==' ')) s++;
	 AnsiString descrUF = "";
	 while (!(*s==')')) { descrUF += *s; s++; }
	 it->count = descrUF.ToInt();
	 s++;
  }
  list->Add(it);
  if(*s=='.'||*s==';')break;
  if(*s!=','||s[1]!=' ')return false;
  s+=2;
 }
 return true;
}
//---------------------------------------------------------------------------
bool ProcessMarketsList(char *&s, AMarkets *list){
 if(!MyCompare(s,KeyPhrases->Get(keyNoneSign))){
  s+=KeyPhrases->Get(keyNoneSign).Length();return true;
 }
 while(1){
  int amount=1;
  if(!MyCompare(s,KeyPhrases->Get(keyUnlimitedSign))){
   s+=KeyPhrases->Get(keyUnlimitedSign).Length();
   if(*s!=' ')return false;
   s++;
   amount=9999;
  }else if(*s>='0'&&*s<='9'){
   amount=atoi(GetNextToken(s).c_str());
   if(*s!=' ')return false;
   s++;
  }
  AnsiString itname=GetNextItemName(s);
  if(!itname.Length())return false;
  AnsiString itabr=GetNextToken(s);
  if(*s!=']') return false;
  s++;
  if(MyCompare(s,KeyPhrases->Get(keyMarketAtSign)))return false;
  s+=KeyPhrases->Get(keyMarketAtSign).Length();
  int price=atoi(GetNextToken(s).c_str());
  AItemType *itype=new AItemType;
  if(amount==1)itype->name=itname;
  else itype->names=itname;
  itype->abr=itabr;
  itype->weight=-1;
  if(!ProcessUpdateItem(itype))return false;
  delete itype;
  AMarket *mt=new AMarket;
  mt->SetType(itabr);
  mt->amount=amount;
  mt->price=price;
  list->Add(mt);
  if(*s=='.')break;
  if(*s!=','||s[1]!=' ')return false;
  s+=2;
 }
 return true;
}
//---------------------------------------------------------------------------
bool ProcessSkillsList(char *&s, ASkills *list){
 if(!MyCompare(s,KeyPhrases->Get(keyNoneSign))){
  s+=KeyPhrases->Get(keyNoneSign).Length();return true;
 }
 while(1){
  AnsiString skillname=GetNextSkillName(s);
  if(!skillname.Length())return false;
  AnsiString skillabr=GetNextToken(s);
  ASkillType *stype=new ASkillType;
  stype->name=skillname;
  stype->abr=skillabr;
  if(!ProcessUpdateSkill(stype))return false;
  delete stype;

  if(*s!=']'||s[1]!=' ')return false;
  s+=2;
  int level=atoi(GetNextToken(s).c_str());
  if(*s!=' '||s[1]!='(')return false;
  s+=2;
  int days=atoi(GetNextToken(s).c_str());
  if(*s!=')')return false;
  s++;
  ASkill *sk=new ASkill;
  sk->SetType(skillabr);
  sk->days=days;
  if(sk->GetLevel()!=level)throw Exception("ProcessSkillsList: level not match");
  list->Add(sk);
  if(*s=='.'||*s==';')break;
  if(*s!=','||s[1]!=' ')return false;
  s+=2;
 }
 return true;
}
//---------------------------------------------------------------------------
bool ProcessShortSkillsList(char *&s, TStringList *list){
 if(!MyCompare(s,KeyPhrases->Get(keyNoneSign))){
  s+=KeyPhrases->Get(keyNoneSign).Length();return true;
 }
 while(1){
  AnsiString skillname=GetNextSkillName(s);
  if(!skillname.Length())return false;
  AnsiString skillabr=GetNextToken(s);
  ASkillType *stype=new ASkillType;
  stype->name=skillname;
  stype->abr=skillabr;
  if(!ProcessUpdateSkill(stype))return false;
  delete stype;

  if(*s!=']')return false;
  s++;
  list->Add(skillabr);
  if(*s=='.'||*s==';')break;
  if(*s!=','||s[1]!=' ')return false;
  s+=2;
 }
 return true;
}
//---------------------------------------------------------------------------
bool ProcessExitInfo(AnsiString str,ARegion *basereg){
 char *s=str.c_str();
 AnsiString dirstr=GetNextToken(s);
 int dir=DirTypes->Find(dirstr);
 if(dir==-1){
  if(str==KeyPhrases->Get(keyNoneSign))
   return true;
  else return false;
 }
 if(*s!=' '||s[1]!=':'||s[2]!=' ')return false;
 s+=3;
 ARegion *reg=ProcessRegionHeader(s);
 if(!reg)return false;
// AddEvent(reg->WriteReport()); //debug
 bool ret=ProcessUpdateRegion(reg);
 if(ret){
  basereg->neighbors[dir].xloc=reg->xloc;
  basereg->neighbors[dir].yloc=reg->yloc;
  basereg->neighbors[dir].zloc=reg->zloc;
 }
 delete reg;
 return ret;
}
//---------------------------------------------------------------------------
bool inline IsUnitReport(const char*s,int tab){
 for(int i=0;i<tab;i++){
  if(s[i]!=' ')return false;
 }
 if(s[tab]=='-'||s[tab]=='*')return true;
 return false;
}
//---------------------------------------------------------------------------
bool IsObjectReport(const char*s){
 if(*s=='+'||IsUnitReport(s,0))return true;
 return false;
}
//---------------------------------------------------------------------------
bool ProcessUnitReport(AnsiString str,AObject *baseobj){
 AnsiString mes;
 char *s=str.c_str();
 AUnit *unit=new AUnit(curTurn);
 unit->info=(*s=='*')?INFO_LOCAL:INFO_UNIT;
 s+=2;
 unit->name=GetNextFactionName(s);
 if(unit->name=="Unit")unit->name="";
 unit->num=atoi(GetNextToken(s).c_str());

 if(*s!=')')return false;
 s++;
 if(!MyCompare(s,KeyPhrases->Get(keyUnitOnGuard))){
  s+=KeyPhrases->Get(keyUnitOnGuard).Length();
  unit->guard=GUARD_GUARD;
 }
 if(*s==','&&s[1]==' '){
  s+=2;
  AnsiString facname=GetNextFactionName(s);
  if(facname.Length()){
   int facnum=atoi(GetNextToken(s).c_str());
   if(facnum==0)return false;
   if(*s!=')')return false;
   s++;
   AFaction *fac=new AFaction(curFactions);
   fac->name=facname;
   fac->number=facnum;
   if(!ProcessUpdateFaction(fac))return false;
   delete fac;
   unit->SetFaction(facnum);
   if(unit->info!=INFO_LOCAL) unit->info=INFO_FACTION;
  }else{
   unit->SetFaction(0);
   s-=2;
  }
 }
 if(!MyCompare(s,KeyPhrases->Get(keyUnitAvoid))){
  s+=KeyPhrases->Get(keyUnitAvoid).Length();
  unit->guard=GUARD_AVOID;
 }
 if(!MyCompare(s,KeyPhrases->Get(keyUnitBehind))){
  s+=KeyPhrases->Get(keyUnitBehind).Length();
  unit->SetFlag(FLAG_BEHIND,true);
 }
 if(!MyCompare(s,KeyPhrases->Get(keyUnitRevUnit))){
  s+=KeyPhrases->Get(keyUnitRevUnit).Length();
  unit->reveal=REVEAL_UNIT;
 }
 if(!MyCompare(s,KeyPhrases->Get(keyUnitRevFaction))){
  s+=KeyPhrases->Get(keyUnitRevFaction).Length();
  unit->reveal=REVEAL_FACTION;
 }
 if(!MyCompare(s,KeyPhrases->Get(keyUnitHold))){
  s+=KeyPhrases->Get(keyUnitHold).Length();
  unit->SetFlag(FLAG_HOLDING,true);
 }
 if(!MyCompare(s,KeyPhrases->Get(keyUnitTax))){
  s+=KeyPhrases->Get(keyUnitTax).Length();
  unit->SetFlag(FLAG_AUTOTAX,true);
 }
 if(!MyCompare(s,KeyPhrases->Get(keyUnitNoAid))){
  s+=KeyPhrases->Get(keyUnitNoAid).Length();
  unit->SetFlag(FLAG_NOAID,true);
 }
 if(!MyCompare(s,KeyPhrases->Get(keyUnitSharing))){
  s+=KeyPhrases->Get(keyUnitSharing).Length();
  unit->SetFlag(FLAG_SHARING,true);
  if(!optServerSharing) optServerSharing=true;
 }
 if(!MyCompare(s,KeyPhrases->Get(keyUnitConsumUnit))){
  s+=KeyPhrases->Get(keyUnitConsumUnit).Length();
  unit->SetFlag(FLAG_CONSUMING_UNIT,true);
 }
 if(!MyCompare(s,KeyPhrases->Get(keyUnitConsumFaction))){
  s+=KeyPhrases->Get(keyUnitConsumFaction).Length();
  unit->SetFlag(FLAG_CONSUMING_FACTION,true);
 }
 if(!MyCompare(s,KeyPhrases->Get(keyUnitNoCross))){
  s+=KeyPhrases->Get(keyUnitNoCross).Length();
  if(!optServNoCross){
   int ret=Application->MessageBox("NOCROSS flag found.\nActivate it?","",MB_YESNO);
   if(ret==IDYES)
    optServNoCross=true;
  }
  if(optServNoCross)
   unit->SetFlag(FLAG_NOCROSS_WATER,true);
 }
 if(!MyCompare(s,KeyPhrases->Get(keyUnitNoSpoils))){
  s+=KeyPhrases->Get(keyUnitNoSpoils).Length();
  if(!optServNoSpoils){
   int ret=Application->MessageBox("NOSPOILS flag found.\nActivate it?","",MB_YESNO);
   if(ret==IDYES)
    optServNoSpoils=true;
  }
  if(optServNoSpoils)
   unit->SetFlag(FLAG_NOSPOILS,true);
 }
 int spoils=-1;
 if(!MyCompare(s,KeyPhrases->Get(keyUnitSpoilsNone))){
  s+=KeyPhrases->Get(keyUnitSpoilsNone).Length();
  spoils=0;
 }else if(!MyCompare(s,KeyPhrases->Get(keyUnitSpoilsWalk))){
  s+=KeyPhrases->Get(keyUnitSpoilsWalk).Length();
  spoils=1;
 }else if(!MyCompare(s,KeyPhrases->Get(keyUnitSpoilsRide))){
  s+=KeyPhrases->Get(keyUnitSpoilsRide).Length();
  spoils=2;
 }else if(!MyCompare(s,KeyPhrases->Get(keyUnitSpoilsFly))){
  s+=KeyPhrases->Get(keyUnitSpoilsFly).Length();
  spoils=3;
 }
 if(spoils>=0){
  if(!optServSpoils){
   int ret=Application->MessageBox("SPOILS flag found.\nActivate it?","",MB_YESNO);
   if(ret==IDYES)
    optServSpoils=true;
  }
  if(optServSpoils)
   unit->flags|=AUnit::GetFlagFromSpoils(spoils);
 }

 if(*s!=','||s[1]!=' ')return false;
 s+=2;
 if(!ProcessItemsList(s,unit->items))return false;
 while(*s=='.'&&s[1]==' ')
 {
   if(!MyCompare(s,KeyPhrases->Get(keyUnitSkills))){
    s+=KeyPhrases->Get(keyUnitSkills).Length();
    if(!ProcessSkillsList(s,unit->skills))return false;
    unit->hasskills=true;
   }else
   if(!MyCompare(s,KeyPhrases->Get(keyUnitCombatSpell))){
    s+=KeyPhrases->Get(keyUnitCombatSpell).Length();
    AnsiString skillname=GetNextSkillName(s);
    if(!skillname.Length())return false;
    AnsiString skillabr=GetNextToken(s);
    ASkillType *stype=new ASkillType;
    stype->name=skillname;
    stype->abr=skillabr;
    if(!ProcessUpdateSkill(stype))return false;
    delete stype;
    stype=SkillTypes->Get(skillabr,2);
    if(!stype)return false;
    int mustflags=ASkillType::MAGIC|ASkillType::COMBAT;
    if((stype->flags&mustflags)!=mustflags){
     mes="Make skill \""+stype->name+"\" combat spell skill?";
     if(Application->MessageBox(mes.c_str(),"New combat spell detected",MB_YESNO)!=IDYES)return false;
     stype->flags|=mustflags;
    }
    if(*s!=']')return false;
    s++;
    unit->combatspell=skillabr;
    unit->hascombatspell=true;
   }else
   if(!MyCompare(s,KeyPhrases->Get(keyUnitCanStudy))){
    s+=KeyPhrases->Get(keyUnitCanStudy).Length();
    TStringList *list=new TStringList;
    if(!ProcessShortSkillsList(s,list))return false;
    unit->canstudy->Assign(list);
    unit->hascanstudy=true;
    delete list;
   }else{
     s+=2;
     char *olds=s;
     while(*s&&*s!=';'&&*s!='.') s++;
     bool cont=false;
     for(int i=0,endi=curTurns->ignore_params->Count;i<endi;i++)
     {
       if(MyCompare(olds,curTurns->ignore_params->Strings[i]))
         continue;
       cont=true;
       break;
     }
     if(cont) continue;
     AnsiString str(olds,s-olds);
     if(unit->params.Length())
       unit->params+=". ";
     unit->params+=str;
   }
 }
 AnsiString descr=GetNextString(s);
 int dlen=descr.Length();
 if(dlen){
  if(descr[dlen]=='.')descr.SetLength(dlen-1);
 }
 unit->describe=descr;
 baseobj->AddUnit(unit);
 return true;
}
//---------------------------------------------------------------------------
bool ProcessObjectReport(char*&s,ARegion *basereg){
 int tab=0;
 AnsiString str;
 AObject *obj;
 if(*s=='+'){
  str=GetNextString(s,-1);
  char*olds=s;
  AnsiString str2;
  while(IsUnitReport(s,2))
  {
    s++;
    str2=GetNextString(s,-1);
    if(strchr(str2.c_str(),'(')!=0)
      break;
    str+=str2;
    olds=s;
  }
  s=str.c_str();
  tab=2;
  s+=2;
  char *ss;
/*  char *ss=s;
  while(*ss!='['&&*ss!='\r'&&*ss!='\n')ss++;
  if(*ss!='[')return false;
  AnsiString name(s,ss-s-1);
  s=ss+1;*/
  AnsiString name=GetNextObjectName(s);
  if(name.Length()==0)return false;
  int num=atoi(GetNextToken(s).c_str());
  if(MyCompare(s,KeyPhrases->Get(keyObjNameEndSign)))return false;
  s+=KeyPhrases->Get(keyObjNameEndSign).Length();
  ss=s;
  while(*ss!=','&&*ss!=';'&&*ss!='.'&&*ss!='\n')ss++;
  if(*ss=='\n')return false;
  AnsiString type(s,ss-s);
  s=ss;
  if(!ProcessUpdateObjectType(type,2))return false;
  obj=new AObject(curTurn);
  obj->num=num;
  obj->name=name;
  obj->type=type;
  basereg->Add(obj);
  int canenter=1;
  if(!MyCompare(s,KeyPhrases->Get(keyObjNeedSign))){
   s+=KeyPhrases->Get(keyObjNeedSign).Length();
   obj->incomplete=atoi(GetNextToken(s).c_str());
  }
  if(!MyCompare(s,KeyPhrases->Get(keyObjDecayWarning))){
   s+=KeyPhrases->Get(keyObjDecayWarning).Length();
   obj->incomplete=-1;
  }
  if(!MyCompare(s,KeyPhrases->Get(keyObjMaintenanceWarning))){
   s+=KeyPhrases->Get(keyObjMaintenanceWarning).Length();
   obj->incomplete=-2;
  }
  if(!MyCompare(s,KeyPhrases->Get(keyObjContainsSign))){
   s+=KeyPhrases->Get(keyObjContainsSign).Length();
   obj->inner=true;
  }
  if(!MyCompare(s,KeyPhrases->Get(keyObjEngravedSign))){
   s+=KeyPhrases->Get(keyObjEngravedSign).Length();
   obj->runes=1;
  }
  if(obj->type == "Fleet")
  {
	// Это Флот.
	s += 2;
	obj->describe = s;
  }
  if(*s==';'){
   s+=2;
   ss=s;
   while(1){
    while(*ss!=','&&*ss!=0) ss++;
    if(*ss==','){
     if(!MyCompare(ss,KeyPhrases->Get(keyObjClosedSign)))
      break;
     ss++;
    }else break;
   }
   AnsiString descr(s,ss-s);
   s=ss;
   int dlen=descr.Length();
   if(dlen&&descr[dlen]=='.')
    descr.SetLength(dlen-1);
   obj->describe=descr;
  }
  if(!MyCompare(s,KeyPhrases->Get(keyObjClosedSign))){
   s+=KeyPhrases->Get(keyObjClosedSign).Length();
   canenter=0;
  }
  s=olds;
  if(!ProcessUpdateObjectType(type,canenter))return false;
 }else{
  obj=basereg->GetDummy();
 }
 while(IsUnitReport(s,tab)){
  s+=tab;
  str=GetNextString(s,tab+1);
  if(!ProcessUnitReport(str,obj))return false;
 }
 if(*s=='\n')s++;

 if(obj->inner&&obj->describe.Pos(";!")==1){
  ARegion *reg=ProcessRegionHeader(obj->describe.SubString(3,obj->describe.Length())+".");
  if(reg){
   if(ProcessUpdateRegion(reg)){
    ARegion *reg2=curRegionList->Get(reg->xloc,reg->yloc,reg->zloc);
    if(reg2){
     AObject *shaft2=0;
     foreach(reg2)
     {
       AObject *obj=*(AObject**)iter;
       if(!obj->inner) continue;
       if(obj->innerloc.xloc!=-1&&!(obj->innerloc==*basereg)) continue;
       shaft2=obj;
       break;
     }
     obj->describe="";
     if(shaft2)
     {
       shaft2->innerloc=*basereg;
       obj->innerloc=*reg2;
     }else{
       if(reg2->hasInfo)
       {
         AnsiString mes="Warning: found shafts from ";
         mes+=basereg->FullName(false);
         mes+=" to ";
         mes+=reg2->FullName(false);
         mes+=" with no free shafts in end region";
         AddEvent(mes);
       }
     }
    }
   }
   delete reg;
  }
 }
 return true;
}
//---------------------------------------------------------------------------
bool ProcessRegionReport(char*&s){
 RegionTurnNum=-1;
 AnsiString header=GetNextString(s,1);
 char *ss;
 if(*s!='-')return false;
 if(!curFaction){
   AnsiString str=GetNextString(s);
   int pos=str.Pos(";");
   if(pos){
     char *ss=str.c_str()+pos;
     int num1=atoi(ss);
     while(*ss&&*ss!='-') ss++;
     if(*ss=='-')
     {
       ss++;
       num1=atoi(ss);
     }
     RegionTurnNum=num1;
   }
 }else
   GetNextString(s);
 ARegion *reg=ProcessRegionHeader(header);
 if(!reg)return false;
 if(curFaction&&curFaction->number)
  reg->lastviewfact=curFaction->number;

 ProcessWeatherInfo(reg,s);

 if(*s=='\n')s++;
 while(*s&&!IsRegionReport(s)&&!IsWagesReport(s)&&(*s!='\n')
  &&!IsObjectReport(s)&&MyCompare(s,KeyPhrases->Get(keyExitSign))
  ){
  GetNextString(s);
 }
 if(*s=='\n')s++;
 if(IsWagesReport(s)){
  s += sizeof(" Wages: $")-1;
  if(*s=='$')s++;
  reg->wages=GetNextToken(s).ToInt();
  AnsiString fractstr;
  if(*s=='.'&&isdigit(s[1]))
  {
    s++;
    fractstr=GetNextToken(s);
  }
  if(reg->wages||fractstr.Length()){
   if(MyCompare(s,KeyPhrases->Get(keyMaxSign)))return false;
   s+=KeyPhrases->Get(keyMaxSign).Length();
   reg->maxwages=GetNextToken(s).ToInt();
  }
  GetNextString(s);
  if(MyCompare(s,KeyPhrases->Get(keyWantedSign)))return false;
  s += KeyPhrases->Get(keyWantedSign).Length()-1;
  if(*s==' ') s++;
  AnsiString wantedstr=GetNextString(s,3);
  ss=wantedstr.c_str();
  if(!ProcessMarketsList(ss,reg->wanteds))return false;


  if(MyCompare(s,KeyPhrases->Get(keySaledSign)))return false;
  s += KeyPhrases->Get(keySaledSign).Length()-1;
  if(*s==' ') s++;
  AnsiString saledstr=GetNextString(s,3);
  ss=saledstr.c_str();
  if(!ProcessMarketsList(ss,reg->saleds))return false;

  if(!MyCompare(s,KeyPhrases->Get(keyEnteSign))){
   s += KeyPhrases->Get(keyEnteSign).Length()-1;
   if(*s==' ') s++; if(*s=='$') s++;
   reg->entertainment=GetNextToken(s).ToInt();
   GetNextString(s);
  }
  if(!MyCompare(s,KeyPhrases->Get(keyProdSign)))
  {
    s+=KeyPhrases->Get(keyProdSign).Length()-1;
    if(*s==' ') s++;
    AnsiString prodstr=GetNextString(s,3);
    ss=prodstr.c_str();
    AItems *items=new AItems;
    if(!ProcessItemsList(ss,items))return false;
    reg->products->Assign(items);
    delete items;
    GetNextString(s);
  }
 }
 if(*s=='\n')s++;
 if(!MyCompare(s,KeyPhrases->Get(keyExitSign))){

  GetNextString(s);
  while(*s==' '&&s[1]==' '){
   s+=2;
   AnsiString exitstr=GetNextString(s,3);
   if(!ProcessExitInfo(exitstr,reg))return false;
  }
 }
 while (*s=='\n') s++;
 if(!MyCompare(s,KeyPhrases->Get(keyGatesSign))){
  s+=KeyPhrases->Get(keyGatesSign).Length();
  int gnum=atoi(GetNextToken(s).c_str());
  if(MyCompare(s,KeyPhrases->Get(keyGatesSign2)))return false;
  s+=KeyPhrases->Get(keyGatesSign2).Length();
  int gkol=atoi(GetNextToken(s).c_str());
  reg->gate=gnum;
  if(curRegionList->numberofgates<gkol)curRegionList->numberofgates=gkol;
  GetNextString(s);
  if(*s=='\n')s++;
 }
 if(!IsObjectReport(s)){
  if(*s=='\n')s++;
  else if(reg->zloc==0){
    while(*s!='\n')
      GetNextString(s);
    if(*s=='\n')s++;
  }
 };
 if(IsObjectReport(s)){
  reg->hasInfo=true;
  reg->oldinfoturnnum=RegionTurnNum==-1?curTurnNumber:RegionTurnNum;
 }
 while(IsObjectReport(s)){
  if(!ProcessObjectReport(s,reg))return false;
 }
 while(*s=='\n')s++;
 if(!(optDontShowEvents&dseRegDataFound))
  AddEvent("Region data for "+reg->FullName()+" found");
 bool ret=ProcessUpdateRegion(reg);
 delete reg;
 return ret;
}
//---------------------------------------------------------------------------
bool IsOrderReport(const char*s){
 return !MyCompare(s,KeyPhrases->Get(keyTemplateSign));
}
//---------------------------------------------------------------------------
bool IsOrdersBegin(const char*s){
 return !MyCompare(s,KeyPhrases->Get(keyTemplateBegin));
}
//---------------------------------------------------------------------------
bool ProcessOrdersReport(char*&s){
  SkipEmptyLines(s);
  if(!IsOrdersBegin(s))
  {
    if(!IsOrderReport(s))return true;
    GetNextString(s);
    GetNextString(s);
  }
  if(!IsOrdersBegin(s))return false;
  s += KeyPhrases->Get(keyTemplateBegin).Length();

  int facnum = atoi(GetNextToken(s).c_str());
  if(!curFaction||curFaction->number!=facnum) curFactions->SetCurFaction(facnum);
  if(!curFaction||curFaction->number!=facnum) return false;
  AnsiString pas;
  if(*s == ' '){
    s++;
    pas = GetNextToken(s);
  }
  if(pas!=curFaction->password){
    AnsiString mes="Found faction "+curFaction->FullName();
    mes+="\npassword changing.";
    mes+="\nChange password?";
    int ret=IDYES;//Application->MessageBox(mes.c_str(),"Updating Faction Password",MB_YESNOCANCEL);
    if(ret==IDCANCEL)return false;
    if(ret==IDYES)
    {
      curFaction->password=pas;
      AddEvent("Faction "+curFaction->FullName()+" password changed");
    }
  }
  GetNextString(s);

  curFaction->orders->Clear();
  AOrder *ord;
  AOrderTurn *turnord = 0;
  AUnit *unit = 0, *former = 0;
  AnsiString str;
  while(1)
  {
	while(*s == '\n') s++;
    if(*s == 0) return false;
    if(*s==';'&&s[1]!='!')
    {
      GetNextString(s);
      continue;
    }
    
    str = GetNextString(s,1);
    ord = ParseOrder(str);
    if(!ord) return false;
    if(ord->type == O_END)
    {
      delete ord;
      if(former) AddEvent("FORM without END");
      if(turnord) AddEvent("TURN without END");
      break;
    }
    if(ord->type == O_UNIT)
    {
      delete ord;
      char *ss = str.c_str();
      while(*ss&&(*ss<'0'||*ss>'9'))ss++;
      if(!*ss)return false;
      int unitnum = atoi(ss);
      unit = curRegionList->GetUnit(unitnum);
      if(!unit)
      {
        str="Unit ";
        str+=unitnum;
		str+=" not found";
		AddEvent(str);
		while (1) {
		  if( (*s == '\n') && (s[1] == '\n' ) ) break;
		  if(*s == '#') break;
		  s++;
		}

		continue;
		//return false;
      }
      if(unit->faction!=curFaction)
      {
        str="Unit ";
        str+=unitnum;
        str+=" does not belong to current faction";
        AddEvent(str);
        return false;
      }
      unit->orders->Clear();
      continue;
    } // if Unit

    if(!unit)
    {
      AddEvent("no UNIT decalred");
      return false;
    }

    if(ord->type == O_FORM)
    {
      if(former)
      {
        AddEvent("FORM in FORM");
        return false;
      }
      char *ss = str.c_str();
      while(*ss&&(*ss<'0'||*ss>'9'))ss++;
      if(!*ss)return false;
      int unitnum = -atoi(ss);
      former = unit;
      unit = former->baseobj->basereg->GetUnit(unitnum);
      if(!unit)
      {
        unit = new AUnit(curTurn);
        unit->num = unitnum;
        former->baseobj->units->Add(unit);
      }
      unit->FormNew(former);
      unit->orders->Clear();
      continue;
    } // If O_FORM

    if(ord->type == O_FORMEND)
    {
      if(!former)
      {
        AddEvent("END without FORM");
        return false;
      }
      unit = former;
      former = 0;
      continue;
    }

    if(ord->type == O_ENDTURN)
    {
      if(!turnord)
      {
		AddEvent("ENDTURN without TURN");
		return false;
	  }
	  turnord = 0;
	  continue;
	}

	if(ord->type == O_TURN)
	{
	  // TURN order. Need find All orders
	  unit->orders->Add(ord);
	  turnord = ((AOrderTurn*)ord);
	  continue;
	}
	if(turnord)
	{
	  ord->turnorder = true;
      turnord->torders->Add(ord);
    } else unit->orders->Add(ord);
 }
 return true;
}
//---------------------------------------------------------------------------
bool ProcessUpdateFaction(AFaction *newFaction,int ver){
 if(ver==-1) ver=curFactions->VerFaction(newFaction->name,newFaction->number);
//0 - not found, 1 - present, 2 - name, not num, 3 - num, not name
 AFaction *fac,*fac2;
 AnsiString mes;
 switch(ver){
  case 0:
   fac=new AFaction(curFactions);
   fac->CreateNew(newFaction);
   curFactions->Add(fac);
   AddEvent(AnsiString("New faction \'")+fac->FullName()+"\' added"+(fac->local?" as local faction":""));
  break;
  case 1:
   if(newFaction->local){
    fac=curFactions->GetFaction(newFaction->number);
    if(!fac->local){
     fac->local=true;
     AddEvent("Set faction "+fac->FullName()+" as local faction");
    }
   }
  break;
  case 2:
   fac=curFactions->GetFaction(newFaction->name);
   mes="Found new faction: "+newFaction->FullName();
   mes+="\nwith present name: "+fac->FullName();
   mes+="\nDelete old faction?";
   int ret;
   if(fac->name=="Faction"||
    (ret=Application->MessageBox(mes.c_str(),"Updating Faction",MB_YESNOCANCEL))==IDNO){
    fac=new AFaction(curFactions);
    fac->CreateNew(newFaction);
    curFactions->Add(fac);
    AddEvent(AnsiString("New faction \'")+fac->FullName()+"\' added"+(fac->local?" as local faction":""));
    return true;
   }
   if(ret==IDCANCEL) return false;
   mes=AnsiString("Old faction ")+fac->FullName()+" will deleted";
   fac->deleted=1;
   AddEvent(mes);
   fac=new AFaction(curFactions);
   fac->CreateNew(newFaction);
   curFactions->Add(fac);
   AddEvent(AnsiString("New faction ")+fac->FullName()+(fac->local?" added as local faction":" added"));
  break;
  case 3:
   fac=curFactions->GetFaction(newFaction->number);
   if(newFaction->name=="Faction") fac2=0;
   else fac2=curFactions->GetFaction(newFaction->name);
   if(!fac2){
/*    mes="Found faction (";
    mes+=newFaction->number;
    mes+=") name changing:\nOld name: "+fac->name;
    mes+="\nNew name: "+newFaction->name+"\nRename faction?";
    if(Application->MessageBox(mes.c_str(),"Updating Faction",MB_YESNO)!=IDYES){
     return false;
    }*/
    fac->local=newFaction->local;
    mes="Faction (";
    mes+=fac->number;
    mes+=") renamed from \'";
    mes+=fac->name;
    mes+="\' to \'";
    mes+=newFaction->name;
    mes+="\'";
    fac->name=newFaction->name;
    AddEvent(mes);
   }else{
    mes="Found faction (";
    mes+=newFaction->number;
    mes+=") name changing to present name:\nOld name: "+fac->name;
    mes+="\nNew name: "+newFaction->name;
    mes+="\nOld faction: "+fac2->FullName();
    mes+="\nRename faction and delete old?";
    if(/*fac2->name=="Faction"||*/
     Application->MessageBox(mes.c_str(),"Updating Faction",MB_YESNOCANCEL)==IDNO){
     mes+="Faction (";
     mes+=fac->number;
     mes+=") renamed";
     fac->name=newFaction->name;
     fac->local=newFaction->local;
     AddEvent(mes);
     return true;
    }
    if(ret==IDCANCEL) return false;
    mes+="Faction (";
    mes+=fac->number;
    mes+=") renamed";
    mes+="; old faction (";
    mes+=fac2->number;
    mes+=") will deleted";
    fac2->deleted=1;
    fac->name=newFaction->name;
    fac->local=newFaction->local;
    AddEvent(mes);
   }
  break;
 }
 return true;
}
//---------------------------------------------------------------------------
bool ProcessUpdateSkill(ASkillType *newSkill,int ver){
 if(ver==-1)ver=SkillTypes->VerSkills(newSkill->name, newSkill->abr);
//0 - not found, 1 - present, 2 - name, not abr, 3 - abr, not name
 ASkillType *sk=0;
 AnsiString mes;
 TEditSkillTypeForm *estf;
 switch(ver){
  case 0:
   sk=new ASkillType;
   sk->CreateNew(newSkill);
   estf=new TEditSkillTypeForm(0,sk);
   estf->Caption=estf->Caption+": New skill found";
   if(estf->ShowModal()!=mrOk){delete estf;return false;}
   delete estf;
   sk->sortindex=SkillTypes->count;
   SkillTypes->AddSkillType(sk);
   SkillTypes->UpdateSortedList();
   AddEvent(AnsiString("New skill ")+sk->FullName()+" added");
  break;
  case 1:
  break;
  case 2:
   sk=SkillTypes->Get(newSkill->name,1);
  case 3:
   if(!sk)sk=SkillTypes->Get(newSkill->abr,2);
   if(!sk)throw Exception("Fatal error in ProcessUpdateSkill: "+newSkill->abr);
   estf=new TEditSkillTypeForm(0,sk,newSkill);
   estf->Caption=estf->Caption+": skill changes found";
   if(estf->ShowModal()!=mrOk){delete estf;return false;}
   delete estf;
/*   mes="Found new skill with present name or abridgement.\nOld skill: ";
   mes+=sk->FullName();
   mes+="\nNew skill: ";
   mes+=newSkill->FullName();
   mes+="\nRename skill?";
   if(Application->MessageBox(mes.c_str(),"Updating Skill types",MB_YESNO)!=IDYES){
	return false;
   }
   mes="Old skill "+sk->FullName()+" renamed to "+newSkill->FullName();
   sk->name=newSkill->name;
   sk->abr=newSkill->abr;*/
   mes="Skill "+sk->FullName()+" updated";
   AddEvent(mes);
  break;
 }
 return true;
}
//---------------------------------------------------------------------------
bool ProcessUpdateSkillInfo(ASkillInfo *newSi){
 ASkillInfo *si=SkillInfos->Get(newSi->skill,newSi->level);
 AnsiString mes;
 if(!si){
  si=new ASkillInfo;
  si->CreateNew(newSi);
  SkillInfos->AddSkillInfo(si);
  AddEvent(AnsiString("New skill info ")+si->FullName()+" added");
 }else{
  if(si->text==newSi->text)return true;
  mes="Found skill "+si->FullName()+" with other description\nOld descr: ";
  mes+=si->text;
  mes+="\nNew descr: ";
  mes+=newSi->text;
  mes+="\nReplace?";
  int ret=Application->MessageBox(mes.c_str(),"Updating skill report",MB_YESNOCANCEL);
  if(ret==IDCANCEL)return false;
  if(ret==IDYES){
   si->text=newSi->text;
   mes="Description for "+si->FullName()+" changed";
   AddEvent(mes);
  }
 }
 return true;
}
//---------------------------------------------------------------------------
bool ProcessUpdateItObjInfo(AItObjInfo*newIoi)
{
 AItObjInfo *ioi=ItObjInfos->Get(newIoi->abr,newIoi->type);
 AnsiString mes;
 if(!ioi){
  ioi=new AItObjInfo;
  ioi->CreateNew(newIoi);
  ItObjInfos->AddItObjInfo(ioi);
  AddEvent(AnsiString("New info for ")+ioi->FullName()+" added");
 }else{
  if(ioi->text==newIoi->text)return true;
  mes="Found info for "+ioi->FullName()+" with other description\nOld descr: ";
  mes+=ioi->text;
  mes+="\nNew descr: ";
  mes+=newIoi->text;
  mes+="\nReplace?";
  int ret=Application->MessageBox(mes.c_str(),"Updating item&object report",MB_YESNOCANCEL);
  if(ret==IDCANCEL)return false;
  if(ret==IDYES){
   ioi->text=newIoi->text;
   mes="Description for "+ioi->FullName()+" changed";
   AddEvent(mes);
  }
 }
 return true;
}
//---------------------------------------------------------------------------
bool ProcessUpdateItem(AItemType *newit,AnsiString descr){
  AItemType *it=ItemTypes->Get(newit->abr,4);
  if(!it)
  {
	if(!it)it=ItemTypes->Get(newit->name,1);
	if(!it)it=ItemTypes->Get(newit->names,2);
	if(it&&newit->abr.Length()&&it->abr.Length()) it=0;
  }

  if( (!it) && (newit->name.Pos("unfinished")))
  {
	 // Unfinished Object
	 it = new AItemType;
	 it->CreateNew(newit);
	 it->type = AItemType::IT_UNFINISHED;
	 ItemTypes->AddItemType(it);
     it->weight = 0;
	 AddEvent(AnsiString("New item ") + it->name + " added");
	 return true;
  }
  AnsiString mes;
  TEditItemTypeForm *eitf;
  if(!it){
  it=new AItemType;
  it->CreateNew(newit);
  eitf=new TEditItemTypeForm(0,it,0,descr);
  eitf->Caption=eitf->Caption+": New item found";
  if(eitf->ShowModal()!=mrOk){delete eitf;return false;}
  delete eitf;
  ItemTypes->AddItemType(it);
  AddEvent(AnsiString("New item ")+it->Print(1)+" added");
 }else{
  if(!it->IsNewInfo(newit))return true;
  eitf=new TEditItemTypeForm(0,it,newit,descr);
  eitf->Caption=eitf->Caption+": item changes found";
  if(eitf->ShowModal()!=mrOk){delete eitf;return false;}
  delete eitf;
  mes="Item "+it->FullName()+" updated";
  AddEvent(mes);
 }
 return true;
}
//---------------------------------------------------------------------------
bool ProcessUpdateTownType(AnsiString newttype){
 int ind=TownTypes->GetIndex(newttype);
 if(ind!=-1)return true;
 AnsiString mes="New Town type found: "+newttype;
 mes+="\nAdd it?";
 if(Application->MessageBox(mes.c_str(),"Updating Town types",MB_YESNO)!=IDYES){
  return false;
 }
 ATownType *ttype=new ATownType;
 ttype->name=newttype;
 TownTypes->AddTownType(ttype);
 mes="new town types \""+newttype+"\" added";
 AddEvent(mes);
 return true;
}
//---------------------------------------------------------------------------
bool ProcessUpdateRegionArray(AnsiString newname){
 int ind=curRegionList->RegionArrays->GetLevelNum(newname);
 if(ind!=-1)return true;
 AnsiString mes="New Level found: "+newname;
 mes+="\nAdd it?";
 if(Application->MessageBox(mes.c_str(),"Updating Level",MB_YESNO)!=IDYES){
  return false;
 }
/* ARegionArray *ra=new ARegionArray;
 ra->name=newname;
 curRegionList->RegionArrays->Add(ra);*/
 mes="new level \""+newname+"\" added";
 AddEvent(mes);
 ATurn *turn0=curTurns->Get(0);
 if(!turn0)return false;
// ra=turn0->RegionList->RegionArrays->Get(newname);
// if(!ra){
  ARegionArray *ra=new ARegionArray;
  ra->name=newname;
  turn0->RegionList->RegionArrays->Add(ra);
 turn0->Modified=true;
  turn0->VerifyNextTurn();
// }
 return true;
}
//---------------------------------------------------------------------------
bool ProcessUpdateRegion(ARegion *newreg){
 int oldturn=curTurn->num;
 {
   if(RegionTurnNum!=-1)
   {
     if(UseRegionTurnNum==-1){
       AnsiString query="Region info contains turn number. Use it?";
       int ret=MessageDlg(query,mtCustom,TMsgDlgButtons()<<mbYes<<mbNo<<mbCancel,0);
       switch(ret)
       {
         case mrCancel: return false;
         case mrNo: UseRegionTurnNum=0; break;
         case mrYes: UseRegionTurnNum=1; break;
       }
     }
   }
   if(!UseRegionTurnNum)
   {
      RegionTurnNum=-1;
      if(newreg->hasInfo==true)
        newreg->oldinfoturnnum=curTurnNumber;
   }
   if(RegionTurnNum!=-1)
   {
     ATurn *lastturn=curTurns->Get(curTurns->count-1);
     ARegion *reg=lastturn->RegionList->Get(newreg->xloc,newreg->yloc,newreg->zloc);
     if(reg)
     {
       if(reg->oldinfoturnnum>RegionTurnNum)
       {
         AnsiString mes="Region ";
         mes+=reg->FullName();
         mes+=" already has data in turn ";
         mes+=reg->oldinfoturnnum;
         mes+=". Ignoring data from turn ";
         mes+=RegionTurnNum;
         AddEvent(mes);
         return true;
       }
     }
     if(oldturn!=RegionTurnNum)
     {
       if(!curTurns->Get(RegionTurnNum)&&!curTurns->PreloadTurn(RegionTurnNum))
       {
         AnsiString mes="Can't load turn ";
         mes+=RegionTurnNum;
         mes+=". Loading to turn 0 region ";
         mes+=newreg->FullName();
         AddEvent(mes);
         newreg->oldinfoturnnum=0;
         RegionTurnNum=0;
       }
	   curTurns->SetCurTurn(RegionTurnNum);
	   if(MinTurnNum>RegionTurnNum)
		 MinTurnNum=RegionTurnNum;
	 }
   }
 }
 ARegion *reg=curRegionList->Get(newreg->xloc,newreg->yloc,newreg->zloc);
 bool bFullInf = true;
 if(!reg){
  reg = new ARegion(curTurn);
//  reg->turn=curTurnNumber;
  reg->xloc=newreg->xloc;
  reg->yloc=newreg->yloc;
  reg->zloc=newreg->zloc;
  reg->UpdateKey();
  curRegionList->Add(reg);
 }else {
   if( ((reg->products->count > 0) && (newreg->products->count == 0)) ||
	   ((reg->population > 0) && (newreg->population == 0)) ) bFullInf = false;
   if(RegionUpdateMode!=rumAlways&&newreg->hasInfo&&!newreg->PresentLocals()){
	   bool presentinfo = reg->hasInfo && reg->oldinfoturnnum==curTurn->num && bFullInf;
	   if(presentinfo)
	   {
		 if(RegionUpdateMode==rumNotUpdate)
		   return true;
		 AnsiString query="Region info for "+reg->FullName()
		   +"\nalready present for current turn.\nReplace it?";
		 int ret=MessageDlg(query,mtCustom,TMsgDlgButtons()<<mbYes<<mbNo<<mbCancel<<mbYesToAll<<mbNoToAll,0);
		 switch(ret)
		 {
		   case mrCancel: return false;
		   case mrNoToAll:RegionUpdateMode=rumNotUpdate; //no break;
		   case mrNo: return true;
		   case mrYesToAll:RegionUpdateMode=rumAlways;break;
		 }
	   }
   }
 }
 if(!bFullInf) return true;
 if(RegionTurnNum != -1) curTurns->LockTurn(RegionTurnNum);
 reg->Update(newreg);
 curTurn->Modified = true;
 ARegionArray *ra = curRegionList->GetRegionArray(reg->zloc);
 ra->VerifySize(reg->xloc, reg->yloc);
 if(RegionTurnNum!=-1) curTurns->UnlockTurn(RegionTurnNum,true);
 return true;
}
//---------------------------------------------------------------------------
bool ProcessUpdateObjectType(AnsiString name,int canenter){
 AObjectType *ot=ObjectTypes->Get(name);
 AnsiString mes;
 if(ot){
  if(canenter==2)return true;
  if(ot->canenter==2){
   ot->canenter=canenter;
   return true;
  }
  if(ot->canenter!=canenter){
   mes="Object \""+name+"\" canenter status changing detected\n";
   mes+="Old value: ";
   mes+=ot->canenter;
   mes+="\nNew value: ";
   mes+=canenter;
   mes+="\nChange it?";
   int r=Application->MessageBox(mes.c_str(),"Updating Object type",MB_YESNOCANCEL);
   if(r==IDCANCEL)return false;
   if(r==IDYES){
    ot->canenter=canenter;
   }
  }
  return true;
 }
 ot=new AObjectType;
 ot->name=name;
 ot->canenter=canenter;
 mes="New Object Type : "+name+" added";
 ObjectTypes->AddObjectType(ot);
 AddEvent(mes);
 return true;
}
//---------------------------------------------------------------------------
bool SaveReport(AnsiString fName,int facnum){
 if(!curFaction||curFaction->number!=facnum){
  curFactions->SetCurFaction(facnum);
 }
 if(!curFaction||curFaction->number!=facnum)return false;
 TFileStream *file=0;
 try{
  file=new TFileStream(fName,fmCreate);
 }catch(Exception &c){
  Application->ShowException(&c);
 }
 if(!file)return false;
 ReportTab=0;
 try{
  if(!curFaction->SaveReport(file))return false;
 }catch(Exception&e){
  delete file;
  Application->MessageBox(e.Message.c_str(),"error saving report",MB_OK);
  return false;
 }
 delete file;
 AnsiString s="Report for faction ";
 s+=curFaction->FullName();
 s+=" saved";
 AddEvent(s);
 return true;
}
//---------------------------------------------------------------------------
bool SaveOrder(AnsiString fName,int facnum){
 Plugins.OnSaveOrder(facnum,true);
 if(!curFaction||curFaction->number!=facnum){
  curFactions->SetCurFaction(facnum);
 }
 if(!curFaction||curFaction->number!=facnum)return false;
 TFileStream *file=0;
 try{
  file=new TFileStream(fName,fmCreate);
 }catch(Exception &c){
  Application->ShowException(&c);
 }
 if(!file)return false;
 try{
  if(!curFaction->SaveTemplate(file))return false;
 }catch(Exception&e){
  delete file;
  Application->MessageBox(e.Message.c_str(),"error saving order",MB_OK);
  return false;
 }
 delete file;
 AnsiString s="Order for faction ";
 s+=curFaction->FullName();
 s+=" saved";
 AddEvent(s);
 Plugins.OnSaveOrder(facnum,false);
 return true;
}
//---------------------------------------------------------------------------
bool SaveMap(AnsiString fName,int mapoptions){
 TFileStream *file=0;
 try{
  file=new TFileStream(fName,fmCreate);
 }catch(Exception &c){
  Application->ShowException(&c);
 }
 if(!file)return false;
 try{
  if(!curTurn->SaveMap(file,mapoptions))return false;
 }catch(Exception&e){
  delete file;
  Application->MessageBox(e.Message.c_str(),"error saving map",MB_OK);
  return false;
 }
 delete file;
 AddEvent("Map saved");
 return true;
}
//---------------------------------------------------------------------------
bool ExtractBattleUnitName(char *&s,int &num,AnsiString *name){
 char *olds=s;
 while(*s&&*s!='\n'&&*s!='(') s++;
 if(*s!='('){
  s=olds;
  return false;
 }
 AnsiString _name(olds,s-olds-1);
 if(name) *name=_name;
 s++;
 num=atoi(GetNextToken(s).c_str());
 if(*s!=')'){
  s=olds;
  return false;
 }
 s++;
 return true;
}
//---------------------------------------------------------------------------
int IsBattleReport(char *s, ALocation *loc){
  int num;
  int type;
  if(!ExtractBattleUnitName(s, num, 0)) return false;
  if(!MyCompare(s,KeyPhrases->Get(keyBattleBeginNorm)))
  {
	s+=KeyPhrases->Get(keyBattleBeginNorm).Length();
	type=1;
	if(!ExtractBattleUnitName(s,num,0)) return false;
	if(MyCompare(s,KeyPhrases->Get(keyInSign))) return false;
	s+=KeyPhrases->Get(keyInSign).Length();
  } else
  if(!MyCompare(s,KeyPhrases->Get(keyBattleBeginAss)))
  {
	s+=KeyPhrases->Get(keyBattleBeginAss).Length();
	type=2;
	if(!ExtractBattleUnitName(s,num,0)) return false;
	if(MyCompare(s,KeyPhrases->Get(keyInSign))) return false;
	s+=KeyPhrases->Get(keyInSign).Length();
  } else
  if(!MyCompare(s,KeyPhrases->Get(keyBattleAssassinated)))
  {
	s+=KeyPhrases->Get(keyBattleAssassinated).Length();
	type=3;
  } else return false;
 ARegion *reg=ProcessRegionHeader(s);
 if(!reg) return false;
 if(loc) *loc=*reg;
 delete reg;
 return type;
}
//---------------------------------------------------------------------------
bool ProcessBattleUnits(char*&s,AUnits *uns){
 AnsiString name;
 int num;
 while(*s&&*s!='\n'){
  if(!ExtractBattleUnitName(s,num,&name))
   return false;
  AUnit *un=new AUnit(0);
  un->num=num;
  un->name=name;
  un->turn=curTurn;
  AnsiString str=GetNextString(s,1);
  char *ss=str.c_str();
  { //skip fraction
    char *sss=ss+1;
    while(*sss&&*sss!='('&&*sss!=',') sss++;
    if(*sss=='('&&isdigit(sss[1]))
    {
      ss=sss;
      ss++;
      int facnum=atoi(ss);
      un->SetFaction(facnum);
      while(*ss&&*ss!=',') ss++;
    }
  }
  if(!MyCompare(ss,KeyPhrases->Get(keyUnitBehind))){
   ss+=KeyPhrases->Get(keyUnitBehind).Length();
   un->SetFlag(FLAG_BEHIND,true);
  }
  while(*ss==','){
   ss++;
   if(*ss!=' ')return false;
   ss++;
   int count=1;
   if(*ss>='0'&&*ss<='9'){
    count=atoi(GetNextToken(ss).c_str());
    if(*ss!=' ')return false;
    ss++;
   }
   AnsiString itname=GetNextItemName(ss);
   if(!itname.Length()){
    AnsiString skillname=GetNextToken(ss);
    ASkillType *stype=SkillTypes->Get(skillname);
    if(!stype){
     AddEvent("Unknown skill: "+skillname);
     return false;
    }
    if(*ss!=' ')return false;
    ss++;
    if(*ss<'0'||*ss>'9')
     return false;
    int level=atoi(GetNextToken(ss).c_str());
    ASkill *sk=new ASkill;
    sk->type=stype;
    sk->SetLevel(level);
    un->skills->Add(sk);
    if(stype->flags&ASkillType::COMBAT){
     un->combatspell=stype->abr; 
    }

   }else{
    AnsiString itabr=GetNextToken(ss);
    if(*ss!=']') return false;
    ss++;
    AItemType *itype=new AItemType;
    if(count==1) itype->name=itname;
    else itype->names=itname;
    bool isillus=false;
    if(*ss==' '&&ss[1]=='('){
     ss+=2;
//- verif monster in battle unit     
     while(*ss&&*ss!=')') ss++;
     ss++;
    }

    if(!isillus && un->items->GetItem(itabr)){
     AnsiString mes;
     mes="Possible illusion item detected \""+itabr+"\"";
     AddEvent(mes);
     isillus=true;
    }
    if(isillus){
     itype->abr=ItemTypes->NormalAbrToIllusion(itabr);
    }else{
     itype->abr=itabr;
    }
    itype->weight=-1;
    if(!ProcessUpdateItem(itype))return false;
    itabr=itype->abr;
    delete itype;
    AItem *it=new AItem;
    it->count=count;
    it->SetType(itabr);
    un->items->Add(it);
   }
  }
  if(*ss==';'){
   AnsiString descr=GetNextString(ss);
   int dlen=descr.Length();
   if(dlen){
    if(descr[dlen]=='.') descr.SetLength(dlen-1);
   }
   un->describe=descr;
  }else if(*ss!='.') return false;
  uns->Add(un);
 }
 return true;
}
//---------------------------------------------------------------------------
bool ProcessBattleRound(char*&s,ABattleRepRound *round){
 int unitnum,loses;
 bool firstloses=true;
 while(*s&&*s!='\n'){
  if(!ExtractBattleUnitName(s,unitnum,0))
   return false;
  if(MyCompare(s,KeyPhrases->Get(keyBattleLoses))){
   round->AddCast(unitnum,GetNextString(s,1));
   continue;
  }
  s+=KeyPhrases->Get(keyBattleLoses).Length();
  loses=atoi(GetNextToken(s).c_str());
  if(*s!='.')
   return false;
  s++;
  if(*s=='\n') s++;
  if(round->round>=0){
   if(firstloses){
    firstloses=false;
    round->attloses=loses;
   }else{
    round->defloses=loses;
   }
  }else{
   round->SetFreeRoundLoses(loses);
   break;
  }
 }
 if(*s=='\n') s++;
 return true;
}
//---------------------------------------------------------------------------
bool ProcessBattleReport(char*&s,ABattleReport *rep){
 if(!rep) return false;
 rep->turnnum=curTurn->num;
 AnsiString str=GetNextString(s,1);
 char *ss=s;
 s=str.c_str();
 if(!IsBattleReport(s)) return false;
 rep->SetSides(0,0);
 rep->Reset();
 int attleadnum,defleadnum,unitnum;
 if(!ExtractBattleUnitName(s,attleadnum,0))
  return false;
 if(!MyCompare(s,KeyPhrases->Get(keyBattleBeginNorm))){
  s+=KeyPhrases->Get(keyBattleBeginNorm).Length();
  rep->ass=false;
 }else if(!MyCompare(s,KeyPhrases->Get(keyBattleBeginAss))){
  s+=KeyPhrases->Get(keyBattleBeginAss).Length();
  rep->ass=true;
 }else
  return false;
 if(!ExtractBattleUnitName(s,defleadnum,0))
  return false;
 if(MyCompare(s,KeyPhrases->Get(keyInSign)))
  return false;
 s+=KeyPhrases->Get(keyInSign).Length();
 ARegion *reg=ProcessRegionHeader(s);
 if(!reg) return false;
 rep->reg->type=reg->type;
 rep->reg->xloc=reg->xloc;
 rep->reg->yloc=reg->yloc;
 rep->reg->zloc=reg->zloc;
 rep->reg->UpdateKey();
 rep->reg->name=reg->name;
 delete reg;
 s=ss;
 if(*s=='\n') s++;
 if(MyCompare(s,KeyPhrases->Get(keyBattleAttackers)))
  return false;
 s+=KeyPhrases->Get(keyBattleAttackers).Length();
 if(*s=='\n') s++;
 if(!ProcessBattleUnits(s,rep->atts))
  return false;
 if(*s=='\n') s++;
 if(MyCompare(s,KeyPhrases->Get(keyBattleDefenders)))
  return false;
 s+=KeyPhrases->Get(keyBattleDefenders).Length();
 if(*s=='\n') s++;
 if(!ProcessBattleUnits(s,rep->defs))
  return false;
 if(*s=='\n') s++;
 rep->attleadernum=attleadnum;
 rep->defleadernum=defleadnum;
 rep->UpdateLeaders();
 while(1){
  if(!MyCompare(s,KeyPhrases->Get(keyBattleTotalCasual)))
   break;
  if(!MyCompare(s,KeyPhrases->Get(keyBattleTie)))
   break;
  if(!MyCompare(s,KeyPhrases->Get(keyBattleTieAss)))
   break;
  ABattleRepRound *round=new ABattleRepRound;
  if(!MyCompare(s,KeyPhrases->Get(keyBattleNormalRound))){
   s+=KeyPhrases->Get(keyBattleNormalRound).Length();
   round->round=atoi(GetNextToken(s).c_str());
   if(*s!=':')
    return false;
   s++;
   if(*s=='\n') s++;
   if(!ProcessBattleRound(s,round))
    return false;
   rep->Add(round);
   continue;
  }
  if(!ExtractBattleUnitName(s,unitnum,0))
   return false;
  bool win=false;
  if(!MyCompare(s,KeyPhrases->Get(keyBattleRouted))){
   s+=KeyPhrases->Get(keyBattleRouted).Length();
   win=true;
  }else if(!MyCompare(s,KeyPhrases->Get(keyBattleDestroyed))){
   s+=KeyPhrases->Get(keyBattleDestroyed).Length();
   win=true;
  }
  if(win){
   if(unitnum==attleadnum){
    rep->wintype=wtWinDef;
   }else if(unitnum==defleadnum){
    rep->wintype=wtWinAtt;
   }
   if(*s=='\n') s++;
   if(!ExtractBattleUnitName(s,unitnum,0))
    break;
  }

  if(MyCompare(s,KeyPhrases->Get(keyBattleFreeRound)))
   break;
  s+=KeyPhrases->Get(keyBattleFreeRound).Length();
  if(*s=='\n') s++;
  if(unitnum==attleadnum){
   round->round=-2;
  }else if(unitnum==defleadnum){
   round->round=-1;
  }
  if(!ProcessBattleRound(s,round))
   return false;
  rep->Add(round);
 }
 if(!MyCompare(s,KeyPhrases->Get(keyBattleTie))){
  GetNextString(s);
  rep->wintype=wtTie;
  if(*s=='\n') s++;
 }
 if(!MyCompare(s,KeyPhrases->Get(keyBattleTieAss))){
  GetNextString(s);
  rep->wintype=wtTie;
  if(*s=='\n') s++;
 }

 if(MyCompare(s,KeyPhrases->Get(keyBattleTotalCasual)))
  return false;
 GetNextString(s);
 for(int i=0;i<2;i++){
  while(1){
   if(!ExtractBattleUnitName(s,unitnum,0))
    return false;
   if(!MyCompare(s,KeyPhrases->Get(keyBattleLoses)))
    break;
   rep->AddHeal(i,unitnum,GetNextString(s));
   continue;
  }
  s+=KeyPhrases->Get(keyBattleLoses).Length();
  int loses=0;
  AnsiString losesstr;
  ss=s;
  if(isdigit(*s))
    loses=atoi(GetNextToken(s).c_str());
  if(*s=='.')
  {
    s++;
    if(*s=='\n') s++;
  }else{
    s=ss;
    losesstr=GetNextString(s,1);
    ss=losesstr.c_str();
    loses=0;
    while(*ss&&*ss!='.')
    {
      int count=1;
      if(*ss>='0'&&*ss<='9'){
        count=atoi(GetNextToken(ss).c_str());
        if(*ss!=' ')return false;
        ss++;
      }
      AnsiString itname=GetNextItemName(ss);
      if(!itname.Length()) break;
//- may be return false
      AnsiString itabr=GetNextToken(ss);
      if(*ss!=']') return false;
      ss++;
      AItemType *itype=ItemTypes->Get(itabr,4);
      if(!itype) return false;
      if(itype->type&(AItemType::IT_MAN|AItemType::IT_MONSTER))
        loses+=count;
      if(*ss=='.') continue;
      if(*ss!=',') return false;
      ss++;
      if(*ss!=' ') return false;
      ss++;
    }
  }
  AUnits *dams;
  if(i==0){
   rep->loses1=loses;
   rep->losesstr1=losesstr;
   dams=rep->damaged1;
  }else{
   rep->loses2=loses;
   rep->losesstr2=losesstr;
   dams=rep->damaged2;
  }
  if(!loses) continue;
  if(MyCompare(s,KeyPhrases->Get(keyBattleDamaged)))
   return false;
  s+=KeyPhrases->Get(keyBattleDamaged).Length();
  AnsiString str=GetNextString(s,1);
  ss=str.c_str();
  while(1){
   int num=atoi(GetNextToken(ss).c_str());
   AUnit *un=rep->atts->GetUnit(num);
   if(!un) un=rep->defs->GetUnit(num);
   if(!un) return false;
   dams->Add(un);
   if(*ss=='.') break;
   if(*ss!=','&&ss[1]!=' ') return false;
   ss+=2;
  }
 }
 if(*s=='\n') s++;
 if(!MyCompare(s,KeyPhrases->Get(keyBattleNoSpoils))){
  GetNextString(s);
 }else{
  if(!MyCompare(s,KeyPhrases->Get(keyBattleSpoils))){
   s+=KeyPhrases->Get(keyBattleSpoils).Length();
   AnsiString str=GetNextString(s,1);
   ss=str.c_str();
   if(!ProcessItemsList(ss,rep->spoils))
    return false;
  }
 }
 if(*s=='\n') s++;
 rep->CompactUnits(); 
 return true;
}
//---------------------------------------------------------------------------
void SkipEmptyLines(char*&s)
{
 while(*s==';'||*s=='\n') GetNextString(s);
}
//---------------------------------------------------------------------------

