//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "items.h"
#include "eapfile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

AItem::AItem()
 :count(0),type(0)
{
}
__fastcall AItem::~AItem()
{
}
void AItem::Read(TFile& in)
{
 AnsiString _type;
 if(BaseVer>=32)
   in>>_type;
 else
   ReadStringAsInt(in,_type);
 SetType(_type,true);
 if(!type)
  throw Exception("Item::Read: Unknown item "+_type);
 in>>count;
}
void AItem::Write(TFile& out)
{
 out<<type->abr;
 out<<count;
}
void AItem::CreateNew(AItem *newit)
{
 type=newit->type;
 count=newit->count;
}
void AItem::SetType(AnsiString abr, bool addunknown){
 type=ItemTypes->Get(abr,4,addunknown);
}

AnsiString AItem::Print(int mode)
{
  AnsiString s;
  if((type)&&(type->type==AItemType::IT_UNFINISHED))
  {
	  s = type->name;
	  s += " [";
	  s += type->abr;
	  s += "] (needs ";
	  s += count;
	  s += ")";
	  return s;
  }
  if(mode&1)
	if(count!=1)
	{
	  s += count;
	  s += " ";
	}
	if(!type)
	{
	  s += "? [?]";
	  return s;
	}

  if(mode&2)
  {
	if(count==1) s += type->name;
	  else s += type->names;
  }
  if(mode&4)
  {
	bool illusion = ItemTypes->IsIllusionAbr(type->abr);
	s += " [";
	if(illusion) s += ItemTypes->IllusionAbrToNormal(type->abr);
	  else s += type->abr;
	s += "]";
	if(illusion) s += KeyPhrases->Get(keyItemIllusionSign);
  }
  return s;
}
AnsiString AItem::WriteReport(bool seeillusions){
 AnsiString s;
 if(count!=1){
  s+=count;
  s+=" ";
 }
 if(!type) return "";
 if(count==1)s+=type->name;
 else s+=type->names;

 bool illusion=ItemTypes->IsIllusionAbr(type->abr);
 s+=" [";
 if(illusion)
  s+=ItemTypes->IllusionAbrToNormal(type->abr);
 else
  s+=type->abr;
 s+="]";
 if(seeillusions&&illusion)
  s+=KeyPhrases->Get(keyItemIllusionSign);
 return s;
}

/*AItemType * AItem::GetType()
{
 return ItemTypes->Get(type,4); //abr find
}*/
bool AItem::IsSoldier()
{
// AItemType *itype=GetType();
 if(type->type&(AItemType::IT_MAN|AItemType::IT_MONSTER))
  return true;
 return false;
}
AItems::AItems()
{
}
__fastcall AItems::~AItems()
{
 Clear();
}
void AItems::Read(TFile& in)
{
 Clear();
 int kol;
 in>>kol;
 Capacity=kol;
 for(int i=0;i<kol;i++){
  AItem *it=new AItem;
  it->Read(in);
  Add(it);
 }
}
void AItems::Write(TFile & out)
{
 int kol=count;
 out<<kol;
 for(int i=0;i<kol;i++){
  AItem *it=Get(i);
  it->Write(out);
 }
}
void __fastcall AItems::Clear()
{
 for(int i=Count-1;i>=0;i--){
  AItem *it=Get(i);
//  TList::Delete(i);
  delete it;
 }
 TList::Clear();
}
AItem* AItems::Get(int index)
{
 return (AItem*)Items[index];
}
AItem* AItems::GetItem(AnsiString type)
{
 foreach(this){
  AItem *it=*(AItem**)iter;
  if(it->type->abr==type)return it;
 }
 return 0;
}
AItem* AItems::GetItem(AItemType *type)
{
 foreach(this){
  AItem *it=*(AItem**)iter;
  if(it->type==type)return it;
 }
 return 0;
}
void AItems::Add(AItem *item)
{
 if(GetItem(item->type->abr))
  throw Exception("Items::Add: Already Exist");
 TList::Add(item);
}
void AItems::CreateNew(AItems * prvitems)
{
 Clear();
 UpdateFromPrev(prvitems);
}
void AItems::UpdateFromPrev(AItems *prvitems)
{
 foreach(prvitems){
  AItem *prvit=*(AItem**)iter;
  AItem *it=GetItem(prvit->type->abr);
  if(it)continue;
  it=new AItem;
  it->CreateNew(prvit);
  Add(it);
 }
 Sort();
}
void AItems::Update(AItems *newitems)
{
 foreach(newitems){
  AItem *newit=*(AItem**)iter;
  AItem *it=GetItem(newit->type->abr);
  if(!it){
   it=new AItem;
   it->CreateNew(newit);
   Add(it);
  }else it->count=newit->count;
 }
 Sort();
}

AnsiString AItems::Print()
{
 AnsiString s;
 foreach(this){
  AItem *it=*(AItem**)iter;
  s+=it->Print()+", ";
 }
 s.SetLength(s.Length()-2);
 return s;
}
AnsiString AItems::WriteReport(int obs,bool seeillusions,bool nofirstcomma){
 AnsiString s;
 foreach(this){
  AItem *it=*(AItem**)iter;
  if(obs!=2){
//   AItemType *itype=it->GetType();
   if(!it->type->weight) continue;
  }
  if(nofirstcomma){
   nofirstcomma=false;
  }else{
   s+=", ";
  }
  s+=it->WriteReport(seeillusions);
 }
 return s;
}
AnsiString AItems::BattleReport(bool nofirstcomma)
{
 AnsiString s;
 foreach(this){
  AItem *it=*(AItem**)iter;
  if(!it->type->combat) continue;
  if(nofirstcomma){
   nofirstcomma=false;
  }else{
   s+=", ";
  }
  s+=it->WriteReport(false);
  if(it->type->type&AItemType::IT_MONSTER){
   AMonstrType *mtype=MonstrTypes->Get(it->type->abr);
   if(mtype){
    s+=KeyPhrases->Get(keyMonstrCombat);
    s+=mtype->attackLevel;
    s+="/";
    s+=mtype->defense[ABattleEnums::ATTACK_COMBAT];
    s+=KeyPhrases->Get(keyMonstrAttacks);
    s+=mtype->numAttacks;
    s+=KeyPhrases->Get(keyMonstrHits);
    s+=mtype->hits;
    s+=KeyPhrases->Get(keyMonstrTactics);
    s+=mtype->tactics;
    s+=")";
   }
  }
 }
 return s;
}
int AItems::GetNum(AnsiString type)
{
 AItem *it=GetItem(type);
 if(!it)return 0;
 return it->count;
}
void AItems::SetNum(AnsiString type, int num)
{
 AItem *it=GetItem(type);
 if(!it){
  if(!num) return;
  it=new AItem;
  it->SetType(type);
  Add(it);
 }
 it->count=num;
}
int AItems::GetNum(AItemType* type)
{
 AItem *it=GetItem(type);
 if(!it)return 0;
 return it->count;
}
void AItems::SetNum(AItemType*type, int num)
{
 AItem *it=GetItem(type);
 if(!it){
  if(!num) return;
  it=new AItem;
  it->type=type;
  Add(it);
 }
 it->count=num;
}
void AItems::DeleteEmpty()
{
 for(int i=Count-1;i>=0;i--){
  AItem *it=Get(i);
  if(it->count==0){
   TList::Delete(i);
   delete it;
  }
 }
}


int AItem::GetSize()
{
 int siz=sizeof(*this);
// siz+=type.Length()+1;
 return siz;
}
int AItems::GetSize()
{
 int siz=sizeof(*this);
 siz+=Capacity*4;
 foreach(this){
  AItem *it=*(AItem**)iter;
  siz+=it->GetSize();
 }
 return siz;
}
void AItems::DeleteNonCombat()
{
 for(int i=Count-1;i>=0;i--){
  AItem *it=Get(i);
//  AItemType *type=it->GetType();
  if(it->type->combat) continue;
  TList::Delete(i);
  delete it;
 }
}
int AItem::GetSortType()
{
  int st=1000;
  if(type->type&AItemType::IT_MAN)
    st=0;
  else if(type->abr=="SILV")
    st=1;
  else if(type->type&AItemType::IT_MONSTER)
    st=2;
  else if(type->type&AItemType::IT_WEAPON)
    st=3;
  else if(type->type&AItemType::IT_ARMOR)
    st=4;
  else if(type->type&AItemType::IT_MOUNT)
    st=5;
  else if(type->hitchItem.Length())
    st=6; //вагоны
  else if(type->walk-type->weight>=150)
    st=6; //также вагоны
  return st;
}
int __fastcall ItemsCompare(void * Item1, void * Item2){
 AItem *it1=(AItem*)Item1,*it2=(AItem*)Item2;
 int d=it1->GetSortType()-it2->GetSortType();
 if(d) return d;
 return it1->type-it2->type;
}
void AItems::Sort()
{
  TList::Sort(ItemsCompare);
}

