//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "skills.h"
#include "eapfile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

ASkill::ASkill()
 :days(0)
{
}
__fastcall ASkill::~ASkill()
{
}
void ASkill::Read(TFile& in)
{
 AnsiString _type;
 if(BaseVer>=32)
   in>>_type;
 else
   ReadStringAsInt(in,_type);
 SetType(_type,true);
 if(!type)
  throw Exception("Skill::Read: Unknown skill "+_type);
 ReadIntAsShort(in,days);
}
void ASkill::Write(TFile & out)
{
 out<<type->abr;
 WriteIntAsShort(out,days);
}
void ASkill::CreateNew(ASkill * newskill)
{
 type=newskill->type;
 days=newskill->days;
}
void ASkill::SetType(AnsiString abr, bool addunknown){
 type=SkillTypes->Get(abr,2,addunknown); //abr
}
AnsiString ASkill::Print(int mode)
{
 AnsiString s;
// ASkillType *stype=GetType();
 if(!type) return "";

 if(mode&1) s=type->name;
 if(mode&2){
  s+=" [";
  s+=type->abr;
  s+="]";
 }
 if(mode&4){
  s+=" ";
  s+=GetLevel();
 }
 if(mode&8){
  s+=" (";
  s+=days;
  s+=")";
 }
 return s;
}
int ASkill::LevelToDays(int level){
 int _days = 0;
 for(;level>0;level--){
  _days+=level*30;
 }
 return _days;
}
int ASkill::DaysToLevel(int dayspermen){
 int z=30;
 int i=0;
 while(dayspermen>=z){
  i++;
  dayspermen-=z;
  z+=30;
 }
 return i;
}
int ASkill::GetLevel()
{
 return DaysToLevel(days);
/* int dayspermen=days;
 int z=30;
 int i=0;
 while(dayspermen>=z){
  i++;
  dayspermen-=z;
  z+=30;
 }
 return i;*/
}
void ASkill::SetLevel(int level)
{
 days=LevelToDays(level);
/* int _days = 0;

 for(;level>0;level--){
  _days+=level*30;
 }
 days=_days;*/
}
ASkills::ASkills()
{
}
__fastcall ASkills::~ASkills()
{
}
void __fastcall ASkills::Clear()
{
 for(int i=count-1;i>=0;i--){
  ASkill *sk=Get(i);
//  TList::Delete(i);
  delete sk;
 }
 TList::Clear();
}
void ASkills::Read(TFile& in)
{
 Clear();
 int kol;
 in>>kol;
 Capacity=kol;
 for(int i=0;i<kol;i++){
  ASkill *sk=new ASkill;
  sk->Read(in);
  Add(sk);
 }
}
void ASkills::Write(TFile& out)
{
 int kol=count;
 out<<kol;
 for(int i=0;i<kol;i++){
  ASkill *sk=Get(i);
  sk->Write(out);
 }
}
ASkill * ASkills::Get(int index)
{
 return (ASkill*)Items[index];
}
ASkill * ASkills::GetSkill(AnsiString type)
{
 foreach(this){
  ASkill *sk=*(ASkill**)iter;
  if(sk->type->abr==type)return sk;
 }
 return 0;
}
ASkill * ASkills::GetSkill(ASkillType *type)
{
 foreach(this){
  ASkill *sk=*(ASkill**)iter;
  if(sk->type==type)return sk;
 }
 return 0;
}
void ASkills::Add(ASkill *sk)
{
 if(GetSkill(sk->type))
  throw Exception("Skills::Add: Already Exist");
 TList::Add(sk);
}
void ASkills::CreateNew(ASkills * prvsks)
{
 Clear();
 UpdateFromPrev(prvsks);
}
void ASkills::Update(ASkills *newsks)
{
 foreach(newsks){
  ASkill *newsk=*(ASkill**)iter;
  ASkill *sk=GetSkill(newsk->type);
  if(!sk){
   sk=new ASkill;
   sk->CreateNew(newsk);
   Add(sk);
  }else{
   sk->days=newsk->days;
  }
 }
}
void ASkills::UpdateFromPrev(ASkills *prvsks)
{
 foreach(prvsks){
  ASkill *prvsk=*(ASkill**)iter;
  ASkill *sk=GetSkill(prvsk->type);
  if(sk)continue;
  sk=new ASkill;
  sk->CreateNew(prvsk);
  Add(sk);
 }
}
AnsiString ASkills::Print(int mode)
{
 AnsiString s;
 if(!count) return KeyPhrases->Get(keyNoneSign);
 foreach(this){
  ASkill *sk=*(ASkill**)iter;
  s+=sk->Print(mode)+", ";
 }
 s.SetLength(s.Length()-2);
 return s;
}
void ASkills::DeleteEmpty()
{
 for(int i=Count-1;i>=0;i--){
  ASkill *sk=Get(i);
  if(sk->days<=0){
   TList::Delete(i);
   delete sk;
  }
 }
}


int ASkill::GetSize()
{
 int siz=sizeof(*this);
// siz+=type.Length()+1;
 return siz;
}


int ASkills::GetSize()
{
 int siz=sizeof(*this);
 siz+=Capacity*4;
 foreach(this){
  ASkill *sk=*(ASkill**)iter;
  siz+=sk->GetSize();
 }
 return siz;
}

