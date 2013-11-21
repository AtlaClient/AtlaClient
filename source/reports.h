//---------------------------------------------------------------------------
#ifndef reportsH
#define reportsH
//---------------------------------------------------------------------------
bool ProcessReport(AnsiString fName);
bool ProcessOrder(AnsiString fName);
bool ProcessHeaderReport(char*&s);
bool ProcessErrorReport(char*&s);
bool ProcessBattlesReport(char*&s);
bool IsEventReport(const char*s);
bool ProcessEventReport(char*&s);
bool IsSkillReport(const char*s);
bool ProcessSkillReport(char*&s);
bool ProcessItemReport(char*&s);
bool ProcessObjectReport(char*&s);
bool ProcessFactionReport(char*&s);
bool ProcessItemsList(char *&s, AItems *list);
bool ProcessMarketsList(char *&s, AMarkets *list);
bool ProcessSkillsList(char *&s, ASkills *list);
bool ProcessShortSkillsList(char *&s, TStringList *list);
bool IsRegionReport(const char*s);
bool ProcessRegionsReport(char*&s);
bool IsOrderReport(const char*s);
bool ProcessOrdersReport(char*&s);
ARegion* ProcessRegionHeader(AnsiString header);
bool ProcessRegionReport(char*&s);
class AFaction;
bool ProcessUpdateFaction(AFaction *newFaction,int ver=-1);
class ASkillType;
bool ProcessUpdateSkill(ASkillType*newSkill,int ver=-1);
class ASkillInfo;
bool ProcessUpdateSkillInfo(ASkillInfo*newSi);
class AItObjInfo;
bool ProcessUpdateItObjInfo(AItObjInfo*newIoi);
class AItemType;
bool ProcessUpdateItem(AItemType*newit,AnsiString descr="");
bool ProcessUpdateTownType(AnsiString newttype);
bool ProcessUpdateRegionArray(AnsiString newname);
class ARegion;
bool ProcessUpdateRegion(ARegion *newreg);
bool ProcessUpdateObjectType(AnsiString name,int canenter); //2 - first 

bool SaveReport(AnsiString fName,int facnum);
bool SaveOrder(AnsiString fName,int facnum);

bool SaveMap(AnsiString fName,int mapoptions);
//1-normal battle, 2-ass attempt, 3-ass info
int IsBattleReport(char *s, ALocation *loc=0);
bool ExtractBattleUnitName(char *&s,int &num,AnsiString *name);
bool ProcessBattleReport(char*&s,ABattleReport *rep);
void SkipEmptyLines(char*&s);

#endif
