//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <shellapi.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <classes.hpp>
#include <FileCtrl.hpp>
#include <stdio.h>

#include "util.h"
#ifdef RULESONLY
#include "gamedata.h"
#include "gamedefs.h"
#include "items.h"
#else
#include "acplugins.h"
#include "EventsFrm.h"
#include "turn.h"
#include "faction.h"
#include "region.h"
#include "reports.h"
#include "mainfrm.h"
#include "unit.h"
#include "eapfile.h"
#endif
#include "army.h"
#include "ReqFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
AnsiString verSign="Ver ";

#ifndef RULESONLY
extern AnsiString ClientPath;
bool optShowEvents=true;
bool optColorUnit=false;
bool optRunRegionOrders=true;
int optTownNameLimit=5;
bool optMaximizeWindow=true;
bool optShowLDescFirst=true; //показывать LocalDescr даже если есть имя
bool optUseOldMail=false; //использовать почту через Clipboard
int optDontShowEvents;
int optSaveMap=roNonuserObject|roAllRegion|roRegionInfo;
bool optCopyOnSaveOrders=false; //сохранять в буфер обмена при Game\Save Orders
AnsiString LoadReportsDir,LoadOrdersDir,LoadBattlesDir,LoadRegionListDir;
AnsiString OrdersDir="orders\\",ReportsDir="reports\\",StoreDir="store\\";
AnsiString optServEMail,optServOrdersSubj; //адрес сервера и сабж для приказов
AnsiString optServTimesSubj,optServTimesBody; //заголовок и тело для запроса новостей
AnsiString optServMailSubj; //заголовок для серверной почты
bool optNoServerMail=false, //использовать почту через email, а не через сервер
 optServNoCross=false,      //использовать флаг NOCROSS
 optServNoSpoils=false,     //использовать флаг NOSPOILS
 optServGiveAll=false,      //использовать команду Give All Except
 optServerSharing=false,    //использовать флаг SHARING
 optServSpoils=false;       //использовать команду SPOILS
int optServFlightOverWater=WFLIGHT_NONE;
bool optConquest=false;
bool optBoldUndergroundGrid=false;
int optAutoSaveTimer=0;
bool optUpdateMovingMap=true;
bool optTaxIsMonthlong=false;
bool optMagicDeep=false;
bool optHideGrid=false;
bool optAlwaysShowMovement=true;
unsigned optMaxMemoryTurns=0;
int optCombinedTaxTrade=0;
AnsiString optRunRegionOrdersEventPrefix;
bool optClearEventsOnRunRegionOrders=false;
AnsiString optPrevNextUnitExpr;
//---------------------------------------------------------------------------

// Events
void AddEvent(AnsiString ev){
  EventsForm->AddEvent(ev);
}

void ClearEvents(){
  EventsForm->ClearEvents();
}

void BeginEvents(){
  EventsForm->Begin();
}

void AddNoClearOnBeginEvents()
{
  EventsForm->AddNoClearOnBegin();
}

void DisableEvents()
{
  EventsForm->Disabled=true;
}

void EnableEvents()
{
  EventsForm->Disabled=false;
}
#endif
//---------------------------------------------------------------------------

TMemoryStream* LoadFile(AnsiString fName)
{
  TMemoryStream *mem = new TMemoryStream;
  mem->LoadFromFile(fName);
  mem->SetSize((int)mem->Size+1);
  char *from = ((char*)mem->Memory), *to = from;
  int MemSize = mem->Size-1;
  from[MemSize] = 0;
  while(*from)
  {
	if(*from != '\r') *to++=*from;
	from++;
  }
  *to = 0;
  return mem;
}

#ifndef RULESONLY
int ReportTab=0;
AnsiString Trunc(AnsiString &str,int len)
{
  if(str.Length()<=len) return "";
  char *s=str.c_str();
  int pos=len;
  while(pos>=0&&s[pos]!=' ') pos--;
  if(pos<0) return "";
  AnsiString str2=str.SubString(pos+2,str.Length());
  str.SetLength(pos);
  return str2;
}

void SaveRepString(TStream *strm, AnsiString str)
{
  if(str.Pos("\n"))
  {
	TStringList *list=new TStringList;
	list->Text=str;
	SaveRepStringList(strm,list);
	delete list;
	return;
  }
  bool comment = false;
  if(*str.c_str()==';')
  {
	comment=true;
	str=str.SubString(2,str.Length());
  }
  AnsiString tabchar=AnsiString::StringOfChar(' ',ReportTab);
  if(*str.c_str()!=' '&&*str.c_str()!=0) str=tabchar+str;
  AnsiString s,str2=Trunc(str);

  if(comment) s=";"; else s="";
  s+=str+"\n";
  strm->Write(s.c_str(),s.Length());

  while(str2.Length())
  {
	str="  "+tabchar+str2;
	str2=Trunc(str);
	if(comment) s=";"; else s="";
	s+=str+"\n";
	strm->Write(s.c_str(),s.Length());
  }
}

void SaveRepStringList(TStream *strm, TStringList *list)
{
  AnsiString s;
  for(int i=0,endi=list->Count;i<endi;i++)
	SaveRepString(strm,list->Strings[i]);
}

void ReadStringList(TFile &in,TStringList *list)
{
  int kol;
  in>>kol;
  AnsiString s;
  list->Clear();
  for(int i=0;i<kol;i++)
  {
	in>>s;
	list->Add(s);
  }
}

void WriteStringList(TFile &out,TStringList *list)
{
  int kol=list->Count;
  out<<kol;
  AnsiString s;
  for(int i=0;i<kol;i++)
  {
	s=list->Strings[i];
	out<<s;
  }
}
#endif

int CompareIStr(AnsiString s1, AnsiString s2)
{
  return stricmp(s1.c_str(),s2.c_str());
}

bool MyCompare(const char *str, AnsiString pattern)
{
  if( strncmpi(str, pattern.c_str(), pattern.Length()) )
  {
	if(pattern[1] == ' ' && pattern[2] == ' ')
	{
	  pattern = pattern.Delete(1, 1);
	  return strncmpi(str, pattern.c_str(), pattern.Length()) ;
	} else return true;
  }
  return false;
}

const char*endtoken=" \n,.():;[]&|>=<!{}"; //+-*/

AnsiString GetNextToken(char*&buf)
{
  char*s,*old=buf;
  if(*buf==0) return "";
  if(*buf=='\"')
  {
	old++;
	s=buf+1;
	while(*s!='\"')s++;
	s++;
	buf=s;
	return AnsiString(old,s-old-1);
  }
   else
  {
	s=buf+1;
	while(*s&&strchr(endtoken,*s)==0)s++;
	buf=s;
	return AnsiString(old,s-old);
  }
}

AnsiString GetNextStringNew(char*&buf)
{
  char *s=buf;
  int i = 0;

  while ((i < sizeof(s)) && (s[i] != '\n') ) i++;


  while(*s&&*s!='\n')s++;
  AnsiString str(buf,s-buf);

  if(*s == ' ')
	if(s[1] == ' ')s++;

  buf = s;
  return str;

}

AnsiString GetNextString(char*&buf, int nextstrtab)
{
  bool verend=false;
  int worktab=nextstrtab;
  if(nextstrtab<0){ verend=true;worktab=-nextstrtab;}
  char *s=buf;
  while(*s&&*s!='\n')s++;
  AnsiString str(buf,s-buf);
  if(*s=='\n')s++;
  if(nextstrtab)
  {
	int i;
	for(i=0;i<worktab;i++)
	{
	  if(s[i]!=' ') break;
	}
	if(i==worktab)
	{
	  if(!verend||(s[worktab+1]!='-'&&s[worktab+1]!='*'))
	  {
		s += worktab;
		if(*s != ' ') s--;
		str += GetNextString(s,nextstrtab);
	  }
	}
  }
  buf=s;
  return str;
}

AnsiString GetNextFactionName(char*&buf){
 if(*buf=='('){
   buf++;
   return "";
 }
 char* s=buf;
 while(*s!='('&&*s!='\n'&&*s/*&&*s!=','&&*s!=';'&&*s!='.'*/)s++;
 if(*s!='(')return "";
 AnsiString str(buf,s-buf-1);
 buf=s+1;
 return str;
}
AnsiString GetNextItemName(char*&buf){
 if(*buf=='['){
   buf++;
   return "";
 }
 char* s=buf;
 while(*s!='['&&*s!='\n'&&*s&&*s!=','&&*s!=';')s++;
 if(*s!='[')return "";
 AnsiString str(buf,s-buf-1);
 str = Trim(str);
 buf=s+1;
 return str;
}
AnsiString GetNextObjectName(char*&buf){
 if(*buf=='['){
   buf++;
   return "";
 }
 char* s=buf;
 while((*s!='['||!isdigit(s[1]))&&*s!='\n'&&*s)s++;
 if(*s!='[')return "";
 AnsiString str(buf,s-buf-1);
 buf=s+1;
 return str;
}
#ifndef RULESONLY
ALocation* GetNextLocation(char*&buf, bool updatelevels){
 int x,y,z;
 char *s=buf;
 if(*s!='(')return 0;
 s++;
 x=GetNextToken(s).ToInt();
 if(*s!=',')return 0;
 s++;
 y=GetNextToken(s).ToInt();
 AnsiString levelname;
 if(*s==','){
  s++;
  char *olds=s;
  while(*s&&*s!=')') s++;
  if(*s)
   levelname=AnsiString(olds,s-olds);
 }
 if(updatelevels&&!ProcessUpdateRegionArray(levelname))return 0;
 z=curRegionList->RegionArrays->GetLevelNum(levelname);
 if(z==-1){
  return 0;
 }
 if(*s!=')')return 0;
 s++;
 ALocation *loc=new ALocation;
 loc->xloc=x;
 loc->yloc=y;
 loc->zloc=z;
 buf=s;
 return loc;
}
#endif
//---------------------------------------------------------------------------

// Terrains
const char*TerrainTxt = "terrain.txt";
ATerrainTypes*TerrainTypes;
ATerrainType::ATerrainType()
  :movepoints(0),mountstypes(0),color(clGray)
  ,ocean(false)
{
  advlist=new TStringList;
}

ATerrainType::~ATerrainType()
{
  delete advlist;
}

AnsiString ATerrainType::Print(bool quotation)
{
  AnsiString s;
  if(quotation)s += "\"";
  s += name;
  if(quotation)s += "\"";
  s += ",";
  s += movepoints;
  if(ocean) s += ",OCEAN";
  if(mountstypes&mtRiding)s += ",RIDING";
  if(mountstypes&mtFlying)s += ",FLYING";
  for(int i=0;i<advlist->Count;i++){
    s += ",\""+advlist->Strings[i] + "\"";
  }
  s += ":";
  s += ColorToString(color);
  return s;
}

bool ATerrainType::CanProduceAdv(AnsiString abr)
{
  return advlist->IndexOf(abr) != -1;
}

int ATerrainType::GetImageIndex(AnsiString weather)
{
#ifdef RULESONLY
  return -1;
#else
  int ind = WeatherTypes->GetIndex(weather);
  if(ind<0) ind = WeatherTypes->normalindex;
  int imgind = imginds[ind];
  if(imgind >= 0)
    return imgind;
  return imginds[WeatherTypes->normalindex];
#endif
}

bool ATerrainType::LoadImage()
{
#ifdef RULESONLY
  return false;
#else
  int size = WeatherTypes->count;
  if(!size)
    throw Exception("WeatherTypes->count=0 in ATerrainType::LoadImage");
  int normalind = WeatherTypes->normalindex;
  AnsiString fname;
  imginds.resize(size);
  for(int i=0;i<size;i++)
  {
    if(i == normalind)
      fname = name;
    else
      fname = name + "_"+WeatherTypes->Get(i)->name;
    imginds[i] = AtlaForm->AddMapImage(fname);
  }
  return imginds[normalind] >= 0;
#endif
}

bool ATerrainType::SaveImage()
{
#ifdef RULESONLY
  return false;
#else
  int size=WeatherTypes->count;
  int normalind=WeatherTypes->normalindex;
  AnsiString fname;
  for(int i=0;i<size;i++)
  {
    if(imginds[i]<0)
      continue;
    if(i==normalind)
      fname=name;
    else
      fname=name+"_"+WeatherTypes->Get(i)->name;
    TBmp *bmp=new TBmp;
    AtlaForm->MapImages->GetBitmap(imginds[i],bmp);
    fname=ClientPath+"images\\terrain\\"+fname+"_.bmp";
    bmp->SaveToFile(fname);
    delete bmp;
  }
  return true;
#endif
}

ATerrainTypes::ATerrainTypes()
 :TList()
{
#ifndef RULESONLY
 TMemoryStream *mem=LoadFile(GetGameFileName(TerrainTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 AnsiString str;
 ATerrainType*ttype;
 int filever=0;
 if(!MyCompare(s,verSign)){
  s+=verSign.Length();
  filever=atoi(s);
  GetNextString(s);
 }
 if(filever>1)
	 throw Exception(AnsiString("Unsupported file version"));
 while(*s=='\"'){
	ttype=new ATerrainType;
	ttype->name=GetNextToken(s);
	if(*s!=','){
	 throw Exception(AnsiString("Bad format by \"")+ttype->name+"\": movecost not found");
	}
  if(filever==0&&ttype->name=="ocean")
    ttype->ocean=true;
  s++;
  str=GetNextToken(s);
  ttype->movepoints=str.ToInt();
  while(*s==','){
   s++;
   if(*s=='\"'){
    str=GetNextToken(s);
    AItemType *itype=ItemTypes->Get(str);
    if(!itype)
     throw Exception(AnsiString("Bad format by \"")+ttype->name+"\": unknown adv item type: "+str);
    if((itype->type&AItemType::IT_ADVANCED)==0)
     throw Exception(AnsiString("Bad format by \"")+ttype->name+"\": item type: "+str+" is not advanced");
    ttype->advlist->Add(itype->abr);
   }else{
    str=GetNextToken(s);
    if(str=="RIDING")  ttype->mountstypes|=ATerrainType::mtRiding;
    else if(str=="FLYING") ttype->mountstypes|=ATerrainType::mtFlying;
    else if(str=="OCEAN") ttype->ocean=true;
    else throw Exception(AnsiString("Bad format by \"")+ttype->name+"\": unknown mountstype: "+str);
   }
  }
  if(*s==':'){
   s++;
   str=GetNextToken(s);
   ttype->color=StringToColor(str);
  }
  if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+ttype->name+"\": unknown mountstype");
  s++;
  AddTerrainType(ttype);
 }
 delete mem;
#endif
 modified=false;

#ifndef RULESONLY
 AtlaForm->MapImages->Clear();
 foreach(this)
 {
   ATerrainType *ttype=*(ATerrainType**)iter;
   ttype->LoadImage();
//   ttype->SaveImage();
 }
extern int InRegListIndex,UnExploredIndex,UnVisitedIndex;
 InRegListIndex=AtlaForm->AddMapImage("..\\inreglist");
 UnExploredIndex=AtlaForm->AddMapImage("..\\unexplored");
 UnVisitedIndex=AtlaForm->AddMapImage("..\\unvisited");
#endif
}
__fastcall ATerrainTypes::~ATerrainTypes()
{
 for(int i=Count-1;i>=0;i--){
  ATerrainType*tt=(ATerrainType*)Items[i];
//  Delete(i);
  delete tt;
 }
 TList::Clear();
 if(TerrainTypes==this)TerrainTypes=0;
}
ATerrainType* ATerrainTypes::Get(int index)
{
 return (ATerrainType*)Items[index];
}

void ATerrainTypes::AddTerrainType(ATerrainType * ttype)
{
 for(int i=Count-1;i>=0;i--){
  if(ttype->name==Get(i)->name)throw Exception(AnsiString("Terrain \"")+ttype->name+"\" already present");
 }
 Add(ttype);
 modified=true;
}

void ATerrainTypes::Save()
{
 TFileStream *file=new TFileStream(GetGameFileName(TerrainTxt),fmCreate);//fmOpenWrite);
 AnsiString s=verSign+"1\r\n";
 file->Write(s.c_str(),s.Length());
 for(int i=0;i<Count;i++){
  AnsiString s=Get(i)->Print(true)+"\r\n";
  file->Write(s.c_str(),s.Length());
 }
 delete file;
 modified=false;
}

ATerrainType* ATerrainTypes::Get(AnsiString terrain)
{
 if(!terrain.Length()) return 0;
 for(int i=Count-1;i>=0;i--){
  if(!CompareIStr(terrain,Get(i)->name))return Get(i);
 }
 return 0;
}

bool ATerrainTypes::IsOcean(AnsiString terrain)
{
  ATerrainType *ttype=Get(terrain);
  if(!ttype) return false;
  return ttype->ocean;
}
//---------------------------------------------------------------------------

// Weather
#ifndef RULESONLY
const char*WeatherTxt="weather.txt";
AWeatherTypes*WeatherTypes;
AWeatherType::AWeatherType()
 :movemult(2)
{
}
AnsiString AWeatherType::Print()
{
 AnsiString s="\"";
 s+=name+"\","+movemult+",\""+curmonthname+"\",\""+nextmonthname+"\"";
 return s;
}
AWeatherTypes::AWeatherTypes()
 :TList(),normalindex(0)
{
 TMemoryStream *mem=LoadFile(GetGameFileName(WeatherTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 AnsiString str;
 AWeatherType*wtype;
 while(*s=='\"'){
  wtype=new AWeatherType;
  wtype->name=GetNextToken(s);
  if(*s!=','){
   throw Exception(AnsiString("Bad format by \"")+wtype->name+"\": movemult not found");
  }
  s++;
  str=GetNextToken(s);
  wtype->movemult=str.ToInt();
  if(*s!=','||s[1]!='\"'){
   throw Exception(AnsiString("Bad format by \"")+wtype->name+"\": curmounthname not found");
  }
  s++;
  str=GetNextToken(s);
  wtype->curmonthname=str;
  if(*s!=','||s[1]!='\"'){
   throw Exception(AnsiString("Bad format by \"")+wtype->name+"\": nextmounthname not found");
  }
  s++;
  str=GetNextToken(s);
  wtype->nextmonthname=str;
  if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+wtype->name+"\": bad end");
  s++;
  if(!wtype->name.AnsiCompareIC("Normal"))
    normalindex=count;
  AddWeatherType(wtype);
 }
 delete mem;
 if(normalindex<0)
   normalindex=0;
 modified=false;
}
__fastcall AWeatherTypes::~AWeatherTypes()
{
 for(int i=Count-1;i>=0;i--){
  AWeatherType*wt=(AWeatherType*)Items[i];
//  Delete(i);
  delete wt;
 }
 TList::Clear();
 if(WeatherTypes==this)WeatherTypes=0;
}
AWeatherType* AWeatherTypes::Get(int index)
{
 return (AWeatherType*)Items[index];
}
AWeatherType* AWeatherTypes::Get(AnsiString name)
{
 foreach(this){
  AWeatherType *wtype=*(AWeatherType**)iter;
  if(wtype->name==name)return wtype;
 }
 return 0;
}
int AWeatherTypes::GetIndex(AnsiString name)
{
  int ind=0;
  foreach(this){
    AWeatherType *wtype=*(AWeatherType**)iter;
    if(wtype->name==name)return ind;
    ind++;
  }
  return -1;
}
void AWeatherTypes::AddWeatherType(AWeatherType * wtype)
{
 for(int i=Count-1;i>=0;i--){
  if(wtype->name==Get(i)->name)throw Exception(AnsiString("Weather \"")+wtype->name+"\" already present");
 }
 Add(wtype);
 modified=true;
}
void AWeatherTypes::Save()
{
 TFileStream *file=new TFileStream(GetGameFileName(WeatherTxt),fmCreate);//fmOpenWrite);
 for(int i=0;i<Count;i++){
  AnsiString s=Get(i)->Print()+"\r\n";
  file->Write(s.c_str(),s.Length());
 }
 delete file;
 modified=false;
}
AnsiString AWeatherTypes::FindCurWeather(AnsiString text)
{
 foreach(this){
  AWeatherType *wtype=*(AWeatherType**)iter;
  if(wtype->curmonthname==text)return wtype->name;
 }
 return "";
}
AnsiString AWeatherTypes::FindNextWeather(AnsiString text)
{
 foreach(this){
  AWeatherType *wtype=*(AWeatherType**)iter;
  if(wtype->nextmonthname==text)return wtype->name;
 }
 return "";
}
//---------------------------------------------------------------------------

// Directs
const char*DirectTxt="direct.txt";
AnsiString ADirType::Print()
{
 AnsiString s="\"";
 s+=name;
 s+="\",\"";
 s+=abr;
 s+="\",";
 s+=num;
 return s;
}
ADirTypes*DirTypes;
ADirTypes::ADirTypes()
 :TList()
{
 TMemoryStream *mem=LoadFile(GetGameFileName(DirectTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 AnsiString str;
 ADirType*dtype;
 while(*s=='\"'){
  dtype=new ADirType;
  dtype->name=GetNextToken(s);
  if(*s!=','){
   throw Exception(AnsiString("Bad format by \"")+dtype->name+"\": abridgement not found");
  }
  s++;
  str=GetNextToken(s);
  dtype->abr=str;
  if(*s!=','){
   throw Exception(AnsiString("Bad format by \"")+dtype->name+"\": number not found");
  }
  s++;
  dtype->num=GetNextToken(s).ToInt();
  if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+dtype->name+"\": bad end");
  s++;
  Add(dtype);
 }
 if(Count>NDIRS)throw Exception("Too many directions");
 delete mem;
 modified=false;
}
__fastcall ADirTypes::~ADirTypes()
{
 for(int i=Count-1;i>=0;i--){
  ADirType*dt=Get(i);
//  Delete(i);
  delete dt;
 }
 TList::Clear();
 if(DirTypes==this)DirTypes=0;
}

void ADirTypes::MakeSkillList(TStringList *list)
{
  if(!list) return;
  foreach(this){
    ADirType *stype = *(ADirType**)iter;
    list->Add(stype->name);
  }
}

ADirType* ADirTypes::Get(int index)
{
 return (ADirType*)Items[index];
}
void ADirTypes::AddDirType(ADirType * dtype)
{
 if(dtype->num!=count)throw Exception("Error ADirTypes::AddDirType(): number not match");
 if(Find(dtype->name)!=-1)throw Exception(AnsiString("Direction \"")+dtype->name+"\" already present");
 if(Find(dtype->abr)!=-1)throw Exception(AnsiString("Direction \"")+dtype->abr+"\" already present");
 Add(dtype);
 modified=true;
}
int ADirTypes::Find(AnsiString str)
{
 for(int i=Count-1;i>=0;i--){
  ADirType *dt=Get(i);
  if(!dt->name.AnsiCompareIC(str)||!dt->abr.AnsiCompareIC(str))return i;
 }
 return -1;
}
void ADirTypes::Save()
{
 TFileStream *file=new TFileStream(GetGameFileName(DirectTxt),fmCreate);//fmOpenWrite);
 for(int i=0;i<count;i++){
  AnsiString s=Get(i)->Print()+"\r\n";
  file->Write(s.c_str(),s.Length());
 }
 delete file;
}
#endif
//---------------------------------------------------------------------------

// Skills
const char*SkillsTxt="skills.txt";
ASkillTypes*SkillTypes;
ASkillType::ASkillType()
 :cost(100),flags(0),sortindex(0)
{
}
AnsiString ASkillType::Print(bool quotation)
{
 AnsiString s;
 if(quotation)s+="\"";
 s+=name;
 if(quotation)s+="\"";
 s+=",";
 if(quotation)s+="\"";
 s+=abr;
 if(quotation)s+="\"";
 s+=",";
 s+=cost;
 if(flags&MAGIC)s+=",MAGIC";
 if(flags&COMBAT)s+=",COMBAT";
 if(flags&CAST)s+=",CAST";
 if(flags&FOUNDATION)s+=",FOUNDATION";
 for(int i=0;i<3;i++){
  if(depends[i].Length()==0)break;
  s+=",";
  if(quotation)s+="\"";
  s+=depends[i];
  if(quotation)s+="\"";
  s+=",";
  s+=levels[i];
 }
 s+=":S:";
 s+=sortindex;
 return s;
}
void ASkillType::CreateNew(ASkillType* prevskill)
{
 name=prevskill->name;
 abr=prevskill->abr;
 cost=prevskill->cost;
 flags=prevskill->flags;
 for(int i=0;i<3;i++){
  depends[i]=prevskill->depends[i];
  levels[i]=prevskill->levels[i];
 }
 sortindex=prevskill->sortindex;
}
AnsiString ASkillType::FullName()
{
 return name+" ["+abr+"]";
}

ASkillTypes::ASkillTypes()
  :slist(new TList)
{
#ifndef RULESONLY
 TMemoryStream *mem=LoadFile(GetGameFileName(SkillsTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 AnsiString str;
 ASkillType*stype;
 while(*s=='\"'){
  stype=new ASkillType;
  stype->name=GetNextToken(s);
  if(*s!=','||s[1]!='\"'){
   throw Exception(AnsiString("Bad format by \"")+stype->name+"\": abridgement not found");
  }
  s++;
  stype->abr=GetNextToken(s);
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+stype->name+"\": cost not found");
  s++;
  str=GetNextToken(s);
  stype->cost=str.ToInt();
  while(*s==','&&s[1]!='\"'){
   s++;
   str=GetNextToken(s);
   if(str=="MAGIC")  stype->flags|=ASkillType::MAGIC;
   else if(str=="COMBAT")  stype->flags|=ASkillType::COMBAT;
   else if(str=="CAST")  stype->flags|=ASkillType::CAST;
   else if(str=="FOUNDATION")  stype->flags|=ASkillType::FOUNDATION;
   else throw Exception(AnsiString("Bad format by \"")+stype->name+"\": unknown flag: "+str);
  }
  int i=0;
  while(*s==','&&s[1]=='\"'){
   if(i>2)
    throw Exception(AnsiString("Bad format by \"")+stype->name+"\": too many depends");
   s++;
   str=GetNextToken(s);
   if(!Get(str)){
//    throw Exception(AnsiString("Bad format by \"")+stype->name+"\": unknown depends: "+str);
    AddEvent("Warning for \""+stype->name+"\": may be unknown depends: "+str);
   }
   stype->depends[i]=str;
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+stype->name+"\": depends level not found");
   s++;
   str=GetNextToken(s);
   stype->levels[i]=str.ToInt();
   i++;
  }
  if(*s==':'&&s[1]=='S'&&s[2]==':')
  {
    s+=3;
    stype->sortindex=atoi(GetNextToken(s).c_str());
  }
  if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+stype->name+"\": bad end");
  s++;
  AddSkillType(stype);
 }
 delete mem;
 UpdateSortedList();
#endif
 modified=false;
}
__fastcall ASkillTypes::~ASkillTypes()
{
 for(int i=Count-1;i>=0;i--){
  ASkillType*dt=Get(i);
//  Delete(i);
  delete dt;
 }
 TList::Clear();
 delete slist;
 if(SkillTypes==this)SkillTypes=0;
}
ASkillType* ASkillTypes::Get(int index)
{
 return (ASkillType*)Items[index];
}
ASkillType* ASkillTypes::Get(AnsiString name,int mode, bool addunknown)
{
 if(mode&2)for(int i=Count-1;i>=0;i--){
  ASkillType *st=Get(i);
  if(!CompareIStr(st->abr,name))return st;
 }
 if(mode&1)for(int i=Count-1;i>=0;i--){
  ASkillType *st=Get(i);
  if(!CompareIStr(st->name,name))return st;
 }
 if(addunknown)
 {
   AnsiString unkname="Unknown"+name;
   AnsiString str="In savebase found unknown skill: "+name+"\nAdd it as "+unkname+"?";
   int ret=Application->MessageBox(str.c_str(),"Error",MB_YESNO);
   if(ret==ID_YES)
   {
     ASkillType* stype=new ASkillType;
     stype->abr=name;
     stype->name=unkname;
     stype->cost=100000;
     stype->sortindex=count;
     AddSkillType(stype);
     UpdateSortedList();
     return stype;
   }
 }
 return 0;
}
void ASkillTypes::AddSkillType(ASkillType* stype)
{
 if(Get(stype->name))throw Exception(AnsiString("Skill \"")+stype->name+"\" already present");
 if(Get(stype->abr))throw Exception(AnsiString("Skill \"")+stype->abr+"\" already present");
 Add(stype);
 modified=true;
}
void ASkillTypes::Save()
{
 TFileStream *file=new TFileStream(GetGameFileName(SkillsTxt),fmCreate);//fmOpenWrite);
 for(int i=0;i<Count;i++){
  AnsiString s=Get(i)->Print(true)+"\r\n";
  file->Write(s.c_str(),s.Length());
 }
 delete file;
 modified=false;
}
//0 - not found, 1 - present, 2 - name, not abr, 3 - abr, not name
int ASkillTypes::VerSkills(AnsiString name, AnsiString abr)
{
 ASkillType*st;
 st=Get(name,1);
 if(st){
  if(st->abr==abr)return 1;
  return 2;
 }
 st=Get(abr,2);
 if(st)return 3;
 return 0;
}
void ASkillTypes::MakeSkillList(TStringList *list)
{
 if(!list)return;
 foreach(this){
  ASkillType *stype=*(ASkillType**)iter;
  list->Add(stype->name);
 }
}
AnsiString ASkillTypes::AbrToName(AnsiString abr)
{
 ASkillType*stype=Get(abr,2);
 if(stype)return stype->name;
 return "";
}
AnsiString ASkillTypes::NameToAbr(AnsiString name)
{
 ASkillType*stype=Get(name,1);
 if(stype)return stype->abr;
 return "";
}
void ASkillTypes::UpdateSortedList()
{
  slist->Clear();
  foreach(this)
  {
    ASkillType *stype=*(ASkillType**)iter;
    int sind=stype->sortindex;
    int i;
    for(i=0;i<slist->Count;i++)
    {
      ASkillType *st=GetSorted(i);
      if(st->sortindex>sind)
        break;
    }
    slist->Insert(i,stype);
  }
  for(int i=0;i<slist->Count;i++)
  {
    ASkillType *st=GetSorted(i);
    st->sortindex=i;
  }
}
ASkillType* ASkillTypes::GetSorted(int index)
{
  return (ASkillType*)slist->Items[index];
}
//---------------------------------------------------------------------------

// Skills Info
const char*SkillRepTxt="skillrep.txt";
ASkillInfos*SkillInfos;
ASkillInfo::ASkillInfo()
 :level(0)
{
}
AnsiString ASkillInfo::Print(bool quotation)
{
 AnsiString s;
 if(quotation)s+="\"";
 s+=skill;
 if(quotation)s+="\"";
 s+=",";
 s+=level;
 s+=",";
 if(quotation)s+="\"";
 s+=text;
 if(quotation)s+="\"";
 return s;
}
AnsiString ASkillInfo::FullName()
{
 return skill+" ("+level+")";
}
void ASkillInfo::CreateNew(ASkillInfo * prevsi)
{
 skill=prevsi->skill;
 level=prevsi->level;
 text=prevsi->text;
}

ASkillInfos::ASkillInfos()
{
#ifndef RULESONLY
 TMemoryStream *mem=LoadFile(GetGameFileName(SkillRepTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 AnsiString str;
 ASkillInfo*sInfo;
 while(*s=='\"'){
  sInfo=new ASkillInfo;
  sInfo->skill=GetNextToken(s);
  if(!SkillTypes->Get(sInfo->skill))throw Exception(AnsiString("Unknown skill \"")+sInfo->skill+"\"");
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+sInfo->skill+"\": level not found");
  s++;
  str=GetNextToken(s);
  sInfo->level=str.ToInt();
  if(*s!=','||s[1]!='\"'){
   throw Exception(AnsiString("Bad format by \"")+sInfo->skill+"\": text not found");
  }
  s++;
  sInfo->text=GetNextToken(s);
  if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+sInfo->skill+"\": bad end");
  s++;
  AddSkillInfo(sInfo);
 }
 delete mem;
#endif
 modified=false;
}
__fastcall ASkillInfos::~ASkillInfos()
{
 for(int i=Count-1;i>=0;i--){
  ASkillInfo*dt=Get(i);
//  Delete(i);
  delete dt;
 }
 TList::Clear();
 if(SkillInfos==this)SkillInfos=0;
}
ASkillInfo* ASkillInfos::Get(int index)
{
 return (ASkillInfo*)Items[index];
}
ASkillInfo* ASkillInfos::Get(AnsiString skill, int level)
{
 for(int i=Count-1;i>=0;i--){
  ASkillInfo*st=Get(i);
  if(!CompareIStr(st->skill,skill)&&st->level==level)return st;
 }
 return 0;
}
void ASkillInfos::AddSkillInfo(ASkillInfo* sInfo)
{
 if(sInfo->level<1)throw Exception(AnsiString("Skill Info ")+sInfo->skill+" ("+sInfo->level+"): bad level");
 if(Get(sInfo->skill,sInfo->level))throw Exception(AnsiString("Skill Info for ")+sInfo->skill+" ("+sInfo->level+") already present");
 Add(sInfo);
 modified=true;
}
void ASkillInfos::Save()
{
 TFileStream *file=new TFileStream(GetGameFileName(SkillRepTxt),fmCreate);//fmOpenWrite);
 for(int i=0;i<Count;i++){
  AnsiString s=Get(i)->Print(true)+"\r\n";
  file->Write(s.c_str(),s.Length());
 }
 delete file;
 modified=false;
}
//---------------------------------------------------------------------------

// Items & Objects Info
const char*ItObjRepTxt="itobjrep.txt";
AItObjInfos*ItObjInfos;
AItObjInfo::AItObjInfo()
 :type(OBJECT)
{
}

AnsiString AItObjInfo::Print(bool quotation)
{
 AnsiString s;
 if(quotation)s+="\"";
 s+=abr;
 if(quotation)s+="\"";
 if(quotation)
 {
   s+=",";
   s+=type;
 }
 s+=",";
 if(quotation)s+="\"";
 s+=text;
 if(quotation)s+="\"";
 return s;
}
AnsiString AItObjInfo::FullName()
{
 AnsiString str=abr+" (";
 switch(type)
 {
   case OBJECT:str+="object";break;
   case ITEM:str+="item";break;
   default:str+="unknown";
 }
 str+=")";
 return str;
}
void AItObjInfo::CreateNew(AItObjInfo * previoi)
{
 abr=previoi->abr;
 type=previoi->type;
 text=previoi->text;
}

AItObjInfos::AItObjInfos()
{
#ifndef RULESONLY
 if(!FileExists(GetGameFileName(ItObjRepTxt)))
 {
   modified=true;
   return;
 }
 TMemoryStream *mem=LoadFile(GetGameFileName(ItObjRepTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 AnsiString str;
 AItObjInfo*ioi;
 while(*s=='\"'){
  ioi=new AItObjInfo;
  ioi->abr=GetNextToken(s);
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+ioi->abr+"\": type not found");
  s++;
  str=GetNextToken(s);
  ioi->type=str.ToInt();
  if(ioi->type==AItObjInfo::OBJECT)
  {
    if(!ObjectTypes->Get(ioi->abr))
    throw Exception(AnsiString("Unknown object \"")+ioi->abr+"\"");
  }else if(ioi->type==AItObjInfo::ITEM)
  {
    if(!ItemTypes->Get(ioi->abr,4))
      throw Exception(AnsiString("Unknown item \"")+ioi->abr+"\"");
  }else
  {
    throw Exception(AnsiString("Bad type for \"")+ioi->abr+"\"");
  }
  if(*s!=','||s[1]!='\"'){
   throw Exception(AnsiString("Bad format by \"")+ioi->abr+"\": text not found");
  }
  s++;
  ioi->text=GetNextToken(s);
  if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+ioi->abr+"\": bad end");
  s++;
  AddItObjInfo(ioi);
 }
 delete mem;
#endif
 modified=false;
}
__fastcall AItObjInfos::~AItObjInfos()
{
 for(int i=Count-1;i>=0;i--){
  AItObjInfo*dt=Get(i);
//  Delete(i);
  delete dt;
 }
 TList::Clear();
 if(ItObjInfos==this)ItObjInfos=0;
}
AItObjInfo* AItObjInfos::Get(int index)
{
 return (AItObjInfo*)Items[index];
}
AItObjInfo* AItObjInfos::Get(AnsiString abr, int type)
{
 for(int i=Count-1;i>=0;i--){
  AItObjInfo*ioi=Get(i);
  if(!CompareIStr(ioi->abr,abr)&&ioi->type==type)return ioi;
 }
 return 0;
}
void AItObjInfos::AddItObjInfo(AItObjInfo* ioi)
{
 if(Get(ioi->abr,ioi->type))throw Exception("Item or Object Info for "+ioi->FullName()+" already present");
 Add(ioi);
 modified=true;
}
void AItObjInfos::Save()
{
 TFileStream *file=new TFileStream(GetGameFileName(ItObjRepTxt),fmCreate);//fmOpenWrite);
 for(int i=0;i<Count;i++){
  AnsiString s=Get(i)->Print(true)+"\r\n";
  file->Write(s.c_str(),s.Length());
 }
 delete file;
 modified=false;
}
//---------------------------------------------------------------------------

// Items
const char*ItemsTxt="items.txt";
AItemTypes*ItemTypes;
AItemType::AItemType()
 :flags(0),level(0),weight(0),type(0),combat(true),
  walk(0),ride(0),fly(0),swim(0),helpprodmult(0)
 ,hitchwalk(0),hitchride(0),hitchfly(0),hitchswim(0)
 ,months(1),out(1),withdrawcost(0)
{
  for(int i=0;i<INPUTCOUNT;i++)
    numbers[i]=0;
}
AnsiString AItemType::Print(bool quotation)
{
 AnsiString s;
 if(quotation)s+="\"";
 s+=name;
 if(quotation)s+="\"";
 s+=",";
 if(quotation)s+="\"";
 s+=names;
 if(quotation)s+="\"";
 s+=",";
 if(quotation)s+="\"";
 s+=abr;
 if(quotation)s+="\"";
 if(flags&CANTGIVE)s+=",CANTGIVE";
 if(skill.Length()){
  s+=",";
  if(quotation)s+="\"";
  s+=skill;
  if(quotation)s+="\"";
  s+=",";
  s+=level;
  for(int i=0;i<INPUTCOUNT;i++)
  {
   if(!inputs[i].Length())
     break;
   if(i)
    s+=":";
   else 
    s+=",";
   if(quotation)s+="\"";
   s+=inputs[i];
   if(quotation)s+="\"";
   s+=",";
   s+=numbers[i];
  }
  s+=",";
  s+=months;
  s+=",";
  s+=out;
 }
 s+=",";
 s+=weight;
 if(type&IT_NORMAL)s+=",NORMAL";
 if(type&IT_ADVANCED)s+=",ADVANCED";
 if(type&IT_TRADE)s+=",TRADE";
 if(type&IT_MAN)s+=",MAN";
 if(type&IT_MONSTER)s+=",MONSTER";
 if(type&IT_MAGIC)s+=",MAGIC";
 if(type&IT_WEAPON)s+=",WEAPON";
 if(type&IT_ARMOR)s+=",ARMOR";
 if(type&IT_MOUNT)s+=",MOUNT";
 if(type&IT_BATTLE)s+=",BATTLE";
 if(type&IT_FOOD)s+=",FOOD";
 if(type&IT_PRODNOTTRADEACTIVITY)s+=",PRODNOTTRADEACTIVITY";
 if(type&IT_LEADER)s+=",LEADER";
 if(type&IT_ORINPUTS)s+=",ORINPUTS";
 if(type&IT_UNFINISHED)
   s += ",UNFINISHED";
 if(!combat)s+=",DONTSHOWINCOMBAT";

 s+=",";
 s+=walk;
 s+=",";
 s+=ride;
 s+=",";
 s+=fly;
 s+=",";
 s+=swim;
 if(helpprodmult!=0){
  s+=",HELPPROD:";
  s+=helpprodmult;
  if(helpproditem.Length()){
   s+=":";
   if(quotation)s+="\"";
   s+=helpproditem;
   if(quotation)s+="\"";
  }
 }
 if(hitchItem.Length()!=0){
  s+=",HITCH:";
  if(quotation)s+="\"";
  s+=hitchItem;
  if(quotation)s+="\"";
  s+=":";
  s+=hitchwalk;
  s+=",";
  s+=hitchride;
  s+=",";
  s+=hitchfly;
  s+=",";
  s+=hitchswim;
 }
 if(withdrawcost)
 {
  s+=",WITHDRAW:";
  s+=withdrawcost;
 }
 if(needbuilding.Length())
 {
  s+=",NEEDBUILDING:\"";
  s+=needbuilding;
  s+="\"";
 }
 return s;
}

void AItemType::CreateNew(AItemType * previt)
{
 name=previt->name;
 names=previt->names;
 abr=previt->abr;
 flags=previt->flags;
 skill=previt->skill;
 level=previt->level;
 for(int i=0;i<INPUTCOUNT;i++)
 {
   inputs[i]=previt->inputs[i];
   numbers[i]=previt->numbers[i];
 }
 weight=previt->weight;
 type=previt->type;
 combat=previt->combat;
 walk=previt->walk;
 ride=previt->ride;
 fly=previt->fly;
 swim=previt->swim;
}
bool AItemType::IsNewInfo(AItemType *it)
{
 if(abr!=it->abr)return true;
 if(it->name.Length()&&name!=it->name)return true;
 if(it->names.Length()&&names!=it->names)return true;
 if(it->weight==-1)return false;
 if(weight!=it->weight)return true;
 if((type&it->type)!=it->type)return true;
 if(walk!=it->walk)return true;
 if(ride!=it->ride)return true;
// if(fly!=it->fly)return true;
 return false;
}
/*void AItemType::UpdateNewInfo(AItemType * newit)
{
 name=newit->name;
 abr=newit->abr;
 weight=newit->weight;
 type|=newit->type;
 walk=newit->walk;
 ride=newit->ride;
 fly=newit->fly;
}*/
AnsiString AItemType::FullName(bool plural)
{
 AnsiString s;
 if(plural)s=names;else s=name;
 s+=" ["+abr+"]";
 return s;
}
AMonstrType *AItemType::GetMonstrType()
{
  if(!(type&AItemType::IT_MONSTER))
    return 0;
  AnsiString _abr=abr;
  if(AItemTypes::IsIllusionAbr(_abr))
    _abr="ILLU";
  return MonstrTypes->Get(_abr);
}
AItemTypes::AItemTypes()
{
#ifndef RULESONLY
 TMemoryStream *mem=LoadFile(GetGameFileName(ItemsTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 AnsiString str;
 AItemType*itype;
 int filever=0;
 if(!MyCompare(s,verSign)){
  s+=verSign.Length();
  filever=atoi(s);
  GetNextString(s);
 }
 if(filever>4)
	 throw Exception(AnsiString("Unsupported file version"));
 while(*s=='\"'){
  itype=new AItemType;
  itype->name=GetNextToken(s);
  if(*s!=','||s[1]!='\"')
   throw Exception(AnsiString("Bad format by \"")+itype->name+"\": plural not found");
  s++;
  itype->names=GetNextToken(s);
  if(*s!=','||s[1]!='\"')
   throw Exception(AnsiString("Bad format by \"")+itype->name+"\": abridgement not found");
  s++;
  itype->abr=GetNextToken(s);
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": comma not found");
  if(s[1]!='\"'&&!isdigit(s[1])&&s[1]!='-'){
   s++;
   str=GetNextToken(s);
   if(str=="CANTGIVE")itype->flags|=AItemType::CANTGIVE;
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": comma not found");
  }
  if(s[1]=='\"'){
   s++;
   str=GetNextToken(s);
   ASkillType *stype=SkillTypes->Get(str);
   if(!stype)throw Exception(AnsiString("Bad format by \"")+itype->name+"\": unknown skill: "+str);
   itype->skill=stype->abr;
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": level not found");
   s++;
   str=GetNextToken(s);
   itype->level=str.ToInt();
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": comma not found");
   if(s[1]=='\"'){
    s++;
    str=GetNextToken(s);
    if(!Get(str)){
     AddEvent("Warning for \""+itype->name+"\": may be unknown input item: "+str);
    }
    int i=0;
    itype->inputs[i]=str;
    if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": input number not found");
    s++;
    str=GetNextToken(s);
    itype->numbers[i]=str.ToInt();
    while(*s==':'&&++i<AItemType::INPUTCOUNT){
     s++;
     str=GetNextToken(s);
     if(!Get(str)){
      AddEvent("Warning for \""+itype->name+"\": may be unknown input item"+(i+1)+": "+str);
     }
     itype->inputs[i]=str;
     if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": input number"+(i+1)+" not found");
     s++;
     str=GetNextToken(s);
     itype->numbers[i]=str.ToInt();
    }
   }
   if(filever<1)
   {
     int m=0;
     if(itype->inputs[0].Length())
     {
       m+=itype->numbers[0];
       if(itype->inputs[1].Length())
         m+=itype->numbers[1];
     }
     itype->months=m;
     itype->out=1;
   }else if(filever>=3||itype->inputs[0].Length())
   {
     if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": months number not found");
     s++;
     str=GetNextToken(s);
     itype->months=str.ToInt();
     if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": out number not found");
     s++;
     str=GetNextToken(s);
     itype->out=str.ToInt();
   }
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": comma not found");
  }
  s++;
  str=GetNextToken(s);
  itype->weight=str.ToInt();
  while(*s==','&&!isdigit(s[1])){
   s++;
   str=GetNextToken(s);
   if(str=="NORMAL")  itype->type|=AItemType::IT_NORMAL;
   else if(str=="ADVANCED")  itype->type|=AItemType::IT_ADVANCED;
   else if(str=="TRADE")  itype->type|=AItemType::IT_TRADE;
   else if(str=="MAN")  itype->type|=AItemType::IT_MAN;
   else if(str=="MONSTER")  itype->type|=AItemType::IT_MONSTER;
   else if(str=="MAGIC")  itype->type|=AItemType::IT_MAGIC;
   else if(str=="WEAPON")  itype->type|=AItemType::IT_WEAPON;
   else if(str=="ARMOR")  itype->type|=AItemType::IT_ARMOR;
   else if(str=="MOUNT")  itype->type|=AItemType::IT_MOUNT;
   else if(str=="BATTLE")  itype->type|=AItemType::IT_BATTLE;
   else if(str=="FOOD")  itype->type|=AItemType::IT_FOOD;
   else if(str=="PRODNOTTRADEACTIVITY")  itype->type|=AItemType::IT_PRODNOTTRADEACTIVITY;
   else if(str=="LEADER") itype->type|=AItemType::IT_LEADER;
   else if(str=="ORINPUTS") itype->type|=AItemType::IT_ORINPUTS;
   else if(str=="DONTSHOWINCOMBAT")  itype->combat = false;
   else if(str=="UNFINISHED")
   itype->type = AItemType::IT_UNFINISHED;
   else throw Exception(AnsiString("Bad format by \"")+itype->name+"\": unknown type: "+str);
  }
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": walk not found");
  if(filever<4)
  {
    if(optConquest)
    {
      if(itype->abr=="MAN")
        itype->type|=AItemType::IT_LEADER;
    }else{
      if(itype->abr=="LEAD")
        itype->type|=AItemType::IT_LEADER;
    }
  }
  if(itype->type&AItemType::IT_LEADER)
    if(!(itype->type&AItemType::IT_MAN))
      throw Exception(AnsiString("Bad format by \"")+itype->name+"\": only MANs may be LEADERs");
  if(itype->type&AItemType::IT_ORINPUTS&&(!itype->inputs[0].Length()||!itype->inputs[1].Length()))
    throw Exception(AnsiString("Bad format by \"")+itype->name+"\": only item with two or more inputs may be ORINPUTS");
  s++;
  str=GetNextToken(s);
  itype->walk=str.ToInt();
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": ride not found");
  s++;
  str=GetNextToken(s);
  itype->ride=str.ToInt();
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": fly not found");
  s++;
  str=GetNextToken(s);
  itype->fly=str.ToInt();
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": swim not found");
  s++;
  str=GetNextToken(s);
  itype->swim=str.ToInt();
  while(*s==','){
   s++;
   str=GetNextToken(s);
   if(str=="HELPPROD"){
    if(*s!=':')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": ':' not found in HELPPROD");
    s++;
    int mult=1;
    bool hasmult=false;
    if(isdigit(*s)){
     hasmult=true;
     mult=atoi(s);
     GetNextToken(s);
     if(*s==':') s++;
    }
    itype->helpprodmult=mult;
    if(*s=='\"'){
     itype->helpproditem=GetNextToken(s);
    }else if(!hasmult)
     throw Exception(AnsiString("Bad format by \"")+itype->name+"\": item not found in HELPPROD");
   }else if(str=="HITCH"){
    if(*s!=':')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": ':' not found in HITCH");
    s++;
    if(*s!='\"')
     throw Exception(AnsiString("Bad format by \"")+itype->name+"\": item not found in HITCH");
    itype->hitchItem=GetNextToken(s);
    if(*s!=':')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": ':' not found in HITCH");
    s++;
    itype->hitchwalk=GetNextToken(s).ToInt();
    if(filever<2)
    {
      itype->hitchride=0;
      itype->hitchfly=0;
      itype->hitchswim=0;
    }else{
      if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": ',' not found in HITCH");
      s++;
      itype->hitchride=GetNextToken(s).ToInt();
      if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": ',' not found in HITCH");
      s++;
      itype->hitchfly=GetNextToken(s).ToInt();
      if(*s!=',')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": ',' not found in HITCH");
      s++;
      itype->hitchswim=GetNextToken(s).ToInt();
    }
   }else if(str=="WITHDRAW"){
    if(*s!=':')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": ':' not found in WITHDRAW");
    s++;
    itype->withdrawcost=GetNextToken(s).ToInt();
   }else if(str=="NEEDBUILDING"){
    if(*s!=':'||s[1]!='\"')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": ':\"' not found in NEEDBUILDING");
    s++;
    itype->needbuilding=GetNextToken(s);
   }else throw Exception(AnsiString("Bad format by \"")+itype->name+"\": bad end");
  }
  if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+itype->name+"\": bad end");
  s++;
  AddItemType(itype);
 }
 delete mem;
#endif
 modified=false;
}

__fastcall AItemTypes::~AItemTypes()
{
  for(int i=Count-1;i>=0;i--){
    AItemType*it=Get(i);
    delete it;
  }
  TList::Clear();
  if(ItemTypes == this)ItemTypes=0;
}

AItemType* AItemTypes::Get(int index)
{
  return (AItemType*)Items[index];
}

AItemType* AItemTypes::Get(AnsiString name,int mode, bool addunknown)
{
  if(name.Length()==0)return 0;
  if(mode&4)for(int i=Count-1;i>=0;i--){
    AItemType*st = Get(i);
    if(!CompareIStr(st->abr,name))return st;
  }
  if(mode&1)for(int i=Count-1;i>=0;i--){
    AItemType*st=Get(i);
    if(!CompareIStr(st->name,name)&& (!(mode&8)||!IsIllusionAbr(st->abr)))return st;
  }
  if(mode&2)for(int i=Count-1;i>=0;i--){
    AItemType*st=Get(i);
    if(!CompareIStr(st->names,name)&& (!(mode&8)||!IsIllusionAbr(st->abr)))return st;
  }
  if(addunknown){
    AnsiString unkname="Unknown"+name;
    AnsiString str="In savebase found unknown item: "+name+"\nAdd it as "+unkname+"?";
    int ret=Application->MessageBox(str.c_str(),"Error",MB_YESNO);
    if(ret==ID_YES){
      AItemType* itype=new AItemType;
      itype->abr=name;
      itype->name=unkname;
      itype->names=unkname;
      itype->weight=100000;
      AddItemType(itype);
      return itype;
    }
  }
  return 0;
}

void AItemTypes::AddItemType(AItemType* itype)
{
  AItemType *it=Get(itype->abr,4);
  if(it) throw Exception(AnsiString("Item \"")+itype->abr+"\" already present");
  Add(itype);
  modified = true;
}

void AItemTypes::Save()
{
  TFileStream *file=new TFileStream(GetGameFileName(ItemsTxt),fmCreate);//fmOpenWrite);
  AnsiString s=verSign+"4\r\n";
  file->Write(s.c_str(),s.Length());
  for(int i=0;i<Count;i++){
    AnsiString s=Get(i)->Print(true)+"\r\n";
    file->Write(s.c_str(),s.Length());
  }
  delete file;
  modified=false;
}

void AItemTypes::MakeItemList(TStringList* list,int ignoretype)
{
  if(!list)return;
  foreach(this){
    AItemType *itype=*(AItemType**)iter;
    if(itype->type&ignoretype) continue;
    list->Add(itype->name);
  }
}

AnsiString AItemTypes::AbrToName(AnsiString abr)
{
  if(!abr.Length()) return abr;
  foreach(this){
    AItemType *itype=*(AItemType**)iter;
    if(itype->abr==abr)return itype->name;
  }
  return "";
}

AnsiString AItemTypes::AbrToNames(AnsiString abr)
{
  if(!abr.Length()) return abr;
  foreach(this){
    AItemType *itype=*(AItemType**)iter;
    if(itype->abr==abr){
      if(itype->names.Length()) return itype->names;
        else return itype->name;
    }
  }
  return "";
}

AnsiString AItemTypes::NameToAbr(AnsiString name)
{
  if(!name.Length()) return name;
  foreach(this){
    AItemType *itype=*(AItemType**)iter;
    if(itype->name==name||itype->names==name)return itype->abr;
  }
  return "";
}

const unsigned char charRusA='А', charEngA='A';

bool AItemTypes::IsIllusionAbr(AnsiString abr)
{
  if(abr.Length()<1) return false;
  unsigned char c = abr[1];
  if(c >= charRusA) return true;
  return false;
}

AnsiString AItemTypes::IllusionAbrToNormal(AnsiString iabr)
{
  AnsiString abr=iabr;
  if(abr.Length()<1) return abr;
  unsigned char c=abr[1];
  if(c<charRusA) return abr;
  c=(unsigned char)(c-charRusA+charEngA);
  abr[1]=c;
  return abr;
}

AnsiString AItemTypes::NormalAbrToIllusion(AnsiString abr)
{
  if(abr.Length()<1) return abr;
  unsigned char c=abr[1];
  if(c>=charRusA) return abr;
  c=(unsigned char)(c-charEngA+charRusA);
  abr[1]=c;
  return abr;
}
//---------------------------------------------------------------------------

// Town types
#ifndef RULESONLY
const char*TownTypeTxt="towntype.txt";
ATownTypes*TownTypes;
ATownType::ATownType()
 :imgnum(-1)
{
}

AnsiString ATownType::Print(){
  AnsiString s="\"";
  s+=name+"\"";
  if(imgnum>=0) s += ",\"" + imgname + "\"";
  return s;
};

ATownTypes::ATownTypes()
{
  TMemoryStream *mem=LoadFile(GetGameFileName(TownTypeTxt));
  char*memory=(char*)mem->Memory,*s=memory;
  AnsiString str;
  ATownType *ttype;
  while(*s=='\"'){
    ttype=new ATownType;
    ttype->name=GetNextToken(s);
    if(*s==','){
      s++;
      ttype->imgname=GetNextToken(s);
      ttype->imgnum=AtlaForm->AddInfoImage(ttype->imgname+".bmp");
    }
    if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+ttype->name+"\": bad end");
    s++;
    AddTownType(ttype);
  }
  delete mem;
  modified=false;
}

__fastcall ATownTypes::~ATownTypes()
{
  for(int i=Count-1;i>=0;i--){
    ATownType*it=Get(i);
    delete it;
  }
  TList::Clear();
  if(TownTypes==this)TownTypes=0;
}

ATownType* ATownTypes::Get(int index){
  return (ATownType*)Items[index];
}

ATownType* ATownTypes::Get(AnsiString type)
{
  for(int i=0;i<count;i++){
    if(Get(i)->name==type)return Get(i);
  }
  return 0;
}

int ATownTypes::GetIndex(AnsiString type)
{
  for(int i=0;i<count;i++){
    if(Get(i)->name==type)return i;
  }
  return -1;
}

void ATownTypes::AddTownType(ATownType *type)
{
  if(GetIndex(type->name)!=-1)throw Exception(AnsiString("Town type \"")+type->name+"\" already present");
  Add(type);
  modified=true;
}

void ATownTypes::Save()
{
  TFileStream *file=new TFileStream(GetGameFileName(TownTypeTxt),fmCreate);//fmOpenWrite);
  for(int i=0;i<Count;i++){
    AnsiString s=Get(i)->Print()+"\r\n";
    file->Write(s.c_str(),s.Length());
  }
  delete file;
}
#endif
//---------------------------------------------------------------------------

// Key Phrases
const char*KeyPhrasesTxt="keyphrases.txt";
AKeyPhrases*KeyPhrases;
AKeyPhrases::AKeyPhrases()
{
 TMemoryStream *mem=LoadFile(GetGameFileName(KeyPhrasesTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 AnsiString str;
 while(*s=='\"'){
  str=GetNextToken(s);
  Add(str);
  if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+str+"\": bad end");
  s++;
 }
 if(NKEYPHRASES!=Count)throw Exception("Keyphrases count mismatch");
 delete mem;
}
__fastcall AKeyPhrases::~AKeyPhrases()
{
 if(KeyPhrases==this)KeyPhrases=0;
}
AnsiString __fastcall AKeyPhrases::Get(int index)
{
 if(index<0||index>=Count)return "";
 return TStringList::Get(index);
}
void __fastcall AKeyPhrases::Set(int index,AnsiString str)
{
 if(index<0||index>=Count)return;
 Strings[index]=str;
}
void AKeyPhrases::Save()
{
 TFileStream *file=new TFileStream(GetGameFileName(KeyPhrasesTxt),fmCreate);//fmOpenWrite);
 for(int i=0;i<Count;i++){
  AnsiString s="\""+Get(i)+"\"\r\n";
  file->Write(s.c_str(),s.Length());
 }
 delete file;
}

AnsiString curGame;
TStringList*Games;
AnsiString GetGameFileName(AnsiString fName){
 AnsiString str=ClientPath;
 if(curGame.Length())str+=curGame+"\\";
 str+=fName;
 return str;
}
#ifndef RULESONLY
AnsiString GetOrderName(int facnum){
 char buf[80];
 sprintf(buf,"%sord%05d.%03d",OrdersDir.c_str(),curTurn->num,facnum);
 return GetGameFileName(buf);
}
AnsiString GetReportName(int facnum,AnsiString basedir){
 char buf[80];
 if(!basedir.Length())
  basedir=ReportsDir;
 sprintf(buf,"%srep%05d.%03d",basedir.c_str(),curTurn->num,facnum);
 return GetGameFileName(buf);
}
void LoadGameBase(){
#define LOAD_TIME
#ifdef LOAD_TIME
 long t0=GetTickCount(),t1;
 AnsiString tt="Loading ";
#endif
 DisableAutoSaveTimer();
 if(curGame.Length()){
  AddEvent("Loading game \""+curGame+"\" ...");
  ForceDirectories(GetGameFileName(ReportsDir));
  LoadOrdersDir=GetGameFileName(OrdersDir);
  ForceDirectories(LoadOrdersDir);
  ForceDirectories(GetGameFileName(StoreDir));
 }else
  AddEvent("Loading default settings ...");
 try{
  delete WeatherTypes;WeatherTypes=0;
  WeatherTypes=new AWeatherTypes;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init weather types",MB_OK);
  _exit(0);
 }
 AddEvent("Weather types initialized");
 try{
  delete WeatherRegions;WeatherRegions=0;
  WeatherRegions=new AWeatherRegions;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init weather regions",MB_OK);
  _exit(0);
 }
 AddEvent("Weather regions initialized");
 try{
  delete DirTypes;DirTypes=0;
  DirTypes=new ADirTypes;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init direction types",MB_OK);
  _exit(0);
 }
 AddEvent("Direction types initialized");
 try{
  delete SkillTypes;
  SkillTypes=new ASkillTypes;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init skill types",MB_OK);
  _exit(0);
 }
 AddEvent("Skill types initialized");
 try{
  delete SkillInfos;
  SkillInfos=new ASkillInfos;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init skill infos",MB_OK);
  _exit(0);
 }
 AddEvent("Skill infos initialized");
 try{
  delete ItemTypes;ItemTypes=0;
  ItemTypes=new AItemTypes;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init item types",MB_OK);
  _exit(0);
 }
 AddEvent("Item types initialized");
 try{
  delete TerrainTypes;TerrainTypes=0;
  TerrainTypes=new ATerrainTypes;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init terrain types",MB_OK);
  _exit(0);
 }
 AddEvent("Terrain types initialized");
 try{
  delete TownTypes;TownTypes=0;
  TownTypes=new ATownTypes;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init town types",MB_OK);
  _exit(0);
 }
 AddEvent("Town types initialized");
 try{
  delete ObjectTypes;
  ObjectTypes=new AObjectTypes;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init object types",MB_OK);
  _exit(0);
 }
 AddEvent("Object types initialized");
 try{
  delete ItObjInfos;
  ItObjInfos=new AItObjInfos;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init item&object infos",MB_OK);
  _exit(0);
 }
 AddEvent("Item&object infos initialized");
 try{
  delete WeaponTypes;
  WeaponTypes=new AWeaponTypes;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init weapon types",MB_OK);
  _exit(0);
 }
 AddEvent("Weapon types initialized");
 try{
  delete ArmorTypes;
  ArmorTypes=new AArmorTypes;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init armor types",MB_OK);
  _exit(0);
 }
 AddEvent("Armor types initialized");
 try{
  delete MountTypes;
  MountTypes=new AMountTypes;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init mount types",MB_OK);
  _exit(0);
 }
 AddEvent("Mount types initialized");
 try{
  delete SpellTypes;
  SpellTypes=new ASpellTypes;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init spell types",MB_OK);
  _exit(0);
 }
 AddEvent("Spell types initialized");
 try{
  delete MonstrTypes;
  MonstrTypes=new AMonstrTypes;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init monster types",MB_OK);
  _exit(0);
 }
 AddEvent("Monster types initialized");
 try{
  delete BatItemTypes;
  BatItemTypes=new ABatItemTypes;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init batitem types",MB_OK);
  _exit(0);
 }
 AddEvent("Batitem types initialized");
 try{
  delete ManTypes;
  ManTypes=new AManTypes;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init Man types",MB_OK);
  _exit(0);
 }
 AddEvent("Man types initialized");
 try{
  delete KeyPhrases;KeyPhrases=0;
  KeyPhrases=new AKeyPhrases;
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init key phrases",MB_OK);
  _exit(0);
 }
 AddEvent("Key phrases initialized");
 if(!curGame.Length())
 {
   AtlaForm->VerGameLoaded();
   return;
 }
 try{
  delete curTurns;curTurns=0;
  Plugins.UpdatePluginInfo();
  curTurns=new ATurns;
  curTurns->Read();
#ifdef LOAD_TIME
 t1=GetTickCount();
 t0=t1-t0;
 tt+=t0*0.001;
 tt+=" sec ";
#endif
//  curTurns->Get(0)->VerifyNextTurn();
  curTurns->RestoreLastParam();
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error init turns info",MB_OK);
  _exit(0);
 }
 seedrandom( time( 0 )/*1783*/ );
 AtlaForm->LookLocalFactions();
 AddEvent("Loading complete");
#ifdef LOAD_TIME
  //t1=GetTickCount()-t1;
  //tt+=t1;
  AddEvent(tt);
#endif
 AtlaForm->OnGameLoaded();
}

void SaveGameBase(){
 DisableAutoSaveTimer();
 if(curGame.Length()){
  AddEvent("Saving game \""+curGame+"\" ...");
  ForceDirectories(GetGameFileName(ReportsDir));
  ForceDirectories(GetGameFileName(OrdersDir));
  ForceDirectories(GetGameFileName(StoreDir));
  curTurns->StoreLastParam();
  curTurns->Write();
 }else
  AddEvent("Saving default settings ...");

 try{
  TerrainTypes->Save();
  WeatherTypes->Save();
  WeatherRegions->Save();
  DirTypes->Save();
  SkillTypes->Save();
  SkillInfos->Save();
  ItemTypes->Save();
  TownTypes->Save();
  KeyPhrases->Save();
  ObjectTypes->Save();
  ItObjInfos->Save();
  WeaponTypes->Save();
  ArmorTypes->Save();
  MountTypes->Save();
  SpellTypes->Save();
  MonstrTypes->Save();
  BatItemTypes->Save();
  ManTypes->Save();
 }catch(Exception&e){
   Application->MessageBox(e.Message.c_str(),"Error save types",MB_OK);
//   _exit(0);
   return;
 }
 if(!curGame.Length())return;
 AddEvent("Saving complete");
 Plugins.OnGameSaved();
 DeleteAutoSaveFiles();
 SetAutoSaveTimer();
}
#endif
void DeleteGameBase(){
#ifndef RULESONLY
 delete WeatherTypes;
 delete WeatherRegions;
 delete DirTypes;
#endif
 delete SkillTypes;
 delete SkillInfos;
 delete ItemTypes;
 delete TerrainTypes;
 delete ObjectTypes;
 delete WeaponTypes;
 delete ArmorTypes;
 delete MountTypes;
 delete MonstrTypes;
 delete BatItemTypes;
 delete ManTypes;
#ifndef RULESONLY
 delete SpellTypes;
 delete TownTypes;
#endif
 delete KeyPhrases;
#ifndef RULESONLY
 delete curTurns;
#endif
}

#ifndef RULESONLY
void SetMainFormCaption(){
 AnsiString s="AtlaClient";
 if(curGame.Length()){
  s+=" Game \"";
  s+=curGame+"\"";
  if(curTurn){
   s+=" turn ";
   s+=curTurn->num;
   if(curTurn->num) s+=" ("+TurnNumberToString(curTurn->num)+") ";
  }
  if(curFaction&&curFaction->number){
   s+=" for ";
   s+=curFaction->FullName();
  }
  if(curRegion){
   s+=" ";
   s+=curRegion->FullName(false);
   s+=" ";
   s+=curRegion->lastviewfact;
  }
/*  if(curUnit){
   s+=" ";
   s+=curUnit->FullName();
  }*/
 }
 AtlaForm->Caption=s;
}
AnsiString MakeDepSkillStr(AnsiString skill,int level){
 if(!SkillTypes)return "";
 ASkillType*stype=SkillTypes->Get(skill);
 if(!stype)return "";//{skill=stype->name;}
 return stype->name+","+level;
}
AnsiString MakeInputItemStr(AnsiString item,int number){
 if(!ItemTypes)return "";
 AItemType*itype=ItemTypes->Get(item);
 if(!itype)return "";//{skill=stype->name;}
 return itype->name+","+number;
}

char *MonthNames[] = {
  "January",
  "February",
  "March",
  "April",
  "May",
  "June",
  "July",
  "August",
  "September",
  "October",
  "November",
  "December"
};
int GetTurnNumberFromString(AnsiString str){
 char*s=str.c_str();
 AnsiString monthname=GetNextToken(s);
 int month;
 for(month=0;month<12;month++){
  if(monthname==MonthNames[month])break;
 }
 if(month==12)throw Exception(AnsiString("Error Process Turn Number: unknown month ")+monthname);
 while(*s&&!isdigit(*s))s++;
 if(!*s)throw Exception("Error Process Turn Number: year number not found");
 int year=atoi(s);
 return (year-1)*12+month+1;
}
AnsiString TurnNumberToString(int number){
 number--;
 int month=(number%12);
 int year=number/12+1;
 AnsiString str=MonthNames[month];
 str+=", Year ";
 str+=year;
 return str;
}
#endif

#ifdef RULESONLY
AnsiString DirTypesStr[6]={
 "N","NE","SE","S","SW","NW"
};
#endif
AnsiString WoodOrStoneStr="WOOD_OR_STONE";
//---------------------------------------------------------------------------

// Object Types
const char*ObjectTypeTxt="objtype.txt";
AObjectTypes *ObjectTypes;
static AnsiString RoadStr=",ROAD:";
static AnsiString BuildNotTradeStr=",BUILDNOTTRADEACTIVITY";
AObjectType::AObjectType()
 :canenter(0),capacity(0),cost(-1),level(-1),
	protect(0),sailors(0),road(0),flags(0),helpprodmult(0)
{
}
AnsiString AObjectType::Print(bool was)
{
  AnsiString s;
  if(was) s += "\"";
  s += name;
  if(was)s += "\"";
  s += ",";
  if(was) { s += protect;  s += ","; }
    else if(protect) { s += "protect="; s += protect; s += ","; }

  if(was) { s += capacity;  s += ","; }
    else if(capacity){ s += "capacity="; s += capacity;  s += ","; }

  if(cost != -1){
    if(was)s += "\"";
    s += item;
    if(was)s += "\"";
    s += ",";
    s += cost;
    s += ",";
    if(level != -1){
      if(was) s += "\"";
      s += skill;
      if(was) s += "\"";
      s += ",";
      s += level;
      s += ",";
    }
  }

  if(was) { s += sailors;  s += ","; }
    else if(sailors) { s += "sailors=";  s += sailors; s += ","; }

  if(was) s += canenter; else s += canenter?"can enter":"can't enter";
  if(production.Length()){
    s += ",";
    if(was) s += "\"";
    s += production;
    if(was) s += "\"";
  }

  if(flags&OT_BUILDNOTTRADEACTIVITY) s += BuildNotTradeStr;
  if(helpprodmult != 0){
    s += ",HELPPROD:";
    s += helpprodmult;
    if(helpproditem.Length()){
      s += ":";
      s += "\"";
      s += helpproditem;
      s += "\"";
    }
  }
  if(road){
    s += RoadStr;
    bool first=true;
    for(int i=0;i<NDIRS;i++){
      if(!(road&(1<<i))) continue;
      if(first) first = false;
	 else s += ",";
#ifdef RULESONLY
	 s += DirTypesStr[i];
#else
	 ADirType *dt = DirTypes->Get(i);
	 s += dt->abr;
#endif
    }
  }
  return s;
}

static int __fastcall SortObjCompare(void * a, void * b){
  if(((AObjectType*)a)->canenter < ((AObjectType*)b)->canenter) return true;
  return ((AObjectType*)a)->name.AnsiCompareIC(((AObjectType*)b)->name);
}

AObjectTypes::AObjectTypes()
{
#ifndef RULESONLY
 TMemoryStream *mem=LoadFile(GetGameFileName(ObjectTypeTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 AnsiString str;
 AObjectType *otype;
 while(*s=='\"'){
  otype=new AObjectType;
  otype->name=GetNextToken(s);
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+otype->name+"\": protect not found");
  s++;
  str=GetNextToken(s);
  otype->protect=atoi(str.c_str());
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+otype->name+"\": capacity not found");
  s++;
  str=GetNextToken(s);
  otype->capacity=atoi(str.c_str());
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+otype->name+"\": sailors not found");
  if(s[1]=='\"'){
   s++;
   str=GetNextToken(s);
   if(str==WoodOrStoneStr);
   else if(!ItemTypes->Get(str,4))throw Exception(AnsiString("Bad format by \"")+otype->name+"\": item \""+str+"\" not found");
   otype->item=str;
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+otype->name+"\": cost not found");
   s++;
   str=GetNextToken(s);
   otype->cost=atoi(str.c_str());
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+otype->name+"\": sailors not found");
   if(s[1]=='\"'){
    s++;
    str=GetNextToken(s);
    ASkillType *stype=SkillTypes->Get(str);
    if(!stype)throw Exception(AnsiString("Bad format by \"")+otype->name+"\": skill \""+str+"\" not found");
    otype->skill=stype->abr;
    if(*s!=',')throw Exception(AnsiString("Bad format by \"")+otype->name+"\": level not found");
    s++;
    str=GetNextToken(s);
    otype->level=atoi(str.c_str());
   }
  }
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+otype->name+"\": sailors not found");
  s++;
  str=GetNextToken(s);
  otype->sailors=atoi(str.c_str());
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+otype->name+"\": canenter not found");
  s++;
  str=GetNextToken(s);
  otype->canenter=atoi(str.c_str());
  if(*s==','&&s[1]=='\"'){
   s+=1;
   str=GetNextToken(s);
   if(!ItemTypes->Get(str,4))throw Exception(AnsiString("Bad format by \"")+otype->name+"\": item \""+str+"\" not found");
   otype->production=str;
  }
	if(!MyCompare(s,BuildNotTradeStr))
  {
   s+=BuildNotTradeStr.Length();
   otype->flags|=AObjectType::OT_BUILDNOTTRADEACTIVITY;
  }
	if(!MyCompare(s,",HELPPROD")){
	  GetNextToken(s);
		if(*s!=':')throw Exception(AnsiString("Bad format by \"")+otype->name+"\": ':' not found in HELPPROD");
		s++;
		int mult=1;
		bool hasmult=false;
		if(isdigit(*s)){
		 hasmult=true;
		 mult=atoi(s);
		 GetNextToken(s);
		 if(*s==':') s++;
		}
		otype->helpprodmult=mult;
		if(*s=='\"'){
		 otype->helpproditem=GetNextToken(s);
		}else if(!hasmult)
		 throw Exception(AnsiString("Bad format by \"")+otype->name+"\": item not found in HELPPROD");
	}
	if(!MyCompare(s,RoadStr)){
   s+=RoadStr.Length();
   char *olds=s;
   while(*s!='\n'){
    str=GetNextToken(s);
    int dir=DirTypes->Find(str);
    if(dir==-1) break;
    olds=s;
    otype->road|=1<<dir;
    if(*s==',') s++;
   }
   s=olds;
  }
  if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+otype->name+"\": bad end");
  s++;
  AddObjectType(otype);
 }
 delete mem;
 Sort(SortObjCompare);
#endif
 modified=false;
}
__fastcall AObjectTypes::~AObjectTypes()
{
 for(int i=Count-1;i>=0;i--){
  AObjectType*it=Get(i);
//  Delete(i);
  delete it;
 }
 TList::Clear();
 if(ObjectTypes==this)ObjectTypes=0;
}


AObjectType* AObjectTypes::Get(int index)
{
 return (AObjectType*)Items[index];
}
AObjectType* AObjectTypes::Get(AnsiString name)
{
 foreachr(this){
  AObjectType*ot=*(AObjectType**)iter;
  if(!CompareIStr(ot->name,name))return ot;
 }
 return 0;
}
void AObjectTypes::AddObjectType(AObjectType*otype)
{
 if(Get(otype->name))throw Exception(AnsiString("ObjectType \"")+otype->name+"\" already present");
 Add(otype);
 modified=true;
}
void AObjectTypes::Save()
{
 TFileStream *file=new TFileStream(GetGameFileName(ObjectTypeTxt),fmCreate);//fmOpenWrite);
 for(int i=0;i<Count;i++){
  AnsiString s=Get(i)->Print()+"\r\n";
  file->Write(s.c_str(),s.Length());
 }
 delete file;
 modified=false;
}
#ifndef RULESONLY
void ReadStringAsInt(TFile &in,AnsiString &str){
 int i;
 in>>i;
 char buf[5]={0,0,0,0,0};
 *(int*)buf=i;
 str=buf;
}
void WriteStringAsInt(TFile &out,AnsiString str){
 if(str.Length()>4)throw Exception("String \""+str+"\" to long");
 int i=*(int*)str.c_str();
 out<<i;
}
void ReadIntAsShort(TFile &in, int &i){
 short sh;
 in>>sh;
 i=sh;
}
void WriteIntAsShort(TFile &out, int i){
 if(i>MAXSHORT)throw Exception(AnsiString("Int number ")+i+" to big (short)");
 short sh=(short)i;
 out<<sh;
}
void ReadIntAsByte(TFile &in, int &i){
 unsigned char ch;
 in>>ch;
 i=ch;
}
void WriteIntAsByte(TFile &out, int i){
 if(i>255)throw Exception(AnsiString("Int number ")+i+" to big (byte)");
 unsigned char ch=(unsigned char)i;
 out<<ch;
}

const char*WeatherRegionTxt="weatherregion.txt";
AWeatherRegions *WeatherRegions;
AWeatherRegion::AWeatherRegion()
 :rect(-1,-1,-1,-1)
{
 for(int i=0;i<12;i++) weathers[i]="Normal";
}
AnsiString AWeatherRegion::Print()
{
 AnsiString s;
 s+="\""+name+"\"";
 s+=",";
 s+=level;
 s+=",";
 s+=rect.left;
 s+=",";
 s+=rect.top;
 s+=",";
 s+=rect.right;
 s+=",";
 s+=rect.bottom;
 foreach(WeatherTypes){
  AWeatherType *wt=*(AWeatherType**)iter;
  if(wt->name=="Normal") continue;
  bool first=true;
  for(int i=0;i<12;i++){
   if(weathers[i]==wt->name){
    if(first){
     first=false;
     s+=",\"";
     s+=wt->name;
     s+="\"";
    }
    s+=",";
    s+=i;
   }
  }
 }
 return s;
}
AWeatherRegions::AWeatherRegions()
{
 TMemoryStream *mem=LoadFile(GetGameFileName(WeatherRegionTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 AnsiString str;
 AWeatherRegion *wreg;
 while(*s=='\"'){
  wreg=new AWeatherRegion;
  wreg->name=GetNextToken(s);
  if(*s!=','){
   throw Exception(AnsiString("Bad format by Weather Region \"")+wreg->name+"\" level not found");
  }
  s++;
  str=GetNextToken(s);
  wreg->level=str.ToInt();
  if(*s!=','){
   throw Exception(AnsiString("Bad format by Weather Region \"")+wreg->name+"\" top not found");
  }
  s++;
  str=GetNextToken(s);
  wreg->rect.left=str.ToInt();
  if(*s!=','){
   throw Exception(AnsiString("Bad format by Weather Region \"")+wreg->name+"\" top not found");
  }
  s++;
  str=GetNextToken(s);
  wreg->rect.top=str.ToInt();
  if(*s!=','){
   throw Exception(AnsiString("Bad format by Weather Region \"")+wreg->name+"\" right not found");
  }
  s++;
  str=GetNextToken(s);
  wreg->rect.right=str.ToInt();
  if(*s!=','){
   throw Exception(AnsiString("Bad format by Weather Region \"")+wreg->name+"\" bottom not found");
  }
  s++;
  str=GetNextToken(s);
  wreg->rect.bottom=str.ToInt();
  
  while(*s==','&&s[1]=='\"'){
   s++;
   AnsiString wname=GetNextToken(s);
   {
    AWeatherType *wtype=WeatherTypes->Get(wname);
    if(!wtype){
     throw Exception(AnsiString("Bad format by Weather Region \"")+wreg->name+"\" unknown weather \""+wname+"\"");
    }
    wname=wtype->name;
   }
   while(*s==','&&s[1]!='\"'){
    s++;
    str=GetNextToken(s);
    int month=str.ToInt();
    if(month<0||month>11){
     throw Exception(AnsiString("Bad format by Weather Region \"")+wreg->name+"\" bad month number "+month);
    }
    wreg->weathers[month]=wname;
   }
  }
  if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+wreg->name+"\": bad end");
  s++;
  AddWeatherRegion(wreg);
 }
 delete mem;
}
__fastcall AWeatherRegions::~AWeatherRegions()
{
 for(int i=Count-1;i>=0;i--){
  AWeatherRegion *wr=(AWeatherRegion*)Items[i];
//  Delete(i);
  delete wr;
 }
 TList::Clear();
 if(WeatherRegions==this)WeatherRegions=0;
}
AWeatherRegion *AWeatherRegions::Get(int index)
{
 return (AWeatherRegion*)Items[index];
}
AWeatherRegion * AWeatherRegions::Get(AnsiString name)
{
 foreach(this){
  AWeatherRegion *wreg=*(AWeatherRegion**)iter;
  if(wreg->name==name)return wreg;
 }
 return 0;
}
void AWeatherRegions::AddWeatherRegion(AWeatherRegion * wreg)
{
 for(int i=Count-1;i>=0;i--){
  if(wreg->name==Get(i)->name) throw Exception(AnsiString("Weather Region \"")+wreg->name+"\" already present");
 }
 Add(wreg);
}


void AWeatherRegions::Save()
{
 TFileStream *file=new TFileStream(GetGameFileName(WeatherRegionTxt),fmCreate);//fmOpenWrite);
 for(int i=0;i<Count;i++){
  AnsiString s=Get(i)->Print()+"\r\n";
  file->Write(s.c_str(),s.Length());
 }
 delete file;
}
AnsiString AWeatherRegions::GetWeather(int x, int y, int z,int turnnum)
{
 if(turnnum==-1) turnnum=curTurnNumber;
 turnnum--;
 int monthnum=turnnum%12;
 if(monthnum<0) monthnum=11;
 foreach(this){
  AWeatherRegion *wreg=*(AWeatherRegion**)iter;
  if(wreg->level!=z) continue;
  if(wreg->rect.left>x||wreg->rect.right<x) continue;
  if(wreg->rect.top>y||wreg->rect.bottom<y) continue;
  return wreg->weathers[monthnum];
 }
 return "Normal";
}
int AWeatherRegions::GetSeasonLength(int x, int y, int z, int turnnum)
{
 if(turnnum==-1) turnnum=curTurnNumber+1;
 AWeatherRegion *wreg0=0;
 foreach(this){
  AWeatherRegion *wreg=*(AWeatherRegion**)iter;
  if(wreg->level!=z) continue;
  if(wreg->rect.left>x||wreg->rect.right<x) continue;
  if(wreg->rect.top>y||wreg->rect.bottom<y) continue;
  wreg0=wreg;
  break;
 }
 if(!wreg0)
   return 12;
 turnnum--;
 int monthnum=turnnum%12;
 if(monthnum<0) monthnum=11;
 AnsiString weat=wreg0->weathers[monthnum],weat2;
 int i=0;
 do{
   i++;
   monthnum++;
   if(monthnum>11) monthnum=0;
   weat2=wreg0->weathers[monthnum];
 }while(weat==weat2&&i<12);
 return i;
}
int DeleteFiles(AnsiString fname){
 SHFILEOPSTRUCT fos;
 char buf[1000];

 memset(&fos,0,sizeof(fos));
 memset(buf,0,sizeof(buf));
// fos.hwnd=0;
 fos.wFunc=FO_DELETE;
 strcpy(buf,fname.c_str());
 fos.pFrom=buf;
 fos.fFlags=FOF_SILENT|FOF_NOCONFIRMATION;
 return SHFileOperation(&fos);
}
int MoveFiles(AnsiString oldname,AnsiString newname){
 SHFILEOPSTRUCT fos;
 char buf1[1000],buf2[1000];

 memset(&fos,0,sizeof(fos));
 memset(buf1,0,sizeof(buf1));
 memset(buf2,0,sizeof(buf2));
// fos.hwnd=0;
 fos.wFunc=FO_MOVE;
 strcpy(buf1,oldname.c_str());
 strcpy(buf2,newname.c_str());
 fos.pFrom=buf1;
 fos.pTo=buf2;
 fos.fFlags=FOF_SILENT|FOF_NOCONFIRMATION;
 return SHFileOperation(&fos);
}
int CopyFiles(AnsiString oldname,AnsiString newname){
 SHFILEOPSTRUCT fos;
 char buf1[1000],buf2[1000];

 memset(&fos,0,sizeof(fos));
 memset(buf1,0,sizeof(buf1));
 memset(buf2,0,sizeof(buf2));
// fos.hwnd=0;
 fos.wFunc=FO_COPY;
 strcpy(buf1,oldname.c_str());
 strcpy(buf2,newname.c_str());
 fos.pFrom=buf1;
 fos.pTo=buf2;
 fos.fFlags=FOF_SILENT|FOF_NOCONFIRMATION;
 return SHFileOperation(&fos);
}
unsigned GetFileSize(AnsiString fname)
{
  TSearchRec sr;
  unsigned size=-1;
  if(FindFirst(fname, 0, sr) == 0)
  {
    size=sr.Size;
    FindClose(sr);
  }
  return size;
}
void SetBits(int &value,int mask,bool set){
 if(set) value|=mask;
 else value&=~mask;
}
#endif
//---------------------------------------------------------------------------

// Weapons
const char*WeaponsTxt="weapons.txt";
AWeaponTypes*WeaponTypes;
AWeaponType::AWeaponType()
 :flags(0),weapClass(SLASHING),attackType(ABattleEnums::ATTACK_COMBAT),
  numAttacks(0),attackBonus(0),defenseBonus(0),mountBonus(0)
{
}

AnsiString AWeaponType::Print(bool quotation)
{
  AnsiString s;
  if(quotation) s += "\"";
  s += item;
  if(quotation) s += "\"";
  if(skill1.Length()){
    s += ",";
    if(quotation) s += "\"";
    s += skill1;
    if(quotation) s += "\"";
    if(skill2.Length()){
      s += ",";
      if(quotation) s += "\"";
      s += skill2;
      if(quotation) s += "\"";
    }
  }
  if(flags&NEEDSKILL) s += ",NEEDSKILL";
  if(flags&ALWAYSREADY) s += ",ALWAYSREADY";
  if(flags&NODEFENSE) s += ",NODEFENSE";
  if(flags&NOFOOT) s += ",NOFOOT";
  if(flags&NOMOUNT) s += ",NOMOUNT";
  if(flags&SHORT) s += ",SHORT";
  if(flags&LONG) s += ",LONG";
  if(flags&RANGED) s += ",RANGED";
  if(flags&NOATTACKERSKILL) s += ",NOATTACKERSKILL";
  if(flags&RIDINGBONUS) s += ",RIDINGBONUS";
  if(flags&RIDINGBONUSDEFENSE) s += ",RIDINGBONUSDEFENSE";
  s += ",";
  s += numAttacks;
  s += ","+WeapClsToStr(weapClass);
  s += ","+AttTypeToStr(attackType);
  s += ",";
  s += attackBonus;
  s += ",";
  s += defenseBonus;
  s += ",";
  s += mountBonus;

  return s;
}

void AWeaponType::CreateNew(AWeaponType * prevw)
{
  item=prevw->item;
  skill1=prevw->skill1;
  skill2=prevw->skill2;
  flags=prevw->flags;
  weapClass=prevw->weapClass;
  attackType=prevw->attackType;
  numAttacks=prevw->numAttacks;
  attackBonus=prevw->attackBonus;
  defenseBonus=prevw->defenseBonus;
  mountBonus=prevw->mountBonus;
}

AWeaponTypes::AWeaponTypes()
{
#ifndef RULESONLY
  TMemoryStream *mem=LoadFile(GetGameFileName(WeaponsTxt));
  char*memory=(char*)mem->Memory,*s=memory;
  AnsiString str;
  AWeaponType*wtype;
  int filever=0;
  if(!MyCompare(s,verSign)){
    s+=verSign.Length();
    filever=atoi(s);
    GetNextString(s);
  }
  if(filever>2) throw Exception(AnsiString("Unsupported file version"));
  while(*s=='\"'){
    wtype = new AWeaponType;
    str = GetNextToken(s);
    AItemType *itype=ItemTypes->Get(str);
    if(!itype) throw Exception(AnsiString("Bad format by \"")+str+"\": is not item");
    if(!(itype->type&AItemType::IT_WEAPON)){
       AnsiString str1 = "Erron on loading data. \"" + str + "\": is not weapon\nWhat shall we do?";
       //str1 = AnsiString("Erron on loading weapons \"") + str + AnsiString("\""));
       switch(ReqForm->ShowFrm(str1, "Set 'weapon' flag", "Remove from weapon list")){
         case mrYes:
           delete wtype;
           while(*s != '\n') s++; s++;
           continue;
         case mrCancel: throw Exception(AnsiString("Bad format by \"")+str+"\": is not weapon");
         case mrOk: itype->type|= AItemType::IT_WEAPON;
       }
    }
    wtype->item=itype->abr;
    if(*s!=',')throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": comma not found");
    s++;
    if(*s=='\"'){
      str=GetNextToken(s);
      ASkillType *stype=SkillTypes->Get(str);
      if(!stype) throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": unknown skill "+wtype->skill1);
      wtype->skill1=stype->abr;
      if(*s!=',')throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": comma not found");
      s++;
      if(*s=='\"'){
        str=GetNextToken(s);
        stype=SkillTypes->Get(str);
        if(!stype) throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": unknown skill "+wtype->skill2);
        wtype->skill2=stype->abr;
        if(*s!=',')throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": comma not found");
        s++;
      }
    }

  if(filever==0){
   wtype->weapClass=AWeaponType::SLASHING;
   wtype->attackType=ABattleEnums::ATTACK_COMBAT;
   while(*s&&!(isdigit(*s)||*s=='-')){
    str=GetNextToken(s);
    if(str=="NEEDSKILL") wtype->flags|=AWeaponType::NEEDSKILL;
    else if(str=="NEEDMOUNT") wtype->flags|=AWeaponType::NOFOOT;
    else if(str=="RANGED"){
     wtype->flags|=AWeaponType::RANGED;
     wtype->weapClass=AWeaponType::PIERCING;
     wtype->attackType=ABattleEnums::ATTACK_RANGED;
     wtype->flags|=AWeaponType::NOATTACKERSKILL;
    }else if(str=="NODEFENSE") ;//wtype->flags|=AWeaponType::NODEFENSE;
    else if(str=="GOODARMOR") wtype->weapClass=AWeaponType::ARMORPIERCING;
    else if(str=="NEVERMISS") wtype->flags|=AWeaponType::ALWAYSREADY;
    else throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": unknown flag: "+str);
    if((wtype->flags&AWeaponType::NOATTACKERSKILL)==0)
      wtype->flags|=AWeaponType::RIDINGBONUS;
    if(*s!=',')throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": comma not found");
    s++;
   }
   str=GetNextToken(s);
   wtype->attackBonus=str.ToInt();
   if(wtype->attackType==ABattleEnums::ATTACK_COMBAT)
    wtype->defenseBonus=wtype->attackBonus;
   else
    wtype->defenseBonus=0;
   wtype->mountBonus=0;
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": comma not found");
   s++;
   str=GetNextToken(s);
   wtype->numAttacks=str.ToInt();
  }else{
   while(*s&&!(isdigit(*s)||*s=='-')){
    str=GetNextToken(s);
    if(str=="NEEDSKILL") wtype->flags|=AWeaponType::NEEDSKILL;
    else if(str=="ALWAYSREADY") wtype->flags|=AWeaponType::ALWAYSREADY;
    else if(str=="NODEFENSE") wtype->flags|=AWeaponType::NODEFENSE;
    else if(str=="NOFOOT") wtype->flags|=AWeaponType::NOFOOT;
    else if(str=="NOMOUNT") wtype->flags|=AWeaponType::NOMOUNT;
    else if(str=="SHORT") wtype->flags|=AWeaponType::SHORT;
    else if(str=="LONG") wtype->flags|=AWeaponType::LONG;
    else if(str=="RANGED") wtype->flags|=AWeaponType::RANGED;
    else if(str=="NOATTACKERSKILL") wtype->flags|=AWeaponType::NOATTACKERSKILL;
    else if(str=="RIDINGBONUS") wtype->flags|=AWeaponType::RIDINGBONUS;
    else if(str=="RIDINGBONUSDEFENSE") wtype->flags|=AWeaponType::RIDINGBONUSDEFENSE;
    else throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": unknown flag: "+str);
    if(*s!=',')throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": comma not found");
    s++;
   }
   if(filever<2&&(wtype->flags&AWeaponType::NOATTACKERSKILL)==0)
     wtype->flags|=AWeaponType::RIDINGBONUS;
   str=GetNextToken(s);
   wtype->numAttacks=str.ToInt();
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": comma not found");
   s++;
   str=GetNextToken(s);
   wtype->weapClass=StrToWeapCls(str);
   if(wtype->weapClass==-1)throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": unknown weapon class "+str);
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": comma not found");
   s++;
   str=GetNextToken(s);
   wtype->attackType=StrToAttType(str);
   if(wtype->attackType==-1)throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": unknown attack type "+str);
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": comma not found");
   s++;
   str=GetNextToken(s);
   wtype->attackBonus=str.ToInt();
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": comma not found");
   s++;
   str=GetNextToken(s);
   wtype->defenseBonus=str.ToInt();
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": comma not found");
   s++;
   str=GetNextToken(s);
   wtype->mountBonus=str.ToInt();
  }
  if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+wtype->item+"\": bad end");
  s++;
  AddWeaponType(wtype);
 }
 delete mem;
#endif
 modified=false;
}
__fastcall AWeaponTypes::~AWeaponTypes()
{
 for(int i=Count-1;i>=0;i--){
  AWeaponType*w=Get(i);
//  Delete(i);
  delete w;
 }
 TList::Clear();
 if(WeaponTypes==this)WeaponTypes=0;
}
AWeaponType* AWeaponTypes::Get(int index)
{
 return (AWeaponType*)Items[index];
}
AWeaponType* AWeaponTypes::Get(AnsiString name)
{
 if(name.Length()==0)return 0;
 for(int i=Count-1;i>=0;i--){
  AWeaponType*wt=Get(i);
  if(!CompareIStr(wt->item,name))return wt;
 }
 return 0;
}

void AWeaponTypes::AddWeaponType(AWeaponType* wtype)
{
  AWeaponType *wt;
  wt = Get(wtype->item);
  if(wt)throw Exception(AnsiString("Weapon \"")+wtype->item+"\" already present");
  Add(wtype);
  modified = true;
}

void AWeaponTypes::DelWeaponType(AnsiString abr)
{
  for(int i=Count-1;i>=0;i--)
  {
	AWeaponType*wt=Get(i);
	if(!CompareIStr(wt->item, abr))
	{
	  Delete(i);
	  break;
    }
  }
  modified = true;
}

void AWeaponTypes::Save()
{
  TFileStream *file=new TFileStream(GetGameFileName(WeaponsTxt),fmCreate);//fmOpenWrite);
  AnsiString s=verSign+"2\r\n";
  file->Write(s.c_str(),s.Length());
  for(int i=0;i<Count;i++)
  if(Get(i))
	{
		AnsiString s=Get(i)->Print(true)+"\r\n";
		file->Write(s.c_str(),s.Length());
    }
  delete file;
  modified = false;
}
//---------------------------------------------------------------------------

// Armors
const char*ArmorsTxt="armors.txt";
AArmorTypes*ArmorTypes;
AArmorType::AArmorType()
 :flags(0),from(100),armorclass(1)
{
 for(int i=0;i<AWeaponType::NUM_WEAPON_CLASSES;i++)
  saves[i]=0;
}
AnsiString AArmorType::Print(bool quotation)
{
 AnsiString s;
 if(quotation)s+="\"";
 s+=item;
 if(quotation)s+="\"";
 if(armorclass)
 {
   s+=",ARMORCLASS:";
   s+=armorclass;
 }
 if(flags&USEINASS)s+=",USEINASS";
 s+=",";
 s+=from;
 for(int i=0;i<AWeaponType::NUM_WEAPON_CLASSES;i++){
  s+=",";
  s+=saves[i];
 }
 return s;
}
void AArmorType::CreateNew(AArmorType * prevat)
{
 item=prevat->item;
 flags=prevat->flags;
 from=prevat->from;
 for(int i=0;i<AWeaponType::NUM_WEAPON_CLASSES;i++)
  saves[i]=prevat->saves[i];
}
AArmorTypes::AArmorTypes()
  :maxarmorclass(0)
{
#ifndef RULESONLY
 TMemoryStream *mem=LoadFile(GetGameFileName(ArmorsTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 AnsiString str;
 AArmorType*atype;
 int filever=0;
 if(!MyCompare(s,verSign)){
  s+=verSign.Length();
  filever=atoi(s);
  GetNextString(s);
 }
 if(filever>2)
	 throw Exception(AnsiString("Unsupported file version"));
 while(*s=='\"'){
  atype=new AArmorType;
  str=GetNextToken(s);
  AItemType *itype=ItemTypes->Get(str);
  if(!itype)
     throw Exception(AnsiString("Bad format by \"")+str+"\": is not item");
  if(!(itype->type&AItemType::IT_ARMOR))
     throw Exception(AnsiString("Bad format by \"")+str+"\": is not armor");
  atype->item=itype->abr;
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+atype->item+"\": comma not found");
  s++;
  while(!isdigit(*s)&&*s!='-'){
   str=GetNextToken(s);
   if(str=="USEINASS") atype->flags|=AArmorType::USEINASS;
   else if(str=="ARMORCLASS")
   {
     if(*s!=':')throw Exception(AnsiString("Bad format by \"")+atype->item+"\": : not found");
     s++;
     int num=atoi(GetNextToken(s).c_str());
     if(num<=0||num>=AArmorType::NUM_ARMOR_CLASSES)
       throw Exception(AnsiString("Bad format by \"")+atype->item+"\": bad ARMORCLASS value");
     if(num>maxarmorclass)
       maxarmorclass=num;
     atype->armorclass=num;
   }
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+atype->item+"\": comma not found");
   s++;
  }
  if(filever==0){
   str=GetNextToken(s);
   int normalChance=str.ToInt();
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+atype->item+"\": comma not found");
   s++;
   str=GetNextToken(s);
   int normalFrom=str.ToInt();
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+atype->item+"\": comma not found");
   s++;
   str=GetNextToken(s);
   int goodChance=str.ToInt();
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+atype->item+"\": comma not found");
   s++;
   str=GetNextToken(s);
   int goodFrom=str.ToInt();
   if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+atype->item+"\": bad end");
   s++;
   if(normalFrom!=goodFrom){
    goodChance*=normalFrom;
    normalChance*=goodFrom;
    normalFrom*=goodFrom;
   }
   atype->from=normalFrom;
   for(int i=0;i<AWeaponType::NUM_WEAPON_CLASSES;i++)
    atype->saves[i]=normalChance;
   for(int i=AWeaponType::ARMORPIERCING;i<=AWeaponType::MAGIC_WEATHER;i++)
    atype->saves[i]=goodChance;
  }else{
   str=GetNextToken(s);
   atype->from=str.ToInt();
   for(int i=0;i<AWeaponType::NUM_WEAPON_CLASSES;i++){
    if(filever<2&&i>=AWeaponType::MAGIC_ENERGY)
    {
      atype->saves[i]=atype->saves[AWeaponType::ARMORPIERCING];
      continue;
    }
    if(*s!=',')throw Exception(AnsiString("Bad format by \"")+atype->item+"\": comma not found");
    s++;
    str=GetNextToken(s);
    atype->saves[i]=str.ToInt();
   }
   if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+atype->item+"\": bad end");
   s++;
  }
  AddArmorType(atype);
 }
 delete mem;
#endif
 modified=false;
}
__fastcall AArmorTypes::~AArmorTypes()
{
 for(int i=Count-1;i>=0;i--){
  AArmorType*at=Get(i);
//  Delete(i);
  delete at;
 }
 TList::Clear();
 if(ArmorTypes==this)ArmorTypes=0;
}
AArmorType* AArmorTypes::Get(int index)
{
 return (AArmorType*)Items[index];
}

AArmorType* AArmorTypes::Get(AnsiString name)
{
 if(name.Length()==0)return 0;
 for(int i=Count-1;i>=0;i--){
  AArmorType*at=Get(i);
  if(!CompareIStr(at->item,name))return at;
 }
 return 0;
}

void AArmorTypes::DelArmorType(AnsiString name)
{
  for(int i=Count-1;i>=0;i--)
  {
	AArmorType*at = Get(i);
	if(!CompareIStr(at->item,name))
	{
		Delete(i);
		break;
    }
  }
}

void AArmorTypes::AddArmorType(AArmorType* atype)
{
 AArmorType *at;
 at=Get(atype->item);
 if(at)throw Exception(AnsiString("Armor \"")+atype->item+"\" already present");
 Add(atype);
 modified=true;
}
void AArmorTypes::Save()
{
 TFileStream *file=new TFileStream(GetGameFileName(ArmorsTxt),fmCreate);//fmOpenWrite);
 AnsiString s=verSign+"2\r\n";
 file->Write(s.c_str(),s.Length());
 for(int i=0;i<Count;i++){
  AnsiString s=Get(i)->Print(true)+"\r\n";
  file->Write(s.c_str(),s.Length());
 }
 delete file;
 modified=false;
}

const char*MountsTxt="mounts.txt";
AMountTypes*MountTypes;
AMountType::AMountType()
 :minBonus(0),maxBonus(0),maxHamperedBonus(0),specLevel(0),items(new TStringList)
{
 monster = false;
}
AMountType::~AMountType(){
 delete items;
}
AnsiString AMountType::Print(bool quotation)
{
 AnsiString s;
 if(quotation)s+="\"";
 s+=skill;
 if(quotation)s+="\"";
 s+=",";
 s+=minBonus;
 s+=",";
 s+=maxBonus;
 s+=",";
 s+=maxHamperedBonus;
 if(spec.Length()){
  s+=":";
  if(quotation)s+="\"";
  s+=spec;
  if(quotation)s+="\"";
  s+=",";
  s+=specLevel;
 }
 for(int i=0,endi=items->Count;i<endi;i++){
  s+=",";
  if(quotation)s+="\"";
  s+=items->Strings[i];
  if(quotation)s+="\"";
 }
 return s;
}
void AMountType::CreateNew(AMountType * prevmt)
{
 skill=prevmt->skill;
 minBonus=prevmt->minBonus;
 maxBonus=prevmt->maxBonus;
 items->Assign(prevmt->items);
}
AMountTypes::AMountTypes()
{
#ifndef RULESONLY
 TMemoryStream *mem=LoadFile(GetGameFileName(MountsTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 AnsiString str;
 AMountType*mtype;
 while(*s=='\"'){
  mtype=new AMountType;
  str=GetNextToken(s);
  ASkillType *stype=SkillTypes->Get(str);
  if(!stype)
     throw Exception(AnsiString("Bad format by \"")+str+"\": is not skill");
  mtype->skill=stype->abr;
  if(*s!=',')throw Exception(AnsiString("Bad format: comma not found"));
  s++;
  str=GetNextToken(s);
  mtype->minBonus=str.ToInt();
  if(*s!=',')throw Exception(AnsiString("Bad format: comma not found"));
  s++;
  str=GetNextToken(s);
  mtype->maxBonus=str.ToInt();
  mtype->maxHamperedBonus=3;
  if(*s==','&&s[1]!='\"'){
   s++;
   str=GetNextToken(s);
   mtype->maxHamperedBonus=str.ToInt();
  }
  if(*s==':'){
   s++;
   str=GetNextToken(s);
   mtype->spec=str;
   if(*s!=',')throw Exception(AnsiString("Bad format: comma not found"));
   s++;
   str=GetNextToken(s);
   mtype->specLevel=str.ToInt();
  }
  while(*s==','&&s[1]=='\"'){
   s++;
   str=GetNextToken(s);
   AItemType *itype=ItemTypes->Get(str);
   if(!itype)
      throw Exception(AnsiString("Bad format by \"")+str+"\": is not item");
   if(!(itype->type&AItemType::IT_MOUNT))
      throw Exception(AnsiString("Bad format by \"")+str+"\": is not mount");
   mtype->items->Add(itype->abr);
  }
  if(!mtype->items->Count)
   throw Exception("Bad format: no items");
  if(*s!='\n')throw Exception(AnsiString("Bad format: bad end"));
  s++;
  AddMountType(mtype);
 }
 delete mem;
#endif
 modified=false;
}
__fastcall AMountTypes::~AMountTypes()
{
 for(int i=Count-1;i>=0;i--){
  AMountType*at=Get(i);
//  Delete(i);
  delete at;
 }
 TList::Clear();
 if(MountTypes==this)MountTypes=0;
}
AMountType* AMountTypes::Get(int index)
{
 return (AMountType*)Items[index];
}
AMountType* AMountTypes::Get(AnsiString name)
{
 if(name.Length()==0)return 0;
 for(int i=Count-1;i>=0;i--){
  AMountType*mt=Get(i);
  for(int j=mt->items->Count-1;j>=0;j--){
   if(!CompareIStr(mt->items->Strings[j],name))return mt;
  }
 }
 return 0;
}

void AMountTypes::DelMountType(AnsiString name)
{
  for(int i=Count-1;i>=0;i--)
  {
	AMountType*mt = Get(i);
	for(int j=mt->items->Count-1;j>=0;j--)
	{
	  if(!CompareIStr(mt->items->Strings[j],name))
	  {
		  Delete(i);
		  return;
      }
	}
  }
}

void AMountTypes::AddMountType(AMountType* mtype)
{
 AMountType *at;
/* for(int i=mtype->items->Count-1;
 at=Get(atype->item);
 if(at)throw Exception(AnsiString("Mount \"")+atype->item+"\" already present");*/
 Add(mtype);
 modified=true;
}
void AMountTypes::Save()
{
 TFileStream *file=new TFileStream(GetGameFileName(MountsTxt),fmCreate);//fmOpenWrite);
 for(int i=0;i<Count;i++){
  AnsiString s=Get(i)->Print(true)+"\r\n";
  file->Write(s.c_str(),s.Length());
 }
 delete file;
 modified=false;
}
#ifndef RULESONLY
const char*SpellsTxt="spells.txt";
ASpellTypes*SpellTypes;
ASpellType::ASpellType()
 :spells(new TStringList)
{
}
ASpellType::~ASpellType(){
 delete spells;
}
AnsiString ASpellType::Print(bool quotation)
{
  AnsiString s;
  if(quotation) s += "\"";
  s += name;
  if(quotation)s += "\"";

  for(int i=0,endi=spells->Count;i<endi;i++)
  {
	s += ",";
	s += spells->Strings[i];
  }
  return s;
}
void ASpellType::CreateNew(ASpellType *prevst)
{
 name=prevst->name;
 spells->Assign(prevst->spells);
}
int ASpellType::GetTypeCount(int type){
 int count=0;
 for(int i=0,endi=spells->Count;i<endi;i++){
  int tt=(int)spells->Objects[i];
  if(tt==type) count++;
 }
 return count;
}
AnsiString ASpellType::GetTypeStr(int type,int ind){
 for(int i=0,endi=spells->Count;i<endi;i++){
  int tt=(int)spells->Objects[i];
  if(tt!=type) continue;
  if(ind--!=0) continue;
  return spells->Strings[i];
 }
 return "";
}
AnsiString ASpellType::FullName(AnsiString abr){
 ASkillType *stype=SkillTypes->Get(abr);
 if(!stype) return abr;
 return stype->name;
}
static char *SpellTypeStr[ASpellType::SPELLSNUM]={
 "UNKNOWN",
 "ATTACK",
 "SHIELD",
 "DEFENCE",
 "CHECKGOODTARGET",
 "CHECKBADTARGET",
 "CASTCREATE",
 "CASTNOP",
};
int ASpellType::GetSpellType(AnsiString str){
 for(int i=0;i<SPELLSNUM;i++){
  if(!CompareIStr(str,SpellTypeStr[i])) return i;
 }
 return 0;
}
AnsiString ASpellType::GetSpellTypeName(int type){
 if(unsigned(type)>=SPELLSNUM)
  type=0;
 return SpellTypeStr[type];
}

bool ASpellType::ParseSpell(char *&buf, int &type){
 AnsiString typestr=GetNextToken(buf);
 type=GetSpellType(typestr);
 if(type==CASTNOP)
  return true;
 if(*buf!='(')
  return false;
 buf++;
 if(type==0)
  return false;
 AnsiString str1,str2;
 int int1;
 switch(type){
  case ATTACK:
  case SHIELD:
  case DEFENCE:
   if(!ParseAttack(buf,str1,int1))
    return false;
   if(type==DEFENCE||type==ATTACK)
    return true; 
   if(int1==ABattleEnums::NUM_ATTACK_TYPES)
    return false;
   return true;
  case CHECKGOODTARGET:
  case CHECKBADTARGET:
   if(!ParseCheckTarget(buf,0))
    return false;
   return true;
  case CASTCREATE:
   return ParseCastCreate(buf,0,str1,str2);
 }
 return false;
}
bool ASpellType::ParseAttack(char *&buf,AnsiString&expr,int&attack_type){
 if(*buf!='\"')
  return false;
 expr=GetNextToken(buf);
 if(*buf!=',')
  return false;
 buf++;
 AnsiString att_type=GetNextToken(buf);
 if(*buf!=')')
  return false;
 buf++;
 attack_type=StrToAttType(att_type);
 if(attack_type<0)
  return false;
 return true;
}
bool ASpellType::ParseCheckTarget(char *&buf,TStringList*targets){
 AnsiString str;
 while(1){
  if(*buf!='\"')
   return false;
  str=GetNextToken(buf);
  AItemType *itype=ItemTypes->Get(str,4);
  if(!itype){
   if(str!="ILLU")
	return false;
  }
  if(targets)
   targets->Add(str);
  if(*buf==')') break;
  if(*buf!=',')
   return false;
  buf++;
 }
 buf++;
 return true;
}

bool ASpellType::ParseCastCreate(char *&buf,TStringList*params,AnsiString &maxnum,AnsiString&param){
 AnsiString str;
 int count=0;
 maxnum.SetLength(0);
 param.SetLength(0);
 while(1){
  if(*buf!='\"'){
   if(!MyCompare(buf,"MAXNUM:")){
    buf+=sizeof("MAXNUM:")-1;
    if(*buf!='\"')
	 return false;
	maxnum=GetNextToken(buf);
   }else if(!MyCompare(buf,"PARAM:")){
	buf+=sizeof("PARAM:")-1;
	if(*buf!='\"')
	 return false;
	param=GetNextToken(buf);
   }else
	return false;
  }else{
   str=GetNextToken(buf);
   if((count&1)==0&&str.Length()){
	AItemType *itype=ItemTypes->Get(str,4);
	if(!itype){
	 return false;
	}
   }
   if(params)
	params->Add(str);
   count++;
  }
  if(*buf==')') break;
  if(*buf!=',')
   return false;
  buf++;
 }
 buf++;
 if(count<2)
  return false;
 if((count&1))
  return false;
 return true;
}

bool ASpellType::GetAttackParam(int ind,AnsiString&expr,int&attack_type){
  AnsiString str=GetTypeStr(ATTACK,ind);
  if(!str.Length()) return false;
  char *buf=str.c_str();
  GetNextToken(buf);
  if(*buf!='(') return false;
  buf++;
  return ParseAttack(buf,expr,attack_type);
}

bool ASpellType::GetShieldParam(int ind,AnsiString&expr,int&attack_type){
  AnsiString str=GetTypeStr(SHIELD,ind);
  if(!str.Length()) return false;
  char *buf=str.c_str();
  GetNextToken(buf);
  if(*buf!='(') return false;
  buf++;
  return ParseAttack(buf,expr,attack_type);
}

bool ASpellType::GetDefenceParam(int ind,AnsiString&expr,int&attack_type){
 AnsiString str=GetTypeStr(DEFENCE,ind);
 if(!str.Length()) return false;
 char *buf=str.c_str();
 GetNextToken(buf);
 if(*buf!='(') return false;
 buf++;
 return ParseAttack(buf,expr,attack_type);
}
bool ASpellType::GetCastCreateParam(int ind,TStringList*params,AnsiString &maxnum,AnsiString&param){
 AnsiString str=GetTypeStr(CASTCREATE,ind);
 if(!str.Length()) return false;
 char *buf=str.c_str();
 GetNextToken(buf);
 if(*buf!='(') return false;
 buf++;
 return ParseCastCreate(buf,params,maxnum,param);
}

ASpellTypes::ASpellTypes()
{
  TMemoryStream *mem = LoadFile(GetGameFileName(SpellsTxt));
  char*memory = (char*)mem->Memory,*s=memory;
  ASpellType *stype;
  while(*s=='\"')
  {
	stype = new ASpellType;
	stype->name = GetNextToken(s);
	while(*s==',')
	{
	  s++;
	  char *beg = s;
	  int type = -1;
	  if(!ASpellType::ParseSpell(s,type))
		throw Exception(AnsiString("Bad format by \"")+stype->name+"\": can't parse spell"+ASpellType::GetSpellTypeName(type));
	  AnsiString str(beg,s-beg);
	  if(type==ASpellType::CHECKGOODTARGET||type==ASpellType::CHECKBADTARGET)
	  {
		if(stype->GetCheckCount())
		  throw Exception(AnsiString("Bad format by \"")+stype->name+"\": only one CHECK allowed");
	  }
	  stype->spells->AddObject(str,(TObject*)type);
	}

	if(*s!='\n') throw Exception(AnsiString("Bad format: bad end"));
	s++;
	AddSpellType(stype);
  }
  delete mem;
  modified = false;
}

__fastcall ASpellTypes::~ASpellTypes()
{
  for(int i=Count-1;i>=0;i--)
  {
	ASpellType*at = Get(i);
	delete at;
  }
  TList::Clear();
  if(SpellTypes == this) SpellTypes = 0;
}

ASpellType* ASpellTypes::Get(int index)
{
  return (ASpellType*)Items[index];
}
ASpellType* ASpellTypes::Get(AnsiString name)
{
  if(name.Length() == 0) return 0;
  for(int i=Count-1;i>=0;i--)
  {
	ASpellType*st = Get(i);
	if(!CompareIStr(st->name,name)) return st;
  }
  return 0;
}

void ASpellTypes::AddSpellType(ASpellType* stype)
{
  ASpellType *st=Get(stype->name);

  if(st)throw Exception(AnsiString("Spell \"")+stype->name+"\" already present");
  Add(stype);
  modified = true;
}

void ASpellTypes::Save()
{
  TFileStream *file = new TFileStream(GetGameFileName(SpellsTxt),fmCreate);//fmOpenWrite);
  for(int i=0;i<Count;i++)
  {
	AnsiString s = Get(i)->Print(true)+"\r\n";
	file->Write(s.c_str(),s.Length());
  }
  delete file;
  modified=false;
}
#endif

const char*MonstrsTxt="monstrs.txt";
AMonstrTypes*MonstrTypes;
AMonstrType::AMonstrType()
 :attackLevel(0),numAttacks(0),hits(0),tactics(0),specLevel(0),flags(0),
 regen(0),stea(0),obse(0)
{
 for(int i=0;i<ABattleEnums::NUM_ATTACK_TYPES;i++)
  defense[i]=0;
}
AMonstrType::~AMonstrType(){
}
AnsiString AMonstrType::Print(bool quotation)
{
 AnsiString s;
 if(quotation)s+="\"";
 s+=item;
 if(quotation)s+="\"";
 s+=",";
 s+=attackLevel;
 s+=",";
 s+=hits;
 s+=",";
 s+=tactics;
 s+=",";
 if(quotation)s+="\"";
 s+=name;
 if(quotation)s+="\"";
 if(spec.Length()){
  s+=",";
  if(quotation)s+="\"";
  s+=spec;
  if(quotation)s+="\"";
  s+=",";
  s+=specLevel;
 }
 for(int i=0;i<ABattleEnums::NUM_ATTACK_TYPES;i++){
  s+=",";
  s+=defense[i];
 }
 s+=",";
 s+=numAttacks;
 if(flags&BEHIND)
 {
   s+=",BEHIND";
 }
 return s;
}
void AMonstrType::CreateNew(AMonstrType *prevmn)
{
 item=prevmn->item;
 attackLevel=prevmn->attackLevel;
 for(int i=0;i<ABattleEnums::NUM_ATTACK_TYPES;i++)
  defense[i]=prevmn->defense[i];
 numAttacks=prevmn->numAttacks;
 hits=prevmn->hits;
 tactics=prevmn->tactics;
 name=prevmn->name;
 spec=prevmn->spec;
 specLevel=prevmn->specLevel;
 flags=prevmn->flags;
}
AMonstrTypes::AMonstrTypes()
{
#ifndef RULESONLY
 TMemoryStream *mem=LoadFile(GetGameFileName(MonstrsTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 AMonstrType*mtype;
 while(*s=='\"'){
  mtype=new AMonstrType;
  AnsiString str=GetNextToken(s);
  if(str!="ILLU"){
   AItemType *itype=ItemTypes->Get(str,4);
   if(!itype)
    throw Exception(AnsiString("Bad format by \"")+str+"\": is not item");
   if(!(itype->type&&AItemType::IT_MONSTER))
    throw Exception(AnsiString("Bad format by \"")+str+"\": is not monster");
   str=itype->abr;
  }
  mtype->item=str;
  if(*s!=',')
    throw Exception(AnsiString("Bad format by \"")+mtype->item+"\": comma not found");
  s++;
  mtype->attackLevel=GetNextToken(s).ToInt();
  if(*s!=',')
    throw Exception(AnsiString("Bad format by \"")+mtype->item+"\": comma not found");
  s++;
  mtype->hits=GetNextToken(s).ToInt();
  if(*s!=',')
    throw Exception(AnsiString("Bad format by \"")+mtype->item+"\": comma not found");
  s++;
  mtype->tactics=GetNextToken(s).ToInt();
  if(*s!=',')
    throw Exception(AnsiString("Bad format by \"")+mtype->item+"\": comma not found");
  s++;
  if(*s!='\"')
   throw Exception(AnsiString("Bad format by \"")+mtype->item+"\": name not found");
  mtype->name=GetNextToken(s);
  if(*s==','&&s[1]=='\"'){
   s++;
   mtype->spec=GetNextToken(s);
   if(*s==','){
    s++;
    mtype->specLevel=GetNextToken(s).ToInt();
   }else{
    mtype->specLevel=mtype->attackLevel;
   }
  }
  if(*s==','){
   for(int i=0;i<ABattleEnums::NUM_ATTACK_TYPES;i++){
    if(*s!=',')throw Exception(AnsiString("Bad format by \"")+mtype->item+"\": comma not found");
    s++;
    str=GetNextToken(s);
    mtype->defense[i]=str.ToInt();
   }
   if(*s!=',')throw Exception(AnsiString("Bad format by \"")+mtype->item+"\": comma not found");
   s++;
   str=GetNextToken(s);
   mtype->numAttacks=str.ToInt();
  }else{//oldtype
   for(int i=0;i<ABattleEnums::NUM_ATTACK_TYPES;i++)
    mtype->defense[i]=mtype->attackLevel;
   mtype->defense[ABattleEnums::ATTACK_RANGED]=0;
   mtype->numAttacks=mtype->hits;
  }
  if(*s==',')
  {
   s++;
   while(*s&&!(*s=='\n'||isdigit(*s)||*s=='-')){
    str=GetNextToken(s);
    if(str=="BEHIND") mtype->flags|=AMonstrType::BEHIND;
    else throw Exception(AnsiString("Bad format by \"")+mtype->item+"\": unknown flag: "+str);
   }
  } 
  if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+mtype->item+"\": bad end");
  s++;
  AddMonstrType(mtype);
 }
 delete mem;
#endif
 modified=false;
}
__fastcall AMonstrTypes::~AMonstrTypes()
{
 for(int i=Count-1;i>=0;i--){
  AMonstrType*mt=Get(i);
  delete mt;
 }
 TList::Clear();
 if(MonstrTypes==this)MonstrTypes=0;
}
AMonstrType* AMonstrTypes::Get(int index)
{
 return (AMonstrType*)Items[index];
}

AMonstrType* AMonstrTypes::Get(AnsiString name)
{
 if(name.Length()==0)return 0;
 if(ItemTypes->IsIllusionAbr(name)){
  name="ILLU";
 }
 for(int i=Count-1;i>=0;i--){
  AMonstrType*mt=Get(i);
  if(!CompareIStr(mt->item,name))return mt;
 }
 return 0;
}

void AMonstrTypes::DelMonstrType(AnsiString name)
{
  for(int i=Count-1;i>=0;i--)
  {
	AMonstrType*mt = Get(i);
	if(!CompareIStr(mt->item,name))
	{
		Delete(i);
		break;
    }
  }
}

void AMonstrTypes::AddMonstrType(AMonstrType* mtype)
{
 AMonstrType *mt=Get(mtype->item);
 if(mt)throw Exception(AnsiString("Monster \"")+mtype->item+"\" already present");
 Add(mtype);
 modified=true;
}
void AMonstrTypes::Save()
{
 TFileStream *file=new TFileStream(GetGameFileName(MonstrsTxt),fmCreate);//fmOpenWrite);
 for(int i=0;i<Count;i++){
  AnsiString s=Get(i)->Print(true)+"\r\n";
  file->Write(s.c_str(),s.Length());
 }
 delete file;
 modified=false;
}

const char*BatItemsTxt="batitems.txt";
ABatItemTypes*BatItemTypes;
ABatItemType::ABatItemType()
 :flags(0),shtype(-1),level(0)
{
}
ABatItemType::~ABatItemType(){
}
AnsiString ABatItemType::Print(bool quotation)
{
 AnsiString s;
 if(quotation)s+="\"";
 s+=item;
 if(quotation)s+="\"";
 if(flags&MAGEONLY)
  s+=",MAGEONLY";
 if(flags&SPECIAL){
  s+=",SPECIAL:";
  if(quotation)s+="\"";
  s+=spec;
  if(quotation)s+="\"";
 }else if(flags&SHIELD){
  s+=",SHIELD:";
  s+=AttTypeToStr(shtype);
 }
 s+=",";
 s+=level;
 return s;
}
void ABatItemType::CreateNew(ABatItemType *prevbt)
{
 item=prevbt->item;
 flags=prevbt->flags;
 shtype=prevbt->shtype;
 spec=prevbt->spec;
 level=prevbt->level;
}
ABatItemTypes::ABatItemTypes()
{
#ifndef RULESONLY
 TMemoryStream *mem=LoadFile(GetGameFileName(BatItemsTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 ABatItemType*bitype;
 while(*s=='\"'){
  bitype=new ABatItemType;
  AnsiString str=GetNextToken(s);
  AItemType *itype=ItemTypes->Get(str,4);
  if(!itype)
   throw Exception(AnsiString("Bad format by \"")+str+"\": is not item");
  if(!(itype->type&&AItemType::IT_BATTLE))
   throw Exception(AnsiString("Bad format by \"")+str+"\": is not battle item");
  bitype->item=itype->abr;
  if(!MyCompare(s,",MAGEONLY")){
   s+=sizeof(",MAGEONLY")-1;
   bitype->flags|=ABatItemType::MAGEONLY;
  }
  if(!MyCompare(s,",SPECIAL:")){
   s+=sizeof(",SPECIAL:")-1;
   bitype->flags|=ABatItemType::SPECIAL;
   if(*s!='\"')
    throw Exception(AnsiString("Bad format by \"")+bitype->item+"\": special not found");
   bitype->spec=GetNextToken(s);
  }else if(!MyCompare(s,",SHIELD:")){
   s+=sizeof(",SHIELD:")-1;
   bitype->flags|=ABatItemType::SHIELD;
   bitype->shtype=StrToAttType(GetNextToken(s));
   if(bitype->shtype<0)
    throw Exception(AnsiString("Bad format by \"")+bitype->item+"\": bad shield type");
  }
  if(*s!=',')throw Exception(AnsiString("Bad format by \"")+bitype->item+"\": comma not found");
  s++;
  bitype->level=GetNextToken(s).ToInt();
  if(*s!='\n')throw Exception(AnsiString("Bad format by \"")+bitype->item+"\": bad end");
  s++;
  AddBatItemType(bitype);
 }
 delete mem;
#endif
 modified=false;
}
__fastcall ABatItemTypes::~ABatItemTypes()
{
 for(int i=Count-1;i>=0;i--){
  ABatItemType*bit=Get(i);
  delete bit;
 }
 TList::Clear();
 if(BatItemTypes==this)BatItemTypes=0;
}
ABatItemType* ABatItemTypes::Get(int index)
{
 return (ABatItemType*)Items[index];
}
ABatItemType* ABatItemTypes::Get(AnsiString name)
{
 if(name.Length()==0)return 0;
 for(int i=Count-1;i>=0;i--){
  ABatItemType*bit=Get(i);
  if(!CompareIStr(bit->item,name))return bit;
 }
 return 0;
}
void ABatItemTypes::AddBatItemType(ABatItemType* bitype)
{
 ABatItemType *bit=Get(bitype->item);
 if(bit)throw Exception(AnsiString("BatItem \"")+bitype->item+"\" already present");
 Add(bitype);
 modified=true;
}
void ABatItemTypes::Save()
{
 TFileStream *file=new TFileStream(GetGameFileName(BatItemsTxt),fmCreate);//fmOpenWrite);
 for(int i=0;i<Count;i++){
  AnsiString s=Get(i)->Print(true)+"\r\n";
  file->Write(s.c_str(),s.Length());
 }
 delete file;
 modified=false;
}

const char*MansTxt="mans.txt";
AManTypes*ManTypes;
AManType::AManType()
 :deflevel(0),skills(new TStringList)
{
}
AManType::~AManType(){
 delete skills;
}
AnsiString AManType::Print(bool quotation)
{
 AnsiString s;
 if(quotation)s+="\"";
 s+=abr;
 if(quotation)s+="\"";
 s+=",";
 s+=deflevel;
 for(int i=0,endi=skills->Count;i<endi;i++){
  s+=",";
  if(quotation)s+="\"";
  s+=skills->Strings[i];
  if(quotation)s+="\"";
  s+=",";
  s+=(int)skills->Objects[i];
 }
 return s;
}
int AManType::GetLevel(AnsiString skill){
 int ind=GetLevelIndex(skill);
 if(ind<0){
  return deflevel;
 }else{
  return (int)skills->Objects[ind];
 }
}
int AManType::GetLevelIndex(AnsiString skill){
 return skills->IndexOf(skill);
}
void AManType::SetLevel(AnsiString skill,int level){
 int ind=GetLevelIndex(skill);
 if(ind<0){
  skills->AddObject(skill,(TObject*)level);
 }else{
  skills->Objects[ind]=(TObject*)level;
 }
}
AManTypes::AManTypes()
  :leadertype(0)
{
#ifndef RULESONLY
 TMemoryStream *mem=LoadFile(GetGameFileName(MansTxt));
 char*memory=(char*)mem->Memory,*s=memory;
 AnsiString str;
 AManType*mtype;
 while(*s=='\"'){
  mtype=new AManType;
  str=GetNextToken(s);
  AItemType *itype=ItemTypes->Get(str);
  if(!itype)
     throw Exception(AnsiString("Bad format by \"")+str+"\": is not item");
  if(!(itype->type&&AItemType::IT_MAN))
   throw Exception(AnsiString("Bad format by \"")+str+"\": is not man");
  mtype->abr=itype->abr;
  if(*s!=',')throw Exception(AnsiString("Bad format: comma not found"));
  s++;
  str=GetNextToken(s);
  mtype->deflevel=str.ToInt();
  while(*s==','&&s[1]=='\"'){
   s++;
   str=GetNextToken(s);
   ASkillType *stype=SkillTypes->Get(str);
   if(!stype)
      throw Exception(AnsiString("Bad format by \"")+str+"\": is not skill");
   if(*s!=',')throw Exception(AnsiString("Bad format: comma not found"));
   s++;
   str=GetNextToken(s);
   int level=str.ToInt();
   if(level<0)
    throw Exception(AnsiString("Bad format by \"")+itype->abr+":"+str+"\": negative level");
   mtype->SetLevel(stype->abr,level);
  }
  if(*s!='\n')throw Exception(AnsiString("Bad format: bad end"));
  s++;
  AddManType(mtype);
  if(leadertype) continue;
  if(itype->type&AItemType::IT_LEADER)
    leadertype=mtype;
 }
 delete mem;
#endif
 modified=false;
}
__fastcall AManTypes::~AManTypes()
{
 for(int i=Count-1;i>=0;i--){
  AManType*at=Get(i);
//  Delete(i);
  delete at;
 }
 TList::Clear();
 if(ManTypes==this)ManTypes=0;
}
AManType* AManTypes::Get(int index)
{
 return (AManType*)Items[index];
}
AManType* AManTypes::Get(AnsiString name)
{
 if(name.Length()==0)return 0;
 for(int i=Count-1;i>=0;i--){
  AManType *mtype=Get(i);
  if(mtype->abr==name)
   return mtype;
 }
 return 0;
}
void AManTypes::DelManType(AnsiString name)
{
  for(int i=Count-1;i>=0;i--)
  {
	AManType *mtype = Get(i);
	if(mtype->abr == name)
	{
	  Delete(i);
	  break;
	}
 }
}

void AManTypes::AddManType(AManType* mtype)
{
 AManType *at;
 if(Get(mtype->abr))
  throw Exception(AnsiString("Man \"")+mtype->abr+"\" already present");
 Add(mtype);
 modified=true;
}
int AManTypes::GetLevel(AnsiString race,AnsiString skill){
 AManType*mtype=Get(race);
 if(!mtype)
  mtype=leadertype;
 if(!mtype)
  return 5;
 return mtype->GetLevel(skill);
}
void AManTypes::Save()
{
 TFileStream *file=new TFileStream(GetGameFileName(MansTxt),fmCreate);//fmOpenWrite);
 for(int i=0;i<Count;i++){
  AnsiString s=Get(i)->Print(true)+"\r\n";
  file->Write(s.c_str(),s.Length());
 }
 delete file;
 modified=false;
}

#ifndef RULESONLY
#include "i_rand.h"
static randctx isaac_ctx;
int getrandom(int range)
{
    if (!range) return 0;
    unsigned long i = isaac_rand( &isaac_ctx );
    i = i % range;
    return (int) i;
}
void seedrandom(int num){
    ub4 i;
    isaac_ctx.randa = isaac_ctx.randb = isaac_ctx.randc = (ub4)0;
    for (i=0; i<256; ++i)
    {
        isaac_ctx.randrsl[i]=(ub4)num+i;
    }
    randinit( &isaac_ctx, TRUE );
}
#endif
static AnsiString atttypestr[ABattleEnums::NUM_ATTACK_TYPES+1]={
 "COMBAT",
 "ENERGY",
 "SPIRIT",
 "WEATHER",
 "RIDING",
 "RANGED",

 "ALLATTACK",
};
AnsiString AttTypeToStr(int atttype){
 if(atttype>ABattleEnums::NUM_ATTACK_TYPES) return "";
 return atttypestr[atttype];
}
int StrToAttType(AnsiString str){
 for(int i=0;i<=ABattleEnums::NUM_ATTACK_TYPES;i++){
  if(!CompareIStr(str,atttypestr[i])) return i;
 }
 return -1;
}
static AnsiString weapclsstr[AWeaponType::NUM_WEAPON_CLASSES]={
 "SLASHING",
 "PIERCING",
 "CRUSHING",
 "CLEAVING",
 "ARMORPIERCING",
 "ENERGY",
 "SPIRIT",
 "WEATHER",
};
AnsiString WeapClsToStr(int weapcls){
 if(weapcls>=AWeaponType::NUM_WEAPON_CLASSES) return "";
 return weapclsstr[weapcls];
}
int StrToWeapCls(AnsiString str){
 for(int i=0;i<AWeaponType::NUM_WEAPON_CLASSES;i++){
  if(!CompareIStr(str,weapclsstr[i])) return i;
 }
 return -1;
}

AnsiString StrForURL(AnsiString str){
 AnsiString s;
 char buf[4];
 for(int i=1;i<=str.Length();i++){
  unsigned char c=str[i];
  if(c>='0'&&c<128){
   s+=(char)c;
   continue;
  }
  sprintf(buf,"%02x",c);
//  itoa(c,buf,16);
  s+="%";
  s+=buf;
 }
 return s;
}
#ifndef RULESONLY
bool InputMemo(AnsiString Caption, AnsiString &Value){
 TForm *frm=new TForm(AtlaForm);
 frm->Caption=Caption;
 frm->Position=poDesktopCenter;
 frm->BorderStyle=bsDialog;
 TPanel *pan=new TPanel(frm);
 pan->Height=35;
 pan->Align=alBottom;
 pan->BevelOuter=bvNone;
 pan->Parent=frm;
 TButton *butt=new TButton(frm);
 butt->Left=10;
 butt->Top=5;
 butt->Caption="OK";
 butt->ModalResult=mrOk;
 butt->Parent=pan;
 TButton *but2=new TButton(frm);
 but2->Left=100;
 but2->Top=5;
 but2->Caption="Cancel";
 but2->ModalResult=mrCancel;
 but2->Parent=pan;

 TMemo *mem=new TMemo(frm);
 mem->Align=alClient;
 mem->Parent=frm;
 mem->Lines->Text=Value;
 mem->TabOrder=0;
 
 int res=frm->ShowModal();
 if(res==mrOk){
  Value=mem->Lines->Text;
 }
 delete frm;

 return res==mrOk;
}
bool inFacDiap(int facdiap, AFaction *fac, AFaction *basefac)
{
  if(!basefac)
    basefac=curFaction;
  if(facdiap==-2) return true;
  if(facdiap==-3) return fac->local;
  if(facdiap==-4) return !fac->local;
  if(basefac)
  {
    if(facdiap==-5) return basefac->GetAttitude(fac->number)>=A_FRIENDLY;
    if(facdiap==-6) return basefac->GetAttitude(fac->number)<=A_UNFRIENDLY;
    if(facdiap==-7) return basefac->GetAttitude(fac->number)<=A_NEUTRAL;
  }
  return facdiap==fac->number;
}
int AnsiIPos(AnsiString str,AnsiString substr) //without case sensitivity
{
  int len1=str.Length();
  int len2=substr.Length();
  if(!len2)
    return 0;
  len1-=len2-1;
  for(int i=0;i<len1;i++)
  {
    if(!AnsiStrLIComp(str.c_str()+i,substr.c_str(),len2))
      return i+1;
  }
  return 0;
}

//---------------------------------------------------------------------------
// Specials & Abilities
/*
static SpecialType spd[] = {
	// NONE
	{"an unknown attack",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 0,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "casts something that is not a spell", "annoying ", "", "unknown"},
	// SPECIAL_FIREBALL
	{"a fireball",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_ENERGY,2,5,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_ENERGY,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "shoots a Fireball", "killing ", "", "fireball"},
	// SPECIAL_ICECONE
	{"an ice cone",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_WEATHER,2,5,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_WEATHER,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "shoots an ice cone", "killing ", "", "ice"},
	// SPECIAL_MINDSTRIKE
	{"a mind strike",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_SPIRIT,2,5,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_SPIRIT,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "strikes the minds of the enemy", "killing ", "", "mind strike"},
	// SPECIAL_HELLFIRE
	{"hellfire",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_ENERGY,2,25,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_ENERGY,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "blasts the enemy with Hellfire", "killing ", "", "hellfire"},
	// SPECIAL_CAUSEFEAR
	{"cause fear",
	 SpecialType::HIT_EFFECTEXCEPT|SpecialType::HIT_NOMONSTER,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {EFFECT_FEAR, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_SPIRIT,2,10,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_SPIRIT,EFFECT_FEAR},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "strikes fear into the enemy", "causing ", " men to cower in terror", "fear"},
	// SPECIAL_LSTRIKE
	{"a lightning strike",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_WEATHER,2,30,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_WEATHER,0},
	  {ABattleEnums::ATTACK_ENERGY,2,30,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_ENERGY,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "unleashes a mighty lightning strike", "killing ", "", "lightning"},
	// SPECIAL_MINDBLAST
	{"a mindblast",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_SPIRIT,2,125,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_SPIRIT,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "blasts the minds of the enemy", "killing ", " troops", "mindblast"},
	// SPECIAL_EARTHQUAKE
	{"an earthquake",
	 SpecialType::HIT_BUILDINGEXCEPT,
	 {O_MAGICALFORTRESS, O_MAGICALTOWER, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV|SpecialType::FX_NOBUILDING,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_COMBAT,2,50,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::ARMORPIERCING,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "invokes a mighty Earthquake", "killing ", "", "earthquake"},
	// SPECIAL_TRAMPLING
	{"his body",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_RIDING,2,1,
	   AWeaponType::ALWAYSREADY,0,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "tramples the enemy", "killing ", "", "trampling"},
	// SPECIAL_FORCE_SHIELD
	{"a force shield",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_SHIELD|SpecialType::FX_USE_LEV|SpecialType::FX_DEFBONUS,
	 {ABattleEnums::ATTACK_RANGED, -1, -1, -1},
	 {{ABattleEnums::ATTACK_COMBAT, 1},{-1, 0},{-1, 0},{-1, 0}},
	 "Force Shield",
	 {{-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "casts something that is not a spell", "annoying ", "", "unknown"},
	// SPECIAL_ENERGY_SHIELD
	{"an energy shield",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_SHIELD,
	 {ABattleEnums::ATTACK_ENERGY, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "Energy Shield",
	 {{-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "casts something that is not a spell", "annoying ", "", "unknown"},
	// SPECIAL_WEATHER_SHIELD
	{"a weather shield",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_SHIELD,
	 {ABattleEnums::ATTACK_WEATHER, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "Weather Shield",
	 {{-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "casts something that is not a spell", "annoying ", "", "unknown"},
	// SPECIAL_SPIRIT_SHIELD
	{"a spirit shield",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_SHIELD,
	 {ABattleEnums::ATTACK_SPIRIT, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "Spirit Shield",
	 {{-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "casts something that is not a spell", "annoying ", "", "unknown"},
	// SPECIAL_DISPEL_ILLUSIONS
        /*
	{"dispel illusion",
	 SpecialType::HIT_ILLUSION,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_DISPEL,4,2,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_ENERGY,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "casts Dispel Illusions", "dispelling ", " illusions", "dispelling"},

	// SPECIAL_SUMMON_STORM
	{"summon storm",
	 SpecialType::HIT_EFFECTEXCEPT,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {EFFECT_STORM, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_WEATHER,2,25,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_WEATHER,EFFECT_STORM},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "summons a terrible storm", "reducing the effectiveness of ", " troops", "storm"},
	// SPECIAL_TORNADO
	{"summon tornado",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_WEATHER,2,60,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_WEATHER, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "summons a wild tornado", "killing ", "", "tornado"},
	// SPECIAL_BLACK_WIND,
        /*
	{"a black wind",
	 0, // SpecialType::HIT_SOLDIEREXCEPT,
	 {-1, -1, -1},
	 0, // {I_SKELETON, I_UNDEAD, I_LICH, I_IMP, I_DEMON, I_BALROG, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_SPIRIT,2,60,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_SPIRIT,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "summons the black wind", "killing ", "", "black wind"},

	// SPECIAL_ENERGY_BLAST,
        /*
	{"an energy blast",
	 SpecialType::HIT_SOLDIEREXCEPT,
	 {-1, -1, -1},
	 {I_SKELETON, I_UNDEAD, I_LICH, I_IMP, I_DEMON, I_BALROG, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_ENERGY,2,60,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_ENERGY,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "blasts the enemy with energy", "killing ", "", "energy blast"},
	// SPECIAL_BANISH_UNDEAD,
	{"banish undead",
	 SpecialType::HIT_SOLDIERIF,
	 {-1, -1, -1},
	 {I_SKELETON, I_UNDEAD, I_ISKELETON, I_IUNDEAD, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_DISPEL,2,5,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_ENERGY,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "casts Banish Undead", "banishing ", " undead", "banishing"},
	// SPECIAL_BANISH_DEMONS,
	{"banish demons",
	 SpecialType::HIT_SOLDIERIF,
	 {-1, -1, -1},
	 {I_IMP, I_DEMON, I_IIMP, I_IDEMON, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_DISPEL,2,5,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_SPIRIT,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "casts Banish Demons", "banishing ", " demons", "banishing"},

	// SPECIAL_FIREBREATH,
	{"fiery breath",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_ENERGY,2,5,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_ENERGY,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "breathes fire", "killing ", "", "firebreath"},
	// SPECIAL_POISONBREATH,
	{"poisonous breath",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_SPIRIT,2,5,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_ENERGY,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "breathes poison", "killing ", "", "poisonbreath"},
	// SPECIAL_ICEBREATH,
	{"icy breath",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_ENERGY,2,5,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_ENERGY,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "breathes coldness", "killing ", "", "icebreath"},
        /*
	// SPECIAL_CAMEL_FEAR
	{"terrify horses",
	 SpecialType::HIT_EFFECTEXCEPT|SpecialType::HIT_MOUNTIF,
	 {-1, -1, -1},
	 {I_HORSE, I_LIZARD, -1, -1, -1, -1, -1},
	 {EFFECT_FEAR, -1, -1},
	 SpecialType::FX_DAMAGE,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_RIDING,1,0,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_SPIRIT,EFFECT_FEAR},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "strikes fear into enemy mounts", "causing ", " mounts to panic", "panic"},

	// SPECIAL_INVULNERABILITY
	{"invulnerability",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_SHIELD,
	 {ABattleEnums::NUM_ATTACK_TYPES, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "invulnerability",
	 {{-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "casts something that is not a spell", "annoying ", "", "unknown"},
	// SPECIAL_MAGESTAFF
	{"terrify enemies",
	 SpecialType::HIT_EFFECTEXCEPT,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {EFFECT_FEAR, -1, -1},
	 SpecialType::FX_DAMAGE,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_SPIRIT,1,0,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_SPIRIT,EFFECT_FEAR},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "strikes fear into enemy hearts", "causing ", " men to panic", "terrifying"},
        /*
	// SPECIAL_RAINBOW,
	{"rainbow rays",
	 SpecialType::HIT_SOLDIEREXCEPT,
	 {-1, -1, -1},
	 {I_RAINBOWDRAKE, I_COMMONMAN, I_GNOLL, I_GULLYDWARF, I_LCOMMONMAN, I_LGNOLL, I_LGULLYDWARF},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE| SpecialType::FX_SHIELD |SpecialType::FX_USE_LEV,
	 {ABattleEnums::ATTACK_ENERGY,ABattleEnums::ATTACK_WEATHER,ABattleEnums::ATTACK_SPIRIT,ABattleEnums::ATTACK_RANGED},
	 {{ABattleEnums::ATTACK_ENERGY, 5},{ABattleEnums::ATTACK_WEATHER, 5},{ABattleEnums::ATTACK_SPIRIT, 5},{ABattleEnums::ATTACK_RANGED, 5}},
	 "a rainbow shield",
	 {{ABattleEnums::ATTACK_ENERGY,2,2, AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_ENERGY,0},
	 {ABattleEnums::ATTACK_WEATHER,2,2, AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_ENERGY,0},
	 {ABattleEnums::ATTACK_SPIRIT,2,2, AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::MAGIC_ENERGY,0},
	  {-1, 0, 0, 0, 0, 0}},
	 "radiates light", "causing ", "men to leave the battlefield", "rainbow"},

	// SPECIAL_ACIDSHOWER,
	{"acid shower",
	 0,
	 {-1, -1, -1},
	 {-1, -1, -1, -1, -1, -1, -1},
	 {-1, -1, -1},
	 SpecialType::FX_DAMAGE|SpecialType::FX_USE_LEV,
	 {-1, -1, -1, -1},
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 "a non-shield spell",
	 {{ABattleEnums::ATTACK_ENERGY,2,2,
	   AWeaponType::ALWAYSREADY|AWeaponType::RANGED,AWeaponType::ARMORPIERCING,0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0},
	  {-1, 0, 0, 0, 0, 0}},
	 "showers with acid", "killing ", "", "acid shower"},
};

SpecialType *SpecialDefs = spd;

static EffectType efd[] = {
	// EFFECT_DAZZLE
	{"dazzle", -2,
	 {{-1, 0},{-1, 0},{-1, 0},{-1, 0}},
	 -1, EffectType::EFF_ONESHOT},
	// EFFECT_FEAR
	{"fear", -2,
	 {{ABattleEnums::ATTACK_COMBAT, -2},{ABattleEnums::ATTACK_RIDING, -2},{-1, 0},{-1, 0}},
	 -1, 0},
	// EFFECT_STORM
	{"storm", -2,
	 {{ABattleEnums::ATTACK_COMBAT, -2},{-1, 0},{-1, 0},{-1, 0}},
	 -1, 0},
};

EffectType *EffectDefs = efd;
*/
#endif


AScript *AScripts::Get(int index)
{
 return (AScript*)Items[index];
}

bool AScripts::Delete(int ind)
{
  AScript *scr = Get(ind);
  TList::Delete(ind);
  delete scr;
  return true;
}

void AScripts::Add(AScript* scr)
{
 TList::Add(scr);
}

void __fastcall AScripts::Clear()
{
	TList::Clear();
}

AScript::AScript()
{
  Strings = new TStringList();
  CountStr = 0;
  caption = "";
  NeedClear = false;
}
__fastcall AScript::~AScript() { delete Strings; };


AScripts*AllScripts;
