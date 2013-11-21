//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdlib.h>
#include <vector.h>
#include <algorithm>
#include "util.h"
#include "orders.h"
#include "items.h"
#include "unit.h"
#include "EditOrderGiveFrm.h"
#include "faction.h"
#include "exprlang.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderGiveForm *EditOrderGiveForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderGiveForm::TEditOrderGiveForm(TComponent* Owner, AOrderGive *_ord, AItems *its, AUnits *tars)
    : TForm(Owner),ord(_ord),items(its),targets(tars)
    ,slist(new TStringList)
    ,facnum_search(0)
{
 cbCommented->Checked=ord->commented;
 cbRepeating->Checked=ord->repeating;

 if(ord->count>=0)
  edCount->Text=ord->count;
 else
  edCount->Text=0; 
 curtarnum=ord->tarnum;
 curitem=ord->item;
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
     if(!fac->local)
       facdiap=-4;
   int ind=cbFacDiap->Items->IndexOfObject((TObject*)facnum);
   if(ind<0)
     cbFacDiap->Items->AddObject(fac->FullName(),(TObject*)facnum);
 }
 cbFacDiap->Items->EndUpdate();
 cbFacDiap->Sorted=true;
 cbFacDiap->ItemIndex=cbFacDiap->Items->IndexOfObject((TObject*)facdiap);

 cbItems->Items->BeginUpdate();
 cbItems->Items->Add("Full Unit");
 if(!ord->item.Length()) cbItems->ItemIndex=0;
 foreach(items){
  AItem *it=*(AItem**)iter;
  AItemType *itype=it->type;//ItemTypes->Get(it->type);
  if(itype->flags&AItemType::CANTGIVE) continue;
  s=itype->name;
  s+=" (max ";
  s+=it->count;
  s+=")";
  cbItems->Items->AddObject(s,(TObject*)itype);
  if(it->type->abr==curitem){
   cbItems->ItemIndex=cbItems->Items->Count-1;
  }
 }
 cbItems->Items->EndUpdate();
 
 cbDiscard->Checked=curtarnum==0;
 if(cbItems->ItemIndex<=0)
   MakeTargetList();
 cbItemsChange(this);
 cbAllClick(0);
 if(ord->count==-2){
  cbAll->Checked=true;
  if(ord->except){
   edCount->Text=ord->except;
   cbExcept->Checked=true;
  }
 }
 if(!optServGiveAll&&ord->count!=-2){
  cbAll->Visible=false;
  cbExcept->Visible=false;
 }
}
__fastcall TEditOrderGiveForm::~TEditOrderGiveForm()
{
  delete slist;
}
struct SortItem{
 int num;
 AUnit *unit;
};
static int SortItemCompare(const SortItem &a,const SortItem &b){
 return a.num>b.num;
}
void TEditOrderGiveForm::MakeTargetList()
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
  si.num=un->enditems->GetNum(curitem);
  if(si.num) itemcount+=si.num;
  list.push_back(si);
 }
 if(itemcount)
  stable_sort(list.begin(),list.end(),SortItemCompare);
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
  s+=si.num; //un->PrintForTree()
  slist->AddObject(s,(TObject*)un);
  if(un->num==curtarnum){
    cbTargets->Text=s;
  }
 }
 cbTargets->Items=slist;
 cbTargetsChange(0);
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderGiveForm::bnOkClick(TObject *Sender)
{
 if(!CalcValues()){
   ModalResult=mrNone;
   return;
 }
 ord->commented=cbCommented->Checked;
 ord->repeating=cbRepeating->Checked;
 ord->count=atoi(edCount->Text.c_str());
 if(cbAll->Checked){
  if(cbExcept->Checked)
   ord->except=ord->count;
  else
   ord->except=0;
  ord->count=-2;
 }

 int inditem=cbItems->ItemIndex;
 if(inditem!=-1){
  AItemType *itype=(AItemType*)cbItems->Items->Objects[inditem];
  if(itype){
   ord->item=itype->abr;
  }else ord->item="";
 }

 if(cbDiscard->Checked){
  if(ord->target!=0){
   ord->tarnum=0;
   ord->DeleteMessage(ord->message);
   ord->target=0;
  }else ord->tarnum=0;
  return;
 }
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
      if(!un)
      {
        if(ord->tarnum!=num)
        {
          ord->tarnum=num;
          ord->DeleteMessage(ord->message);
          ord->target=0;
        }
        return;
      }
    }
  }
  if(!un)
    return;
 }else
  un=(AUnit*)cbTargets->Items->Objects[indtar];
 if(ord->target!=un){
  ord->tarnum=un->num;
  ord->DeleteMessage(ord->message);
  ord->target=un;
 }
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderGiveForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}
void __fastcall TEditOrderGiveForm::edCountExit(TObject *Sender)
{
 CalcValues();
}
void __fastcall TEditOrderGiveForm::cbItemsChange(TObject *Sender)
{
  int ind=cbItems->ItemIndex;
  edCount->Visible=(ind>0)&&(!cbAll->Checked||cbExcept->Checked);
  bnAll->Visible=(ind>0);
  cbAll->Enabled=(ind>0);
  if(ind<=0) return;
  AItemType *itype=(AItemType*)cbItems->Items->Objects[ind];
  if(itype) curitem=itype->abr; else curitem="";
  MakeTargetList();
  ShowCapacity();
}
void __fastcall TEditOrderGiveForm::bnAllClick(TObject *Sender)
{
 int ind=cbItems->ItemIndex;
 if(ind<=0) return;
 AItemType *itype=(AItemType*)cbItems->Items->Objects[ind];
 if(!itype) return;
 edCount->Text=items->GetNum(itype->abr);
}
void __fastcall TEditOrderGiveForm::cbTargetsChange(TObject *Sender)
{
 AnsiString str=cbTargets->Text;
 int indtar=cbTargets->Items->IndexOf(str);
 if(indtar==-1){
   TStrings *list=cbTargets->Items;
   if(!str.Length())
     list->Clear();
   else
     cbDiscard->Checked=false;
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
 cbDiscard->Checked=false;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderGiveForm::cbAllClick(TObject *Sender)
{
 bool b=cbAll->Checked;
 cbExcept->Enabled=b;
 if(!b){
  cbExcept->Checked=false;
 }
 if(!cbAll->Enabled)
   return;
 edCount->Visible=!b||cbExcept->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TEditOrderGiveForm::cbExceptClick(TObject *Sender)
{
 edCount->Visible=!cbAll->Checked||cbExcept->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TEditOrderGiveForm::cbFacDiapChange(TObject *Sender)
{
  int ind=cbFacDiap->ItemIndex;
  if(ind<0) return;
  ind=(int)cbFacDiap->Items->Objects[ind];
  if(facdiap==ind) return;
  facdiap=ind;
  MakeTargetList();
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderGiveForm::cbFacDiapEnter(TObject *Sender)
{
  facnum_search=0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderGiveForm::cbFacDiapKeyPress(TObject *Sender,
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
bool TEditOrderGiveForm::CalcValues()
{
  AnsiString str=edCount->Text;
  if(!str.Length()) return false;
  ExprSetUnitMode(ord->orders->unit);
  try{
    int val=ExprProcessExpr(str);
    edCount->Text=val;
  }catch (Exception &exception){
    Application->ShowException(&exception);
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
void TEditOrderGiveForm::ShowCapacity(){
  AnsiString str;

  labCapacity->Caption=str;

}
//---------------------------------------------------------------------------

