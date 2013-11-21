//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "util.h"
#include "orders.h"
#include "unit.h"
#include "region.h"
#include "EditOrderMovingFrm.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderMovingForm *EditOrderMovingForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderMovingForm::TEditOrderMovingForm(TComponent* Owner, AOrderMoving *_ord)
    : TForm(Owner),ord(_ord)
{
 Left=0;
 olddirs=new TList;
 for(int i=0;i<_ord->dirs->Count;i++){
  olddirs->Add(_ord->dirs->Items[i]);
 }
 avdirs=new TList;
 oldtype=_ord->type;
 switch(oldtype){
  case O_MOVE: cbType->ItemIndex=0; break;
  case O_ADVANCE: cbType->ItemIndex=1; break;
  case O_SAIL: cbType->ItemIndex=2; break;
 }
 oldcomm=_ord->commented;
 cbCommented->Checked=_ord->commented;
 cbRepeating->Checked=_ord->repeating;
 edCustom->Text=_ord->custom;
 cbUpdateMap->Checked=optUpdateMovingMap;
}
__fastcall TEditOrderMovingForm::~TEditOrderMovingForm()
{
 optUpdateMovingMap=cbUpdateMap->Checked;
 delete olddirs;
 delete avdirs;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderMovingForm::bnOkClick(TObject *Sender)
{
// ord->commented=cbCommented->Checked;
 ord->repeating=cbRepeating->Checked;
 ord->custom=edCustom->Text;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderMovingForm::bnCancelClick(TObject *Sender)
{
 ord->orders->unit->baseobj->basereg->runned=false;
 TList *dirs=ord->dirs;
 dirs->Clear();
 for(int i=0;i<olddirs->Count;i++){
  dirs->Add(olddirs->Items[i]);
 }
 ord->type=oldtype;
 ord->commented=oldcomm;
 ord=0;
}
void TEditOrderMovingForm::Show()
{
 cbRepeating->Visible=!preparemode;
 cbType->Visible=!preparemode;
 Label2->Visible=!preparemode;
 edCustom->Visible=!preparemode;

 AUnit *unit=ord->orders->unit;
 ord->isedited=true;
 if(preparemode){
  ord->Run(0);
 }else{
  DisableEvents();
  unit->baseobj->basereg->RunOrders(true);
  EnableEvents();
 }
 if(cbUpdateMap->Checked)
   AtlaForm->DrawMap();
 ord->isedited=false;
 int _type=ord->type;
 if(_type==O_AUTOMOVE)
   _type=ord->issail?O_SAIL:O_MOVE;
 AnsiString str;
 if(preparemode){
  str=_type==O_MOVE?"Moving":"Sailing";
  str+=" from ";
  str+=unit->baseobj->basereg->FullName(false);
  str+="\nto ";
  str+=unit->endobj->endreg->FullName(false);
  str+=".";
 }else{
  str="Located in ";
  if(unit->endobj->num){
   str+=unit->endobj->FullName();
   str+=" in ";
  }
  str+=unit->endobj->endreg->FullName(false);
  str+=".\nMovepoints: ";
  str+=unit->movepoints;
 }
 labLoc->Caption=str;

 int maxsize=0;
 lbDirs->Items->BeginUpdate();
 lbDirs->Items->Clear();
 for(int *iter=ord->List,*end=iter+ord->count;iter<end;iter++){
  int dir=*iter;
  if(dir<MOVE_IN){
   ADirType *dt=DirTypes->Get(dir);
   if(!dt){
    str="Bad direction";
   }else{
    str=dt->name;
   }
  }else if(dir==MOVE_IN){
   str="IN";
  }else if(dir==MOVE_OUT){
   str="OUT";
  }else{
   dir-=MOVE_OUT;
   str=dir;
  }
  lbDirs->Items->Add(str);
  TSize siz=lbDirs->Canvas->TextExtent(str);
  if(siz.cx>maxsize)maxsize=siz.cx;
 }
 lbDirs->Items->EndUpdate();
 lbDirs->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);

 maxsize=0;
 lbAvDirs->Items->BeginUpdate();
 lbAvDirs->Items->Clear();
 avdirs->Clear();

 AObject *obj=unit->endobj;
 ARegion *reg=obj->endreg;
 int dir,baddir=-1;
 bool fullmove=unit->movepoints>=0;
 if(ord->commented) fullmove=false;
// if(!(reg->infoStatus&isExitsInfo)) fullmove=false;
 if(ord->count){
  baddir=ord->Get(ord->count-1);
  if(baddir<3) baddir+=3;
  else if(baddir<6) baddir-=3;
  else baddir=-1;
 }
 for(dir=0;dir<NDIRS;dir++){
  ADirType *dt=DirTypes->Get(dir);
  if(reg->hasInfo&&(fullmove||preparemode)){
   if(dir==baddir) continue;
   ARegion *newreg=curRegionList->Get(reg->neighbors[dir]);
   if(!newreg) continue;
   if(_type==O_SAIL){
     if(obj->type!="Balloon")
     {
       if(!newreg->IsCoastal()) continue;
       if(!TerrainTypes->IsOcean(reg->type)&&!TerrainTypes->IsOcean(newreg->type)) continue;
     }
   }else{
    if(TerrainTypes->IsOcean(newreg->type)&&
	 (!unit->CanSwim()||unit->GetEndFlag(FLAG_NOCROSS_WATER)))
     continue;
   }
   str=dt->abr;
   str+=": ";
   if(!newreg->hasInfo)
     str+="?";
   str+=newreg->FullName(false);
  }else{
   str=dt->name;
  }
  avdirs->Add((void*)dir);
  lbAvDirs->Items->Add(str);
  TSize siz=lbDirs->Canvas->TextExtent(str);
  if(siz.cx>maxsize)maxsize=siz.cx;
 }
 if(fullmove&&_type!=O_SAIL){
  if(obj->inner){
   str="IN";
   if(obj->innerloc.xloc==-1){
    str+=" (not linked)";
   }else{
    str+=" (";
    ARegion *rg=curRegionList->Get(obj->innerloc);
    str+=rg->FullName(false);
    str+=")";
   }
   avdirs->Add((void*)MOVE_IN);
   lbAvDirs->Items->Add(str);
   TSize siz=lbDirs->Canvas->TextExtent(str);
   if(siz.cx>maxsize)maxsize=siz.cx;
  }
  if(obj->num){
   str="OUT";
   avdirs->Add((void*)MOVE_OUT);
   lbAvDirs->Items->Add(str);
   TSize siz=lbDirs->Canvas->TextExtent(str);
   if(siz.cx>maxsize)maxsize=siz.cx;
  }
  foreach(reg){
   AObject *o=*(AObject**)iter;
   if(!o->num) continue;
   if(o->num==obj->num) continue;
   AObjectType *ot=ObjectTypes->Get(o->type);
   if(ot &&ot->canenter==false) continue;
   if(preparemode&&(!o->inner||o->innerloc.xloc==-1)) continue;
   str=o->FullName();
   dir=MOVE_OUT+o->num;
   avdirs->Add((void*)dir);
   lbAvDirs->Items->Add(str);
   TSize siz=lbDirs->Canvas->TextExtent(str);
   if(siz.cx>maxsize)maxsize=siz.cx;
  }
 }

 lbAvDirs->Items->EndUpdate();
 lbAvDirs->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
}

void __fastcall TEditOrderMovingForm::bnDeleteClick(TObject *Sender)
{
 int pos=ord->count-1;
 if(pos<0) return;
 ord->Delete(pos);
 Show();   
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderMovingForm::lbAvDirsDblClick(TObject *Sender)
{
 int ind=lbAvDirs->ItemIndex;
 if(ind<0) return;
 int dir=(int)avdirs->Items[ind];
 ord->Add(dir);
 Show();
}
//---------------------------------------------------------------------------

void __fastcall TEditOrderMovingForm::cbCommentedClick(TObject *Sender)
{
 ord->commented=cbCommented->Checked;
 Show();
}
//---------------------------------------------------------------------------

void __fastcall TEditOrderMovingForm::cbTypeChange(TObject *Sender)
{
 switch(cbType->ItemIndex){
  case 0: ord->type=O_MOVE; break;
  case 1: ord->type=O_ADVANCE; break;
  case 2: ord->type=O_SAIL; break;
 }
 Show();
}
//---------------------------------------------------------------------------

void __fastcall TEditOrderMovingForm::Action1Execute(TObject *Sender)
{
 preparemode=!preparemode;
 Show();
}
//---------------------------------------------------------------------------

void __fastcall TEditOrderMovingForm::FormShow(TObject *Sender)
{
 Show();
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderMovingForm::FormKeyPress(TObject *Sender,
      char &Key)
{
  if(ActiveControl==edCustom) return;
  if(Key==0x8)
  {
    bnDelete->Click();
    return;
  }
  int dir=-1;
  switch(Key)
  {
    case '8': dir=0; break;
    case '9': dir=1; break;
    case '3': dir=2; break;
    case '2': dir=3; break;
    case '1': dir=4; break;
    case '7': dir=5; break;
  }
  if(dir<0) return;
  Key=0;
  int ind=avdirs->IndexOf((void*)dir);
  if(ind<0) return;
  ord->Add(dir);
  Show();
}
//---------------------------------------------------------------------------

