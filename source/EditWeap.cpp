//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "EditWeap.h"
#include "util.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfEditWeap *fEditWeap;
//---------------------------------------------------------------------------
__fastcall TfEditWeap::TfEditWeap(TComponent* Owner,
                   AWeaponType *weap, AnsiString descr): TForm(Owner)
{
  if(!weap){
    bNewWeap = true;
    Fweap = new AWeaponType;
  } else {
    bNewWeap = false;
    Fweap = weap;
  }

  Memo->Visible = descr.Length();
  Memo->Lines->Text = descr;
}
//---------------------------------------------------------------------------

void __fastcall TfEditWeap::FormCreate(TObject *Sender)
{
  TStringList*list = new TStringList;

  list->Clear();
  SkillTypes->MakeSkillList(list);
  list->Sort();
  cbSkill->Items->Clear();
  cbSkill->Items->Add("None");
  cbSkill->Items->AddStrings(list);

  cbSkill2->Items->Clear();
  cbSkill2->Items->Add("None");
  cbSkill2->Items->AddStrings(list);
  delete list;

  int ind = cbSkill->Items->IndexOf(SkillTypes->AbrToName(Fweap->skill1));
  if(ind == -1)ind = 0;
  cbSkill->ItemIndex = ind;
  cbSkillChange(this);

  if(cbSkill2->Visible){
    ind = cbSkill2->Items->IndexOf(SkillTypes->AbrToName(Fweap->skill2));
    if(ind == -1)ind = 0;
    cbSkill2->ItemIndex = ind;
  } else cbSkill2->ItemIndex = 0;

  cbWeapClass->ItemIndex = Fweap->weapClass;
  cbAttType->ItemIndex = Fweap->attackType;
  edAttBonus->Text = Fweap->attackBonus;
  edDefBonus->Text = Fweap->defenseBonus;
  edMountBonus->Text = Fweap->mountBonus;
  if(!Fweap->numAttacks) Fweap->numAttacks = 1;
  if(Fweap->numAttacks < 0){
    edNumAtt->Text = "1";
    edPerRound->Text = (-1*Fweap->numAttacks);
  } else {
    edNumAtt->Text = Fweap->numAttacks;
    edPerRound->Text = "1";;
  }
  cbALWAYSREADY->Checked = Fweap->flags&AWeaponType::ALWAYSREADY;
  cbNODEFENSE->Checked = Fweap->flags&AWeaponType::NODEFENSE;
  cbNOFOOT->Checked = Fweap->flags&AWeaponType::NOFOOT;
  cbNOMOUNT->Checked = Fweap->flags&AWeaponType::NOMOUNT;
  cbSHORT->Checked = Fweap->flags&AWeaponType::SHORT;
  cbLONG->Checked = Fweap->flags&AWeaponType::LONG;
  cbRANGED->Checked = Fweap->flags&AWeaponType::RANGED;
  cbNOATTACKERSKILL->Checked = Fweap->flags&AWeaponType::NOATTACKERSKILL;
  cbRIDINGBONUS->Checked = Fweap->flags&AWeaponType::RIDINGBONUS;
  cbRIDINGBONUSDEFENSE->Checked = Fweap->flags&AWeaponType::RIDINGBONUSDEFENSE;
}
//---------------------------------------------------------------------------

void __fastcall TfEditWeap::cbSkillChange(TObject *Sender)
{
  lbSk2->Visible = (cbSkill->ItemIndex > 0);
  cbSkill2->Visible = lbSk2->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TfEditWeap::bnCancelClick(TObject *Sender)
{
  if(bNewWeap) delete Fweap;
}
//---------------------------------------------------------------------------

void __fastcall TfEditWeap::bnOkClick(TObject *Sender)
{
  Fweap->flags = 0;

  int ind = cbSkill->ItemIndex;
  if(ind == 0) Fweap->skill1 = ""; Fweap->skill1 = SkillTypes->NameToAbr(cbSkill->Items->Strings[ind]);
  ind = cbSkill2->ItemIndex;
  if(cbSkill2->Visible && ind > 0) Fweap->skill2 = SkillTypes->NameToAbr(cbSkill2->Items->Strings[ind]);
    else Fweap->skill2 = "";

  Fweap->weapClass = cbWeapClass->ItemIndex;
  Fweap->attackType = cbAttType->ItemIndex;
  Fweap->attackBonus = edAttBonus->Text.ToInt();
  Fweap->defenseBonus = edDefBonus->Text.ToInt();
  Fweap->mountBonus = edMountBonus->Text.ToInt();
  if(edPerRound->Text == "1"){
    Fweap->numAttacks = edNumAtt->Text.ToInt();
  } else {
    Fweap->numAttacks = -1*edPerRound->Text.ToInt();
  }

  if(cbALWAYSREADY->Checked) Fweap->flags|= AWeaponType::ALWAYSREADY;
  if(cbNODEFENSE->Checked) Fweap->flags|= AWeaponType::NODEFENSE;
  if(cbNOFOOT->Checked) Fweap->flags|= AWeaponType::NOFOOT;
  if(cbNOMOUNT->Checked) Fweap->flags|= AWeaponType::NOMOUNT;
  if(cbSHORT->Checked) Fweap->flags|= AWeaponType::SHORT;
  if(cbLONG->Checked) Fweap->flags|= AWeaponType::LONG;
  if(cbRANGED->Checked) Fweap->flags|= AWeaponType::RANGED;
  if(cbNOATTACKERSKILL->Checked) Fweap->flags|= AWeaponType::NOATTACKERSKILL;
  if(cbRIDINGBONUS->Checked) Fweap->flags|= AWeaponType::RIDINGBONUS;
  if(cbRIDINGBONUSDEFENSE->Checked) Fweap->flags|= AWeaponType::RIDINGBONUSDEFENSE;

  if(bNewWeap) {
    Fweap->item = TekItem; 
    WeaponTypes->AddWeaponType(Fweap);
  }
}
//---------------------------------------------------------------------------

