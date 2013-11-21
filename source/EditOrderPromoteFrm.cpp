//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdlib.h>
#include <vector.h>
#include <algorithm>
#include "util.h"
#include "orders.h"
#include "unit.h"
#include "EditOrderPromoteFrm.h"
#include "faction.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderPromoteForm *EditOrderPromoteForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderPromoteForm::TEditOrderPromoteForm(TComponent* Owner, AOrderInt *_ord, AUnits *tars)
    : TForm(Owner),ord(_ord),targets(tars)
    ,slist(new TStringList)
    ,facnum_search(0)
{
 cbCommented->Checked=ord->commented;
 cbRepeating->Checked=ord->repeating;

 curtarnum=ord->value;
 AnsiString s;
 facdiap=-3;
 cbFacDiap->Items->BeginUpdate();
 cbFacDiap->Items->AddObject(" all",(TObject*)-2);
 cbFacDiap->Items->AddObject(" local",(TObject*)-3);
 cbFacDiap->Items->AddObject(" nonlocal",(TObject*)-4);
 cbFacDiap->Items->AddObject(" friendly",(TObject*)-5);
 foreach(tars)
 {
   AUnit *un=*(AUnit**)iter;
   AFaction *fac=un->faction;
   int facnum=fac->number;
   if(un->num==curtarnum)
   {
     if(un->faction->local)
       facdiap=-3;
     else
       facdiap=-4;
   }
   int ind=cbFacDiap->Items->IndexOfObject((TObject*)facnum);
   if(ind<0)
     cbFacDiap->Items->AddObject(fac->FullName(),(TObject*)facnum);
 }
 cbFacDiap->Items->EndUpdate();
 cbFacDiap->Sorted=true;
 cbFacDiap->ItemIndex=cbFacDiap->Items->IndexOfObject((TObject*)facdiap);

 MakeTargetList();
 s.SetLength(0);
 AUnit *un=ord->orders->unit;
 AObject *obj=un->endobj;
 if(obj->num==0)
   s="Error: outside object";
 else if(obj->GetOwner()!=un)
   s="Error: is not owner";
 labError->Caption=s;
}
__fastcall TEditOrderPromoteForm::~TEditOrderPromoteForm()
{
  delete slist;
}
struct SortItem{
 int num;
 AUnit *unit;
};
static int SortItemCompare(const SortItem &a,const SortItem &b){
 return a.num<b.num;
}
void TEditOrderPromoteForm::MakeTargetList()
{
 AnsiString s;

 vector<SortItem> list;
 int itemcount=0;
 AFaction *basefac=ord->orders->unit->faction;
 foreach(targets){
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
 else if(facdiap==-3)
 {
   int ind=cbFacDiap->Items->IndexOfObject((TObject*)facdiap);
   cbFacDiap->Items->Delete(ind);
   facdiap=-2;
   cbFacDiap->ItemIndex=cbFacDiap->Items->IndexOfObject((TObject*)facdiap);
   MakeTargetList();
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
  slist->AddObject(s,(TObject*)un);
  if(un->num==curtarnum){
    cbTargets->Text=s;
  }
 }
 cbTargets->Items=slist;
 cbTargetsChange(0);
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderPromoteForm::bnOkClick(TObject *Sender)
{
 ord->commented=cbCommented->Checked;
 ord->repeating=cbRepeating->Checked;

 int indtar=cbTargets->Items->IndexOf(cbTargets->Text);
 AUnit *un=0;
 if(indtar<0)
 {
  AnsiString str=cbTargets->Text;
  if(str.Length())
  {
    if(isdigit(str[1])&&(str.Length()<2||isdigit(str[str.Length()-1])))
    {
      int num=atoi(str.c_str());
      un=targets->GetUnit(num);
    }
  }
  if(!un)
  {
    if(ord->value==0)
      ord=0;
    return;
  }
 }else
  un=(AUnit*)cbTargets->Items->Objects[indtar];
 ord->value=un->num;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderPromoteForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderPromoteForm::cbTargetsChange(TObject *Sender)
{
 AnsiString str=cbTargets->Text;
 int indtar=cbTargets->Items->IndexOf(str);
 if(indtar==-1){
   TStrings *list=cbTargets->Items;
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
     int i=cbTargets->SelStart;
     list->Assign(slist);
     cbTargets->SelStart=i;
   }
   return;
 }
 AUnit *un=(AUnit*)cbTargets->Items->Objects[indtar];
 curtarnum=un->num;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderPromoteForm::cbFacDiapChange(TObject *Sender)
{
  int ind=cbFacDiap->ItemIndex;
  if(ind<0) return;
  ind=(int)cbFacDiap->Items->Objects[ind];
  if(facdiap==ind) return;
  facdiap=ind;
  MakeTargetList();
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderPromoteForm::cbFacDiapEnter(TObject *Sender)
{
  facnum_search=0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderPromoteForm::cbFacDiapKeyPress(TObject *Sender,
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

