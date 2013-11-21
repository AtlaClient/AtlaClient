//---------------------------------------------------------------------------
#ifndef acpluginsH
#define acpluginsH
#include <vector.h>

class EData;
class TPlugin;
class TPlugins:public vector<TPlugin*>
{
public:
  TPlugins(){};
  ~TPlugins();
  void UpdatePluginInfo(int num = -1);
  void ExitAtlaClient(int from);
  void ProcessEvent(int type, void *param);
  int ProcessSpecEvent(int type, void *param);
  bool FindOrderPlugin(int OrderType,const char *str, int &ModuleNumber, int &OrderNumber);
  int ProcessOrderEvent(int OrderType,int ModuleNumber, int type, void*param, AUnit *un);
  int GetOrderPluginFlag(int ModuleNumber, int OrderNumber);
  int CalcMoveCost(AUnit *un, ARegion *to, int dir, int movetype, int turnnum=-1);
  int CalcMovePoints(AUnit *un, int movetype);
  void OnGameLoaded();
  void OnGameSaved();
  void OnTurnChanged(bool uncond);
  void OnLevelChanged();
  void OnRegionChanged();
  void OnTreeItemChanged();
  void OnSaveOrder(int facnum,bool before);
  void CreateMenu(TMenuItem * first,TNotifyEvent OnClick);
  void CreateConfigMenu(TMenuItem * root,TNotifyEvent OnClick);
  void CreateFunctionMenu(TMenuItem * root,int exprmode,TNotifyEvent OnClick);
  void CreateOrderMenu(TMenuItem * root,int ordertype,TNotifyEvent OnClick);
  void RunMenu(int index);
  void RunConfigure(int index);
  EData * RunExpression(AnsiString name, int exprmode, char *&buf);
  void RunClientOrder(AnsiString str);
  void InitPlugins();
  void ClearPlugins();
};
extern TPlugins Plugins;
extern ARegion *PluginRegion;
extern AUnit *PluginUnit;
extern bool PluginNeedMakeTree;
void InitPlugins();

struct PluginVariable;
//internal PVT_LIST support function
bool ListCreate(PluginVariable *list, int elemtype);
bool ListAddReg(PluginVariable *list, ARegion *reg);
bool ListAddUnit(PluginVariable *list, AUnit *un);
//---------------------------------------------------------------------------
#endif
