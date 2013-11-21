//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "EditSkillTypeFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditSkillTypeForm *EditSkillTypeForm;
//---------------------------------------------------------------------------
__fastcall TEditSkillTypeForm::TEditSkillTypeForm(TComponent* Owner,ASkillType*oldskill,ASkillType*newskill)
    : TForm(Owner),list(new TStringList)
{
 FoldSkill=oldskill;
 FnewSkill=newskill;
 if(!oldskill)return;
 if(!SkillTypes)return;
 SkillTypes->MakeSkillList(list);
 int ind=list->IndexOf(oldskill->name);
 if(ind!=-1)list->Delete(ind);
 list->Sort();
}
//---------------------------------------------------------------------------
__fastcall TEditSkillTypeForm::~TEditSkillTypeForm()
{
 delete list;
}
void __fastcall TEditSkillTypeForm::FormCreate(TObject *Sender)
{
 if(!FoldSkill)return;
 if(!SkillTypes)return;
 Table->ColCount=11;
 Table->ColWidths[0]=25;
 Table->Cells[1][0]="Name";
 Table->ColWidths[1]=100;
 Table->Cells[2][0]="Abr";
 Table->ColWidths[2]=35;
 Table->Cells[3][0]="Cost";
 Table->ColWidths[3]=30;
 Table->Cells[4][0]="Magic";
 Table->ColWidths[4]=35;
 Table->Cells[5][0]="Comb";
 Table->ColWidths[5]=35;
 Table->Cells[6][0]="Cast";
 Table->ColWidths[6]=35;
 Table->Cells[7][0]="Found";
 Table->ColWidths[7]=35;
 Table->Cells[8][0]="Depend1";
 Table->ColWidths[8]=80;
 Table->Cells[9][0]="Depend2";
 Table->ColWidths[9]=80;
 Table->Cells[10][0]="Depend3";
 Table->ColWidths[10]=80;

 Table->Cells[1][1]=FoldSkill->name;
 Table->Cells[2][1]=FoldSkill->abr;
 Table->Cells[3][1]=FoldSkill->cost;
 Table->Cells[4][1]=FoldSkill->flags&ASkillType::MAGIC?"   x":"";
 Table->Cells[5][1]=FoldSkill->flags&ASkillType::COMBAT?"   x":"";
 Table->Cells[6][1]=FoldSkill->flags&ASkillType::CAST?"   x":"";
 Table->Cells[7][1]=FoldSkill->flags&ASkillType::FOUNDATION?"   x":"";
 Table->Cells[8][1]=MakeDepSkillStr(FoldSkill->depends[0],FoldSkill->levels[0]);
 Table->Cells[9][1]=MakeDepSkillStr(FoldSkill->depends[1],FoldSkill->levels[1]);
 Table->Cells[10][1]=MakeDepSkillStr(FoldSkill->depends[2],FoldSkill->levels[2]);
 if(FnewSkill){
  Table->RowCount=3;
  Table->Cells[0][1]="Cur";
  Table->Cells[0][2]="New";
  Table->Cells[1][2]=FnewSkill->name;
  Table->Cells[2][2]=FnewSkill->abr;
  Table->Cells[3][2]=FnewSkill->cost;
  Table->Cells[4][2]=FnewSkill->flags&ASkillType::MAGIC?"   x":"";
  Table->Cells[5][2]=FnewSkill->flags&ASkillType::COMBAT?"   x":"";
  Table->Cells[6][2]=FnewSkill->flags&ASkillType::CAST?"   x":"";
  Table->Cells[7][2]=FnewSkill->flags&ASkillType::FOUNDATION?"   x":"";
  Table->Cells[8][2]=MakeDepSkillStr(FnewSkill->depends[0],FnewSkill->levels[0]);
  Table->Cells[9][2]=MakeDepSkillStr(FnewSkill->depends[1],FnewSkill->levels[1]);
  Table->Cells[10][2]=MakeDepSkillStr(FnewSkill->depends[2],FnewSkill->levels[2]);

 }
 PrepareName(FoldSkill);
 PrepareAbr(FoldSkill);
 PrepareCost(FoldSkill);
 PrepareMagic(FoldSkill);
 PrepareCombat(FoldSkill);
 PrepareCast(FoldSkill);
 PrepareFoundation(FoldSkill);
 PrepareDep1(FoldSkill);
 PrepareDep2(FoldSkill);
 PrepareDep3(FoldSkill);
}
void __fastcall TEditSkillTypeForm::FormShow(TObject *Sender)
{
 if(!FoldSkill||!SkillTypes){ModalResult=mrCancel;return;}
}
void TEditSkillTypeForm::PrepareName(ASkillType* stype)
{
 edName->Text=stype->name;
}
void TEditSkillTypeForm::PrepareAbr(ASkillType* stype)
{
 edAbr->Text=stype->abr;
}
void TEditSkillTypeForm::PrepareCost(ASkillType* stype)
{
 edCost->Text=stype->cost;
}
void TEditSkillTypeForm::PrepareMagic(ASkillType* stype)
{
 cbMagic->Checked=stype->flags&ASkillType::MAGIC;
}
void TEditSkillTypeForm::PrepareCombat(ASkillType* stype)
{
 cbCombat->Checked=stype->flags&ASkillType::COMBAT;
}
void TEditSkillTypeForm::PrepareCast(ASkillType* stype)
{
 cbCast->Checked=stype->flags&ASkillType::CAST;
}
void TEditSkillTypeForm::PrepareFoundation(ASkillType* stype)
{
 cbFoundation->Checked=stype->flags&ASkillType::FOUNDATION;
}
void TEditSkillTypeForm::PrepareDep1(ASkillType* stype)
{
 cbDep1->Items->Clear();
 cbDep1->Items->Add("None");
 cbDep1->Items->AddStrings(list);
 int ind=cbDep1->Items->IndexOf(SkillTypes->AbrToName(stype->depends[0]));
 if(ind==-1)ind=0;
 cbDep1->ItemIndex=ind;
 if(cbDep1->ItemIndex!=0){
//  cbLev1->Visible=true;
  cbLev1->ItemIndex=stype->levels[0]-1;
 }/*else{
  cbLev1->Visible=false;
  cbLev1->ItemIndex=0;
  cbDep2->Visible=false;
 }*/
 cbDep1Change(this);
}
void TEditSkillTypeForm::PrepareDep2(ASkillType* stype)
{
 cbDep2->Items->Clear();
 cbDep2->Items->Add("None");
 cbDep2->Items->AddStrings(list);
 if(cbDep2->Visible==false){cbDep2->ItemIndex=0;return;}
 int ind=cbDep2->Items->IndexOf(SkillTypes->AbrToName(stype->depends[1]));
 if(ind==-1)ind=0;
 cbDep2->ItemIndex=ind;
 if(cbDep2->ItemIndex!=0){
//  cbLev2->Visible=true;
  cbLev2->ItemIndex=stype->levels[1]-1;
 }/*else{
  cbLev2->Visible=false;
  cbLev2->ItemIndex=0;
  cbDep3->Visible=false;
 }*/
 cbDep2Change(this);
}
void TEditSkillTypeForm::PrepareDep3(ASkillType* stype)
{
 cbDep3->Items->Clear();
 cbDep3->Items->Add("None");
 cbDep3->Items->AddStrings(list);
 if(cbDep3->Visible==false){cbDep3->ItemIndex=0;return;}
 int ind=cbDep3->Items->IndexOf(SkillTypes->AbrToName(stype->depends[2]));
 if(ind==-1)ind=0;
 cbDep3->ItemIndex=ind;
 if(cbDep3->ItemIndex!=0){
//  cbLev3->Visible=true;
  cbLev3->ItemIndex=stype->levels[2]-1;
 }/*else{
  cbLev3->Visible=false;
  cbLev3->ItemIndex=0;
 }*/
 cbDep3Change(this);
}
void __fastcall TEditSkillTypeForm::cbDep1Change(TObject *Sender)
{
 if(cbDep1->ItemIndex<=0){
  cbLev1->Visible=false;
  cbLev1->ItemIndex=0;
  cbDep2->ItemIndex=0;
  cbDep2Change(Sender);
  cbDep2->Visible=false;
 }else{
  cbLev1->Visible=true;
  cbDep2->Visible=true;
 }
}
//---------------------------------------------------------------------------

void __fastcall TEditSkillTypeForm::cbDep2Change(TObject *Sender)
{
 if(cbDep2->ItemIndex<=0){
  cbLev2->Visible=false;
  cbLev2->ItemIndex=0;
  cbDep3->ItemIndex=0;
  cbDep3Change(Sender);
  cbDep3->Visible=false;
 }else{
  cbLev2->Visible=true;
  cbDep3->Visible=true;
 }
}
//---------------------------------------------------------------------------

void __fastcall TEditSkillTypeForm::cbDep3Change(TObject *Sender)
{
 if(cbDep3->ItemIndex<=0){
  cbLev3->Visible=false;
  cbLev3->ItemIndex=0;
 }else{
  cbLev3->Visible=true;
 }
}
void __fastcall TEditSkillTypeForm::bnOkClick(TObject *Sender)
{
 FoldSkill->name=edName->Text;
 FoldSkill->abr=edAbr->Text;
 FoldSkill->cost=edCost->Text.ToInt();
 FoldSkill->flags=0;
 if(cbMagic->Checked)FoldSkill->flags|=ASkillType::MAGIC;
 if(cbCast->Checked)FoldSkill->flags|=ASkillType::CAST;
 if(cbCombat->Checked)FoldSkill->flags|=ASkillType::COMBAT;
 if(cbFoundation->Checked)FoldSkill->flags|=ASkillType::FOUNDATION;
 int ind;
 ind=cbDep1->ItemIndex;
 if(ind==0)FoldSkill->depends[0]="";
 else{
  FoldSkill->depends[0]=SkillTypes->NameToAbr(cbDep1->Items->Strings[ind]);
  FoldSkill->levels[0]=cbLev1->ItemIndex+1;
 }
 ind=cbDep2->ItemIndex;
 if(ind==0)FoldSkill->depends[1]="";
 else{
  FoldSkill->depends[1]=SkillTypes->NameToAbr(cbDep2->Items->Strings[ind]);
  FoldSkill->levels[1]=cbLev2->ItemIndex+1;
 }
 ind=cbDep3->ItemIndex;
 if(ind==0)FoldSkill->depends[2]="";
 else{
  FoldSkill->depends[2]=SkillTypes->NameToAbr(cbDep3->Items->Strings[ind]);
  FoldSkill->levels[2]=cbLev3->ItemIndex+1;
 }
}



void __fastcall TEditSkillTypeForm::TableDblClick(TObject *Sender)
{
 if(Table->Row==0||Table->Col==0)return;
 ASkillType *st=FoldSkill;
 if(Table->Row==2)st=FnewSkill;
 switch(Table->Col){
  case 1:
   PrepareName(st);
  break;
  case 2:
   PrepareAbr(st);
  break;
  case 3:
   PrepareCost(st);
  break;
  case 4:
   PrepareMagic(st);
  break;
  case 5:
   PrepareCombat(st);
  break;
  case 6:
   PrepareCast(st);
  break;
  case 7:
   PrepareFoundation(st);
  break;
  case 8:
   PrepareDep1(st);
  break;
  case 9:
   PrepareDep2(st);
  break;
  case 10:
   PrepareDep3(st);
  break;
 }
}
//---------------------------------------------------------------------------

