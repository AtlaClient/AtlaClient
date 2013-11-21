//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "markets.h"
#include "eapfile.h"
#include "items.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


AMarket::AMarket()
 :amount(0),price(0)
{
}
__fastcall AMarket::~AMarket()
{
}
void AMarket::Read(TFile& in)
{
 AnsiString _type;
 if(BaseVer>=32)
   in>>_type;
 else
   ReadStringAsInt(in,_type);
 SetType(_type,true);
 if(!type)
  throw Exception("Market::Read: Unknown item "+_type);
 in>>amount;
 in>>price;
}
void AMarket::Write(TFile & out)
{
 out<<type->abr;
 out<<amount;
 out<<price;
}
void AMarket::CreateNew(AMarket * newmark)
{
 type=newmark->type;
 amount=newmark->amount;
 price=newmark->price;
}
void AMarket::SetType(AnsiString abr, bool addunknown){
 type=ItemTypes->Get(abr,4,addunknown);
}
AnsiString AMarket::Print(int mode)
{
 AnsiString s;
 if(mode&1){
  if(amount==9999)s=KeyPhrases->Get(keyUnlimitedSign)+" ";
  else if(amount!=1){
   s+=amount;
   s+=" ";
  }
 }
// AItemType *itype=GetType();
 if(!type){
  s+="? [?]";
 }else{
  if(mode&2){
   if(amount==1)s+=type->name;
   else s+=type->names;
  }
  if(mode&4){
   s+=" [";
   s+=type->abr;
   s+="]";
  }
 }
 if(mode&8){
  s+=KeyPhrases->Get(keyMarketAtSign);
  s+=price;
 }
 return s;
}
/*AItemType* AMarket::GetType()
{
 return ItemTypes->Get(type,4); //abr find
}*/

AMarkets::AMarkets()
{
}
__fastcall AMarkets::~AMarkets()
{
}
void __fastcall AMarkets::Clear()
{
 int i=Count-1;
 if(i<0) return;
 AMarket **iter=(AMarket**)List+i;
 for(;i>=0;i--,iter--){
  delete *iter;
// for(int i=count-1;i>=0;i--){
//  AMarket *it=Get(i);
//  TList::Delete(i);
//  delete it;
 }
 TList::Clear();
}
void AMarkets::Delete(int ind)
{
 AMarket *it=Get(ind);
 TList::Delete(ind);
 delete it;
}
void AMarkets::Read(TFile& in)
{
 Clear();
 int kol;
 in>>kol;
 Capacity=kol;
 for(int i=0;i<kol;i++){
  AMarket *mt=new AMarket;
  mt->Read(in);
  Add(mt);
 }
}
void AMarkets::Write(TFile& out)
{
 int kol=count;
 out<<kol;
 foreach(this){
  AMarket *mt=*(AMarket**) iter;
  mt->Write(out);
 }
}
AMarket * AMarkets::Get(int index)
{
 return (AMarket*)Items[index];
}
AMarket * AMarkets::GetMarket(AnsiString type)
{
 foreach(this){
  AMarket *mt=*(AMarket**) iter;
  if(mt->type->abr==type)return mt;
 }
 return 0;
}
AMarket * AMarkets::GetMarket(AItemType *type)
{
 foreach(this){
  AMarket *mt=*(AMarket**) iter;
  if(mt->type==type)return mt;
 }
 return 0;
}
int AMarkets::GetNum(AnsiString type)
{
 foreach(this){
  AMarket *mt=*(AMarket**) iter;
  if(mt->type->abr==type)return mt->amount;
 }
 return 0;
}

void AMarkets::Add(AMarket * mark)
{
 if(GetMarket(mark->type))
  throw Exception("Markets::Add: Already Exist");
 TList::Add(mark);
}
void AMarkets::CreateNew(AMarkets * prvmarks)
{
 Clear();
 foreach(prvmarks){
  AMarket *prvmt=*(AMarket**) iter;
  AMarket *mt=GetMarket(prvmt->type);
  if(mt)continue;
  mt=new AMarket;
  mt->CreateNew(prvmt);
  Add(mt);
 }
}
void AMarkets::Update(AMarkets *newmarks)
{
 foreach(newmarks){
  AMarket *newmt=*(AMarket**) iter;
  AMarket *mt=GetMarket(newmt->type);
  if(!mt){
   mt=new AMarket;
   mt->CreateNew(newmt);
   Add(mt);
  }else{
   mt->amount=newmt->amount;
   mt->price=newmt->price;
  }
 }
}
void AMarkets::UpdateFromPrev(AMarkets * prvmarks)
{
 foreach(prvmarks){
  AMarket *prvmt=*(AMarket**) iter;
  if(!(prvmt->type->type&AItemType::IT_ADVANCED)) continue;
  AMarket *mt=GetMarket(prvmt->type);
  if(mt)continue;
  mt=new AMarket;
  mt->CreateNew(prvmt);
  Add(mt);
 }
}
//if mode&16, не записывать AdvItems
AnsiString AMarkets::Print(int mode)
{
 bool noadv=mode&16;
 mode&=~16;
 AnsiString s;
 if(!count)return KeyPhrases->Get(keyNoneSign);
 foreach(this){
  AMarket *mt=*(AMarket**) iter;
  if(noadv&&(mt->type->type&AItemType::IT_ADVANCED)) continue;
  s+=mt->Print(mode)+", ";
 }
 s.SetLength(s.Length()-2);
 return s;
}
void AMarkets::Assign(AItems *list)
{
 Clear();
 foreach(list){
  AItem *item=*(AItem**) iter;
  AMarket *mark=new AMarket;
  mark->type=item->type;
  mark->amount=item->count;
  mark->price=0;
  Add(mark);
 }
}
void AMarkets::DeleteEmpty()
{
 int i=Count-1;
 AMarket **iter=(AMarket**)List+i;
 for(;i>=0;i--,iter--){
// for(int i=Count-1;i>=0;i--){
  AMarket *mt=*iter; //Get(i);
  if(mt->amount<=0){
   TList::Delete(i);
   delete mt;
  }
 }
}
int AMarket::GetSize()
{
 int siz=sizeof(*this);
// siz+=type.Length()+1;
 return siz;
}
int AMarkets::GetSize()
{
 int siz=sizeof(*this);
 siz+=Capacity*4;
 foreach(this){
  AMarket *mark=*(AMarket**)iter;
  siz+=mark->GetSize();
 }
 return siz;
}

