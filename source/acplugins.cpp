//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <memory>
//#include <map.h>
#include "util.h"
#include "acplugins.h"
#include "plugdoc\plugins.h"
#include "EventsFrm.h"
#include "turn.h"
#include "region.h"
#include "orders.h"
#include "faction.h"
#include "unit.h"
#include "MainFrm.h"
#include "markets.h"
#include "skills.h"
#include "exprlang.h"
#include "items.h"
#include "viewitems.h"
#include "RegionListFrm.h"
#include "shortcuts.h"
#include "gameparams.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
extern AnsiString ACVersionStr; //from AboutFrm
extern AnsiString ClientPath;

static unsigned ACVersion=0;
AnsiString PluginsPath="plugins\\";
ARegion *PluginRegion=0;
AUnit *PluginUnit=0;
bool PluginNeedMakeTree=false;
static EData *ExprArg=0,*ExprRetVal=0;
static int OldTurnNum=-1;
static int OldLevel=-1;
static ARegion *OldRegion=0;
static PluginVariableType OldTreeNodeType=PVT_NONE;
static void *OldTreeNodeData=0;

static map<int, TPlugin*> specevents;

enum TPluginStatus{
  plsNoBase=0,
  plsNormal=1,
  plsExpression=2,
  plsEvent=3,
  plsRunOrder=4,
};
TPluginStatus PluginStatus;

struct PluginVariable{
  int Type;
  union{
    struct{
      int value;
      char *str;
      int reserved;
    };
    struct{ //PVT_TURN
      ATurn *turn;
    };
    struct{ //PVT_REGION
      ARegion *reg;
    };
    struct{ //PVT_OBJECT
      AObject *obj;
      AObjectType *objtype;
    };
    struct{ //PVT_UNIT
      AUnit *unit;
    };
    struct{ //PVT_ITEM
      AItemType *itemtype;
      int itemcount;
      int itemprice;
    };
    struct{ //PVT_SKILL
      ASkillType *skilltype;
      int skilldays;
      int skilllevel;
    };
    struct{ //PVT_ORDER
      AOrder *order;
    };
    struct{ //PVT_FACTION
      AFaction *faction;
    };
    struct{ //PVT_GAME
    };
    struct{ //PVT_LIST
      int ElemType;
      vector<void*> *list;
    };
  };
};
#if sizeof(PluginVariable)!=16
#error Bad alligment: sizeof(PluginVariable)!=16
#endif

class TIndexVars{
protected:
  class TVar{
  public:
    int index; //номер свойства
    bool set;
    PluginVariableType type;
    int intvalue;
    AnsiString strvalue;

    TVar(int _index,PluginVariableType _type)
      :index(_index),set(false),type(_type),intvalue(0){}
  };
  vector<TVar> vars;
  int lastsetindex; //внутренний

  int FindIndexIndex(int index);
public:
  TIndexVars():lastsetindex(-1){}
  void AddVariable(int index,PluginVariableType type);
  int GetLastSetIndex();
  bool IsVarSet(int index);
  bool SetVar(int index,PluginVariable *value);
  bool GetVar(int index,PluginVariable *result);
  int GetVarInt(int index=-1);
  AnsiString GetVarString(int index=-1);
};
TIndexVars IndexVars[PVT_last];

typedef int WINAPI (*_SetStartupInfo)(const struct PluginStartupInfo *Info);
typedef void WINAPI (*_GetPluginInfo)(struct PluginInfo *Info);
typedef HANDLE WINAPI (*_OpenPlugin)(int OpenFrom,int Item);
typedef int WINAPI (*_ProcessEvent)(int event, void *param);

class TPlugin
{
public:
  TPlugin():ModuleNumber(-1),hLib(0){memset(&info,0,sizeof(info));}
  ~TPlugin(){if(hLib)FreeLibrary(hLib);}
  bool Init();
  void UpdateInfo();

  AnsiString ModuleName;
  AnsiString ShortModuleName;
  int ModuleNumber;
  HINSTANCE hLib;
  PluginInfo info;

  _SetStartupInfo SetStartupInfo;
  _GetPluginInfo GetPluginInfo;
  _OpenPlugin OpenPlugin;
  _ProcessEvent ProcessEvent;
};
/*
class TPlugins:public vector<TPlugin*>
{
public:
  TPlugins(){};
  ~TPlugins();
};
*/
TPlugins Plugins;

int WINAPI CopyVariable(PluginVariable *to, const PluginVariable *from);
int WINAPI FreeVariable(PluginVariable *var);
int WINAPI GetVariableProperty(PluginVariable *var, int index, PluginVariable *result);
int WINAPI SetVariableProperty(PluginVariable *var, int index, PluginVariable *value);
int WINAPI RunVariableFunction(PluginVariable *var, int index, PluginVariable *arg);
int WINAPI GetGlobalVariable(int index, PluginVariable * result);
int WINAPI AddEvent(const char *str);
int WINAPI CreateStringVariable(const char *s, PluginVariable * result);
int WINAPI LockPlugin(int ModuleNumber, int locktype, int reserved);

int inline CreateIntVariable(int value, PluginVariable * result)//not exported & no freevar
{
  result->Type=PVT_INTEGER;
  result->value=value;
  return 0;
}

int WINAPI NullCopyVar(PluginVariable *to, const PluginVariable *from);
int WINAPI NullFreeVar(PluginVariable *var);

int WINAPI TurnGetVarProp(PluginVariable *var, int index, PluginVariable *result);
int WINAPI TurnSetVarProp(PluginVariable *var, int index, PluginVariable *value);
int WINAPI RegionGetVarProp(PluginVariable *var, int index, PluginVariable *result);
int WINAPI RegionSetVarProp(PluginVariable *var, int index, PluginVariable *value);
int WINAPI RegionRunVarFunc(PluginVariable *var, int index, PluginVariable *arg);
int WINAPI ObjectGetVarProp(PluginVariable *var, int index, PluginVariable *result);
int WINAPI ObjectSetVarProp(PluginVariable *var, int index, PluginVariable *value);
int WINAPI UnitGetVarProp(PluginVariable *var, int index, PluginVariable *result);
int WINAPI UnitSetVarProp(PluginVariable *var, int index, PluginVariable *value);
int WINAPI UnitRunVarFunc(PluginVariable *var, int index, PluginVariable *arg);
int WINAPI ItemGetVarProp(PluginVariable *var, int index, PluginVariable *result);
int WINAPI ItemSetVarProp(PluginVariable *var, int index, PluginVariable *value);
int WINAPI SkillGetVarProp(PluginVariable *var, int index, PluginVariable *result);
int WINAPI SkillSetVarProp(PluginVariable *var, int index, PluginVariable *value);
int WINAPI OrderCopyVar(PluginVariable *to, const PluginVariable *from);
int WINAPI OrderFreeVar(PluginVariable *var);
int WINAPI OrderGetVarProp(PluginVariable *var, int index, PluginVariable *result);
int WINAPI OrderSetVarProp(PluginVariable *var, int index, PluginVariable *value);
int WINAPI FactionGetVarProp(PluginVariable *var, int index, PluginVariable *result);
int WINAPI FactionSetVarProp(PluginVariable *var, int index, PluginVariable *value);
int WINAPI GameGetVarProp(PluginVariable *var, int index, PluginVariable *result);
int WINAPI GameSetVarProp(PluginVariable *var, int index, PluginVariable *value);
int WINAPI GameRunVarFunc(PluginVariable *var, int index, PluginVariable *arg);
int WINAPI ListCopyVar(PluginVariable *to, const PluginVariable *from);
int WINAPI ListFreeVar(PluginVariable *var);
int WINAPI ListGetVarProp(PluginVariable *var, int index, PluginVariable *result);
int WINAPI ListSetVarProp(PluginVariable *var, int index, PluginVariable *value);

struct TVarFuncs{
  ACAPICOPYVARIABLE CopyVariable;
  ACAPIFREEVARIABLE FreeVariable;
  ACAPIGETVARIABLEPROPERTY GetVariableProperty;
  ACAPISETVARIABLEPROPERTY SetVariableProperty;
  ACAPIRUNVARIABLEFUNCTION RunVariableFunction;
}VarFuncs[PVT_last]={
  {0,0,0,0,0},//PVT_NONE
  {0,0,0,0,0},//PVT_INTEGER,
  {0,0,0,0,0},//PVT_STRING,
  {NullCopyVar,NullFreeVar,TurnGetVarProp,TurnSetVarProp,0},//PVT_TURN,
  {NullCopyVar,NullFreeVar,RegionGetVarProp,RegionSetVarProp,RegionRunVarFunc},//PVT_REGION,
  {NullCopyVar,NullFreeVar,ObjectGetVarProp,ObjectSetVarProp,0},//PVT_OBJECT,
  {NullCopyVar,NullFreeVar,UnitGetVarProp,UnitSetVarProp,UnitRunVarFunc},//PVT_UNIT,
  {NullCopyVar,NullFreeVar,ItemGetVarProp,ItemSetVarProp,0},//PVT_ITEM,
  {NullCopyVar,NullFreeVar,SkillGetVarProp,SkillSetVarProp,0},//PVT_SKILL,
  {OrderCopyVar,OrderFreeVar,OrderGetVarProp,OrderSetVarProp,0},//PVT_ORDER,
  {NullCopyVar,NullFreeVar,FactionGetVarProp,FactionSetVarProp,0},//PVT_FACTION,
  {NullCopyVar,NullFreeVar,GameGetVarProp,GameSetVarProp,GameRunVarFunc},//PVT_GAME,
  {ListCopyVar,ListFreeVar,ListGetVarProp,ListSetVarProp,0},//PVT_LIST,
};
const int VarFuncsCount=sizeof(VarFuncs)/sizeof(*VarFuncs);
bool TPlugin::Init()
{
  if(!SetStartupInfo||!GetPluginInfo)
    return false;
  PluginStartupInfo psi;
  memset(&psi,0,sizeof(psi));
  psi.StructSize=sizeof(psi);
  psi.AtlaClientVersion=ACVersion;
  strcpy(psi.ModuleName,ModuleName.c_str());
  psi.ModuleNumber=ModuleNumber;
  psi.CopyVariable=CopyVariable;
  psi.FreeVariable=FreeVariable;
  psi.GetVariableProperty=GetVariableProperty;
  psi.SetVariableProperty=SetVariableProperty;
  psi.RunVariableFunction=RunVariableFunction;
  psi.GetGlobalVariable=GetGlobalVariable;
  psi.AddEvent=AddEvent;
  psi.CreateStringVariable=CreateStringVariable;
  psi.LockPlugin=LockPlugin;
  int ret=SetStartupInfo(&psi);
  return ret!=0;
}
void TPlugin::UpdateInfo()
{
  memset(&info,0,sizeof(info));
  info.StructSize=sizeof(info);
  GetPluginInfo(&info);
}
TPlugins::~TPlugins()
{
  ClearPlugins();
}
void ProcessFindDirectory(AnsiString subpath,TStringList *list)
{
static AnsiString DllExt=".dll";
  TSearchRec sr;
  if (FindFirst(PluginsPath+subpath+"\\*.*", 0 , sr) == 0)
  {
    do
    {
      AnsiString ext=ExtractFileExt(sr.Name);
      if(!ext.AnsiCompareIC(DllExt))
        list->Add(subpath+"\\"+sr.Name);
    }while(FindNext(sr)==0);
    FindClose(sr);
  }
}
void InitPlugins()
{
  if(ACVersionStr.Length()){
    char *s=ACVersionStr.c_str();
    int major=atoi(GetNextToken(s).c_str());
    if(major>=(1<<7))
      ShowMessage("VERSION:Major");
    if(*s=='.') s++;
    int minor=atoi(GetNextToken(s).c_str());
    if(minor>=(1<<6))
      ShowMessage("VERSION:Minor");
    if(*s=='.') s++;
    int release=atoi(GetNextToken(s).c_str());
    if(release>=(1<<6))
      ShowMessage("VERSION:Release");
    if(*s=='.') s++;
    int build=atoi(GetNextToken(s).c_str());
    if(build>=(1<<13))
      ShowMessage("VERSION:Build");
    ACVersion=MAKEACVERSION(major,minor,release,build);
  }
  PluginsPath=ClientPath+PluginsPath;
  Plugins.InitPlugins();
  if(!Plugins.size())
    return;
  IndexVars[PVT_TURN].AddVariable(-1,PVT_INTEGER);
  IndexVars[PVT_TURN].AddVariable(-2,PVT_INTEGER);
  IndexVars[PVT_TURN].AddVariable(-3,PVT_STRING);
  IndexVars[PVT_TURN].AddVariable(-4,PVT_INTEGER);
  IndexVars[PVT_TURN].AddVariable(2,PVT_INTEGER);
  IndexVars[PVT_TURN].AddVariable(3,PVT_INTEGER);
  IndexVars[PVT_TURN].AddVariable(4,PVT_INTEGER);
  IndexVars[PVT_REGION].AddVariable(-1,PVT_INTEGER);
  IndexVars[PVT_REGION].AddVariable(-2,PVT_INTEGER);
  IndexVars[PVT_OBJECT].AddVariable(-1,PVT_INTEGER);
  IndexVars[PVT_OBJECT].AddVariable(-2,PVT_INTEGER);
  IndexVars[PVT_UNIT].AddVariable(-1,PVT_INTEGER);
  IndexVars[PVT_UNIT].AddVariable(-3,PVT_STRING);
  IndexVars[PVT_UNIT].AddVariable(-4,PVT_INTEGER);
  IndexVars[PVT_ITEM].AddVariable(-1,PVT_INTEGER);
  IndexVars[PVT_SKILL].AddVariable(-1,PVT_INTEGER);
  IndexVars[PVT_FACTION].AddVariable(-1,PVT_INTEGER);
  IndexVars[PVT_FACTION].AddVariable(-2,PVT_INTEGER);
  IndexVars[PVT_GAME].AddVariable(-1,PVT_INTEGER);
  IndexVars[PVT_GAME].AddVariable(-3,PVT_STRING);
  IndexVars[PVT_GAME].AddVariable(-4,PVT_INTEGER);
  IndexVars[PVT_LIST].AddVariable(-1,PVT_INTEGER);
}
void TPlugins::InitPlugins()
{
  auto_ptr<TStringList> list(new TStringList);
  TSearchRec sr;
  if (FindFirst(PluginsPath+"*.*", faDirectory , sr) == 0)
  {
    do
    {
      if(*sr.Name.c_str()=='.') continue;
      if((sr.Attr & faDirectory))
      {
        if(*sr.Name.c_str()=='-')
          continue;
        ProcessFindDirectory(sr.Name,list.get());
      }
    }while(FindNext(sr)==0);
    FindClose(sr);
  }
  list->Sort();
  int num=0;
  while(list->Count)
  {
    AnsiString name=list->Strings[0];
    AnsiString fname=PluginsPath+name;
    HINSTANCE hLib=LoadLibrary(fname.c_str());
    if(hLib)
    {
      TPlugin *plg=new TPlugin;
      plg->ModuleName=fname;
      plg->ShortModuleName=ChangeFileExt(fname.Delete(1,PluginsPath.Length()),"");
      plg->ModuleNumber=num;
      plg->hLib=hLib;
      plg->SetStartupInfo=(_SetStartupInfo)GetProcAddress(hLib,"SetStartupInfo");
      plg->GetPluginInfo=(_GetPluginInfo)GetProcAddress(hLib,"GetPluginInfo");
      plg->OpenPlugin=(_OpenPlugin)GetProcAddress(hLib,"OpenPlugin");
      plg->ProcessEvent=(_ProcessEvent)GetProcAddress(hLib,"ProcessEvent");
      if(!plg->Init())
      {
        delete plg;
      }else
      {
        Plugins.push_back(plg);
        num++;
      }
    }
    list->Delete(0);
  }
}
void TPlugins::ClearPlugins()
{
  for(TPlugin **i=begin().operator ->(),**endi=end().operator ->();i<endi;i++)
  {
    delete *i;
    *i=0;
  }
  clear();
  specevents.clear();
}
int WINAPI CopyVariable(PluginVariable *to, const PluginVariable *from)
{
  if(to->Type!=PVT_NONE)
    FreeVariable(to);
  unsigned type=from->Type;
  if(type>=VarFuncsCount)
    return ERROR_BADVARIABLE;
  if(type>PVT_STRING)
  {
    if(!VarFuncs[type].CopyVariable)
      return ERROR_BADVARIABLE;
    return VarFuncs[type].CopyVariable(to,from);
  }
  if(type==PVT_STRING)
  {
    int len=strlen(from->str)+1;
    to->str=new char[len];
    strcpy(to->str,from->str);
    to->Type=PVT_STRING;
    return 0;
  }
  to->value=from->value;
  return 0;
}
int WINAPI FreeVariable(PluginVariable *var)
{
  unsigned type=var->Type;
  if(type>=VarFuncsCount)
    return ERROR_BADVARIABLE;
  if(type>PVT_STRING)
  {
    if(!VarFuncs[type].FreeVariable)
      return ERROR_BADVARIABLE;
    return VarFuncs[type].FreeVariable(var);
  }
  if(type==PVT_STRING)
    delete var->str;
  var->Type=PVT_NONE;
  return 0;
}
int WINAPI GetVariableProperty(PluginVariable *var, int index, PluginVariable *result)
{
  if(PluginStatus==plsNoBase) return ERROR_PLUGINNOTLOCKED;
  if(result->Type!=PVT_NONE)
    FreeVariable(result);
  unsigned type=var->Type;
  if(type>=VarFuncsCount)
    return ERROR_BADVARIABLE;
  if(type<=PVT_STRING)
    return ERROR_BADVARIABLE;
  if(!VarFuncs[type].GetVariableProperty)
    return ERROR_BADPROPERTYINDEX;
  return VarFuncs[type].GetVariableProperty(var,index,result);
}
int WINAPI SetVariableProperty(PluginVariable *var, int index, PluginVariable *value)
{
  if(PluginStatus==plsNoBase) return ERROR_PLUGINNOTLOCKED;
  unsigned type=var->Type;
  if(type>=VarFuncsCount)
    return ERROR_BADVARIABLE;
  if(type<=PVT_STRING)
    return ERROR_BADVARIABLE;
  if(!VarFuncs[type].SetVariableProperty)
    return ERROR_BADPROPERTYINDEX;
  return VarFuncs[type].SetVariableProperty(var,index,value);
}
int WINAPI RunVariableFunction(PluginVariable *var, int index, PluginVariable *arg)
{
  if(PluginStatus==plsNoBase) return ERROR_PLUGINNOTLOCKED;
  unsigned type=var->Type;
  if(type>=VarFuncsCount)
    return ERROR_BADVARIABLE;
  if(type<=PVT_STRING)
    return ERROR_BADVARIABLE;
  if(!VarFuncs[type].RunVariableFunction)
    return ERROR_BADPROPERTYINDEX;
  return VarFuncs[type].RunVariableFunction(var,index,arg);
}
int WINAPI GetGlobalVariable(int index, PluginVariable * result)
{
  if(PluginStatus==plsNoBase) return ERROR_PLUGINNOTLOCKED;
  if(result->Type!=PVT_NONE)
    FreeVariable(result);
  switch(index)
  {
    case 0:
      if(!PluginRegion)
        return ERROR_BADPROPERTYINDEX;
      result->Type=PVT_REGION;
      result->reg=PluginRegion;
    return 0;
    case 1:
      if(!PluginUnit)
        return ERROR_BADPROPERTYINDEX;
      result->Type=PVT_UNIT;
      result->unit=PluginUnit;
    return 0;
    case 2:
      result->Type=PVT_TURN;
      result->turn=curTurn;
    return 0;
    case 3:
      if(!curRegion)
        return ERROR_BADPROPERTYINDEX;
      result->Type=PVT_REGION;
      result->reg=curRegion;
    return 0;
    case 4:
      if(!curUnit)
        return ERROR_BADPROPERTYINDEX;
      result->Type=PVT_UNIT;
      result->unit=curUnit;
    return 0;
    case 5:
    {
      if(!ExprArg)
        return 0;
      if(ExprArg->type==dtInt)
        return CreateIntVariable(((EDataInt*)ExprArg)->value,result);
      else if(ExprArg->type==dtString)
        return CreateStringVariable(((EDataString*)ExprArg)->value.c_str(),result);
      return ERROR_BADARGUMENTTYPE;
    }
    case 6:
    {
      AOrderCustom *ord=new AOrderCustom;
      result->Type=PVT_ORDER;
      result->order=ord;
      return 0;
    }
    case 7:
      result->Type=PVT_GAME;
    return 0;
    case 8:
      return CreateIntVariable((int)AtlaForm->Handle,result);
    case 9:
    {
    if(!curRegion) return 0;
    TTreeNode *curNode=AtlaForm->TreeView->Selected;
    if(!curNode) return 0;
    VTreeNode *vnode=(VTreeNode*)curNode->Data;
    switch(vnode->type){
      case tntFaction:
        result->faction=curFactions->GetFaction(vnode->value);
        if(!result->faction) return 0;
        result->Type=PVT_FACTION;
      return 0;
      case tntRegion:
        result->reg=curRegion;
        result->Type=PVT_REGION;
      return 0;
      case tntObject:
        result->obj=curRegion->GetObject(vnode->value);
        if(!result->obj) return 0;
        result->objtype=ObjectTypes->Get(result->obj->type);
        if(!result->objtype) return 0;
        result->Type=PVT_OBJECT;
      return 0;
      case tntUnit:
        result->unit=(AUnit*)vnode->data;
        if(!result->unit) return 0;
        result->Type=PVT_UNIT;
      return 0;
    }
    return 0;
    }
    case 10:
      return CreateIntVariable((int)Application->Handle,result);
  }
  return ERROR_BADVARIABLE;
}
int WINAPI AddEvent(const char *str)
{
  if(!EventsForm)
    return 1;
  EventsForm->AddEvent(str);
  return 0;
}
int WINAPI CreateStringVariable(const char *s, PluginVariable * result)
{
  if(result->Type!=PVT_NONE)
    FreeVariable(result);
  static char *nullbuf="";
  if(!s) s=nullbuf;  
  int len=strlen(s)+1;
  result->str=new char[len];
  strcpy(result->str,s);
  result->Type=PVT_STRING;
  return 0;
}
int WINAPI LockPlugin(int ModuleNumber, int locktype, int reserved)
{
  if(locktype)
  {
    if(PluginStatus!=plsNoBase)
      return ERROR_PLUGINNOTLOCKED;
    PluginStatus=plsNormal;
    PluginUnit=0;
    PluginRegion=0;
  }else
  {
    if(PluginStatus!=plsNormal)
      return ERROR_PLUGINNOTLOCKED;
    PluginStatus=plsNoBase;
  }
  return 0;
}
void TPlugins::UpdatePluginInfo(int num)
{
  specevents.clear();
  for(TPlugin **i=begin().operator ->(),**endi=end().operator ->();i<endi;i++)
  {
    (*i)->UpdateInfo();
    PluginInfo &pi=(*i)->info;
    for(int j=0;j<pi.SpecEventsNumber;j++)
    {
      int v=pi.SpecEvents[j];
      if(v<PE_CALCSTUDYCOST) continue;
      map<int,TPlugin*>::iterator ptr;
      ptr=specevents.find(v);
      if(ptr!=specevents.end()) continue;
      specevents[v]=*i;
    }
  }
}
void TPlugins::ExitAtlaClient(int from)
{
  PluginStatus=from?plsEvent:plsNoBase;
  PluginRegion=0;
  PluginUnit=0;
  int event=from?PE_EXITCLIENT_BASE:PE_EXITCLIENT_NOBASE;
  ProcessEvent(event,0);
}
void TPlugins::ProcessEvent(int type, void *param)
{
  for(TPlugin **i=begin().operator ->(),**endi=end().operator ->();i<endi;i++)
  {
    if((*i)->ProcessEvent)
      (*i)->ProcessEvent(type,param);
  }
  PluginStatus=plsNoBase;
}
int TPlugins::ProcessSpecEvent(int type, void *param)
{
  map<int,TPlugin*>::iterator ptr;
  ptr=specevents.find(type);
  if(ptr==specevents.end()) return ERROR_BADPROPERTYINDEX;
  TPlugin *i=ptr->second;
  int val=ERROR_BADPROPERTYINDEX;
  PluginStatus=plsEvent;
  if(i->ProcessEvent)
    val=i->ProcessEvent(type,param);
  PluginStatus=plsNoBase;
  return val;
}
bool TPlugins::FindOrderPlugin(int OrderType,const char *str, int &ModuleNumber, int &OrderNumber)
{
  ModuleNumber=-1;
  for(TPlugin **i=begin().operator ->(),**endi=end().operator ->();i<endi;i++)
  {
    PluginInfo &info=(*i)->info;
    for(int ind=0;ind<info.PluginOrderStringsNumber;ind++)
    {
      char *s=info.PluginOrderStrings[ind];
      if(s==NULL||*s==0) continue;
      if((info.PluginOrderFlags[ind]&PO_TYPEMASK)!=OrderType) continue;
      int l=strlen(s);
      if(strncmpi(s,str,l)!=0) continue;
      ModuleNumber=(*i)->ModuleNumber;
      OrderNumber=ind;
      return true;
    }
  }
  return false;
}
int TPlugins::ProcessOrderEvent(int OrderType,int ModuleNumber, int type, void*param, AUnit *un)
{
  if((unsigned)ModuleNumber>=Plugins.size())
    return ERROR_BADINDEXVALUE;
  TPlugin *i=at(ModuleNumber);
  if(i->ModuleNumber!=ModuleNumber)
    return ERROR_BADINDEXVALUE;
  int OrderNumber=((EvPluginOrder*)param)->ordernum;
  if((i->info.PluginOrderFlags[OrderNumber]&PO_TYPEMASK)!=OrderType)
    return ERROR_BADINDEXVALUE;
  int val=ERROR_BADPROPERTYINDEX;
  if(type==PE_ORDERRUN)
    PluginStatus=OrderType==PO_NORMAL?plsRunOrder:plsNormal;
  else
    PluginStatus=plsEvent;
  PluginUnit=un;
  PluginRegion=curRegion;
  if(i->ProcessEvent)
    val=i->ProcessEvent(type,param);
  PluginStatus=plsNoBase;
  return val;
}
int TPlugins::GetOrderPluginFlag(int ModuleNumber, int OrderNumber)
{
  if((unsigned)ModuleNumber>=Plugins.size())
    return 0;
  TPlugin *i=at(ModuleNumber);
  if(i->ModuleNumber!=ModuleNumber)
    return 0;
  return i->info.PluginOrderFlags[OrderNumber];
}
int TPlugins::CalcMoveCost(AUnit *un,ARegion *to, int dir, int movetype, int turnnum)
{
  if(turnnum<0)
    turnnum=to->turn->num+1;
  const type=PE_CALCMOVECOST;
  map<int,TPlugin*>::iterator ptr;
  ptr=specevents.find(type);
  if(ptr==specevents.end()) return ERROR_BADPROPERTYINDEX;
  TPlugin *i=ptr->second;
  int val=ERROR_BADPROPERTYINDEX;
  PluginUnit=un;
  PluginRegion=to;
  PluginStatus=plsEvent;
  CalcMoveCostParams params;
  params.dir=dir;
  params.turnnum=turnnum;
  params.movetype=movetype;
  if(i->ProcessEvent)
    val=i->ProcessEvent(type,&params);
  PluginStatus=plsNoBase;
  return val;
}
int TPlugins::CalcMovePoints(AUnit *un, int movetype)
{
  const type=PE_CALCMOVEPOINTS;
  map<int,TPlugin*>::iterator ptr;
  ptr=specevents.find(type);
  if(ptr==specevents.end()) return ERROR_BADPROPERTYINDEX;
  TPlugin *i=ptr->second;
  int val=ERROR_BADPROPERTYINDEX;
  PluginUnit=un;
  PluginRegion=un->baseobj->basereg;
  PluginStatus=plsEvent;
  if(i->ProcessEvent)
    val=i->ProcessEvent(type,&movetype);
  PluginStatus=plsNoBase;
  return val;
}
void TPlugins::OnGameLoaded()
{
  OldTurnNum=-1;
  OldLevel=-1;
  OldRegion=0;
  OldTreeNodeType=PVT_NONE;
  OldTreeNodeData=0;
  PluginStatus=plsEvent;
  PluginRegion=curRegion;
  PluginUnit=curUnit;
  ProcessEvent(PE_LOADGAME,0);
}
void TPlugins::OnGameSaved()
{
  PluginStatus=plsEvent;
  PluginRegion=curRegion;
  PluginUnit=curUnit;
  ProcessEvent(PE_SAVEGAME,0);
}
void TPlugins::OnTurnChanged(bool uncond)
{
  if(!curTurn) return;
  if(!uncond&&OldTurnNum==curTurn->num) return;
  OldTurnNum=curTurn->num;
  PluginStatus=plsEvent;
  PluginRegion=curRegion;
  PluginUnit=0;
  ProcessEvent(PE_TURNCHANGED,0);
}
void TPlugins::OnLevelChanged()
{
  if(!curRegion) return;
  if(OldLevel==curRegion->zloc) return;
  OldLevel=curRegion->zloc;
  PluginStatus=plsEvent;
  PluginRegion=curRegion;
  PluginUnit=0;
  ProcessEvent(PE_LEVELCHANGED,0);
}
void TPlugins::OnRegionChanged()
{
  if(!curRegion) return;
  if(OldRegion==curRegion) return;
  OldRegion=curRegion;
  PluginStatus=plsEvent;
  PluginRegion=curRegion;
  PluginUnit=0;
  ProcessEvent(PE_REGIONCHANGED,0);
}
void TPlugins::OnTreeItemChanged()
{
  if(!curRegion) return;
  TTreeNode *curNode=AtlaForm->TreeView->Selected;
  if(!curNode) return;
  VTreeNode *vnode=(VTreeNode*)curNode->Data;
  PluginUnit=0;
  PluginVariableType tnType=PVT_NONE;
  void *tnData=0;
  switch(vnode->type){
    case tntFaction:
      tnData=curFactions->GetFaction(vnode->value);
      if(!tnData)break;
      tnType=PVT_FACTION;
    break;
    case tntRegion:
      tnData=curRegion;
      tnType=PVT_REGION;
    break;
    case tntObject:
      tnData=curRegion->GetObject(vnode->value);
      if(!tnData)break;
      tnType=PVT_OBJECT;
    break;
    case tntUnit:
      PluginUnit=(AUnit*)vnode->data;
      tnData=PluginUnit;
      if(!tnData)break;
      tnType=PVT_UNIT;
    break;
  }
  if(tnType==OldTreeNodeType&&tnData==OldTreeNodeData) return;
  OldTreeNodeType=tnType;
  OldTreeNodeData=tnData;
  PluginStatus=plsEvent;
  PluginRegion=curRegion;
  ProcessEvent(PE_TREEITEMCHANGED,0);
}
void TPlugins::OnSaveOrder(int facnum,bool before)
{
  PluginStatus=plsEvent;
  PluginRegion=0;
  PluginUnit=0;
  ProcessEvent(before?PE_BEFORESAVEORDER:PE_AFTERSAVEORDER,(void*)facnum);
}
void TPlugins::CreateMenu(TMenuItem * first,TNotifyEvent OnClick)
{
  const AnsiString prefix="PluginsMenu";
  const int sortorder=100;
  ShortCuts.DeleteOwnersWithPrefix(prefix);
  ShortCuts.AddOwnerWithCodeRoot(prefix,"Plugins menu",sortorder);

  TMenuItem *root=first->Parent;
  int ind=first->MenuIndex+1;
  while(root->Count>ind)
  {
    root->Delete(ind);
  }
  int index=0;
  for(TPlugin **i=begin().operator ->(),**endi=end().operator ->();i<endi;i++)
  {
    TMenuItem *parent=root,*last=0;
    int oldlevel=0;
    PluginInfo &info=(*i)->info;
    AnsiString PluginName=(*i)->ShortModuleName+":";
    for(int i=0;i<info.PluginMenuStringsNumber;i++)
    {
      if(!info.PluginMenuStrings[i]){
        index++;
        continue;
      }
      int level=0;
      if(info.PluginMenuLevels)
        level=info.PluginMenuLevels[i];
      if(level==oldlevel+1)
      {
        if(last){
          parent=last;
          oldlevel++;
        }
      }else if(level<oldlevel)
      {
        while(level<oldlevel&&parent->Parent)
        {
          parent=parent->Parent;
          oldlevel--;
        }
      }
      TMenuItem *mi=new TMenuItem(parent);
      mi->Caption=info.PluginMenuStrings[i];
      mi->Tag=index++;
      mi->OnClick=OnClick;
      parent->Add(mi);
      last=mi;
      AnsiString code=PluginName+mi->Caption;
      ShortCuts.AddOwnerWithCode(mi,code,prefix,level+1,sortorder);
    }
  }
  if(!index)
  {
    TMenuItem *mi=new TMenuItem(root);
    mi->Caption="no plugins";
    mi->Enabled=false;
    root->Add(mi);
  }
}
void TPlugins::CreateConfigMenu(TMenuItem * root,TNotifyEvent OnClick)
{
  const AnsiString prefix="PluginsConfig";
  const int sortorder=110;
  ShortCuts.DeleteOwnersWithPrefix(prefix);
  ShortCuts.AddOwnerWithCodeRoot(prefix,"Plugins Config",sortorder);

  root->Clear();
  int index=0;
  for(TPlugin **i=begin().operator ->(),**endi=end().operator ->();i<endi;i++)
  {
    PluginInfo &info=(*i)->info;
    AnsiString PluginName=(*i)->ShortModuleName+":";
    for(int i=0;i<info.PluginConfigStringsNumber;i++)
    {
      if(!info.PluginConfigStrings[i]){
        index++;
        continue;
      }
      TMenuItem *mi=new TMenuItem(root);
      mi->Caption=info.PluginConfigStrings[i];
      mi->Tag=index++;
      mi->OnClick=OnClick;
      root->Add(mi);
      AnsiString code=PluginName+mi->Caption;
      ShortCuts.AddOwnerWithCode(mi,code,prefix,1,sortorder);
    }
  }
  if(!index)
  {
    TMenuItem *mi=new TMenuItem(root);
    mi->Caption="no configs";
    mi->Enabled=false;
    root->Add(mi);
  }
}
void TPlugins::CreateFunctionMenu(TMenuItem * root,int exprmode,TNotifyEvent OnClick)
{
  bool first=true;
  for(TPlugin **i=begin().operator ->(),**endi=end().operator ->();i<endi;i++)
  {
    PluginInfo &info=(*i)->info;
    for(int i=0;i<info.ExprFuncsStringsNumber;i++)
    {
      if(!info.ExprFuncsStrings[i]) continue;
      if((info.ExprFuncsFlags[i]&exprmode)==0) continue;
      TMenuItem *mi=new TMenuItem(root);
      AnsiString caption=info.ExprFuncsStrings[i];
      caption+="()";
      mi->Caption=caption;
      mi->OnClick=OnClick;
      if(first)
      {
        first=false;
        mi->Break=mbBarBreak;
      }
      root->Add(mi);
    }
  }
}
void TPlugins::CreateOrderMenu(TMenuItem * root,int ordertype,TNotifyEvent OnClick)
{
  AnsiString prefix="PluginsOrders";
  int sortorder=120;
  AnsiString RootName="Plugins Orders";
  switch(ordertype)
  {
    case PO_AUTO:
      prefix="PluginsAutoOrders";
      sortorder=130;
      RootName="Plugins Auto Orders";
    break;
    case PO_CLIENT:
      prefix="PluginsClientOrders";
      sortorder=140;
      RootName="Plugins Client Orders";
    break;
  }
  ShortCuts.DeleteOwnersWithPrefix(prefix);
  ShortCuts.AddOwnerWithCodeRoot(prefix,RootName,sortorder);

  for(int i=root->Count-1;i>=0;i--)
  {
    TMenuItem *mi=root->Items[i];
    if(mi->Tag<100000)
      break;
    root->Delete(i);
  }
  int num=0;
  bool first=true;
  for(TPlugin **i=begin().operator ->(),**endi=end().operator ->();i<endi;i++)
  {
    PluginInfo &info=(*i)->info;
    AnsiString PluginName=(*i)->ShortModuleName+":";
    for(int i=0;i<info.PluginOrderStringsNumber;i++)
    {
      char *s=info.PluginOrderStrings[i];
      if(s==NULL||*s==0) continue;
      if((info.PluginOrderFlags[i]&PO_TYPEMASK)!=ordertype) continue;
      TMenuItem *mi=new TMenuItem(root);
      mi->Caption=s;
      mi->OnClick=OnClick;
      mi->Tag=100000*ordertype+num*100+i;
      if(first)
      {
        mi->Break=mbBarBreak;
        first=false;
      }
      root->Add(mi);
      AnsiString code=PluginName+mi->Caption;
      ShortCuts.AddOwnerWithCode(mi,code,prefix,1,sortorder);
    }
    num++;
  }
}
void TPlugins::RunMenu(int index)
{
  PluginStatus=plsNormal;
  PluginRegion=curRegion;
  PluginUnit=curUnit;
  PluginNeedMakeTree=false;
  for(TPlugin **i=begin().operator ->(),**endi=end().operator ->();i<endi;i++)
  {
    PluginInfo &info=(*i)->info;
    if(index>=info.PluginMenuStringsNumber)
    {
      index-=info.PluginMenuStringsNumber;
      continue;
    }
    if((*i)->OpenPlugin)
      (*i)->OpenPlugin(OPEN_MENU,index);
    break;
  }
  PluginStatus=plsNoBase;
}
void TPlugins::RunConfigure(int index)
{
  PluginStatus=plsNormal;
  PluginRegion=curRegion;
  PluginUnit=curUnit;
  PluginNeedMakeTree=false;
  for(TPlugin **i=begin().operator ->(),**endi=end().operator ->();i<endi;i++)
  {
    PluginInfo &info=(*i)->info;
    if(index>=info.PluginConfigStringsNumber)
    {
      index-=info.PluginConfigStringsNumber;
      continue;
    }
    if((*i)->OpenPlugin)
      (*i)->OpenPlugin(OPEN_CONFIGURE,index);
    break;
  }
  PluginStatus=plsNoBase;
}
EData *ExprLast(char *&buf);
EData * TPlugins::RunExpression(AnsiString name, int exprmode, char *&buf)
{
  PluginNeedMakeTree=false;
  TPlugin *plg=0;
  int index=-1;
  for(TPlugin **i=begin().operator ->(),**endi=end().operator ->();i<endi&&!plg;i++)
  {
    PluginInfo &info=(*i)->info;
    for(int j=0;j<info.ExprFuncsStringsNumber;j++)
    {
      if(strcmpi(info.ExprFuncsStrings[j],name.c_str()))
        continue;
      if((info.ExprFuncsFlags[j]&exprmode)==0)
        continue;
      plg=*i;
      index=j;
      break;
    }
  }
  if(!plg) return 0;
  ExprArg=0;
  if(*buf!=')')
  {
    ExprArg=ExprLast(buf);
    if(ExprArg->type!=dtInt&&ExprArg->type!=dtString)
      throw Exception("Bad argument type for "+name);
  }
  PluginStatus=plsExpression;
  if(plg->OpenPlugin)
    plg->OpenPlugin(OPEN_EXPRESSION,index);
  PluginStatus=plsNoBase;
  delete ExprArg;ExprArg=0;
  EData *e=ExprRetVal;
  ExprRetVal=0;
  return e;
}
void TPlugins::RunClientOrder(AnsiString str)
{
  int mn,on;
  if(!Plugins.FindOrderPlugin(PO_CLIENT,str.c_str(),mn,on))
     return;
  PluginStatus=plsNormal;
  PluginRegion=curRegion;
  PluginUnit=curUnit;
  PluginNeedMakeTree=false;
  TPlugin *i=at(mn);
  if(i->OpenPlugin)
    i->OpenPlugin(OPEN_MENU,1000000+on);
  PluginStatus=plsNoBase;
}

int WINAPI NullCopyVar(PluginVariable *to, const PluginVariable *from)
{
  memcpy(to,from,sizeof(PluginVariable));
  return 0;
}
int WINAPI NullFreeVar(PluginVariable *var)
{
  var->Type=PVT_NONE;
  return 0;
}
int TIndexVars::FindIndexIndex(int index)
{
  for(TVar *i=vars.begin().operator ->(),*endi=vars.end().operator ->();i<endi;i++)
  {
    if(i->index!=index)
      continue;
    return i-vars.begin().operator ->();  
  }
  return -1;
}
void TIndexVars::AddVariable(int index,PluginVariableType type)
{
  if(type!=PVT_STRING&&type!=PVT_INTEGER)
    throw 1;
  if(FindIndexIndex(index)!=-1)
    throw 2;
  TVar var(index,type);
  vars.push_back(var);
}
int TIndexVars::GetLastSetIndex()
{
  if(lastsetindex<0) return -1;
  return vars[lastsetindex].index;
}
bool TIndexVars::IsVarSet(int index)
{
  index=FindIndexIndex(index);
  if(index<0)
    return false;
  return vars[index].set;
}
bool TIndexVars::SetVar(int index,PluginVariable *value)
{
  index=FindIndexIndex(index);
  if(index<0)
    return false;
  TVar &var=vars[index];
  if(value->Type!=var.type)
    return false;
  if(var.type==PVT_STRING)
    var.strvalue=value->str;
  else
    var.intvalue=value->value;
  var.set=true;
  lastsetindex=index;
  return true;
}
bool TIndexVars::GetVar(int index,PluginVariable *result)
{
  index=FindIndexIndex(index);
  if(index<0)
    return false;
  TVar &var=vars[index];
  if(!var.set)
    return false;
  if(var.type==PVT_STRING)
  {
    result->str=new char[var.strvalue.Length()+1];
    strcpy(result->str,var.strvalue.c_str());
  }else
    result->value=var.intvalue;
  result->Type=var.type;
  return true;
}
int TIndexVars::GetVarInt(int index)
{
  if(index<0)
    index=lastsetindex;
  else
    index=FindIndexIndex(index);
  if(index<0)
    return 0;
  TVar &var=vars[index];
  if(!var.set)
    return 0;
  if(var.type==PVT_INTEGER)
    return var.intvalue;
  return 0;
}
AnsiString TIndexVars::GetVarString(int index)
{
  AnsiString str;
  if(index<0)
    index=lastsetindex;
  else
    index=FindIndexIndex(index);
  if(index<0)
    return str;
  TVar &var=vars[index];
  if(!var.set)
    return str;
  if(var.type==PVT_STRING)
    return var.strvalue;
  return str;
}
int WINAPI TurnGetVarProp(PluginVariable *var, int index, PluginVariable *result)
{
  ATurn *turn=var->turn;
  switch(index)
  {
    case 0:
      return CreateIntVariable(turn->num,result);
    case 1:
    {
      int lastindex=IndexVars[PVT_TURN].GetLastSetIndex();
      switch(lastindex)
      {
        case 2:case 3:case 4:
        {
          if(!IndexVars[PVT_TURN].IsVarSet(2)
            ||!IndexVars[PVT_TURN].IsVarSet(3)
            ||!IndexVars[PVT_TURN].IsVarSet(4))
            return ERROR_INDEXNOTSET;
          int x=IndexVars[PVT_TURN].GetVarInt(2);
          int y=IndexVars[PVT_TURN].GetVarInt(3);
          int z=IndexVars[PVT_TURN].GetVarInt(4);
          ARegion *reg=turn->RegionList->Get(x,y,z);
          if(!reg)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_REGION;
          result->reg=reg;
          return 0;
        }
        case -1:
        {
          int value=IndexVars[PVT_TURN].GetVarInt();
          if(value<0)
            return CreateIntVariable(turn->RegionList->count,result);
          if(value>=turn->RegionList->count)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_REGION;
          result->reg=turn->RegionList->Get(value);
          return 0;
        }
        case -3:
        {
          AnsiString str=IndexVars[PVT_TURN].GetVarString();
          str+=")";
          if(str[1]!='(') str.Insert("(",1);
          char *s=str.c_str();
          ALocation *loc=GetNextLocation(s,false);
          if(!loc)
            return ERROR_BADINDEXVALUE;
          ARegion *reg=turn->RegionList->Get(*loc);
          delete loc;
          if(!reg)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_REGION;
          result->reg=reg;
          return 0;
        }
        case -4:
        {
          int value=IndexVars[PVT_TURN].GetVarInt();
          ARegion *reg=turn->RegionList->GetByKey(value);
          if(!reg)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_REGION;
          result->reg=reg;
          return 0;
        }
        default: return ERROR_INDEXNOTSET;
      }
    }
    case 5:
    {
      if(IndexVars[PVT_TURN].GetLastSetIndex()!=4)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_TURN].GetVarInt();
      if(value<0)
        return CreateIntVariable(turn->RegionList->RegionArrays->count,result);
      if(value>=turn->RegionList->RegionArrays->count)
        return ERROR_BADINDEXVALUE;
      return CreateIntVariable(turn->RegionList->RegionArrays->Get(value)->x,result);
    }
    case 6:
    {
      if(IndexVars[PVT_TURN].GetLastSetIndex()!=4)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_TURN].GetVarInt();
      if(value<0)
        return CreateIntVariable(turn->RegionList->RegionArrays->count,result);
      if(value>=turn->RegionList->RegionArrays->count)
        return ERROR_BADINDEXVALUE;
      return CreateIntVariable(turn->RegionList->RegionArrays->Get(value)->y,result);
    }
    case 7:
    {
      if(IndexVars[PVT_TURN].GetLastSetIndex()!=4)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_TURN].GetVarInt();
      if(value<0)
        return CreateIntVariable(turn->RegionList->RegionArrays->count,result);
      if(value>=turn->RegionList->RegionArrays->count)
        return ERROR_BADINDEXVALUE;
      return CreateStringVariable(turn->RegionList->RegionArrays->Get(value)->name.c_str(),result);
    }
    case 8:
    {
      int lastindex=IndexVars[PVT_TURN].GetLastSetIndex();
      switch(lastindex)
      {
        case -1:
        {
          int value=IndexVars[PVT_TURN].GetVarInt();
          if(value<0)
            return CreateIntVariable(turn->factions->count,result);
          if(value>=turn->factions->count)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_FACTION;
          result->faction=turn->factions->Get(value);
          return 0;
        }
        case -2:
        {
          int value=IndexVars[PVT_TURN].GetVarInt();
          AFaction *fac=turn->factions->GetFaction(value);
          if(!fac)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_FACTION;
          result->faction=fac;
          return 0;
        }
        default: return ERROR_INDEXNOTSET;
      }
    }
    case 10:
    {
      int lastindex=IndexVars[PVT_TURN].GetLastSetIndex();
      switch(lastindex)
      {
        case -2:
        {
          int value=IndexVars[PVT_TURN].GetVarInt();
          AUnit *un=turn->RegionList->GetUnit(value);
          if(!un)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_UNIT;
          result->unit=un;
          return 0;
        }
        default: return ERROR_INDEXNOTSET;
      }
    }
    case 11:
    {
      if(!ListCreate(result,PVT_REGION))
        return ERROR_BADVARIABLE;
      foreach(turn->RegionList)
      {
        ARegion *reg=*(ARegion**)iter;
        if(reg->gate==0) continue;
        ListAddReg(result,reg);
      }
      return 0;
    }
    case 12:
    {
      if(!ListCreate(result,PVT_UNIT))
        return ERROR_BADVARIABLE;
      foreach(turn->RegionList)
      {
        ARegion *reg=*(ARegion**)iter;
        foreach(reg)
        {
          AObject *obj=*(AObject**)iter;
          foreach(obj->units)
          {
            AUnit *un=*(AUnit**)iter;
            ListAddUnit(result,un);
          }
        }
      }
      return 0;
    }
    case 13:
    {
      if(!ListCreate(result,PVT_UNIT))
        return ERROR_BADVARIABLE;
      foreach(turn->RegionList)
      {
        ARegion *reg=*(ARegion**)iter;
        foreach(reg)
        {
          AObject *obj=*(AObject**)iter;
          foreach(obj->units)
          {
            AUnit *un=*(AUnit**)iter;
            if(!un->IsLocal()) continue;
            ListAddUnit(result,un);
          }
        }
      }
      return 0;
    }
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI TurnSetVarProp(PluginVariable *var, int index, PluginVariable *value)
{
  switch(index)
  {
    case 2:case 3:case 4:case -1:case -2:case -3:case -4:
      if(!IndexVars[PVT_TURN].SetVar(index,value))
        return ERROR_BADARGUMENTTYPE;
      return 0;
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI RegionGetVarProp(PluginVariable *var, int index, PluginVariable *result)
{
  ARegion *reg=var->reg;
  switch(index)
  {
    case 0:
      return CreateIntVariable(reg->xloc,result);
    case 1:
      return CreateIntVariable(reg->yloc,result);
    case 2:
      return CreateIntVariable(reg->zloc,result);
    case 3:
    {
      int lastindex=IndexVars[PVT_REGION].GetLastSetIndex();
      switch(lastindex)
      {
        case -1:
        {
          int value=IndexVars[PVT_REGION].GetVarInt();
          if(value<0)
            return CreateIntVariable(reg->count,result);
          if(value>=reg->count)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_OBJECT;
          result->obj=reg->Get(value);
          AObjectType *otype=ObjectTypes->Get(result->obj->type);
          result->objtype=otype;
          return 0;
        }
        case -2:
        {
          int value=IndexVars[PVT_REGION].GetVarInt();
          AObject *obj=reg->GetObject(value);
          if(!obj)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_OBJECT;
          result->obj=obj;
          AObjectType *otype=ObjectTypes->Get(obj->type);
          result->objtype=otype;
          return 0;
        }
        default: return ERROR_INDEXNOTSET;
      }
    }
    case 4:
    {
      ATerrainType *ttype=TerrainTypes->Get(reg->type);
      if(!ttype)
        return ERROR_BADPROPERTYINDEX;
      return CreateIntVariable(ttype->movepoints,result);
    }
    case 5:
    {
      if(IndexVars[PVT_REGION].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_REGION].GetVarInt();
      if(value<0)
        return CreateIntVariable(6,result);
      if(value>=6)
        return ERROR_BADINDEXVALUE;
      if(!reg->HasExitRoad(value))
        return CreateIntVariable(0,result);
      ALocation loc=reg->neighbors[value];
      if(loc.xloc<0)
        return CreateIntVariable(0,result);
      ARegion *reg2=reg->turn->RegionList->Get(loc);
      if(!reg2)
        return CreateIntVariable(0,result);
      if(!reg2->HasConnectingRoad(value))
        return CreateIntVariable(0,result);
      return CreateIntVariable(1,result);
    }
    case 6:
      return CreateStringVariable(reg->type.c_str(),result);
    case 7:
      return CreateStringVariable(reg->name.c_str(),result);
    case 8:
      return CreateStringVariable(reg->towntype.c_str(),result);
    case 9:
      return CreateStringVariable(reg->townname.c_str(),result);
    case 10:
      return CreateStringVariable(reg->race.c_str(),result);
    case 11:
      return CreateIntVariable(reg->population,result);
    case 12:
      return CreateIntVariable(reg->money,result);
    case 13:
      return CreateIntVariable(reg->weather!="Normal",result);
    case 14:
      return CreateIntVariable(reg->nxtweather!="Normal",result);
    case 15:
      return CreateIntVariable(reg->wages,result);
    case 16:
      return CreateIntVariable(reg->maxwages,result);
    case 17:
      return CreateIntVariable(reg->entertainment,result);
    case 18:
    {
      if(IndexVars[PVT_REGION].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_REGION].GetVarInt();
      if(value<0)
        return CreateIntVariable(6,result);
      if(value>=6)
        return ERROR_BADINDEXVALUE;
      ALocation loc=reg->neighbors[value];
      if(loc.xloc<0)
        return 0;
      ARegion *reg2=reg->turn->RegionList->Get(loc);
      if(!reg2)
        return 0;
      result->Type=PVT_REGION;
      result->reg=reg2;
      return 0;
    }
    case 19:
      return CreateIntVariable(reg->gate,result);
    case 20:
    {
      if(IndexVars[PVT_REGION].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_REGION].GetVarInt();
      if(value<0)
        return CreateIntVariable(reg->wanteds->count,result);
      if(value>=reg->wanteds->count)
        return ERROR_BADINDEXVALUE;
      AMarket *mark=reg->wanteds->Get(value);
      result->Type=PVT_ITEM;
      result->itemtype=mark->type;
      result->itemcount=mark->amount;
      result->itemprice=mark->price;
      return 0;
    }
    case 21:
    {
      if(IndexVars[PVT_REGION].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_REGION].GetVarInt();
      if(value<0)
        return CreateIntVariable(reg->saleds->count,result);
      if(value>=reg->saleds->count)
        return ERROR_BADINDEXVALUE;
      AMarket *mark=reg->saleds->Get(value);
      result->Type=PVT_ITEM;
      result->itemtype=mark->type;
      result->itemcount=mark->amount;
      result->itemprice=mark->price;
      return 0;
    }
    case 22:
    {
      if(IndexVars[PVT_REGION].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_REGION].GetVarInt();
      if(value<0)
        return CreateIntVariable(reg->products->count,result);
      if(value>=reg->products->count)
        return ERROR_BADINDEXVALUE;
      AMarket *mark=reg->products->Get(value);
      result->Type=PVT_ITEM;
      result->itemtype=mark->type;
      result->itemcount=mark->amount;
      result->itemprice=0;
      return 0;
    }
    case 23:
      return CreateStringVariable(reg->descr.c_str(),result);
    case 24:
      result->Type=PVT_TURN;
      result->turn=reg->turn;
      return 0;
    case 25:
      return CreateIntVariable(reg->taxers,result);
    case 26:
      return CreateIntVariable(reg->taxincome,result);
    case 27:
      return CreateIntVariable(reg->workers,result);
    case 28:
      return CreateIntVariable(reg->workincome,result);
    case 29:
      return CreateIntVariable(reg->entattempt,result);
    case 30:
      return CreateIntVariable(reg->entincome,result);
    case 31:
      return CreateIntVariable(reg->runned,result);
    case 32:
    {
      int lastindex=IndexVars[PVT_REGION].GetLastSetIndex();
      switch(lastindex)
      {
        case -2:
        {
          int value=IndexVars[PVT_REGION].GetVarInt();
          AUnit *un=reg->GetUnit(value);
          if(!un)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_UNIT;
          result->unit=un;
          return 0;
        }
        default: return ERROR_INDEXNOTSET;
      }
    }
    case 33:
    {
      if(!ListCreate(result,PVT_UNIT))
        return ERROR_BADVARIABLE;
      foreach(reg)
      {
        AObject *obj=*(AObject**)iter;
        foreach(obj->units)
        {
          AUnit *un=*(AUnit**)iter;
          ListAddUnit(result,un);
        }
      }
      return 0;
    }
    case 34:
      return CreateIntVariable(reg->hasInfo,result);
    case 35:
      return CreateIntVariable(reg->turn->RegionList->IndexOf(reg),result);
    case 36:
    {
      if(IndexVars[PVT_REGION].GetLastSetIndex()!=-2)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_REGION].GetVarInt();
      if(value<0)
        return ERROR_BADINDEXVALUE;
      AnsiString weat=WeatherRegions->GetWeather(reg->xloc,reg->yloc,reg->zloc,value);
      return CreateIntVariable(weat!="Normal",result);
    }
    case 37:
      return CreateIntVariable(reg->key,result);
    case 38:
      return CreateIntVariable(TerrainTypes->IsOcean(reg->type),result);
    case 39:
    {
      if(!ListCreate(result,PVT_UNIT))
        return ERROR_BADVARIABLE;
      foreach(reg)
      {
        AObject *obj=*(AObject**)iter;
        foreach(obj->units)
        {
          AUnit *un=*(AUnit**)iter;
          if(!un->IsLocal()) continue;
          ListAddUnit(result,un);
        }
      }
      return 0;
		}
		case 40:
		{
			if(IndexVars[PVT_REGION].GetLastSetIndex()!=-2)
				return ERROR_INDEXNOTSET;
			int value=IndexVars[PVT_REGION].GetVarInt();
			if(value<0)
				return ERROR_BADINDEXVALUE;
      return CreateIntVariable(reg->IsTaxRegion(value),result);
		}
		case 41:
		{
			if(IndexVars[PVT_REGION].GetLastSetIndex()!=-2)
				return ERROR_INDEXNOTSET;
			int value=IndexVars[PVT_REGION].GetVarInt();
			if(value<0)
				return ERROR_BADINDEXVALUE;
      return CreateIntVariable(reg->IsTradeRegion(value),result);
		}
	}
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI RegionSetVarProp(PluginVariable *var, int index, PluginVariable *value)
{
  switch(index)
  {
    case -1:case -2:
      if(!IndexVars[PVT_REGION].SetVar(index,value))
        return ERROR_BADARGUMENTTYPE;
      return 0;
    case 23:
      if(value->Type!=PVT_STRING)
        return ERROR_BADARGUMENTTYPE;
      var->reg->descr=value->str;
      var->reg->turn->Modified=true;
      return 0;
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI RegionRunVarFunc(PluginVariable *var, int index, PluginVariable *arg)
{
  ARegion *reg=var->reg;
  switch(index)
  {
    case 0:
      RunStop rs=rsNoStop;
      int type=PVT_NONE;
      if(arg) type=arg->Type;
      if(type==PVT_INTEGER)
        switch(arg->value)
        {
          case PRS_AFTERGIVE:rs=rsAfterGive;break;
          case PRS_AFTERBUY:rs=rsAfterBuy;break;
          case PRS_BEFOREMONTHLONG:rs=rsAfterPrepareConsume;break;
          case PRS_BEFOREBUILD:rs=rsBeforeBuild;break;
        }

      TPluginStatus ps=PluginStatus;
      DisableEvents();
      try{
        reg->RunOrders(true,rs);
      }catch(...){}
      EnableEvents();
      PluginStatus=ps;
      return 0;
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI ObjectGetVarProp(PluginVariable *var, int index, PluginVariable *result)
{
  AObject *obj=var->obj;
  AObjectType *otype=var->objtype;
  switch(index)
  {
    case 0:
      return CreateIntVariable(obj->num,result);
    case 1:
      return CreateStringVariable(obj->name.c_str(),result);
    case 2:
      return CreateStringVariable(otype?otype->name.c_str():0,result);
    case 3:
      return CreateStringVariable(obj->describe.c_str(),result);
    case 4:
      return CreateIntVariable(obj->inner,result);
    case 5:
    {
      ARegion *reg=obj->basereg->turn->RegionList->Get(obj->innerloc);
      if(!reg)
        return ERROR_BADINDEXVALUE;
      result->Type=PVT_REGION;
      result->reg=reg;
      return 0;
    }
    case 6:
      return CreateIntVariable(obj->endincomplete,result);
    case 7:
      return CreateIntVariable(obj->incomplete,result);
    case 8:
      return CreateIntVariable(obj->runes,result);
    case 9:
      result->Type=PVT_REGION;
      result->reg=obj->endreg;
      return 0;
    case 10:
      result->Type=PVT_REGION;
      result->reg=obj->basereg;
      return 0;
    case 11:
    {
      int lastindex=IndexVars[PVT_OBJECT].GetLastSetIndex();
      switch(lastindex)
      {
        case -1:
        {
          int value=IndexVars[PVT_OBJECT].GetVarInt();
          if(value<0)
            return CreateIntVariable(obj->units->count,result);
          if(value>=obj->units->count)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_UNIT;
          result->unit=obj->units->Get(value);
          return 0;
        }
        case -2:
        {
          int value=IndexVars[PVT_OBJECT].GetVarInt();
          AUnit *un=obj->GetUnit(value);
          if(!un)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_UNIT;
          result->unit=un;
          return 0;
        }
        default: return ERROR_INDEXNOTSET;
      }
    }
    case 15:
      return CreateIntVariable(otype?otype->protect:0,result);
    case 16:
      return CreateIntVariable(otype?otype->capacity:0,result);
    case 17:
    {
      if(IndexVars[PVT_OBJECT].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_OBJECT].GetVarInt();
      int itemcount=0;
      if(otype)
      {
        if(otype->item.Length())
          itemcount=1;
        if(otype->item==WoodOrStoneStr)
          itemcount=2;
      }    
      if(value<0)
        return CreateIntVariable(itemcount,result);
      if(value>=itemcount)
        return ERROR_BADINDEXVALUE;
	  AItemType *itype;
      if(itemcount==1)
        itype=ItemTypes->Get(otype->item);
      else if(value==0)
        itype=ItemTypes->Get("WOOD");
      else
        itype=ItemTypes->Get("STON");
      if(!itype)
        return ERROR_BADINDEXVALUE;
      result->Type=PVT_ITEM;
      result->itemtype=itype;
      result->itemcount=otype->cost;
      result->itemprice=0;
      return 0;
    }
    case 18:
    {
      if(!otype||!otype->skill.Length())
        return 0;
      ASkillType *stype=SkillTypes->Get(otype->skill);
      if(!stype)
        return ERROR_BADINDEXVALUE;
      result->Type=PVT_SKILL;
      result->skilltype=stype;
      result->skilldays=0;
      result->skilllevel=otype->level;
      return 0;
    }
    case 19:
      return CreateIntVariable(otype?otype->sailors:0,result);
    case 20:
      return CreateIntVariable(otype?otype->canenter:1,result);
    case 21:
      return CreateIntVariable(otype?otype->road:0,result);
    case 22:
    {
      if(!otype||!otype->production.Length())
        return 0;
      AItemType *itype=ItemTypes->Get(otype->production);
      if(!itype)
        return ERROR_BADINDEXVALUE;
      result->Type=PVT_ITEM;
      result->itemtype=itype;
      result->itemcount=0;
      result->itemprice=0;
      return 0;
    }
    case 23:
      return CreateIntVariable(obj->basereg->IndexOf(obj),result);
    case 24:  
    {
      if(!ListCreate(result,PVT_UNIT))
        return ERROR_BADVARIABLE;
      foreach(obj->basereg)
      {
        AObject *_obj=*(AObject**)iter;
        foreach(_obj->units){
         AUnit *un=*(AUnit**) iter;
         if(un->endobj!=obj) continue;
          ListAddUnit(result,un);
        }
      }
      return 0;
    }
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI ObjectSetVarProp(PluginVariable *var, int index, PluginVariable *value)
{
  switch(index)
  {
    case -1:case -2:
      if(!IndexVars[PVT_OBJECT].SetVar(index,value))
        return ERROR_BADARGUMENTTYPE;
      return 0;
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI UnitGetVarProp(PluginVariable *var, int index, PluginVariable *result)
{
  AUnit *unit=var->unit;
  switch(index)
  {
    case 0:
      return CreateStringVariable(unit->name.c_str(),result);
    case 1:
      return CreateStringVariable(unit->describe.c_str(),result);
    case 2:
      return CreateIntVariable(unit->num,result);
    case 3:
      return CreateIntVariable(unit->IsLocal(),result);
    case 4:
      return CreateIntVariable(bool(unit->endflags&FLAG_BEHIND),result);
    case 5:
      return CreateIntVariable(bool(unit->endflags&FLAG_NOCROSS_WATER),result);
    case 6:
      return CreateIntVariable(bool(unit->endflags&FLAG_AUTOTAX),result);
    case 7:
      return CreateIntVariable(bool(unit->endflags&FLAG_HOLDING),result);
    case 8:
      return CreateIntVariable(bool(unit->endflags&FLAG_NOAID),result);
    case 9:
    {
      int consume=REVEAL_NONE; //it is no error
      if(unit->endflags&FLAG_CONSUMING_UNIT)
        consume=REVEAL_UNIT;
      if(unit->endflags&FLAG_CONSUMING_FACTION)
      consume=REVEAL_FACTION;
      return CreateIntVariable(consume,result);
    }
    case 10:
      return CreateIntVariable(unit->GetSpoilsFromFlag(unit->endflags),result);
    case 11:
      return CreateIntVariable(bool(unit->endguard&GUARD_GUARD),result);
    case 12:
      return CreateIntVariable(bool(unit->endguard&GUARD_AVOID),result);
    case 13:
      return CreateIntVariable(unit->endreveal,result);
    case 14:
    {
      result->Type=PVT_OBJECT;
      result->obj=unit->endobj;
      AObjectType *otype=ObjectTypes->Get(result->obj->type);
      result->objtype=otype;
      return 0;
    }
    case 15:
    {
      result->Type=PVT_OBJECT;
      result->obj=unit->baseobj;
      AObjectType *otype=ObjectTypes->Get(result->obj->type);
      result->objtype=otype;
      return 0;
    }
    case 16:
      result->Type=PVT_FACTION;
      result->faction=unit->endfaction;
      return 0;
    case 17:
      result->Type=PVT_FACTION;
      result->faction=unit->faction;
      return 0;
    case 18:
    {
      if(IndexVars[PVT_UNIT].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_UNIT].GetVarInt();
      if(value<0)
        return CreateIntVariable(unit->orders->count,result);
      if(value>=unit->orders->count)
        return ERROR_BADINDEXVALUE;
      AOrder *o=unit->orders->Get(value);
      result->Type=PVT_ORDER;
      result->order=o;
      return 0;
    }
    case 19:
      return CreateIntVariable(unit->IsLeader(),result);
    case 20:
      return CreateIntVariable(unit->GetMen(),result);
    case 21:
    {
      int lastindex=IndexVars[PVT_UNIT].GetLastSetIndex();
      switch(lastindex)
      {
        case -1:
        {
          int value=IndexVars[PVT_UNIT].GetVarInt();
          if(value<0)
            return CreateIntVariable(unit->enditems->count,result);
          if(value>=unit->enditems->count)
            return ERROR_BADINDEXVALUE;
          AItem *item=unit->enditems->Get(value);
          result->Type=PVT_ITEM;
          result->itemtype=item->type;
          result->itemcount=item->count;
          result->itemprice=0;
          return 0;
        }
        case -3:
        {
          AnsiString value=IndexVars[PVT_UNIT].GetVarString();
          AItemType *itype=ItemTypes->Get(value);
          if(!itype)
            return ERROR_BADINDEXVALUE;
          AItem *item=unit->enditems->GetItem(itype);
          result->Type=PVT_ITEM;
          result->itemtype=itype;
          result->itemcount=item?item->count:0;
          result->itemprice=0;
          return 0;
        }
        case -4:
        {
          int value=IndexVars[PVT_UNIT].GetVarInt();
          AItemType *itype2=(AItemType*)value;
          AItem *item=unit->enditems->GetItem(itype2);
          if(!item||!item->count)
            return 0;
          result->Type=PVT_ITEM;
          result->itemtype=item->type;
          result->itemcount=item->count;
          result->itemprice=0;
          return 0;
        }
        default: return ERROR_INDEXNOTSET;
      }
    }
    case 22:
    {
      int lastindex=IndexVars[PVT_UNIT].GetLastSetIndex();
      switch(lastindex)
      {
        case -1:
        {
          int value=IndexVars[PVT_UNIT].GetVarInt();
          if(value<0)
            return CreateIntVariable(unit->items->count,result);
          if(value>=unit->items->count)
            return ERROR_BADINDEXVALUE;
          AItem *item=unit->items->Get(value);
          result->Type=PVT_ITEM;
          result->itemtype=item->type;
          result->itemcount=item->count;
          result->itemprice=0;
          return 0;
        }
        case -3:
        {
          AnsiString value=IndexVars[PVT_UNIT].GetVarString();
          AItemType *itype=ItemTypes->Get(value);
          if(!itype)
            return ERROR_BADINDEXVALUE;
          AItem *item=unit->items->GetItem(itype);
          result->Type=PVT_ITEM;
          result->itemtype=itype;
          result->itemcount=item?item->count:0;
          result->itemprice=0;
          return 0;
        }
        case -4:
        {
          int value=IndexVars[PVT_UNIT].GetVarInt();
          AItemType *itype2=(AItemType*)value;
          AItem *item=unit->items->GetItem(itype2);
          if(!item||!item->count)
            return 0;
          result->Type=PVT_ITEM;
          result->itemtype=item->type;
          result->itemcount=item->count;
          result->itemprice=0;
          return 0;
        }
        default: return ERROR_INDEXNOTSET;
      }
    }
    case 23:
    {
      int lastindex=IndexVars[PVT_UNIT].GetLastSetIndex();
      switch(lastindex)
      {
        case -1:
        {
          int value=IndexVars[PVT_UNIT].GetVarInt();
          if(value<0)
            return CreateIntVariable(unit->endskills->count,result);
          if(value>=unit->endskills->count)
            return ERROR_BADINDEXVALUE;
          ASkill *skill=unit->endskills->Get(value);
          result->Type=PVT_SKILL;
          result->skilltype=skill->type;
          result->skilldays=skill->days;
          result->skilllevel=skill->GetLevel();
          return 0;
        }
        case -3:
        {
          AnsiString value=IndexVars[PVT_UNIT].GetVarString();
          ASkillType *stype=SkillTypes->Get(value);
          if(!stype)
            return ERROR_BADINDEXVALUE;
          ASkill *skill=unit->endskills->GetSkill(stype);
          result->Type=PVT_SKILL;
          result->skilltype=stype;
          result->skilldays=skill?skill->days:0;
          result->skilllevel=skill?skill->GetLevel():0;
          return 0;
        }
        case -4:
        {
          int value=IndexVars[PVT_UNIT].GetVarInt();
          ASkillType *stype2=(ASkillType*)value;
          ASkill *skill=unit->endskills->GetSkill(stype2);
          if(!skill||!skill->days)
            return 0;
          result->Type=PVT_SKILL;
          result->skilltype=skill->type;
          result->skilldays=skill->days;
          result->skilllevel=skill->GetLevel();
          return 0;
        }
        default: return ERROR_INDEXNOTSET;
      }
    }
    case 24:
    {
      int lastindex=IndexVars[PVT_UNIT].GetLastSetIndex();
      switch(lastindex)
      {
        case -1:
        {
          int value=IndexVars[PVT_UNIT].GetVarInt();
          if(value<0)
            return CreateIntVariable(unit->skills->count,result);
          if(value>=unit->skills->count)
            return ERROR_BADINDEXVALUE;
          ASkill *skill=unit->skills->Get(value);
          result->Type=PVT_SKILL;
          result->skilltype=skill->type;
          result->skilldays=skill->days;
          result->skilllevel=skill->GetLevel();
          return 0;
        }
        case -3:
        {
          AnsiString value=IndexVars[PVT_UNIT].GetVarString();
          ASkillType *stype=SkillTypes->Get(value);
          if(!stype)
            return ERROR_BADINDEXVALUE;
          ASkill *skill=unit->skills->GetSkill(stype);
          result->Type=PVT_SKILL;
          result->skilltype=stype;
          result->skilldays=skill?skill->days:0;
          result->skilllevel=skill?skill->GetLevel():0;
          return 0;
        }
        case -4:
        {
          int value=IndexVars[PVT_UNIT].GetVarInt();
          ASkillType *stype2=(ASkillType*)value;
          ASkill *skill=unit->skills->GetSkill(stype2);
          if(!skill||!skill->days)
            return 0;
          result->Type=PVT_SKILL;
          result->skilltype=skill->type;
          result->skilldays=skill->days;
          result->skilllevel=skill->GetLevel();
          return 0;
        }
        default: return ERROR_INDEXNOTSET;
      }
    }
    case 25:
      return CreateStringVariable(unit->combatspell.c_str(),result);
    case 26:
      return CreateIntVariable(unit->weight,result);
    case 27:
      return CreateIntVariable(unit->walkweight,result);
    case 28:
      return CreateIntVariable(unit->ridweight,result);
    case 29:
      return CreateIntVariable(unit->flyweight,result);
    case 30:
      return CreateIntVariable(unit->swimweight,result);
    case 31:
      return CreateStringVariable(unit->GetLocalDescr().c_str(),result);
    case 32:
      return CreateIntVariable(unit->IsMage(),result);
    case 33:
    {
      AOrder *ord=unit->orders->GetMonthLong();
      if(!ord)
        return 0;
      result->Type=PVT_ORDER;
      result->order=ord;
      return 0;
    }
    case 34:
    {
      if(IndexVars[PVT_UNIT].GetLastSetIndex()!=-3)
        return ERROR_INDEXNOTSET;
      AnsiString value=IndexVars[PVT_UNIT].GetVarString();
      AnsiString str=unit->GetComment(value);
      return CreateStringVariable(str.c_str(),result);
    }
    case 35:
      return CreateIntVariable(unit->baseobj->units->IndexOf(unit),result);
    case 36:
      return CreateStringVariable(unit->FullName(false).c_str(),result);
    case 37:
      return CreateStringVariable(unit->FullName(true).c_str(),result);
    case 38:
    {
      if(PluginStatus!=plsNormal)
        return ERROR_BADPROPERTYINDEX;
      AObject *obj=unit->baseobj;
      ARegion *reg=obj->basereg;
      AUnit *newun=new AUnit(curTurn);
      newun->num=reg->GetNewUnitNum();
      newun->FormNew(unit);
      obj->units->Add(newun);
      newun->PrepareOrders();
      newun->SetFlagOrders(unit->endguard,unit->endreveal,unit->endflags);
      reg->turn->Modified=true;
      reg->runned=false;
      PluginNeedMakeTree=true;
      result->Type=PVT_UNIT;
      result->unit=newun;
      return 0;
    }
    case 39:
      return CreateStringVariable(unit->params.c_str(),result);
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI UnitSetVarProp(PluginVariable *var, int index, PluginVariable *value)
{
  AUnit *unit=var->unit;
  ARegion *reg=unit->baseobj->basereg;
  switch(index)
  {
    case -1:case -3:case -4:
      if(!IndexVars[PVT_UNIT].SetVar(index,value))
        return ERROR_BADARGUMENTTYPE;
      return 0;
    case 4:
    {
      if(value->Type!=PVT_INTEGER)
        return ERROR_BADARGUMENTTYPE;
      int val=value->value;  
      unsigned guard=unit->endguard;
      unsigned reveal=unit->endreveal;
      unsigned flags=unit->endflags;
      flags&=~FLAG_BEHIND;
      if(val)
        flags|=FLAG_BEHIND;
      unit->SetFlagOrders(guard,reveal,flags);
      reg->turn->Modified=true;
      reg->runned=false;
      return 0;
    }
    case 5:
    {
      if(value->Type!=PVT_INTEGER)
        return ERROR_BADARGUMENTTYPE;
      int val=value->value;
      unsigned guard=unit->endguard;
      unsigned reveal=unit->endreveal;
      unsigned flags=unit->endflags;
      flags&=~FLAG_NOCROSS_WATER;
      if(val)
        flags|=FLAG_NOCROSS_WATER;
      unit->SetFlagOrders(guard,reveal,flags);
      reg->turn->Modified=true;
      reg->runned=false;
      return 0;
    }
    case 6:
    {
      if(value->Type!=PVT_INTEGER)
        return ERROR_BADARGUMENTTYPE;
      int val=value->value;
      unsigned guard=unit->endguard;
      unsigned reveal=unit->endreveal;
      unsigned flags=unit->endflags;
      flags&=~FLAG_AUTOTAX;
      if(val)
        flags|=FLAG_AUTOTAX;
      unit->SetFlagOrders(guard,reveal,flags);
      reg->turn->Modified=true;
      reg->runned=false;
      return 0;
    }
    case 7:
    {
      if(value->Type!=PVT_INTEGER)
        return ERROR_BADARGUMENTTYPE;
      int val=value->value;
      unsigned guard=unit->endguard;
      unsigned reveal=unit->endreveal;
      unsigned flags=unit->endflags;
      flags&=~FLAG_HOLDING;
      if(val)
        flags|=FLAG_HOLDING;
      unit->SetFlagOrders(guard,reveal,flags);
      reg->turn->Modified=true;
      reg->runned=false;
      return 0;
    }
    case 8:
    {
      if(value->Type!=PVT_INTEGER)
        return ERROR_BADARGUMENTTYPE;
      int val=value->value;
      unsigned guard=unit->endguard;
      unsigned reveal=unit->endreveal;
      unsigned flags=unit->endflags;
      flags&=~FLAG_NOAID;
      if(val)
        flags|=FLAG_NOAID;
      unit->SetFlagOrders(guard,reveal,flags);
      reg->turn->Modified=true;
      reg->runned=false;
      return 0;
    }
    case 9:
    {
      if(value->Type!=PVT_INTEGER)
        return ERROR_BADARGUMENTTYPE;
      int val=value->value;
      unsigned guard=unit->endguard;
      unsigned reveal=unit->endreveal;
      unsigned flags=unit->endflags;
      flags&=~(FLAG_CONSUMING);
      if(val==1)
        flags|=FLAG_CONSUMING_UNIT;
      else if(val==2)
        flags|=FLAG_CONSUMING_FACTION;
      unit->SetFlagOrders(guard,reveal,flags);
      reg->turn->Modified=true;
      reg->runned=false;
      return 0;
    }
    case 10:
    {
      if(value->Type!=PVT_INTEGER)
        return ERROR_BADARGUMENTTYPE;
      int val=value->value;
      unsigned guard=unit->endguard;
      unsigned reveal=unit->endreveal;
      unsigned flags=unit->endflags;
      flags&=~(FLAG_NOSPOILS|FLAG_FLYSPOILS|FLAG_WALKSPOILS|FLAG_RIDESPOILS);
      flags|=AUnit::GetFlagFromSpoils(val);
      unit->SetFlagOrders(guard,reveal,flags);
      reg->turn->Modified=true;
      reg->runned=false;
      return 0;
    }
    case 11:
    {
      if(value->Type!=PVT_INTEGER)
        return ERROR_BADARGUMENTTYPE;
      int val=value->value;
      unsigned guard;//=unit->endguard;
      unsigned reveal=unit->endreveal;
      unsigned flags=unit->endflags;
      guard=val?GUARD_GUARD:GUARD_NONE;
      unit->SetFlagOrders(guard,reveal,flags);
      reg->turn->Modified=true;
      reg->runned=false;
      return 0;
    }
    case 12:
    {
      if(value->Type!=PVT_INTEGER)
        return ERROR_BADARGUMENTTYPE;
      int val=value->value;
	  unsigned guard;//=unit->endguard;
      unsigned reveal=unit->endreveal;
      unsigned flags=unit->endflags;
      guard=val?GUARD_AVOID:GUARD_NONE;
      unit->SetFlagOrders(guard,reveal,flags);
      reg->turn->Modified=true;
      reg->runned=false;
      return 0;
    }
    case 13:
    {
      if(value->Type!=PVT_INTEGER)
        return ERROR_BADARGUMENTTYPE;
      int val=value->value;
      unsigned guard=unit->endguard;
	  unsigned reveal;//=unit->endreveal;
      unsigned flags=unit->endflags;
      reveal=val%3;
      unit->SetFlagOrders(guard,reveal,flags);
      reg->turn->Modified=true;
      reg->runned=false;
      return 0;
    }
    case 21:
    {
      if(PluginStatus!=plsRunOrder)
        return ERROR_BADPROPERTYINDEX;
      if(value->Type!=PVT_INTEGER)
        return ERROR_BADARGUMENTTYPE;
      int lastindex=IndexVars[PVT_UNIT].GetLastSetIndex();
      switch(lastindex)
      {
        case -3:
        {
          AnsiString name=IndexVars[PVT_UNIT].GetVarString();
          AItemType *itype=ItemTypes->Get(name);
          if(!itype)
            return ERROR_BADINDEXVALUE;
          unit->enditems->SetNum(itype,value->value);
          return 0;
        }
        case -4:
        {
          int val=IndexVars[PVT_UNIT].GetVarInt();
          AItemType *itype=(AItemType*)val;
          if(!ItemTypes->IndexOf(itype))
            return ERROR_BADINDEXVALUE;
          unit->enditems->SetNum(itype,value->value);
          return 0;
        }
        default: return ERROR_INDEXNOTSET;
      }
    }
    case 23:
    {
      if(PluginStatus!=plsRunOrder)
        return ERROR_BADPROPERTYINDEX;
      if(value->Type!=PVT_INTEGER)
        return ERROR_BADARGUMENTTYPE;
      int lastindex=IndexVars[PVT_UNIT].GetLastSetIndex();
      switch(lastindex)
      {
        case -3:
        {
          AnsiString name=IndexVars[PVT_UNIT].GetVarString();
          ASkillType *stype=SkillTypes->Get(name);
          if(!stype)
            return ERROR_BADINDEXVALUE;
          unit->SetSkillDays(stype->abr,value->value);
          return 0;
        }
        case -4:
        {
          int val=IndexVars[PVT_UNIT].GetVarInt();
          ASkillType *stype=(ASkillType*)val;
          if(!SkillTypes->IndexOf(stype))
            return ERROR_BADINDEXVALUE;
          unit->SetSkillDays(stype->abr,value->value);
          return 0;
        }
        default: return ERROR_INDEXNOTSET;
      }
    }
    case 31:
      if(value->Type!=PVT_STRING)
        return ERROR_BADARGUMENTTYPE;
      unit->SetLocalDescr(value->str);
      reg->turn->Modified=true;
      return 0;
    case 34:
    {
      if(value->Type!=PVT_STRING)
        return ERROR_BADARGUMENTTYPE;
      if(IndexVars[PVT_UNIT].GetLastSetIndex()!=-3)
        return ERROR_INDEXNOTSET;
      AnsiString val=IndexVars[PVT_UNIT].GetVarString();
      AnsiString str=value->str;
      unit->SetComment(val,str);
      return 0;
    }
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI UnitRunVarFunc(PluginVariable *var, int index, PluginVariable *arg)
{
  AUnit *unit=var->unit;
  switch(index)
  {
    case 0:
    {
      if(arg->Type!=PVT_ORDER)
        return ERROR_BADARGUMENTTYPE;
      AOrder *ord=arg->order;
      if(ord->orders)
        return ERROR_BADARGUMENTTYPE;
      unit->orders->Add(ord);
      unit->baseobj->basereg->turn->Modified=true;
      unit->baseobj->basereg->runned=false;
      return 0;
    }
    case 1:
    {
      if(IndexVars[PVT_UNIT].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_UNIT].GetVarInt();
      if(value<0||value>=unit->orders->count)
        return ERROR_BADINDEXVALUE;
      unit->orders->Delete(value);
      unit->baseobj->basereg->turn->Modified=true;
      unit->baseobj->basereg->runned=false;
      return 0;
    }
    case 2:
    {
      if(arg->Type!=PVT_ORDER)
        return ERROR_BADARGUMENTTYPE;
      AOrder *ord=arg->order;
      if(ord->orders)
        return ERROR_BADARGUMENTTYPE;
      if(IndexVars[PVT_UNIT].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_UNIT].GetVarInt();
      if(value<0||value>unit->orders->count)
        return ERROR_BADINDEXVALUE;
      unit->orders->Insert(value,ord);
      unit->baseobj->basereg->turn->Modified=true;
      unit->baseobj->basereg->runned=false;
      return 0;
    }
    case 3:
    {
      if(IndexVars[PVT_UNIT].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_UNIT].GetVarInt();
      if(value<0||value>=unit->orders->count)
        return ERROR_BADINDEXVALUE;
      AUnit *oldCurUnit=curUnit;
      curUnit=unit;
      TPluginStatus ps=PluginStatus;
      AtlaForm->EditOrder(value);
      PluginStatus=ps;
      curUnit=oldCurUnit;
      return 0;
    }
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI ItemGetVarProp(PluginVariable *var, int index, PluginVariable *result)
{
  AItemType *itype=var->itemtype;
  switch(index)
  {
    case 0:
      return CreateIntVariable(var->itemcount,result);
    case 1:
      return CreateIntVariable(var->itemprice,result);
    case 2:
      return CreateStringVariable(itype->abr.c_str(),result);
    case 3:
      return CreateStringVariable(itype->name.c_str(),result);
    case 4:
      return CreateStringVariable(itype->names.c_str(),result);
    case 5:
      return CreateIntVariable(itype->weight,result);
    case 6:
      return CreateIntVariable(itype->walk,result);
    case 7:
      return CreateIntVariable(itype->ride,result);
    case 8:
      return CreateIntVariable(itype->fly,result);
    case 9:
      return CreateIntVariable(itype->swim,result);
    case 10:
      return CreateIntVariable(itype->flags&AItemType::CANTGIVE,result);
    case 11:
    {
      if(!itype->skill.Length())
        return 0;
      ASkillType *stype=SkillTypes->Get(itype->skill);
      if(!stype)
        return ERROR_BADINDEXVALUE;
      result->Type=PVT_SKILL;
      result->skilltype=stype;
      result->skilldays=0;
      result->skilllevel=itype->level;
      return 0;
    }
    case 12:
    {
      if(IndexVars[PVT_ITEM].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_ITEM].GetVarInt();
      int inputcount=0;
      while(itype->inputs[inputcount].Length())
        inputcount++;
      if(value<0)
        return CreateIntVariable(inputcount,result);
      if(value>=inputcount)
        return ERROR_BADINDEXVALUE;
      AItemType *itype2=ItemTypes->Get(itype->inputs[value]);
      if(!itype2)
        return ERROR_BADINDEXVALUE;
      result->Type=PVT_ITEM;
      result->itemtype=itype2;
      result->itemcount=itype->numbers[value];
      result->itemprice=0;
      return 0;
    }
    case 13:
      return CreateIntVariable(int(itype),result);
    case 14:
      if(!itype->inputs[0].Length())
        return ERROR_BADPROPERTYINDEX;
      return CreateIntVariable(itype->months,result);
    case 15:
      if(!itype->inputs[0].Length())
        return ERROR_BADPROPERTYINDEX;
      return CreateIntVariable(itype->out,result);
    case 16:
      return CreateIntVariable(!itype->combat,result);
    case 17:
    {
      if(!itype->helpprodmult)
        return 0;
      AItemType *itype2=ItemTypes->Get(itype->helpproditem);
      if(!itype2)
        return ERROR_BADINDEXVALUE;
      result->Type=PVT_ITEM;
      result->itemtype=itype2;
      result->itemcount=itype->helpprodmult;
      result->itemprice=0;
      return 0;
    }
    case 18:
    {
      if(!itype->hitchItem.Length())
        return 0;
      AItemType *itype2=ItemTypes->Get(itype->hitchItem);
      if(!itype2)
        return ERROR_BADINDEXVALUE;
      result->Type=PVT_ITEM;
      result->itemtype=itype2;
      result->itemcount=itype->hitchwalk;
      result->itemprice=0;
      return 0;
    }
    case 19:
      return CreateIntVariable(itype->type,result);
    case 20:
      if(!(itype->type&IT_MAN))
        return ERROR_BADPROPERTYINDEX;
      AManType *mtype=ManTypes->Get(itype->abr);
      if(!mtype)
        return ERROR_BADPROPERTYINDEX;
      if(IndexVars[PVT_ITEM].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_ITEM].GetVarInt();
      int count=mtype->skills->Count+1;
      if(value<0)
        return CreateIntVariable(count,result);
      if(value>=count)
        return ERROR_BADINDEXVALUE;
      if(value==0)
        return CreateIntVariable(mtype->deflevel,result);
      value--;
      ASkillType *stype=SkillTypes->Get(mtype->skills->Strings[value]);
      if(!stype)
        return ERROR_BADINDEXVALUE;
      int level=(int)mtype->skills->Objects[value];
      result->Type=PVT_SKILL;
      result->skilltype=stype;
      result->skilldays=0;
      result->skilllevel=level;
      return 0;
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI ItemSetVarProp(PluginVariable *var, int index, PluginVariable *value)
{
  switch(index)
  {
    case -1:
      if(!IndexVars[PVT_ITEM].SetVar(index,value))
        return ERROR_BADARGUMENTTYPE;
      return 0;
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI SkillGetVarProp(PluginVariable *var, int index, PluginVariable *result)
{
  ASkillType *stype=var->skilltype;
  switch(index)
  {
    case 0:
      return CreateIntVariable(var->skilldays,result);
    case 1:
      return CreateIntVariable(var->skilllevel,result);
    case 2:
      return CreateStringVariable(stype->abr.c_str(),result);
    case 3:
      return CreateStringVariable(stype->name.c_str(),result);
    case 4:
      return CreateIntVariable(stype->cost,result);
    case 5:
      return CreateIntVariable(stype->flags,result);
    case 6:
    {
      int lastindex=IndexVars[PVT_SKILL].GetLastSetIndex();
      switch(lastindex)
      {
        case -1:
        {
          int value=IndexVars[PVT_SKILL].GetVarInt();
          int count=3;
          for(int i=0;i<count;i++)
          {
            if(stype->depends[i].Length()) continue;
            count=i;
            break;
          }
          if(value<0)
            return CreateIntVariable(count,result);
          if(value>=count)
            return ERROR_BADINDEXVALUE;
          AnsiString str=stype->depends[value];
          if(!str.Length())
            return ERROR_BADINDEXVALUE;
          ASkillType *stype2=SkillTypes->Get(str,2);
          if(!stype2)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_SKILL;
          result->skilltype=stype2;
          result->skilldays=0;
          result->skilllevel=stype->levels[value];
          return 0;
        }
        default:return ERROR_INDEXNOTSET;
      }
    }
    case 7:
      return CreateIntVariable(int(stype),result);
    case 8:
      return CreateIntVariable(stype->sortindex,result);
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI SkillSetVarProp(PluginVariable *var, int index, PluginVariable *value)
{
  switch(index)
  {
    case -1:
      if(!IndexVars[PVT_SKILL].SetVar(index,value))
        return ERROR_BADARGUMENTTYPE;
      return 0;
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI OrderCopyVar(PluginVariable *to, const PluginVariable *from)
{
  if(from->order->orders)
    return NullCopyVar(to,from);
  AnsiString str=from->order->WriteTemplate();
  AOrder *ord=ParseOrder(str);
  if(!ord)
    return ERROR_BADVARIABLE;
  to->Type=PVT_ORDER;
  to->order=ord;
  return 0;
}
int WINAPI OrderFreeVar(PluginVariable *var)
{
  if(var->order->orders)
    return NullFreeVar(var);
  delete var->order;
  var->Type=PVT_NONE;
  return 0;
}
int WINAPI OrderGetVarProp(PluginVariable *var, int index, PluginVariable *result)
{
  AOrder *ord=var->order;
  switch(index)
  {
    case 0:
      return CreateIntVariable(ord->type,result);
    case 1:
    {
      AnsiString str=ord->WriteTemplate();
      return CreateStringVariable(str.c_str(),result);
    }
    case 2:
      return CreateIntVariable(ord->IsMonthLong(),result);
    case 3:
      return CreateIntVariable(ord->repeating,result);
    case 4:
      return CreateIntVariable(ord->commented,result);
    case 5:
      if(!ord->orders)
        return ERROR_BADPROPERTYINDEX;
      return CreateIntVariable(ord->orders->IndexOf(ord),result);
    case 6:
      if(!ord->orders)
        return ERROR_BADPROPERTYINDEX;
      result->Type=PVT_UNIT;
      result->unit=ord->orders->unit;
      return 0;
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI OrderSetVarProp(PluginVariable *var, int index, PluginVariable *value)
{
  AOrder *ord=var->order;
  switch(index)
  {
    case 1:
    {
      if(value->Type!=PVT_STRING)
        return ERROR_BADARGUMENTTYPE;
      AnsiString str=value->str;
      AOrder *neword=ParseOrder(str);
      if(!neword)
        return ERROR_BADARGUMENTTYPE;
      if(ord->orders)
      {
        if(!ord->orders->Replace(ord,neword))
        {
          delete neword;
          return ERROR_BADARGUMENTTYPE;
        }
        ord->orders->unit->baseobj->basereg->turn->Modified=true;
        ord->orders->unit->baseobj->basereg->runned=false;
      }else
        delete ord;
      var->order=neword;
      return 0;
    }
    case 3:
    {
      if(value->Type!=PVT_INTEGER)
        return ERROR_BADARGUMENTTYPE;
      ord->repeating=value->value;
      if(ord->orders)
        ord->orders->unit->baseobj->basereg->turn->Modified=true;
      return 0;
    }
    case 4:
    {
      if(value->Type!=PVT_INTEGER)
        return ERROR_BADARGUMENTTYPE;
      ord->commented=value->value;
      if(ord->orders)
      {
        ord->orders->unit->baseobj->basereg->turn->Modified=true;
        ord->orders->unit->baseobj->basereg->runned=false;
      }
      return 0;
    }
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI FactionGetVarProp(PluginVariable *var, int index, PluginVariable *result)
{
  AFaction *fac=var->faction;
  switch(index)
  {
    case 0:
      return CreateIntVariable(fac->number,result);
    case 1:
      return CreateStringVariable(fac->name.c_str(),result);
    case 2:
      return CreateIntVariable(fac->local,result);
    case 3:
      return CreateIntVariable(fac->unclaimed,result);
    case 4:
      return CreateStringVariable(fac->address.c_str(),result);
    case 5:
      return CreateIntVariable(fac->defattitude,result);
    case 6:
      return CreateIntVariable(fac->warpts,result);
    case 7:
      return CreateIntVariable(fac->tradepts,result);
    case 8:
      return CreateIntVariable(fac->magepts,result);
    case 9:
      return CreateIntVariable(fac->warmax,result);
    case 10:
      return CreateIntVariable(fac->trademax,result);
    case 11:
      return CreateIntVariable(fac->magemax,result);
    case 12:
      return CreateIntVariable(fac->warused,result);
    case 13:
      return CreateIntVariable(fac->tradeused,result);
    case 14:
      return CreateIntVariable(fac->mageused,result);
    case 15:
    {
      if(IndexVars[PVT_FACTION].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_FACTION].GetVarInt();
      if(value<0)
        return CreateIntVariable(fac->unkfacstatus->Count,result);
      if(value>=fac->unkfacstatus->Count)
        return ERROR_BADINDEXVALUE;
      return CreateStringVariable(fac->unkfacstatus->Strings[value].c_str(),result);
    }
    case 16:
    {
      if(IndexVars[PVT_FACTION].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_FACTION].GetVarInt();
      if(value<0)
        return CreateIntVariable(fac->unkfacmoneys->Count,result);
      if(value>=fac->unkfacmoneys->Count)
        return ERROR_BADINDEXVALUE;
      return CreateStringVariable(fac->unkfacmoneys->Strings[value].c_str(),result);
    }
    case 17:
    {
      if(IndexVars[PVT_FACTION].GetLastSetIndex()!=-2)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_FACTION].GetVarInt();
      return CreateIntVariable(fac->GetAttitude(value),result);
    }
    case 19:
    {
      if(IndexVars[PVT_FACTION].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_FACTION].GetVarInt();
      if(value<0)
        return CreateIntVariable(fac->errors->Count,result);
      if(value>=fac->errors->Count)
        return ERROR_BADINDEXVALUE;
      return CreateStringVariable(fac->errors->Strings[value].c_str(),result);
    }
    case 20:
    {
      if(IndexVars[PVT_FACTION].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_FACTION].GetVarInt();
      if(value<0)
        return CreateIntVariable(fac->battles->Count,result);
      if(value>=fac->battles->Count)
        return ERROR_BADINDEXVALUE;
      return CreateStringVariable(fac->battles->Strings[value].c_str(),result);
    }
    case 21:
    {
      if(IndexVars[PVT_FACTION].GetLastSetIndex()!=-1)
        return ERROR_INDEXNOTSET;
      int value=IndexVars[PVT_FACTION].GetVarInt();
      if(value<0)
        return CreateIntVariable(fac->events->Count,result);
      if(value>=fac->events->Count)
        return ERROR_BADINDEXVALUE;
      return CreateStringVariable(fac->events->Strings[value].c_str(),result);
    }
		case 22:
			return CreateIntVariable(fac->CountTaxRegions(),result);
		case 23:
			return CreateIntVariable(fac->CountTradeRegions(),result);
		case 24:
			return CreateIntVariable(fac->CountMages(),result);
	}
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI FactionSetVarProp(PluginVariable *var, int index, PluginVariable *value)
{
  switch(index)
  {
    case 4:
    {
      if(value->Type!=PVT_STRING)
        return ERROR_BADARGUMENTTYPE;
      AnsiString str=value->str;
      AFaction *fac=var->faction;
      if(fac->address==str)
        return 0;
      fac->address=str;
      fac->factions->turn->Modified=true;
      return 0;
    }
    case -1:case -2:
       if(!IndexVars[PVT_FACTION].SetVar(index,value))
        return ERROR_BADARGUMENTTYPE;
      return 0;
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI GameGetVarProp(PluginVariable *var, int index, PluginVariable *result)
{
  switch(index)
  {
    case 1:
      return CreateStringVariable(curGame.c_str(),result);
    case 2:
    {
      if(IndexVars[PVT_GAME].GetLastSetIndex()!=-3)
        return ERROR_INDEXNOTSET;
      AnsiString name=IndexVars[PVT_GAME].GetVarString();
      return CreateStringVariable(curTurns->pluginsstr->Values[name].c_str(),result);
    }
    case 4:
    {
      int lastindex=IndexVars[PVT_GAME].GetLastSetIndex();
      switch(lastindex)
      {
        case -4:
        {
          int value=IndexVars[PVT_GAME].GetVarInt();
          ASkillType *stype=(ASkillType*)value;
          if(SkillTypes->IndexOf(stype)<0)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_SKILL;
          result->skilltype=stype;
          result->skilldays=0;
          result->skilllevel=0;
          return 0;
        }
        case -3:
        {
          AnsiString value=IndexVars[PVT_GAME].GetVarString();
          ASkillType *stype=SkillTypes->Get(value);
          if(!stype)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_SKILL;
          result->skilltype=stype;
          result->skilldays=0;
          result->skilllevel=0;
          return 0;
        }
        case -1:
        {
          int value=IndexVars[PVT_GAME].GetVarInt();
          if(value<0)
            return CreateIntVariable(SkillTypes->count,result);
          if(value>=SkillTypes->count)
            return ERROR_BADINDEXVALUE;
          ASkillType *stype=SkillTypes->Get(value);
          result->Type=PVT_SKILL;
          result->skilltype=stype;
          result->skilldays=0;
          result->skilllevel=0;
          return 0;
        }
        default:return ERROR_INDEXNOTSET;
      }
    }
    case 5:
    {
      int lastindex=IndexVars[PVT_GAME].GetLastSetIndex();
      switch(lastindex)
      {
        case -4:
        {
          int value=IndexVars[PVT_GAME].GetVarInt();
          AItemType *itype=(AItemType*)value;
          if(ItemTypes->IndexOf(itype)<0)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_ITEM;
          result->itemtype=itype;
          result->itemcount=0;
          result->itemprice=0;
          return 0;
        }
        case -3:
        {
          AnsiString value=IndexVars[PVT_GAME].GetVarString();
          AItemType *itype=ItemTypes->Get(value);
          if(!itype)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_ITEM;
          result->itemtype=itype;
          result->itemcount=0;
          result->itemprice=0;
          return 0;
        }
        case -1:
        {
          int value=IndexVars[PVT_GAME].GetVarInt();
          if(value<0)
            return CreateIntVariable(ItemTypes->count,result);
          if(value>=ItemTypes->count)
            return ERROR_BADINDEXVALUE;
          AItemType *itype=ItemTypes->Get(value);
          result->Type=PVT_ITEM;
          result->itemtype=itype;
          result->itemcount=0;
          result->itemprice=0;
          return 0;
        }
        default:return ERROR_INDEXNOTSET;
      }
    }
    case 6:
    {
      int lastindex=IndexVars[PVT_GAME].GetLastSetIndex();
      switch(lastindex)
      {
        case -3:
        {
          AnsiString value=IndexVars[PVT_GAME].GetVarString();
          AObjectType *otype=ObjectTypes->Get(value);
          if(!otype)
            return ERROR_BADINDEXVALUE;
          result->Type=PVT_OBJECT;
          result->obj=0;
          result->objtype=otype;
          return 0;
        }
        case -1:
        {
          int value=IndexVars[PVT_GAME].GetVarInt();
          if(value<0)
            return CreateIntVariable(ObjectTypes->count,result);
          if(value>=ObjectTypes->count)
            return ERROR_BADINDEXVALUE;
          AObjectType *otype=ObjectTypes->Get(value);
          result->Type=PVT_OBJECT;
          result->obj=0;
          result->objtype=otype;
          return 0;
        }
        default:return ERROR_INDEXNOTSET;
      }
    }
    case 7:
    {
      if(!ListCreate(result,PVT_REGION))
        return ERROR_BADVARIABLE;
      if(!RegionListForm->FillList(result))
        return ERROR_BADVARIABLE;
      return 0;
    }
    case 8:
    {
      if(IndexVars[PVT_GAME].GetLastSetIndex()!=-3)
        return ERROR_INDEXNOTSET;
      AnsiString name=IndexVars[PVT_GAME].GetVarString();
      const TGameParam *gp=GameParams.Find_GameParam(name);
      if(!gp)
        return ERROR_BADPROPERTYINDEX;
      if(gp->type==0) //integer
        return CreateIntVariable(*gp->ptr,result);
      if(gp->type==1) //string
        return CreateStringVariable(gp->ptrstr->c_str(),result);
      return ERROR_BADVARIABLE;
    }
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI GameSetVarProp(PluginVariable *var, int index, PluginVariable *value)
{
  switch(index)
  {
    case 0:
      if(ExprRetVal)
      {
        delete ExprRetVal;ExprRetVal=0;
      }
      switch(value->Type)
      {
        case PVT_INTEGER:
        {
          EDataInt *e=new EDataInt;
          e->value=value->value;
          ExprRetVal=e;
          return 0;
        }
        case PVT_STRING:
        {
          EDataString *e=new EDataString;
          e->value=value->str;
          ExprRetVal=e;
          return 0;
        }
        default:
          return ERROR_BADARGUMENTTYPE;
      }
    case 2:
    {
      if(IndexVars[PVT_GAME].GetLastSetIndex()!=-3)
        return ERROR_INDEXNOTSET;
      if(value->Type!=PVT_STRING)
          return ERROR_BADARGUMENTTYPE;
      AnsiString name=IndexVars[PVT_GAME].GetVarString();
      curTurns->pluginsstr->Values[name]=value->str;
      return 0;
    }
    case -1:case -3:case -4:
      if(!IndexVars[PVT_GAME].SetVar(index,value))
        return ERROR_BADARGUMENTTYPE;
      return 0;
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI GameRunVarFunc(PluginVariable *var, int index, PluginVariable *arg)
{
  TPluginStatus ps;
  switch(index)
  {
    case 0:
    {
      if(arg->Type!=PVT_REGION)
        return ERROR_BADARGUMENTTYPE;
      ARegion *reg=arg->reg;
      ps=PluginStatus;
      AtlaForm->GotoRegion(reg);
      PluginStatus=ps;
      return 0;
    }
    case 1:
    {
      if(arg->Type!=PVT_UNIT)
        return ERROR_BADARGUMENTTYPE;
      AUnit *un=arg->unit;
      ps=PluginStatus;
      AtlaForm->GotoUnit(un);
      PluginStatus=ps;
      return 0;
    }
    case 2:
      if(PluginStatus!=plsNormal)
        return ERROR_BADPROPERTYINDEX;
      ps=PluginStatus;
      AtlaForm->MakeTreeView();
      PluginStatus=ps;
      return 0;
    case 3:
      if(PluginStatus!=plsNormal)
        return ERROR_BADPROPERTYINDEX;
      ps=PluginStatus;
      AtlaForm->ViewShow();
      PluginStatus=ps;
      return 0;
  }
  return ERROR_BADPROPERTYINDEX;
}

bool ListCreate(PluginVariable *list, int elemtype)
{
  if(elemtype!=PVT_REGION&&elemtype!=PVT_UNIT)
    return false;
  list->Type=PVT_LIST;
  list->ElemType=elemtype;
  list->list=new vector<void*>;
  return true;
}
bool ListAddReg(PluginVariable *list, ARegion *reg)
{
  if(list->Type!=PVT_LIST||list->ElemType!=PVT_REGION)
    return false;
  list->list->push_back(reg);
  return true;
}
bool ListAddUnit(PluginVariable *list, AUnit *un)
{
  if(list->Type!=PVT_LIST||list->ElemType!=PVT_UNIT)
    return false;
  list->list->push_back(un);
  return true;
}

int WINAPI ListCopyVar(PluginVariable *to, const PluginVariable *from)
{
  if(!ListCreate(to,from->ElemType))
    return ERROR_BADVARIABLE;
  *(to->list)=*(from->list);
  return 0;
}
int WINAPI ListFreeVar(PluginVariable *var)
{
  delete var->list;
  var->Type=PVT_NONE;
  return 0;
}
int WINAPI ListGetVarProp(PluginVariable *var, int index, PluginVariable *result)
{
  switch(index)
  {
    case 0:
      return CreateIntVariable(var->ElemType,result);
    case 1:
    {
      int lastindex=IndexVars[PVT_LIST].GetLastSetIndex();
      switch(lastindex)
      {
        case -1:
        {
          int value=IndexVars[PVT_LIST].GetVarInt();
          int count=var->list->size();
          if(value<0)
            return CreateIntVariable(count,result);
          if(value>=count)
            return ERROR_BADINDEXVALUE;
          void *ptr=(*var->list)[value];
          result->Type=var->ElemType;
          switch(var->ElemType)
          {
            case PVT_REGION:
              result->reg=(ARegion*)ptr;
            break;
            case PVT_UNIT:
              result->unit=(AUnit*)ptr;
            break;
            default:  return ERROR_BADVARIABLE;
          }
          return 0;
        }
        default:return ERROR_INDEXNOTSET;
      }
    }
  }
  return ERROR_BADPROPERTYINDEX;
}
int WINAPI ListSetVarProp(PluginVariable *var, int index, PluginVariable *value)
{
  switch(index)
  {
    case -1:
      if(!IndexVars[PVT_LIST].SetVar(index,value))
        return ERROR_BADARGUMENTTYPE;
      return 0;
  }    
  return ERROR_BADPROPERTYINDEX;
}

