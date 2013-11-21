//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <inifiles.hpp>
#include <algorithm>

#include "turn.h"
#include "util.h"
#include "eapfile.h"
#include "region.h"
#include "faction.h"
#include "unit.h"
#include "ViewUnitsFrm.h"
#include "gameparams.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
int BaseVer,LastBaseVer=33;
int curTurnNumber=-1;
extern TColor curregColor,wallColor;
extern TColor gridColor,textColor,roadColor;
extern unsigned char ProdModeAttr;
extern TColor wedgecolors[6];
extern TColor infocolors[7];
extern AnsiString wedgeexpr[6];
extern char* SettingsSect,*ExprCommsSect,*WarningExprsSect,*EventsWarningsSect
  ,*ViewUnitsSect,*PluginsStrSect;
extern bool RemoveLastTurn;
bool LoadIniColor(TColor &color,AnsiString paramname,TIniFile *ini); //in MainFrm
void SaveIniColor(TColor color,AnsiString paramname,TIniFile *ini); //in MainFrm

ATurn*curTurn;
ATurn::ATurn()
 :RegionList(new ARegionList(this)),factions(new AFactions(this)),
  num(-1),Modified(false),turns(0),FRegUserInfos(new TStringList)
  ,AllOrdersRunned(false)
{
}
ATurn::~ATurn()
{
 delete RegionList;
 delete factions;
 delete FRegUserInfos;
 if(curTurn==this){curTurn=0;curTurnNumber=-1;}
}
AnsiString ATurn::GetFileName(){
 AnsiString fName="turn";
 fName+=num;
 fName+=".agb";
 return fName;
}
void ATurn::Read()
{
 TFile in;
 AnsiString fName=GetFileName();
 in.Open(GetGameFileName(fName));
 if(!in.Opened()){num=-1;return;}
 in>>BaseVer;
 if(BaseVer<8){
  AnsiString s="Error loading turn ";
  s+=num;
  s+=". Old format. Need ver 0.9.1.11";
  throw Exception(s);
 }
 if(BaseVer<23){
  AnsiString s="Error loading turn ";
  s+=num;
  s+=". Old format. Need ver 0.9.1.24";
  throw Exception(s);
 }
 if(BaseVer>LastBaseVer){
  AnsiString s="Error loading turn";
  s+=num;
  s+=". Unknown format";
  throw Exception(s);
 }
 int newnum=-1;
 in>>newnum;
 if(newnum!=num)
  throw Exception(AnsiString("Error loading turn ")+num);
 num=newnum;
 if(BaseVer<26)
 {
   unsigned char attr;
   in>>attr;
 }
 factions->Read(in);
 RegionList->Read(in);
 {
  AnsiString str;
  in>>str;
  FRegUserInfos->Text=str;
 }
 Modified=false;
 AddEvent(AnsiString("Turn ")+num+" loaded");
}
void ATurn::Write()
{
 if(!Modified) return;
 TFile out;
 AnsiString fName=GetFileName();
 out.Open(GetGameFileName(fName),true);
 BaseVer=LastBaseVer;
 out<<BaseVer;
 out<<num;
 factions->Write(out);
 RegionList->Write(out);
 {
  AnsiString str=FRegUserInfos->Text;
  out<<str;
 }
 Modified=false;
}
void ATurn::CreateNew(ATurn *prevturn)
{
 if(prevturn){
  num=prevturn->num+1;
//  RegionList->turn=num;
  factions->CreateNew(prevturn->factions);
  RegionList->CreateNew(prevturn->RegionList);
  for(int i=prevturn->FRegUserInfos->Count-1;i>=0;i--){
   AnsiString key=prevturn->FRegUserInfos->Names[i];
   AnsiString str=prevturn->FRegUserInfos->Values[key];
   int pos=str.Pos("-");
   if(pos)
    str.SetLength(pos-1);
   if(str.Length())
    FRegUserInfos->Values[key]=str;
  }
 }else{
  num=0;
//  RegionList->turn=num;
  factions->CreateNew(0);
  RegionList->CreateNew(0);
  FRegUserInfos->Clear();
 }
 Modified=true;
 AddEvent(AnsiString("Turn ")+num+" created");
}
void ATurn::VerifyNextTurn(bool withadvprods)
{
 if(turns->count<=num+1)return; //curturns
 int nxtturnnum=turns->GetNextTurnNum(num);
 if(nxtturnnum<0) return;
 ATurn *nxtturn=turns->Get(nxtturnnum);
/* if(num==0){
	nxtturn=curTurns->Get(curTurns->curbeginturn);
 }else{
	nxtturn=curTurns->Get(num+1);
 }*/
 nxtturn->RegionList->UpdateFromPrev(RegionList);
 if(withadvprods)
	 nxtturn->RegionList->UpdateAdvProdsFromPrev(RegionList);
 if(Modified) nxtturn->Modified=true;
 nxtturn->VerifyNextTurn(withadvprods);
}
ATurns*curTurns;
ATurns::ATurns()
 :TList()/*,lastturn(0),lastx(0),lasty(0)*/,lastz(0),lastfac(0),
  tertypes(new TStringList),provincies(new TStringList),
  towns(new TStringList),towntypes(new TStringList),
  curbeginturn(1),newbeginturn(1),autobeginturn(2),
  keyturn(4),expr_comms(new TStringList),faccomments(new TStringList)
  ,warning_exprs(new TStringList)
  ,ignore_params(new TStringList)
  ,pluginsstr(new TStringList)
  ,extra_magic_skills(new TStringList)
  ,first_magic_skills(new TStringList)
  ,last_magic_skills(new TStringList)
{
 for(int i=0;i<MaxLastLevels;i++){
  lastx[i]=0;lasty[i]=0;
 }
}
__fastcall ATurns::~ATurns()
{
 Clear();
 if(curTurns==this)curTurns=0;
 delete tertypes;
 delete provincies;
 delete towns;
 delete towntypes;
 delete expr_comms;
 delete faccomments;
 delete warning_exprs;
 delete ignore_params;
 delete pluginsstr;
 delete extra_magic_skills;
 delete first_magic_skills;
 delete last_magic_skills;
}
void __fastcall ATurns::Clear()
{
 for(int i=Count-1;i>=0;i--){
  ATurn*it=Get(i);
//  Delete(i);
  delete it;
 }
 TList::Clear();
 locks.clear();
}
void ATurns::DeleteLast()
{
 int i=Count-1;
 UnlockTurn(i);
 ATurn*it=Get(i);
 TList::Delete(i);
 delete it;
 SetCurTurn(Count-1);
}
void ATurns::Read()
{
 Clear();
 TFile in;
 in.Open(GetGameFileName("turns.agb"));
 in>>BaseVer;
 int oldBaseVer=BaseVer;
 if(BaseVer<8){
  throw Exception("Error loading turns. Too old format. Need ver 0.9.1.11");
 }
 if(BaseVer<23){
  throw Exception("Error loading turns. Too old format. Need ver 0.9.1.24");
 }
 if(BaseVer>LastBaseVer){
  throw Exception("Error loading turns. Unknown format");
 }
 int size;
 if(BaseVer>=25)
   in>>size;
 in>>lastfac;
 in>>ver1>>ver2;
 for(int i=0;i<6;i++){
  in>>wedgeexpr[i];
 }
 if(BaseVer<25)
   in>>size;
 in>>curbeginturn;
 newbeginturn=curbeginturn;
 in>>autobeginturn;
 in>>keyturn;

 ReadStringList(in,tertypes);
 ReadStringList(in,provincies);
 ReadStringList(in,towns);
 ReadStringList(in,towntypes);

 if(RemoveLastTurn)
 {
   AddEvent("Removing last turn");
   RemoveLastTurn=false;
   size--;
 }
 Capacity=size;
 ATurn *prev=0;
 int keybeginturn=curbeginturn-curbeginturn%keyturn;
 for(int i=0;i<size;i++){
  if(i!=0&&i<keybeginturn){
   TList::Add(0);
   continue;
  }
  ATurn *turn=new ATurn;
  turn->num=i;
  turn->Read();
  if(turn->num==-1){
    TList::Add(0);
    delete turn;
    continue;
  }
  Add(turn);
  if(prev){
   turn->RegionList->UpdateFromPrev(prev->RegionList);
   int prvnum=prev->num;
   if(prvnum&&prvnum<curbeginturn&&prvnum!=keybeginturn){
    prev->Write();
    Items[prvnum]=0;
    delete prev;
   }else
     LockTurn(prvnum);
  }
  prev=turn;
 }
 LockTurn(size-1);
 BaseVer=oldBaseVer;
 in>>lastz;
 ATurn *turn=Get(count-1);
 int levels=turn->RegionList->RegionArrays->count;
 if(levels>MaxLastLevels) levels=MaxLastLevels;
 int _lastx,_lasty;
 for(int i=0;i<levels;i++){
  in>>_lastx>>_lasty;
  lastx[i]=_lastx;
  lasty[i]=_lasty;
 }
  try{
   TIniFile *ini=new TIniFile(GetGameFileName("options.txt"));
   LoadIniColor(curregColor,"curregColor",ini);
   LoadIniColor(wallColor,"wallColor",ini);
   LoadIniColor(gridColor,"gridColor",ini);
   LoadIniColor(textColor,"textColor",ini);
   LoadIniColor(roadColor,"roadColor",ini);
   for(int i=0;i<6;i++){
    LoadIniColor(wedgecolors[i],AnsiString("wedgeColor")+i,ini);
   }
   
   for(int i=0;i<7;i++){
     LoadIniColor(infocolors[i],AnsiString("infoColors")+i,ini);
   }

   ProdModeAttr=atoi(ini->ReadString(SettingsSect,"ProdModeAttr",ProdModeAttr).c_str());
   LoadReportsDir=ini->ReadString(SettingsSect,"LoadReportsDir","");
   optServEMail=ini->ReadString(SettingsSect,"ServEMail","");
   optNoServerMail=atoi(ini->ReadString(SettingsSect,"NoServerMail","0").c_str());

   optServOrdersSubj=ini->ReadString(SettingsSect,"ServOrdersSubj","");
   optServTimesSubj=ini->ReadString(SettingsSect,"ServTimesSubj","");
   optServTimesBody=ini->ReadString(SettingsSect,"ServTimesBody","");
   optServMailSubj=ini->ReadString(SettingsSect,"ServMailSubj","");

   optServNoCross=atoi(ini->ReadString(SettingsSect,"ServNoCross","0").c_str());
   optServNoSpoils=atoi(ini->ReadString(SettingsSect,"ServNoSpoils","0").c_str());
   optServerSharing=atoi(ini->ReadString(SettingsSect,"ServSharing","0").c_str());
   optServGiveAll=atoi(ini->ReadString(SettingsSect,"ServGiveAll","0").c_str());
   optServSpoils=atoi(ini->ReadString(SettingsSect,"ServSpoils","0").c_str());
   optServFlightOverWater=atoi(ini->ReadString(SettingsSect,"ServFlightOverWater","0").c_str());
   if(optServFlightOverWater>WFLIGHT_UNLIMITED)
    optServFlightOverWater=WFLIGHT_NONE;
   optConquest=atoi(ini->ReadString(SettingsSect,"Conquest","0").c_str());
   optBoldUndergroundGrid=atoi(ini->ReadString(SettingsSect,"BoldUndergroundGrid","0").c_str());
   optTaxIsMonthlong=atoi(ini->ReadString(SettingsSect,"TaxIsMonthlong","0").c_str());
   optMagicDeep=atoi(ini->ReadString(SettingsSect,"MagicDeep","0").c_str());
   optHideGrid=atoi(ini->ReadString(SettingsSect,"HideGrid","0").c_str());
   optMaxMemoryTurns=atoi(ini->ReadString(SettingsSect,"MaxMemoryTurns","0").c_str());
   optCombinedTaxTrade=atoi(ini->ReadString(SettingsSect,"CombinedTaxTrade","0").c_str());
   optRunRegionOrdersEventPrefix=ini->ReadString(SettingsSect,"RunRegionOrdersEventPrefix",EmptyStr);
   optClearEventsOnRunRegionOrders=atoi(ini->ReadString(SettingsSect,"ClearEventsOnRunRegionOrders","0").c_str());
   optPrevNextUnitExpr=ini->ReadString(SettingsSect,"PrevNextUnitExpr",EmptyStr);


   ignore_params->Clear();
   if(BaseVer<30)
   {
     ignore_params->Add("Weight:");
     ignore_params->Add("Capacity:");
   }else
   {
     AnsiString str=ini->ReadString(SettingsSect,"IgnoreUnitParams","");
     try{
       ignore_params->CommaText=str;
     }catch(...){}
   }
   {
     extra_magic_skills->Clear();
     AnsiString str=ini->ReadString(SettingsSect,"ExtraMagicSkills","");
     try{
       extra_magic_skills->CommaText=str;
     }catch(...){}
   }
   {
     first_magic_skills->Clear();
     AnsiString str=ini->ReadString(SettingsSect,"FirstMagicSkills","");
     try{
       first_magic_skills->CommaText=str;
     }catch(...){}
   }
   {
     last_magic_skills->Clear();
     AnsiString str=ini->ReadString(SettingsSect,"LastMagicSkills","");
     try{
       last_magic_skills->CommaText=str;
     }catch(...){}
   }
   expr_comms->Clear();
   int count=atoi(ini->ReadString(ExprCommsSect,"Count","").c_str());
   for(int i=0;i<count;i++){
    AnsiString key=i+1;
    AnsiString expr=ini->ReadString(ExprCommsSect,"Expr"+key,"");
    AnsiString comm=ini->ReadString(ExprCommsSect,"Comm"+key,"");
    if(expr.Length()||comm.Length()){
     expr_comms->Add(expr);
     expr_comms->Add(comm);
    }
   }
   warning_exprs->Clear();
   count=atoi(ini->ReadString(WarningExprsSect,"Count","").c_str());
   for(int i=0;i<count;i++){
    AnsiString key=i+1;
    AnsiString expr=ini->ReadString(WarningExprsSect,"Expr"+key,"");
    AnsiString comm=ini->ReadString(WarningExprsSect,"Mess"+key,"");
    if(expr.Length()||comm.Length()){
     warning_exprs->Add(expr);
     warning_exprs->Add(comm);
    }
   }
   {
     AnsiString str=ini->ReadString(EventsWarningsSect,"Count","");
     if(!str.Length())
       FillEventsWarning();
     else
     {
       int count=atoi(str.c_str());
       EventsWarnings->Clear();
       for(int i=0;i<count;i++){
         AnsiString key=i+1;
         AnsiString str=ini->ReadString(EventsWarningsSect,"Str"+key,"");
         if(str.Length())
           EventsWarnings->Add(str);
       }
     }
   }
   {
     AnsiString str=ini->ReadString(ViewUnitsSect,"Default","");
     TViewUnitsForm::Headers=str;

     str=ini->ReadString(ViewUnitsSect,"Count","");
     ViewUnitsCols->Clear();
     if(str.Length())
     {
       int count=atoi(str.c_str());
       ViewUnitsCols->Clear();
       for(int i=0;i<count;i++){
         AnsiString key=i+1;
         AnsiString str=ini->ReadString(ViewUnitsSect,"Str"+key,"");
         if(str.Length())
           ViewUnitsCols->Add(str);
       }
     }
     str=ini->ReadString(ViewUnitsSect,"SetCount","");
     ViewUnitsSets->Clear();
     if(str.Length())
     {
       int count=atoi(str.c_str());
       for(int i=0;i<count;i++){
         AnsiString key=i+1;
         AnsiString str=ini->ReadString(ViewUnitsSect,"SetStr"+key,"");
         if(str.Length())
           ViewUnitsSets->Add(str);
       }
     }
   }
   ini->ReadSectionValues(PluginsStrSect,pluginsstr);
   GameParams.Load(ini);
   delete ini;
  }catch(...){
  }
 {
  int kolvo;
  in>>kolvo;
  for(int i=0;i<kolvo;i++){
   int j;
   AnsiString str;
   in>>j>>str;
   faccomments->AddObject(str,(TObject*)j);
  }
 }
}
void ATurns::Write()
{
 int size=count;
 if(!size)
 {
   AnsiString mes="Turns count=0. Save aborted.\nPlease contact to author.";
   ShowMessage(mes);
   return;
 }
 AnsiString fname=GetGameFileName("turns.agb");
 if(GetFileSize(fname)>20)
 {
   AnsiString bakfname=ChangeFileExt(fname,".bak");
   CopyFile(fname.c_str(),bakfname.c_str(),FALSE);
 }
 TFile out;
 out.Open(fname,true);
 BaseVer=LastBaseVer;
 out<<BaseVer;
 out<<size;
 out<<lastfac;
 out<<ver1<<ver2;
 for(int i=0;i<6;i++){
  out<<wedgeexpr[i];
 }

 if(autobeginturn>0){
  newbeginturn=count-autobeginturn;
 }
 if(newbeginturn>curbeginturn){
  int i;
  for(i=curbeginturn;i<newbeginturn;i++){
   ATurn *turn=Get(i);
   if(!turn) break;
   turn->Write();
   delete turn;
   Items[i]=0;
  }
  curbeginturn=i;
  newbeginturn=i;
 }
 if(newbeginturn<1) newbeginturn=1;
 if(newbeginturn<curbeginturn){
  int i;
  for(i=newbeginturn;i<curbeginturn;i++){
   PreloadTurn(i);
  }
  for(i=curbeginturn-1;i>=newbeginturn;i--){
   if(!Get(i)) break;
  }
  i++;
  curbeginturn=i;
  newbeginturn=i;
 }
 foreach(this){
  ATurn*turn=*(ATurn**)iter;
  if(!turn) continue;
  turn->Write();
 }
 out<<curbeginturn;
 out<<autobeginturn;
 out<<keyturn;
 WriteStringList(out,tertypes);
 WriteStringList(out,provincies);
 WriteStringList(out,towns);
 WriteStringList(out,towntypes);

 {
  out<<lastz;
  ATurn *turn=Get(0);
  int levels=turn->RegionList->RegionArrays->count;
  if(levels>MaxLastLevels) levels=MaxLastLevels;
  int _lastx,_lasty;
  for(int i=0;i<levels;i++){
   _lastx=lastx[i];
   _lasty=lasty[i];
   out<<_lastx<<_lasty;
  }
 }
 try{
  TIniFile *ini=new TIniFile(GetGameFileName("options.txt"));
  ini->WriteString(SettingsSect,"LoadReportsDir",LoadReportsDir);
  ini->WriteString(SettingsSect,"ServEMail",optServEMail);
  ini->WriteString(SettingsSect,"NoServerMail",int(optNoServerMail));

  ini->WriteString(SettingsSect,"ServOrdersSubj",optServOrdersSubj);
  ini->WriteString(SettingsSect,"ServTimesSubj",optServTimesSubj);
  ini->WriteString(SettingsSect,"ServTimesBody",optServTimesBody);
  ini->WriteString(SettingsSect,"ServMailSubj",optServMailSubj);

  ini->WriteString(SettingsSect,"ServNoCross",int(optServNoCross));
  ini->WriteString(SettingsSect,"ServNoSpoils",int(optServNoSpoils));
  ini->WriteString(SettingsSect,"ServSharing",int(optServerSharing));
  ini->WriteString(SettingsSect,"ServGiveAll",int(optServGiveAll));
  ini->WriteString(SettingsSect,"ServSpoils",int(optServSpoils));
  ini->WriteString(SettingsSect,"ServFlightOverWater",optServFlightOverWater);
  ini->WriteString(SettingsSect,"Conquest",int(optConquest));
  ini->WriteString(SettingsSect,"BoldUndergroundGrid",int(optBoldUndergroundGrid));
  ini->WriteString(SettingsSect,"TaxIsMonthlong",int(optTaxIsMonthlong));
  ini->WriteString(SettingsSect,"MagicDeep",int(optMagicDeep));
  ini->WriteString(SettingsSect,"HideGrid",int(optHideGrid));
  ini->WriteString(SettingsSect,"MaxMemoryTurns",optMaxMemoryTurns);
  ini->WriteString(SettingsSect,"CombinedTaxTrade",optCombinedTaxTrade);
  ini->WriteString(SettingsSect,"RunRegionOrdersEventPrefix","\""+optRunRegionOrdersEventPrefix+"\"");
  ini->WriteString(SettingsSect,"ClearEventsOnRunRegionOrders",(int)optClearEventsOnRunRegionOrders);
  ini->WriteString(SettingsSect,"PrevNextUnitExpr","\""+optPrevNextUnitExpr+"\"");

  SaveIniColor(curregColor,"curregColor",ini);
  SaveIniColor(wallColor,"wallColor",ini);
  SaveIniColor(gridColor,"gridColor",ini);
  SaveIniColor(textColor,"textColor",ini);
  SaveIniColor(roadColor,"roadColor",ini);
  for(int i=0;i<6;i++){
   SaveIniColor(wedgecolors[i],AnsiString("wedgeColor")+i,ini);
  }

  for(int i=0;i<7;i++){
   SaveIniColor(infocolors[i],AnsiString("infoColors")+i,ini);
  }

  ini->WriteString(SettingsSect,"ProdModeAttr",ProdModeAttr);
  {
    AnsiString str="\""+ignore_params->CommaText+"\"";
    ini->WriteString(SettingsSect,"IgnoreUnitParams",str);
  }
  ini->WriteString(SettingsSect,"ExtraMagicSkills",extra_magic_skills->CommaText);
  ini->WriteString(SettingsSect,"FirstMagicSkills",first_magic_skills->CommaText);
  ini->WriteString(SettingsSect,"LastMagicSkills",last_magic_skills->CommaText);

  int exprcount=expr_comms->Count/2;
  ini->EraseSection(ExprCommsSect);
  ini->WriteString(ExprCommsSect,"Count",exprcount);
  for(int i=0;i<exprcount;i++){
   AnsiString expr=expr_comms->Strings[i*2];
   AnsiString comm=expr_comms->Strings[i*2+1];
   AnsiString key=i+1;
   if(expr.Length())
    ini->WriteString(ExprCommsSect,"Expr"+key,expr);
   if(comm.Length()){
    comm="\""+comm+"\"";
    ini->WriteString(ExprCommsSect,"Comm"+key,comm);
   }
  }

  exprcount=warning_exprs->Count/2;
  ini->EraseSection(WarningExprsSect);
  ini->WriteString(WarningExprsSect,"Count",exprcount);
  for(int i=0;i<exprcount;i++){
   AnsiString expr=warning_exprs->Strings[i*2];
   AnsiString comm=warning_exprs->Strings[i*2+1];
   AnsiString key=i+1;
   if(expr.Length())
    ini->WriteString(WarningExprsSect,"Expr"+key,expr);
   if(comm.Length()){
    ini->WriteString(WarningExprsSect,"Mess"+key,comm);
   }
  }

  exprcount=EventsWarnings->Count;
  ini->EraseSection(EventsWarningsSect);
  ini->WriteString(EventsWarningsSect,"Count",exprcount);
  for(int i=0;i<exprcount;i++){
   AnsiString str=EventsWarnings->Strings[i];
   AnsiString key=i+1;
   if(str.Length())
    ini->WriteString(EventsWarningsSect,"Str"+key,"\""+str+"\"");
  }

  ini->EraseSection(ViewUnitsSect);
  TViewUnitsForm::Headers.name.SetLength(0);
  ini->WriteString(ViewUnitsSect,"Default","\""+AnsiString(TViewUnitsForm::Headers)+"\"");
  exprcount=ViewUnitsCols->Count;
  ini->WriteString(ViewUnitsSect,"Count",exprcount);
  for(int i=0;i<exprcount;i++){
   AnsiString str=ViewUnitsCols->Strings[i];
   AnsiString key=i+1;
   if(str.Length())
    ini->WriteString(ViewUnitsSect,"Str"+key,"\""+str+"\"");
  }
  exprcount=ViewUnitsSets->Count;
  ini->WriteString(ViewUnitsSect,"SetCount",exprcount);
  for(int i=0;i<exprcount;i++){
   AnsiString str=ViewUnitsSets->Strings[i];
   AnsiString key=i+1;
   if(str.Length())
    ini->WriteString(ViewUnitsSect,"SetStr"+key,"\""+str+"\"");
  }
  for(int i=0;i<pluginsstr->Count;i++)
  {
    AnsiString name=pluginsstr->Names[i];
    ini->WriteString(PluginsStrSect,name,pluginsstr->Values[name]);
  }
  GameParams.Save(ini);
  delete ini;
 }catch(...){
 }
 int kolvo=faccomments->Count;
 out<<kolvo;
 for(int i=0;i<kolvo;i++){
  int j=(int)faccomments->Objects[i];
  AnsiString str=faccomments->Strings[i];
  out<<j<<str;
 }
}
void ATurns::Add(ATurn * turn)
{
 if(turn->num==-1) turn->num=count;
 if(turn->num!=count)
  throw Exception("Error ATurns::Add(): number not match");
 turn->turns=this;
 TList::Add(turn);
}
ATurn* ATurns::Get(int turnnum)
{
 return (ATurn*)Items[turnnum];
}
bool ATurns::SetCurTurn(int turnnum)
{
 if(curTurns!=this)return false;
 ATurn *t=Get(turnnum);
 if(t&&t==curTurn)return true;
 curTurn=0;
 curFactions=0;
 curRegionList=0;
 curFaction=0;
 curRegion=0;
 curRegionArray=0;
 if(!t)return false;
 AddEvent(AnsiString("Turn ")+turnnum+" activated");
 curTurn=t;
 curTurnNumber=turnnum;
 curFactions=t->factions;
 curRegionList=t->RegionList;
 if(!curFactions->SetCurFaction(lastfac)){
  if(!curFactions->SetCurFaction(0))return false;
 }
 int _lastx=0,_lasty=0;
 if(lastz<MaxLastLevels){
  _lastx=lastx[lastz];
  _lasty=lasty[lastz];
 }
 if(!curRegionList->SetCurRegion(_lastx,_lasty,lastz)){
  ARegion *reg=curRegionList->GetNearestRegion(_lastx,_lasty,lastz);
  if(!reg){
   if(!curRegionList->SetCurRegion(0,0,0))return false;
  }else{
   if(!curRegionList->SetCurRegion(reg->xloc,reg->yloc,reg->zloc))return false;
  }
 }
 curTurn->PrepareOrders();
 return true;
}
bool ATurns::RestoreLastParam()
{
 return SetCurTurn(count-1);
}
void ATurns::StoreLastParam()
{
 if(curFaction){
  lastfac=curFaction->number;
 }else lastfac=0;
 if(curRegion){

//  lastx=curRegion->xloc;
//  lasty=curRegion->yloc;
  lastz=curRegion->zloc;
  StoreLastXY();
 }else{
  /*lastx=lasty=*/lastz=0;
 }
}
void ATurns::RestoreLastXY(int &xloc,int &yloc,int zloc){
 if(zloc<MaxLastLevels){
  xloc=lastx[zloc];
  yloc=lasty[zloc];
 }else{
  xloc=0;
  yloc=0;
 }
}
void ATurns::StoreLastXY(){
 if(!curRegion) return;
 int level=curRegion->zloc;
 if(level>=MaxLastLevels) return;
 lastx[level]=curRegion->xloc;
 lasty[level]=curRegion->yloc;
}
void ATurns::CreateNew()
{
 /*lastturn=lastx=lasty=*/lastz=lastfac=0;
 ver1="";ver2="";
 Clear();
 ATurn*turn=new ATurn;
 turn->CreateNew(0);
 Add(turn);
 FillEventsWarning();
 optServNoCross = false;
 optServNoSpoils = false;
 optServerSharing = false;
}
void ATurns::PrepareTurn(int num)
{
 if(num<=0){
  SetCurTurn(0);
  return;
 }
//- prepare turn
 if(count==1){
  curbeginturn=num;
  newbeginturn=num;
  for(int t=1;t<num;t++){
   TList::Add(0);
  }
  ATurn *prevturn=Get(0);
  ATurn *newturn=new ATurn;
  newturn->CreateNew(prevturn);
  newturn->num=num;
  Add(newturn);
 }else/* if(num<curbeginturn){
/  newbeginturn=num;
  SaveGameBase();
  ATurn *prevturn=Get(0);
  prevturn->VerifyNextTurn();
  for(int t=num;t<curbeginturn;t++){
   ATurn *newturn=new ATurn;
   newturn->CreateNew(prevturn);
   newturn->num=t;
   Items[t]=newturn;
   prevturn=newturn;
  }
  curbeginturn=num;
  newbeginturn=num;*/
 if(num<count){ 
  if(!PreloadTurn(num)){

   int prvnum=GetPrevTurnNum(num);
   ATurn *prvturn=Get(prvnum);
   ATurn *newturn=new ATurn;
   newturn->CreateNew(prvturn);
   newturn->num=num;
   newturn->turns=this;
   Items[num]=newturn;
  }
 }else{
  ATurn *prevturn=Get(count-1);
  for(int t=count-1;t<num;t++){
   ATurn *newturn=new ATurn;
   newturn->CreateNew(prevturn);
   Add(newturn);
   prevturn=newturn;
  }
 }
 SetCurTurn(num);
}
void ATurn::PrepareOrders()
{
 foreach(RegionList){
  ARegion *reg=*(ARegion**)iter;
  reg->PrepareOrders();
 }
 AllOrdersRunned=false;
}
void ATurn::RunOrders()
{
 TValueRestorer<bool> vr(optClearEventsOnRunRegionOrders);
 if(optClearEventsOnRunRegionOrders)
   ClearEvents();
 optClearEventsOnRunRegionOrders=false;
 TMapII claims;
 foreach(RegionList){
  ARegion *reg=*(ARegion**)iter;
  reg->RunOrders(true);
  for(TMapII::iterator i=reg->claims.begin();i!=reg->claims.end();i++)
    claims[i->first]+=i->second;
 }
 AnsiString str;
 if(!optConquest)
 foreach(curFactions){
  AFaction *fac=*(AFaction**)iter;
  if(!fac->local) continue;
  if(optCombinedTaxTrade==0)
  {
    if(fac->CountTaxRegions()>fac->warmax){
     str="Error: faction ";
     str+=fac->FullName();
     str+=" tax in many regions";
     AddEvent(str);
    }
    if(fac->CountTradeRegions()>fac->trademax){
     str="Error: faction ";
     str+=fac->FullName();
     str+=" trading in many regions";
     AddEvent(str);
    }
    if(fac->warpts+fac->tradepts+fac->magepts<MaxFactionsPoints){
     str="Warning: faction ";
     str+=fac->FullName();
     str+=" does not use all faction points";
     AddEvent(str);
    }
	}else if(optCombinedTaxTrade==1)
	{
		if(fac->CountTaxRegions()+fac->CountTradeRegions()>fac->warmax){
		 str="Error: faction ";
		 str+=fac->FullName();
		 str+=" tax+trade in many regions";
		 AddEvent(str);
		}
	}else if(optCombinedTaxTrade==-1)
	{
	}
	int claimed=claims[fac->number];
  if(fac->unclaimed<claimed)
  {
   str="Warning: faction ";
   str+=fac->FullName();
   str+=" attempt claims $";
   str+=claimed;
   str+=" from $";
   str+=fac->unclaimed;
   AddEvent(str);
  }
 }
 AllOrdersRunned=true;
}
void ATurn::RunAutoOrders()
{
 if(optClearEventsOnRunRegionOrders)
   ClearEvents();
 foreach(RegionList){
  ARegion *reg=*(ARegion**)iter;
  reg->RunAutoOrders1();
 }
 AllOrdersRunned=false;
// RunOrders();
 AddNoClearOnBeginEvents();
}
/*void ATurn::Update0(ATurn *newturn)
{
 if(num!=0) return;
 if(newturn->num==0) return;
 foreach(newturn->RegionList){
  ARegion *newreg=*(ARegion**) iter;
  ARegion *reg0=RegionList->Get(newreg->xloc,newreg->yloc,newreg->zloc);
  if(!reg0){
   reg0=new ARegion(this);
   reg0->xloc=newreg->xloc;
   reg0->yloc=newreg->yloc;
   reg0->zloc=newreg->zloc;
   RegionList->Add(reg0);
   ARegionArray *ra=RegionList->GetRegionArray(reg0->zloc);
   ra->VerifySize(reg0->xloc, reg0->yloc);
  }
  reg0->Update0(newreg);
 }
}*/

BYTE ATurns::Tertypes(AnsiString s)
{
 int ind=tertypes->IndexOf(s);
 if(ind==-1){
  ind=tertypes->Add(s);
 }
 if(ind>250)
  throw Exception("Too many terrain types");
 return BYTE(ind);
}
WORD ATurns::Provincies(AnsiString s)
{
 int ind=provincies->IndexOf(s);
 if(ind==-1){
  ind=provincies->Add(s);
 }
 if(ind>65000)
  throw Exception("Too many provincies");
 return (WORD)ind;
}
WORD ATurns::Towns(AnsiString s)
{
 int ind=towns->IndexOf(s);
 if(ind==-1){
  ind=towns->Add(s);
 }
 if(ind>65000)
  throw Exception("Too many towns");
 return (WORD)ind;
}
BYTE ATurns::Towntypes(AnsiString s)
{
 int ind=towntypes->IndexOf(s);
 if(ind==-1){
  ind=towntypes->Add(s);
 }
 if(ind>250)
  throw Exception("Too many city types");
 return BYTE(ind);
}


int ATurns::GetSize()
{
 int siz=sizeof(*this);
 siz+=Capacity*4;
 siz+=ver1.Length();
 siz+=ver2.Length();
 siz+=4*sizeof(TStringList);
 
 foreach(this){
  ATurn *turn=*(ATurn**) iter;
  if(!turn) continue;
  siz+=turn->GetSize();
 }
 return siz;
}


int ATurn::GetSize()
{
 int siz=sizeof(*this);
 siz+=RegionList->GetSize();
 siz+=factions->GetSize();
 return siz;
}




bool ATurn::SaveMap(TFileStream * file,int mapoptions)
{
 int oldcurfac=0;
 if(curFaction) oldcurfac=curFaction->number;
 curFaction=0;
 bool notallreg=!(mapoptions&roAllRegion);
 bool regionlist=mapoptions&roRegionList;
 if(regionlist) notallreg=false;
 foreach(RegionList){
  ARegion *reg=*(ARegion**)iter;
//  if(reg->zloc==0) continue;
  if(notallreg){
   if(reg->oldinfoturnnum!=num)
    continue;
/*   AFaction *fac=factions->GetFaction(reg->lastviewfact);
   if(!fac->local)
//   if(=number&&!reg->PresentFaction(this))
    continue;*/
  }
  if(regionlist&&!InRegionList(reg))
    continue;
  if(!reg->WriteReport(file,mapoptions)) return false;
 }
 curFactions->SetCurFaction(oldcurfac); 
 return true;
}


int ATurns::GetNextTurnNum(int num)
{
 if(count<=num+1) return -1; //curturns
 ATurn *turn;
 while(1){
  num++;
  turn=Get(num);
  if(turn) break;
 }
 return num;
}

int ATurns::GetPrevTurnNum(int num)
{
 if(count<=0) return -1;
 ATurn *turn;
 while(1){
  num--;
  turn=Get(num);
  if(turn) break;
 }
 return num;
}


bool ATurns::PreloadTurn(int num)
{
 ATurn *turn=Get(num),*prev;
 if(turn) return true;
 int prevloadednum=this->GetPrevTurnNum(num);
 int keyturnnum=num-num%keyturn;
 int beginnum=-1;
 if(prevloadednum>=keyturnnum) keyturnnum=prevloadednum;
 else{
  turn=new ATurn;
  turn->num=keyturnnum;
  turn->Read();
  if(turn->num==-1){
   delete turn;
   if(keyturnnum==num) return false;
   beginnum=keyturnnum+1;
   keyturnnum=prevloadednum;
  }else{
   prev=Get(prevloadednum);
   turn->RegionList->UpdateFromPrev(prev->RegionList);
   turn->turns=this;
   Items[keyturnnum]=turn;
  }
 }
 if(beginnum<0) beginnum=keyturnnum+1;
 prev=Get(keyturnnum);
 LockTurn(keyturnnum);
 for(int i=beginnum;i<=num;i++){
  turn=new ATurn;
  turn->num=i;
  turn->Read();
  if(turn->num==-1){
   delete turn;
   continue;
  }
  turn->RegionList->UpdateFromPrev(prev->RegionList);
  if(prev->num!=keyturnnum){
   prev->Write();
   delete prev;prev=0;
  }
  if(i==num){
   turn->turns=this;
   Items[num]=turn;
   break;
  }
  prev=turn;
 }
 if(prev&&prev->num!=keyturnnum){
  delete prev;
 }
/* turn=Get(num);
 if(turn){
  turn->VerifyNextTurn();
  return true;
 }else return false;*/
 UnlockTurn(keyturnnum);
 return Get(num)!=0;
}
bool ATurns::PreloadTurnIfNeed(int num, bool question)
{
 if(Get(num))
  return true;
 AnsiString str;
 if(question){
  str="Turn ";
  str+=num;
  str+=" not loaded. Load it now?";
  if(Application->MessageBox(str.c_str(),"",MB_YESNO)!=IDYES) return false;
 }
 if(!PreloadTurn(num)){
  str="Error loading turn ";
  str+=num;
  Application->MessageBox(str.c_str(),"ERROR",MB_OK);
  return false;
 }
 return true;
}
int ATurns::LoadedCount(AnsiString *list)
{
 int lc=0;
 for(int i=0,endi=count;i<endi;i++){
  if(!Get(i)) continue;
  if(list){
   if(lc) *list+=",";
   *list+=i;
  }
  lc++;
 }
 return lc;
}


void ATurns::MinimizeTurns(int curnum)
{
 int keynum=curbeginturn-curbeginturn%keyturn;
 for(int i=1;i<curbeginturn;i++){
  ATurn *turn=Get(i);
  if(!turn) continue;
  if(i==curnum) continue;
  if(i==keynum) continue;
  UnlockTurn(i);
  turn->Write();
  Items[i]=0;
  delete turn;
 }
}


void ATurns::UpdateTurns()
{
 ATurn *turn, *prev=0;
 bool delturn,delprev;
 int first=0;
 for(int i=0,endi=count;i<endi;i++){
  turn=Get(i);
  if(!turn){
   delturn=true;
   turn=new ATurn;
   turn->num=i;
   turn->Read();
	 if(turn->num==-1){
    if(first){
     turn->CreateNew(prev);
     turn->num=i;
    }else{
     delete turn;
     continue;
    }
   }
  }else delturn=false;
  if(first==0) first=i;
  turn->Modified=true;
  if(prev){
	 turn->RegionList->UpdateFromPrev(prev->RegionList);
   turn->RegionList->UpdateAdvProdsFromPrev(prev->RegionList);
	 if(delprev){
    prev->Write();
    delete prev;
   }
  }
  prev=turn;
  delprev=delturn;
 }
 if(prev&&delprev){
  prev->Write();
  delete prev;
 }
}

void __fastcall ATurns::SetFacComments(int facnum, AnsiString value)
{
 int ind=GetFacCommentIndex(facnum);
 if(ind<0){
  if(!value.Length())
   return;
  faccomments->AddObject(value,(TObject*)facnum); 
 }else{
  if(!value.Length()){
   faccomments->Delete(ind);
  }else{
   faccomments->Strings[ind]=value;
  }
 }
}
AnsiString __fastcall ATurns::GetFacComments(int facnum)
{
 AnsiString str;
 int ind=GetFacCommentIndex(facnum);
 if(ind>=0)
  str=faccomments->Strings[ind];
 return str;
}
int ATurns::GetFacCommentIndex(int facnum){
 if(facnum<0) return -1;
 for(int i=0,endi=faccomments->Count;i<endi;i++){
  int num=(int)faccomments->Objects[i];
  if(num==facnum)
   return i;
 }
 return -1;
}
void __fastcall ATurn::SetRegUserInfos(ARegion * reg, AnsiString value)
{
 AnsiString key;
 key.printf("%d,%d",reg->xloc,reg->yloc);
 if(reg->zloc!=1)
  key+=AnsiString().sprintf(",%d",reg->zloc);
 FRegUserInfos->Values[key]=value;
}
AnsiString __fastcall ATurn::GetRegUserInfos(ARegion * reg)
{
 AnsiString key;
 key.printf("%d,%d",reg->xloc,reg->yloc);
 if(reg->zloc!=1)
  key+=AnsiString().sprintf(",%d",reg->zloc);
 return FRegUserInfos->Values[key];
}
void ATurns::FillEventsWarning()
{
  EventsWarnings->Clear();
  EventsWarnings->Add("prevented by amulet");
  EventsWarnings->Add(" stolen.");
  EventsWarnings->Add(" steals ");
  EventsWarnings->Add("Is refused entry");
  EventsWarnings->Add("is stopped by guards");
  EventsWarnings->Add("Is forbidden entry to");
  EventsWarnings->Add("Forbids entry to ");
  EventsWarnings->Add(" is caught ");
  EventsWarnings->Add("silver for maintenance.");
}
bool ATurn::FixShafts(int * added, int * deleted)
{
 if(added) *added=0;
 if(deleted) *deleted=0;
 foreach(RegionList){
  ARegion *reg=*(ARegion**) iter;
  ALocation inner;
  foreach(reg)
  {
    AObject *obj=*(AObject**) iter;
    if(!obj->inner) continue;
    if(obj->innerloc.xloc==-1) continue;
    if(inner.xloc!=-1)
    {
      if(obj->innerloc==inner)
      {
        obj->innerloc.xloc=-1;
        if(deleted) (*deleted)++;
        continue;
      }
    }
    inner=obj->innerloc;
    ARegion *reg2=RegionList->Get(inner);
    if(!reg2){
      obj->innerloc.xloc=-1;
      if(deleted) (*deleted)++;
      continue;
    }
    AObject *shaft2=0;
    foreach(reg2)
    {
      AObject *obj2=*(AObject**)iter;
      if(!obj2->inner) continue;
      if(obj2->innerloc==*reg)
      {
        shaft2=obj2;
        break;
      }
      if(obj2->innerloc.xloc!=-1) continue;
      if(!shaft2)
        shaft2=obj2;
    }
    if(shaft2)
    {
      if(shaft2->innerloc==*reg) continue;
      shaft2->innerloc=*reg;
      if(added) (*added)++;
      continue;
    }else{
      obj->innerloc.xloc=-1;
      if(deleted) (*deleted)++;
      continue;
    }
  }
 }
 return true;
}
void ATurns::Delete(int num)
{
  ATurn *turn=Get(num);
  Items[num]=0;
  delete turn;
}
void ATurns::UnPlug()
{
  for(int i=0;i<count;i++)
  {
    ATurn *turn=Get(i);
    if(!turn) continue;
    foreach(turn->RegionList)
    {
      ARegion *reg=*(ARegion**)iter;
      foreach(reg)
      {
        AObject *obj=*(AObject**)iter;
        foreach(obj->units)
        {
          AUnit *un=*(AUnit**)iter;
          un->UnPlug();
        }
      }
    }
  }
}
void ATurns::LockTurn(int turn)
{
  const int usedmul=100;
  for(Locks *l=locks.begin().operator ->();l<locks.end().operator ->();l++)
  {
    if(l->turn!=turn) continue;
    l->locks++;
    l->used++;
    l->tick=GetTickCount()+l->used*usedmul;
    return;
  }
  Locks l(turn);
  l.locks++;
  l.used++;
  l.tick=GetTickCount()+l.used*usedmul;
  locks.push_back(l);
}
void ATurns::UnlockTurn(int turn, bool clean)
{
  for(Locks *l=locks.begin().operator ->();l<locks.end().operator ->();l++)
  {
    if(l->turn!=turn) continue;
    if(l->locks)
      l->locks--;
    if(clean)
      CleanTurns();
    return;
  }
}
void ATurns::CleanTurns()
{
  if(!optMaxMemoryTurns) return;
  if(locks.size()<optMaxMemoryTurns)
    return;
  stable_sort(locks.begin(),locks.end());
  for(Locks *l=locks.end().operator ->()-1;l>=locks.begin().operator ->();l--)
  {
    if(l->locks) continue;
    if(l->turn==curTurn->num) continue;
    Delete(l->turn);
    locks.erase(l);
    if(locks.size()<optMaxMemoryTurns)
      break;
  }
}

