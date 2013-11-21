 //---------------------------------------------------------------------------
#ifndef utilH
#define utilH
//---------------------------------------------------------------------------
#include <vector.h>
#ifdef RULESONLY
#include <FileCtrl.hpp>
extern AnsiString ClientPath;
#else
const NDIRS=6;
#endif
namespace EAP{class TFile;};
typedef Graphics::TBitmap TBmp;
const MapImageWidth=46,MapImageHeight=39;
const MapDX=35,MapDY=20;

using namespace EAP;
extern AnsiString curGame;
extern TStringList*Games;
extern AnsiString LoadReportsDir/*Saved with turns*/,LoadOrdersDir,LoadBattlesDir,LoadRegionListDir;
extern AnsiString OrdersDir,ReportsDir,StoreDir;
extern AnsiString IniName;

class AOrder;
class AOrders;

class ASkill;
class ASkills;

class AItem;
class AItems;

class AMarket;
class AMarkets;

class AUnit;
class AUnits;

class ALocation;
class AObject;
class ARegion;
class ARegionArray;

class AFaction;
class AFactions;

class ATurn;
class ATurns;

class VTreeNode;
class VTreeNodes;
class VListItem;
class VListItems;

class ABattleRepRound;
class ABattleReport;

class AScript;
class AScripts;


// KeyPhrases->Get(keyAtlErrHead)
enum{
 keyAtlRepHead=0,
 keyWarSign,
 keyTradeSign,
 keyMagicSign,
 keyFacStat,
 keyTaxStatSign,
 keyTradeStatSign,
 keyMagesStatSign,
 keyAtlErrHead,
 keyAtlBatHead,
 keyAtlEvnHead,
 keySkillHead,
 keyItemHead,
 keyObjHead,
 keyIllusoryItemSign,
 keyWeightSign,
 keyFlyCapacSign,
 keyRideCapacSign,
 keyWalkCapacSign,
 keySwimCapacSign,
 keyHitchedSign,
 keyCanFly,
 keyCanRide,
 keyCanWalk,
 keyCanSwim,
 keyIsMan,
 keyIsMon,
 keyIsWeapon,
 keyIsArmor,
 keyIsTrade,
 keyIsMount,
 keyIsBattle,
 keyIsCantGive,
 keyIsObjectBuild,
 keyIsObjectShip,
 keyAttitudes,
 keyUnclaimed,
 keyInSign,
 keyContains,
 keyPeasants,
 keySilvSign,
 keyWages,
 keyMaxSign,
 keyNoneSign,
 keyUnlimitedSign,
 keyMarketAtSign,
 keyWantedSign,
 keySaledSign,
 keyEnteSign,
 keyProdSign,
 keyExitSign,
 keyGatesSign,
 keyGatesSign2,
 keyObjNameEndSign,
 keyObjNeedSign,
 keyObjContainsSign,
 keyObjEngravedSign,
 keyObjClosedSign,
 keyUnitOnGuard,
 keyUnitAvoid,
 keyUnitBehind,
 keyUnitRevUnit,
 keyUnitRevFaction,
 keyUnitSharing,
 keyUnitHold,
 keyUnitTax,
 keyUnitNoAid,
 keyUnitConsumUnit,
 keyUnitConsumFaction,
 keyUnitNoCross,
 keyUnitNoSpoils,
 keyUnitSpoilsNone,
 keyUnitSpoilsWalk,
 keyUnitSpoilsRide,
 keyUnitSpoilsFly,
 keyUnitSkills,
 keyUnitCombatSpell,
 keyUnitCanStudy,
 keyTemplateSign,
 keyTemplateBegin,
 keyItemIllusionSign,
 keyBattleBeginAss,
 keyBattleBeginNorm,
 keyBattleAttackers,
 keyBattleDefenders,
 keyBattleFreeRound,
 keyBattleNormalRound,
 keyBattleLoses,
 keyBattleRouted,
 keyBattleDestroyed,
 keyBattleAssassinated,
 keyBattleTie,
 keyBattleTieAss,
 keyBattleTotalCasual,
 keyBattleDamaged,
 keyBattleSpoils,
 keyBattleNoSpoils,
 keyMonstrCombat,
 keyMonstrAttacks,
 keyMonstrHits,
 keyMonstrTactics,
 keyObjDecayWarning,
 keyObjMaintenanceWarning,
 NKEYPHRASES
};

enum ReportOptions{
 roLocalUnit=0x0001,
 roLocalAsNonlocalUnit=0x0002,
 roNonlocalUnit=0x0004,
 roUserObject=0x0008,
 roNonuserObject=0x0010,
 roAllRegion=0x0020,
 roRegionInfo=0x0040,
 roAdvProducts=0x0080,
 roRegionList=0x0100,
 roNormalReport=roLocalUnit|roNonlocalUnit|roUserObject|roNonuserObject|roRegionInfo|roAdvProducts,
};

class AKeyPhrases:TStringList{
public:
    AKeyPhrases();
    virtual __fastcall ~AKeyPhrases();
    __fastcall AnsiString Get(int index);
    __fastcall void Set(int index,AnsiString str);

    void Save();
};
extern AKeyPhrases *KeyPhrases;

void AddEvent(AnsiString ev);
void ClearEvents();
void BeginEvents();
void AddNoClearOnBeginEvents();
void DisableEvents();
void EnableEvents();
/*#define foreach(listptr,elem)  \
for(int abcdef=0;abcdef<listptr->count;abcdef++){\
 elem=listptr->Get(abcdef);*/
#define foreach(listptr) \
for(void **iter=listptr->List,**end=iter+listptr->count;iter<end;iter++)


/*#define foreachr(listptr,elem)  \
for(int abcdef=listptr->count-1;abcdef>=0;abcdef--){\
 elem=listptr->Get(abcdef);*/
#define foreachr(listptr) \
if(listptr->List) for(void **begin=listptr->List,**iter=begin+(listptr->count-1);iter>=begin;iter--)

TMemoryStream* LoadFile(AnsiString fName);
void ReadStringList(TFile &in,TStringList *list);
void WriteStringList(TFile &out,TStringList *list);
int CompareIStr(AnsiString s1, AnsiString s2);
bool MyCompare(const char *str,AnsiString pattern); //false - equal
extern int ReportTab;
AnsiString Trunc(AnsiString &str,int len=70);
void SaveRepString(TStream *strm, AnsiString str);
void SaveRepStringList(TStream *strm, TStringList *list);


AnsiString GetNextToken(char*&buf);
AnsiString GetNextString(char*&buf, int nextstrtab=0);
AnsiString GetNextStringNew(char*&buf);
AnsiString GetNextFactionName(char*&buf);
AnsiString GetNextUnitName(char*&buf){return GetNextFactionName(buf);}
AnsiString GetNextItemName(char*&buf);
AnsiString GetNextObjectName(char*&buf);
AnsiString GetNextSkillName(char*&buf){return GetNextItemName(buf);}
ALocation* GetNextLocation(char*&buf, bool updatelevels=true);
class ATerrainType{
public:
enum{
 mtRiding=1,
 mtFlying=2
};
    AnsiString name;
    int movepoints;
    int mountstypes;//1-riding,2-flying
    TColor color;
    TStringList *advlist;
    vector<int> imginds;
    bool ocean;
    ATerrainType();
    ~ATerrainType();
    AnsiString Print(bool quotation=false);
    bool CanProduceAdv(AnsiString abr);
    int GetImageIndex(AnsiString weather);
    bool LoadImage();
    bool SaveImage();
};
class ATerrainTypes:TList{
public:
    ATerrainTypes();
    virtual __fastcall ~ATerrainTypes();
    ATerrainType* Get(int index);
    void AddTerrainType(ATerrainType * ttype);
    bool modified;
    void Save();
    ATerrainType* Get(AnsiString terrain);
    bool IsOcean(AnsiString terrain);
    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};
extern ATerrainTypes*TerrainTypes;
class AWeatherType{
public:
    AnsiString name;
    int movemult;
    AnsiString curmonthname;
    AnsiString nextmonthname;
    AWeatherType();
    AnsiString Print();
};
class AWeatherTypes:TList{
public:
    int normalindex;

    AWeatherTypes();
    virtual __fastcall ~AWeatherTypes();
    AWeatherType* Get(int index);
    void AddWeatherType(AWeatherType * wtype);
    bool modified;
    void Save();
    AWeatherType* Get(AnsiString name);
    int GetIndex(AnsiString name);
    AnsiString FindCurWeather(AnsiString text);
    AnsiString FindNextWeather(AnsiString text);
    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};
extern AWeatherTypes*WeatherTypes;
class ADirType{
public:
    AnsiString name;
    AnsiString abr;
    int num;
    AnsiString Print();
};
class ADirTypes:TList{
public:
    ADirTypes();
    virtual __fastcall ~ADirTypes();
    ADirType* Get(int index);
    int Find(AnsiString str);
    void Save();
    bool modified;
    void AddDirType(ADirType * dtype);
    void MakeSkillList(TStringList *list);
    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};
extern ADirTypes*DirTypes;
class ASkillType{
public:
enum{
 MAGIC = 0x1,
 COMBAT = 0x2,
 CAST = 0x4,
 FOUNDATION = 0x8,
 MAGICCOMBAT = MAGIC |COMBAT,
};
    AnsiString name;
    AnsiString abr;
    int cost;
    int flags;
    AnsiString depends[3];
    int levels[3];
    int sortindex;

    ASkillType();
    AnsiString Print(bool quotation = false);
    void CreateNew(ASkillType* prevskill);
    AnsiString FullName();
};
class ASkillTypes:TList{
public:
    ASkillTypes();
    virtual __fastcall ~ASkillTypes();
    ASkillType* Get(int index);
    ASkillType* Get(AnsiString name,int mode=3, bool addunknown=false);
    void AddSkillType(ASkillType* stype);
    bool modified;
    void Save();
//0 - not found, 1 - present, 2 - name, not abr, 3 - abr, not name
    int VerSkills(AnsiString name, AnsiString abr);
    void MakeSkillList(TStringList * list);
    AnsiString AbrToName(AnsiString abr);
    AnsiString NameToAbr(AnsiString name);
    void UpdateSortedList();
    ASkillType* GetSorted(int index);
    TList::IndexOf;
    __property int count  = { read = Getcount };
    __property List;
private:
    TList *slist;

    int __fastcall Getcount(){return Count;}
};
extern ASkillTypes*SkillTypes;
class ASkillInfo{
public:
    AnsiString skill;
    int level;
    AnsiString text;
    AnsiString Print(bool quotation=false);
    ASkillInfo();
    AnsiString FullName();
    void CreateNew(ASkillInfo * prevsi);
};
class ASkillInfos:TList{
public:
    ASkillInfos();
    virtual __fastcall ~ASkillInfos();
    ASkillInfo* Get(int index);
    ASkillInfo* Get(AnsiString skill, int level);
    void AddSkillInfo(ASkillInfo * si);
    bool modified;
    void Save();
    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};
extern ASkillInfos *SkillInfos;
class AItObjInfo{
public:
    enum{
      OBJECT=0,
      ITEM,
    };
    AnsiString abr;
    int type;
    AnsiString text;
    AnsiString Print(bool quotation=false);
    AItObjInfo();
    AnsiString FullName();
    void CreateNew(AItObjInfo * previoi);
};
class AItObjInfos:TList{
public:
    AItObjInfos();
    virtual __fastcall ~AItObjInfos();
    AItObjInfo* Get(int index);
    AItObjInfo* Get(AnsiString abr, int type);
    void AddItObjInfo(AItObjInfo * ioi);
    bool modified;
    void Save();
    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};
extern AItObjInfos *ItObjInfos;
class AMonstrType;
class AItemType{
public:
enum {
    IT_NORMAL = 0x01,
    IT_ADVANCED = 0x02,
    IT_TRADE = 0x04,
    IT_MAN = 0x08,
    IT_MONSTER = 0x10,
    IT_MAGIC = 0x20,
    IT_WEAPON = 0x40,
    IT_ARMOR = 0x80,
    IT_MOUNT = 0x100,
    IT_BATTLE = 0x200,
//	IT_SPECIAL = 0x0400,
//	IT_TOOL = 0x0800,
    IT_FOOD = 0x1000,

    IT_PRODNOTTRADEACTIVITY=0x10000,
    IT_LEADER=0x20000,
	IT_ORINPUTS=0x40000,
	IT_UNFINISHED=0x80000,
};
enum{
    CANTGIVE=1,
};
enum{
    INPUTCOUNT=5
};
    AnsiString name;
    AnsiString names;
    AnsiString abr;
    int flags; //cantgive
    AnsiString skill;  //requared to prod
    int level;
    int months; //человеко-месяцы только при наличии input
    int out;    //кол-во производимых только при наличии input
    AnsiString inputs[INPUTCOUNT];
    int numbers[INPUTCOUNT];
    int weight;
    int type;
    bool combat; //show in battlerep
    int walk;
    int ride;
    int fly;
    int swim;
    int helpprodmult;
    AnsiString helpproditem;
    int withdrawcost;
    AnsiString needbuilding;

	AnsiString hitchItem; //для связки Лошадь/вагон
	int hitchwalk;        //указывает на Лошадь
	int hitchride,hitchfly,hitchswim;

    AItemType();
    AnsiString Print(bool quotation = false);
    bool IsNewInfo(AItemType* it);
    void CreateNew(AItemType * previt);
//    void UpdateNewInfo(AItemType * newit);
    AnsiString FullName(bool plural = false);
    AMonstrType *GetMonstrType();
};
class AItemTypes:TList{
public:
    AItemTypes();
    virtual __fastcall ~AItemTypes();
    AItemType* Get(int index);
    AItemType* Get(AnsiString name,int mode=7, bool addunknown=false);//4 - abr,8-no illus
    void AddItemType(AItemType* itype);
    bool modified;
    void Save();
    void MakeItemList(TStringList* list,int ignoretype=0);
    AnsiString AbrToName(AnsiString abr);
    AnsiString AbrToNames(AnsiString abr);
    AnsiString NameToAbr(AnsiString name);
    static bool IsIllusionAbr(AnsiString abr);
    static AnsiString IllusionAbrToNormal(AnsiString iabr);
    static AnsiString NormalAbrToIllusion(AnsiString abr);
    TList::IndexOf;
    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};
extern AItemTypes*ItemTypes;
class ATownType{
public:
    AnsiString name;
    AnsiString imgname;
    int imgnum;
    AnsiString Print();
    ATownType();
};
class ATownTypes:TList{
public:
    ATownTypes();
    virtual __fastcall ~ATownTypes();
    ATownType*Get(int index);
    ATownType* Get(AnsiString type);
    int GetIndex(AnsiString type);
    void AddTownType(ATownType *type);
    bool modified;
    void Save();
    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};
extern ATownTypes*TownTypes;
extern AnsiString WoodOrStoneStr;
class AObjectType{
public:
enum{
    OT_BUILDNOTTRADEACTIVITY=0x10000,
};
    AnsiString name;
    int protect;
    int capacity;
    AnsiString item;
    int cost;
    AnsiString skill;
    int level;
    int sailors;
    int canenter;
    int road;
    int flags;
    AnsiString production;
    int helpprodmult;
    AnsiString helpproditem;
    AnsiString Print(bool was = true);
    AObjectType();
};
class AObjectTypes:TList{
public:
    AObjectTypes();
    virtual __fastcall ~AObjectTypes();
    AObjectType* Get(int index);
    AObjectType* Get(AnsiString name);
    void AddObjectType(AObjectType *otype);
    bool modified;
    void Save();
    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}

};
extern AObjectTypes *ObjectTypes;

class AWeatherRegion{
public:
    AnsiString name;
    int level;
    TRect rect;
    AnsiString weathers[12];
    AWeatherRegion();
    AnsiString Print();

};
class AWeatherRegions:TList{
public:
    AWeatherRegions();
    virtual __fastcall ~AWeatherRegions();
    AWeatherRegion *Get(int index);
    AWeatherRegion *Get(AnsiString name);
    void AddWeatherRegion(AWeatherRegion * reg);
    void Save();
    AnsiString GetWeather(int x, int y, int z, int turnnum=-1);
    int GetSeasonLength(int x, int y, int z, int turnnum=-1);
    __property int count  = { read = Getcount };
    __property List;

private:
    int __fastcall Getcount(){return Count;}

};
extern AWeatherRegions *WeatherRegions;

class ABattleEnums{
public:
enum {
    ATTACK_COMBAT,
    ATTACK_ENERGY,
    ATTACK_SPIRIT,
    ATTACK_WEATHER,
    ATTACK_RIDING,
    ATTACK_RANGED,
    //ATTACK_DISPEL,
    NUM_ATTACK_TYPES
};
enum {
    HEAL_NONE,
    HEAL_THREE,
    HEAL_TWO,
    HEAL_ONE
};
};

class AWeaponType{
public:
enum {
	SLASHING, // e.g. sword attack (This is default)
	PIERCING, // e.g. spear or arrow attack
	CRUSHING, // e.g. mace attack
	CLEAVING, // e.g. axe attack
	ARMORPIERCING, // Armour piercing attack (crossbow, magic)
	MAGIC_ENERGY,	// e.g. fire, dragon breath
	MAGIC_SPIRIT,	// e.g. black wind
	MAGIC_WEATHER,	// e.g. tornado
	NUM_WEAPON_CLASSES
};
enum{
    NEEDSKILL = 0x1, // No bonus or use unless skilled
	ALWAYSREADY = 0x2, // Ignore the 50% chance to attack
	NODEFENSE = 0x4, // No combat defense against this weapon
	NOFOOT = 0x8, // Weapon cannot be used on foot (e.g. lance)
	NOMOUNT = 0x10, // Weapon cannot be used mounted (e.g. pike)
	SHORT = 0x20, // Short melee weapon (e.g. shortsword, hatchet)
	LONG = 0x40, // Long melee weapon (e.g. lance, pike)
	RANGED = 0x80, // Missile weapon
	NOATTACKERSKILL = 0x100, // Attacker gets no combat/skill defense.
        RIDINGBONUS = 0x200, // Unit gets riding bonus on att and def.
        RIDINGBONUSDEFENSE = 0x400, // Unit gets riding bonus on def only.

        SPECIAL_FLAGS=ALWAYSREADY,

        NUM_ATTACKS_HALF_SKILL = 50,
        NUM_ATTACKS_SKILL = 100,
};
    AnsiString item;
    AnsiString skill1;
    AnsiString skill2;
    int flags;

    int weapClass;
	int attackType;


    int numAttacks;

    int attackBonus,defenseBonus;
    int mountBonus;//против солдат на конях

    AWeaponType();
    AnsiString Print(bool quotation = false);
    void CreateNew(AWeaponType * prevw);
};

class AWeaponTypes:TList{
public:
    AWeaponTypes();
    virtual __fastcall ~AWeaponTypes();
    AWeaponType* Get(int index);
    AWeaponType* Get(AnsiString abr);
    void AddWeaponType(AWeaponType* itype);
    void DelWeaponType(AnsiString abr);
	bool modified;
    void Save();
    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};
extern AWeaponTypes*WeaponTypes;

class AArmorType{
public:
enum{
    USEINASS=0x1,

    NUM_ARMOR_CLASSES=5,
};
    AnsiString item;
    int flags;
    int from;
	int saves[AWeaponType::NUM_WEAPON_CLASSES];
    int armorclass;


    AArmorType();
    AnsiString Print(bool quotation = false);
    void CreateNew(AArmorType * prevat);
};

class AArmorTypes:TList{
public:
    int maxarmorclass; //номер максимального загруженного типа брони
    
    AArmorTypes();
    virtual __fastcall ~AArmorTypes();
    AArmorType* Get(int index);
	AArmorType* Get(AnsiString abr);
	void AddArmorType(AArmorType* atype);
	void DelArmorType(AnsiString abr);
	bool modified;
    void Save();
    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};
extern AArmorTypes*ArmorTypes;

class AMountType{
public:
	AnsiString skill;
	int minBonus;
	int maxBonus;
	// This is the max bonus a mount will grant if it can normally fly
	// but the region doesn't allow flying mounts
	int maxHamperedBonus;//3
// If the mount has a special effect it generates when ridden in combat
	AnsiString spec;
	int specLevel;
	bool monster;
	TStringList *items;
	AMountType();
	~AMountType();
	AnsiString Print(bool quotation = false);
	void CreateNew(AMountType * prevmt);
};

class AMountTypes:TList{
public:
	AMountTypes();
	virtual __fastcall ~AMountTypes();
	AMountType* Get(int index);
	AMountType* Get(AnsiString abr);
	void AddMountType(AMountType* mtype);
	void DelMountType(AnsiString abr);
	bool modified;
	void Save();
	__property int count  = { read = Getcount };
	__property List;
private:
	int __fastcall Getcount(){return Count;}
};
extern AMountTypes*MountTypes;

class ASpellType{
public:
enum{
 ATTACK=1,
 SHIELD,
 DEFENCE,
 CHECKGOODTARGET,
 CHECKBADTARGET,
 CASTCREATE,
 CASTNOP,


 SPELLSNUM
};
    AnsiString name;
    TStringList *spells;

    ASpellType();
    ~ASpellType();
    AnsiString Print(bool quotation = false);
    void CreateNew(ASpellType * prevst);
    int GetTypeCount(int type);
    int GetAttackCount(){return GetTypeCount(ATTACK);}
    int GetShieldCount(){return GetTypeCount(SHIELD);}
    int GetDefenceCount(){return GetTypeCount(DEFENCE);}
    int GetCombatCount(){
     return GetAttackCount()+GetShieldCount()+GetDefenceCount();
    }
    int GetCheckGoodTargetCount(){return GetTypeCount(CHECKGOODTARGET);}
    int GetCheckBadTargetCount(){return GetTypeCount(CHECKBADTARGET);}
    int GetCheckCount(){
     return GetCheckGoodTargetCount()+GetCheckBadTargetCount();
    }
    int GetCastCreateCount(){return GetTypeCount(CASTCREATE);}
    int GetCastNOPCount(){return GetTypeCount(CASTNOP);}
    int GetCastCount(){
     return GetCastCreateCount()+GetCastNOPCount();
    }

    AnsiString GetTypeStr(int type,int ind);

    static AnsiString FullName(AnsiString abr);
    AnsiString FullName(){ return FullName(name);}
    static int GetSpellType(AnsiString str);
    static AnsiString GetSpellTypeName(int type);
    static bool ParseSpell(char *&buf, int &type);
    static bool ParseAttack(char *&buf,AnsiString&expr,int&attack_type);
    static bool ParseCheckTarget(char *&buf,TStringList*targets);
    static bool ParseCastCreate(char *&buf,TStringList*params,AnsiString &maxnum,AnsiString&param);

    bool GetAttackParam(int ind,AnsiString&expr,int&attack_type);
    bool GetShieldParam(int ind,AnsiString&expr,int&attack_type);
    bool GetDefenceParam(int ind,AnsiString&expr,int&attack_type);
    bool GetCastCreateParam(int ind,TStringList*params,AnsiString &maxnum,AnsiString&param);
};
class ASpellTypes:TList{
public:
    ASpellTypes();
    virtual __fastcall ~ASpellTypes();
    ASpellType* Get(int index);
    ASpellType* Get(AnsiString abr);
    void AddSpellType(ASpellType* stype);
    bool modified;
    void Save();

    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};
extern ASpellTypes*SpellTypes;

class AMonstrType{
public:
    enum{
      BEHIND=1,
    };
    AnsiString item;
    int attackLevel;
    int defense[ABattleEnums::NUM_ATTACK_TYPES];

    int numAttacks;
    int hits;
    int tactics;
    AnsiString name;

    AnsiString spec;
    int specLevel;

    int flags;
    int regen;
    int stea;
    int obse;

    AMonstrType();
    ~AMonstrType();
    AnsiString Print(bool quotation = false);
    void CreateNew(AMonstrType * prevmn);
};
class AMonstrTypes:TList{
public:
    AMonstrTypes();
    virtual __fastcall ~AMonstrTypes();
    AMonstrType* Get(int index);
    AMonstrType* Get(AnsiString abr);
    void AddMonstrType(AMonstrType* mtype);
    void DelMonstrType(AnsiString name);
	bool modified;
    void Save();

    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};
extern AMonstrTypes*MonstrTypes;

class ABatItemType{
public:
enum{
 MAGEONLY=0x1,
 SPECIAL=0x2,
 SHIELD=0x4,
};
    AnsiString item;
    int flags;
    int shtype;
    AnsiString spec;
    int level;

    ABatItemType();
    ~ABatItemType();
    AnsiString Print(bool quotation = false);
    void CreateNew(ABatItemType * prevbt);
};
class ABatItemTypes:TList{
public:
    ABatItemTypes();
    virtual __fastcall ~ABatItemTypes();
    ABatItemType* Get(int index);
    ABatItemType* Get(AnsiString abr);
    void AddBatItemType(ABatItemType* bitype);
    bool modified;
    void Save();

    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};
extern ABatItemTypes*BatItemTypes;

class AManType{
public:
    AnsiString abr;
    int deflevel;
    TStringList *skills;

    AManType();
    ~AManType();
    AnsiString Print(bool quotation = false);
    int GetLevel(AnsiString skill);
    int GetLevelIndex(AnsiString skill);
    void SetLevel(AnsiString skill,int level);
};
class AManTypes:TList{
public:
    AManType*leadertype;

    AManTypes();
    virtual __fastcall ~AManTypes();
    AManType* Get(int index);
    AManType* Get(AnsiString abr);
	void AddManType(AManType* mtype);
	void DelManType(AnsiString name);
	int GetLevel(AnsiString race,AnsiString skill);
    bool modified;
    void Save();
    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};
extern AManTypes*ManTypes;

AnsiString MakeDepSkillStr(AnsiString skill,int level);
AnsiString MakeInputItemStr(AnsiString item,int number);

AnsiString GetGameFileName(AnsiString fName);
AnsiString GetOrderName(int facnum);
AnsiString GetReportName(int facnum,AnsiString basedir="");
void LoadGameBase();
void SaveGameBase();
void DeleteGameBase();
void DisableAutoSaveTimer(); //in MainFrm.cpp
void SetAutoSaveTimer();     //in MainFrm.cpp
void DeleteAutoSaveFiles();     //in MainFrm.cpp

void SetMainFormCaption();
int GetTurnNumberFromString(AnsiString str);
AnsiString TurnNumberToString(int number);

bool InRegionList(ARegion *reg); //in RegionListForm.cpp
bool inFacDiap(int facdiap, AFaction *fac, AFaction *basefac=0);
//facdiap: -2 - all, -3 - local, -4 - nonlocal, -5 - friendly, -6 -unfriendly
//-7 - nonfriendly

extern int curTurnNumber;
extern ATurn*curTurn;
extern ATurns*curTurns;
extern ARegion*curRegion;
extern int BaseVer;
extern bool optShowEvents;
extern bool optColorUnit;
extern bool optRunRegionOrders;
extern int optTownNameLimit;
extern bool optMaximizeWindow;
extern bool optShowLDescFirst;
extern bool optUseOldMail;

extern AnsiString optServEMail,optServOrdersSubj,optServTimesSubj,optServTimesBody
 ,optServMailSubj;
enum ServOpt{
 WFLIGHT_NONE=0,
 WFLIGHT_MUST_LAND,
 WFLIGHT_UNLIMITED,
};
extern int optServFlightOverWater;
extern bool optConquest;
extern bool optNoServerMail,optServNoCross,optServNoSpoils,optServGiveAll,optServSpoils,optServerSharing;
extern bool optBoldUndergroundGrid;
extern int optAutoSaveTimer;
extern bool optUpdateMovingMap;
extern bool optTaxIsMonthlong;
extern bool optMagicDeep;
extern bool optHideGrid;
extern bool optAlwaysShowMovement;
extern TStringList *EventsWarnings, *ViewUnitsCols,*ViewUnitsSets;
extern unsigned optMaxMemoryTurns;
extern int optCombinedTaxTrade;
extern AnsiString optRunRegionOrdersEventPrefix;
extern bool optClearEventsOnRunRegionOrders;
extern AnsiString optPrevNextUnitExpr;

enum EnDontShowEvents{
 dseDiscard=1<<0,
 dseNoMonth=1<<1,
 dseRegDataFound=1<<2,
 dseIncome=1<<3,
 dseCantReach=1<<4,
 dseBorrows=1<<5,
 dseTeachNonLocNotStudy=1<<6,
};
extern int optDontShowEvents;
extern int optSaveMap;
extern bool optCopyOnSaveOrders;

void ReadStringAsInt(TFile &in,AnsiString &str);
void WriteStringAsInt(TFile &out,AnsiString str);
void ReadIntAsShort(TFile &in, int &i);
void WriteIntAsShort(TFile &out, int i);
void ReadIntAsByte(TFile &in, int &i);
void WriteIntAsByte(TFile &out, int i);

int DeleteFiles(AnsiString fname);
int MoveFiles(AnsiString oldname,AnsiString newname);
int CopyFiles(AnsiString oldname,AnsiString newname);
unsigned GetFileSize(AnsiString fname);
void SetBits(int &value,int mask,bool set);

void seedrandom(int num);
int getrandom(int range);
AnsiString AttTypeToStr(int atttype);
int StrToAttType(AnsiString str);
AnsiString WeapClsToStr(int weapcls);
int StrToWeapCls(AnsiString str);

AnsiString StrForURL(AnsiString str);
bool InputMemo(AnsiString Caption, AnsiString &Value);
int AnsiIPos(AnsiString str,AnsiString substr); //without case sensitivity

void LogItem(int logtype, int number, AItemType *itype, AUnit *un); //in BalanceForm.cpp
void LogItem(int logtype, int number, AnsiString item, AUnit *un); //in BalanceForm.cpp

template<class T> class TValueRestorer{
private:
  T&ptr;
  T value;
public:
  TValueRestorer(T&p):ptr(p),value(p){}
  ~TValueRestorer(){ptr=value;}
};

extern AScripts*AllScripts;

class AScripts:TList{
public:
  AScript* Get(int index);
  void Add(AScript* scr);
  int IndexOf(AScript *scr){ return TList::IndexOf(scr);}
  bool Delete(int ind);
  void __fastcall Clear();
  
	__property int count  = { read = Getcount };
	__property List;

private:
	int __fastcall Getcount(){return Count;}
};

class AScript{
public:
	AScript();
	virtual __fastcall ~AScript();
	AnsiString caption;
	bool NeedClear;
	int CountStr;
	TStringList *Strings;
};
//---------------------------------------------------------------------------
// Specials & Abilities
/*
class DefenseMod {
	public:
		int type;
		int val;
};
//---------------------------------------------------------------------------
class DamageType {
	public:
		int type;
		int minnum;
		int value;
		int flags;
		int dclass;
		int effect;
};
//---------------------------------------------------------------------------
class SpecialType {
	public:
		char *specialname;

		enum {
			HIT_BUILDINGIF		= 0x001,	// mutually exclusive (1)
			HIT_BUILDINGEXCEPT	= 0x002,	// mutually exclusive (1)
			HIT_SOLDIERIF		= 0x004,	// mutually exclusive (2)
			HIT_SOLDIEREXCEPT	= 0x008,	// mutually exclusive (2)
			HIT_MOUNTIF		= 0x010,	// mutually exclusive (2)
			HIT_MOUNTEXCEPT		= 0x020,	// mutually exclusive (2)
			HIT_EFFECTIF		= 0x040,	// mutually exclusive (3)
			HIT_EFFECTEXCEPT	= 0x080,	//* mutually exclusive (3)
			HIT_ILLUSION		= 0x100,
			HIT_NOMONSTER		= 0x200,
		};
		int targflags;

		int buildings[3];
		int targets[7];
		int effects[3];

		enum {
			FX_SHIELD	=	0x01,
			FX_DAMAGE	=	0x02,
			FX_USE_LEV	=	0x04,
			FX_DEFBONUS	=	0x08,
			FX_NOBUILDING =	0x10,
			FX_DONT_COMBINE=0x20,
		};
		int effectflags;

		int shield[4];
		DefenseMod defs[4];
		char *shielddesc;

		DamageType damage[4];
		char *spelldesc;
		char *spelldesc2;
		char *spelltarget;
		char *attackdesc;
};
extern SpecialType *SpecialDefs;
//---------------------------------------------------------------------------
class EffectType {
	public:
		char *name;
		int attackVal;
		DefenseMod defMods[ABattleEnums::NUM_ATTACK_TYPES];
		int cancelEffect;

		enum {
			EFF_ONESHOT = 0x001,
			EFF_NOSET = 0x002,
		};
		int flags;
};
extern EffectType *EffectDefs;

enum {
	EFFECT_DAZZLE,				//
	EFFECT_FEAR,				//
	EFFECT_STORM,				//
	NUMEFFECTS
};

//
// Types of objects.
//
enum {
	O_DUMMY,				//0
	O_ROWINGBOAT,				//1
	O_LONGBOAT,				//2
	O_PINK,  				//3
	O_GALLEON,				//4
	O_WARGALLEY,				//5
	O_WARGALLEON,				//6
	O_AIRSHIP,				//7
	O_HERMITSHUT,				//8
	O_TOWER,				//9
	O_GRANDTOWER,				//10
	O_STOCKADE,				//11
	O_GRANDSTOCKADE,			//12
	O_FORT,					//13
	O_GRANDFORT,				//14
	O_CASTLE,				//15
	O_GRANDCASTLE,				//16
	O_CITADEL,				//17
	O_GRANDCITADEL,				//18
	O_FORTRESS,				//19
	O_GRANDFORTRESS,			//20
	O_MAGICALTOWER,				//21
	O_MAGICALCASTLE,			//22
	O_MAGICALFORTRESS,			//23
	O_PALACE,				//24
	O_CPALACE,				//25
	O_CLOUDPALACE,				//26
	O_HTOWER,				//27
	O_HPTOWER,				//28
	O_NGUILD,				//29
	O_AGUILD,				//30
	O_GDCLIFFS,				//31
	O_EDCLIFFS,				//32
	O_SHAFT,				//33
	O_GREENLAIR,				//34
	O_BROWNLAIR,				//35
	O_BLUELAIR,				//36
	O_REDLAIR,				//37
	O_WHITELAIR,				//38
	O_BLACKLAIR,				//39
	O_RUIN,					//40
	O_CAVE,					//41
	O_DEMONPIT,				//42
	O_CRYPT,				//43
	O_ISLE,					//44
	O_DERELICT,				//45
	O_PEATBOG,				//46
	O_WHIRL,				//47
	O_MAGETOWER,				//48
	O_DARKTOWER,				//49
	O_GIANTCASTLE,				//50
	O_ILAIR,				//51
	O_ICECAVE,				//52
	O_THICKET,				//53
	O_PYRAMID,				//54
	O_BKEEP,				//55
	O_ATEMPLE,				//56
	O_TEMPLE,				//57
	O_HUT,					//58
	O_HOUSE,				//59
	O_AQUEDUCT,				//60
	O_INN,					//61
	O_FAIR,					//62
	O_FARM,					//63
	O_MILL,					//64
	O_MUSHROOMFARM,				//65
	O_IRRIGATION,				//66
	O_VINEYARD,				//67
	O_WINEPRESS,				//68
	O_COTTONPLANTATION,			//69
	O_COTTONCOLLECTORS,			//70
	O_TRUFFLESPLANTATION,			//71
	O_BOARPEN,				//72
	O_TOBACCOPLANTATION,			//73
	O_HORTICULTURE,				//74
	O_SHEEPFARM,				//75
	O_SHEEPDIP,				//76
	O_WOOLINGERFARM,			//77
	O_WOOLINGERDIP,				//78
	O_RANCH,				//79
	O_SLAUGHTERHOUSE,			//80
	O_LIZARDRANCH,				//81
	O_LIZARDSLAUGHTERHOUSE,			//82
	O_SHEPHERDHUT,				//83
	O_MENAGERIE,				//84
	O_FISHINGFLEET,				//85
	O_JETTY,				//86
	O_ARTIFICIALREEF,			//87
	O_DIVINGFLOATS,				//88
	O_WHALINGFLEET,				//89
	O_MINE,					//90
	O_FURNACE,				//91
	O_GEMMINE,				//92
	O_GEMAPPRAISER,				//93
	O_MITHRILMINE,				//94
	O_PROSPECTORSHUT,			//95
	O_ADMANTIUMMINE,			//96
	O_GOLDMINE,				//97
	O_TREENURSERY,				//98
	O_TIMBERYARD,				//99
	O_TREETENDERSHUT,			//100
	O_SAWINGMILL,				//101
	O_YEWGROVE,				//102
	O_QUARRY,				//103
	O_STONECUTTERSHUT,			//104
	O_ROOTSTONEQUARRY,			//105
	O_STONEMASONSHUT,			//106
	O_CORESTONEQUARRY,			//107
	O_HUNTINGHUT,				//108
	O_HUNTINGSTAND,				//109
	O_TRAPPINGHUT,				//110
	O_CAGETRAP,				//111
	O_WALKWAY,				//112
	O_GASTANK,				//113
	O_TRAPPINGLODGE,			//114
	O_HERBGARDEN,				//115
	O_HERBALISTSTORE,			//116
	O_FUNGUSGARDEN,				//117
	O_DRYINGSHED,				//118
	O_DRUIDCIRCLE,				//119
	O_PONYSTABLE,				//120
	O_WOLFPEN,				//121
	O_SPIDERSTABLE,				//122
	O_STABLE,				//123
	O_CAMELSTABLE,				//124
	O_LIZARDSTABLE,				//125
	O_EYRIE,				//126
	O_BATCAVE,				//127
	O_ELEPHANTSTABLE,			//128
	O_MAMMOTHSTABLE,			//129
	O_MOLEBURROW,				//130
	O_WINGEDHORSESTABLE,			//131
	O_FLYINGLIZARDSTABLE,			//132
	O_WARGPIT,				//133
	O_TOURNAMENTCOURSE,			//134
	O_TRAININGPIT,				//135
	O_HARPYNEST,				//136
	O_BREEDINGPIT,				//137
	O_CHIMERAROOST,				//138
	O_MAMMOTHWARSCHOOL,			//139
	O_VOLCANICPOOL,				//140
	O_KENNEL,				//141
	O_TOURNAMENTPARCOURS,			//142
	O_TRAININGPEN,				//143
	O_VIRGINARY,				//144
	O_DRAKENSTEIN,				//145
	O_GRIFFONSTABLE,			//146
	O_ELEPHANTWARSCHOOL,			//147
	O_FIREDRAKECLIFF,			//148
	O_TAVERN,				//149
	O_JOINER,				//150
	O_SMITHY,				//151
	O_WEAPONFORGE,				//152
	O_ARMORFORGE,				//153
	O_TEAMTRAINER,				//154
	O_APOTHECARY,				//155
	O_PELTMONGER,				//156
	O_SCULPTOR,				//157
	O_JEWELER,				//158
	O_FISHERY,				//159
	O_FARMHOUSE,				//160
	O_RANCHHOUSE,				//161
	O_CHANDLER,				//162
	O_RUNEMASTER,				//163
	O_ROADN,				//164
	O_ROADNW,				//165
	O_ROADNE,				//166
	O_ROADSW,				//167
	O_ROADSE,				//168
	O_ROADS,				//169
	O_RAFT,
	O_TEPUKE,
	O_SLOOP,
	O_CUTTER,
	O_SCHOONER,
	O_SNOW,
	O_JUNK,
	O_DHOW,
	O_BRIG,
	O_TARTAN,
	O_CARAVEL,
	O_CARRACK,
	O_GALLEY,
	O_COG,
	O_FLOATINGFORTRESS,
	O_GHOSTSHIP,
	O_BALLOON,
	O_SIGNPOST,
	O_ELEFANTWARSCHOOL,
	O_BEHEMOTHWARSCHOOL,
	NOBJECTS
};
*/
#endif
