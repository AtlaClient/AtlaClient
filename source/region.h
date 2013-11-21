//---------------------------------------------------------------------------
#ifndef regionH
#define regionH
//---------------------------------------------------------------------------
#include <comctrls.hpp>
#include <map.h>
#include "MainFrm.h"
class AUnit;
class AUnits;
class ARegion;

unsigned GetXYZKey(unsigned x,unsigned y,unsigned z);

class ALocation{
public:
 ALocation():xloc(-1),yloc(-1),zloc(-1){};
 ALocation(int x,int y,int z):xloc(x),yloc(y),zloc(z){};
 int xloc,yloc,zloc;
 void Read(TFile & in);
 void Write(TFile & out);
 void operator=(const ARegion &reg);
 int operator==(const ARegion &reg);
 int operator==(const ALocation &loc){
  return xloc==loc.xloc&&yloc==loc.yloc&&zloc==loc.zloc;
 }
};

class AObject{
public:
    AnsiString name;
    AnsiString describe;
    bool inner;
    ALocation innerloc; //xloc==-1 not linked
    int num; //0 - DUMMY
    AnsiString type;
    int incomplete,endincomplete; //for road (-1 decay warning, -2 maint warning)
    int runes;
    AUnits *units;
    ARegion *basereg,*endreg;
    int cursailors;

    AUnit *owner;

    ATurn *turn;
    AObject(ATurn *t);
    virtual __fastcall ~AObject();
    void __fastcall Clear();

    bool WriteReport(TStream *strm,int obs,int truesight,bool detfac,int mapoptions);
    AnsiString Print();
    AnsiString FullName();
    AnsiString ObjectHeader(bool isreport);
    void CreateNew(AObject * obj);
    void Update(AObject * newobj);
    void UpdateFromPrev(AObject * prvobj);
    void Read(TFile & in);
    void Write(TFile & out);
    void AddUnit(AUnit* unit);
    void MakeTree(TTreeNode * parnode, VTreeNodes * vtns);
    void MakeView(VListItems * vlis);
    AUnit * GetUnit(int num);
    bool PresentLocals();
    int Protect();
    int Capacity();
    int Sailors();
    bool IsTower();
    bool IsBuilding();
    bool IsLair();
    bool IsShip();
    int IsRoad();
	AUnit * GetOwner();
    int GetSize();
    bool Destroyable();
    void SetNextOwner();

};
enum RunStop{
 rsBeforeBuild=0,rsAfterGive,rsAfterPrepareConsume,
 rsAfterBuy,rsAfterEnter,

 rsNoStop
};
extern AObject *curObject;
class ARegionStatus{
public:
  bool HasTower:1;
  bool HasBuilding:1;
  bool HasShaft:1;
  bool HasLair:1;
  bool HasShip:1;
  int HasRoad:NDIRS;
  int HasGoodRoad:NDIRS;

  ARegionStatus();
};

typedef map<int,int> TMapII;
class ARegion:public TList{
public:
    unsigned key;
    AnsiString type;
    int xloc,yloc,zloc;
    bool hasInfo;
    int oldinfoturnnum;
//    int infoStatus;
    AnsiString name;
    AnsiString towntype;
    AnsiString townname;
	AnsiString race;        //isRegionInfo
    int population;         //isRegionInfo
    int money;              //isRegionInfo
    int taxincome;        //0 - set to auto
    int curtaxprod;
    int taxers;
    int workincome;        //0 - set to auto
    int workers;
    int entincome;         //0 - set to auto
    int entattempt; 
    AnsiString weather,nxtweather; //isWeatherInfo

    int wages;              //isWagesInfo
    int maxwages;           //isWagesInfo
    int entertainment;      //isEntertainInfo
    ALocation neighbors[NDIRS];   //isExitsInfo
    int gate;               //isGateInfo 0 -not set
    AMarkets *wanteds;      //isWantedInfo
    AMarkets *saleds;       //isSaledInfo
    AMarkets *products;       //isProductInfo

    int lastviewfact;
    AnsiString descr;
    bool runned;

    TMapII claims;

/*    struct{
     bool hasLocal:1;
     bool hasLocalTaxers:1;
    };*/
    ATurn *turn;
    ARegion(ATurn *t);
    virtual _fastcall ~ARegion();
    void UpdateKey(){key=GetXYZKey(xloc,yloc,zloc);}
    void __fastcall Clear();
    void Add(AObject * obj);
    AObject * Get(int num);
	AObject * GetObject(int objnum);
    void Read(TFile &in);
    void Write(TFile &out);
    void CreateNew(ARegion * prevreg);
	AnsiString ShortName();
	AnsiString GetLocation();
	AnsiString FullName(bool fortemplate=true);
	bool WriteReport(TStream *strm,int mapoptions);
    void Update(ARegion * newreg);
    int GetDistance(int x, int y, int z);
    void UpdateFromPrev(ARegion * prvreg);
    AnsiString Print();
    void CreateDummy();
    AObject * GetDummy();
    void MakeTree(TTreeNode * parnode, VTreeNodes * vtns);
    void MakeView(VListItems * vlis);
    AUnit * GetUnit(int num);
    bool PresentLocals();
    bool CanTax(AUnit * u);
    void RunOrders(bool immed,RunStop rs=rsNoStop);
    void RunTaxOrders();
    bool HasLocalTaxers();
    bool HasLocalTraders();
    void RunConsumeOrders();
    void RunStudyOrders();
    void RunAttackOrders();
    bool RunStealOrders();
    bool RunGiveOrdersForUnit(AUnit *un);
    bool RunGiveOrders();
    bool PrepareOrders(); //false if no units
    void PrepareOrders2();
    void RunCastOrders();
    void VerifyBuySellOrders();
    bool RunSellOrders();
    bool RunBuyOrders();
    bool RunBuySellOrders(int type,AMarket * mark);
    void RunEntertainOrders();
    void RunWorkOrders();
	void PrepareConsume();
    void RunFlagOrders();
    void CheckUnitMaintenanceItem(AUnits *units,AnsiString item, int value, bool consume);
    int CheckFactionMaintenanceItem(AUnits *units,AnsiString item, int value, bool consume);
    int CheckFactionSharedItems(AUnit *tekU, AnsiString item, int value, int typeS);
    bool RunForgetOrders();
    void RunWithdrawOrders();
    bool RunTeachOrders();
    bool RunPillageOrders();
    void RunProduceOrdersA();
    void RunProduceOrdersA(AMarket *mark);
    void RunProduceOrders();
    bool CanMakeAdv(AFaction * fac, AnsiString item);
    void RunEnterLeaveOrders();
    void RunPromoteOrders();
    bool IsCoastal();
    int MoveCost(int movetype);
    AnsiString NxtWeather();
    void RunGuard1Orders();
    void RunMoveOrders();
    void RunAutoOrders1();
    void RunBuildOrders();
    void RunSailOrders();
    void RunPluginOrders(int runtime);
    void RunPluginAutoOrders(int runtime);
    void ClearRegionStatus();
    void UpdateRegionStatus();
    bool PresentFaction(AFaction *fac);
    int GetObservation(AFaction * fac);
    int GetTrueSight(AFaction * fac);
    int GetSize();
    bool HasExitRoad(int dir);
    bool HasConnectingRoad(int dir);
    bool IsSaveRegion();
    void CreateExits(int exits);
	int GetNewUnitNum();
	bool IsTaxRegion(int facnum);
	bool IsTradeRegion(int facnum);
	bool HasBattles();
	void MakeTreeBattle(TTreeNode * parnode, VTreeNodes * vtns);
	
	__property int count  = { read = Getcount };
	__property List;
	__property ARegionStatus* RegionStatus  = { read = GetRegionStatus };
private:
	ARegionStatus* FRegionStatus;
	int __fastcall Getcount(){return Count;}
	ARegionStatus* __fastcall GetRegionStatus();
};

extern ARegion*curRegion;

class ARegionArray{//:TList{
public:
	ARegionArray();
	int x,y;
	AnsiString name;
	int levelnum;
	ARegion* Get(int num);
	ARegion* Get(int x, int y);
	void Read(TFile &in);
	void Write(TFile &out);
	void CreateNew(ARegionArray* prevra);
    ~ARegionArray();
    void NormCoord(int & xloc, int & yloc);
    void UpdateFromPrev(ARegionArray * prvar);
    void VerifySize(int xloc, int yloc);
    void NormCoord2(int & xloc, int & yloc);
    int GetSize();
    AnsiString GetName();
//    void Add(ARegion* reg);
//    DYNAMIC void __fastcall Clear();
};

extern ARegionArray*curRegionArray;

class ARegionArrays:TList{
public:
	ARegionArrays();
	virtual __fastcall ~ARegionArrays();
	void Read(TFile &in);
	void Write(TFile& out);
	void Add(ARegionArray* ar);
	ARegionArray* Get(int levelnum);
	void __fastcall Clear();
	void CreateNew(ARegionArrays * prevras);
	ARegionArray* Get(AnsiString levelname);
	int GetLevelNum(AnsiString levelname);
	void UpdateFromPrev(ARegionArrays * prvars);
	int GetSize();

	__property int count  = { read = Getcount };
	__property List;
private:
	int __fastcall Getcount(){return Count;}
};

class ARegionList:TList{
public:
	int numberofgates;
	ARegionArrays*RegionArrays;

	ATurn *turn;
	ARegionList(ATurn *t);
	virtual __fastcall ~ARegionList();
	void __fastcall Clear();
	void Read(TFile& in);
	void Write(TFile& out);
	void Add(ARegion * reg);
	void Delete(ARegion *reg);
	TList::IndexOf;
	ARegion* Get(int regnum);
	ARegion* GetByKey(unsigned key);
	ARegion* Get(int x, int y, int z);
	ARegion * Get(ALocation & loc){return Get(loc.xloc,loc.yloc,loc.zloc);}
	ARegionArray* GetRegionArray(int level);
	bool SetCurRegionArray(int levelnum);
	void CreateNew(ARegionList* prevrl);
	bool SetCurRegion(int x, int y, int z);
	ARegion * GetNearestRegion(int x, int y, int z);
	void UpdateFromPrev(ARegionList * prvrl);
	//return true, if advprods added
	bool UpdateAdvProdsFromPrev(ARegionList * prvrl,bool withmessages=false);
	//void UpdateFromNext(ARegionList * nxtrl);
	AUnit * GetUnit(int num);
	int GetNewUnitNum();

	int GetSize();
	__property int count  = { read = Getcount };
	__property List;
private:
	int __fastcall Getcount(){return Count;}
};

extern ARegionList*curRegionList;
#endif
