//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "battle.h"
#include "army.h"
#include "unit.h"
#include "region.h"
#include "items.h"
#include "battlerep.h"
#include "exprlang.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

ABattle::ABattle()
{

}

ABattle::~ABattle()
{
}
void ABattle::FreeRound(ABattleRepRound *rep,AArmy *att,AArmy *def){
 att->shields->Clear();
 UpdateShields(rep,att);
 def->shields->Clear();
 UpdateShields(rep,def);

 att->round++;
 int alv=def->NumAlive();
 while(att->CanAttack()&&def->NumAlive()){
  int num=getrandom(att->CanAttack());
  bool behind;
  ASoldier *a=att->GetAttacker(num,behind);
  DoAttack(rep,att->round,a,att,def,behind);
 }
 alv-=def->NumAlive();
 rep->SetFreeRoundLoses(alv);
 att->Reset();
}
void ABattle::DoAttack(ABattleRepRound *rep,int round, ASoldier * a, AArmy * atts, AArmy * defs, bool behind)
{
 DoSpecialAttack(rep,round,a,atts,defs,behind);
 if(!defs->NumAlive()) return;
 int numattacks=a->attacks;
 if(numattacks<0){
  if(round%(-1*numattacks)==1){
   numattacks=1;
  }else{
   numattacks=0;
  }
 }
 AWeaponType *wtype=a->weapon;
 for(int i=0;i<numattacks;i++){
  if(behind){
   if(!wtype) break;
   if(!(wtype->flags&AWeaponType::RANGED)) break;
  }
  int flags=AWeaponType::SHORT;
  int attackType=ABattleEnums::ATTACK_COMBAT;
  int mountBonus=0;
  int attackClass=AWeaponType::SLASHING;
  if(wtype){
   flags=wtype->flags;
   attackType=wtype->attackType;
   mountBonus=wtype->mountBonus;
   attackClass=wtype->weapClass;
  }
  defs->DoAnAttack(0,1,attackType,a->askill,flags,attackClass,"",mountBonus);
  if(!defs->NumAlive()) break;
 }
}
void ABattle::NormalRound(ABattleRepRound *rep,AArmy *a,AArmy *b){

 UpdateShields(rep,a);
 UpdateShields(rep,b);

 a->round++;
 b->round++;
 int aalive=a->NumAlive();
 int aialive=aalive;
 int balive=b->NumAlive();
 int bialive=balive;
 int aatt=a->CanAttack();
 int batt=b->CanAttack();

 while(aalive&&balive&&(aatt||batt)){
  int num = getrandom(aatt + batt);
  bool behind;
  if(num>=aatt){
   num-=aatt;
   ASoldier *s=b->GetAttacker(num, behind);
   DoAttack(rep,b->round, s, b, a, behind);
  }else{
   ASoldier *s=a->GetAttacker(num, behind);
   DoAttack(rep,a->round, s, a, b, behind);
  }
  aalive = a->NumAlive();
  balive = b->NumAlive();
  aatt = a->CanAttack();
  batt = b->CanAttack();
 }

 aialive -= aalive;
 rep->attloses=aialive;
 bialive -= balive;
 rep->defloses=bialive;
 a->Reset();
 b->Reset();
}

void ABattle::Run(ABattleReport *rep)
{
 AArmy *attarmy;
 AArmy *defarmy;

 attarmy=new AArmy(rep->atts,rep->reg->type,false);
 defarmy=new AArmy(rep->defs,rep->reg->type,false);

 if(attarmy->tac>defarmy->tac){
  ABattleRepRound *round=new ABattleRepRound;
  round->round=-2;
  FreeRound(round,attarmy,defarmy);
  rep->Add(round);
 }else if(defarmy->tac>attarmy->tac){
  ABattleRepRound *round=new ABattleRepRound;
  round->round=-1;
  FreeRound(round,defarmy,attarmy);
  rep->Add(round);
 }

 int roundnum = 1;
 while(!attarmy->Broken()&&!defarmy->Broken()&&roundnum < 101){
  if(rep->roundlimit>=0&&roundnum>rep->roundlimit) break;
  ABattleRepRound *round=new ABattleRepRound;
  round->round=roundnum++;
  NormalRound(round,attarmy,defarmy);
  rep->Add(round);
 }
 if(rep->roundlimit>=0&&roundnum>rep->roundlimit
   &&attarmy->NumAlive()&&defarmy->NumAlive())
 {
  rep->wintype=wtTie;
  attarmy->Tie(rep,true);
  defarmy->Tie(rep,false);
 }else if((attarmy->Broken() && !defarmy->Broken()) ||
  (!attarmy->NumAlive() && defarmy->NumAlive())) {
  rep->wintype=wtWinDef;
  if(attarmy->NumAlive()){
   ABattleRepRound *round=new ABattleRepRound;
   round->round=-1;
   FreeRound(round,defarmy,attarmy);
   rep->Add(round);
  }
  AItems *spoils = new AItems;
  attarmy->Lose(rep);
  GetSpoils(rep->atts, spoils);
  rep->spoils->CreateNew(spoils);
  defarmy->Win(rep);
  delete spoils;
 }else if((defarmy->Broken() && !attarmy->Broken()) ||
  (!defarmy->NumAlive() && attarmy->NumAlive())) {
  rep->wintype=wtWinAtt;
  if (defarmy->NumAlive()){
   ABattleRepRound *round=new ABattleRepRound;
   round->round=-2;
   FreeRound(round,attarmy,defarmy);
   rep->Add(round);
  }
  AItems *spoils = new AItems;
  defarmy->Lose(rep);
  GetSpoils(rep->defs,spoils);
  rep->spoils->CreateNew(spoils);
  attarmy->Win(rep);
  delete spoils;
 }else{
  rep->wintype=wtTie;
  attarmy->Tie(rep,true);
  defarmy->Tie(rep,false);
 }
 delete attarmy;
 delete defarmy;
}
void ABattle::GetSpoils(AUnits * losers, AItems * spoils)
{
 foreach(losers){
  AUnit *u=*(AUnit**)iter;
  int numalive = u->GetSoldiers();
  int numdead = u->losses;
  foreach(u->enditems) {
   AItem *i =*(AItem **)iter;
   if (!i->IsSoldier()) {
    int num=(i->count*numdead+getrandom(numalive+numdead))/
     (numalive + numdead);
    int num2=(num+getrandom(2))/2;
    if(num2)
     spoils->SetNum(i->type,spoils->GetNum(i->type) + num2);
    i->count-=num;
    if(i->count<0) i->count=0;
   }
  }
//  u->enditems->DeleteEmpty();
 }
}
void ABattle::DoSpecialAttack(ABattleRepRound *rep,int round, ASoldier * a, AArmy * atts, AArmy * defs, bool behind){
 ASpellType *stype=a->special;
 if(!stype) return;
 int num=stype->GetAttackCount();
 if(!num) return;
 ExprSetUnitMode(a->unit);
 EDataInt *e=new EDataInt;
 e->value=a->slevel;
 variables[7].data=e;
 AnsiString expr;
 int attack_type;
 int killed=0,castnum;
 for(int i=0;i<num;i++){
  if(!stype->GetAttackParam(i,expr,attack_type))
   continue;
  try{
   castnum=ExprProcessExpr(expr);
  }catch (Exception &exception){
   Application->ShowException(&exception);
   continue;
  }
  killed+=defs->DoAnAttack(a->special,castnum,attack_type,a->slevel,
   AWeaponType::SPECIAL_FLAGS,AWeaponType::ARMORPIERCING,"",0);
 }
 AnsiString str=" kill ";
 str+=killed;
 str+=" by ";
 str+=stype->FullName();
 str+=".";
 rep->AddCast(a->unit->num,str);
}
void ABattle::UpdateShields(ABattleRepRound *rep,AArmy *a){
 for(int i=0;i<a->notbehind;i++){
  ASoldier *s=a->GetTarget(i);
  ASpellType *stype=s->special;
  if(!stype) continue;
  int num=stype->GetShieldCount();
  int numdef=a->round?0:stype->GetDefenceCount();//apply once
  if(num+numdef==0) continue;
  ExprSetRegionMode(curRegion);
  EDataInt *e=new EDataInt;
  e->value=s->slevel;
  variables[7].data=e;
  AnsiString expr;
  int attack_type;
  int level;
  for(int i=0;i<num;i++){
   if(!stype->GetShieldParam(i,expr,attack_type))
    continue;
   try{
    level=ExprProcessExpr(expr);
   }catch (Exception &exception){
    Application->ShowException(&exception);
    continue;
   }
   a->shields->AddShield(attack_type,level);
   AnsiString str=" raise ";
   str+=AttTypeToStr(attack_type);
   str+=" shield by ";
   str+=stype->FullName();
   str+=".";
   rep->AddCast(s->unit->num,str);
  }
  for(int i=0;i<numdef;i++){
   if(!stype->GetDefenceParam(i,expr,attack_type))
    continue;
   try{
    level=ExprProcessExpr(expr);
   }catch (Exception &exception){
    Application->ShowException(&exception);
    continue;
   }
   if(attack_type==ABattleEnums::NUM_ATTACK_TYPES){
    for(int i=0;i<ABattleEnums::NUM_ATTACK_TYPES;i++){
     s->dskill[i]+=level;
    }
   }else{
    s->dskill[attack_type]+=level;
   }
   AnsiString str=" add personal ";
   str+=AttTypeToStr(attack_type);
   str+=" defence by ";
   str+=stype->FullName();
   str+=".";
   rep->AddCast(s->unit->num,str);
  }
 }
}



