#ifndef __PLUGINS_H__
#define __PLUGINS_H__

/*
  plugin.h

  Plugin API for AtlaClient
*/
#ifndef _WINDOWS_
#include <windows.h>
#endif

#define MAKEACVERSION(major,minor,release,build) ( ((major)<<12) | ((minor)<<6) | (release) | ((build)<<19))

#ifdef ATLACLIENT
#define ACEXPORT
#else
#define ACEXPORT __declspec (dllexport)
#endif

enum OpenPluginFromType{
  OPEN_MENU=1,
  OPEN_CONFIGURE,
  OPEN_EXPRESSION,
//  OPEN_AFTERRUN,
};
/*
enum PluginFlagsType{
  PF_AFTERRUN=0x0001,
};
*/
enum ExprFuncFlagType{
  EM_REGION=0x0001,
  EM_UNIT=0x0002,
};
enum PluginOrderFlagType{
  PO_NORMAL=1,
  PO_AUTO=2,
  PO_CLIENT=3,
  PO_TYPEMASK=0x0007,
  PO_DOUBLEPASS=0x0008,
  
  PO_RUNMASK=0xff00,
  PO_RUNNONE=0,
  PO_RUNBEFOREFLAGS   =0x0100,
  PO_RUNBEFOREGIVE    =0x0200,
  PO_RUNAFTERGIVE     =0x0300,
  PO_RUNAFTERTAX      =0x0400,
  PO_RUNAFTERCAST     =0x0500,
  PO_RUNAFTERBUY      =0x0600,
  PO_RUNAFTERFORGET   =0x0700,
  PO_RUNAFTERWITHDRAW =0x0800,
  PO_RUNAFTERSAIL     =0x0900,
  PO_RUNAFTERMOVE     =0x0a00,
  PO_RUNAFTERMONTHLONG=0x0b00,
  PO_RUNAFTERRUN      =0x0c00,
};
enum PluginEventsType{
 PE_EXITCLIENT_BASE=0,
 PE_EXITCLIENT_NOBASE,
 PE_LOADGAME,
 PE_SAVEGAME,
 PE_TURNCHANGED,
 PE_LEVELCHANGED,
 PE_REGIONCHANGED,
 PE_TREEITEMCHANGED,
 PE_BEFORESAVEORDER,
 PE_AFTERSAVEORDER,
 PE_ORDERCREATE,
 PE_ORDERDESTROY,
 PE_ORDERTEMPLATESTR,
 PE_ORDERVIEWSTR,
 PE_ORDERRUN,
 PE_ORDEREDIT,

 PE_CALCSTUDYCOST=100000,
 PE_CALCMAINTENANCE,
 PE_CALCUNITMAXLEVEL,
 PE_CALCMOVECOST,
 PE_CALCMOVEPOINTS,
};
enum PluginRunStop{
 PRS_AFTERGIVE=0,
 PRS_AFTERBUY,
 PRS_BEFOREMONTHLONG,
 PRS_BEFOREBUILD,
};

enum PluginVariableType{
  PVT_NONE=0,
  PVT_INTEGER,
  PVT_STRING,
  PVT_TURN,
  PVT_REGION,
  PVT_OBJECT,
  PVT_UNIT,
  PVT_ITEM,
  PVT_SKILL,
  PVT_ORDER,
  PVT_FACTION,
  PVT_GAME,
  PVT_LIST,
  PVT_last
};
enum SkillFlag{
 MAGIC = 0x1,
 COMBAT = 0x2,
 CAST = 0x4,
 FOUNDATION = 0x8,
};
enum ItemFlag{
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
  IT_FOOD = 0x400,
};
enum ErrorType{
  ERROR_BADVARIABLE=-1,
  ERROR_BADPROPERTYINDEX=-2,
  ERROR_BADARGUMENTTYPE=-3,
  ERROR_INDEXNOTSET=-4,
  ERROR_BADINDEXVALUE=-5,
  ERROR_PLUGINNOTLOCKED=-6,
};
#ifndef ATLACLIENT
enum ACOrderTypes{
  O_NONE=0,
  O_END,//not used
  O_UNIT,//not used
  O_CUSTOMORDER,
  O_LOCALDESCR,
  O_COMMENT,
  O_ADDRESS,
  O_ADVANCE,
  O_ASSASSINATE,
  O_ATTACK,
  O_AUTOTAX,
  O_AVOID,
  O_BEHIND,
  O_BUILD,
  O_BUY,
  O_CAST,
  O_CLAIM,
  O_COMBAT,
  O_CONSUME,
  O_DECLARE,
  O_DESCRIBE,
  O_DESTROY,
  O_ENDFORM,//not used
  O_ENTER,
  O_ENTERTAIN,
  O_FACTION,
  O_FIND,
  O_FORGET,
  O_FORM,//not used
  O_FORMEND,//not used
  O_GIVE,
  O_MESSAGE,
  O_GUARD,
  O_HOLD,
  O_LEAVE,
  O_MOVE,
  O_NAME,
  O_NOAID,
  O_OPTION,
  O_PASSWORD,
  O_PILLAGE,
  O_PRODUCE,
  O_PROMOTE,
  O_QUIT,
  O_RESTART,
  O_REVEAL,
  O_SAIL,
  O_SELL,
  O_SHOW,
  O_STEAL,
  O_STUDY,
  O_TAX,
  O_TEACH,
  O_TEACHED,
  O_WORK,
  O_AUTOGIVE,
  O_NOP,
  O_AUTOMOVE,
  O_WITHDRAW,
  O_NOCROSS,
  O_NOSPOILS,
  O_SPOILS,
  O_AUTOWARNING,
  O_TURN,
  O_ENDTURN,
  NORDERS
};
enum ACAttitudes{
    A_HOSTILE=0,
    A_UNFRIENDLY,
    A_NEUTRAL,
    A_FRIENDLY,
    A_ALLY,
    NATTITUDES
};
enum ACMoveTypes{
  M_NONE=0,
  M_WALK,
  M_RIDE,
  M_FLY,
  M_SAIL
};
enum ACMoveDirs{
  MOVE_N=0,
  MOVE_NE,
  MOVE_SE,
  MOVE_S,
  MOVE_SW,
  MOVE_NW,

  MOVE_IN=10,
  MOVE_OUT=11,
/* Enter is MOVE_ENTER + num of object */
  MOVE_ENTER,
};

#endif

struct PluginVariable;

typedef int (WINAPI *ACAPICOPYVARIABLE)(PluginVariable *to, const PluginVariable *from);
typedef int (WINAPI *ACAPIFREEVARIABLE)(PluginVariable *var);
typedef int (WINAPI *ACAPIGETVARIABLEPROPERTY)(PluginVariable *var, int index, PluginVariable *result);
typedef int (WINAPI *ACAPISETVARIABLEPROPERTY)(PluginVariable *var, int index, PluginVariable *value);
typedef int (WINAPI *ACAPIRUNVARIABLEFUNCTION)(PluginVariable *var, int index, PluginVariable *arg);
typedef int (WINAPI *ACAPIGETGLOBALVARIABLE)(int index, PluginVariable * result);
typedef int (WINAPI *ACAPIADDEVENT)(const char *str);
typedef int (WINAPI *ACAPICREATESTRINGVARIABLE)(const char *s, PluginVariable * result);
typedef int (WINAPI *ACAPILOCKPLUGIN)(int ModuleNumber, int locktype, int reserved);

#pragma pack(push,4)
struct PluginStartupInfo
{
int StructSize;
unsigned AtlaClientVersion;
char ModuleName[MAX_PATH];
int ModuleNumber;
ACAPICOPYVARIABLE CopyVariable;
ACAPIFREEVARIABLE FreeVariable;
ACAPIGETVARIABLEPROPERTY GetVariableProperty;
ACAPISETVARIABLEPROPERTY SetVariableProperty;
ACAPIRUNVARIABLEFUNCTION RunVariableFunction;
ACAPIGETGLOBALVARIABLE GetGlobalVariable;
ACAPIADDEVENT AddEvent;
ACAPICREATESTRINGVARIABLE CreateStringVariable;
ACAPILOCKPLUGIN LockPlugin;
};

struct PluginInfo
{
  int StructSize;
  DWORD Flags;
  char **PluginMenuStrings;
  int PluginMenuStringsNumber;
  char **PluginConfigStrings;
  int PluginConfigStringsNumber;
  char **ExprFuncsStrings;
  int *ExprFuncsFlags;
  int ExprFuncsStringsNumber;
  int *SpecEvents;
  int SpecEventsNumber;
  char **PluginOrderStrings;
  int *PluginOrderFlags;
  int PluginOrderStringsNumber;
  int *PluginMenuLevels;
};

#ifndef ATLACLIENT
struct PluginVariable{
  int Type;
  int value;
  const char *str;
  int reserved;
  PluginVariable():Type(PVT_NONE){};
};
#ifndef _MSC_VER
#if sizeof(PluginVariable)!=16
#error Bad alligment: sizeof(PluginVariable)!=16
#endif
#endif
#endif

struct CalcMoveCostParams{
  int dir;
  int turnnum;
  int movetype;
};

struct EvPluginOrder
{
  int ordernum;
  char uuid[8];
  const char* str;
  int flags;
  char retstr[2048-20];
};
#ifndef _MSC_VER
#if sizeof(EvPluginOrder)!=2048
#error Bad alligment: sizeof(EvPluginOrder)!=2048
#endif
#endif

#pragma pack(pop)

extern "C"{

ACEXPORT int WINAPI SetStartupInfo(const struct PluginStartupInfo *Info);
ACEXPORT void WINAPI GetPluginInfo(struct PluginInfo *Info);
ACEXPORT HANDLE WINAPI OpenPlugin(int OpenFrom,int Item);
ACEXPORT int WINAPI ProcessEvent(int event, void *param);

};
#endif
