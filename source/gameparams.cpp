//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <inifiles.hpp>
#include <memory>
#include "gameparams.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
int LeaderMaintCost=20;
int ManMaintCost=10;
int FoodMaintKoeff=10;
int TaxEffect=50;
int EntertainEffect=20;
int StudyAddDays=30;
int TeachAddDays=30;
int TeachMaxStudents=10;
int SailSpeed=4;
int WalkSpeed=2;
int RideSpeed=4;
int FlySpeed=6;
int WindSpeed=8; // маг с SWIN на летающем средстве передвижения
AnsiString SailSkill="SAIL";
AnsiString EnteSkill="ENTE";
AnsiString CombSkill="COMB";
AnsiString RidiSkill="RIDI";
AnsiString LbowSkill="LBOW";
AnsiString XbowSkill="XBOW";
AnsiString TactSkill="TACT";
AnsiString SwinSkill="SWIN";
AnsiString SteaSkill="STEA";
AnsiString ObseSkill="OBSE";
AnsiString TrueSkill="TRUE";
AnsiString IlluSkill="ILLU";
AnsiString MindSkill="MIND";
AnsiString PhenSkill="PHEN";
AnsiString RingItem="RING";
AnsiString AmtsItem="AMTS";
int RingSteaBonus=3;
int AmtsObseBonus=2;
AnsiString XXXXItem="XXXX";
int MaxFactionsPoints=5;
int SilverWeight = 0;

TGameParams GameParams;
const AnsiString Section="GameParams";
//---------------------------------------------------------------------------
bool TGameParam::IsPtrSet()
{
  switch(type)
  {
    case 0: return ptr;
    case 1: return ptrstr;
  }
  return false;
}
//---------------------------------------------------------------------------
AnsiString TGameParam::GetString()
{
  switch(type)
  {
    case 0:
     if(!ptr)
       return EmptyStr;
     return *ptr;
    case 1:
     if(!ptrstr)
       return EmptyStr;
     return *ptrstr;
  }
  return EmptyStr;
}
//---------------------------------------------------------------------------
bool TGameParam::SetString(const AnsiString &value)
{
  switch(type)
  {
    case 0:
      if(!ptr) return false;
      *ptr=atoi(value.c_str());
    break;
    case 1:
      if(!ptrstr) return false;
      *ptrstr=value;
    break;
    default:
      return false;
  }
  return true;
}
//---------------------------------------------------------------------------
TGameParams::TGameParams()
{
}
//---------------------------------------------------------------------------
TGameParams::~TGameParams()
{
}
//---------------------------------------------------------------------------
bool TGameParams::Load(TIniFile *ini)
{
  try{
    auto_ptr<TStringList> list(new TStringList);
    ini->ReadSectionValues(Section,&*list);
    for(int i=0;i<list->Count;i++)
    {
      AnsiString name=list->Names[i];
      AnsiString valuestr=list->Values[name];
      SetGameParam(name,valuestr);
    }
  }catch(...){
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
bool TGameParams::Save(TIniFile *ini)
{
  try{
    ini->EraseSection(Section);
	for(TGameParam *gp=params.begin().operator ->(),*endgp=params.end().operator ->();gp<endgp;gp++)
    {
      if(!gp->IsPtrSet()) continue;
      ini->WriteString(Section,gp->name,gp->GetString());
    }
  }catch(...){
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
bool TGameParams::Init()
{
#define ADDGAMEPARAM(name,comment) AddGameParam(#name,&name,comment)
  ADDGAMEPARAM(LeaderMaintCost,"Maintenance cost of one leader");
  ADDGAMEPARAM(ManMaintCost,"Maintenance cost of one peasant");
  ADDGAMEPARAM(FoodMaintKoeff,"Maintenance effect from one food item");
  ADDGAMEPARAM(TaxEffect,"Silv collected by one taxer");
  ADDGAMEPARAM(EntertainEffect,"Silv collected by one entertainer with ENTE1");
  ADDGAMEPARAM(EnteSkill,"ENTE skill abr");
  ADDGAMEPARAM(PhenSkill,"PHEN skill abr (may be empty)");
  ADDGAMEPARAM(StudyAddDays,"Days adds to skill from one STUDY command");
  ADDGAMEPARAM(TeachAddDays,"Extra days from teacher");
  ADDGAMEPARAM(TeachMaxStudents,"Max students per one teacher");
  ADDGAMEPARAM(SailSpeed,"Sailing speed");
  ADDGAMEPARAM(WalkSpeed,"Walking speed");
  ADDGAMEPARAM(RideSpeed,"Riding speed");
  ADDGAMEPARAM(FlySpeed,"Flying speed");
  ADDGAMEPARAM(WindSpeed,"Speed of flying mage with SummonWind");
  ADDGAMEPARAM(SwinSkill,"Summon Wind skill abr (may be empty)");
  ADDGAMEPARAM(SailSkill,"SAIL skill abr");
  ADDGAMEPARAM(CombSkill,"COMB skill abr");
  ADDGAMEPARAM(RidiSkill,"RIDI skill abr");
  ADDGAMEPARAM(LbowSkill,"LBOW skill abr");
  ADDGAMEPARAM(XbowSkill,"XBOW skill abr");
  ADDGAMEPARAM(TactSkill,"TACT skill abr");
  ADDGAMEPARAM(SteaSkill,"STEA skill abr");
  ADDGAMEPARAM(ObseSkill,"OBSE skill abr");
  ADDGAMEPARAM(TrueSkill,"TRUE skill abr");
  ADDGAMEPARAM(IlluSkill,"ILLU skill abr");
  ADDGAMEPARAM(MindSkill,"MIND skill abr (may be empty)");
  ADDGAMEPARAM(RingItem,"RING item abr");
  ADDGAMEPARAM(RingSteaBonus,"RING bonus to stealth");
  ADDGAMEPARAM(AmtsItem,"AMTS item abr");
  ADDGAMEPARAM(AmtsObseBonus,"AMTS bonus to observation");
  ADDGAMEPARAM(XXXXItem,"Amulet of Invulnerability abr");
  ADDGAMEPARAM(MaxFactionsPoints, "Maximum Faction Points");
  ADDGAMEPARAM(SilverWeight, "Silver Weight");
#undef  ADDGAMEPARAM
  return true;
}
//---------------------------------------------------------------------------
bool TGameParams::AddGameParam(AnsiString name, int * ptr, AnsiString comment)
{
  TGameParam *gp0=FindGameParam(name);
  if(gp0) return false;
  TGameParam gp;
  gp.name=name;
  gp.type=0;
  gp.ptr=ptr;
  if(!comment.Length())
	comment=name;
  gp.comment=comment;
  params.push_back(gp);
  return true;
}
//---------------------------------------------------------------------------
bool TGameParams::AddGameParam(AnsiString name, AnsiString * ptr, AnsiString comment)
{
  TGameParam *gp0=FindGameParam(name);
  if(gp0) return false;
  TGameParam gp;
  gp.name=name;
  gp.type=1;
  gp.ptrstr=ptr;
  if(!comment.Length())
	comment=name;
  gp.comment=comment;
  params.push_back(gp);
  return true;
}
//---------------------------------------------------------------------------
TGameParam *TGameParams::FindGameParam(AnsiString name)
{
  for(TGameParam *gp=params.begin().operator ->(),*endgp=params.end().operator ->();gp<endgp;gp++)
  {
    if(name.AnsiCompareIC(gp->name)) continue;
    return gp;
  }
  return 0;
}
//---------------------------------------------------------------------------
const TGameParam *TGameParams::Find_GameParam(const AnsiString& name) const
{
  for(const TGameParam *gp=params.begin().operator ->(),*endgp=params.end().operator ->();gp<endgp;gp++)
  {
    if(name.AnsiCompareIC(gp->name)) continue;
    return gp;
  }
  return 0;
}
//---------------------------------------------------------------------------
bool TGameParams::SetGameParam(AnsiString name, const AnsiString &value)
{
  TGameParam *gp=FindGameParam(name);
  if(!gp) return false;
  return gp->SetString(value);
}
//---------------------------------------------------------------------------

