//---------------------------------------------------------------------------
#ifndef battlerepH
#define battlerepH
//---------------------------------------------------------------------------
class ABattleReport;
class ABattleRepRound{
public:
 int round; //-2 free att, -1 - free def
 TStringList *casts;
 int attloses,defloses;

 ABattleRepRound();
 ~ABattleRepRound();
 bool Print(ABattleReport * rep,TStringList *list, bool shortmode);
 void AddCast(int num, AnsiString str);
 void SetFreeRoundLoses(int loses);
};
enum WinTypes{
 wtTie=0,//no win
 wtWinAtt,
 wtWinDef
};
class ABattleReport:TList{
public:
    bool ass;
    AUnits *atts,*defs;
    int attleadernum,defleadernum;
    AnsiString attleadername,defleadername;
    int attcount,defcount;
    ARegion *reg;
    WinTypes wintype;
    int loses1,loses2;
    AnsiString losesstr1,losesstr2;
    AUnits *damaged1,*damaged2;
    TStringList *heals1,*heals2;
    AItems *spoils;

    int turnnum;
    int roundlimit;

    __fastcall ABattleReport();
    __fastcall ~ABattleReport();
    AnsiString GetUnitName(int num);
    void __fastcall Clear();
    void Reset();
    void Add(ABattleRepRound * round);
    ABattleRepRound* Get(int ind);
    void SetSides(AUnits * _atts, AUnits * _defs);
    void UpdateLeaders();
    void SetRegion(ARegion * reg);
    void Print(TStringList * list, bool shortmode);
    void WriteSides(TStringList *list);
    void AddHeal(int ind,int num, AnsiString str);
    void PrintDamaged(AUnits * uns,TStringList *list);
    void Run();
    void CompactUnits();
/*    void AutoSetCombatSpell();
    void AutoSetCombatSpell(AUnits *uns);*/
    int PresentUnit(int num);
//    void CreateNew(ABattleReport * rep);
    void Check(); //check for turn is deleted


    __property int count  = { read = Getcount };
    __property List;
    __property int AttLoses  = { read=GetAttLoses };
    __property int DefLoses  = { read=GetDefLoses };
private:
    int __fastcall Getcount(){return Count;}
    int __fastcall GetAttLoses();
    int __fastcall GetDefLoses();
};
#endif
