//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <inifiles.hpp>
#include <Clipbrd.hpp>
#include <stdlib.h>

#include "import.h"
#include "util.h"
#include "region.h"
#include "faction.h"
#include "turn.h"
#include "reports.h"
#include "markets.h"
#include "unit.h"
#include "skills.h"
#include "orders.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void ImportAtlaCartRegion(char *&s){
 AnsiString errbeg="Error loading region ",err=errbeg;
 ARegion *reg=new ARegion(curTurn);
 reg->zloc=1;
 bool skipregion=false,addexits=true;
 try{
  s+=3;
  if(!isdigit(*s))
   throw Exception(err);
  reg->xloc=atoi(GetNextToken(s).c_str());

  if(*s!=' ')
   throw Exception(err);
  s++;
  if(!isdigit(*s))
   throw Exception(err);
  reg->yloc=atoi(GetNextToken(s).c_str());
  if(*s!=' ')
   throw Exception(err);
  s++;
  if(!isdigit(*s))
   throw Exception(err);
  {
   int type=atoi(GetNextToken(s).c_str());
   if(type<1||type>TerrainTypes->count)
    throw Exception(AnsiString("Bad terrain type ")+type);
   ATerrainType *ttype=TerrainTypes->Get(type-1);
   reg->type=ttype->name;
  }
  err=errbeg+reg->FullName()+"?";
  if(*s!='\n')
   throw Exception(err);
  s++;

  if(!MyCompare(s,"LV ")){
   int lev=atoi(s+3)+1;
   addexits=false;
   if(curRegionList->RegionArrays->count>lev){
    reg->zloc=lev;
   }else
    skipregion=true;
   GetNextString(s);
  }
  reg->UpdateKey();
  if(MyCompare(s,"NM "))
   throw Exception(err);
  s+=3;
  reg->name=GetNextString(s);
  err=errbeg+reg->FullName();
  if(!MyCompare(s,"CT ")){
   s+=3;
   if(!isdigit(*s))
    throw Exception(err);
   int towntype=atoi(GetNextToken(s).c_str());
   ATownType *ttype=TownTypes->Get(towntype);
   reg->towntype=ttype->name;
   if(!ttype)
    throw Exception(AnsiString("Bad town type ")+towntype);
   if(*s!='\n')
    throw Exception(err);
   s++;
   if(MyCompare(s,"CN "))
    throw Exception(err);
   s+=3;
   reg->townname=GetNextString(s);
   err=errbeg+reg->FullName();
  }
  if(!MyCompare(s,"PP ")){
   reg->hasInfo=true;
   s+=3;
   if(!isdigit(*s))
    throw Exception(err);
   reg->population=atoi(GetNextToken(s).c_str());
   if(*s!='\n')
    throw Exception(err);
   s++;
   if(!MyCompare(s,"RA ")){
    s+=3;
    if(!isdigit(*s))
     throw Exception(err);
    int race=atoi(GetNextToken(s).c_str());
//   if(type<1||type>TerrainTypes->count)
    AItemType *itype=ItemTypes->Get(race);
    if(!itype||!(itype->type&AItemType::IT_MAN))
     throw Exception(AnsiString("Bad race type ")+race);
    reg->race=itype->names;
    if(*s!='\n')
     throw Exception(err);
    s++;
   }else{
    reg->race=optConquest?"MAN":"LEAD";
    AddEvent("Warning: region "+reg->FullName()+" has bad peasants");
   }
  }
  if(!MyCompare(s,"IN ")){
   reg->hasInfo=true;
   s+=3;
   if(!isdigit(*s))
    throw Exception(err);
   reg->money=atoi(GetNextToken(s).c_str());
   if(*s!='\n')
    throw Exception(err);
   s++;
  }
  if(!MyCompare(s,"WG ")){
   reg->hasInfo=true;
   s+=3;
   if(!isdigit(*s))
    throw Exception(err);
   reg->wages=atoi(GetNextToken(s).c_str());
   if(*s!='\n')
    throw Exception(err);
   s++;
  }
  if(!MyCompare(s,"MW ")){
   reg->hasInfo=true;
   s+=3;
   if(!isdigit(*s))
    throw Exception(err);
   reg->maxwages=atoi(GetNextToken(s).c_str());
   if(*s!='\n')
    throw Exception(err);
   s++;
  }
  if(!MyCompare(s,"EN ")){
   reg->hasInfo=true;
   s+=3;
   if(!isdigit(*s))
    throw Exception(err);
   reg->entertainment=atoi(GetNextToken(s).c_str());
   if(*s!='\n')
    throw Exception(err);
   s++;
  }
  if(!MyCompare(s,"TN ")){
   GetNextString(s);
  }
  while(!MyCompare(s,"DM ")){
   reg->hasInfo=true;
   s+=3;
   if(!isdigit(*s)&&*s!='-')
    throw Exception(err);
   int kolvo=atoi(GetNextToken(s).c_str());
   if(kolvo<0){
    kolvo=9999;
   }
   if(*s!=' ')
    throw Exception(err);
   s++;
   AnsiString type=GetNextToken(s);
   AItemType *itype=ItemTypes->Get(type);
   if(!itype)
    throw Exception(AnsiString("Bad wanted type ")+type);
   if(*s!=' ')
    throw Exception(err);
   s++;
   if(!isdigit(*s))
    throw Exception(err);
   int price=atoi(GetNextToken(s).c_str());
   AMarket *mark=new AMarket;
   mark->type=itype;
   mark->amount=kolvo;
   mark->price=price;
   reg->wanteds->Add(mark);
   if(*s!='\n')
    throw Exception(err);
   s++;
  }
  while(!MyCompare(s,"OF ")){
   reg->hasInfo=true;
   s+=3;
   if(!isdigit(*s)&&*s!='-')
    throw Exception(err);
   int kolvo=atoi(GetNextToken(s).c_str());
   if(kolvo<0){
    kolvo=9999;
   }
   if(*s!=' ')
    throw Exception(err);
   s++;
   AnsiString type=GetNextToken(s);
   AItemType *itype=ItemTypes->Get(type);
   if(!itype)
    throw Exception(AnsiString("Bad saled type ")+type);
   if(*s!=' ')
    throw Exception(err);
   s++;
   if(!isdigit(*s))
    throw Exception(err);
   int price=atoi(GetNextToken(s).c_str());
   AMarket *mark=new AMarket;
   mark->type=itype;
   mark->amount=kolvo;
   mark->price=price;
   reg->saleds->Add(mark);
   if(*s!='\n')
    throw Exception(err);
   s++;
  }
  while(!MyCompare(s,"PR ")){
   reg->hasInfo=true;
   s+=3;
   if(!isdigit(*s))
    throw Exception(err);
   int kolvo=atoi(GetNextToken(s).c_str());
   if(*s!=' ')
    throw Exception(err);
   s++;
   AnsiString type=GetNextToken(s);
   AItemType *itype=ItemTypes->Get(type);
   if(!itype)
    throw Exception(AnsiString("Bad product type ")+type);
   AMarket *mark=new AMarket;
   mark->type=itype;
   mark->amount=kolvo;
   reg->products->Add(mark);
   if(*s!='\n')
    throw Exception(err);
   s++;
  }

  if(MyCompare(s,"EH\n"))
   throw Exception(err);
  s+=3;
  if(*s=='\n') s++;
  if(!skipregion){
   reg->oldinfoturnnum=0;
   reg->weather=WeatherRegions->GetWeather(reg->xloc,reg->yloc,reg->zloc,curTurnNumber);
   reg->nxtweather=WeatherRegions->GetWeather(reg->xloc,reg->yloc,reg->zloc,curTurnNumber+1);
   if(addexits)
    reg->CreateExits(63);
/*    for(int i=0;i<6;i++){
    int x=reg->xloc+movingdx[i];
    int y=reg->yloc+movingdy[i];
    if(y<0||y>63) continue;
    if(x<0) x+=64;
    if(x>63) x-=64;
    reg->neighbors[i].xloc=x;
    reg->neighbors[i].yloc=y;
    reg->neighbors[i].zloc=1;
   }*/
   if(!ProcessUpdateRegion(reg))
    throw Exception(err);
   AddEvent("Region info added for "+reg->FullName());
  }else{
   AddEvent("Underground region info skipped for "+reg->FullName());
  }
 }__finally{
  delete reg;
 }
}
bool ImportAtlaCart(AnsiString fName){
 TMemoryStream*mem=0;
 curTurns->SetCurTurn(0);
 curFactions->SetCurFaction(0);
 try{
  try{
   mem=LoadFile(fName);
   char*s=(char*)mem->Memory;
   while(!MyCompare(s,"HX ")){
    ImportAtlaCartRegion(s);
   }
  }__finally{
   delete mem;
   curTurn->Modified=true;
  }
 }catch(Exception &e){
  Application->ShowException(&e);
  return false;
 }
 return true;
}
bool ImportCrystalBall(AnsiString fName){
 curTurns->SetCurTurn(0);
 curFactions->SetCurFaction(0);
 try{
  TIniFile *inifile=0;
  TStringList *sections=new TStringList;
  try{
   inifile=new TIniFile(fName);
   inifile->ReadSections(sections);
   for(int i=0,endi=sections->Count;i<endi;i++){
    AnsiString section=sections->Strings[i];
    char *s=section.c_str();
    AnsiString sectype=GetNextToken(s);
    if(sectype=="Hex"){
     AnsiString errbeg="Error loading region ",err=errbeg;
     ARegion *reg=new ARegion(curTurn);
     reg->zloc=1;
     bool skipregion=false;
     try{
      int exits=63;//N,NE,SE,S,SW,NW
      s++;
      reg->xloc=atoi(GetNextToken(s).c_str());
      if(*s!=',')
       throw Exception(err);
      s++;
      reg->yloc=atoi(GetNextToken(s).c_str());
      if(*s!=',')
       throw Exception(err);
      s++;
      AnsiString zlocname=GetNextToken(s);
      if(zlocname=="Underworld"){
       if(curRegionList->RegionArrays->count>2){
        reg->zloc=2;
        exits=atoi(inifile->ReadString(section,"Exits","").c_str());
       }else{
        skipregion=true;
       }
      }
      reg->UpdateKey();
      err=errbeg+reg->FullName();
      AnsiString terrainstr=inifile->ReadString(section,"Terrain","");
      ATerrainType *ttype=TerrainTypes->Get(terrainstr);
      if(!ttype)
       throw Exception(err);
      reg->type=ttype->name;
      reg->name=inifile->ReadString(section,"Province","");
      err=errbeg+reg->FullName();
      AnsiString townname;
      townname=inifile->ReadString(section,"City","");
      if(townname.Length()){
       reg->towntype="city";
       reg->townname=townname;
      }else{
       townname=inifile->ReadString(section,"Town","");
       if(townname.Length()){
        reg->towntype="town";
        reg->townname=townname;
       }else{
        townname=inifile->ReadString(section,"Village","");
        if(townname.Length()){
         reg->towntype="village";
         reg->townname=townname;
        }
       }
      }
      err=errbeg+reg->FullName();
      {
       AnsiString peasants=inifile->ReadString(section,"Peasants","");
       char *s=peasants.c_str();
       if(*s){
        AnsiString race=GetNextToken(s);
        if(*s!=',')
         throw Exception(err);
        s++;
        AItemType *itype=ItemTypes->Get(race);
        if(!itype||!(itype->type&AItemType::IT_MAN))
         throw Exception(err);
        reg->race=itype->names;
        reg->population=atoi(GetNextToken(s).c_str());
       }else{
//        skipregion=true;
       }
      }
      reg->money=atoi(inifile->ReadString(section,"Tax Base","").c_str());
      reg->wages=atoi(inifile->ReadString(section,"Wages","").c_str());
      reg->maxwages=atoi(inifile->ReadString(section,"Max Wages","").c_str());
      reg->entertainment=atoi(inifile->ReadString(section,"Entertainment","").c_str());
      AnsiString productstr=inifile->ReadString(section,"Products","");
      if(productstr.Length()){
       reg->hasInfo=true;
       char *s=productstr.c_str();
       while(1){
        if(!*s) break;
        if(*s==';') s++;
        AnsiString prod=GetNextToken(s);
        AItemType *itype=ItemTypes->Get(prod);
        if(!itype)
         throw Exception(err);
        if(*s!=',')
         throw Exception(err);
        s++;
        int amount=atoi(GetNextToken(s).c_str());
        AMarket *mark=new AMarket;
        mark->type=itype;
        mark->amount=amount;
        reg->products->Add(mark);
       }
      }
      AnsiString saledstr=inifile->ReadString(section,"For Sale","");
      if(saledstr.Length()){
       char *s=saledstr.c_str();
       while(1){
        if(!*s) break;
        if(*s==';') s++;
        AnsiString prod=GetNextToken(s);
        AItemType *itype=ItemTypes->Get(prod);
        if(!itype)
         throw Exception(err);
        if(*s!=',')
         throw Exception(err);
        s++;
        int amount=atoi(GetNextToken(s).c_str());
        if(amount<0) amount=9999;
        if(*s!=',')
         throw Exception(err);
        s++;
        int price=atoi(GetNextToken(s).c_str());
        AMarket *mark=new AMarket;
        mark->type=itype;
        mark->amount=amount;
        mark->price=price;
        reg->saleds->Add(mark);
       }
      }
      AnsiString wantedstr=inifile->ReadString(section,"Wanted","");
      if(wantedstr.Length()){
       char *s=wantedstr.c_str();
       while(1){
        if(!*s) break;
        if(*s==';') s++;
        AnsiString prod=GetNextToken(s);
        AItemType *itype=ItemTypes->Get(prod);
        if(!itype)
         throw Exception(err);
        if(*s!=',')
         throw Exception(err);
        s++;
        int amount=atoi(GetNextToken(s).c_str());
        if(amount<0) amount=9999;
        if(*s!=',')
         throw Exception(err);
        s++;
        int price=atoi(GetNextToken(s).c_str());
        AMarket *mark=new AMarket;
        mark->type=itype;
        mark->amount=amount;
        mark->price=price;
        reg->wanteds->Add(mark);
       }
      }
      if(!skipregion){
       reg->oldinfoturnnum=0;
       reg->weather=WeatherRegions->GetWeather(reg->xloc,reg->yloc,reg->zloc,curTurnNumber);
       reg->nxtweather=WeatherRegions->GetWeather(reg->xloc,reg->yloc,reg->zloc,curTurnNumber+1);
       reg->CreateExits(exits);
 /*      int xsize,ysize;
       ARegionArray *ra=curRegionList->GetRegionArray(reg->zloc);

       if(exits){
        for(int i=0;i<6;i++){
         if(!(exits&(1<<i))) continue;
         int x=reg->xloc+movingdx[i];
         int y=reg->yloc+movingdy[i];
         if(y<0||y>=64) continue;
         if(x<0) x+=64;
 //        if(x>=64) x-=64;
         reg->neighbors[i].xloc=x;
         reg->neighbors[i].yloc=y;
         reg->neighbors[i].zloc=1;
        }
       }*/
       if(!ProcessUpdateRegion(reg))
        throw Exception(err);
       AddEvent("Region info added for "+reg->FullName());
      }else{
       AddEvent("Underground region info skipped for "+reg->FullName());
      }
     }__finally{
      delete reg;
     }
     continue;
    }
   }
  }__finally{
   delete inifile;
   delete sections;
   curTurn->Modified=true;
  }
 }catch(Exception &e){
  Application->ShowException(&e);
  return false;
 }

 return true;
}
AnsiString ExportMageStr(AUnit *un)
{
  AnsiString s;
  s="!";
  s+=un->num;
  s+=":";
  s+=un->faction->number;
  s+="\r\n";
  AnsiString str;
  TStringList *extra_magic_skills=curTurns->extra_magic_skills;
  bool first=true;
  foreach(un->skills)
  {
    ASkill *sk=*(ASkill**)iter;
    if(!(sk->type->flags&ASkillType::MAGIC)
      &&extra_magic_skills->IndexOf(sk->type->abr)<0)
        continue;
    AnsiString s;
    if(first)
      first=false;
    else
      str+=", ";
    str+=sk->type->abr;
    str+=" ";
    str+=sk->days;
  }
  AnsiString str2;
  while(1){
    str2=Trunc(str);
    s+=str+"\r\n";
    if(!str2.Length())
      break;
    str="  "+str2;
  }
  AOrder *ord=un->orders->GetMonthLong();
  if(ord)
    s+=ord->WriteTemplate()+"\r\n";
  return s;
}
static int __fastcall UnitFactionCompare(void * Item1, void * Item2)
{
  return ((AUnit*)Item1)->faction->number-((AUnit*)Item2)->faction->number;
}
bool ExportMages(AnsiString fName)
{
  int localmode=0;//0-unk,1-localonly,2-all
  TStringList *list=new TStringList;
  AUnits *uns=new AUnits(0);
  uns->autodelete=false;
  try
  {
    try
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
            if(un->num<0) continue;
            if(!un->IsMage()) continue;
            if(!un->faction->local)
            {
              if(localmode==0)
              {
                int ret=Application->MessageBox("Non local mage found. Export it?","Mages export",MB_YESNOCANCEL);
                if(ret==IDCANCEL)
                  throw 1;
                localmode=ret==IDYES?2:1;
              }
              if(localmode==1)
                continue;
            }
            uns->Add(un);
          }
        }
      }
      uns->Sort(UnitFactionCompare);
      foreach(uns)
      {
        AUnit *un=*(AUnit**)iter;
        list->Add(ExportMageStr(un));
      }
      Clipboard()->AsText=list->Text;
      if(fName.Length())
        list->SaveToFile(fName);
    }catch(...){}
  }__finally
  {
    delete uns;
    delete list;
  }
  return true;
}
bool ImportMages(AnsiString fName)
{
  int localmode=0;//0-unk,1-localonly,2-all
  AnsiString mes;
  TMemoryStream*mem=0;
  try{
    try{
      mem=LoadFile(fName);
      char*s=(char*)mem->Memory;
      AddEvent(AnsiString());
      SkipEmptyLines(s);
      while(*s=='!'){
        s++;
        int num=atoi(GetNextToken(s).c_str());
        int facnum=0;
        if(*s==':')
        {
          s++;
          facnum=atoi(GetNextToken(s).c_str());
        }
        GetNextString(s);
        AnsiString skills=GetNextString(s,1);
        AnsiString order;
        if(*s!='\n')
          order=GetNextString(s,1);
        SkipEmptyLines(s);
        AUnit *un=curRegionList->GetUnit(num);
        if(!un)
        {
          mes="Warning: unit ";
          mes+=num;
          mes+=" not found";
          AddEvent(mes);
          continue;
        }
        if(!un->faction->local)
        {
          if(localmode==0)
          {
            int ret=Application->MessageBox("Non local mage found. Import it?","Mages import",MB_YESNOCANCEL);
            if(ret==IDCANCEL)
              throw 1;
            localmode=ret==IDYES?2:1;
          }
          if(localmode==1)
            continue;

          if(facnum&&un->faction->number==0)
            un->SetFaction(facnum);
          un->info=INFO_LOCAL;
          char *s=skills.c_str();
          while(*s)
          {
            while(*s==' ') s++;
            AnsiString abr=GetNextToken(s);
            while(*s==' ') s++;
            if(!isdigit(*s))
              break;
            int days=atoi(GetNextToken(s).c_str());
            while(*s==' ') s++;
            ASkillType *stype=SkillTypes->Get(abr,2);
            if(!stype)
            {
              mes="Error: Unknown type ";
              mes+=abr;
              AddEvent(mes);
              ShowMessage(mes);
              throw 2;
            }
            int days2=un->GetBegSkillDays(abr);
            if(days2<days)
            {
              un->SetBegSkillDays(abr,days);
              un->hasskills=true;
            }
            if(*s!=',')
              break;
            s++;  
          }
        }
        AnsiString mes="Imported ";
        mes+=un->FullName();
        if(order.Length())
        {
          AOrder *ord=un->orders->GetMonthLong();
          if(ord&&ord->WriteTemplate()!=order)
          {
            ord->commented=true;
            ord=0;
          }
          if(!ord)
          {
            ord=ParseOrder(order);
            if(ord)
            {
              un->orders->Add(ord);
              mes+=" with changes of monthlong order";
            }
          }
        }
        AddEvent(mes);
      }
    }__finally{
      delete mem;
      curTurn->Modified=true;
    }
  }catch(Exception &e){
    Application->ShowException(&e);
    return false;
  }catch(...){}
  return true;
}

