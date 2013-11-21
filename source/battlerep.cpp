//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "battlerep.h"
#include "unit.h"
#include "region.h"
#include "items.h"
#include "battle.h"
#include "skills.h"
#include "turn.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


ABattleRepRound::ABattleRepRound()
 :round(0),casts(new TStringList)
{
}
ABattleRepRound::~ABattleRepRound()
{
 delete casts;
}
bool ABattleRepRound::Print(ABattleReport * rep,TStringList *list, bool shortmode)
{
 AnsiString str;
 if(round>=0){
  str+=KeyPhrases->Get(keyBattleNormalRound);
  str+=round;
  str+=":";
 }else if(round==-2){
  str+=rep->attleadername+KeyPhrases->Get(keyBattleFreeRound);
 }else
  str+=rep->defleadername+KeyPhrases->Get(keyBattleFreeRound);
 list->Add(str);
 if(!shortmode)
 for(int i=0,endi=casts->Count;i<endi;i++){
  int num=(int)casts->Objects[i];
  str=rep->GetUnitName(num)+casts->Strings[i];
  list->Add(str);
 }
 if(round!=-2)
  list->Add(rep->attleadername+KeyPhrases->Get(keyBattleLoses)+attloses+".");
 if(round!=-1)
  list->Add(rep->defleadername+KeyPhrases->Get(keyBattleLoses)+defloses+".");
 list->Add("");
 return true;
}
void ABattleRepRound::AddCast(int num, AnsiString str)
{
 casts->AddObject(str,(TObject*)num);
}
void ABattleRepRound::SetFreeRoundLoses(int loses)
{
 if(round>=0)
  return;
 if(round==-2){
  defloses=loses;
 }else{
  attloses=loses;
 }
}

__fastcall ABattleReport::ABattleReport()
 :heals1(new TStringList),heals2(new TStringList)
 ,turnnum(0),roundlimit(-1)
{
 reg=new ARegion(0);
 reg->xloc=0;
 reg->yloc=0;
 reg->zloc=0;
 reg->UpdateKey();
 reg->type="";
 AObject *obj=reg->GetDummy();
 atts=new AUnits(obj);
 defs=new AUnits(obj);
 damaged1=new AUnits(obj);
 damaged1->autodelete=false;
 damaged2=new AUnits(obj);
 damaged2->autodelete=false;
 spoils=new AItems;
}

__fastcall ABattleReport::~ABattleReport()
{
 Clear();
 delete damaged1;
 delete damaged2;

 delete atts;
 delete defs;
 delete reg;

 delete heals1;
 delete heals2;
 delete spoils;
}
AnsiString ABattleReport::GetUnitName(int num){
 AUnit *un=atts->GetUnit(num);
 if(un) return un->FullName(true);
 un=defs->GetUnit(num);
 if(un) return un->FullName(true);
 return "???";
}

void __fastcall ABattleReport::Clear()
{
 int i=Count-1;
 if(i<0) return;
 ABattleRepRound **iter=(ABattleRepRound**)List+i;
 for(;i>=0;i--,iter--){
  delete *iter;
 }
 TList::Clear();
}
void ABattleReport::Add(ABattleRepRound * round)
{
 TList::Add(round);
}
ABattleRepRound* ABattleReport::Get(int ind)
{
 return (ABattleRepRound*)TList::Get(ind);
}
void ABattleReport::SetSides(AUnits * _atts, AUnits * _defs)
{
 atts->Clear();
 if(_atts)
 foreach(_atts){
  AUnit *un=*(AUnit**)iter;
  AUnit *newun=un->CreateBattleUnit();
  atts->Add(newun);
 }
 defs->Clear();
 if(_defs)
 foreach(_defs){
  AUnit *un=*(AUnit**)iter;
  AUnit *newun=un->CreateBattleUnit();
  defs->Add(newun);
 }
 Reset();
}
void ABattleReport::UpdateLeaders()
{
 AUnit *un;
 un=atts->GetUnit(attleadernum);
 if(!un&&atts->count)
  un=atts->Get(0);
 if(un)
  attleadername=un->FullName(true);
 else
  attleadername="??";
 un=defs->GetUnit(defleadernum);
 if(!un&&defs->count)
  un=defs->Get(0);
 if(un)
  defleadername=un->FullName(true);
 else
  defleadername="??";
}
void ABattleReport::Reset()
{
 ass=false;
 heals1->Clear();
 heals2->Clear();
 loses1=0;
 loses2=0;
 losesstr1.SetLength(0);
 losesstr2.SetLength(0);
 damaged1->Clear();
 damaged2->Clear();
 spoils->Clear();
 Clear();
 UpdateLeaders();
}
void ABattleReport::SetRegion(ARegion * _reg)
{
 reg->xloc=_reg->xloc;
 reg->yloc=_reg->yloc;
 reg->zloc=_reg->zloc;
 reg->UpdateKey();
 reg->type=_reg->type;
 reg->name=_reg->name;
 reg->towntype=_reg->towntype;
 reg->townname=_reg->townname;
}


void ABattleReport::Print(TStringList * list, bool shortmode)
{
 list->Add(attleadername+ KeyPhrases->Get(keyBattleBeginNorm)
           + defleadername +KeyPhrases->Get(keyInSign)+reg->ShortName() + "!");
 list->Add("");
 WriteSides(list);
 if(!count) return;
 bool first=true,showedrouted=false;
 foreach(this){
  ABattleRepRound *round=*(ABattleRepRound**)iter;
  if(round->round<0&&!first){
   if(round->round==-2){
    list->Add(defleadername+KeyPhrases->Get(keyBattleRouted));
    showedrouted=true;
   }else{
    list->Add(attleadername+KeyPhrases->Get(keyBattleRouted));
    showedrouted=true;
   }
  }
  first=false;
  round->Print(this,list,shortmode);
 }
 if(!showedrouted){
  if(wintype==wtWinAtt)
   list->Add(defleadername+KeyPhrases->Get(keyBattleRouted));
  else if(wintype==wtWinDef)
   list->Add(attleadername+KeyPhrases->Get(keyBattleRouted));
 }
 if(wintype==wtTie){
  list->Add(KeyPhrases->Get(keyBattleTie));
  list->Add("");
 }
 list->Add(KeyPhrases->Get(keyBattleTotalCasual));
 if(!shortmode)
 for(int i=0,endi=heals1->Count;i<endi;i++){
  int num=(int)heals1->Objects[i];
  list->Add(GetUnitName(num)+heals1->Strings[i]);
 }
 if(wintype!=wtWinAtt){
  AnsiString str=attleadername+KeyPhrases->Get(keyBattleLoses);
  if(losesstr1.Length())
    str+=losesstr1;
  else
    str+=AnsiString(loses1)+".";
  list->Add(str);
 }else{
  AnsiString str=defleadername+KeyPhrases->Get(keyBattleLoses);
  if(losesstr1.Length())
    str+=losesstr1;
  else
    str+=AnsiString(loses1)+".";
  list->Add(str);
 }
 PrintDamaged(damaged1,list);
 if(!shortmode)
 for(int i=0,endi=heals2->Count;i<endi;i++){
  int num=(int)heals2->Objects[i];
  list->Add(GetUnitName(num)+heals2->Strings[i]);
 }
 if(wintype!=wtWinAtt){
  AnsiString str=defleadername+KeyPhrases->Get(keyBattleLoses);
  if(losesstr2.Length())
    str+=losesstr2;
  else
    str+=AnsiString(loses2)+".";
  list->Add(str);
 }else{
  AnsiString str=attleadername+KeyPhrases->Get(keyBattleLoses);
  if(losesstr2.Length())
    str+=losesstr2;
  else
    str+=AnsiString(loses2)+".";
  list->Add(str);
 }
 PrintDamaged(damaged2,list);
 list->Add("");
 if(wintype!=wtTie){
  if(spoils->count)
   list->Add(KeyPhrases->Get(keyBattleSpoils)+spoils->WriteReport(2,false,true)+".");
  else
   list->Add(KeyPhrases->Get(keyBattleNoSpoils));
  list->Add("");
 }
}

void ABattleReport::WriteSides(TStringList *list)
{
 list->Add(KeyPhrases->Get(keyBattleAttackers));
 foreach(atts){
  AUnit *un=*(AUnit**) iter;
  list->Add(un->BattleReport());
 }
 list->Add("");
 list->Add(KeyPhrases->Get(keyBattleDefenders));
 foreach(defs){
  AUnit *un=*(AUnit**) iter;
  list->Add(un->BattleReport());
 }
 list->Add("");
}
void ABattleReport::AddHeal(int ind,int num, AnsiString str)
{
 TStringList *heals;
 if(ind==1)
  heals=heals2;
 else
  heals=heals2;
 heals->AddObject(str,(TObject*)num);
}
void ABattleReport::PrintDamaged(AUnits * uns,TStringList *list)
{
 if(!uns->count) return;
 AnsiString str=KeyPhrases->Get(keyBattleDamaged);
 bool first=true;
 foreach(uns){
  AUnit *un=*(AUnit**)iter;
  if(!first)
   str+=", ";
  first=false;
  str+=un->num;
 }
 str+=".";
 list->Add(str);
}


void ABattleReport::Run()
{
 if(!atts->count) return;
 if(!defs->count) return;
 Reset();
 foreach(atts){
  AUnit *u=*(AUnit**)iter;
  u->enditems->CreateNew(u->items);
 }
 foreach(defs){
  AUnit *u=*(AUnit**)iter;
  u->enditems->CreateNew(u->items);
 }
 ABattle *battle=new ABattle;
 battle->Run(this);
 delete battle;
}
void ABattleReport::CompactUnits()
{
 attcount=0;
 for(int i=atts->count-1;i>=0;i--){
  AUnit *un=atts->Get(i);
  un->enditems->CreateNew(un->items);
  int num=un->GetSoldiers();
  attcount+=num;
  if(num) continue;
  atts->Delete(un->num);
 }
 defcount=0;
 for(int i=defs->count-1;i>=0;i--){
  AUnit *un=defs->Get(i);
  un->enditems->CreateNew(un->items);
  int num=un->GetSoldiers();
  defcount+=num;
  if(num) continue;
  defs->Delete(un->num);
 }
}
int ABattleReport::PresentUnit(int num)
{
 if(atts->GetUnit(num)) return 1;
 if(defs->GetUnit(num)) return 2;
 return 0;
}


/*void ABattleReport::CreateNew(ABattleReport * rep)
{
//to
}*/

/*void ABattleReport::AutoSetCombatSpell(){
 AutoSetCombatSpell(atts);
 AutoSetCombatSpell(defs);
}
void ABattleReport::AutoSetCombatSpell(AUnits *uns){
 foreach(uns){
  AUnit *u=*(AUnit**)iter;
  if(u->combatspell.Length()){
   int lev=u->GetBegSkill(u->combatspell);
   if(lev) continue;
   u->combatspell="";
  }
  foreach(u->skills){
   ASkill *sk=*(ASkill**)iter;
   if(!(sk->type->flags&ASkillType::COMBAT)) continue;
   ASpellType*stype=SpellTypes->Get(sk->type->abr);
   if(!stype) continue;
   if(!stype->GetCombatCount()) continue;
   u->combatspell=sk->type->abr;
  }
 }
}*/


int __fastcall ABattleReport::GetAttLoses()
{
 if(wintype!=wtWinAtt)
  return loses1;
 return loses2;
}

int __fastcall ABattleReport::GetDefLoses()
{
 if(wintype==wtWinAtt)
  return loses1;
 return loses2;
}
void ABattleReport::Check()
{
  if(turnnum<curTurns->count)
  {
    ATurn *t=curTurns->Get(turnnum);
    if(t)
      return;
  }
  foreach(atts)
  {
    AUnit *un=*(AUnit**)iter;
    un->faction=0;
  }
  foreach(defs)
  {
    AUnit *un=*(AUnit**)iter;
    un->faction=0;
  }
  turnnum=0;
}

