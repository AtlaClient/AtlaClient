//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <algorithm>
#include "util.h"
#include "orders.h"
#include "unit.h"
#include "skills.h"
#include "EditOrderTeachFrm.h"
#include "gameparams.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderTeachForm *EditOrderTeachForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderTeachForm::TEditOrderTeachForm(TComponent* Owner, AOrderTeach *_ord, AUnits *_units)
    : TForm(Owner),ord(_ord)
{
 maxstudents=ord->orders->unit->GetMen()*TeachMaxStudents;
 cbCommented->Checked=ord->commented;
 cbRepeating->Checked=ord->repeating;
 targets=new AUnits(ord->orders->unit->baseobj);
 targets->autodelete=false;
 for(int i=0;i<ord->targets->Count;i++){
  AUnitPtr *ptr=ord->GetTarget(i);
  targets->Add(ptr->ptr);
 }
 units.reserve(_units->count);
 foreach(_units)
 {
   AUnit *un=*(AUnit**)iter;
   TUnitInfo ui;
   ui.un=un;
   ui.str=PrintUnit(un);
   ui.sortord=GetSortOrd(un);
   units.push_back(ui);
 }
 stable_sort(units.begin(),units.end());
 MakeLists();
}
void __fastcall TEditOrderTeachForm::bnOkClick(TObject *Sender)
{
 ord->commented=cbCommented->Checked;
 ord->repeating=cbRepeating->Checked;
 ord->Clear();
 foreach(targets){
  AUnit *un=*(AUnit**)iter;
  ord->AddTarget(un->num,un);
 }
}
void __fastcall TEditOrderTeachForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}
void TEditOrderTeachForm::MakeLists()
{
 lbTargets->Items->BeginUpdate();
 lbTargets->Items->Clear();
 int maxsize=0;
 int students=0;
 foreach(targets){
  AUnit *un=*(AUnit**)iter;
  AnsiString s=PrintUnit(un);
  students+=un->GetMen();
  lbTargets->Items->Add(s);
  TSize siz=lbTargets->Canvas->TextExtent(s);
  if(siz.cx>maxsize)maxsize=siz.cx;
 }
 lbTargets->Items->EndUpdate();
 lbTargets->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
 maxsize=0;
 lbUnits->Items->BeginUpdate();
 lbUnits->Items->Clear();
 for(TUnitInfo *ui=units.begin().operator ->(),*endui=units.end().operator ->();ui<endui;ui++)
 {
  AUnit *un=ui->un;
  if(targets->GetUnit(un->num))continue;
  AnsiString s=ui->str;
  lbUnits->Items->Add(s);
  TSize siz=lbUnits->Canvas->TextExtent(s);
  if(siz.cx>maxsize)maxsize=siz.cx;
 }
 lbUnits->Items->EndUpdate();
 lbUnits->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
 AnsiString s="Teaching ";
 s+=students;
 s+=" of ";
 s+=maxstudents;
 s+=" students";
 Label1->Caption=s;
}
void __fastcall TEditOrderTeachForm::FormDestroy(TObject *Sender)
{
 delete targets;
}
void __fastcall TEditOrderTeachForm::bnAddClick(TObject *Sender)
{
 int ind=lbUnits->ItemIndex;
 if(ind==-1)return;
 AnsiString str=lbUnits->Items->Strings[ind];
 for(TUnitInfo *ui=units.begin().operator ->(),*endui=units.end().operator ->();ui<endui;ui++)
 {
  if(str==ui->str){
   targets->Add(ui->un);
   MakeLists();
   return;
  }
 }
}
void __fastcall TEditOrderTeachForm::bnDeleteClick(TObject *Sender)
{
 int ind=lbTargets->ItemIndex;
 if(ind==-1)return;
 targets->DeleteByIndex(ind);
 MakeLists();
}
AnsiString TEditOrderTeachForm::PrintUnit(AUnit * un)
{
 AnsiString s;
 s=un->GetMen();
 s+=":";
 s+=un->FullName();
 AOrderAbr *sord=dynamic_cast<AOrderAbr*>(un->orders->Find(O_STUDY,2));
 if(!sord)return s;
 s+=" ";
 AnsiString abr=sord->abr;
 ASkill sk;
 sk.SetType(abr);
 sk.days=un->GetSkillDays(sk.type->abr);
 s+=sk.Print();
 int maxdays=ASkill::LevelToDays(un->GetMaxLevel(abr));
 if(sk.days>=maxdays-StudyAddDays) s="! "+s;
 else if(ord->orders->unit->GetRealSkill(abr)<=un->GetRealSkill(abr)) s="!!"+s;
 if(sord->number>=un->GetMen()*(TeachAddDays+StudyAddDays)) s="* "+s;
 return s;
}
int TEditOrderTeachForm::GetSortOrd(AUnit * un)
{
 AOrderAbr *sord=dynamic_cast<AOrderAbr*>(un->orders->Find(O_STUDY,2));
 if(!sord)return -1;
 ASkillType *stype=SkillTypes->Get(sord->abr);
 if(!stype) return -1;
 int ord=stype->sortindex;
 int days=un->GetSkillDays(sord->abr);
 int maxdays=ASkill::LevelToDays(un->GetMaxLevel(sord->abr));
 if(days>=maxdays-StudyAddDays) ord+=SkillTypes->count*2;
 if(sord->number>=un->GetMen()*(TeachAddDays+StudyAddDays)) ord+=SkillTypes->count;
 return ord;
}

