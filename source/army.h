//---------------------------------------------------------------------------

#ifndef armyH
#define armyH
//---------------------------------------------------------------------------
#include "util.h"
class ABattle;
class ABattleReport;
class ASoldier{
public:
 AUnit *unit;
 AItemType *race;
 AnsiString riding;//riding item
 AObjectType *building;
 AWeaponType *weapon;
 int askill;
 int attacks;
 ASpellType *special;
 int slevel;
 int dskill[ABattleEnums::NUM_ATTACK_TYPES];
 AArmorType *armors[AArmorType::NUM_ARMOR_CLASSES];
 int hits;
 TStringList *battleitems;
 bool immortal;

 ASoldier(AUnit * u, AObject * o, AnsiString regtype, AItemType *r,bool ass=false);
 ~ASoldier();
 void SetupSpell();
 void SetupBattleItems();

 bool ArmorProcect(int weaponClass);
 void RestoreItems();
 void Alive(int state);
 void Dead();

 AnsiString Print();
};
class AShields:TStringList{
public:
    __fastcall AShields();
    __fastcall ~AShields();
    TStringList::Clear;
    TStringList::Delete;
    void AddShield(int shtype,int level);
    int GetHighShield(int shtype,int&level);
};

class AArmy:TList{
public:
   int round;
   int tac;
   int canfront;
   int canbehind;
   int notfront;
   int notbehind;

   AShields *shields;

   AArmy(AUnits * units, AnsiString regtype, bool ass);
    __fastcall ~AArmy();
    void __fastcall Clear();
    void Reset();
    bool Broken();
    int NumAlive();
    int CanAttack();
    int NumFront();
    ASoldier* GetAttacker(int i, bool & behind);
    int GetTargetNum(ASpellType *special);
    ASoldier* GetTarget(int i);
    int DoAnAttack(ASpellType *special, int numAttacks, int attackType,
     int attackLevel, int flags, int weaponClass, AnsiString effect, int mountBonus);
    bool CheckSpecialTarget(ASpellType *special, int i);
    void Kill(int i);
    int GetLoses();
    void GetDamaged(AUnits *uns);
    void Win(ABattleReport *rep);
    void Lose(ABattleReport *rep);
    void Tie(ABattleReport *rep,bool first);


    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};
#endif
