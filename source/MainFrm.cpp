//todo:Не законченные корабли
//		1. при ордере BUILD - проверка на наличие unfinished в инвентори
// 		2. Вес корабля
// 		3. Проверка наличия объекта для айтема

//todo:Fleet - поддержка
//		1. У объектов - флаг флота. Для флотов - capalcity & need Sailors. + Max Speed
//		2. При чтении репы - ставить флаг флота и инфу заполнять о флоте
// 		3. При ордерах - флот обрабатывать как корабль но учитывать не свойства объекта а св-ва текущего флота

//todo:Sharing - Проверка, когда все хексы смотрится... Особенно еслиф это не тот левел...
//todo:New Order - Предложить поместить его в TURN...

//todo:Receive Typed Items - может более подробнее??
//		weapon : ranged, weak (tools), ближнего боя, мифрильное или лучше
//		armor: слабое (кожа и хуже), средней (плейт, кольчуга), мифрильное или лучше
//

//todo:Give ALL typed Items

//todo 2:Create New Form for Edit Items - Почти всё....
//New Form for Edit Items - при загрузке репорта!!!

//todo 2:Capalcity for Get order
//todo 2:Calculator in Give-Receive Order  -- Need RxLib

//todo 5 :Specials in Battle
//   Store
//   Edit
//   Use in Battle

//todo 5 :TURN order
//   Add New Order
//     1. Give
//       a) Выбор все итемы - имеющиеся - на конец хода
//       b) Регион - этот же, на конец хода, конкретный, все юниты(?), номер руками
//     2. Teach
//     3. {O_ASSASSINATE,"assassinate "},
//     4. {O_STEAL,"steal "},
//     5. {O_ATTACK,"attack "},
//     6. {O_COMBAT,"combat"},
//     7. {O_PRODUCE,"produce "},
//     8. {O_STUDY,"study "}

//todo:Build Help <unit>

//todo:сделать линковку шахт на целевой регион (а не на шахту)
//todo:редактировать приказы всех юнитов в данном гексе.
//todo:поиск юнита в предыдущих ходах.
//todo:Настройку для бегства по хитам в BSIM.
//todo:Wedge: BattleInRegion
//todo:добавить в edit cast чтение списка параметров
//todo:Будет очень удобно, когда юнит - владелец объекта будет помечаться в списке юнитов, находящихся внут
//todo:Как  на  счет  того, чтоб в таблице Units сделать возможным копировать > содержимое ячеек и суммарной ячейки?
//todo:Указание, что "голодающий" юнит двигается
//todo:таблица юнитов - контекстную менюшку с приказами....
//todo:сортировку events'ов по типам
//todo:показ разных типов движения разными цветами

//todo:-> Собственно новый autoorder хачу, что-то типа
//  @;!collect [item1], [item2], ...,[itemN]
//  То есть сграбастать всё вещи указанных типов.

//todo:Что знать обокрали тебя или нет приходится каждый раз лезть в реп, сделай какойнибудь значек, чтобы было видно на карте.
//todo:Сейчас места с чудиками отмечаются черепом. Но при этом не понятно есть там чудики или нет....
//todo:Добавить команду AutoBuy. Причем предусмотреть, покупать постоянно или до некого предела.
//todo:В дереве региона хорошо бы сортировать народ по фракциям, непривязанный народ так и оставлять в самом регионе,
//     а фракции > выполнить отдельными подветвями. а то утомляет выискивать свиих рабочих среди чужих :)
//todo:цвета для приказов

//todo:commands:
//     address ?
//     faction --
//     find ?
//     option --
//     password --
//     quit ?
//     restart ?
//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdio.h>
#include <clipbrd.hpp>
#include <stdlib.h>
#include <math.h>
#include <memory>
#include "util.h"
#include "MainFrm.h"
#include "NewGameFrm.h"
#include "turn.h"
#include "ChangeGameFrm.h"
#include "reports.h"
//#include "ShowSkillTypesFrm.h"
#include "ShowItemTypesFrm.h"
#include "ShowObjTypesFrm.h"
//#include "ShowSkillInfosFrm.h"
#include "region.h"
#include "viewitems.h"
#include "faction.h"
#include "unit.h"
#include "LinkShaftFrm.h"
#include "orders.h"
#include "EventsFrm.h"
#include "markets.h"
#include "ProdModeSetupFrm.h"
#include "skills.h"
#include "WedgeModeSetupFrm.h"
#include "exprlang.h"
#include "StoreFrm.h"
#include "RegInfoFrm.h"
#include "MapFrm.h"
#include "import.h"
#include "OptionsFrm.h"
#include "EditOrderReceiveFrm.h"
#include "AboutFrm.h"
#include "FormNewUnitFrm.h"
#include "SaveMapFrm.h"
#include "BattleSimFrm.h"
#include "ShowWeatherRegionsFrm.h"
#include "SetFlagsFrm.h"
#include "EditOrdersFrm.h"
#include "ViewFactionsFrm.h"
#include "ViewMagesFrm.h"
#include "ViewTaxRegionsFrm.h"
#include "changes.h"
#include "SetLevelSizeFrm.h"
#include "items.h"
#include "ViewGatesFrm.h"
#include "RegionListFrm.h"
#include "SupportBuildingFrm.h"
#include "ViewSpecialistsFrm.h"
#include "ViewUnitsFrm.h"
#include "smapi.h"
#include "ShortCutsFrm.h"
#include "acplugins.h"
#include "plugdoc\plugins.h"
#include "BalanceFrm.h"
#include "ProductsFrm.h"
#include "ShowItObjInfosFrm.h"
#include "AnalyzeCityRegion.h"
#include "shortcuts.h"
#include "gameparams.h"
#include "ExpressionEditorFrm.h"
//#include "ShowWeapFrm.h"
//#include "ShowArmsFrm.h"
#include "TownTrade.h"
#include "FlagFiltr.h"
#include "SkillsTree.h"
#include "SelEditScr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//#define DRAWMAPTIMING

TAtlaForm *AtlaForm;

AnsiString ClientPath;
AnsiString TempPath;
AnsiString IniName="AtlaClient.ini", ScriptsName="Scripts.ini";
const char*IniSect="games", *SettingsSect="settings",*ExprCommsSect="wedges"
 ,*WarningExprsSect="warnings",*EventsWarningsSect="eventwarnings"
 ,*ViewUnitsSect="viewunits",*PluginsStrSect="pluginsstr";
int GridXs[7]={
 -12,11,23,11,-12,-24,-12
};
int GridYs[7]={
 -20,-20,0,20,20,0,-20
};
int PoligXs[7]={
 -12,11,23,11,-12,-24,-12
};
int PoligYs[7]={
 -20,-20,0,20,20,0,-20
};
TColor curregColor=clRed,wallColor=clWhite/*clLtGray*/;
TColor gridColor=clBlack,textColor=clYellow,roadColor=(TColor)0x282864;
static int curregGrids[NDIRS]={3,3,3,3,3,3};
TPopupMenu *TreeViewPopupMenu = 0;
// 1 - food, 2 - normal, 4 - adv, 8 - show null, 16 - race
unsigned char ProdModeAttr;

int HasTowerIndex=-1,HasBuildingIndex=-1, HasShaftIndex=-1, HasLairIndex=-1, HasShipIndex=-1, HasBattleIndex=-1;
int InRegListIndex=-1;
int UnExploredIndex=-1;
int UnVisitedIndex=-1;
static int MakingTree=0;
static bool ShowUnitsOnStart=false;
AOrder* orderBuf;
TStringList *EventsWarnings=0,*ViewUnitsCols=0,*ViewUnitsSets=0;

AnsiString LockFName="atlaclient.lock";
HANDLE hLock=INVALID_HANDLE_VALUE;

bool NowRegShow = false;
bool RemoveLastTurn = false;
bool onSaveNow = false;
bool NeedDraw = true;

TarFlags arFiltrFlags;

void SetEnglish()
{
  const bufsize=10;
  HKL klays[bufsize];
  int numklays=GetKeyboardLayoutList(bufsize,klays);
  if(!numklays) return;
  for(int i=0;i<numklays;i++){
   if((unsigned(klays[i])&0xffff)==0x409){
    ActivateKeyboardLayout(klays[i],0);
	return;
   }
  }
}
//---------------------------------------------------------------------------
__fastcall TAtlaForm::TAtlaForm(TComponent* Owner)
    : TForm(Owner),NoSave(false)
{
// SetEnglish();
 vtreenodes=new VTreeNodes;
 vlistitems=new VListItems;
 EventsWarnings=new TStringList;
 ViewUnitsCols=new TStringList;
 ViewUnitsSets=new TStringList;
 ClientPath=ExtractFilePath(Application->ExeName);
 IniName=ClientPath+IniName;
 ScriptsName=ClientPath+ScriptsName;
 {
  char buf[MAX_PATH+1];
  GetTempPath(sizeof(buf),buf);
  TempPath=buf;
 }
 MapMode=0;
 ProdModeAttr=31;

 miTemporary->Visible=false;
 InitSimpleMAPI();
 TAboutForm::UpdateVersionString();
 InitPlugins();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::FormShow(TObject *Sender)
{
 AnsiString fname=ClientPath+LockFName;
 DeleteFile(fname.c_str());
 hLock=CreateFile(fname.c_str(),GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_FLAG_DELETE_ON_CLOSE,NULL);
 if(hLock==INVALID_HANDLE_VALUE){
  ShowMessage("AtlaClient already running");
  NoSave=true;
  curGame.SetLength(0);
  PostMessage(Handle, WM_CLOSE, 0, 0);
  return;
 }

 LoadOptions();
 LoadGameBase();
 PanelMapResize(this);
}
//---------------------------------------------------------------------------
__fastcall TAtlaForm::~TAtlaForm()
{
 DeleteGameBase();

 delete Games;

 delete vtreenodes;
 delete vlistitems;
 if(curGame.Length())
   Plugins.ExitAtlaClient(0);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
/* if(TerrainTypes->modified){
  int i=Application->MessageBox("Terrain types modified\nSave it?","Warning",MB_YESNOCANCEL);
  if(i==IDCANCEL){
   CanClose=false;
  }else if(i==IDYES){
   TerrainTypes->Save();
  }
 }
 if(WeatherTypes->modified){
  int i=Application->MessageBox("Weather types modified\nSave it?","Warning",MB_YESNOCANCEL);
  if(i==IDCANCEL){
   CanClose=false;
  }else if(i==IDYES){
   WeatherTypes->Save();
  }
 }
 if(SkillTypes->modified){
  int i=Application->MessageBox("Skill types modified\nSave it?","Warning",MB_YESNOCANCEL);
  if(i==IDCANCEL){
   CanClose=false;
  }else if(i==IDYES){
   SkillTypes->Save();
  }
 }
 if(ItemTypes->modified){
  int i=Application->MessageBox("Item types modified\nSave it?","Warning",MB_YESNOCANCEL);
  if(i==IDCANCEL){
   CanClose=false;
  }else if(i==IDYES){
   ItemTypes->Save();
  }
 }
 if(TownTypes->modified){
  int i=Application->MessageBox("Town types modified\nSave it?","Warning",MB_YESNOCANCEL);
  if(i==IDCANCEL){
   CanClose=false;
  }else if(i==IDYES){
   TownTypes->Save();
  }
 }*/
// SetEnglish();
 if(curGame.Length())
   Plugins.ExitAtlaClient(1);
 if(NoSave) return;
 if(ViewUnitsForm)
   ViewUnitsForm->SaveColWidths();
 SaveOptions();
 SaveGameBase();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::Exit1Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
void TAtlaForm::MakeScrMenu()
{
  TIniFile *iniS = new TIniFile(ScriptsName);
  int i, cc, crows;
  cc = iniS->ReadInteger("Scripts", "count", 0);

  while (miExecScr->Count > 0) miExecScr->Delete(0);
  AllScripts->Clear();
  AnsiString str;

  for(i = 0; i < cc; i++)
  {
	AScript *scr = new AScript;
	AnsiString Sec = "Script";
	Sec += i;
	TMenuItem *mi = new TMenuItem(miExecScr);
	mi->Caption = iniS->ReadString(Sec, "caption", i);
	mi->Tag = i;
	mi->OnClick = ExecScriptClick;
	miExecScr->Add(mi);

	scr->caption  = mi->Caption;
	scr->CountStr = iniS->ReadInteger(Sec, "countstr", 0);
	scr->NeedClear= iniS->ReadBool(Sec, "needclear", false);
	for(int ii=0; ii<scr->CountStr; ii++)
	{
	  str = "str";
	  str += ii;
	  str = iniS->ReadString(Sec, str, "");
	  if(str != "") scr->Strings->Add(str);
	}

	AllScripts->Add(scr);
  }
  delete iniS;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::FormCreate(TObject *Sender)
{
  AllScripts = new AScripts;
  int i;
  facdiap = -2;
  FskilFiltr = "";
  FmoFiltr = 0;
  Games=new TStringList;
  TIniFile *ini=new TIniFile(IniName);
  curGame=ini->ReadString(IniSect,"current","");
 // actShowMap->Checked=atoi(ini->ReadString(SettingsSect,"ShowMap","").c_str());
 // actShowRegInfo->Checked=atoi(ini->ReadString(SettingsSect,"ShowRegInfo","").c_str());
 TStringList*slst=new TStringList;
 ini->ReadSectionValues(IniSect,slst);
 for(i=0;i<slst->Count;i++){
  AnsiString str=slst->Strings[i];
  if(str.Pos("game")!=1)continue;
  int pos=str.Pos("=");
  if(pos==0)continue;
  Games->Add(str.SubString(pos+1,str.Length()-pos));
 }
 delete slst;
 miGameChange->Enabled=(Games->Count>1);
 for(int i=1;i<=ParamCount();i++)
 {
   AnsiString str=ParamStr(i);
   if(!MyCompare(str.c_str(),"-g:"))
   {
     char *s=str.c_str()+3;
	 str=s;
     curGame=str;
   }
   if(!str.AnsiCompareIC("-rlt"))
      RemoveLastTurn=true;
 }
 for(i=0;i<Games->Count;i++){
  if(Games->Strings[i]==curGame)break;
 }
 if(i==Games->Count) curGame="";
 if(!curGame.Length())
   miGameChange->Enabled=(Games->Count>0);
 delete ini;
 MapImages->Width=MapImageWidth;
 MapImages->Height=MapImageHeight;
// AddMapImage("null.bmp");
 OpenDialog->InitialDir=ClientPath;
 OpenDialog->Filter="All files|*.*";

 HasTowerIndex=AddInfoImage("tower.bmp");
 HasBuildingIndex=AddInfoImage("building.bmp");
 HasShaftIndex=AddInfoImage("shaft.bmp");
 HasLairIndex=AddInfoImage("lair.bmp");
 HasShipIndex=AddInfoImage("ship.bmp");
 HasBattleIndex=AddInfoImage("battle.bmp");

 actShowEvents->ImageIndex=AddToolBarImage("showevents.bmp");
 actRunOrders->ImageIndex=AddToolBarImage("runorders.bmp");
 actRunAutoOrders->ImageIndex=AddToolBarImage("runautoorders.bmp");
 actTownMode->ImageIndex=AddToolBarImage("townmode.bmp");
 actProdMode->ImageIndex=AddToolBarImage("prodmode.bmp");
 actWedgeMode->ImageIndex=AddToolBarImage("wedgemode.bmp");
 TrayIcon->IconIndex=bnWedgeMode->ImageIndex;
 actLevelUp->ImageIndex=AddToolBarImage("levelup.bmp");
 actLevelDown->ImageIndex=AddToolBarImage("leveldown.bmp");
 actPrevTurn->ImageIndex=AddToolBarImage("turnprev.bmp");
 actNextTurn->ImageIndex=AddToolBarImage("turnnext.bmp");
 btInfMode->ImageIndex=AddToolBarImage("silvinfo.bmp");

 actPrevUnit->ImageIndex=actPrevTurn->ImageIndex;
 actNextUnit->ImageIndex=actNextTurn->ImageIndex;
 actPrevExprUnit->ImageIndex=actPrevTurn->ImageIndex;
 actNextExprUnit->ImageIndex=actNextTurn->ImageIndex;

 miAddOrderCustom->Tag=O_CUSTOMORDER;
 miAddOrderComment->Tag=O_COMMENT;
 miAddOrderLocDescr->Tag=O_LOCALDESCR;
 miAddOrderGive->Tag=O_GIVE;
 miAddOrderCast->Tag=O_CAST;
 miAddOrderTeach->Tag=O_TEACH;
 miAddOrderBuild->Tag=O_BUILD;
 miAddOrderNameDescribe->Tag=O_NAME;
 miAddOrderSteal->Tag=O_STEAL;
 miAddOrderAssassinate->Tag=O_ASSASSINATE;
 miAddOrderAttack->Tag=O_ATTACK;
 miAddOrderAutoGive->Tag=O_AUTOGIVE;
 miAddOrderAutoMove->Tag=O_AUTOMOVE;
 miAddOrderAutoWarning->Tag=O_AUTOWARNING;

  miAdvMO->Tag = O_ADVANCE;
  miBuiMO->Tag = O_BUILD;
  miEntMO->Tag = O_ENTERTAIN;
  miMovMO->Tag = O_MOVE;
  miProMO->Tag = O_PRODUCE;
  miSaiMO->Tag = O_SAIL;
  miStuMO->Tag = O_STUDY;
  miTeaMO->Tag = O_TEACH;
  miWorMO->Tag = O_WORK;

 miReceive->Tag=-1;
 miSetFlags->Tag=-2;
 miAddOrderWithdrawClient->Tag=-3;
 miAddOrderFormNew->Tag=-4;
 miCopyNewUnit->Tag=-5;

 miAddOrderDestroy->Tag=O_DESTROY;
 miAddOrderEntertain->Tag=O_ENTERTAIN;
 miAddOrderPillage->Tag=O_PILLAGE;
 miAddOrderTax->Tag=O_TAX;
 miAddOrderWork->Tag=O_WORK;

 miAddOrderNOP->Tag=O_NOP;

 miAddOrderAutotax->Tag=O_AUTOTAX;
 miAddOrderAvoid->Tag=O_AVOID;
 miAddOrderBehind->Tag=O_BEHIND;
 miAddOrderGuard->Tag=O_GUARD;
 miAddOrderHold->Tag=O_HOLD;
 miAddOrderNoaid->Tag=O_NOAID;
 miAddOrderReveal->Tag=O_REVEAL;
 miAddOrderConsume->Tag=O_CONSUME;
 miAddOrderNoCross->Tag=O_NOCROSS;
 miAddOrderNoSpoils->Tag=O_NOSPOILS;
 miAddOrderSpoils->Tag=O_SPOILS;
 miAddOrderSharing->Tag=O_SHARING;

 miAddOrderCombat->Tag=O_COMBAT;
 miAddOrderForget->Tag=O_FORGET;
 miAddOrderProduce->Tag=O_PRODUCE;
 miAddOrderStudy->Tag=O_STUDY;

 miAddOrderBuy->Tag=O_BUY;
 miAddOrderSell->Tag=O_SELL;

 miAddOrderClaim->Tag=O_CLAIM;
 miAddOrderWithdraw->Tag=O_WITHDRAW;

 miAddOrderLeave->Tag=O_LEAVE;
 miAddOrderEnter->Tag=O_ENTER;
 miAddOrderPromote->Tag=O_PROMOTE;
 miAddOrderMove->Tag=O_MOVE;
 miAddOrderAdvance->Tag=O_ADVANCE;
 miAddOrderSail->Tag=O_SAIL;

 MakeScrMenu();

 InitShortCuts();
 GameParams.Init();

 ShowMapMode();
}
//---------------------------------------------------------------------------
void TAtlaForm::SaveGameList()
{
 TIniFile *ini=new TIniFile(IniName);
 ini->EraseSection(IniSect);
 for(int i=0;i<Games->Count;i++){
  ini->WriteString(IniSect,AnsiString("game")+i,Games->Strings[i]);
 }
 delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::FormDestroy(TObject *Sender)
{
 SaveGameList();
 if(!NoSave)
   ShortCuts.Save();

 TIniFile *ini=new TIniFile(IniName);
 ini->WriteString(IniSect,"current",curGame);
// ini->WriteString(SettingsSect,"ShowMap",int(actShowMap->Checked));
// ini->WriteString(SettingsSect,"ShowRegInfo",int(actShowRegInfo->Checked));
 delete ini;
// delete curTurns;
 delete EventsWarnings;
 delete ViewUnitsCols;
 delete ViewUnitsSets;
 if(hLock!=INVALID_HANDLE_VALUE)
   CloseHandle(hLock);
 DeinitSimpleMAPI();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miGameNewClick(TObject *Sender)
{
 TNewGameForm *ngf=new TNewGameForm(this);
 ngf->ShowModal();
 delete ngf;
 VerGameLoaded();
 MapForm->ResetMap();
 ChangeTurn(0,false,true);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miGameChangeClick(TObject *Sender)
{
 if(ViewUnitsForm)
   ViewUnitsForm->SaveColWidths();
 TChangeGameForm *cgf=new TChangeGameForm(this);
 cgf->ShowModal();
 delete cgf;
}
void TAtlaForm::VerGameLoaded()
{
 bool loaded=true;
 if(!curGame.Length())loaded=false;
 if(!curTurns)loaded=false;
// N2->Enaible=loaded;
 miLoadReport->Enabled=loaded;
 miLoadOrder->Enabled=loaded;
 miLoadFromClipboard->Enabled=loaded;
 miSaveOrder->Enabled=loaded;
 miSaveReport->Enabled=loaded;
 miSaveMap->Enabled=loaded;


// miPackTurns->Enabled=loaded;
//- need restore
 miStoreTurns->Enabled=loaded;
 miImport->Enabled=loaded;

 miView->Visible=loaded;
 miAnalyze->Visible=loaded;
 miFix->Visible=loaded;
 miMail->Visible=loaded;
 miPlugins->Visible=loaded;

 PanelMain->Visible=loaded;

 if(loaded) BattleSimForm->FormShow(0);
}

void __fastcall TAtlaForm::miLoadReportClick(TObject *Sender)
{
 curTurns->StoreLastParam();
 OpenDialog->Filter="All files|*.*";
 if(LoadReportsDir.Length()){
  OpenDialog->InitialDir=LoadReportsDir;
  if(ExtractFilePath(OpenDialog->FileName)!=LoadReportsDir)
   OpenDialog->FileName="";
 }
 if(OpenDialog->Execute()){
  LoadReportsDir=ExtractFilePath(OpenDialog->FileName);

  bool ok=true;
  TStringList *list=new TStringList;
  list->Assign(OpenDialog->Files);
  list->Sort();
  for(int i=0;ok&&i<list->Count;i++){
   if(!ProcessReport(list->Strings[i])) ok=false;
  }
  delete list;
  ChangeTurn(0,false,true);

  miAdvProdsClick(0);
 }
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miShowItemTypesClick(TObject *Sender)
{
  if(!ShowItemTypesForm) ShowItemTypesForm = new TShowItemTypesForm(this);
  ShowItemTypesForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miShowItObjReportsClick(TObject *Sender)
{
 TShowItObjInfosForm *sioi=new TShowItObjInfosForm(this);
 sioi->Show();
}
//---------------------------------------------------------------------------
static ARegion *FindRgn(ARegion **beg, ARegion **end,int x,int y,int z)
{
  while(beg<end){
	ARegion *reg=*beg;
	if(reg->yloc!=y||reg->zloc!=z) break;
	if(reg->xloc==x)
	  return reg;
	beg++;                     
  }                                   
  return 0;
}
//---------------------------------------------------------------------------
void TAtlaForm::DrawMap()
{
 TCanvas *Canv=ImageMap->Canvas;
 Canv->Brush->Color=clLtGray;
 TRect r(0,0,ImageMap->Width,ImageMap->Height);
 Canv->FillRect(r);
 if(!curRegion)return;
#ifdef DRAWMAPTIMING
 long t0=GetTickCount();
#endif

  int xc=ImageMap->Width/2,yc=ImageMap->Height/2;
  int xloc=xcentr,yloc=ycentr,zloc=zcentr;
  int bx=xloc-xc/MapDX-1,by=yloc-yc/MapDY-1;
  curRegionArray->NormCoord2(bx,by);
  int ex=xloc+xc/MapDX+1,ey=yloc+yc/MapDY+1;
  curRegionArray->NormCoord2(ex,ey);

  int _x,_y;
  ARegion **beg,**end,**zbeg;
  zbeg=(ARegion**)curRegionList->List;
  end=zbeg+curRegionList->count;
  while(zbeg<end&&(*zbeg)->zloc!=zloc) zbeg++;
  beg=zbeg;
  for(int y=by;y<=ey;y++)
  {
	while(beg<end&&(*beg)->zloc==zloc&&(*beg)->yloc<y) beg++;
	for(int x=bx;x<=ex;x++)
	{
	  if((x+y)&1)continue;
	  int xx=x;
	  curRegionArray->NormCoord(xx,y);
	  _x=xc+MapDX*(x-xloc);
	  _y=yc+MapDY*(y-yloc);
	  ARegion *reg=FindRgn(beg,end,xx,y,zloc);
	  DrawRegion(Canv,reg,xx,y,zloc,_x,_y);
	}
  }
  
#ifdef DRAWMAPTIMING
 long t1=GetTickCount();
#endif
 beg=zbeg;
 bool bold=zloc>1&&optBoldUndergroundGrid;
 for(int y=by;y<=ey;y++){
  while(beg<end&&(*beg)->zloc==zloc&&(*beg)->yloc<y)
    beg++;
  for(int x=bx;x<=ex;x++){
   if((x+y)&1)continue;
   _x=xc+MapDX*(x-xloc);
   _y=yc+MapDY*(y-yloc);
   int xx=x;
   curRegionArray->NormCoord(xx,y);
   ARegion *reg=FindRgn(beg,end,xx,y,zloc);
   if(!reg) continue;
   if(!reg->hasInfo) continue;
   int grids[NDIRS];
   for(int dir=0;dir<NDIRS;dir++){
    grids[dir]=reg->neighbors[dir].xloc==-1?1:(zloc>1?2:0);
   }
   DrawGrid(Canv,_x,_y,grids,bold);
  }
 }
#ifdef DRAWMAPTIMING
 long t2=GetTickCount();
#endif
 int maxx=curRegionArray->x;
 if(maxx&1) maxx++;
 int xx=curRegion->xloc-xloc;
 if(xx<-(maxx/2)) xx+=maxx;
 if(xx>maxx/2) xx-=maxx;
 _x=xc+MapDX*(xx);
 _y=yc+MapDY*(curRegion->yloc-yloc);
 DrawGrid(Canv,_x,_y,curregGrids,false);
 Canv->Font->Color=textColor;
 Canv->Font->Name="Arial";
 Canv->Brush->Style=bsClear;

  if(MapMode==0){
    Canv->Font->Size=-11;
    beg=zbeg;
    for(int y=by;y<=ey;y++){
      while(beg<end&&(*beg)->zloc==zloc&&(*beg)->yloc<y)
		beg++;
      for(int x=bx;x<=ex;x++){
        if((x+y)&1)continue;
        int xx=x;
        curRegionArray->NormCoord(xx,y);
        _x=xc+MapDX*(x-xloc);
        _y=yc+MapDY*(y-yloc);
        ARegion *reg=FindRgn(beg,end,xx,y,zloc);
        if(!reg) continue;
        DrawRegionTown(Canv,reg,_x,_y);
      }
    }
  } else if(MapMode==1){
    Canv->Font->Size=-9;
    beg=zbeg;
    for(int y=by;y<=ey;y++){
      while(beg<end&&(*beg)->zloc==zloc&&(*beg)->yloc<y)
        beg++;
      for(int x=bx;x<=ex;x++){
        if((x+y)&1)continue;
        int xx=x;
        curRegionArray->NormCoord(xx,y);
        _x=xc+MapDX*(x-xloc);
        _y=yc+MapDY*(y-yloc);
        ARegion *reg=FindRgn(beg,end,xx,y,zloc);
        if(!reg) continue;
        if(!reg->hasInfo) continue;
        DrawRegionProd(Canv,reg,_x,_y);
      }
    }
  } else if(MapMode==2){
    beg=zbeg;
    for(int y=by;y<=ey;y++){
      while(beg<end&&(*beg)->zloc==zloc&&(*beg)->yloc<y)
        beg++;
      for(int x=bx;x<=ex;x++){
        if((x+y)&1)continue;
        int xx=x;
        curRegionArray->NormCoord(xx,y);
        _x=xc+MapDX*(x-xloc);
        _y=yc+MapDY*(y-yloc);
        ARegion *reg=FindRgn(beg,end,xx,y,zloc);
        if(!reg) continue;
        DrawRegionWedge(Canv,reg,_x,_y);
      }
    }
  } else if(MapMode == 3){
    beg=zbeg;
    for(int y=by;y<=ey;y++){
      while(beg<end&&(*beg)->zloc==zloc&&(*beg)->yloc<y)
		beg++;
      for(int x=bx;x<=ex;x++){
        if((x+y)&1)continue;
        int xx=x;
        curRegionArray->NormCoord(xx,y);
        _x=xc+MapDX*(x-xloc);
        _y=yc+MapDY*(y-yloc);
        ARegion *reg=FindRgn(beg,end,xx,y,zloc);
        if(!reg) continue;
        DrawRegionInfoI(Canv, reg, _x, _y);
      }
    }
  }

 if(MapMode==0||optAlwaysShowMovement){
   if(!curUnit&&GetKeyState(VK_SHIFT)<0)
   {
    beg=zbeg;
    for(int y=by;y<=ey;y++){
    while(beg<end&&(*beg)->zloc==zloc&&(*beg)->yloc<y)
       beg++;
     for(int x=bx;x<=ex;x++){
      if((x+y)&1)continue;
      int xx=x;
      curRegionArray->NormCoord(xx,y);
      _x=xc+MapDX*(x-xloc);
      _y=yc+MapDY*(y-yloc);
      ARegion *reg=FindRgn(beg,end,xx,y,zloc);
	  if(!reg) continue;
	  reg->RunOrders(false);
      DrawRegionMove(Canv,reg,x,y,zloc,_x,_y);
     }
    }
   }
   else
   {
     int x=curRegion->xloc;
     int y=curRegion->yloc;
     int xx=x-xloc;
     if(xx<-(maxx/2)) xx+=maxx;
     if(xx>maxx/2) xx-=maxx;
     _x=xc+MapDX*(xx);
     _y=yc+MapDY*(y-yloc);
     DrawRegionMove(Canv,curRegion,x,y,zloc,_x,_y);
   }
 }
#ifdef DRAWMAPTIMING
 long t3=GetTickCount();
 AnsiString str=t3-t0;
 str+="=";
 str+=t1-t0;
 str+="+";
 str+=t2-t1;
 str+="+";
 str+=t3-t2;
 AddEvent(str);
#endif
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::PanelMapResize(TObject *Sender)
{
 TBmp *bmp=ImageMap->Picture->Bitmap;
 if(!bmp)return;
 bmp->Width=ImageMap->Width;
 bmp->Height=ImageMap->Height;
 MapForm->ReCalcRect();
 MapForm->ApplyDockType();
 DrawMap();
}
//---------------------------------------------------------------------------
void TAtlaForm::DrawRegion(TCanvas *Canv,ARegion *reg,int x,int y,int z, int xc, int yc)
{
 TPoint p[6];
 for(int i=0;i<NDIRS;i++){
  p[i].x=PoligXs[i]+xc;
  p[i].y=PoligYs[i]+yc;
 }
 Canv->Pen->Color=gridColor; //clBlack;
 ATerrainType *ttype=0;
 if(reg){
  ttype=TerrainTypes->Get(reg->type);
 }
 Canv->Brush->Style=bsSolid;
 if(ttype){
  if(optHideGrid)
    Canv->Pen->Color=ttype->color;
  int imgind=ttype->GetImageIndex(reg->nxtweather);
  if(imgind<0)
    Canv->Brush->Color=ttype->color;
  else{
    MapImages->Draw(Canv,xc-MapImageWidth/2,yc-MapImageHeight/2,imgind,true);
    Canv->Brush->Style=bsClear;
  }
 }else{
  if(UnExploredIndex<0)
    Canv->Brush->Color=clWhite;
  else{
    MapImages->Draw(Canv,xc-MapImageWidth/2,yc-MapImageHeight/2,UnExploredIndex,true);
    Canv->Brush->Style=bsClear;
  }
 }
 Canv->Polygon(p,5);
 Canv->Brush->Style=bsSolid;
 if(!reg) return;
 if(UnVisitedIndex>=0&&!reg->hasInfo)
   MapImages->Draw(Canv,xc-MapImageWidth/2,yc-MapImageHeight/2,UnVisitedIndex,true);
 if(RegionListForm->Visible)
   if(InRegListIndex>-1&&reg->hasInfo)
     if(RegionListForm->InRegionList(x,y,z))
       MapImages->Draw(Canv,xc-MapImageWidth/2,yc-MapImageHeight/2,InRegListIndex,true);
}
//---------------------------------------------------------------------------
void TAtlaForm::DrawRegionTown(TCanvas * Canv,ARegion *reg, int xc, int yc)
{
  if(!reg)return;
  ARegionStatus *rs = reg->RegionStatus;
  { // Roads
	Canv->Pen->Color = roadColor;
	int road = rs->HasRoad, goodroad = rs->HasGoodRoad;
	if(goodroad&1) Canv->Pen->Style=psSolid;
	  else Canv->Pen->Style = psDot;
	if(road&1){ Canv->MoveTo(xc,yc-19); Canv->LineTo(xc,yc); }
    if(goodroad&2) Canv->Pen->Style = psSolid;
      else Canv->Pen->Style = psDot;
    if(road&2){ Canv->MoveTo(xc+17,yc-10); Canv->LineTo(xc,yc); }
    if(goodroad&4) Canv->Pen->Style = psSolid;
      else Canv->Pen->Style = psDot;
    if(road&4){ Canv->MoveTo(xc+17,yc+10); Canv->LineTo(xc,yc); }
    if(goodroad&8) Canv->Pen->Style = psSolid;
      else Canv->Pen->Style = psDot;
    if(road&8){ Canv->MoveTo(xc,yc+19); Canv->LineTo(xc,yc); }
    if(goodroad&16) Canv->Pen->Style = psSolid;
      else Canv->Pen->Style = psDot;
    if(road&16){ Canv->MoveTo(xc-17,yc+10); Canv->LineTo(xc,yc); }
    if(goodroad&32) Canv->Pen->Style = psSolid;
      else Canv->Pen->Style = psDot;
    if(road&32){ Canv->MoveTo(xc-17,yc-10); Canv->LineTo(xc,yc); }
    Canv->Pen->Style = psSolid;
  }

  if(reg->townname.Length())
  { // Town
    ATownType *tt = TownTypes->Get(reg->towntype);
    if(tt&&tt->imgnum!=-1)
    {
      InfoImages->Draw(Canv,xc-7,yc-18,tt->imgnum,true);
    }
    AnsiString name=reg->townname;
    if(optTownNameLimit>0)
    {
      if(name.Length()>optTownNameLimit)
      name.SetLength(optTownNameLimit);
    }
	TSize s=Canv->TextExtent(name);
	Canv->TextOut(xc-s.cx/2,yc,name);
  }

  if(rs->HasTower) InfoImages->Draw(Canv,xc+2,yc-18,HasTowerIndex,true);
  if(rs->HasBuilding) InfoImages->Draw(Canv,xc+2,yc-8,HasBuildingIndex,true);
  if(rs->HasShaft) InfoImages->Draw(Canv,xc-17,yc-8,HasShaftIndex,true);
  if(rs->HasLair) InfoImages->Draw(Canv,xc+11,yc-8,HasLairIndex,true);
  if(rs->HasShip) InfoImages->Draw(Canv,xc-7,yc-8,HasShipIndex,true);
  if(reg->HasBattles()) InfoImages->Draw(Canv,xc-7,yc+10,HasBattleIndex,true);

  if(reg->PresentLocals())
  {
	Canv->Pen->Color=textColor;
	Canv->MoveTo(xc-12,yc-MapImageHeight/2+1);
	Canv->LineTo(xc-11,yc-9);
	Canv->LineTo(xc-7,yc-MapImageHeight/2);
	if(reg->HasLocalTaxers())
	{
	  Canv->MoveTo(xc-12,yc-MapImageHeight/2+1);
	  Canv->LineTo(xc-7,yc-MapImageHeight/2+1);
	}
	if(reg->HasLocalTraders())
	{
	  Canv->MoveTo(xc-12,yc-MapImageHeight/2+4);
	  Canv->LineTo(xc-6,yc-MapImageHeight/2+4);
	}
  }
}
//---------------------------------------------------------------------------
void TAtlaForm::DrawRegionInfoI(TCanvas * Canv,ARegion *reg, int xc, int yc){
  if(!reg) return;
  AnsiString str;
  switch(TekInfMode)
  {
    case 0: str = reg->wages;  break;
    case 1: str = reg->money; break;
    case 2: str = reg->maxwages; break;
    default: str = reg->entertainment; break;
  }
  if(str == "0") return;
  int xx = xc - Canv->TextWidth(str)/2, yy = yc - 7;// - MapImageHeight/2 - 0;
  Canv->TextOut(xx, yy, str);
}
//---------------------------------------------------------------------------
void TAtlaForm::DrawRegionProd(TCanvas * Canv,ARegion *reg, int xc, int yc)
{
 if(!reg)return;
 int xx=xc-13,yy=yc-MapImageHeight/2-0;
 int num=0;
 foreach(reg->products){
  AMarket *mark=*(AMarket**)iter;
  AItemType *itype=mark->type;//ItemTypes->Get(mark->type);
  if(itype->type&AItemType::IT_FOOD){
   if(!(ProdModeAttr&1))continue;
  }else if(itype->type&AItemType::IT_NORMAL){
   if(!(ProdModeAttr&2))continue;
  }else if(itype->type&AItemType::IT_ADVANCED){
   if(!(ProdModeAttr&4))continue;
  }
  if(mark->amount==0){
   if(!(ProdModeAttr&8))continue;
   Canv->TextOut(xx,yy,mark->type->abr);
   Canv->MoveTo(xx,yy+5);Canv->LineTo(xc+13,yy+5);
  }else{
   Canv->TextOut(xx,yy,mark->type->abr);
  }
  yy+=9;
  if(++num>=4)break;
 }
 if(num>=4) return;
 if(ProdModeAttr&16){
  Canv->TextOut(xx,yy,ItemTypes->NameToAbr(reg->race));
 }
}
static int secpoint[]={0,1,2,3,4,5},tripoint[]={1,2,3,4,5,0};

TColor wedgecolors[6]={
 clRed, clGreen, clBlue, clYellow,clOlive,TColor(0xff00b0)
};

TColor infocolors[7]={
  clWindowText, clWindowText, clWindowText, clWindowText, clWindowText, clWindowText, clWindowText
};
//---------------------------------------------------------------------------
void TAtlaForm::DrawRegionWedge(TCanvas * Canv,ARegion *reg, int xc, int yc)
{
 if(!reg)return;
 if(reg->townname.Length()){
  ATownType *tt=TownTypes->Get(reg->towntype);
  if(tt&&tt->imgnum!=-1){
   InfoImages->Draw(Canv,xc-7,yc-MapImageHeight/2+1,tt->imgnum,true);
  }
 }
 TPoint p[4];
 p[0].x=p[3].x=xc;
 p[0].y=p[3].y=yc;
 AnsiString expr;
 ExprSetRegionMode(reg);
#define AAA *2/3
 for(int i=0;i<6;i++){
  expr=wedgeexpr[i];
  if(!expr.Length()) continue;
  if(!ExprProcessExpr(expr)) continue;
  p[1].x=PoligXs[secpoint[i]] AAA+xc;
  p[1].y=PoligYs[secpoint[i]] AAA+yc;
  p[2].x=PoligXs[tripoint[i]] AAA+xc;
  p[2].y=PoligYs[tripoint[i]] AAA+yc;
#undef AAA
  Canv->Pen->Color=clBlack;
  Canv->Brush->Color=wedgecolors[i];
  Canv->Polygon(p,3);
 }
}
extern int movingdx[NDIRS], movingdy[NDIRS];
//---------------------------------------------------------------------------
void TAtlaForm::DrawRegionMove(TCanvas * Canv,ARegion *reg, int x, int y, int z, int xc, int yc)
{
 if(!reg)return;
 Canv->Pen->Color=curregColor;
 Canv->Brush->Color=curregColor;
 vector<ALocation> targlocs;
 ARegionArray *ra=curRegionList->GetRegionArray(z);
 if(curUnit)
 {
   ARegion *endreg=curUnit->endobj->endreg;
   {
	 AOrderAutoMoving *ord=0;
	 foreachr(curUnit->orders)
	 {
	   AOrder *o=*(AOrder**)iter;
	   if(o->type!=O_AUTOMOVE)
		 continue;
	   if(!o->isedited)
		 continue;
	   ord=dynamic_cast<AOrderAutoMoving*>(o);
	   break;
     }
     if(ord)
     {
       ALocation loc,oldloc;
       oldloc=ord->GetLoc(0);
       for(int i=1,endi=ord->count;i<=endi;i++)
       {
         loc=ord->GetLoc(i);
         if(loc==oldloc) continue;
         if(loc.zloc!=oldloc.zloc) break;
		 DrawRegionArrow(Canv,ra,oldloc.xloc,oldloc.yloc,loc.xloc,loc.yloc,x,y,xc,yc);
         oldloc=loc;
       }
       return;
     }
   }
   AOrderMoving *ord=dynamic_cast<AOrderMoving*>(curUnit->orders->GetMonthLong());
   if(ord&&ord->count)
   {
     ALocation loc,endloc,oldloc;
     loc=*curRegion;
     endloc=*endreg;
     oldloc=loc;
     for(int i=0,endi=ord->moved;i<endi;i++)
     {
       int dir=ord->Get(i);
       if(dir<NDIRS)
       {
         loc.xloc+=movingdx[dir];
		 loc.yloc+=movingdy[dir];
         DrawRegionArrow(Canv,ra,oldloc.xloc,oldloc.yloc,loc.xloc,loc.yloc,x,y,xc,yc);
         oldloc=loc;
         continue;
       }
       if(dir==MOVE_IN)
         break;
     }
     return;
   }
   if(endreg==curRegion)
     return;
   if(endreg->zloc==z)
   {
     ALocation loc;
     loc=*endreg;
     targlocs.push_back(loc);
   }
 }
 else
 foreach(reg)
 {
   AObject *obj=*(AObject**) iter;
   foreach(obj->units){
     AUnit *u=*(AUnit**)iter;
     ARegion *endreg=u->endobj->endreg;
     if(endreg==reg) continue;
     ALocation loc;
     loc=*endreg;
     if(endreg->zloc!=z) continue;
	 bool found=false;
	 for(ALocation* i=targlocs.begin().operator ->(); i<targlocs.end().operator ->();i++)
     {
       if(*i==loc){
         found=true;
         break;
       }
     }
     if(found) continue;
     targlocs.push_back(loc);
   }
 }
 for(ALocation* i=targlocs.begin().operator ->();i<targlocs.end().operator ->();i++)
 {
   DrawRegionArrow(Canv,ra,x,y,i->xloc,i->yloc,x,y,xc,yc);
 }
}
//---------------------------------------------------------------------------
void TAtlaForm::DrawRegionArrow(TCanvas * Canv, ARegionArray *ra, int x1, int y1, int x2, int y2, int x, int y, int xc, int yc)
{
  TPoint points[3];
  {
    int dx=x1-x2;
    if(abs(dx)>ra->x/2)
    {
      if(dx>0)
        x2+=ra->x;
      else
        x2-=ra->x;
    }
  }
  int xc1=xc+MapDX*(x1-x),yc1=yc+MapDY*(y1-y);
  int xc2=xc+MapDX*(x2-x),yc2=yc+MapDY*(y2-y);
  double dx=xc1-xc2,dy=yc1-yc2;
  if(!dx&&!dy) return;
  double h=hypot(dx,dy)/15;
  dx/=h;dy/=h;
  xc2+=dx/2.5;yc2+=dy/2.5;
  int xc3=xc2+dx+dy/4;
  int yc3=yc2-dx/4+dy;
  int xc4=xc2+dx-dy/4;
  int yc4=yc2+dx/4+dy;
  Canv->MoveTo(xc1-dx/2,yc1-dy/2);
  Canv->LineTo(xc2,yc2);
  points[0].x=xc2;points[0].y=yc2;
  points[1].x=xc3;points[1].y=yc3;
  points[2].x=xc4;points[2].y=yc4;
  Canv->Polygon(points,2);
}
//---------------------------------------------------------------------------
void TAtlaForm::DrawGrid(TCanvas *Canv,int xc, int yc, int grids[NDIRS], bool bold)
{
 for(int dir=0;dir<NDIRS;dir++){
  switch(grids[dir]){
   case 0:
	continue;
   case 1:
	Canv->Pen->Color=wallColor;
	Canv->Pen->Width=bold?2:1;
   break;
   case 2:{
	Canv->Pen->Color=wallColor;
	Canv->Pen->Width=bold?2:1;
	int x1=xc+GridXs[dir],y1=yc+GridYs[dir];
	int x2=xc+GridXs[dir+1],y2=yc+GridYs[dir+1];
	int dx=(x2-x1)/4,dy=(y2-y1)/4;
	Canv->MoveTo(x1,y1);
	Canv->LineTo(x1+dx,y1+dy);
	Canv->MoveTo(x2,y2);
	Canv->LineTo(x2-dx,y2-dy);
   }
   continue;
   case 3:{
    Canv->Pen->Color=curregColor;
    Canv->Pen->Width=2;
    int x1=xc+GridXs[dir],y1=yc+GridYs[dir];
    int x2=xc+GridXs[dir+1],y2=yc+GridYs[dir+1];
    int dx=(x2-x1)/4,dy=(y2-y1)/4;
    Canv->MoveTo(x1,y1);
    Canv->LineTo(x1+dx,y1+dy);
    Canv->MoveTo(x2,y2);
    Canv->LineTo(x2-dx,y2-dy);
   }
   continue;
//       Canv->Pen->Color=curregColor;
//   break;
  }
  Canv->MoveTo(xc+GridXs[dir],yc+GridYs[dir]);
  Canv->LineTo(xc+GridXs[dir+1],yc+GridYs[dir+1]);
 }
 Canv->Pen->Width=1;
}

int TAtlaForm::AddMapImage(AnsiString fName)
{
  int ind=-1;
  AnsiString fname=ClientPath+"images\\terrain\\"+fName+".bmp";
  if(!FileExists(fname)) return ind;
  TBmp *bmp=new TBmp;
  try{
	bmp->LoadFromFile(fname);
    bmp->TransparentMode=tmAuto;
    if(bmp->Width!=MapImageWidth||bmp->Height!=MapImageHeight)
      throw 1;
    TCanvas *canv=bmp->Canvas;
    TPoint p[4];
    int xc=MapImageWidth/2,yc=MapImageHeight/2;
    canv->Pen->Color=bmp->TransparentColor;
    canv->Brush->Color=bmp->TransparentColor;
    {
      p[0].x=p[3].x=0;
      p[0].y=p[3].y=0;
      p[1].x=xc+GridXs[5];
      p[1].y=yc+GridYs[5];
      p[2].x=xc+GridXs[6];
      p[2].y=yc+GridYs[6];
      canv->Polygon(p,3);
    }
    {
      p[0].x=p[3].x=MapImageWidth;
      p[0].y=p[3].y=0;
      p[1].x=xc+GridXs[1];
      p[1].y=yc+GridYs[1];
      p[2].x=xc+GridXs[2];
      p[2].y=yc+GridYs[2];
      canv->Polygon(p,3);
    }
    {
      p[0].x=p[3].x=MapImageWidth;
      p[0].y=p[3].y=MapImageHeight;
      p[1].x=xc+GridXs[2];
      p[1].y=yc+GridYs[2];
      p[2].x=xc+GridXs[3];
      p[2].y=yc+GridYs[3];
      canv->Polygon(p,3);
    }
    {
      p[0].x=p[3].x=0;
      p[0].y=p[3].y=MapImageHeight;
      p[1].x=xc+GridXs[4];
      p[1].y=yc+GridYs[4];
      p[2].x=xc+GridXs[5];
      p[2].y=yc+GridYs[5];
      canv->Polygon(p,3);
    }
    ind=MapImages->AddMasked(bmp,bmp->TransparentColor);
  }catch(...){
  }
 delete bmp;
 return ind;
}
//---------------------------------------------------------------------------
int TAtlaForm::AddInfoImage(AnsiString fName)
{
 TBmp *bmp=new TBmp;
 int ind;
 try{
  bmp->LoadFromFile(ClientPath+"images\\"+fName);
  bmp->TransparentMode=tmAuto;
  ind=InfoImages->AddMasked(bmp,bmp->TransparentColor);
  delete bmp;
 }catch(...){
  delete bmp;
  return -1;
 }
 return ind;
}
//---------------------------------------------------------------------------
int TAtlaForm::AddToolBarImage(AnsiString fName)
{
 TBmp *bmp=new TBmp;
 int ind;
 try{
  bmp->LoadFromFile(ClientPath+"images\\"+fName);
  bmp->TransparentMode=tmAuto;
  ind=ToolBarImages->AddMasked(bmp,bmp->TransparentColor);
  delete bmp;
 }catch(...){
  delete bmp;
  return -1;
 }
 return ind;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::actLevelUpExecute(TObject *Sender)
{
 ChangeLevel(-1);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::actLevelDownExecute(TObject *Sender)
{
 ChangeLevel(1);
}
//---------------------------------------------------------------------------
void TAtlaForm::ChangeLevel(int dir)
{
/* if(dir!=0){

 }*/
 curTurns->StoreLastXY();
 int newlevel=curRegion->zloc+dir;
 int newx;//=curRegion->xloc;
 int newy;//=curRegion->yloc;
 curTurns->RestoreLastXY(newx,newy,newlevel);
/* curRegionList->SetCurRegionArray(newlevel);
 if(newx>curRegionArray->x)newx=curRegionArray->x;
 if(newy>curRegionArray->y)newy=curRegionArray->y;*/
 ARegion *reg=curRegionList->GetNearestRegion(newx,newy,newlevel);
 if(!reg)return;
 curRegionList->SetCurRegion(reg->xloc,reg->yloc,reg->zloc);
 Plugins.OnLevelChanged();
 {
  bool ena=(newlevel!=0);
  actLevelUp->Enabled=ena;
 }
 {
  bool ena=(curRegionList->RegionArrays->count>newlevel+1);
  actLevelDown->Enabled=ena;
 }
 xcentr=curRegion->xloc;
 ycentr=curRegion->yloc;
 zcentr=curRegion->zloc;
 MapForm->ReCalcRect();
 OnSetCurRegion();
}
//---------------------------------------------------------------------------
void TAtlaForm::PointToCoord(int x, int y, int & xloc, int & yloc)
{
 int sign,_xloc,_yloc;
 _xloc=x-ImageMap->Width/2;
 sign=_xloc<0?-1:1;
 _xloc=(sign*_xloc+MapDX/2)/MapDX;

 xloc=xcentr+_xloc*sign;
 _yloc=y-ImageMap->Height/2;
 sign=_yloc<0?-1:1;
 if(_xloc&1){
  _yloc=(sign*_yloc/MapDY)|1;
 }else{
  _yloc=(sign*_yloc/MapDY+1)&(~1);
 }
 yloc=ycentr+_yloc*sign;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::ImageMapMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
 int xloc,yloc;
 PointToCoord(X,Y,xloc,yloc);
 if(!curRegionArray) return;
 curRegionArray->NormCoord(xloc,yloc);
 AnsiString mes;
 if(Button==mbRight){
  xcentr=xloc;
  ycentr=yloc;
  MapForm->ReCalcRect();
  DrawMap();
  ImageMapMouseMove(Sender,Shift,X,Y);
  return;
 }
 if(Button==mbLeft){
#ifdef DRAWMAPTIMING
 long t0=GetTickCount();
#endif
  ARegion *reg=curRegionList->GetNearestRegion(xloc,yloc,zcentr);
  curRegionList->SetCurRegion(reg->xloc,reg->yloc,reg->zloc);
#ifdef DRAWMAPTIMING
 long t1=GetTickCount();
#endif
  OnSetCurRegion();
#ifdef DRAWMAPTIMING
 long t2=GetTickCount();
 AnsiString str=t2-t0;
 str+="=";
 str+=t1-t0;
 str+="+";
 str+=t2-t1;
 str = "On click" + str;
 AddEvent(str);
#endif
  return;
 }
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::ImageMapMouseMove(TObject *Sender,
	  TShiftState Shift, int X, int Y)
{
 int xloc,yloc;
 if(!curRegionArray) return;
 PointToCoord(X,Y,xloc,yloc);
 SetlbCoords(xloc,yloc);
}
//---------------------------------------------------------------------------
void TAtlaForm::SetlbCoords(int xloc, int yloc)
{
 curRegionArray->NormCoord(xloc,yloc);
 AnsiString mes;
 mes+=xloc;
 mes+=",";
 mes+=yloc;
 mes+=" R:";
 mes+=curRegion->GetDistance(xloc,yloc,curRegion->zloc);
 lbCoord->Caption=mes;
 RegInfoForm->UpdateRegionInfo(xloc,yloc,curRegion->zloc);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::actPrevTurnExecute(TObject *Sender)
{
  NeedDraw = false;
  try
  {
	if(GetKeyState(VK_SHIFT)<0) ChangeTurn(-curTurn->num,false);
		else
	if(GetKeyState(VK_CONTROL)<0) ChangeTurn(-1,false);
		else ChangeTurn(-1,true);
  } __finally { NeedDraw = true; }
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::actNextTurnExecute(TObject *Sender)
{
  NeedDraw = false;
  try
  {
    if(GetKeyState(VK_SHIFT)<0) ChangeTurn(curTurns->count-curTurn->num-1,false);
        else
    if(GetKeyState(VK_CONTROL)<0) ChangeTurn(1,false);
        else ChangeTurn(1,true);
  } __finally { NeedDraw = true; }
}
//---------------------------------------------------------------------------

void TAtlaForm::ChangeTurn(int dir,bool load,bool updatemaps)
{
 if(!curTurns) return;
 int newturn=0;

 if(curTurn){
  newturn=curTurn->num+dir;
  if(newturn<0) newturn=0;
  if(newturn>=curTurns->count) newturn=curTurns->count-1;
  {
   ATurn *turn=curTurns->Get(newturn);
   if(!turn){
	if(load){
	 curTurns->PreloadTurnIfNeed(newturn,true);
	 turn=curTurns->Get(newturn);
	}
	if(!turn){
	 if(dir<0){
	  newturn=curTurns->GetPrevTurnNum(newturn);
	 }else{
	  newturn=curTurns->GetNextTurnNum(newturn);
	 }
	}
   }
  }
 }
/* if(newturn!=0&&newturn<curTurns->curbeginturn){
  if(dir<0) newturn=0; else newturn=curTurns->curbeginturn;
 }*/
 if(newturn<0) return;
 curTurns->StoreLastParam();
 if(!curTurns->SetCurTurn(newturn))return;
 curTurns->LockTurn(newturn);
 curTurns->UnlockTurn(newturn);
 actPrevTurn->Enabled=newturn>0;
 actNextTurn->Enabled=newturn+1<curTurns->count;
 curTurn->PrepareOrders();
 Plugins.OnTurnChanged(updatemaps);
 LookLocalFactions();
 if(dir||updatemaps)
 {
   MapForm->ResetMap();
   if(updatemaps)
	 if(ViewUnitsForm)
	   ViewUnitsForm->MakeFacList();
   if(TViewUnitsForm::regdiap==TViewUnitsForm::rdtAll
	 ||TViewUnitsForm::regdiap==TViewUnitsForm::rdtRegionList)
	 if(ViewUnitsForm)
	   ViewUnitsForm->ReMake();
 }
 NeedDraw = true;
 ChangeLevel(0);
}
//---------------------------------------------------------------------------

void TAtlaForm::HideMenuItems(TMenu *menu)
{
  if(menu==popupFactionMessages)
  {
	miFacClearErrors->Visible=false;
	miFacClearError->Visible=false;
	miFacClearBattles->Visible=false;
	miFacClearBattle->Visible=false;
	miFacGotoBattle->Visible=false;
	miFacBattleToSim->Visible=false;
	miFacBattleToClipboard->Visible=false;
	miFacClearEvents->Visible=false;
	miFacClearEvent->Visible=false;
  }
}
//---------------------------------------------------------------------------
void TAtlaForm::MakeFacList()
{
  TStrings *list = cbFacDiap->Items;
  list->BeginUpdate();
  list->Clear();
  list->AddObject(" all",(TObject*)-2);
  list->AddObject(" local",(TObject*)-3);
  list->AddObject(" nonlocal",(TObject*)-4);
  list->AddObject(" friendly",(TObject*)-5);
  list->AddObject(" nonfriendly",(TObject*)-7);
  list->AddObject(" unfriendly",(TObject*)-6);
  foreach(curFactions)
  {
	AFaction *fac=*(AFaction**)iter;
	list->AddObject(fac->FullName(),(TObject*)fac->number);
  }
  list->EndUpdate();
  int ind = list->IndexOfObject((TObject*)facdiap);
  cbFacDiap->ItemIndex = ind;

  cbProfSel->Items->Clear();
  TStringList *list1 = new TStringList;
  list1->Clear();
  SkillTypes->MakeSkillList(list1);
  list1->Sort();
  cbProfSel->Items->Add(" all");
  cbProfSel->Items->AddStrings(list1);
  delete list1;
  ind = cbProfSel->Items->IndexOf(FskilFiltr);
  if(ind < 0) ind = 0;
  cbProfSel->ItemIndex = ind;
  btAllFoff->Enabled = (cbFacDiap->ItemIndex > 0) || (cbProfSel->ItemIndex > 0) ||
	  (!miFlead0->Checked) || (!miNoFF->Checked) || (FmoFiltr != 0);
}
//---------------------------------------------------------------------------
void TAtlaForm::ViewShow()
{
  static int InViewShow = 0;
  curUnit = 0;
  if(!curNode)return;
  VTreeNode *vnode=(VTreeNode*)curNode->Data;
  if(!vnode)return;
  if(InViewShow) return;
  InViewShow++;
  vlistitems->Clear();
  AFaction *fac;
  TreeViewPopupMenu = 0;
  NowRegShow = false;
  switch(vnode->type){
   case tntFaction:
	 fac=curFactions->GetFaction(vnode->value);
	 if(!fac)break;
	 fac->MakeView(vlistitems);
	 break;
   case tntFactionErrors:
	 fac=curFactions->GetFaction(vnode->value);
	 if(!fac)break;
	 fac->MakeViewErrors(vlistitems);
	 TreeViewPopupMenu=popupFactionMessages;
	 popupFactionMessages->Tag=vnode->value;
	 HideMenuItems(popupFactionMessages);
	 miFacClearErrors->Visible=true;
	 break;
   case tntFactionBattles:
	 fac=curFactions->GetFaction(vnode->value);
	 if(!fac)break;
	 fac->MakeViewBattles(vlistitems);
	 TreeViewPopupMenu=popupFactionMessages;
	 popupFactionMessages->Tag=vnode->value;
	 HideMenuItems(popupFactionMessages);
	 miFacClearBattles->Visible=true;
	 break;
   case tntFactionBattle:
	 fac=curFactions->GetFaction(vnode->value/10000);
	 if(!fac)break;
	 fac->MakeViewBattle(vlistitems,vnode->value%10000);
	 TreeViewPopupMenu=popupFactionMessages;
	 popupFactionMessages->Tag=fac->number;
	 HideMenuItems(popupFactionMessages);
	 miFacClearBattles->Visible=true;
	 miFacClearBattle->Visible=true;
	 miFacGotoBattle->Visible=true;
	 miFacBattleToSim->Visible=true;
	 miFacBattleToClipboard->Visible=true;
	 break;
   case tntFactionEvents:
	 fac=curFactions->GetFaction(vnode->value);
	 if(!fac)break;
	 fac->MakeViewEvents(vlistitems);
	 fac->VerifyEventsWarnings();
	 TreeViewPopupMenu=popupFactionMessages;
	 popupFactionMessages->Tag=vnode->value;
	 HideMenuItems(popupFactionMessages);
	 miFacClearEvents->Visible=true;
	 break;
   case tntRegion:
	 curRegion->MakeView(vlistitems);
	 TreeViewPopupMenu=popupViewRegion;
	 popupViewRegion->Tag=-1;
	 NowRegShow = true;
	 break;
   case tntObject:
   {
	 AObject *obj=curRegion->GetObject(vnode->value);
	 if(!obj)break;
	 obj->MakeView(vlistitems);
	 TreeViewPopupMenu=popupViewRegObject;
	 popupViewRegObject->Tag=vnode->value;
	 break;
   }
   case tntUnit:
   {
	 AUnit *unit=(AUnit*) vnode->data;//curRegion->GetUnit(vnode->value);
	 if(!unit)break;
	 unit->MakeView(vlistitems);
	 vnode->SetText(unit->PrintForTree());
	 curUnit=unit;
	 TreeViewPopupMenu=popupViewUnit;
	 popupViewUnit->Tag=-1;
	 UpdateObjectsTreeInfo();
	 break;
   }
  } // end Switch

  View->ItemIndex=-1;
  TStrings *list=View->Items;
  int maxsize = 0;
  list->BeginUpdate();
  list->Clear();
  for(int i=0;i<vlistitems->count;i++)
  {
	AnsiString str = vlistitems->Get(i)->text;
	list->Add(str);
	TSize siz = View->Canvas->TextExtent(str);
	if(siz.cx > maxsize) maxsize = siz.cx;
  }
  list->EndUpdate();
  View->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
  // SetMainFormCaption();
  InViewShow--;
}
//---------------------------------------------------------------------------
void TAtlaForm::UpdateObjectsTreeInfo()
{
		for(int i=vtreenodes->count-1;i>=0;i--){
		VTreeNode *vnode=vtreenodes->Get(i);
		if(vnode->type!=tntObject) continue;
		AObject *obj=curRegion->GetObject(vnode->value);
		if(!obj) continue;
		vnode->SetText(obj->ObjectHeader(false));
	}
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::bnShowEventsClick(TObject *Sender)
{
 EventsForm->Show();
 EventsForm->BringToFront();
}
//---------------------------------------------------------------------------
void TAtlaForm::OnSetCurRegion()
{
 if(onSaveNow)return;
 if(!curRegion) return;
#ifdef DRAWMAPTIMING
 long t0=GetTickCount();
#endif
 MakingTree++;
 SetMainFormCaption();
 if(optRunRegionOrders) curRegion->runned=false;
#ifdef DRAWMAPTIMING
 long t1=GetTickCount();
#endif
 MakeTreeView();
#ifdef DRAWMAPTIMING
 long t2=GetTickCount();
#endif
 MakingTree--;
 DrawMap();
#ifdef DRAWMAPTIMING
 long t3=GetTickCount();
#endif
 RegInfoForm->ShowRegInfo=actShowRegInfo->Checked;
 RegInfoForm->UpdateRegionInfo(curRegion->xloc,curRegion->yloc,curRegion->zloc);

#ifdef DRAWMAPTIMING
 long t4=GetTickCount();
#endif

 MapForm->ShowMap=actShowMap->Checked;
 MapForm->UpdateMap(curRegion->zloc);

#ifdef DRAWMAPTIMING
 long t5=GetTickCount();
#endif

 if(TViewUnitsForm::regdiap!=TViewUnitsForm::rdtAll
  &&TViewUnitsForm::regdiap!=TViewUnitsForm::rdtRegionList)
 {
   if(ViewUnitsForm)
	 ViewUnitsForm->ReMake();
 }
#ifdef DRAWMAPTIMING
 long t6=GetTickCount();
#endif
 ViewShow();
#ifdef DRAWMAPTIMING
 long t7=GetTickCount();
#endif
 Plugins.OnRegionChanged();
#ifdef DRAWMAPTIMING
 long t8=GetTickCount();
 AnsiString str=t8-t0;
 str+="=";
 str+=t1-t0;
 str+="+";
 str+=t2-t1;
 str+="+";
 str+=t3-t2;
 str+="+";
 str+=t4-t3;
 str+="+";
 str+=t5-t4;
 str+="+";
 str+=t6-t5;
 str+="+";
 str+=t7-t6;
 str+="+";
 str+=t8-t7;
 str = ">>>>" + str;
 AddEvent(str);
#endif
}
//---------------------------------------------------------------------------
void TAtlaForm::MakeTreeView()
{
  MakingTree++;
  TreeViewSelected = 0;
  curRegion->RunOrders(false);
  TTreeNodes *tns = TreeView->Items;
  tns->BeginUpdate();
  tns->Clear();
  TTreeNode *tn;
  vtreenodes->Clear();
  VTreeNode *vtn;

  // Инфо по фракции
  vtn = new VTreeNode(vtreenodes);
  tn = tns->AddChildObject(0,"Local factions",vtn);
  vtn->node = tn;
  curFactions->MakeTree(tn,vtreenodes);
  tn->Collapse(false);
  // Регион
  MakeFacList();
  vtn = new VTreeNode(vtreenodes);
  tn = tns->AddChildObject(0,"Region",vtn);
  vtn->node = tn;
  curRegion->MakeTree(tn,vtreenodes);
  tn->Expand(true);

  tn = tn->getFirstChild();
  TreeViewSelected = 0;
  TreeView->AlphaSort();
  tns->EndUpdate();
  TreeViewSelected = tn;
  MakingTree--;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::FormShortCut(TWMKey &Msg, bool &Handled)
{
  if(!Active) return;
  if(ActiveControl!=TreeView) return;
  if(!TreeViewPopupMenu) return;
  Handled=TreeViewPopupMenu->IsShortCut(Msg);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::TreeViewKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
 if(!TreeViewPopupMenu) return;
 if(Key==VK_APPS){
  TPoint mp=Mouse->CursorPos;
  if(TreeViewPopupMenu){
   TreeViewPopupMenu->PopupComponent=TreeView;
   TreeViewPopupMenu->Popup(mp.x,mp.y);
  }
  Key=0;
 }
}

void __fastcall TAtlaForm::TreeViewMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(Button != mbRight) return;
  TTreeNode *TN;
  TN = TreeView->GetNodeAt(X,Y);
  if (TN) TreeViewSelected = TN;
  TPoint mp = Mouse->CursorPos;
  if(TreeViewPopupMenu)
  {
	TreeViewPopupMenu->PopupComponent=TreeView;
	TreeViewPopupMenu->Popup(mp.x, mp.y);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::TreeViewChange(TObject *Sender, TTreeNode *Node)
{
  if(curNode == Node)return;
  curNode = Node;
  if(MakingTree)return;
  Plugins.OnTreeItemChanged();
  ViewShow();
  if(MapMode==0 || optAlwaysShowMovement)    DrawMap();
  TreeView->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::ViewClick(TObject *Sender)
{
 int ind=View->ItemIndex;
 if(ind==-1)return;
 VListItem *vli=vlistitems->Get(ind);
 if(!vli)return;
 View->PopupMenu=0;
 switch(vli->type){
  case litRegHeader:
  case litRegWages:
  case litRegEnte:

   View->PopupMenu=popupViewRegion;
   popupViewRegion->Tag=-1;
  break;
  case litRegObj:case litObjHeader:
   View->PopupMenu=popupViewRegObject;
   popupViewRegObject->Tag=vli->value;
  break;
  case litUnitHeader:case litUnitOrder:case litUnitInfo:
   View->PopupMenu=popupViewUnit;
   if(vli->type!=litUnitOrder){
    popupViewUnit->Tag=-1;
   }else{
    popupViewUnit->Tag=vli->value;
   }
  break;
  case litFactErrors:case litFactErrorsHeader:
   View->PopupMenu=popupFactionMessages;
   popupFactionMessages->Tag=vli->value;
   HideMenuItems(popupFactionMessages);
   miFacClearErrors->Visible=true;
  break;
  case litFactError:
   View->PopupMenu=popupFactionMessages;
   popupFactionMessages->Tag=vli->value;
   HideMenuItems(popupFactionMessages);
   miFacClearError->Visible=true;
  break;
  case litFactBattles:case litFactBattlesHeader:
   View->PopupMenu=popupFactionMessages;
   popupFactionMessages->Tag=vli->value;
   HideMenuItems(popupFactionMessages);
   miFacClearBattles->Visible=true;
  break;
  case litFactBattle:case litFactBattleHeader:
   View->PopupMenu=popupFactionMessages;
   popupFactionMessages->Tag=vli->value;
   HideMenuItems(popupFactionMessages);
   miFacClearBattle->Visible=vli->type==litFactBattleHeader;
   miFacGotoBattle->Visible=true;
   miFacBattleToSim->Visible=true;
   miFacBattleToClipboard->Visible=true;
  break;
  case litFactEvents: case litFactEventsHeader:
  case litUnitEvents:
   View->PopupMenu=popupFactionMessages;
   popupFactionMessages->Tag=vli->value;
   HideMenuItems(popupFactionMessages);
   miFacClearEvents->Visible=true;
  break;
  case litFactEvent: case litUnitEvent:
   View->PopupMenu=popupFactionMessages;
   popupFactionMessages->Tag=vli->value;
   HideMenuItems(popupFactionMessages);
   miFacClearEvent->Visible=true;
  break;
  default:
   View->PopupMenu=popupViewDefault;
 }
}
void __fastcall TAtlaForm::ViewDblClick(TObject *Sender)
{
 int ind=View->ItemIndex;
 if(ind==-1)return;
 VListItem *vli=vlistitems->Get(ind);
 if(!vli)return;
 VTreeNode *vnode=0;
 switch(vli->type){
  case litFactErrors:
   vnode=FindTreeNode(tntFactionErrors,vli->value);
  break;
  case litFactBattles:
   vnode=FindTreeNode(tntFactionBattles,vli->value);
  break;
  case litFactEvents:
   vnode=FindTreeNode(tntFactionEvents,vli->value);
  break;
  case litFactErrorsHeader:
  case litFactBattlesHeader:
  case litFactEventsHeader:
   vnode=FindTreeNode(tntFaction,vli->value);
  break;
  case litFactBattleHeader:
   vnode=FindTreeNode(tntFactionBattle,vli->value*10000+ind-1);
  break;
  case litRegHeader:
//////
  break;
  case litRegObj:
   vnode=FindTreeNode(tntObject,vli->value);
  break;
  case litObjHeader:
   vnode=FindTreeNode(tntRegion,-1);
  break;
  case litObjUnit:
   vnode=FindTreeNode(tntUnit,vli->value);
  break;
  case litUnitHeader:
   {
    if(!curUnit)break;
    AObject *obj=curUnit->baseobj;
    if(!obj)break;
    if(obj->num){
     vnode=FindTreeNode(tntObject,obj->num);
    }else{
     vnode=FindTreeNode(tntRegion,-1);
    }
   }
  break;
  case litUnitOrder:
   EditOrder(vli->value);
  break;
 }
 if(vnode) TreeViewSelected=vnode->node;
}
//---------------------------------------------------------------------------


VTreeNode* TAtlaForm::FindTreeNode(int type, int value)
{
 for(int i=vtreenodes->count-1;i>=0;i--){
  VTreeNode *vnode=vtreenodes->Get(i);
  if(vnode->type==type&&vnode->value==value)return vnode;
 }
 return 0;
}

//---------------------------------------------------------------------------

void __fastcall TAtlaForm::ViewMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if(Button==mbRight){
  {
   TWMMouse mes;
   mes.Msg=WM_LBUTTONDOWN;
   mes.Keys=MK_LBUTTON;
   mes.XPos=short(X);
   mes.YPos=short(Y);
   View->Dispatch(&mes);
  }
  {
   TWMMouse mes;
   mes.Msg=WM_LBUTTONUP;
   mes.Keys=MK_LBUTTON;
   mes.XPos=short(X);
   mes.YPos=short(Y);
   View->Dispatch(&mes);
  }
 }
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::popupViewRegObjectPopup(TObject *Sender)
{
 AObject *obj=curRegion->GetObject(popupViewRegObject->Tag);
 if(!obj)return;
 bool IsInner=(obj->inner);
 miLinkInner->Enabled=IsInner;
 miGotoLinked->Enabled=IsInner&&obj->innerloc.xloc!=-1;
 miCopyToClipboard1->Visible=popupViewRegObject->PopupComponent==View;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miLinkInnerClick(TObject *Sender)
{
 AObject *obj=curRegion->GetObject(popupViewRegObject->Tag);
 if(!obj)return;
 TLinkShaftForm *frm=new TLinkShaftForm(this,obj);
 int res=frm->ShowModal();
 ALocation loc=frm->GotoLoc;
 delete frm;
 if(res==mrOk){
  curTurn->FixShafts(0,0);
  curTurn->Modified=true;
  curTurn->VerifyNextTurn();
  OnSetCurRegion();
 }else if(res==mrYes){
  ARegion *reg=curRegionList->Get(loc);
  if(reg)
	GotoRegion(reg);
 }
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miGotoLinkedClick(TObject *Sender)
{
 AObject *obj=curRegion->GetObject(popupViewRegObject->Tag);
 if(!obj)return;
 if(obj->innerloc.xloc==-1)return;
 curTurns->StoreLastXY();
 curRegionList->SetCurRegion(obj->innerloc.xloc,obj->innerloc.yloc,obj->innerloc.zloc);
 ChangeLevel(0);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miLoadOrderClick(TObject *Sender)
{
 curTurns->StoreLastParam();
 char buf[10];
 sprintf(buf,"ord%05d.*",curTurnNumber);
 OpenDialog->Filter=AnsiString("Order file for current turn|")+buf+"|All files|*.*";
 if(LoadOrdersDir.Length()){
  OpenDialog->InitialDir=LoadOrdersDir;
  if(ExtractFilePath(OpenDialog->FileName)!=LoadOrdersDir)
   OpenDialog->FileName="";
 }
 if(OpenDialog->Execute()){
  LoadOrdersDir=ExtractFilePath(OpenDialog->FileName);
  bool ok=true;
  TStringList *list=new TStringList;
  list->Assign(OpenDialog->Files);
  list->Sort();
  for(int i=0;ok&&i<list->Count;i++){
   if(!ProcessOrder(list->Strings[i]))ok=false;
  }
  delete list;
  ChangeTurn(0);
 }
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miSaveOrderClick(TObject *Sender)
{
  bool first=true;
  onSaveNow = true;
  int saveCF = miCF->Tag;
  foreach(curFactions){
	AFaction *fac=*(AFaction**)iter;
	if(!fac->local)continue;
    int facnum=fac->number;
    AnsiString fName=GetOrderName(facnum);
    if(!SaveOrder(fName,facnum)){
      AnsiString str="Error saving order for faction ";
      str+=fac->FullName();
      AddEvent(str);
      break;
    }
    if(first&&optCopyOnSaveOrders){
      first=false;
      TStringList *list=new TStringList;
      try
      {
        list->LoadFromFile(fName);
        Clipboard()->AsText=list->Text;
      } catch(Exception&c){  }
      delete list;
    }
  }
  DeleteAutoSaveFiles();
  if(curFaction->number != saveCF) curFactions->SetCurFaction(saveCF);
  onSaveNow = false;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::popupViewUnitPopup(TObject *Sender)
{
 int ind=popupViewUnit->Tag;
 miEditOrder->Enabled=(ind>=0);
 miDeleteOrder->Enabled=(ind>=0);
// miAddOrder->Enabled=(curUnit->IsLocal());
 miDeleteNewUnit->Enabled=(curUnit->num<0);
 miCopyNewUnit->Enabled=(curUnit->num<0);

 miAddOrderFormNew->Enabled=/*(curUnit->num>0)&&*/(curUnit->faction->local);
 miGotoOrderTarget->Enabled=(ind>=0);

 miPasteOrder->Enabled=(orderBuf);

 miCopyToClipboard2->Visible=popupViewUnit->PopupComponent==View;

 miMoveOrderUp->Enabled=(ind>=1);
 miMoveOrderDown->Enabled=(ind>=0&&ind<curUnit->orders->count-1);

 miAddOrderNoCross->Visible=optServNoCross;
 miAddOrderNoSpoils->Visible=optServNoSpoils;
 miAddOrderSharing->Visible=optServerSharing;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miEditOrderClick(TObject *Sender)
{
 EditOrder(popupViewUnit->Tag);
}
//---------------------------------------------------------------------------
void TAtlaForm::EditOrder(int ind, AOrder *neworder)
{
  if(!curUnit)return;
  AOrder *ord;
  AOrder *oldmonthlong = curUnit->orders->GetMonthLong(); //for restore commented
  if(neworder)
  {
	ord = neworder;
	curUnit->orders->Add(ord);
  } else ord = curUnit->GetOrder(ind);
  if(!ord)return;
  bool isautomove = (ord->type==O_AUTOMOVE);
  if(ord->type == O_MESSAGE)
  {
	if(neworder)throw Exception("Can't Create & Edit Message");
	miGotoOrderTargetClick(0);
	return;
  }
  AOrder *neword = ord->Edit();
  if(neworder)
  {
	if(!isautomove)
	{
	  if(ord != neword)
	  {
		if(!neword)
		{
		  if(neworder->IsMonthLong())
			if(oldmonthlong) oldmonthlong->commented = false;
		  curUnit->orders->Delete(ord);
		  if(!curRegion->runned)
		  {
			curRegion->RunOrders(false);
			if(MapMode==0 || optAlwaysShowMovement) DrawMap();
		  }
		  return;
		}
		curUnit->orders->Delete(ord);
		curUnit->orders->Add(neword);
	  }
	  if(!curUnit->IsLocal())
	  {
		if(ProcessNonLocalOrders(neword)) curUnit->orders->Delete(neword);
	  }
	}
  } else
  {
	if(!neword)
	{
	  if(!isautomove)
	  {
		if(!curRegion->runned)
		{
		  curRegion->RunOrders(false);
		  if(MapMode==0||optAlwaysShowMovement) DrawMap();
		}
		return;
	  }
	} else
	if(ord!=neword)
	{
	  if(!curUnit->orders->Replace(ord,neword)) delete neword;
	}
  }
  curTurn->Modified = true;
  curRegion->runned = false;
  ViewShow();
  if(MapMode==0||optAlwaysShowMovement) DrawMap();
  if(ViewUnitsForm) ViewUnitsForm->UpdateUnit(curUnit);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miDeleteOrderClick(TObject *Sender)
{
 if(!curUnit)return;
 int ind=popupViewUnit->Tag;
 AOrder *ord=curUnit->GetOrder(ind);
 if(!ord)return;
 AnsiString mes;
 if(ord->type==O_MESSAGE){
  AOrderMessage *mess=(AOrderMessage*) ord;
  if(mess->subtype==OM_BORROWSTO){
   AUnit *targ=mess->target;
   AUnit *base=mess->orders->unit;
   AOrderGive *gord=new AOrderGive;
   gord->tarnum=base->num;
   gord->target=base;
   gord->count=mess->ivalue1;
   gord->item=mess->svalue1;
   targ->orders->Add(gord);
   curTurn->Modified=true;
   curRegion->runned=false;
   ViewShow();
   return;
  }
  if(mess->subtype==OM_BORROWSFROM){
   AUnit *targ=mess->target;
   AUnit *base=mess->orders->unit;
   AOrderGive *gord=new AOrderGive;
   gord->tarnum=targ->num;
   gord->target=targ;
   gord->count=mess->ivalue1;
   gord->item=mess->svalue1;
   base->orders->Add(gord);
   curTurn->Modified=true;
   curRegion->runned=false;
   ViewShow();
   return;
  }
  AOrder *targ=mess->base;
  if(targ){
   if(targ->type!=O_GIVE) return;
   mes="You are sure to delete base GIVE order?\n";
   mes+=targ->Print();
   if(Application->MessageBox(mes.c_str(),"Deleting order",MB_YESNO)!=IDYES)return;
   targ->orders->Delete(targ);
   curTurn->Modified=true;
   curRegion->runned=false;
  }
  ViewShow();
  return;
 }
 mes="You are sure to delete this order?\n";
 mes+=ord->Print();
 if(Application->MessageBox(mes.c_str(),"Deleting order",MB_YESNO)!=IDYES)return;
 if(ord->type == O_TAX)
   if(curUnit->GetEndFlag(FLAG_AUTOTAX))
   {
	 mes="Do you wish to set flag autotax 0?";
	 if(Application->MessageBox(mes.c_str(),"Deleting order",MB_YESNO)!=IDYES)return;
	 curUnit->SetFlagOrders(curUnit->guard, curUnit->reveal, (curUnit->flags&~FLAG_AUTOTAX));
   }
 curUnit->orders->Delete(ind);
 curTurn->Modified=true;
 curRegion->runned=false;
 ViewShow();
 if(MapMode==0||optAlwaysShowMovement)
   DrawMap();
 if(ViewUnitsForm)
   ViewUnitsForm->UpdateUnit(curUnit);
}
//---------------------------------------------------------------------------
bool TAtlaForm::ProcessNonLocalOrders(AOrder *ord)
{
 if(ord->type==O_STUDY){
  AOrderAbr *_ord=dynamic_cast<AOrderAbr*>(ord);
  AnsiString type=_ord->abr;
  ASkill *sk=curUnit->skills->GetSkill(type);
  if(!sk){
   sk=new ASkill;
   sk->SetType(type);
   curUnit->skills->Add(sk);
  }
 // if(val>450) val=450;
  int lev=sk->GetLevel();
  do{
   sk->days+=30;
  }while(lev>=sk->GetLevel());
  curUnit->hasskills=true;
  return true;
 }
 if(ord->type==O_FORGET){
  AOrderAbr *_ord=dynamic_cast<AOrderAbr*>(ord);
  AnsiString type=_ord->abr;
  ASkill *sk=curUnit->skills->GetSkill(type);
  if(sk){
   sk->days=0;
   curUnit->skills->DeleteEmpty();
  }
  return true;
 }
 return false;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderCustomClick(TObject *Sender)
{
  EditOrder(0, new AOrderCustom);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderCommentClick(TObject *Sender)
{
 EditOrder(0,new AOrderComment);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderLocDescrClick(TObject *Sender)
{
 EditOrder(0,new AOrderLocDescr);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miFacClearErrorsClick(TObject *Sender)
{
 AFaction *fac=curFactions->GetFaction(popupFactionMessages->Tag);
 if(!fac)return;
 fac->errors->Clear();
 curTurn->Modified=true;
 MakeTreeView();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miFacClearBattlesClick(TObject *Sender)
{
 AFaction *fac=curFactions->GetFaction(popupFactionMessages->Tag);
 if(!fac)return;
 fac->battles->Clear();
 fac->battleindexs.clear();
 curTurn->Modified=true;
 MakeTreeView();
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miFacClearEventsClick(TObject *Sender)
{
 int facnum=popupFactionMessages->Tag;
 if(facnum>0){
  AFaction *fac=curFactions->GetFaction(facnum);
  if(!fac)return;
  fac->events->Clear();
  curTurn->Modified=true;
  MakeTreeView();
 }else{
  AFaction *fac=curFactions->GetFaction(-facnum);
  if(!fac)return;
  int ind=View->ItemIndex;
  if(ind<=0)return;
  AnsiString str=View->Items->Strings[ind+1];
  int pos=str.Pos(')');
  if(!pos) return;
  str.SetLength(pos);
  for(int i=fac->events->Count-1;i>=0;i--){
   char* s=fac->events->Strings[i].c_str();
   if(MyCompare(s,str)) continue;
   fac->events->Delete(i);
   curTurn->Modified=true;
  }
  if(fac->events->Count){
   int ind=View->ItemIndex;
   ViewShow();
   View->ItemIndex=ind;
  }else MakeTreeView();
 }
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miFacClearErrorClick(TObject *Sender)
{
 AFaction *fac=curFactions->GetFaction(popupFactionMessages->Tag);
 if(!fac)return;
 int ind=View->ItemIndex;
 int tind=View->TopIndex;
 if(ind<=0)return;
 ind--;
/* AnsiString str=View->Items->Strings[ind];
 ind=fac->errors->IndexOf(str);
 if(ind==-1)return;*/
 fac->errors->Delete(ind);
 curTurn->Modified=true;
 if(fac->errors->Count){
  int ind=View->ItemIndex;
  ViewShow();
  View->ItemIndex=ind;
  View->TopIndex=tind;
  ViewClick(this);
 }else MakeTreeView();
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miFacClearBattleClick(TObject *Sender)
{
 if(!curNode) return;
 VTreeNode *vnode=(VTreeNode*)curNode->Data;
 AFaction *fac=curFactions->GetFaction(popupFactionMessages->Tag);
 if(!fac)return;
 int ind=View->ItemIndex;
 int tind=View->TopIndex;
 if(ind<=0&&vnode->type!=tntFactionBattle)return;
 ind--;
 int bind=-1;
 if(vnode->type==tntFactionBattles)
 {
   if(fac->battleindexs.size())
     bind=ind;
 }else if(vnode->type==tntFactionBattle)
 {
   bind=vnode->value%10000;
 }
 if(bind>=0)
 {
   fac->DeleteBattle(bind);
   MakeTreeView();
   return;
 }

/* AnsiString str=View->Items->Strings[ind];
 ind=fac->battles->IndexOf(str);
 if(ind==-1)return;*/
 fac->battles->Delete(ind);
 curTurn->Modified=true;
 if(fac->battles->Count){
  int ind=View->ItemIndex;
  ViewShow();
  View->ItemIndex=ind;
  View->TopIndex=tind;
  ViewClick(this);
 }else MakeTreeView();
}
void __fastcall TAtlaForm::miFacClearEventClick(TObject *Sender)
{
 int facnum=popupFactionMessages->Tag;
 AFaction *fac=curFactions->GetFaction(abs(facnum));
 if(!fac)return;
 int ind=View->ItemIndex;
 int tind=View->TopIndex;
 if(ind<=0)return;
 if(facnum>0){
  ind--;
 }else{
  AnsiString str=View->Items->Strings[ind];
  ind=fac->events->IndexOf(str);
  if(ind<0)return;
 }
 fac->events->Delete(ind);
 curTurn->Modified=true;
 if(fac->events->Count){
  int ind=View->ItemIndex;
  ViewShow();
  View->ItemIndex=ind;
  View->TopIndex=tind;
  ViewClick(this);
 }else MakeTreeView();
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::edFindUnitKeyPress(TObject *Sender, char &Key)
{
 if(Key==13){
  AnsiString str=edFindUnit->Text;
  if(str.Pos(","))
  {
    str="("+str+")";
    char *s=str.c_str();
    ALocation *loc=GetNextLocation(s,false);
    if(!loc) return;
    ARegion *reg=curRegionList->Get(*loc);
    delete loc;
    if(!reg) return;
    GotoRegion(reg);
    edFindUnit->Text="";
    Key=0;
    return;
  }
  int num=atoi(str.c_str());
  AUnit *un;
  un=curRegion->GetUnit(num);
  if(!un){
   un=curRegionList->GetUnit(num);
   if(!un)return;
  }
  GotoUnit(un);
  edFindUnit->Text="";
  Key=0;
 }
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miAddOrderFormNewClick(TObject *Sender)
{
 if(!curUnit)return;
 AObject *obj=curUnit->baseobj;
 if(!obj)return;
 AnsiString mes;
 int num;
 AnsiString name,localdescr,descr;
 if(!FormNewUnitForm) FormNewUnitForm = new TFormNewUnitForm(this);
 else
 {
  FormNewUnitForm->edCountB->Value = 0;
  FormNewUnitForm->cbRepB->Checked = false;
  FormNewUnitForm->cbStudy->Checked = false;
  FormNewUnitForm->edReceive->Value = 0;
  FormNewUnitForm->edCopy->Value = 1;
  FormNewUnitForm->edMonS->Value = 1;
  FormNewUnitForm->edMonth->Value = 0;
  FormNewUnitForm->cbCommB->Checked = false;
  FormNewUnitForm->cbAllB->Checked = false;
  FormNewUnitForm->cbCommS->Checked = false;
  FormNewUnitForm->cbRepS->Checked = false;
  FormNewUnitForm->cbRepR->Checked = false;

  FormNewUnitForm->edName->Text = "";
  FormNewUnitForm->edLocalDescr->Text = "";
  FormNewUnitForm->edDescr->Text = "";

  FormNewUnitForm->PrepareReg();
 }
 TFormNewUnitForm *frm = FormNewUnitForm;

  while(1){
    int res = frm->ShowModal();
	if(res!=mrOk) return;

	num = atoi(frm->edAlias->Text.c_str());
	if(num <= 0)
	{
	  mes = "Bad alias \"";
	  mes+= frm->edAlias->Text;
	  mes+= "\"!";
	  Application->MessageBox(mes.c_str(), "Error creating unit", MB_OK);
	  continue;
	}
	num = -num;
	if(curRegion->GetUnit(num))
	{
	  mes = "Unit with alias ";
	  mes+= -num;
	  mes+= " already exist";
	  Application->MessageBox(mes.c_str(), "Error creating unit", MB_OK);
	  continue;
	}
	break;
  }
  name = frm->edName->Text;
  localdescr=frm->edLocalDescr->Text;
  descr=frm->edDescr->Text;

  for(int ii=0; ii < frm->edCopy->Value; ii++)
  {
	AUnit *newun=new AUnit(curTurn);
	while(curRegion->GetUnit(num)) num--;
	newun->num = num;
	newun->FormNew(curUnit);
	obj->units->Add(newun);
	int _guard = GUARD_NONE;
	if(frm->cbGuard->Checked) _guard = GUARD_GUARD;
	else if(frm->cbAvoid->Checked) _guard = GUARD_AVOID;
	int _flags=0;
	switch(frm->rgConsume->ItemIndex)
	{
	  case REVEAL_UNIT: _flags|=FLAG_CONSUMING_UNIT;break;
	  case REVEAL_FACTION: _flags|=FLAG_CONSUMING_FACTION;break;
	}
	if(frm->cbNoAid->Checked) _flags|=FLAG_NOAID;
	if(frm->cbHold->Checked) _flags|=FLAG_HOLDING;
	if(frm->cbAutoTax->Checked) _flags|=FLAG_AUTOTAX;
	if(frm->cbBehind->Checked) _flags|=FLAG_BEHIND;
	if(frm->cbNoCross->Checked) _flags|=FLAG_NOCROSS_WATER;
	if(frm->cbSharing->Checked) _flags|=FLAG_SHARING;
	if(optServSpoils) _flags|=AUnit::GetFlagFromSpoils(frm->rgSpoils->ItemIndex);
	  else if(frm->cbNoSpoils->Checked) _flags|=FLAG_NOSPOILS;
	newun->SetFlagOrders(_guard, frm->rgReveal->ItemIndex,_flags);
	if(name.Length())
	{
	  AOrderNameDescribe *ord = new AOrderNameDescribe;
	  ord->type = O_NAME;
	  ord->subtype = 0;
	  ord->value = name;
	  newun->orders->Add(ord);
	}
	if(localdescr.Length())
	{
	  AOrderLocDescr *ord = new AOrderLocDescr;
	  ord->text = localdescr;
	  newun->orders->Add(ord);
	}
	if(descr.Length())
	{
	  AOrderNameDescribe *ord = new AOrderNameDescribe;
	  ord->type = O_DESCRIBE;
	  ord->subtype = 0;
	  ord->value = descr;
	  newun->orders->Add(ord);
	}
	// Buy
	if(((frm->edCountB->Value > 0) || (frm->cbAllB->Checked)) && (frm->cbItemsB->ItemIndex >= 0))
	{
	  AOrderBuySell *ord = new AOrderBuySell();
	  ord->type = O_BUY;
	  ord->commented = frm->cbCommB->Checked;
	  ord->repeating = frm->cbRepB->Checked;
	  if(frm->cbAllB->Checked) ord->amount = -1; else ord->amount = frm->edCountB->Value;
	  AMarket *mt = ((AMarket*)frm->cbItemsB->Items->Objects[frm->cbItemsB->ItemIndex]);
	  ord->item = mt->type->abr;
	  newun->orders->Add(ord);
	}
	// Study
	if(frm->cbStudy->Checked && (frm->cbListS->ItemIndex >= 0))
	{
	  AOrderAbr *ord = new AOrderAbr();
	  ord->type = O_STUDY;
	  ord->commented = frm->cbCommS->Checked;
	  ord->repeating = frm->cbRepS->Checked;
	  ASkillType *stype = ((ASkillType*)frm->cbListS->Items->Objects[frm->cbListS->ItemIndex]);
	  ord->abr = stype->abr;
	  newun->orders->Add(ord);
	}
	// Get
	if((frm->edReceive->Value > 0) && (frm->cbTargetsU->ItemIndex >= 0))
	{
	  AOrderGive *ord = new AOrderGive;
	  AUnit *un = ((AUnit*)frm->cbTargetsU->Items->Objects[frm->cbTargetsU->ItemIndex]);
	  ord->tarnum = num;
	  ord->target = newun;
	  ord->item   = "SILV";
	  ord->count  = frm->edReceive->Value;
	  ord->repeating = frm->cbRepR->Checked;
	  un->orders->Add(ord);
	}

	newun->PrepareOrders();
  }

  curTurn->Modified=true;
  curRegion->runned=false;
  MakingTree++;
  OnSetCurRegion();
  MakingTree--;
  VTreeNode *vnode=FindTreeNode(tntUnit, num);
  if(vnode) TreeViewSelected=vnode->node;
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miCopyNewUnitClick(TObject *Sender)
{
  int num=curRegion->GetNewUnitNum();
  if(curRegion->GetUnit(num)){
    AnsiString mes="Unit with alias ";
    mes+=-num;
    mes+=" already exist";
    Application->MessageBox(mes.c_str(),"Error creating unit",MB_OK);
    return;
  }
  AUnit *newun=new AUnit(curTurn);
  newun->num=num;
  newun->FormNew(curUnit);
  curUnit->baseobj->units->Add(newun);
  foreach(curUnit->orders)
  {
    AOrder *_ord=*(AOrder**) iter;
    if(_ord->type == O_MESSAGE)
    {
      AOrderMessage *ord=(AOrderMessage*)_ord;
      AOrder* _bord=ord->base;
      if(!_bord) continue;
      switch(_bord->type)
      {
        case O_GIVE:
        {
          AOrderGive *bord=(AOrderGive*)_bord;
          AOrderGive *nord=bord->CreateCopy();
          nord->tarnum=num;
          bord->orders->Add(nord);
        }
        break;
        case O_TEACH:
        {
          AOrderTeach *bord=(AOrderTeach*)_bord;
          bord->AddTarget(num,newun);
        }
        break;
      }
      continue;
    }
    AnsiString str=_ord->WriteTemplate();
    AOrder *neword=ParseOrder(str);
    if(!neword) continue;
    newun->orders->Add(neword);
    if(_ord->type == O_TURN)
    {
      foreach(((AOrderTurn*)_ord)->torders)
      {
        AOrder *tord=*(AOrder**) iter;
        AnsiString str = tord->WriteTemplate();
        AOrder *newTord = ParseOrder(str);
        if(!newTord) continue;
        newTord->turnorder = true;
        ((AOrderTurn*)neword)->torders->Add(newTord);
      }
    }
  }
  newun->PrepareOrders();
  curTurn->Modified=true;
  curRegion->runned=false;
  MakingTree++;
  OnSetCurRegion();
  MakingTree--;
  VTreeNode *vnode=FindTreeNode(tntUnit,num);
  if(vnode) TreeViewSelected=vnode->node;
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miDeleteNewUnitClick(TObject *Sender)
{
 if(!curUnit)return;
 if(curUnit->num>0)return;
 AnsiString mes;
 mes="You are sure to delete this unit?\n";
 mes+=curUnit->FullName();
 if(Application->MessageBox(mes.c_str(),"Deleting unit",MB_YESNO)!=IDYES)return;
 foreachr(curUnit->orders)
 {
   AOrder *_ord=*(AOrder**)iter;
   if(_ord->type!=O_MESSAGE) continue;
   AOrder *bord=((AOrderMessage*)_ord)->base;
   if(!bord) continue;
   if(bord->type!=O_GIVE) continue;
   if(bord->repeating) continue;
   bord->orders->Delete(bord);
 }
 curUnit->baseobj->units->Delete(curUnit->num);
 curUnit=0;
 curTurn->Modified=true;
 curRegion->runned=false;
 OnSetCurRegion();
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miSetTaxIncClick(TObject *Sender)
{
 if(!curRegion)return;
 AnsiString mes=curRegion->taxincome;
 if(!InputQuery("Tax income","Enter tax producivity (0 - auto)",mes))return;
 curRegion->taxincome=atoi(mes.c_str());
 curTurn->Modified=true;
 curRegion->runned=false;
 ViewShow();
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miSetWorkIncClick(TObject *Sender)
{
 if(!curRegion)return;
 AnsiString mes=curRegion->workincome;
 if(!InputQuery("Work income","Enter work producivity (0 - auto)",mes))return;
 curRegion->workincome=atoi(mes.c_str());
 curTurn->Modified=true;
 curRegion->runned=false;
 ViewShow();
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miSetEntIncClick(TObject *Sender)
{
 if(!curRegion)return;
 AnsiString mes=curRegion->entincome;
 if(!InputQuery("Entartainment income","Enter entertainment producivity (0 - auto)",mes))return;
 curRegion->entincome=atoi(mes.c_str());
 curTurn->Modified=true;
 curRegion->runned=false;
 ViewShow();
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::popupViewRegionPopup(TObject *Sender)
{
 AnsiString s="&Goto last info turn (";
// int lastturn=curRegion->oldexitsinfo;
 int lastturn=curRegion->oldinfoturnnum;
 s+=lastturn;
 s+=")";
 miGotoLastInfo->Visible=(lastturn>=0)&&curRegion->hasInfo;
 miGotoLastInfo->Caption=s;
 miWeather->Visible=!optConquest;
 if(!optConquest)
 for(int i=0;i<12;i++){
  TMenuItem *mi=miWeather->Items[i];
  int newturn=curTurn->num+i+1;
  AnsiString s="Turn ";
  s+=newturn;
  s+=": ";
  s+=TurnNumberToString(newturn);
  s+=": "+WeatherRegions->GetWeather(curRegion->xloc,curRegion->yloc,
   curRegion->zloc,newturn);
  mi->Caption=s;
 }
 miCopyToClipboard4->Visible=popupViewRegion->PopupComponent==View;
 miSetRegDescription->Enabled=lastturn==curTurn->num;
 miAddToRegionList->Enabled=curRegion->hasInfo;
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miAddOrderSimpleClick(TObject *Sender)
{
 TMenuItem *mi=dynamic_cast<TMenuItem*>(Sender);
 if(!mi)return;
 if(mi->Tag==0)return;
 AOrderSimple *ord=new AOrderSimple;
 ord->type = OrderTypes(mi->Tag);
 EditOrder(0,ord);
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miAddOrderByteClick(TObject *Sender)
{
 TMenuItem *mi=dynamic_cast<TMenuItem*>(Sender);
 if(!mi)return;
 if(mi->Tag<=0)return;
 AOrderByte *ord=new AOrderByte;
 ord->type=OrderTypes(mi->Tag);
 EditOrder(0,ord);
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miAddOrderAbrClick(TObject *Sender)
{
 TMenuItem *mi=dynamic_cast<TMenuItem*>(Sender);
 if(!mi)return;
 if(mi->Tag<=0)return;
 AOrderAbr *ord=new AOrderAbr;
 ord->type=OrderTypes(mi->Tag);
 EditOrder(0,ord);
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miAddOrderInt(TObject *Sender)
{
 TMenuItem *mi=dynamic_cast<TMenuItem*>(Sender);
 if(!mi)return;
 if(mi->Tag<=0)return;
 AOrderInt *ord=new AOrderInt;
 ord->type=OrderTypes(mi->Tag);
 EditOrder(0,ord);
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miAddOrderGiveClick(TObject *Sender)
{
  EditOrder(0,new AOrderGive);
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miAddOrderAutoGiveClick(TObject *Sender)
{
  EditOrder(0,new AOrderAutoGive);
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miAddOrderAutoMoveClick(TObject *Sender)
{
 if(!curUnit) return;
 AOrder *ord=new AOrderAutoMoving;
// curUnit->orders->Add(ord);
// EditOrder(curUnit->orders->count-1,0);
 EditOrder(0,ord);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderAutoWarningClick(TObject *Sender)
{
  EditOrder(0,new AOrderAutoWarning);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderBuySellClick(TObject *Sender)
{
  TMenuItem *mi=dynamic_cast<TMenuItem*>(Sender);
  if(!mi)return;
  if(mi->Tag<=0)return;
  AOrderBuySell *ord=new AOrderBuySell;
  ord->type=OrderTypes(mi->Tag);
  EditOrder(0,ord);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderIntClick(TObject *Sender)
{
 TMenuItem *mi=dynamic_cast<TMenuItem*>(Sender);
 if(!mi)return;
 if(mi->Tag==0)return;
 AOrderInt *ord=new AOrderInt;
 ord->type=OrderTypes(mi->Tag);
 EditOrder(0,ord);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderMovingClick(TObject *Sender)
{
 TMenuItem *mi=dynamic_cast<TMenuItem*>(Sender);
 if(!mi)return;
 if(mi->Tag==0)return;
 AOrderMoving *ord=new AOrderMoving;
 ord->type=OrderTypes(mi->Tag);
 EditOrder(0,ord);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderStealClick(TObject *Sender)
{
 AOrderSteal*ord=new AOrderSteal;
 EditOrder(0,ord);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderAssassinateClick(TObject *Sender)
{
 AOrderAssassinate*ord=new AOrderAssassinate;
 EditOrder(0,ord);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderAttackClick(TObject *Sender)
{
 AOrderAttack*ord=new AOrderAttack;
 EditOrder(0,ord);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::bnRunOrdersClick(TObject *Sender)
{
  curTurn->RunOrders();
  if(MapMode==2||MapMode==0||optAlwaysShowMovement) DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::bnRunAutoOrdersClick(TObject *Sender)
{
 curTurn->RunAutoOrders();
 ViewShow();
 if(MapMode==2||MapMode==0||optAlwaysShowMovement)
  DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miGotoOrderTargetClick(TObject *Sender)
{
 if(!curUnit)return;
 int ind=popupViewUnit->Tag;
 AOrder *ord=curUnit->GetOrder(ind);
 if(!ord)return;
 int num=0;
 if(ord->type==O_MESSAGE){
  AOrderMessage *o=dynamic_cast<AOrderMessage*>(ord);
  if(o->target) num=o->target->num;
 }else if(ord->type==O_GIVE){
  AOrderGive *o=dynamic_cast<AOrderGive*>(ord);
  num=o->tarnum;
 }else if(ord->type==O_STEAL){
  AOrderSteal *o=dynamic_cast<AOrderSteal*>(ord);
  num=o->tarnum;
 }else if(ord->type==O_ASSASSINATE){
  AOrderAssassinate *o=dynamic_cast<AOrderAssassinate*>(ord);
  num=o->tarnum;
 }
 if(!num)return;
 VTreeNode *vnode=FindTreeNode(tntUnit,num);
 if(vnode) TreeViewSelected=vnode->node;
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::edTurnNumberKeyPress(TObject *Sender, char &Key)
{
 if(Key==13){
  AnsiString str=edTurnNumber->Text;
  if(!str.Length())return;
  int i=atoi(str.c_str());
  if(i<0) i=0;
  if(i>=curTurns->count) i=curTurns->count-1;
  if(!curTurns->PreloadTurnIfNeed(i,true))
   return;
  i-=curTurnNumber;
  if(!i)return;
  ChangeTurn(i);
  View->SetFocus();
  edTurnNumber->Text="";
  Key=0;
 }
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miAddOrderTeachClick(TObject *Sender)
{
 EditOrder(0,new AOrderTeach);
}
//---------------------------------------------------------------------------
void TAtlaForm::ShowMapMode()
{
  switch(MapMode)
  {
    case 1: bnProdMode->Down = true; break;
    case 2: bnWedgeMode->Down = true; break;
    case 3: btInfMode->Down = true; break;
    default: bnTownMode->Down = true; break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::bnTownModeClick(TObject *Sender)
{
 MapMode=0;
 ShowMapMode();
 DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::bnProdModeClick(TObject *Sender)
{
 MapMode=1;
 ShowMapMode();
 DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::bnWedgeModeClick(TObject *Sender)
{
 MapMode=2;
 ShowMapMode();
 DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miProdModeSetupClick(TObject *Sender)
{
 TProdModeSetupForm *frm=new TProdModeSetupForm(this,ProdModeAttr);
 if(frm->ShowModal()==mrOk){
  ProdModeAttr=frm->mode;
  DrawMap();
 }
 delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miWedgemodesettingsClick(TObject *Sender)
{
 TWedgeModeSetupForm *frm=new TWedgeModeSetupForm(this);
 if(frm->ShowModal()==mrOk){
  UpdateWedgeGroup();
  DrawMap();
 }
 delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAdvProdsClick(TObject *Sender)
{
 if(!curTurn->num) return;
 ATurn *prvturn=curTurns->Get(curTurn->num-1);
 if(!prvturn) return;
 ARegionList *prvrl=prvturn->RegionList;
 if(curTurn->RegionList->UpdateAdvProdsFromPrev(prvrl,true))
 {
	 curTurn->VerifyNextTurn(true);
	 DrawMap();
 }
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miGameSaveClick(TObject *Sender)
{
 SaveGameBase();
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miVerifynextturnClick(TObject *Sender)
{
 curTurn->VerifyNextTurn(true);
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miAdvancedProductsFixClick(TObject *Sender)
{
 AnsiString mes;
 foreach(curTurns){
  ATurn *turn=*(ATurn**)iter;
  if(!turn) continue;
  foreach(turn->RegionList){
   ARegion* reg=*(ARegion**)iter;
   ATerrainType *ttype=TerrainTypes->Get(reg->type);
   for(int i=reg->products->count-1;i>=0;i--){\
    AMarket*prod=reg->products->Get(i);
    if(prod->amount) continue;
    AItemType *itype=prod->type;//ItemTypes->Get(prod->type,4);
    if(ttype->CanProduceAdv(itype->abr)) continue;
    reg->products->Delete(i);
    mes="Null advanced product \"";
    mes+=itype->name;
    mes+="\" removed in ";
    mes+=reg->FullName();
    AddEvent(mes);
    turn->Modified=true;
   }
  }
 }
 DrawMap();
 AddEvent("Advanced products fixed");
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::TreeViewCompare(TObject *Sender,
      TTreeNode *Node1, TTreeNode *Node2, int Data, int &Compare)
{
  int strcomp=Node1->Text.AnsiCompare(Node2->Text);
  VTreeNode *vn1=(VTreeNode*)(Node1->Data);
  if(!vn1) throw Exception("Error1 in TreeCompare");
  VTreeNode *vn2=(VTreeNode*)(Node2->Data);
  if(!vn2) throw Exception("Error2 in TreeCompare");

  if(vn1->type==tntObject)
  {
	if(vn2->type==tntObject)
	{
	  Compare=vn1->value-vn2->value;
	} else {
	  Compare = 1;
	}
  } else
  if(vn2->type == tntObject)
  {
	Compare = -1;
  } else
  if(vn1->type == tntFactionBattles)
  {
	Node1->Expanded = false;
	Compare =-1;
  } else
  if(vn2->type == tntFactionBattles)
  {
	Node2->Expanded = false;
	Compare = 1;
  } else

 if(vn1->type==tntUnit&&vn2->type==tntUnit){
  AUnit *un1=(AUnit*) vn1->data;
  if(!un1) throw Exception("Error3 in TreeCompare");
  AUnit *un2=(AUnit*) vn2->data;
  if(!un2) throw Exception("Error4 in TreeCompare");
  if(un1->IsLocal()){
   if(un2->IsLocal()){
    Compare=strcomp;
   }else{
    Compare=-1;
   }
  }else if(un2->IsLocal()){
   Compare=1;
  }else{
   Compare=strcomp;
  }
 }else if(vn1->type==tntFactionBattle&&vn2->type==tntFactionBattle)
 {
   bool p1=(bool*)vn1->data;
   bool p2=(bool*)vn2->data;
   if(p1){
     if(p2) Compare=vn1->value-vn2->value;
     else Compare=-1;
   }else{
     if(p2) Compare=1;
     else Compare=vn1->value-vn2->value;
   }
 }else{
  Compare=strcomp;
 }
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miCopyOrderClick(TObject *Sender)
{
  if(!curUnit)return;
  int ind = popupViewUnit->Tag;
  AOrder *ord = curUnit->GetOrder(ind);
  if(!ord) return;
  orderBuf = ParseOrder(ord->WriteTemplate());
  if(!orderBuf) return;

  if(ord->type == O_TURN)
  {
	foreach(((AOrderTurn*)ord)->torders)
	{
	  AOrder *tord = *(AOrder**) iter;
	  AnsiString str = tord->WriteTemplate();
	  AOrder *newTord = ParseOrder(str);
	  if(!newTord) continue;
	  newTord->turnorder = true;
	  ((AOrderTurn*)orderBuf)->torders->Add(newTord);
	}

  }

}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miPasteOrderClick(TObject *Sender)
{
  if(!orderBuf) return;

  if(orderBuf->type == O_TURN)
  {
	AOrderTurn*neword = new AOrderTurn;
	foreach(((AOrderTurn*)orderBuf)->torders)
	{
	  AOrder *tord = *(AOrder**) iter;
	  AnsiString str = tord->WriteTemplate();
	  AOrder *newTord = ParseOrder(str);
	  if(!newTord) continue;
	  newTord->turnorder = true;
	  neword->torders->Add(newTord);
	}
	EditOrder(0, neword);
	return;
  }

  AOrder *ord = ParseOrder(orderBuf->WriteTemplate());
  if(!ord) return;
  EditOrder(0, ord);
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::Exitwithoutsaving1Click(TObject *Sender)
{
 if(MessageDlg("Exit without saving\nYou are sure?",mtWarning,TMsgDlgButtons()<<mbYes<<mbNo,0)!=mrYes) return;
 NoSave=true;
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::VerTime1Click(TObject *Sender)
{
 int t1=GetTickCount();
 for(int i=0;i<10;i++)
  curTurns->Get(0)->VerifyNextTurn();
//  LoadGameBase();
 int t2=GetTickCount();
 VerGameLoaded();
 ChangeTurn(0);
 PanelMapResize(this);
 t2-=t1;
 Beep();
 ShowMessage(AnsiString(t2));

}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::actEnableTempExecute(TObject *Sender)
{
 miTemporary->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderBuildClick(TObject *Sender)
{
  EditOrder(0,new AOrderBuild);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderCastClick(TObject *Sender)
{
 EditOrder(0,new AOrderCast);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miGotoLastInfoClick(TObject *Sender)
{
 if(!curRegion) return;
 if(curRegion->hasInfo&&curRegion->oldinfoturnnum==curTurn->num)
  return;
 char Key=13;
 edTurnNumber->Text=curRegion->oldinfoturnnum;
 edTurnNumberKeyPress(0,Key);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miStoreTurnsClick(TObject *Sender)
{
 TStoreForm *frm=new TStoreForm(this);
 int res=frm->ShowModal();
 delete frm;
 if(res!=mrOk) return;
 SaveGameBase();
/* LoadGameBase();
 VerGameLoaded();*/
 ChangeTurn(0);
}
void __fastcall TAtlaForm::actShowRegInfoExecute(TObject *Sender)
{
 bool ch=!actShowRegInfo->Checked;
 actShowRegInfo->Checked=ch;
 RegInfoForm->ShowRegInfo=ch;
}

void __fastcall TAtlaForm::actShowMapExecute(TObject *Sender)
{
 bool ch=!actShowMap->Checked;
 actShowMap->Checked=ch;
 MapForm->ShowMap=ch;
}
void __fastcall TAtlaForm::miSaveReportClick(TObject *Sender)
{
 foreach(curFactions){
  AFaction *fac=*(AFaction**)iter;
//  if(fac->number!=198) continue; //temp
  if(!fac->local)continue;
  int facnum=fac->number;
  AnsiString fName=GetReportName(facnum);
  if(!SaveReport(fName,facnum)){
   AnsiString str="Error saving report for faction ";
   str+=fac->FullName();
   AddEvent(str);
   break;
  }
 }
}
void __fastcall TAtlaForm::miSaveMapClick(TObject *Sender)
{
 {
  TSaveMapForm *frm=new TSaveMapForm(this);
  if(frm->ShowModal()!=mrOk) return;
  delete frm;
 }
 AnsiString fName=GetGameFileName(ReportsDir+"map."+curTurn->num);
/* int mapoptions=0
//                |roLocalAsNonlocalUnit
//                |roNonlocalUnit
//                |roUserObject
                |roNonuserObject
                |roAllRegion
                |roRegionInfo
 ;*/
 if(!SaveMap(fName,optSaveMap)){
  AnsiString str="Error saving map";
  AddEvent(str);
 }
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAtlaCartClick(TObject *Sender)
{
 OpenDialog->Filter="All files|*.*";
 if(OpenDialog->Execute()){
  OpenDialog->InitialDir=ExtractFilePath(OpenDialog->FileName);
  bool ok=true;
  TStringList *list=new TStringList;
  list->Assign(OpenDialog->Files);
  list->Sort();
  for(int i=0;ok&&i<list->Count;i++){
   if(ImportAtlaCart(list->Strings[i])) ok = false;
  }
  delete list;
  ChangeTurn(0,false,true);
	if(ok) curTurn->VerifyNextTurn(true);
 }
}
//---------------------------------------------------------------------------
bool LoadIniColor(TColor &color,AnsiString paramname,TIniFile *ini){
 AnsiString str=ini->ReadString(SettingsSect,paramname,"");
 if(!str.Length()) return false;
 try{
  color=StringToColor(str);
 }catch(...){
  return false;
 }
 return true;
}
void TAtlaForm::LoadOptions()
{
 TIniFile *ini=new TIniFile(IniName);
 try{
  actShowMap->Checked=atoi(ini->ReadString(SettingsSect,"ShowMap","").c_str());
  actShowRegInfo->Checked=atoi(ini->ReadString(SettingsSect,"ShowRegInfo","").c_str());
  optShowEvents=atoi(ini->ReadString(SettingsSect,"ShowEvents","1").c_str());
  optRunRegionOrders=atoi(ini->ReadString(SettingsSect,"RunRegionOrders","1").c_str());
  optTownNameLimit=atoi(ini->ReadString(SettingsSect,"TownNameLimit","0").c_str());
  optMaximizeWindow=atoi(ini->ReadString(SettingsSect,"MaximizeWindow","1").c_str());
  WindowState=optMaximizeWindow?wsMaximized:wsNormal;
  optDontShowEvents=atoi(ini->ReadString(SettingsSect,"DontShowEvents","4").c_str());
  optSaveMap=atoi(ini->ReadString(SettingsSect,"SaveMap","0").c_str());
  optShowLDescFirst=atoi(ini->ReadString(SettingsSect,"ShowLDescFirst","1").c_str());
  optCopyOnSaveOrders=atoi(ini->ReadString(SettingsSect,"CopyOnSaveOrders","1").c_str());
  optColorUnit=atoi(ini->ReadString(SettingsSect,"ColorUnit","").c_str());
  try{
   AnsiString sc=ini->ReadString(SettingsSect,"HideShortCut","");
   if(sc.Length())
    actHideClient->ShortCut=TextToShortCut(sc);
  }catch(...){}
  try{
   AnsiString str=ini->ReadString(SettingsSect,"WWidth","");
   PanelViewer->Width=str.ToInt();
  }catch(...){}
  try{
   AnsiString str=ini->ReadString(SettingsSect,"WHeight","");
   TreeView->Height=str.ToInt();
  }catch(...){}
  TViewMagesForm::ColsWidth=atoi(ini->ReadString(SettingsSect,"MagesColsWidth","150").c_str());
  TViewUnitsForm::height=atoi(ini->ReadString(SettingsSect,"UnitsHeight","180").c_str());
  TViewUnitsForm::SetColWidthsStr(ini->ReadString(SettingsSect,"UnitsColWidths",""));
  ShowUnitsOnStart=atoi(ini->ReadString(SettingsSect,"ShowUnits","").c_str());
  optUseOldMail=atoi(ini->ReadString(SettingsSect,"UseOldMail","").c_str());
  optAutoSaveTimer=atoi(ini->ReadString(SettingsSect,"AutoSaveTimer","").c_str());
  optUpdateMovingMap=atoi(ini->ReadString(SettingsSect,"UpdateMovingMap","1").c_str());
  optAlwaysShowMovement=atoi(ini->ReadString(SettingsSect,"AlwaysShowMovement","1").c_str());
  MapMode=atoi(ini->ReadString(SettingsSect,"MapMode","0").c_str());
  ShowMapMode();
 }__finally{
  delete ini;
 }
}
void SaveIniColor(TColor color,AnsiString paramname,TIniFile *ini){
 ini->WriteString(SettingsSect,paramname,ColorToString(color));
}
void TAtlaForm::SaveOptions()
{
 TIniFile *ini=new TIniFile(IniName);
 try{
  ini->WriteString(SettingsSect,"ShowMap",int(actShowMap->Checked));
  ini->WriteString(SettingsSect,"ShowRegInfo",int(actShowRegInfo->Checked));
  ini->WriteString(SettingsSect,"ShowEvents",int(optShowEvents));
  ini->WriteString(SettingsSect,"RunRegionOrders",int(optRunRegionOrders));
  ini->WriteString(SettingsSect,"TownNameLimit",int(optTownNameLimit));
  ini->WriteString(SettingsSect,"MaximizeWindow",int(optMaximizeWindow));
  ini->WriteString(SettingsSect,"DontShowEvents",optDontShowEvents);
  ini->WriteString(SettingsSect,"SaveMap",optSaveMap);
  ini->WriteString(SettingsSect,"ShowLDescFirst",int(optShowLDescFirst));
  ini->WriteString(SettingsSect,"CopyOnSaveOrders",int(optCopyOnSaveOrders));
  ini->WriteString(SettingsSect,"ColorUnit",int(optColorUnit));

  ini->WriteString(SettingsSect,"HideShortCut",ShortCutToText(actHideClient->ShortCut));

  ini->WriteString(SettingsSect,"WWidth",PanelViewer->Width);
  ini->WriteString(SettingsSect,"WHeight",TreeView->Height);

  ini->WriteString(SettingsSect,"MagesColsWidth",TViewMagesForm::ColsWidth);
  ini->WriteString(SettingsSect,"UnitsHeight",TViewUnitsForm::height);
  ini->WriteString(SettingsSect,"UnitsColWidths",TViewUnitsForm::GetColWidthsStr());
  ini->WriteString(SettingsSect,"ShowUnits",ViewUnitsForm?"1":"0");
  ini->WriteString(SettingsSect,"UseOldMail",optUseOldMail?"1":"0");
  ini->WriteString(SettingsSect,"AutoSaveTimer",optAutoSaveTimer);
  ini->WriteString(SettingsSect,"UpdateMovingMap",optUpdateMovingMap?"1":"0");
  ini->WriteString(SettingsSect,"AlwaysShowMovement",optAlwaysShowMovement?"1":"0");
  ini->WriteString(SettingsSect,"MapMode",MapMode);
 }__finally{
  delete ini;
 }
}
void __fastcall TAtlaForm::miOptionsClick(TObject *Sender)
{
 TOptionsForm *frm=new TOptionsForm(this);
 frm->ShowModal();
 delete frm;
 SetAutoSaveTimer();
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miReceiveClick(TObject *Sender)
{
 TEditOrderReceiveForm *frm=new TEditOrderReceiveForm(this);
 if(frm->ShowModal()==mrOk)
	 curTurn->Modified=true;
 delete frm;
 ViewShow();
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miAboutClick(TObject *Sender)
{
 TAboutForm *frm=new TAboutForm(this);
 frm->ShowModal();
 delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::actPrevUnitExecute(TObject *Sender)
{
 if(!curNode)return;
 VTreeNode *vnode;
 TTreeNode *findNode=curNode;
 bool needprev=true;
 do{
  while(findNode){
   if(needprev){
    findNode=findNode->GetPrev();
   }
   needprev=true;
   if(!findNode) continue;
   vnode=(VTreeNode*)findNode->Data;
   if(vnode->type!=tntUnit) continue;
   AUnit *unit=(AUnit*) vnode->data;
   if(!unit->IsLocal()) continue;
   TreeViewSelected=findNode;
   return;
  }
  if(!findNode){
   for(int i=curRegionList->IndexOf(curRegion)-1;i>=0;i--){
    ARegion *reg=curRegionList->Get(i);
    if(!reg->PresentLocals()) continue;
    GotoRegion(reg);
    findNode=TreeView->Items->Item[TreeView->Items->Count-1];
    needprev=false;
    break;
   }
  }
 }while(findNode);
}
void __fastcall TAtlaForm::actNextUnitExecute(TObject *Sender)
{
 if(!curNode)return;
 VTreeNode *vnode;
 TTreeNode *findNode=curNode;
 do{
  while(findNode){
   findNode=findNode->GetNext();
   if(!findNode) continue;
   vnode=(VTreeNode*)findNode->Data;
   if(vnode->type!=tntUnit) continue;
   AUnit *unit=(AUnit*) vnode->data;
   if(!unit->IsLocal()) continue;
   TreeViewSelected=findNode;
   return;
  }
  if(!findNode){
   for(int i=curRegionList->IndexOf(curRegion)+1,endi=curRegionList->count-1;i<endi;i++){
    ARegion *reg=curRegionList->Get(i);
    if(!reg->PresentLocals()) continue;
    GotoRegion(reg);
    findNode=TreeView->Items->GetFirstNode();
    break;
   }
  }
 }while(findNode);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::actPrevExprUnitExecute(TObject *Sender)
{
  if(!optPrevNextUnitExpr.Length())
  {
    actPrevUnitExecute(0);
    return;
  }
 if(!curNode)return;
 VTreeNode *vnode;
 TTreeNode *findNode=curNode;
 bool needprev=true;
 do{
  while(findNode){
   if(needprev){
    findNode=findNode->GetPrev();
   }
   needprev=true;
   if(!findNode) continue;
   vnode=(VTreeNode*)findNode->Data;
   if(vnode->type!=tntUnit) continue;
   AUnit *unit=(AUnit*) vnode->data;
   if(!unit->CalcExpr(optPrevNextUnitExpr)) continue;
   TreeViewSelected=findNode;
   return;
  }
  if(!findNode){
   for(int i=curRegionList->IndexOf(curRegion)-1;i>=0;i--){
    ARegion *reg=curRegionList->Get(i);
    bool found=false;
    foreach(reg){
      AObject *obj=*(AObject**)iter;
      foreach(obj->units)
      {
        AUnit *unit=*(AUnit**)iter;
        if(!unit->CalcExpr(optPrevNextUnitExpr)) continue;
        found=true;
        break;
      }
      if(found) break;
    }
    if(!found) continue;
    GotoRegion(reg);
    findNode=TreeView->Items->Item[TreeView->Items->Count-1];
    needprev=false;
    break;
   }
  }
 }while(findNode);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::actNextExprUnitExecute(TObject *Sender)
{
  if(!optPrevNextUnitExpr.Length())
  {
    actNextUnitExecute(0);
    return;
  }
 if(!curNode)return;
 VTreeNode *vnode;
 TTreeNode *findNode=curNode;
 do{
  while(findNode){
   findNode=findNode->GetNext();
   if(!findNode) continue;
   vnode=(VTreeNode*)findNode->Data;
   if(vnode->type!=tntUnit) continue;
   AUnit *unit=(AUnit*) vnode->data;
   if(!unit->CalcExpr(optPrevNextUnitExpr)) continue;
   TreeViewSelected=findNode;
   return;
  }
  if(!findNode){
   for(int i=curRegionList->IndexOf(curRegion)+1,endi=curRegionList->count-1;i<endi;i++){
    ARegion *reg=curRegionList->Get(i);
    bool found=false;
    foreach(reg){
      AObject *obj=*(AObject**)iter;
      foreach(obj->units)
      {
        AUnit *unit=*(AUnit**)iter;
        if(!unit->CalcExpr(optPrevNextUnitExpr)) continue;
        found=true;
        break;
      }
      if(found) break;
    }
    if(!found) continue;
    GotoRegion(reg);
    findNode=TreeView->Items->GetFirstNode();
    break;
   }
  }
 }while(findNode);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miSavereginfoClick(TObject *Sender)
{
/* int x=curRegion->xloc,y=curRegion->yloc,z=curRegion->zloc;
 FILE *f=fopen("atlaclient.log","w");
 if(!f) return;
 for(int i=13,endi=curTurns->count;i<endi;i++){
  bool packed=false;
  ATurn *turn=new ATurn;
  turn->num=i;
  turn->Read();
  if(turn->num==-1){
    packed=true;
    turn->Packed=true;
    turn->num=i;
    AnsiString fName=turn->GetFileName(1);
    if(!CopyFiles(GetGameFileName(StoreDir+fName),GetGameFileName(fName)))
/*     throw 1;* /
     turn->Packed=false;
    else turn->num=-1;
  }
  if(turn->num==i){
   ARegion *reg=turn->RegionList->Get(x,y,z);
   if(reg&&reg->population){
    fprintf(f,"%d",reg->population);
    fprintf(f,",%d",reg->wanteds->GetNum("GRAI"));
    fprintf(f,",%d",reg->wanteds->GetNum("LIVE"));
    fprintf(f,",%d",reg->wanteds->GetNum("HERB"));
    fprintf(f,",%d",reg->wanteds->GetNum("BAXE"));
    fprintf(f,",%d",reg->wanteds->GetNum("PARM"));
    fprintf(f,",%d",reg->wanteds->GetNum("LARM"));
    fprintf(f,",%d",reg->wanteds->GetNum("JEWE"));
    fprintf(f,",%d",reg->wanteds->GetNum("CAVI"));
    fprintf(f,"\n");
   }
  }
  if(packed){
   DeleteFiles(GetGameFileName(turn->GetFileName(1)));
  }
  delete turn;
 }
 fclose(f);*/
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::actCopyToClipboardClick(TObject *Sender)
{
 int ind=View->ItemIndex;
 AnsiString str;
 if(ind>=0)
  str=View->Items->Strings[ind];
 TClipboard *clp=Clipboard();
 clp->AsText=str;
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::popupFactionMessagesPopup(TObject *Sender)
{
 miCopyToClipboard3->Visible=popupFactionMessages->PopupComponent==View;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::popupViewDefaultPopup(TObject *Sender)
{
 miCopyToClipboard0->Visible=popupViewDefault->PopupComponent==View;
}
void __fastcall TAtlaForm::miMoveOrderUpClick(TObject *Sender)
{
  int ind = popupViewUnit->Tag;
  if(curUnit->orders->Swap(ind, ind-1))
  {
    curTurn->Modified = true;
    curRegion->runned = false;
    int i;
    for(i=0; i<vlistitems->count; i++)
    {
      VListItem *vli = vlistitems->Get(i);
      if(!vli) continue;
      if(vli->type != litUnitOrder) continue;
      if(vli->value == (ind-1)) break;
    }
    //int ind1=View->ItemIndex-1;
    ViewShow();
    View->ItemIndex = i;//ind;
    ViewClick(0);
  }
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miMoveOrderDownClick(TObject *Sender)
{
  int ind = popupViewUnit->Tag;
  if(curUnit->orders->Swap(ind, ind+1))
  {
    curTurn->Modified = true;
    curRegion->runned = false;
    ViewShow();
    int i;
    for(i=0; i<vlistitems->count; i++)
    {
      VListItem *vli = vlistitems->Get(i);
      if(!vli) continue;
      if(vli->type != litUnitOrder) continue;
      if(vli->value == (ind+1)) break;
    }
    View->ItemIndex = i;
    ViewClick(0);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miCountLoadedTurnsClick(TObject *Sender)
{
 AnsiString list;
 int lc=curTurns->LoadedCount(&list);
 AnsiString str="In memory loaded ";
 str+=lc;
 str+=" turns with size ";
 str+=curTurns->GetSize();
 str+=" bytes";
 AddEvent(str);
 AddEvent(list);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miMinimizeTurnsClick(TObject *Sender)
{
 curTurns->MinimizeTurns(curTurn->num);
 AddEvent("Turns minimized");
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miUpdateTurnsClick(TObject *Sender)
{
 int t=GetTickCount();
 ATurn *t0=curTurns->Get(0);
 t0->Modified=true;
 curTurns->UpdateTurns();
 t=GetTickCount()-t;
 AddEvent(AnsiString("Turns updated: ")+(t*.001)+" sec");
 ChangeTurn(0,false,true);
}

void __fastcall TAtlaForm::miCrystalBallClick(TObject *Sender)
{
 OpenDialog->Filter="All files|*.*";
 if(OpenDialog->Execute()){
  OpenDialog->InitialDir=ExtractFilePath(OpenDialog->FileName);
  bool ok=true;
  TStringList *list=new TStringList;
  list->Assign(OpenDialog->Files);
  list->Sort();
  for(int i=0;ok&&i<list->Count;i++){
   if(!ImportCrystalBall(list->Strings[i])) ok=false;
  }
  delete list;
  ChangeTurn(0,false,true);
	if(ok) curTurn->VerifyNextTurn(true);
 }
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miBattleSimClick(TObject *Sender)
{
 BattleSimForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miAddAttackerClick(TObject *Sender)
{
 BattleSimForm->AddAttacker(curUnit->num);
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miAddDefenderClick(TObject *Sender)
{
 BattleSimForm->AddDefender(curUnit->num);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miFacGotoBattleClick(TObject *Sender)
{
 if(!curNode) return;
 VTreeNode *vnode=(VTreeNode*)curNode->Data;
 AFaction *fac=curFactions->GetFaction(popupFactionMessages->Tag);
 if(!fac)return;
 int ind=View->ItemIndex-1;
 if(vnode->type==tntFactionBattle&&ind<0)
   ind=0;
 if(ind<0) return;
 if(vnode->type==tntFactionBattles)
 {
   if(fac->battleindexs.size())
     ind=fac->battleindexs[ind].ind;
 }else if(vnode->type==tntFactionBattle)
 {
   int i=vnode->value%10000;
   ind+=fac->battleindexs[i].ind;
 }
 AnsiString str=fac->battles->Strings[ind];
 ALocation loc;
 if(!IsBattleReport(str.c_str(),&loc)) return;
 ARegion *reg=curRegionList->GetByKey(GetXYZKey(loc.xloc,loc.yloc,loc.zloc));
 if(!reg)
   return;
 GotoRegion(reg);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miFacBattleToSimClick(TObject *Sender)
{
 if(!curNode) return;
 VTreeNode *vnode=(VTreeNode*)curNode->Data;
 AFaction *fac=curFactions->GetFaction(popupFactionMessages->Tag);
 if(!fac)return;
 int ind=View->ItemIndex-1;
 if(vnode->type==tntFactionBattle&&ind<0)
   ind=0;
 if(ind<0) return;
 if(vnode->type==tntFactionBattles)
 {
   if(fac->battleindexs.size())
     ind=fac->battleindexs[ind].ind;
 }else if(vnode->type==tntFactionBattle)
 {
   int i=vnode->value%10000;
   ind+=fac->battleindexs[i].ind;
 }
 if(BattleSimForm->LoadFromList(fac->battles,ind))
  miBattleSimClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miFacBattleToClipboardClick(TObject *Sender)
{
 if(!curNode) return;
 VTreeNode *vnode=(VTreeNode*)curNode->Data;
 AFaction *fac=curFactions->GetFaction(popupFactionMessages->Tag);
 if(!fac)return;
 int ind=View->ItemIndex-1;
 if(vnode->type==tntFactionBattles)
 {
   if(unsigned(ind)>=fac->battleindexs.size())
     ind=-1;
 }else if(vnode->type==tntFactionBattle)
 {
   ind=vnode->value%10000;
 }
 if(ind<0) return;
 int bind=fac->battleindexs[ind].ind;
 int eind=fac->battles->Count;
 if(ind+1<(int)fac->battleindexs.size())
   eind=fac->battleindexs[ind+1].ind;
 std::auto_ptr<TStringList> list(new TStringList);
 for(ind=bind;ind<eind;ind++)
   list->Add(fac->battles->Strings[ind]);
 TClipboard *clp=Clipboard();
 clp->AsText=list->Text;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miVerifyTypesClick(TObject *Sender)
{
 AnsiString str;
 foreach(ItemTypes){
  AItemType *itype=*(AItemType**) iter;
  str="Item Type \""+itype->name+"\" has no ";
  if(itype->type&AItemType::IT_MAN){
   AManType *mtype=ManTypes->Get(itype->abr);
   if(!mtype) AddEvent(str+"man info");
  }
  if(itype->type&AItemType::IT_MONSTER){
   AMonstrType *mtype=MonstrTypes->Get(itype->abr);
   if(!mtype) AddEvent(str+"monster info");
  }
  if(itype->type&AItemType::IT_WEAPON){
   AWeaponType *wtype=WeaponTypes->Get(itype->abr);
   if(!wtype) AddEvent(str+"weapon info");
  }
  if(itype->type&AItemType::IT_ARMOR){
   AArmorType *atype=ArmorTypes->Get(itype->abr);
   if(!atype) AddEvent(str+"armor info");
  }
  if(itype->type&AItemType::IT_MOUNT){
   AMountType *mtype=MountTypes->Get(itype->abr);
   if(!mtype) AddEvent(str+"mount info");
  }
  if(itype->type&AItemType::IT_BATTLE){
   ABatItemType *bitype=BatItemTypes->Get(itype->abr);
   if(!bitype) AddEvent(str+"battleitem info");
  }
  if(itype->hitchItem.Length()){
   AItemType *itype2=ItemTypes->Get(itype->hitchItem);
   if(!itype2) AddEvent("Item Type \""+itype->name+"\" has unknown Hitch item");
  }
 }
 foreach(SkillTypes){
  ASkillType *stype=*(ASkillType**)iter;
  str="Skill Type \""+stype->name+"\" "+stype->abr+" has no ";
  if(stype->flags&ASkillType::COMBAT){
   ASpellType *sptype=SpellTypes->Get(stype->abr);
   if(!sptype){
    AddEvent(str+"spell info");
    continue;
   }
   if(!sptype->GetCombatCount()){
    AddEvent(str+"COMBAT spell info");
    continue;
   }
  }
  if(stype->flags&ASkillType::CAST){
   ASpellType *sptype=SpellTypes->Get(stype->abr);
   if(!sptype){
    AddEvent(str+"spell info");
    continue;
   }
   if(!sptype->GetCastCount()){
    AddEvent(str+"CAST spell info");
    continue;
   }
  }
 }
 foreach(BatItemTypes){
  ABatItemType *bitype=*(ABatItemType**)iter;
  str="BattleItem Type \""+bitype->item+"\" has no ";
  if(bitype->flags&ABatItemType::SPECIAL){
   ASpellType *sptype=SpellTypes->Get(bitype->spec);
   if(!sptype){
    AddEvent(str+" has no spell info");
    continue;
   }
   if(!sptype->GetCombatCount()){
    AddEvent(str+" has no COMBAT spell info");
    continue;
   }
  }
 }
 foreach(MonstrTypes){
  AMonstrType *mtype=*(AMonstrType**)iter;
  str="Monster Type \""+mtype->item+"\" has no ";
  if(mtype->spec.Length()){
   ASpellType *sptype=SpellTypes->Get(mtype->spec);
   if(!sptype){
    AddEvent(str+" has no spell info");
    continue;
   }
   if(!sptype->GetCombatCount()){
    AddEvent(str+" has no COMBAT spell info");
    continue;
   }
  }
 }
 foreach(MountTypes){
  AMountType *mtype=*(AMountType**)iter;
  str="Mount Type for \""+ItemTypes->AbrToNames(mtype->items->Strings[0])
   +"\" has no ";
  if(mtype->spec.Length()){
   ASpellType *sptype=SpellTypes->Get(mtype->spec);
   if(!sptype){
    AddEvent(str+" has no spell info");
    continue;
   }
   if(!sptype->GetCombatCount()){
    AddEvent(str+" has no COMBAT spell info");
    continue;
   }
  }
 }
}
void __fastcall TAtlaForm::miShowWeatherRegionsClick(TObject *Sender)
{
 TShowWeatherRegionsForm *swr=new TShowWeatherRegionsForm(this);
 swr->ShowModal();
 delete swr;
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miLoadFromClipboardClick(TObject *Sender)
{
  curTurns->StoreLastParam();
  TClipboard *clb = Clipboard();
  if(!clb->HasFormat(CF_TEXT)) return;
  AnsiString tempFName = TempPath + "report.txt";
  int type = 0; //0-report,1-order,2-magereport
  TStringList *list = new TStringList;
  list->Text = clb->AsText;
  while(list->Count && (list->Strings[0].Length() == 0) )
	list->Delete(0);

  AnsiString ss = list->Text;

  if ( ss.Pos("\n Wages:") )
  {
	ss = StringReplace(ss, "\n ", "\n  ", TReplaceFlags()<<rfReplaceAll);
	list->Text = ss;
  }

  if(list->Count && list->Strings[0].Length() && list->Strings[0][1]=='#')
  {
	type=1;
	tempFName = TempPath+"order.txt";
  } else
  if(list->Strings[0].Length() && list->Strings[0][1] == '!')
  {
	type=2;
	tempFName=TempPath+"mages.txt";
  }
  list->SaveToFile(tempFName);
  delete list;
  if(type==1)
  {
	ProcessOrder(tempFName);
	ChangeTurn(0);
	return;
  } else
  if(type==2)
  {
	ImportMages(tempFName);
	ChangeTurn(0);
	return;
  }

  bool ok = ProcessReport(tempFName);
  ChangeTurn(0,false,true);
  miAdvProdsClick(0);

  if(!ok) return;
  if(!curFaction) return;
  int facnum=curFaction->number;
  AnsiString fname=GetReportName(facnum,StoreDir);
  if(Application->MessageBox("Store report","",MB_YESNO)!=ID_YES) return;
  if(!MoveFiles(tempFName,fname)) AddEvent("Report stored");
	else AddEvent("Error: Report not stored");
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miSetFlagsClick(TObject *Sender)
{
 TSetFlagsForm *frm=new TSetFlagsForm(this);
 int res=frm->ShowModal();
 delete frm;
 if(res!=mrOk)
  return;
 curRegion->runned=false;
 ViewShow();
 if(ViewUnitsForm)
   ViewUnitsForm->UpdateUnit(curUnit);
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miEditOrdersClick(TObject *Sender)
{
  if(!curUnit) return;
  TEditOrdersForm *frm=new TEditOrdersForm(this);
  TStrings *list=frm->Editor->Lines;
  AnsiString s;
  foreach(curUnit->orders)
  {
	AOrder *ord=*(AOrder**) iter;
	s=ord->WriteTemplate();
	int pos,oldpos=0;
	while((pos=s.SubString(oldpos+1,s.Length()).Pos("\n"))!=0)
	{
	  pos+=oldpos;
	  s.Insert("\r",pos);
	  oldpos=pos+1;
	}
	if(!s.Length())continue;
	list->Add(s);
  }

  if(frm->ShowModal()==mrOk)
  {
	AOrder *ord;
	AOrderTurn *turnord = 0;
	curUnit->orders->Clear();
	for(int i=0,endi=list->Count;i<endi;i++)
	{
	  s = list->Strings[i];
	  if(!s.Length()) continue;
	  ord = ParseOrder(s);
	  if(!ord)
	  {
		AddEvent("Error parsing order: " + s);
		continue;
	  }

	  if(ord->type == O_ENDTURN)
	  {
		if(!turnord) AddEvent("ENDTURN without TURN");
		turnord = 0;
		continue;
	  }

	  if(turnord){
		if(ord->type == O_TURN){
		  AddEvent("TURN order cannot be used in TURN");
		  continue;
		}
		if(ord->type==O_END||ord->type==O_UNIT)
		{
		  AddEvent("This order cannot be used here: "+s);
		  continue;
		}
		ord->turnorder = true;
		turnord->torders->Add(ord);
		continue;
	  }
	  if(ord->type==O_END||ord->type==O_UNIT
		 ||ord->type==O_FORM||ord->type==O_FORMEND)
	  {
		AddEvent("This order cannot be used here: "+s);
		continue;
	  }
	  if(ord->type == O_TURN)
	  {
		curUnit->orders->Add(ord);
		turnord = ((AOrderTurn*)ord);
        turnord->torders->Clear();
		continue;
	  }

	  curUnit->orders->Add(ord);
	} // for
   //if(turnord)
   //{
   //	  AddEvent("TURN order without ENDTURN. Added");
   //	  ord = ParseOrder("endturn");
   //	  curUnit->orders->Add(ord);
   //}
   curRegion->runned = false;
   curTurn->Modified=true;
  }

 delete frm;
 ViewShow();
 if(MapMode==0||optAlwaysShowMovement)
   DrawMap();
 if(ViewUnitsForm)
   ViewUnitsForm->UpdateUnit(curUnit);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miSendTimesRequestClick(TObject *Sender)
{
 AnsiString str="mailto:";
 str+=optServEMail;
 str+="?subject=";
 str+=StrForURL(optServTimesSubj);
 if(optServTimesBody.Length()){
  str+="&body=";
  str+=StrForURL(optServTimesBody);
 }
 ShellExecute(0,"open",str.c_str(),0,0,SW_SHOW);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miSendOrdersClick(TObject *Sender)
{
 if(curTurn->num!=curTurns->count-1)
 {
   ShowMessage("Warning: it's not last turn");
 }
 miSaveOrderClick(0);
 foreach(curFactions){
  AFaction *fac=*(AFaction**)iter;
  if(!fac->local)continue;
  int facnum=fac->number;
  AnsiString fName=GetOrderName(facnum);
  TStringList *list=new TStringList;
  try{
   list->LoadFromFile(fName);
   if(optUseOldMail)
   {
     Clipboard()->AsText=list->Text;
     AnsiString str="mailto:";
     str+=optServEMail;
     str+="?subject=";
     str+=StrForURL(optServOrdersSubj);
     ShellExecute(0,"open",str.c_str(),0,0,SW_SHOW);
     str="Orders for "+fac->FullName()+" copied to Clipboard";
     Application->MessageBox(str.c_str(),"Atlaclient",MB_OK);
   }else
   {
     SendMailMessage(fac->address,optServEMail,StrForURL(optServOrdersSubj),list->Text);
   }
  }catch(Exception&c){
  }
  delete list;
 }
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miMailClick(TObject *Sender)
{
 bool hasadress=optServEMail.Length();
 miSendTimesRequest->Enabled=hasadress&&optServTimesSubj.Length();
 miSendOrders->Enabled=hasadress&&optServOrdersSubj.Length();
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miViewFactionsClick(TObject *Sender)
{
  TViewFactionsForm *frm=new TViewFactionsForm(this);
  frm->ShowModal();
  delete frm;
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miSetRegDescriptionClick(TObject *Sender)
{
 if(!curRegion)return;
 AnsiString mes=curRegion->descr;
 if(!InputQuery("Region Description","",mes))return;
 curRegion->descr=mes;
 curTurn->Modified=true;
 curTurn->VerifyNextTurn();
 ViewShow();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderWithdrawClick(TObject *Sender)
{
  EditOrder(0,new AOrderWithdraw(false));
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderWithdrawClientClick(TObject *Sender)
{
 EditOrder(0,new AOrderWithdraw(true));
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddAttackersClick(TObject *Sender)
{
 BattleSimForm->AddAttackers(curUnit);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddDefendersClick(TObject *Sender)
{
 BattleSimForm->AddDefenders(curUnit);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::cbWedgeGroupChange(TObject *Sender)
{
 int ind=cbWedgeGroup->ItemIndex;
 if(ind<0) return;

 ind = int(cbWedgeGroup->Items->Objects[ind])+1;
 for(int i=0;i<6;i++){
  wedgeexpr[i]="";
 }
 for(int i=0;i<6&&ind<curTurns->expr_comms->Count/2;i++,ind++){
  AnsiString str=curTurns->expr_comms->Strings[ind*2];
  if(str==":") break;
  wedgeexpr[i]=str;
 }
 if(MapMode==2)
  DrawMap();
}
//---------------------------------------------------------------------------
void TAtlaForm::OnGameLoaded()
{
  VerGameLoaded();
  Plugins.OnGameLoaded();
  Plugins.UpdatePluginInfo();
  if(TViewUnitsForm::facdiap>=0)
    TViewUnitsForm::facdiap=-3;
  if(ViewUnitsForm)
  {
    ViewUnitsForm->MakeFacList();
    ViewUnitsForm->ReMake(true);
  }
  MapForm->ResetMap();
  ChangeTurn(0);
  UpdateWedgeGroup();
  if(ShowUnitsOnStart)
  {
    ShowUnitsOnStart=false;
    miViewUnitsClick(0);
  }
  if(VerifyAutoSaveFiles())
    SetAutoSaveTimer();
  Plugins.UpdatePluginInfo();
  Plugins.CreateMenu(miPluginsRoot,miRunPluginClick);
  Plugins.CreateConfigMenu(miConfigure,miConfigurePluginClick);
  Plugins.CreateOrderMenu(miAddOrder,PO_NORMAL,miPluginOrderClick);
  Plugins.CreateOrderMenu(miAddOrderAuto,PO_AUTO,miPluginOrderClick);
  Plugins.CreateOrderMenu(miAddClientOrder,PO_CLIENT,miPluginOrderClick);
  ShortCuts.SortOwners();
}
void TAtlaForm::UpdateWedgeGroup()
{
 TWedgeModeSetupForm::FillGroupList(cbWedgeGroup->Items);
}
void __fastcall TAtlaForm::miCopyRegToClipboardClick(TObject *Sender)
{
 int mapoptions=optSaveMap;//roNormalReport;
 TMemoryStream *strm=new TMemoryStream;
 if(curRegion->WriteReport(strm,mapoptions)){
  char c=0;
  strm->Write(&c,1);
  char *s=(char*)strm->Memory;
  AnsiString str=s;
  for(int i=1;i<=str.Length();i++){
   if(str[i]=='\n'){
    str.Insert("\r",i);
    i++;
   }
  }
  TClipboard *clp=Clipboard();
  clp->AsText=str;
 }
 delete strm;
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miDeleteLastTurnClick(TObject *Sender)
{
  if(curTurns->count<=1) return;
  if(Application->MessageBox("Realy delete last turn?","",MB_YESNO)!=IDYES) return;
  curTurns->DeleteLast();
  SaveGameBase();
  LoadGameBase();
  VerGameLoaded();
  ChangeTurn(0);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::actHideClientExecute(TObject *Sender)
{
  try{
    TrayIcon->Visible=true;
    //TrayIcon->Minimize();
  }catch(...){}
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::TrayIconRestore(TObject *Sender)
{
  try{
    TrayIcon->Visible=false;
  }catch(...){}
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miSetRegionUserInfoClick(TObject *Sender)
{
 if(!curRegion)return;
 AnsiString mes=curTurn->RegUserInfos[curRegion];
 if(!InputQuery("Region User Info","",mes))return;
 curTurn->RegUserInfos[curRegion]=mes;
 curTurn->Modified=true;
 ViewShow();
 if(MapMode==2)
  DrawMap();
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miViewMagesClick(TObject *Sender)
{
 TViewMagesForm *frm=new TViewMagesForm(this);
 frm->ShowModal();
 delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miViewTaxRegionsClick(TObject *Sender)
{
 TViewTaxRegionsForm *frm=new TViewTaxRegionsForm(this);
 frm->ShowModal();
 OnSetCurRegion();
 delete frm;
}
void TAtlaForm::GotoUnit(AUnit * un)
{
 if(!un) return;
 ARegion *reg=un->baseobj->basereg;
 GotoRegion(reg);
 VTreeNode *vnode=FindTreeNode(tntUnit,un->num);
 if(vnode)TreeViewSelected=vnode->node;
}
void TAtlaForm::GotoRegion(ARegion * reg)
{
 if(curRegion==reg)
  return;
 curRegionList->SetCurRegion(reg->xloc,reg->yloc,reg->zloc);
 ChangeLevel(0);
}
void __fastcall TAtlaForm::SetTreeViewSelected(TTreeNode * value)
{
  TreeView->Selected=value;
  TreeViewChange(0,value);
}
//---------------------------------------------------------------------------
TTreeNode * __fastcall TAtlaForm::GetTreeViewSelected()
{
  return TreeView->Selected;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miChangesCollectClick(TObject *Sender)
{
  if(Changes.Collect())
    Changes.Edit();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miChangesViewClick(TObject *Sender)
{
  Changes.Edit();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAnalyzeRegionPopClick(TObject *Sender)
{
  AnsiString str;
  curRegion->RunOrders(false);
//check production
  int activity=0,amount=0;
  foreach(curRegion->products)
  {
    AMarket *mark=*(AMarket**)iter;
    if(!(mark->type->type&AItemType::IT_NORMAL)) continue;
    amount+=mark->amount;
    int act=mark->price;
    if(act>mark->amount) act=mark->amount;
    activity+=act;
  }
  int bonus=activity*100/(2*amount);
  str+="Non town pop bonus ";
  str+=bonus;
  str+="%";
//check markets
  if(curRegion->towntype.Length())
  {
    int amt = 0;
    int tot = 0;
    map<AnsiString,int> amounts;
    ALocation loc;
    int bp;
    loc=*curRegion;
    FillConfirmedMarketMaxAmounts(loc,amounts,bp);
    foreach(curRegion->wanteds)
    {
      AMarket *mark=*(AMarket**)iter;
      if(mark->type->type&AItemType::IT_TRADE) continue;
      int markact=0;
      AnsiString item=mark->type->abr;
      foreach(curRegion){
        AObject *o=*(AObject **) iter;
        foreach(o->units){
          AUnit *u=*(AUnit**)iter;
          foreach(u->orders){
            AOrder *_ord=*(AOrder**)iter;
            if(_ord->type!=O_SELL)continue;
            if(_ord->commented)continue;

            AOrderBuySell *ord=dynamic_cast<AOrderBuySell*>(_ord);
            if(ord->item!=item)continue;
            markact+=ord->number;
          }
        }
      }
      int maxamt=amounts[mark->type->abr];
      if(maxamt)
        tot+=maxamt;
      else
        tot+=mark->amount;
      amt+=markact;
    }
    foreach(curRegion->saleds)
    {
      AMarket *mark=*(AMarket**)iter;
      if(!(mark->type->type&AItemType::IT_TRADE)) continue;
      int markact=0;
      AnsiString item=mark->type->abr;
      foreach(curRegion){
        AObject *o=*(AObject **) iter;
        foreach(o->units){
          AUnit *u=*(AUnit**)iter;
          foreach(u->orders){
            AOrder *_ord=*(AOrder**)iter;
            if(_ord->type!=O_BUY)continue;
            if(_ord->commented)continue;

            AOrderBuySell *ord=dynamic_cast<AOrderBuySell*>(_ord);
            if(ord->item!=item)continue;
            markact+=ord->number;
          }
        }
      }
      int maxamt=amounts[mark->type->abr];
      if(maxamt)
        tot+=5*maxamt;
      else
        tot+=5*mark->amount;
      amt+=5*markact;
    }
//РґРѕР±Р°РІР»РµРЅРёРµ РїРѕРґС‚РІРµСЂР¶РґРµРЅРЅС‹С… advanced, РєРѕС‚РѕСЂС‹С… РЅРµС‚Сѓ РІ СЃРїРёСЃРєРµ
    if(amounts.size())
    for(map<AnsiString,int>::iterator i=amounts.begin(),endi=amounts.end();i!=endi;i++)
    {
      AnsiString item=i->first;
      AItemType *itype=ItemTypes->Get(item,4);
      if(!itype) continue;
      if(!(itype->type&AItemType::IT_ADVANCED)) continue;
      AMarket *mark=curRegion->wanteds->GetMarket(itype);
      if(mark&&mark->amount) continue;
      tot+=i->second;
    }

    const CITY_POP=4000;
    int tarpop;
    if (tot)
    {
      tarpop = (CITY_POP * amt) / tot;
    }
    else
    {
      tarpop = 0;
    }
    // Let's bump tarpop up
    tarpop = (tarpop * 3) / 2;
    if (tarpop > CITY_POP) tarpop = CITY_POP;
    str+="\nTarget town pop ";
    str+=tarpop;
    double proc=(curRegion->population-2000-bp)/2000.;
    int tarpop2=0;
    int tarpop3=0;
    if(proc>0)
    {
      if(proc>1) proc=1;
      int advcount=0,magiccount=0;
      foreach(ItemTypes)
      {
        AItemType *itype=*(AItemType**)iter;
        if(curRegion->wanteds->GetMarket(itype)) continue;
        if(itype->type&AItemType::IT_MAGIC)
          magiccount++;
        if(!(itype->type&AItemType::IT_ADVANCED)) continue;
        if(amounts[itype->abr])
          continue;
        bool found=false;
        foreach(curRegion)
        {
          AObject *obj=*(AObject**)iter;
          foreach(obj->units)
          {
            AUnit *un=*(AUnit**)iter;
            if(!un->IsLocal()) continue;
            if(!un->items->GetItem(itype)) continue;
            found=true;
            break;
          }
          if(found) break;
        }
        if(found) continue;
        advcount++;
      }
      int tot2=tot;
      tot2+=advcount*(20*1.5)*proc/4;
//      tot2+=magiccount*(20*1.5)*proc/8;
      if (tot2)
        tarpop2 = (CITY_POP * amt) / tot2;
      else
        tarpop2 = 0;
      // Let's bump tarpop up
      tarpop2 = (tarpop2 * 3) / 2;
      if (tarpop2 > CITY_POP) tarpop2 = CITY_POP;
      int tot3=tot;
      tot3+=advcount*(20*2)*proc;
//      tot3+=magiccount*(20*1.5)*proc;
      if (tot3)
        tarpop3 = (CITY_POP * amt) / tot3;
      else
        tarpop3 = 0;
      // Let's bump tarpop up
      tarpop3 = (tarpop3 * 3) / 2;
      if (tarpop3 > CITY_POP) tarpop3 = CITY_POP;
      str+=" (";
      str+=tarpop2;
      str+=" - ";
      str+=tarpop3;
      str+=" with adv)";
    }
    ATurn *turn=curTurnNumber<curTurns->count-1?(curTurns->Get(curTurnNumber+1)):0;
    if(turn)
    {
      ARegion *newreg=turn->RegionList->Get(curRegion->xloc,curRegion->yloc,curRegion->zloc);
      if(newreg)
      {
        int popdiff=newreg->population-curRegion->population;
        int townpop=tarpop-5*popdiff;
        str+="\nCur town pop ";
        str+=townpop;
        int townpop2=tarpop2-5*popdiff;
        int townpop3=tarpop3-5*popdiff;
        if(tarpop2)
        {
          str+=" (";
          str+=townpop2;
          str+=" - ";
          str+=townpop3;
          str+=")";
        }
        str+="\nCur non town pop ";
        str+=curRegion->population-townpop;
        if(tarpop2)
        {
          str+=" (";
          str+=curRegion->population-townpop2;
          str+=" - ";
          str+=curRegion->population-townpop3;
          str+=")";
        }
      }
    }
  }
  ShowMessage(str);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miSetLevelSizeClick(TObject *Sender)
{
  int x=curRegion->xloc,y=curRegion->yloc,z=curRegion->zloc;
  TSetLevelSizeForm *frm=new TSetLevelSizeForm(this);
  frm->ShowModal();
  delete frm;
  curRegion=curRegionList->GetNearestRegion(x,y,z);
  ChangeTurn(0,false,true);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddOrderNameDescribeClick(TObject *Sender)
{
 AOrderNameDescribe* ord=new AOrderNameDescribe;
 ord->type=O_NAME;
 ord->subtype=0;
 EditOrder(0,ord);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miViewGatesClick(TObject *Sender)
{
  TViewGatesForm *frm=new TViewGatesForm(this);
  frm->ShowModal();
  delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::actShowRegionListExecute(TObject *Sender)
{
  bool ch=!actShowRegionList->Checked;
  actShowRegionList->Checked=ch;
  RegionListForm->Visible=ch;
  DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAddToRegionListClick(TObject *Sender)
{
  RegionListForm->AddRegion(curRegion);
  if(RegionListForm->Visible)
    DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miDeleteFromRegionListClick(TObject *Sender)
{
  RegionListForm->DeleteRegion(curRegion);
  if(RegionListForm->Visible)
    DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miSupportBuildingClick(TObject *Sender)
{
  TSupportBuildingForm *frm=new TSupportBuildingForm(this);
  frm->ShowModal();
  delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miSpecialistsClick(TObject *Sender)
{
  TViewSpecialistsForm *frm=new TViewSpecialistsForm(this);
  frm->ShowModal();
  delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miViewUnitsClick(TObject *Sender)
{
  if(!ViewUnitsForm)
  {
    new TViewUnitsForm(this);
    ViewUnitsForm->Align=alBottom;
    ViewUnitsForm->Width=ClientWidth;
    ViewUnitsForm->Parent=this;
    ViewUnitsForm->Show();
    Splitter3->Visible=true;
  }else{
    Splitter3->Visible=false;
    delete ViewUnitsForm;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miViewClick(TObject *Sender)
{
  miViewUnits->Checked=ViewUnitsForm!=0;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::pmShowEventsPopup(TObject *Sender)
{
  miRunOrdersOnChange->Checked=optRunRegionOrders;
  miShowDiscardWarnings->Checked=!(optDontShowEvents&dseDiscard);
  miShowNoMonthLongErrors->Checked=!(optDontShowEvents&dseNoMonth);
  miShowTaxWarnings->Checked=!(optDontShowEvents&dseIncome);
  miShowMoveError->Checked=!(optDontShowEvents&dseCantReach);
  miShowBorrowsWarnings->Checked=!(optDontShowEvents&dseBorrows);
  miShowTeachNonLocNotStudy->Checked=!(optDontShowEvents&dseTeachNonLocNotStudy);
  miShowRegionDataFound->Checked=!(optDontShowEvents&dseRegDataFound);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miRunOrdersOnChangeClick(TObject *Sender)
{
  optRunRegionOrders=!optRunRegionOrders;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miShowDiscardWarningsClick(TObject *Sender)
{
  optDontShowEvents^=dseDiscard;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miShowNoMonthLongErrorsClick(TObject *Sender)
{
  optDontShowEvents^=dseNoMonth;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miShowTaxWarningsClick(TObject *Sender)
{
  optDontShowEvents^=dseIncome;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miShowMoveErrorClick(TObject *Sender)
{
  optDontShowEvents^=dseCantReach;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miShowBorrowsWarningsClick(TObject *Sender)
{
  optDontShowEvents^=dseBorrows;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miShowRegionDataFoundClick(TObject *Sender)
{
  optDontShowEvents^=dseRegDataFound;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miShowTeachNonLocNotStudyClick(TObject *Sender)
{
  optDontShowEvents^=dseTeachNonLocNotStudy;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::pmLevelChangePopup(TObject *Sender)
{
  TMenuItem *items=pmLevelChange->Items;
  items->Clear();
  foreach(curRegionList->RegionArrays)
  {
	ARegionArray *ra=*(ARegionArray**)iter;
	AnsiString name=ra->GetName();
	TMenuItem *mi=new TMenuItem(items);
	mi->Caption=name;
	mi->Tag=ra->levelnum;
	mi->GroupIndex=1;
	mi->Checked=ra->levelnum==curRegion->zloc;
	mi->OnClick=pmLevelChangeClick;
	items->Add(mi);
  }
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::pmLevelChangeClick(TObject *Sender)
{
  TComponent *comp=dynamic_cast<TComponent*>(Sender);
  if(!comp) return;
  int levelnum=comp->Tag;
  ChangeLevel(levelnum-curRegion->zloc);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miUpdateNonLocalClick(TObject *Sender)
{
  if(curTurn->num==0) return;
  ATurn *prvturn=curTurns->Get(curTurn->num-1);
  if(!prvturn) return;
  ARegionList *prvrl=prvturn->RegionList;
  long t0=GetTickCount();
  foreach(curRegionList)
  {
    ARegion *reg=*(ARegion**)iter;
    foreach(reg)
    {
      AObject *obj=*(AObject**)iter;
      foreach(obj->units)
      {
        AUnit *un=*(AUnit**)iter;
        if(un->IsLocal()){
          if(un->faction->local)
            continue;
          if(un->GetLocalDescr().Length())
            continue;
          AUnit *prvun=prvrl->GetUnit(un->num);
          if(!prvun)
            continue;
          AnsiString ld=prvun->GetLocalDescr();
          if(!ld.Length())
            continue;
          AOrderLocDescr *ord=new AOrderLocDescr;
          ord->text=ld;
          un->orders->Add(ord);
          continue;
        }
        AUnit *prvun=prvrl->GetUnit(un->num);
        if(!prvun) continue;
        if(prvun->IsLocal()) continue;
        un->UpdateFromPrev(prvun);
      }
    }
  }
  curRegion->runned=false;
  curTurn->Modified=true;
  t0=GetTickCount()-t0;
  AddEvent(AnsiString("NonLocal units updated: ")+(t0/1000.));
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miExportMagesClick(TObject *Sender)
{
  ExportMages(ClientPath+"_mages.txt");
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miExportMageClick(TObject *Sender)
{
  if(!curUnit) return;
  if(!curUnit->IsLocal()) return;
  if(curUnit->num<0) return;
  if(!curUnit->IsMage()) return;
  Clipboard()->AsText=ExportMageStr(curUnit)+"\r\n";
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miShortCutsClick(TObject *Sender)
{
  TShortCutsForm *frm=new TShortCutsForm(this);
  frm->ShowModal();
  delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miClearImportedMagesClick(TObject *Sender)
{
  foreach(curRegionList)
  {
    ARegion *reg=*(ARegion**)iter;
    foreach(reg)
    {
      AObject *obj=*(AObject**)iter;
      foreach(obj->units)
      {
        AUnit *un=*(AUnit**)iter;
        if(!un->IsLocal()) continue;
        if(un->faction->local) continue;
        un->info=INFO_FACTION;
        AOrder *ord=un->orders->GetMonthLong();
        if(ord)
          ord->repeating=false;
      }
    }
  }
  curTurn->Modified=true;
  curTurn->RunOrders();
  OnSetCurRegion();
}
//---------------------------------------------------------------------------
void DisableAutoSaveTimer()
{
  AtlaForm->AutoSaveTimer->Enabled=false;
}
//---------------------------------------------------------------------------
void SetAutoSaveTimer()
{
  AtlaForm->AutoSaveTimer->Enabled=false;
  if(optAutoSaveTimer<=0)
    return;
  AtlaForm->AutoSaveTimer->Interval=optAutoSaveTimer*60000;
  AtlaForm->AutoSaveTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::AutoSaveTimerTimer(TObject *Sender)
{
  DisableEvents();
  int oldcurfac=0;
  onSaveNow = true;
  if(curFaction) oldcurfac=curFaction->number;
  foreach(curFactions){
    AFaction *fac=*(AFaction**)iter;
    if(!fac->local)continue;
    int facnum=fac->number;
    AnsiString fName=GetTempOrderName(facnum);
    SaveOrder(fName,facnum);
  }
  if(oldcurfac) curFactions->SetCurFaction(oldcurfac);
  curRegion->RunOrders(false);
  EnableEvents();
  onSaveNow = false;
}
//---------------------------------------------------------------------------
void DeleteAutoSaveFiles()
{
  foreach(curFactions)
  {
    AFaction *fac=*(AFaction**)iter;
    if(!fac->local)
      continue;
    AnsiString fname=AtlaForm->GetTempOrderName(fac->number);
    DeleteFile(fname);
  }
}
//---------------------------------------------------------------------------
AnsiString TAtlaForm::GetTempOrderName(int facnum)
{
  AnsiString name=ExtractFileName(GetOrderName(facnum));
  char buf[500];
  GetTempPath(sizeof(buf),buf);
  name=buf+name;
  return name;
}
//---------------------------------------------------------------------------
bool TAtlaForm::VerifyAutoSaveFiles()
{
  AnsiString mes;
  foreach(curFactions)
  {
    AFaction *fac=*(AFaction**)iter;
    if(!fac->local)
      continue;
    AnsiString fname=AtlaForm->GetTempOrderName(fac->number);
    if(!FileExists(fname))
      continue;
    mes="Backup orders found for ";
    mes+=fac->FullName();
    mes+=".\nLoad it?";
    int ret=Application->MessageBox(mes.c_str(),"",MB_YESNOCANCEL);
    if(ret==IDCANCEL)
    {
      AddEvent("Backup timer disabled");
      return false;
    }
    if(ret==IDNO)
      continue;
    ProcessOrder(fname);
    ChangeTurn(0);
  }
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miRunPluginClick(TObject *Sender)
{
  TMenuItem *mi=dynamic_cast<TMenuItem*>(Sender);
  if(!mi) return;
  if(mi->Count) return;
  Plugins.RunMenu(mi->Tag);
  if(PluginNeedMakeTree)
    OnSetCurRegion();
  else
    ViewShow();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miPluginOrderClick(TObject *Sender)
{
  TMenuItem*mi=dynamic_cast<TMenuItem*>(Sender);
  if(!mi) return;
  int type=mi->Tag/100000;
  switch(type)
  {
    case PO_NORMAL:
    {
      int mn,on;
      if(!Plugins.FindOrderPlugin(PO_NORMAL,mi->Caption.c_str(),mn,on))
        return;
      EditOrder(0,new AOrderPlugin(mn,on));
      break;
    }
    case PO_AUTO:
    {
      int mn,on;
      if(!Plugins.FindOrderPlugin(PO_AUTO,mi->Caption.c_str(),mn,on))
        return;
      AOrderPlugin *ord=new AOrderPlugin(mn,on);
      ord->OrderType=PO_AUTO;
      EditOrder(0,ord);
      break;
    }
    case PO_CLIENT:
      Plugins.RunClientOrder(mi->Caption);
      if(PluginNeedMakeTree)
        OnSetCurRegion();
      else
        ViewShow();
    break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miConfigurePluginClick(TObject *Sender)
{
  TComponent *comp=dynamic_cast<TComponent*>(Sender);
  if(!comp) return;
  Plugins.RunConfigure(comp->Tag);
  if(PluginNeedMakeTree)
    OnSetCurRegion();
  else
    ViewShow();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miPluginsUnloadClick(TObject *Sender)
{
  Plugins.ClearPlugins();
  Plugins.UpdatePluginInfo();
  Plugins.CreateMenu(miPluginsRoot,miRunPluginClick);
  Plugins.CreateConfigMenu(miConfigure,miConfigurePluginClick);
  Plugins.CreateOrderMenu(miAddOrder,PO_NORMAL,miPluginOrderClick);
  Plugins.CreateOrderMenu(miAddOrderAuto,PO_AUTO,miPluginOrderClick);
  Plugins.CreateOrderMenu(miAddClientOrder,PO_CLIENT,miPluginOrderClick);
  ShortCuts.SortOwners();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miPluginsReloadClick(TObject *Sender)
{
  Plugins.ClearPlugins();
  Plugins.InitPlugins();
  Plugins.OnGameLoaded();
  Plugins.UpdatePluginInfo();
  Plugins.CreateMenu(miPluginsRoot,miRunPluginClick);
  Plugins.CreateConfigMenu(miConfigure,miConfigurePluginClick);
  Plugins.CreateOrderMenu(miAddOrder,PO_NORMAL,miPluginOrderClick);
  Plugins.CreateOrderMenu(miAddOrderAuto,PO_AUTO,miPluginOrderClick);
  Plugins.CreateOrderMenu(miAddClientOrder,PO_CLIENT,miPluginOrderClick);
  ShortCuts.SortOwners();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::WMMove(TWMMove &mes)
{
  if(MapForm)
    MapForm->ApplyDockType();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAnalyzeBalanceClick(TObject *Sender)
{
  TBalanceForm *frm=new TBalanceForm(this);
  frm->ShowModal();
  delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAnalyzeProductsClick(TObject *Sender)
{
  TProductsForm *frm=new TProductsForm(this);
  frm->ShowModal();
  delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miAnalyzeCityRegionClick(TObject *Sender)
{
  TAnalyzeCityRegionFrm *frm=new TAnalyzeCityRegionFrm(this);
  frm->ShowModal();
  delete frm;
}
//---------------------------------------------------------------------------
void TAtlaForm::InitShortCuts()
{
  int eq;
  eq=ShortCuts.CreateNewEqualGroup();
  ShortCuts.AddToEqualGroup(eq,miCopyToClipboard3);
  ShortCuts.AddToEqualGroup(eq,miFacBattleToClipboard);

  int pgregion=ShortCuts.CreateNewPrefixGroup();
  ShortCuts.AddToPrefixGroup(pgregion,"PopupViewRegion");
  int pgobject=ShortCuts.CreateNewPrefixGroup();
  ShortCuts.AddToPrefixGroup(pgobject,"PopupViewObject");
  int pgunit=ShortCuts.CreateNewPrefixGroup();
  ShortCuts.AddToPrefixGroup(pgunit,"PopupViewUnit");
  ShortCuts.AddToPrefixGroup(pgunit,"Orders");
  ShortCuts.AddToPrefixGroup(pgunit,"AutoOrders");
  ShortCuts.AddToPrefixGroup(pgunit,"ClientOrders");
  ShortCuts.AddToPrefixGroup(pgunit,"PluginsOrders");
  ShortCuts.AddToPrefixGroup(pgunit,"PluginsAutoOrders");
  ShortCuts.AddToPrefixGroup(pgunit,"PluginsClientOrders");

  int pgfaction=ShortCuts.CreateNewPrefixGroup();
  ShortCuts.AddToPrefixGroup(pgfaction,"PopupViewFaction");
  ShortCuts.AddEqualPrefixGroups(pgregion,pgobject);
  ShortCuts.AddEqualPrefixGroups(pgregion,pgunit);
  ShortCuts.AddEqualPrefixGroups(pgregion,pgfaction);
  ShortCuts.AddEqualPrefixGroups(pgobject,pgunit);
  ShortCuts.AddEqualPrefixGroups(pgobject,pgfaction);
  ShortCuts.AddEqualPrefixGroups(pgunit,pgfaction);

  ShortCuts.AddBlockedOwner(miTemporary);
  ShortCuts.AddBlockedOwner(miPlugins);
  ShortCuts.AddOwnerMenu(MainMenu1->Items,"MainMenu","Main Menu",0);
  ShortCuts.AddOwnerMenu(pmToolBar->Items,"MainToolBar","Main ToolBar",0);
  ShortCuts.AddBlockedOwner(miWeather);
  ShortCuts.AddOwnerMenu(popupViewRegion->Items,"PopupViewRegion","Region Context Menu",1);
  ShortCuts.AddOwnerMenu(popupViewRegObject->Items,"PopupViewObject","Object Context Menu",2);
  ShortCuts.AddBlockedOwner(miAddOrder);
  ShortCuts.AddBlockedOwner(miAddOrderAuto);
  ShortCuts.AddBlockedOwner(miAddClientOrder);
  ShortCuts.AddOwnerMenu(popupViewUnit->Items,"PopupViewUnit","Unit Context Menu",3);
  ShortCuts.AddOwnerMenu(popupFactionMessages->Items,"PopupViewFaction","Faction Context Menu",4);
  ShortCuts.ClearBlockedOwners();

  ShortCuts.AddOwnerWithCodeRoot("Orders","Orders",10);
  ShortCuts.AddOwnerWithCodeRoot("AutoOrders","Auto Orders",20);
  ShortCuts.AddOwnerWithCodeRoot("ClientOrders","Client Orders",30);

  ShortCuts.ClearBlockedOwners();
  ShortCuts.SortOwners();
  ShortCuts.Load();

  for(int i=0;i<miAddOrder->Count;i++)
  {
    TMenuItem *mi=miAddOrder->Items[i];
    if(mi->IsLine()) continue;
    AnsiString code=mi->Tag;
    ShortCuts.AddOwnerWithCode(mi,code,"Orders",1,10);
  }
  for(int i=0;i<miAddOrderAuto->Count;i++)
  {
    TMenuItem *mi=miAddOrderAuto->Items[i];
    if(mi->IsLine()) continue;
    AnsiString code=mi->Tag;
    ShortCuts.AddOwnerWithCode(mi,code,"AutoOrders",1,20);
  }
  for(int i=0;i<miAddClientOrder->Count;i++)
  {
    TMenuItem *mi=miAddClientOrder->Items[i];
    if(mi->IsLine()) continue;
    AnsiString code=mi->Tag;
    ShortCuts.AddOwnerWithCode(mi,code,"ClientOrders",1,30);
  }
  ShortCuts.SortOwners();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::TreeViewDblClick(TObject *Sender)
{
  TPopupMenu *pm=TreeViewPopupMenu;
  if(!pm) return;
  for(int i=0;i<pm->Items->Count;i++)
  {
    TMenuItem *mi=pm->Items->Items[i];
    if(!mi->Visible) continue;
    if(!mi->Default) continue;
    mi->Click();
    return;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miSetUnitExprClick(TObject *Sender)
{
  if(!curUnit)
  {
    ShowMessage("No unit selected");
    return;
  }
  TExpressionEditorForm *frm=new TExpressionEditorForm(this);
  frm->mode=emUnit;
  frm->expr=optPrevNextUnitExpr;
  if(frm->ShowModal()==mrOk)
    optPrevNextUnitExpr=frm->expr;
  delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::Objecttypes1Click(TObject *Sender)
{
  if(!ShowObjTypesForm) ShowObjTypesForm = new TShowObjTypesForm(this);
  ShowObjTypesForm->Show();
}
//---------------------------------------------------------------------------
void TAtlaForm::setTekInfMode(int val)
{
  FTekInfMode = val;
  if(MapMode == 3)
  {
	ShowMapMode();
	DrawMap();
  }
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::btInfModeClick(TObject *Sender)
{
  MapMode = 3;
  ShowMapMode();
  DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::Entertaimens1Click(TObject *Sender)
{
  TMenuItem *mi = dynamic_cast<TMenuItem*>(Sender);
  if(!mi)return;
  TekInfMode = mi->Tag;
  mi->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::TownTrade1Click(TObject *Sender)
{
  if(!fTownTrade) fTownTrade = new TfTownTrade(this);
  fTownTrade->Show();
}
//---------------------------------------------------------------------------
TColor TAtlaForm::LookFactionColor(AnsiString str){
  TColor result = clWindowText ;

  if(!curFactions)return result;
  if(!curFaction) curFactions->SetCurFaction(miCF->Tag);
  if(!curFaction) return result;
  if(!str.Length()) return result;

  char *s = str.c_str();
  s += 2;
  GetNextFactionName(s);
  GetNextToken(s);
  if(*s != ')')return result;
  s++;
  if(!MyCompare(s,KeyPhrases->Get(keyUnitOnGuard))) s += KeyPhrases->Get(keyUnitOnGuard).Length();

  if(*s==','&&s[1]==' '){
    s += 2;
    GetNextFactionName(s);
    int facnum = atoi(GetNextToken(s).c_str());
    if(!facnum)return result;
    if(curFactions->GetFaction(facnum)->color != clWindowText) return curFactions->GetFaction(facnum)->color;
    int att = curFaction->GetAttitude(facnum);
    result = infocolors[att];
  } else result = infocolors[6];
  return result;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::ViewDrawItem(TWinControl *Control, int Index,
	  TRect &Rect, TOwnerDrawState State)
{
  TCanvas *pCanvas = ((TListBox *)Control)->Canvas;
  pCanvas->FillRect(Rect); // clear the rectangle
  AnsiString str = View->Items->Strings[Index];
  pCanvas->Font->Style = TFontStyles();
  if(optColorUnit && NowRegShow && str.Length()){
	if(str[1] == '*'){
	  pCanvas->Font->Color = infocolors[5];
	  pCanvas->Font->Style = pCanvas->Font->Style<<fsBold;
	}
	  else if(str[1] == '-' && str[2] == '-' && str[3] != '-' ) pCanvas->Font->Color = infocolors[6];
		else if(str[1] == '-' && str[2] == ' ' ) pCanvas->Font->Color = LookFactionColor(str);
  }
  pCanvas->TextOut(Rect.Left, Rect.Top, str);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::TreeViewCustomDrawItem(TCustomTreeView *Sender,
	  TTreeNode *Node, TCustomDrawState State, bool &DefaultDraw)
{
  DefaultDraw = true;
  if(!NeedDraw) return;
  TCanvas *pCanvas = TreeView->Canvas;
  pCanvas->Font->Style = TFontStyles();

  if(!Node)return;
  VTreeNode *vnode = (VTreeNode*)Node->Data;
  if(!vnode) return;
  if(vnode->type != tntUnit) return;
  AUnit *unit = (AUnit*) vnode->data;
  if(!unit) return;
  try {
	if(unit->IsLocal())
	{
	  pCanvas->Font->Style = pCanvas->Font->Style<<fsBold;
	  return;
	}
  } catch(...) { return; }
  if (State.Contains(cdsSelected)) return;
  if(!unit->faction) return;
  try { if(!unit->faction->number) return; } catch (...) { return; }

  if(!curFaction) curFactions->SetCurFaction(miCF->Tag);

  if(unit->faction->number == 0) pCanvas->Font->Color = infocolors[6];
	else {
	  AFaction *fac = curFactions->GetFaction(unit->faction->number);
	  if(!fac) return;
	  if(fac->color != clWindowText)
		pCanvas->Font->Color = fac->color;
		else pCanvas->Font->Color = infocolors[curFaction->GetAttitude(unit->faction->number)];
	}
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::TreeViewCustomDraw(TCustomTreeView *Sender,
      const TRect &ARect, bool &DefaultDraw)
{
  TreeView->Canvas->FillRect(ARect);
  DefaultDraw = true;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::LookLocalFactions()
{
  int num = 0;
  TMenuItem *mi;

  for (int i = miCF->Count-1; i >= 0; i--)
  {
    delete miCF->Items[i];
  }

  foreach(curFactions){
    AFaction *fac = *(AFaction**)iter;
    if(fac->local)
    {
      num++;
	  mi = new TMenuItem(this);
      mi->Caption = fac->name;
      mi->Tag = fac->number;
      mi->RadioItem = true;
	  mi->GroupIndex = 10;
      if(!curFaction) curFactions->SetCurFaction(fac->number);
      if(curFaction->number == fac->number)
      {
        mi->Checked = true;
        miCF->Tag = fac->number;
      }
      mi->OnClick = SelFacClick;
      miCF->Add(mi);
      //curFactions->SetCurFaction(fac->number);
    }
  }
  miCF->Visible = (num > 1);
  miCFr->Visible = miCF->Visible;

}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::SelFacClick(TObject *Sender)
{
  TMenuItem *mi = dynamic_cast<TMenuItem*>(Sender);
  if((mi->Tag == miCF->Tag) && curFaction && (curFaction->number ==mi->Tag)) return;
  mi->Checked = true;
  miCF->Tag = mi->Tag;
  curFactions->SetCurFaction(mi->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::TURNorder1Click(TObject *Sender)
{
  AOrderTurn *ordT = new AOrderTurn;
  AOrderComment *ord = new AOrderComment();
  ord->ParseOrder("Orders for next turn");
  ordT->torders->Add(ord);
  EditOrder(0, ordT);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::cbFacDiapChange(TObject *Sender)
{
  int ind = cbFacDiap->ItemIndex;
  if(ind < 0) return;
  ind = (int)cbFacDiap->Items->Objects[ind];
  if(facdiap == ind) return;
  facdiap = ind;
  MakeTreeView();
}
//---------------------------------------------------------------------------
bool TAtlaForm::NeedUnitInTree(AUnit *un)
{
  if(!inFacDiap(facdiap, un->faction)) return False;
  if(miFlead1->Checked && !un->IsLeader()) return False;
  if(miFlead2->Checked && un->IsLeader()) return False;
  if(miSetFF->Checked)
  {
	 // Фильтр по флагам
	if((arFiltrFlags.guard==1) && (un->endguard==GUARD_GUARD)) return False;
	if((arFiltrFlags.guard==2) && (un->endguard!=GUARD_GUARD)) return False;

	if((arFiltrFlags.avoid==1) && (un->endguard==GUARD_AVOID)) return False;
	if((arFiltrFlags.avoid==2) && (un->endguard!=GUARD_AVOID)) return False;

	if((arFiltrFlags.reveal==1) && (un->endreveal!=REVEAL_NONE)) return False;
	if((arFiltrFlags.reveal==2) && (un->endreveal!=REVEAL_UNIT)) return False;
	if((arFiltrFlags.reveal==3) && (un->endreveal!=REVEAL_FACTION)) return False;

	if((arFiltrFlags.behind==1) && un->GetEndFlag(FLAG_BEHIND)) return False;
	if((arFiltrFlags.behind==2) && !un->GetEndFlag(FLAG_BEHIND)) return False;

	if((arFiltrFlags.autotax==1) && un->GetEndFlag(FLAG_AUTOTAX)) return False;
	if((arFiltrFlags.autotax==2) && !un->GetEndFlag(FLAG_AUTOTAX)) return False;

	if((arFiltrFlags.hold==1) && un->GetEndFlag(FLAG_HOLDING)) return False;
	if((arFiltrFlags.hold==2) && !un->GetEndFlag(FLAG_HOLDING)) return False;

	if((arFiltrFlags.consume==1) && un->GetEndFlag(FLAG_CONSUMING)) return False;
	if((arFiltrFlags.consume==2) && !un->GetEndFlag(FLAG_CONSUMING_UNIT)) return False;
	if((arFiltrFlags.consume==3) && !un->GetEndFlag(FLAG_CONSUMING_FACTION)) return False;

	if((arFiltrFlags.noaid==1) && un->GetEndFlag(FLAG_NOAID)) return False;
	if((arFiltrFlags.noaid==2) && !un->GetEndFlag(FLAG_NOAID)) return False;

	if((arFiltrFlags.nocroswater==1) && un->GetEndFlag(FLAG_NOCROSS_WATER)) return False;
	if((arFiltrFlags.nocroswater==2) && !un->GetEndFlag(FLAG_NOCROSS_WATER)) return False;

	if((arFiltrFlags.nobattspoils==1) && un->GetEndFlag(FLAG_NOSPOILS)) return False;
	if((arFiltrFlags.nobattspoils==2) && !un->GetEndFlag(FLAG_NOSPOILS)) return False;

	if((arFiltrFlags.sharing==1) && un->GetEndFlag(FLAG_SHARING)) return False;
	if((arFiltrFlags.sharing==2) && !un->GetEndFlag(FLAG_SHARING)) return False;

	if((arFiltrFlags.spoils==1)  && !un->GetEndFlag(FLAG_NOSPOILS)) return False;
	if((arFiltrFlags.spoils==2)  && !un->GetEndFlag(FLAG_WALKSPOILS)) return False;
	if((arFiltrFlags.spoils==3)  && !un->GetEndFlag(FLAG_RIDESPOILS)) return False;
	if((arFiltrFlags.spoils==4)  && !un->GetEndFlag(FLAG_FLYSPOILS)) return False;
	if(arFiltrFlags.spoils==5)
	  if(un->GetEndFlag(FLAG_NOSPOILS) || un->GetEndFlag(FLAG_WALKSPOILS) ||
		 un->GetEndFlag(FLAG_RIDESPOILS) || un->GetEndFlag(FLAG_FLYSPOILS) ) return False;
  }
  if(FskilFiltr != "")
  {
	AnsiString abrsk = SkillTypes->NameToAbr(FskilFiltr);
	ASkill *skil1 = un->skills->GetSkill(abrsk);
	if(!skil1) return False;
  }
  if(FmoFiltr != 0)
  {
	AOrder *MOord = un->orders->GetMonthLong();
	if(!MOord && (FmoFiltr != -1)) return False;
	if(MOord && (FmoFiltr != MOord->type)) return False;
  }
  return True;
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::SpeedButton3Click(TObject *Sender)
{
  TPoint p;
  p.x = 0;
  p.y = btFiltLead->Height;
  p = btFiltLead->ClientToScreen(p);
  pmSelFlead->Popup(p.x, p.y);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miFlead2Click(TObject *Sender)
{
  TMenuItem *mi=dynamic_cast<TMenuItem*>(Sender);
  if(!mi) return;
  btFiltLead->Tag = mi->Tag;
  btFiltLead->Down = (mi->Tag > 0);
  MakeTreeView();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::SpeedButton5Click(TObject *Sender)
{
  TPoint p;
  p.x = 0;
  p.y = btFF->Height;
  p = btFF->ClientToScreen(p);
  pmSelFflag->Popup(p.x, p.y);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miNoFFClick(TObject *Sender)
{
  btFF->Down = False;
  MakeTreeView();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miSetFFClick(TObject *Sender)
{
  // Установка фильтра по флагам
  TfFlagFiltr *frm = new TfFlagFiltr(this);
  frm->SetFlags(arFiltrFlags);
  if(frm->ShowModal() == mrOk)
  {
	arFiltrFlags.guard = frm->cbGuard->ItemIndex;
	arFiltrFlags.avoid = frm->cbAvoid->ItemIndex;
	arFiltrFlags.reveal = frm->cbReveal->ItemIndex;
	arFiltrFlags.behind = frm->cbBehind->ItemIndex;
	arFiltrFlags.autotax = frm->cbAutoTax->ItemIndex;
	arFiltrFlags.hold = frm->cbHold->ItemIndex;
	arFiltrFlags.consume = frm->cbConsume->ItemIndex;
	arFiltrFlags.noaid = frm->cbNoaid->ItemIndex;
	arFiltrFlags.nocroswater = frm->cbNoCross->ItemIndex;
	arFiltrFlags.nobattspoils = frm->cbNoSpoils->ItemIndex;
	arFiltrFlags.spoils = frm->cbSpoils->ItemIndex;
	arFiltrFlags.sharing = frm->cbSharing->ItemIndex;
	miSetFF->Checked = True;
	btFF->Down = True;
	MakeTreeView();
  }
  frm->Free();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::btFFClick(TObject *Sender)
{
  if(!btFF->Down) miNoFF->Click();
  else {
	miSetFF->Checked = True;
	MakeTreeView();
  }
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::btFiltLeadClick(TObject *Sender)
{
  if(!btFiltLead->Down) miFlead0->Click(); else
  if(btFiltLead->Tag == 1) miFlead1->Click(); else miFlead2->Click();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::cbProfSelChange(TObject *Sender)
{
  if(cbProfSel->ItemIndex == 0) FskilFiltr = ""; else FskilFiltr = cbProfSel->Text;
  MakeTreeView();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::btAllFoffClick(TObject *Sender)
{
  FskilFiltr = "";
  facdiap = -2;
  FmoFiltr = 0;
  btFiltLead->Down = False;
  btFF->Down = False;
  cbFacDiap->ItemIndex = 0;
  cbProfSel->ItemIndex = 0;
  miFlead0->Checked = True;
  miNoFF->Checked = True;
  btAllFoff->Enabled = False;
  btFiltMO->Down = False;
  miNofilterMO->Checked = True;
  MakeTreeView();
}
//---------------------------------------------------------------------------
/*
 {O_ADVANCE,"advance "},
 {O_BUILD,"build"},
 {O_ENTERTAIN,"entertain"},
 {O_MOVE,"move "},
 {O_PRODUCE,"produce "},
 {O_SAIL,"sail"},
 {O_STUDY,"study "},
 {O_TEACH,"teach "},
 {O_WORK,"work"},

advance
build
entertain
move
produce
sail
study
teach
work
*/
void __fastcall TAtlaForm::SpeedButton2Click(TObject *Sender)
{
  TPoint p;
  p.x = 0;
  p.y = btFiltMO->Height;
  p = btFiltMO->ClientToScreen(p);
  pmSelMO->Popup(p.x, p.y);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miFiltMO(TObject *Sender)
{
  TMenuItem *mi=dynamic_cast<TMenuItem*>(Sender);
  if(!mi) return;
  FmoFiltr = mi->Tag;
  if(FmoFiltr != 0) btFiltMO->Tag = FmoFiltr;
  btFiltMO->Down = (FmoFiltr != 0);
  mi->Checked = True;
  MakeTreeView();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::btFiltMOClick(TObject *Sender)
{
  if(btFiltMO->Down)
  {
	if(btFiltMO->Tag == 0) btFiltMO->Down = False;
	else
	{
	  for(int i=0; i<pmSelMO->Items->Count; i++)
	  {
		TMenuItem *mi = pmSelMO->Items->Items[i];
		if(mi->Tag == btFiltMO->Tag)
		{
		  mi->Click();
		  return;
		}
	  }
	}
  } else miFiltMO(miNofilterMO);
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::miSkillsTreeClick(TObject *Sender)
{
  if(!fSkillsTree) fSkillsTree = new TfSkillsTree(this);
  fSkillsTree->Show();
}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::Editor1Click(TObject *Sender)
{
  fSelEditScr = new TfSelEditScr(this);
  fSelEditScr->ShowModal();
  if(fSelEditScr->Changed)
  {
	 TIniFile *iniS = new TIniFile(ScriptsName);
	 int i = 0;
	 for(; i < AllScripts->count; i++)
	 {
	   AnsiString Sec = "Script";
	   Sec += i;
	   AScript *scr = AllScripts->Get(i);
	   iniS->WriteString(Sec, "caption", scr->caption);
	   iniS->WriteBool(Sec, "needclear",  scr->NeedClear);
	   int cc = 0;
	   for(int j=0; j<scr->Strings->Count; j++)
	   {
		  AnsiString s = scr->Strings->Strings[j];
		  s = s.Trim();
		  if(s == "") continue;
		  AnsiString str = "str";
		  str += cc;
		  iniS->WriteString(Sec, str, s);
		  cc++;
	   }
	   iniS->WriteInteger(Sec, "countstr", cc);
	 }
	 iniS->WriteInteger("Scripts", "count", i);
	 iniS->UpdateFile();
	 delete iniS;
  }
  delete fSelEditScr;
  MakeScrMenu();
}
//---------------------------------------------------------------------------
void __fastcall TAtlaForm::ExecScriptClick(TObject *Sender)
{
  if(!curUnit) return;
  TMenuItem *mi=dynamic_cast<TMenuItem*>(Sender);
  if(!mi)return;
  AScript *scr = AllScripts->Get(mi->Tag);
  if(!scr) return;

  // Execute Script
  TEditOrdersForm *frm=new TEditOrdersForm(this);
  TStrings *list = frm->Editor->Lines;
  AnsiString s;
  if(!scr->NeedClear)
  {
	  foreach(curUnit->orders)
	  {
		AOrder *ord=*(AOrder**) iter;
		s=ord->WriteTemplate();
		int pos,oldpos=0;
		while((pos=s.SubString(oldpos+1,s.Length()).Pos("\n"))!=0)
		{
		  pos+=oldpos;
		  s.Insert("\r",pos);
		  oldpos=pos+1;
		}
		if(!s.Length())continue;
		list->Add(s);
	  }
	list->Add("");
  }
  s = list->Text;
  s += scr->Strings->Text;
  list->Text = s;
  
  if(frm->ShowModal()==mrOk)
  {
	AOrder *ord;
	AOrderTurn *turnord = 0;
	curUnit->orders->Clear();
	for(int i=0,endi=list->Count;i<endi;i++)
	{
	  s = list->Strings[i];
	  if(!s.Length()) continue;
	  ord = ParseOrder(s);
	  if(!ord)
	  {
		AddEvent("Error parsing order: " + s);
		continue;
	  }

	  if(ord->type == O_ENDTURN)
	  {
		if(!turnord) AddEvent("ENDTURN without TURN");
		turnord = 0;
		continue;
	  }

	  if(turnord){
		if(ord->type == O_TURN){
		  AddEvent("TURN order cannot be used in TURN");
		  continue;
		}
		if(ord->type==O_END||ord->type==O_UNIT)
		{
		  AddEvent("This order cannot be used here: "+s);
		  continue;
		}
		ord->turnorder = true;
		turnord->torders->Add(ord);
		continue;
	  }
	  if(ord->type==O_END||ord->type==O_UNIT
		 ||ord->type==O_FORM||ord->type==O_FORMEND)
	  {
		AddEvent("This order cannot be used here: "+s);
		continue;
	  }
	  if(ord->type == O_TURN)
	  {
		curUnit->orders->Add(ord);
		turnord = ((AOrderTurn*)ord);
        turnord->torders->Clear();
		continue;
	  }

	  curUnit->orders->Add(ord);
	} // for
   //if(turnord)
   //{
   //	  AddEvent("TURN order without ENDTURN. Added");
   //	  ord = ParseOrder("endturn");
   //	  curUnit->orders->Add(ord);
   //}
   curRegion->runned = false;
   curTurn->Modified=true;
  }

 delete frm;
 ViewShow();
 if(MapMode==0||optAlwaysShowMovement)
   DrawMap();
 if(ViewUnitsForm)
   ViewUnitsForm->UpdateUnit(curUnit);

}
//---------------------------------------------------------------------------

void __fastcall TAtlaForm::miAddOrderAutoFollowClick(TObject *Sender)
{
  EditOrder(0,new AOrderAutoFollow);

}
//---------------------------------------------------------------------------

