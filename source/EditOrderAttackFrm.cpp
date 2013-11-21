//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdlib.h>
#include <vector.h>
#include <algorithm>
#include "util.h"
#include "orders.h"
#include "unit.h"
#include "EditOrderAttackFrm.h"
#include "faction.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderAttackForm *EditOrderAttackForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderAttackForm::TEditOrderAttackForm(TComponent* Owner, AOrderAttack *_ord, AUnits *tars)
    : TForm(Owner),ord(_ord),cantargets(tars)
    ,slist(new TStringList)
    ,facnum_search(0)
{
 cbCommented->Checked=ord->commented;
 cbRepeating->Checked=ord->repeating;


 curtarnum=0;
 AnsiString s;
 facdiap=-6;
 cbFacDiap->Items->BeginUpdate();
 cbFacDiap->Items->AddObject(" nonlocal",(TObject*)-4);
 cbFacDiap->Items->AddObject(" nonfriendly",(TObject*)-7);
 cbFacDiap->Items->AddObject(" unfriendly",(TObject*)-6);
 foreach(tars)
 {
   AUnit *un=*(AUnit**)iter;
   AFaction *fac=un->faction;
   int facnum=fac->number;
   int ind=cbFacDiap->Items->IndexOfObject((TObject*)facnum);
   if(ind<0)
     cbFacDiap->Items->AddObject(fac->FullName(),(TObject*)facnum);
 }
 cbFacDiap->Items->EndUpdate();
 cbFacDiap->Sorted=true;
 cbFacDiap->ItemIndex=cbFacDiap->Items->IndexOfObject((TObject*)facdiap);

 targets=new TList;
 for(int i=0;i<ord->targets->Count;i++){
  AUnitPtr *ptr=ord->GetTarget(i);
  if(ptr->ptr)
    targets->Add(ptr->ptr);
 }
 MakeCanTargetList();
 MakeTargetList();
}
__fastcall TEditOrderAttackForm::~TEditOrderAttackForm()
{
  delete slist;
  delete targets;
}
struct SortItem{
 int num;
 AUnit *unit;
};
static int SortItemCompare(const SortItem &a,const SortItem &b){
 return a.num<b.num;
}
void TEditOrderAttackForm::MakeCanTargetList()
{
 AnsiString s;

 vector<SortItem> list;
 int itemcount=0;
 AFaction *basefac=ord->orders->unit->faction;
 foreach(cantargets){
  AUnit *un=*(AUnit**)iter;
  if(!inFacDiap(facdiap,un->faction,basefac))
    continue;
  SortItem si;
  si.unit=un;
  si.num=un->GetMen();
  if(si.num) itemcount+=si.num;
  list.push_back(si);
 }
 if(itemcount)
  stable_sort(list.begin(),list.end(),SortItemCompare);
 else if(facdiap==-6)
 {
   int ind=cbFacDiap->Items->IndexOfObject((TObject*)facdiap);
   cbFacDiap->Items->Delete(ind);
   facdiap=-7;
   cbFacDiap->ItemIndex=cbFacDiap->Items->IndexOfObject((TObject*)facdiap);
   MakeCanTargetList();
   return;
 }
 slist->Clear();
 for(int i=0,endi=list.size();i<endi;i++){
  SortItem &si=list[i];
  AUnit *un=si.unit;
  s=un->FullName();
  if(un->faction->number){
   s+=", ";
   s+=un->faction->FullName();
  }
  s+=" - ";
  s+=si.num;
  slist->AddObject(s,(TObject*)un);
  if(un->num==curtarnum){
    cbCanTargets->Text=s;
  }
 }
 cbCanTargets->Items=slist;
 cbCanTargetsChange(0);
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAttackForm::bnAddClick(TObject *Sender)
{
 int indtar=cbCanTargets->Items->IndexOf(cbCanTargets->Text);
 AUnit *un=0;
 if(indtar<0)
 {
  AnsiString str=cbCanTargets->Text;
  if(str.Length())
  {
    if(isdigit(str[1])&&(str.Length()<2||isdigit(str[str.Length()-1])))
    {
      int num=atoi(str.c_str());
      un=cantargets->GetUnit(num);
    }
  }
 }else
   un=(AUnit*)cbCanTargets->Items->Objects[indtar];
 if(!un)
   return;
 targets->Add(un);
 MakeTargetList();
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAttackForm::bnDeleteClick(TObject *Sender)
{
 int ind=lbTargets->ItemIndex;
 if(ind==-1)return;
 targets->Delete(ind);
 MakeTargetList();
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAttackForm::bnOkClick(TObject *Sender)
{
 ord->commented=cbCommented->Checked;
 ord->repeating=cbRepeating->Checked;
 ord->Clear();
 for(int i=0;i<targets->Count;i++)
 {
  AUnit *un=(AUnit*)targets->Items[i];
  ord->AddTarget(un->num,un);
 }
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAttackForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAttackForm::cbCanTargetsChange(TObject *Sender)
{
 AnsiString str=cbCanTargets->Text;
 int indtar=cbCanTargets->Items->IndexOf(str);
 if(indtar==-1){
   TStrings *list=cbCanTargets->Items;
   if(!str.Length())
     list->Clear();
   for(int i=list->Count-1;i>=0;i--)
   {
     if(AnsiIPos(list->Strings[i],str)!=0)
       continue;
     list->Delete(i);
   }
   if(!list->Count)
   {
     int i=cbCanTargets->SelStart;
     list->Assign(slist);
     cbCanTargets->SelStart=i;
   }
   return;
 }
 AUnit *un=(AUnit*)cbCanTargets->Items->Objects[indtar];
 curtarnum=un->num;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAttackForm::cbFacDiapChange(TObject *Sender)
{
  int ind=cbFacDiap->ItemIndex;
  if(ind<0) return;
  ind=(int)cbFacDiap->Items->Objects[ind];
  if(facdiap==ind) return;
  facdiap=ind;
  MakeCanTargetList();
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAttackForm::cbFacDiapEnter(TObject *Sender)
{
  facnum_search=0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAttackForm::cbFacDiapKeyPress(TObject *Sender,
      char &Key)
{
  if(Key>='0'&&Key<='9')
  {
    facnum_search*=10;
    facnum_search+=Key-'0';
    Key=0;
    int ind=cbFacDiap->Items->IndexOfObject((TObject*)facnum_search);
    if(ind<0)
      return;
    cbFacDiap->ItemIndex=ind;
    cbFacDiapChange(0);
  }else
  {
    facnum_search=0;
  }
}
//---------------------------------------------------------------------------
void TEditOrderAttackForm::MakeTargetList()
{
 lbTargets->Items->BeginUpdate();
 lbTargets->Items->Clear();
 int maxsize=0;
 for(int i=0;i<targets->Count;i++)
 {
  AUnit *un=(AUnit*)targets->Items[i];
  AnsiString s=PrintUnit(un);
  lbTargets->Items->Add(s);
  TSize siz=lbTargets->Canvas->TextExtent(s);
  if(siz.cx>maxsize)maxsize=siz.cx;
 }
 lbTargets->Items->EndUpdate();
 lbTargets->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
}
//---------------------------------------------------------------------------
AnsiString TEditOrderAttackForm::PrintUnit(AUnit * un)
{
 AnsiString s=un->FullName();
  if(un->faction->number){
   s+=", ";
   s+=un->faction->FullName();
  }
 return s;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAttackForm::bnAddAllClick(TObject *Sender)
{
  for(int i=0,endi=cbCanTargets->Items->Count;i<endi;i++)
  {
    AUnit *un=(AUnit*)cbCanTargets->Items->Objects[i];
    if(un)
      targets->Add(un);
  }
  MakeTargetList();
}
//---------------------------------------------------------------------------

