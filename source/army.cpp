//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "battle.h"
#include "army.h"
#include "region.h"
#include "unit.h"
#include "items.h"
#include "battlerep.h"
#include "gameparams.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

enum {
  WIN_NO_DEAD,
  WIN_DEAD,
  LOSS
};


ASoldier::ASoldier(AUnit * u, AObject * o, AnsiString regtype,AItemType *r,bool ass)
 :battleitems(new TStringList)
{
 race=r;
 unit=u;
 riding=EmptyStr;
 building=0;
 weapon=0;
 askill=0;
 attacks=1;
 special=0;
 slevel=0;
 immortal=false;

 dskill[ABattleEnums::ATTACK_COMBAT] =0;
 dskill[ABattleEnums::ATTACK_ENERGY] =-2;
 dskill[ABattleEnums::ATTACK_SPIRIT] =-2;
 dskill[ABattleEnums::ATTACK_WEATHER]=-2;
 dskill[ABattleEnums::ATTACK_RIDING] =0;
 dskill[ABattleEnums::ATTACK_RANGED] =0;
 for(int i=0;i<AArmorType::NUM_ARMOR_CLASSES;i++)
   armors[i]=0;
 hits=1;
 if(o){
  AObjectType *otype=ObjectTypes->Get(o->type);
  if(otype&&otype->capacity){
   building=otype;
   for(int i=0;i<ABattleEnums::NUM_ATTACK_TYPES;i++)
   {
    dskill[i] += 2;
   }
   if(o->runes){
    dskill[ABattleEnums::ATTACK_ENERGY] = o->runes;
    dskill[ABattleEnums::ATTACK_SPIRIT] = o->runes;
   }
  }
 }
 if(r->type&AItemType::IT_MONSTER){
  AnsiString abr=r->abr;
  if(AItemTypes::IsIllusionAbr(abr))
   abr="ILLU";
  AMonstrType *mtype=MonstrTypes->Get(abr);
  if(!mtype) return;
  askill=mtype->attackLevel;
  dskill[ABattleEnums::ATTACK_COMBAT]+=mtype->defense[ABattleEnums::ATTACK_COMBAT];
  for(int i=ABattleEnums::ATTACK_ENERGY;i<=ABattleEnums::ATTACK_WEATHER;i++){
   if(mtype->defense[i]>dskill[i]){
    dskill[i]=mtype->defense[i];
   }
  } 
  dskill[ABattleEnums::ATTACK_RIDING]+=mtype->defense[ABattleEnums::ATTACK_RIDING];
  dskill[ABattleEnums::ATTACK_RANGED]+=mtype->defense[ABattleEnums::ATTACK_RANGED];
  hits=mtype->hits;
  if(!hits) hits=1;
  attacks=mtype->numAttacks;
  if(!attacks) attacks=1;
  special=SpellTypes->Get(mtype->spec);
  slevel=mtype->specLevel;
  return;
 }
 SetupSpell();
 SetupBattleItems();

 for(int ac=0;ac<=ArmorTypes->maxarmorclass;ac++)
   foreach(ArmorTypes){
    AArmorType *atype=*(AArmorType**)iter;
    if(atype->armorclass!=ac) continue;
    if(ass){
     if(!(atype->flags&&AArmorType::USEINASS))
      continue;
    }
    if(u->GetBattleItem(AItemType::IT_ARMOR,atype->item)){
     armors[ac]=atype;
     break;
    }
   }
 ATerrainType *ttype=TerrainTypes->Get(regtype);
 int ridingBonus=0;
 if(ttype->mountstypes&(ATerrainType::mtFlying|ATerrainType::mtRiding)){
  foreach(MountTypes){
   AMountType*mt=*(AMountType**)iter;
   AnsiString item;
   for(int i=0,endi=mt->items->Count;i<endi;i++){
    AnsiString it=mt->items->Strings[i];
    if(u->GetBattleItem(AItemType::IT_MOUNT,it)){
     item=it;
     break;
    }
   }
   if(!item.Length()) continue;
   AItemType *itype=ItemTypes->Get(item,4);
   if(ttype->mountstypes&ATerrainType::mtFlying){
    if(!itype->fly){
     if(!(ttype->mountstypes&ATerrainType::mtRiding)){
      u->ReturnBattleItem(item);
      continue;
     }
    }
   }else{
    if(!itype->ride){
     u->ReturnBattleItem(item);
     continue;
    }
   }
   int bonus=u->GetBegSkill(mt->skill);
   if(bonus<mt->minBonus){
    u->ReturnBattleItem(item);
    continue;
   }
   if(bonus>mt->maxBonus){
    bonus=mt->maxBonus;
   }
   if(itype->fly&&!(ttype->mountstypes&ATerrainType::mtFlying)){
    if(bonus>mt->maxHamperedBonus){
     bonus=mt->maxHamperedBonus;
    }
   }
   ridingBonus=bonus;
   dskill[ABattleEnums::ATTACK_RIDING]+=ridingBonus;
   riding=item;
   break;
  }
 }
 int combatSkill=u->GetBegSkill(CombSkill);
 int attackBonus=0;
 int defenseBonus=0;
 foreach(WeaponTypes){
  AWeaponType *wtype=*(AWeaponType**)iter;
  if(u->GetBattleItem(AItemType::IT_WEAPON,wtype->item)){
   int baseSkillLevel=u->CanUseWeapon(wtype,riding.Length());
   if(baseSkillLevel==-1){
    u->ReturnBattleItem(wtype->item);
    continue;
   }
   weapon=wtype;
   if(!(wtype->flags&AWeaponType::NEEDSKILL)){
    baseSkillLevel=combatSkill;
   }
   attackBonus=baseSkillLevel+wtype->attackBonus;
   defenseBonus+=wtype->defenseBonus;
   if(!(wtype->flags&AWeaponType::NOATTACKERSKILL)){
     defenseBonus+=baseSkillLevel;
   }
   if(wtype->flags&AWeaponType::RIDINGBONUS)
     attackBonus+=ridingBonus;
   if(wtype->flags&(AWeaponType::RIDINGBONUSDEFENSE|AWeaponType::RIDINGBONUS))
     defenseBonus+=ridingBonus;
   attacks=wtype->numAttacks;
   if(attacks>=AWeaponType::NUM_ATTACKS_SKILL)
    attacks+=baseSkillLevel-AWeaponType::NUM_ATTACKS_SKILL;
   else if(attacks>=AWeaponType::NUM_ATTACKS_HALF_SKILL)
		attacks+=(baseSkillLevel+1)/2-AWeaponType::NUM_ATTACKS_HALF_SKILL;
   if(attacks==0) attacks=1; 
   break;
  }
 }
 if(!weapon){
  attackBonus=combatSkill+ridingBonus;
  defenseBonus=attackBonus;
  attacks=1;
 }else
 {
  // Okay.  We got a weapon.  If this weapon also has a special
	// and we don't have a special set, use that special.
	// Weapons (like Runeswords) which are both weapons and battle
	// items will be skipped in the battle items setup and handled
	// here.
  if(!special)
  {
   ABatItemType *bitype=BatItemTypes->Get(weapon->item);
   if(bitype&&(bitype->flags&ABatItemType::SPECIAL))
   {
    special=SpellTypes->Get(bitype->spec);
    slevel=bitype->level;
   }
  }
 }
 askill+=attackBonus;
 dskill[ABattleEnums::ATTACK_COMBAT]+=defenseBonus;
}
ASoldier::~ASoldier(){
 delete battleitems;
}
void ASoldier::SetupSpell(){
 if(!unit->combatspell.Length()) return;
 slevel=unit->GetBegSkill(unit->combatspell);
 if(!slevel){
  unit->combatspell="";
  return;
 }
 ASkillType *st=SkillTypes->Get(unit->combatspell,2);
 if(!(st->flags&ASkillType::COMBAT)){
  unit->combatspell="";
  return;
 }
 ASpellType *stype=SpellTypes->Get(unit->combatspell);
 if(!stype){
  unit->combatspell="";
  return;
 }
 special=stype;
}
void ASoldier::SetupBattleItems(){
 foreach(BatItemTypes){
  ABatItemType *bitype=*(ABatItemType**)iter;
  if((bitype->flags&ABatItemType::SPECIAL)&&special)
   continue;
  if(!unit->GetBattleItem(AItemType::IT_BATTLE,bitype->item))
	 continue;
	if(bitype->item==XXXXItem)
	  immortal=true;
  battleitems->Add(bitype->item);
  int level=bitype->level;
  if(bitype->flags&ABatItemType::SPECIAL){
   special=SpellTypes->Get(bitype->spec);
   slevel=level;
  }else   if(bitype->flags&ABatItemType::SHIELD){
   if(bitype->shtype==ABattleEnums::NUM_ATTACK_TYPES){
    for(int i=0;i<ABattleEnums::NUM_ATTACK_TYPES;i++){
     if(dskill[i]<level) dskill[i]=level;
    }
   }else{
    if(dskill[bitype->shtype]<level) dskill[bitype->shtype]=level;
   }
  }
 }
}
bool ASoldier::ArmorProcect(int weaponClass)
{
 int A=0,B=0,C,D;
 for(int ac=0;ac<=ArmorTypes->maxarmorclass;ac++)
 {
   AArmorType *atype=armors[ac];
   if(!atype) continue;
   if(B==0)
   {
     A=atype->saves[weaponClass];
     B=atype->from;
     continue;
   }
   C=atype->saves[weaponClass];
   D=atype->from;
   A=A*D+B*C-A*C;
   B=B*D;
 }
 if(B==0) return false;
 if(A<=0) return false;
 if(A>getrandom(B)) return true;
 return false;
}
void ASoldier::RestoreItems()
{
 if(weapon){
  unit->ReturnBattleItem(weapon->item);
 }
 for(int i=0;i<AArmorType::NUM_ARMOR_CLASSES;i++)
   if(armors[i])
     unit->ReturnBattleItem(armors[i]->item);
 if(riding.Length()){
  unit->ReturnBattleItem(riding);
 }
 for(int i=battleitems->Count-1;i>=0;i--){
  unit->ReturnBattleItem(battleitems->Strings[i]);
 }
 battleitems->Clear();
}

void ASoldier::Alive(int state)
{
 RestoreItems();
}

void ASoldier::Dead()
{
 RestoreItems();
 unit->enditems->SetNum(race->abr,unit->enditems->GetNum(race->abr) - 1);
}
AArmy::AArmy(AUnits * units, AnsiString regtype, bool ass)
 :shields(new AShields)
{
 round=0;
 tac=0;
 int _count=0;
 foreach(units){
  AUnit *un=*(AUnit**)iter;
  _count+=un->GetSoldiers();
  un->losses=0;
 }
 if(ass){
 _count=1;
 }
 int x=0;
 int y=_count;
 TList::Count=_count;
 foreach(units){
  AUnit *un=*(AUnit**)iter;
  AObject *obj=un->baseobj;
  int temp=un->GetBegSkill(TactSkill);
  if(temp>tac) tac=temp;
  if(ass){
   foreach(un->enditems){
    AItem *it=*(AItem**)iter;
    if(!it->count) continue;
    AItemType *itype=it->type;//GetType();
    if(!(itype->type&AItemType::IT_MAN)) continue;
    TList::Items[x++]=new ASoldier(un,obj,regtype,itype,ass);
    break;
   }
   break;
  }else{
   bool behind=un->GetFlag(FLAG_BEHIND);
   foreach(un->enditems){
    AItem *it=*(AItem**)iter;
    if(!it->count) continue;
    if(!it->IsSoldier()) continue;
    AItemType *itype=it->type;//GetType();
    for(int i=0;i<it->count;i++){
     bool locbehind=(itype->type&AItemType::IT_MAN)&&behind;
     AMonstrType *mtype=itype->GetMonstrType();
     if(mtype)
      locbehind=mtype->flags&AMonstrType::BEHIND;
     if(locbehind)
     {
      TList::Items[--y]=new ASoldier(un,obj,regtype,itype);
     }else{
      TList::Items[x++]=new ASoldier(un,obj,regtype,itype);
     }
    }
   }
  }
 }
 canfront=x;
 canbehind=_count;
 notfront=_count;
 notbehind=_count;
 if(!NumFront()){
  canfront = canbehind;
  notfront = notbehind;
 }
}
__fastcall AArmy::~AArmy()
{
 Clear();
 delete shields;
}
void __fastcall AArmy::Clear()
{
 int i=Count-1;
 if(i<0) return;
 ASoldier **iter=(ASoldier**)List+i;
 for(;i>=0;i--,iter--){
  delete *iter;
 }
 TList::Clear();
}


void AArmy::Reset()
{
 canfront = notfront;
 canbehind = notbehind;
 notfront = notbehind;
}
int AArmy::GetLoses()
{
 return count-NumAlive();
}
void AArmy::GetDamaged(AUnits *uns)
{
 if(uns->autodelete)
  throw Exception("Error in Army::GetDamaged");
 uns->Clear();
 if(notbehind!=count){
  for(int i=notbehind;i<count;i++){
   ASoldier *s=GetTarget(i);
   if (uns->IndexOf(s->unit)!=-1) continue;
   uns->Add(s->unit);
  }
 }
}
bool AArmy::Broken()
{
 if ((NumAlive() * 2 / count) >= 1) return false;
  return true;
}

int AArmy::NumAlive()
{
 return notbehind;
}
int AArmy::CanAttack()
{
 return canbehind;
}
int AArmy::NumFront()
{
 return (canfront+notfront-canbehind);
}
int pow(int b,int p)
{
    int b2 = b;
    for(int i=1; i<p; i++)
    {
        b2 *= b;
    }
    return b2;
}

int Hits(int a,int d)
{
    int tohit = 1,tomiss = 1;
    if (a>d)
    {
        tohit = pow(2,a-d);
    }
    else if (d>a)
    {
        tomiss = pow(2,d-a);
    }
    if (getrandom(tohit+tomiss) < tohit) return 1;
    return 0;
}
ASoldier* AArmy::GetAttacker(int i, bool & behind)
{
 ASoldier *retval =GetTarget(i);
 if(i<canfront){
  Items[i]=Items[canfront-1];
  Items[canfront-1]=Items[canbehind-1];
  Items[canbehind-1]=retval;
  canfront--;
  canbehind--;
  behind=false;
  return retval;
 }
 Items[i]=Items[canbehind-1];
 Items[canbehind-1]=Items[notfront-1];
 Items[notfront-1]=retval;
 canbehind--;
 notfront--;
 behind=true;
 return retval;
}
int AArmy::GetTargetNum(ASpellType *special)
{
 int tars=NumFront();
 if(tars==0){
  canfront=canbehind;
  notfront=notbehind;
  tars=NumFront();
  if(tars==0)
   return -1;
 }
 for(int retries=0;retries<10;retries++){
  int i=getrandom(tars);
  if(i<canfront){
   if (CheckSpecialTarget(special,i)) return i;
   continue;
  }
  i+=canbehind-canfront;
  if (CheckSpecialTarget(special,i)) return i;
  continue;
 }
 return -1;
}
ASoldier* AArmy::GetTarget(int i)
{
 return (ASoldier*)TList::Items[i];
}
int AArmy::DoAnAttack(ASpellType *special, int numAttacks, int attackType,
     int attackLevel, int flags, int weaponClass, AnsiString effect, int mountBonus)
{
 bool canShield=false;
 bool combat=false;
 switch(attackType){
  case ABattleEnums::ATTACK_RANGED:
   canShield=true;
   // fall through
  case ABattleEnums::ATTACK_COMBAT:
  case ABattleEnums::ATTACK_RIDING:
   combat=true;
   break;
  case ABattleEnums::ATTACK_ENERGY:
  case ABattleEnums::ATTACK_WEATHER:
  case ABattleEnums::ATTACK_SPIRIT:
   canShield=true;
  break;
 }
 if(canShield){
  int shind,shlevel;
  shind=shields->GetHighShield(attackType,shlevel);
  if(shind>=0){
   if(!Hits(attackLevel,shlevel))
    return 0;
   if(!effect.Length()&&!combat){
    shields->Delete(shind);
   }
  }
 }
 int ret=0;
 for(int i=0;i<numAttacks;i++){
  int tarnum=GetTargetNum(special);
  if(tarnum==-1) continue;
  ASoldier *tar=GetTarget(tarnum);
  int tarFlags=0;
  if(tar->weapon){
   tarFlags=tar->weapon->flags;
  }
  int tlev=0;
  if(attackType!=ABattleEnums::NUM_ATTACK_TYPES)
   tlev=tar->dskill[attackType];

  if((flags&AWeaponType::NODEFENSE)&&(tlev>0))
   tlev=0;
  if(!(flags&AWeaponType::RANGED)){
   int attLen=1;
   int defLen=1;
   if(flags&AWeaponType::LONG) attLen=2;
   else if(flags&AWeaponType::SHORT) attLen=0;
   if(tarFlags&AWeaponType::LONG) defLen=2;
   else if(tarFlags&AWeaponType::SHORT) defLen=0;
   if(attLen>defLen) attackLevel++;
   else if(defLen>attLen) tlev++;
  }
  if(tar->riding.Length())
   attackLevel+=mountBonus; 

  if(attackType!=ABattleEnums::NUM_ATTACK_TYPES){
   if(!(flags&AWeaponType::ALWAYSREADY)){
    if(getrandom(2))
     continue;
   }
   if(!Hits(attackLevel,tlev))
    continue;
  }
  if(!effect.Length()){
   if(tar->ArmorProcect(weaponClass))
    continue;
   Kill(tarnum);
   ret++;
  }else{
  }
 }
 return ret;
}
bool AArmy::CheckSpecialTarget(ASpellType *special, int i)
{
 if(!special) return true;
 int num1=special->GetCheckGoodTargetCount();
 int num2=special->GetCheckBadTargetCount();
 if(!(num1+num2))
  return true;
 ASoldier *s=GetTarget(i);
 AnsiString str,race="\""+s->race->abr+"\"";
 if(num1){
  for(int i=0;i<num1;i++){
   str=special->GetTypeStr(ASpellType::CHECKGOODTARGET,i);
   if(str.Pos(race))
    return true;
  }
  return false;
 }
 if(num2){
  for(int i=0;i<num2;i++){
   str=special->GetTypeStr(ASpellType::CHECKBADTARGET,i);
   if(str.Pos(race))
    return false;
  }
  return true;
 }
 return true;
}

void AArmy::Kill(int i)
{
 ASoldier *temp=GetTarget(i);
 if(temp->immortal) return;
 temp->hits--;
 if(temp->hits) return;
 temp->unit->losses++;
 if(i<canfront){
  canfront--;
  Items[i]=Items[canfront];
  Items[canfront]=temp;
  i=canfront;
 }
 if(i<canbehind){
  canbehind--;
  Items[i]=Items[canbehind];
  Items[canbehind]=temp;
  i=canbehind;
 }
 if(i<notfront){
  notfront--;
  Items[i]=Items[notfront];
  Items[notfront]=temp;
  i=notfront;
 }
 notbehind--;
 Items[i]=Items[notbehind];
 Items[notbehind]=temp;
}
void AArmy::Win(ABattleReport *rep)
{
 int wintype;
 if(count-NumAlive()) {
  wintype=WIN_DEAD;
 }else{
  wintype=WIN_NO_DEAD;
 }

// DoHeal(b);
 rep->loses2=GetLoses();
 GetDamaged(rep->damaged2);

 for(int x=0; x<count; x++) {
  ASoldier *s=GetTarget(x);
  if (x<NumAlive()) {
   s->Alive(wintype);
  }else{
   s->Dead();
  }
//  delete s;
 }
}
void AArmy::Lose(ABattleReport *rep)
{
 rep->loses1=GetLoses();
 GetDamaged(rep->damaged1);
 for (int i=0; i<count; i++){
  ASoldier * s = GetTarget(i);
  if (i<notbehind){
   s->Alive(LOSS);
  }else{
   s->Dead();
  }
//  delete s;
 }
}
void AArmy::Tie(ABattleReport *rep,bool first)
{
 if(first){
  rep->loses1=GetLoses();
  GetDamaged(rep->damaged1);
 }else{
  rep->loses2=GetLoses();
  GetDamaged(rep->damaged2);
 }
 for(int x=0;x<count;x++){
  ASoldier *s = GetTarget(x);
  if (x<NumAlive()) {
   s->Alive(WIN_DEAD);
  }else{
   s->Dead();
  }
//  delete s;
 }
}


__fastcall AShields::AShields()
{
}

__fastcall AShields::~AShields()
{
 Clear();
}
void AShields::AddShield(int shtype,int level){
 AddObject(AnsiString(level),(TObject*)shtype);
}
int AShields::GetHighShield(int shtype,int&level){
 int hlevel=0,hind=-1;
 for(int i=Count-1;i>=0;i--){
  int _type=(int)Objects[i];
  if(_type!=shtype) continue;
  int _level=atoi(Strings[i].c_str());
  if(_level>hlevel){
   hind=i;
   hlevel=_level;
  }
 }
 level=hlevel;
 return hind;
}
AnsiString ASoldier::Print()
{
  AnsiString str;
  int attacktype;
  int weapclass;
  str=race->name;
  str+="  Weapon:";
  if(!weapon)
  {
    str+="none";
    weapclass=AWeaponType::SLASHING;
    attacktype=ABattleEnums::ATTACK_COMBAT;
  }else
  {
    str+=ItemTypes->AbrToName(weapon->item);
    weapclass=weapon->weapClass;
    attacktype=weapon->attackType;
  }
  str+="  Armor:";
  bool first=true;
  for(int i=0;i<=ArmorTypes->maxarmorclass;i++)
  {
    if(!armors[i]) continue;
    if(first)
      first=false;
    else
      str+="+";
    str+=ItemTypes->AbrToName(armors[i]->item);
  }
  if(first) str+="none";
  str+="  Mount:";
  if(!riding.Length())
    str+="none";
  else
    str+=ItemTypes->AbrToName(riding);
  str+=" Attack:";
  str+=AttTypeToStr(attacktype);
  str+=" "+WeapClsToStr(weapclass);
  if(weapon)
  {
    if(weapon->flags&AWeaponType::LONG)
      str+=" LONG";
    if(weapon->flags&AWeaponType::SHORT)
      str+=" SHORT";
  }else
      str+=" SHORT";
  str+=":";
  str+=askill;
  str+="*";
  str+=attacks;
  str+="  Defense:";
  for(int i=0;i<ABattleEnums::NUM_ATTACK_TYPES;i++)
  {
    if(i) str+=",";
    str+=dskill[i];
  }
  str+="  Hits:";
  str+=hits;
  if(special)
  {
    str+=" Special:";
    str+=special->name;
    str+=":";
    str+=slevel;
  }
  return str;
}

