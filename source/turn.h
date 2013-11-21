//---------------------------------------------------------------------------
#ifndef turnH
#define turnH
//---------------------------------------------------------------------------
#include <vector.h>
const MaxLastLevels=10;
class ARegion;
class ARegionList;
class AFactions;
class ATurns;
class ATurn{
private:
    TStringList *FRegUserInfos;
    void __fastcall SetRegUserInfos(ARegion * reg, AnsiString value);
    AnsiString __fastcall GetRegUserInfos(ARegion * reg);
public:

    int num;
    bool Modified;
    bool AllOrdersRunned;
    ATurns *turns;

    ARegionList*RegionList;
    AFactions*factions;
    ATurn();
    ~ATurn();
    void Read();
    void Write();
    void CreateNew(ATurn * prevturn);
//    void Update0(ATurn *newturn);
    void VerifyNextTurn(bool withadvprods=false);
    void PrepareOrders();
    void RunOrders();
    void RunAutoOrders();
    int GetSize();
    AnsiString GetFileName();
    bool SaveMap(TFileStream * file,int mapoptions);
  bool FixShafts(int * added, int * deleted);

    __property AnsiString RegUserInfos[ARegion *reg]  = { read=GetRegUserInfos, write=SetRegUserInfos };
};
class ATurns:TList{
protected:
    class Locks{
    public:
      int turn;
      int locks;
      int tick;
      int used;
      Locks(int t):turn(t),locks(0),tick(0),used(0){}
      bool operator<(const Locks&b) const {return tick>b.tick;}
    };
    vector<Locks> locks;
public:
    TStringList *tertypes,*provincies,*towns,*towntypes;
    TStringList *expr_comms;
    TStringList *warning_exprs;
    TStringList *ignore_params;
    TStringList *pluginsstr;
    TStringList *extra_magic_skills;
    TStringList *first_magic_skills;
    TStringList *last_magic_skills;
    int lastfac;
    int /*lastx,lasty,*/lastz;
    int lastx[MaxLastLevels],lasty[MaxLastLevels];
    AnsiString ver1,ver2;
    int curbeginturn,newbeginturn;
    int autobeginturn;
    int keyturn;
    TStringList *faccomments;

    ATurns();
    virtual __fastcall ~ATurns();
    void __fastcall Clear();
    void Delete(int num);
    void DeleteLast();
    void Read();
    void Write();
    void Add(ATurn * turn);
    ATurn* Get(int turnnum);
    bool SetCurTurn(int turnnum);
    void CreateNew();
    bool RestoreLastParam();
    void StoreLastParam();
    void RestoreLastXY(int &xloc,int &yloc,int zloc);
    void StoreLastXY();
    void PrepareTurn(int num);
    BYTE Tertypes(AnsiString);
    WORD Provincies(AnsiString);
    WORD Towns(AnsiString);
    BYTE Towntypes(AnsiString);
    int GetSize();
    int GetNextTurnNum(int num);
    int GetPrevTurnNum(int num);
    bool PreloadTurn(int num);
    bool PreloadTurnIfNeed(int num, bool question=false);
    int LoadedCount(AnsiString *list=0);
    void MinimizeTurns(int curnum);
    void UpdateTurns();
    void FillEventsWarning();
    void UnPlug();
  void LockTurn(int turn);
  void UnlockTurn(int turn, bool clean=false);
  void CleanTurns();
    __property int count  = { read = Getcount };
    __property List;
    __property AnsiString FacComments[int facnum]  = { read=GetFacComments, write=SetFacComments };
private:
    int __fastcall Getcount(){return Count;}
    void __fastcall SetFacComments(int facnum, AnsiString value);
    AnsiString __fastcall GetFacComments(int facnum);
    int GetFacCommentIndex(int facnum);
};

#endif
