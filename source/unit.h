//---------------------------------------------------------------------------
#ifndef unitH
#define unitH
//---------------------------------------------------------------------------
#include <comctrls.hpp>
enum {
 INFO_NONE=0,
 INFO_UNIT,
 INFO_FACTION,
 INFO_LOCAL
};
enum {
	GUARD_NONE=0,
    GUARD_GUARD,
    GUARD_AVOID,
    GUARD_SET,
//    GUARD_ADVANCE
};
enum {
  REVEAL_NONE=0,
  REVEAL_UNIT,
  REVEAL_FACTION
};
enum{
 FLAG_BEHIND=1,
 FLAG_NOCROSS_WATER=2,
 FLAG_AUTOTAX=4,
 FLAG_HOLDING=8,
 FLAG_NOAID=16,
 FLAG_INVIS=32,
 FLAG_CONSUMING_UNIT=64,
 FLAG_CONSUMING_FACTION=128,
 FLAG_CONSUMING=FLAG_CONSUMING_UNIT|FLAG_CONSUMING_FACTION,
 FLAG_NOSPOILS=0x100,
 FLAG_FLYSPOILS=0x200,
 FLAG_WALKSPOILS=0x400,
 FLAG_RIDESPOILS=0x800,
 FLAG_SHARING=0x1000,
};
class AOrderComment;
class AUnit{
public:
    AnsiString name;
    AnsiString describe;
    int num; //minus for new
    union{
     struct{
      unsigned flags :16;
	  unsigned info :3;
      unsigned guard :3;
      unsigned reveal:3;
      unsigned hasskills:1;
      unsigned hascombatspell:1;
      unsigned hascanstudy:1;
      unsigned hasparams:1;
      unsigned reserved:3;
     };
     int globalflags;
    };
    unsigned endflags;
    unsigned endguard;
    unsigned endreveal;
    AFaction *endfaction;
    int needed; //used for maint

    AOrders *orders;
    AItems *items, *enditems;
    ASkills *skills, *endskills;
    AnsiString combatspell;
    TStringList *canstudy;

    int weight, walkweight,ridweight, flyweight,swimweight;
    AObject *baseobj, *endobj;
	AFaction *faction;
    ATurn *turn;

    int movepoints;
    int losses; //for BSim

    AnsiString params; //extra params (ex. Weight: .. Capacity: ..)

    AUnit(ATurn *t);
    virtual __fastcall ~AUnit();
    void Read(TFile & in);
    void Write(TFile& out);
    void CreateNew(AUnit * prvunit);
    void Update(AUnit* newun);
    void UpdateFromPrev(AUnit * prvun);
    AnsiString Print();
    bool WriteReport(TStream *strm,int obs,int truesight,bool detfac,bool autosee);
    AnsiString BattleReport(int obs=2);
    AnsiString FullName(bool fortemplate=false);
    void MakeTree(TTreeNode * parnode, VTreeNodes * vtns);
    AnsiString PrintForTree();
    void MakeView(VListItems * vlis);
    void FillEventsList(TStrings *list);
    AnsiString GetFlagsStr1(); //guard
    AnsiString GetFlagsStr2(); //avoid,behind
	AnsiString GetFlagsStr3(); //REVEAL,...
    AnsiString GetEndFlagsStr(bool fortree=false); //guard, faction,avoid,...
    AnsiString UnitHeader(bool fortemplate=false);
    bool GetFlag(int flag);
    void SetFlag(int flag, bool value);
    bool GetEndFlag(int flag);
    void SetEndFlag(int flag, bool value);
    AnsiString GetLocalDescr();
    void SetLocalDescr(AnsiString descr);
    AOrder * GetOrder(int num);
    bool IsLocal();
    bool SaveTemplate(TFileStream * file);
    void FormNew(AUnit *base);
    void RunOrders();
    int GetMen(AnsiString type);
    int GetMen();
    int GetBegMen();
    int GetAttitude(ARegion * reg, AUnit * u);
    bool IsAlive();
    int CanSee(ARegion * r, AUnit * u);
    void SetFaction(int facnum);
    bool HasTaxOrder();
    bool HasTradeOrder();
    int Taxers();
    void SetMoney(int count);
	int GetMoney();
    AnsiString Alias();
    int GetBegRealSkill(AnsiString type);
    int GetBegSkill(AnsiString type);
    int GetRealSkill(AnsiString type);
    int GetSkill(AnsiString type);
    int GetBegSkillDays(AnsiString type);
    int GetSkillDays(AnsiString type);
    void SetBegSkillDays(AnsiString type, int val);
    void SetSkillDays(AnsiString type, int val);
    bool IsLeader();
    bool IsNormal();
    void UpdateWeight();
    bool CanStudy(AnsiString skill);
    bool BegCanStudy(AnsiString skill);
    bool CheckDepend(int lev, AnsiString dep, int deplev);
    bool BegCheckDepend(int lev, AnsiString dep, int deplev);
    int MoveType();
    int CalcMovePoints(int movetype=-1); //use plugins PE_CALCMOVEPOINTS
    AnsiString FullName2();
    int GetBegObservationBonus();
    int GetBegObservation();
    int GetBegStealthBonus();
    int GetBegStealth();
	int GetBegTactics();
	int GetBuildBonus(AObjectType *otype);
    int GetEndEntertainment();
    bool IsMage();
    int GetSize();
    bool GetBattleItem(int battype, AnsiString item);
    void ReturnBattleItem(AnsiString item);
    int GetSoldiers();
    AUnit * CreateBattleUnit();
    void SetFlagOrders(unsigned guard,unsigned reveal,unsigned flags);
    bool CanFly();
    bool CanReallySwim();
    bool CanSwim();
    int GetMaxLevel(AnsiString skill);
    static int GetSpoilsFromFlag(unsigned flags);
    static unsigned GetFlagFromSpoils(int spoils);
    int CanUseWeapon(AWeaponType * wtype, bool riding);
    int CanUseWeapon(AWeaponType * wtype);
    void PrepareOrders();
    bool CanSpy();
    void UnPlug();//unplugs all orders
    int CalcExpr(AnsiString expr);

//ищет коментарий, начинающийся с строки
    AOrderComment *FindCommentOrder(AnsiString str);
    AnsiString GetComment(AnsiString str);
	void SetComment(AnsiString str, AnsiString value);
    int CalcNeeded();
    int CalcStudyCost(AnsiString abr);
};
extern AUnit *curUnit;
class AUnits:TList{
public:
    bool autodelete;

    AObject *object;
    AUnits(AObject *obj);
    virtual __fastcall ~AUnits();
    void __fastcall Clear();
    void Read(TFile& in);
    void Write(TFile & out);
    AUnit* Get(int index);
    void Add(AUnit* un);
    void CreateNew(AUnits* prvunits);
    void Update(AUnits* newunits);
    int IndexOf(AUnit *un){ return TList::IndexOf(un);}
    AUnit* GetUnit(int num);
    bool Delete(int num);
    bool DeleteByIndex(int ind);
    int GetSize();
    TList::Sort;

    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}

};
#endif
