//---------------------------------------------------------------------------
#ifndef changesH
#define changesH
#include <vector.h>
//---------------------------------------------------------------------------
class TChanges
{
public:
  class TElem{
  public:
    enum{ctUnk=0,
      ctAdded=1,ctDeleted=2,
      ctChangedFaction=4,
      ctChangedItems=8,
      ctChangedLocation=16,

      ctChanged=ctChangedFaction|ctChangedItems|ctChangedLocation,
    };
    int type;
    AUnit *from,*to;
    int turnnum; //для from

    TElem():type(ctUnk),from(0),to(0),turnnum(0){}
  };
  vector<TElem> elems;
  int turnnum;
  int addcount,delcount,changecount;

  TChanges(){}
  ~TChanges(){Clear();}
  void Clear();
  bool Collect();
  bool Edit();
  bool CompareFaction(AUnit * from, AUnit * to);
  bool CompareItems(AUnit * from, AUnit * to);
  bool CompareLocation(AUnit * from, AUnit * to);
  bool CompareRegion(AUnit * from, AUnit * to);
  int Compare(AUnit * from, AUnit * to);
  bool VerifyAdded(int turncount);
};

extern TChanges Changes;
#endif
