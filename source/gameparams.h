//---------------------------------------------------------------------------
#ifndef gameparamsH
#define gameparamsH
//---------------------------------------------------------------------------
#include <vector.h>
//---------------------------------------------------------------------------
extern int LeaderMaintCost;
extern int ManMaintCost;
extern int FoodMaintKoeff;
extern int TaxEffect;
extern int EntertainEffect;
extern int StudyAddDays;
extern int TeachAddDays;
extern int TeachMaxStudents;
extern int SailSpeed;
extern int WalkSpeed;
extern int RideSpeed;
extern int FlySpeed;
extern int WindSpeed;
extern AnsiString SailSkill;
extern AnsiString EnteSkill;
extern AnsiString CombSkill;
extern AnsiString RidiSkill;
extern AnsiString LbowSkill;
extern AnsiString XbowSkill;
extern AnsiString TactSkill;
extern AnsiString SwinSkill;
extern AnsiString SteaSkill;
extern AnsiString ObseSkill;
extern AnsiString TrueSkill;
extern AnsiString IlluSkill;
extern AnsiString MindSkill;
extern AnsiString PhenSkill;
extern AnsiString RingItem;
extern AnsiString AmtsItem;
extern int RingSteaBonus;
extern int AmtsObseBonus;
extern AnsiString XXXXItem;
extern int MaxFactionsPoints;
extern int SilverWeight;

namespace Inifiles{class TIniFile;};using namespace Inifiles;
class TGameParamsForm;
class TGameParam{
public:
  int type; //0-int,1-AnsiString
  AnsiString name;
  AnsiString comment;
  int *ptr;
  AnsiString *ptrstr;
  TGameParam():ptr(0),ptrstr(0){}
  bool IsPtrSet();
  AnsiString GetString();
  bool SetString(const AnsiString &value);
};
class TGameParams{
friend TGameParamsForm;
private:
  vector<TGameParam> params;
  bool AddGameParam(AnsiString name, int * ptr, AnsiString comment);
  bool AddGameParam(AnsiString name, AnsiString * ptr, AnsiString comment);
  TGameParam *FindGameParam(AnsiString name);
  bool SetGameParam(AnsiString name, const AnsiString &value);
public:
  TGameParams();
  ~TGameParams();
  const TGameParam *Find_GameParam(const AnsiString& name) const;
  bool Load(TIniFile *ini);
  bool Save(TIniFile *ini);
  bool Init();
};
extern TGameParams GameParams;
#endif
