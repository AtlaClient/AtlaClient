//todo:Mount : Need skill for riding; Spec.effects
//     Mag.production
//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "EditItems.h"
#include "GetItem.h"
#include "GetSkilLv.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TfEditItems *fEditItems;
//---------------------------------------------------------------------------
__fastcall TfEditItems::TfEditItems(TComponent* Owner,AItemType*oldItem
    ,AItemType*newItem,AnsiString descr)
        : TForm(Owner)
{
  FoldItem = oldItem;
  FnewItem = newItem;
  if(!oldItem) return;
  if(!ItemTypes) return;
  if(!SkillTypes) return;
  Memo->Visible = descr.Length();
  Memo->Lines->Text = descr;
  FAbr = oldItem->abr;

  FArmor = ArmorTypes->Get(FAbr);
  if(!FArmor) FArmor = new AArmorType();

  FWeapon = WeaponTypes->Get(FAbr);
  if(!FWeapon) FWeapon = new AWeaponType();

  FMonster = MonstrTypes->Get(FAbr);
  if(!FMonster) FMonster = new AMonstrType();

  FMount = MountTypes->Get(FAbr);
  if(!FMount) FMount = new AMountType();

  FMan = ManTypes->Get(FAbr);
  if(!FMan) FMan = new AManType();

}
//---------------------------------------------------------------------------
void __fastcall TfEditItems::FormCreate(TObject *Sender)
{
  if(!FoldItem) return;
  if(!ItemTypes) return;
  if(!SkillTypes) return;

  AnsiString str;
  TStringList* list = new TStringList;

  ItemTypes->MakeItemList(list, AItemType::IT_TRADE|AItemType::IT_MONSTER|AItemType::IT_MAN);
  list->Sort();
  // For Adding Items;

  // Help Production
  cbHelpProd->Items->Clear();
  cbHelpProd->Items->Add("None");
  cbHelpProd->Items->AddStrings(list);
  cbHelpProd->ItemIndex = 0;

  int ind = list->IndexOf(FoldItem->name);
  if(ind != -1)list->Delete(ind);

  fGetItem->cbInput->Items->Clear();
  fGetItem->cbInput->Items->Add("None");
  fGetItem->cbInput->Items->AddStrings(list);

  list->Clear();
  SkillTypes->MakeSkillList(list);
  list->Sort();

  cbSkill->Items->Clear();
  cbSkill->Items->Add("None");
  cbSkill->Items->AddStrings(list);
  cbWskil1->Items->Assign(cbSkill->Items);
  cbWskil2->Items->Assign(cbSkill->Items);
  cbMountS->Items->Assign(cbSkill->Items);

  edName->Text = FoldItem->name;
  edNames->Text = FoldItem->names;
  edAbr->Text = FoldItem->abr;
  cbCantGive->Checked = (FoldItem->flags&AItemType::CANTGIVE)||ItemTypes->IsIllusionAbr(FoldItem->abr);
  cbCombat->Checked = FoldItem->combat;

  // Production - From Item Info
  ind = cbSkill->Items->IndexOf(SkillTypes->AbrToName(FoldItem->skill));
  if(ind == -1)ind=0;
  cbSkill->ItemIndex = ind;
  if(ind) cbLevel->ItemIndex = FoldItem->level-1; else cbLevel->ItemIndex = 0;
  cbSkillChange(this);

  //Inputs
  grInput->RowCount = 1;
  int numr = 0;
  for(int i=0; i<AItemType::INPUTCOUNT; i++){
    if(FoldItem->inputs[i].Length()){
      numr++;
      if(grInput->RowCount < numr) grInput->RowCount = numr;
      grInput->Cells[0][grInput->RowCount-1] = ItemTypes->AbrToName(FoldItem->inputs[i]);
      grInput->Cells[1][grInput->RowCount-1] = FoldItem->numbers[i];
    }
  }

  cbOrIn->Checked = (FoldItem->type&AItemType::IT_ORINPUTS);
  edOut->Value = FoldItem->out;
  edMonths->Value = FoldItem->months;

  edWeight->Value = FoldItem->weight;

  cbNormal->Checked = FoldItem->type&AItemType::IT_NORMAL;
  cbAdvanced->Checked = FoldItem->type&AItemType::IT_ADVANCED;
  cbTrade->Checked = FoldItem->type&AItemType::IT_TRADE;
  cbMagic->Checked = FoldItem->type&AItemType::IT_MAGIC;
  cbBattle->Checked = FoldItem->type&AItemType::IT_BATTLE;
  cbFood->Checked = FoldItem->type&AItemType::IT_FOOD;

  if(FoldItem->type&AItemType::IT_UNFINISHED) cbItType->ItemIndex = 6; else
  if(FoldItem->type&AItemType::IT_MAN) cbItType->ItemIndex = 1; else
  if(FoldItem->type&AItemType::IT_MONSTER) cbItType->ItemIndex = 2; else
  if(FoldItem->type&AItemType::IT_MOUNT) cbItType->ItemIndex = 3; else
  if(FoldItem->type&AItemType::IT_WEAPON) cbItType->ItemIndex = 4; else
  if(FoldItem->type&AItemType::IT_ARMOR) cbItType->ItemIndex = 5; else cbItType->ItemIndex = 0;

  ind = cbHelpProd->Items->IndexOf(ItemTypes->AbrToName(FoldItem->helpproditem));
  if(ind == -1) ind = 0;
  cbHelpProd->ItemIndex = ind;
  edHelpMult->Value = FoldItem->helpprodmult;
  // Mag.Prod - from ????

  // Armor
  cbAT->ItemIndex = FArmor->armorclass - 1;
  cbAss->Checked = (FArmor->flags&&AArmorType::USEINASS);
  edFrom->Text = FArmor->from;
  edAd1->Value = FArmor->saves[0];
  edAd2->Value = FArmor->saves[1];
  edAd3->Value = FArmor->saves[2];
  edAd4->Value = FArmor->saves[3];
  edAd5->Value = FArmor->saves[4];
  edAd6->Value = FArmor->saves[5];
  edAd7->Value = FArmor->saves[6];
  edAd8->Value = FArmor->saves[7];

  // Weapon
  ind = cbWskil1->Items->IndexOf(SkillTypes->AbrToName(FWeapon->skill1));
  if(ind == -1)ind = 0;
  cbWskil1->ItemIndex = ind;

  if(cbWskil2->Visible){
	ind = cbWskil2->Items->IndexOf(SkillTypes->AbrToName(FWeapon->skill2));
	if(ind == -1)ind = 0;
	cbWskil2->ItemIndex = ind;
  } else cbWskil2->ItemIndex = 0;

  cbWeapClass->ItemIndex = FWeapon->weapClass;
  cbAttType->ItemIndex = FWeapon->attackType;
  edAttBonus->Text = FWeapon->attackBonus;
  edDefBonus->Text = FWeapon->defenseBonus;
  edMountBonus->Text = FWeapon->mountBonus;

  if(!FWeapon->numAttacks) FWeapon->numAttacks = 1;
  if(FWeapon->numAttacks == -10)
  {
	 // Skill
	 cbAttPR->ItemIndex = 2;
  }
  else
  if(FWeapon->numAttacks == -11)
  {
	 // Skill
	 cbAttPR->ItemIndex = 3;
  }
  else
  if(FWeapon->numAttacks < 0)
  {
	cbAttPR->ItemIndex = 1;
	edNumAtt->Text = (-1*FWeapon->numAttacks);
  }
  else
  {
	edNumAtt->Text = FWeapon->numAttacks;
	cbAttPR->ItemIndex = 0;
  }

  cbALWAYSREADY->Checked = FWeapon->flags&AWeaponType::ALWAYSREADY;
  cbNODEFENSE->Checked = FWeapon->flags&AWeaponType::NODEFENSE;
  cbNOFOOT->Checked = FWeapon->flags&AWeaponType::NOFOOT;
  cbNOMOUNT->Checked = FWeapon->flags&AWeaponType::NOMOUNT;
  cbSHORT->Checked = FWeapon->flags&AWeaponType::SHORT;
  cbLONG->Checked = FWeapon->flags&AWeaponType::LONG;
  cbRANGED->Checked = FWeapon->flags&AWeaponType::RANGED;
  cbNOATTACKERSKILL->Checked = FWeapon->flags&AWeaponType::NOATTACKERSKILL;
  cbRIDINGBONUS->Checked = FWeapon->flags&AWeaponType::RIDINGBONUS;
  cbRIDINGBONUSDEFENSE->Checked = FWeapon->flags&AWeaponType::RIDINGBONUSDEFENSE;

  // Monster
  edMonComb->Value = FMonster->attackLevel;
  edMonTact->Value = FMonster->tactics;
  edMonAtts->Value = FMonster->numAttacks;
  edMonHits->Value = FMonster->hits;
  // !!!!!
  edMonRegen->Value = FMonster->regen;
  edMonStea->Value = FMonster->stea;
  edMonObse->Value = FMonster->obse;

  // !!!!
  // edMonAbil->ItemIndex = ????
  edMonAbilLev->Value = FMonster->specLevel;

  edMonD1->Value = FMonster->defense[0];
  edMonD2->Value = FMonster->defense[1];
  edMonD3->Value = FMonster->defense[2];
  edMonD4->Value = FMonster->defense[3];
  edMonD5->Value = FMonster->defense[4];
  edMonD6->Value = FMonster->defense[5];
  edMonD7->Value = FMonster->defense[6];
  edMonD8->Value = FMonster->defense[7];

  // Mount
  ind = cbMountS->Items->IndexOf(SkillTypes->AbrToName(FMount->skill));
  if(ind == -1)ind = 0;
  cbMountS->ItemIndex = ind;
  edMouMinB->Value = FMount->minBonus;
  edMouMaxB->Value = FMount->maxBonus;
  edMouHamB->Value = FMount->maxHamperedBonus;
  cbMouMonstr->Checked = FMount->monster;
  //!!!
  // cbMouSpec->ItemIndex

  // Man
  cbLeader->Checked = (FoldItem->type&AItemType::IT_LEADER);
  if(FMan->deflevel < 1) FMan->deflevel = 1;
  edManDefL->ItemIndex = (FMan->deflevel-1);

  numr = 1;
  grManSL->RowCount = 2;
  for(int i=0,endi=FMan->skills->Count;i<endi;i++)
  {
	numr++;
	if(grManSL->RowCount < numr) grManSL->RowCount = numr;
	grManSL->Cells[0][grManSL->RowCount-1] = SkillTypes->AbrToName(FMan->skills->Strings[i]);
	grManSL->Cells[1][grManSL->RowCount-1] = (int)FMan->skills->Objects[i];
  }

  // Capasity
  edWalk->Value = FoldItem->walk;
  edRide->Value = FoldItem->ride;
  edFly->Value  = FoldItem->fly;
  edSwim->Value = FoldItem->swim;

  cbItTypeChange(0);
}
//---------------------------------------------------------------------------

void __fastcall TfEditItems::cbSkillChange(TObject *Sender)
{
  PanelProd->Visible = cbSkill->ItemIndex;
  PanelInput->Visible = cbSkill->ItemIndex;
  cbLevel->Visible = cbSkill->ItemIndex;
  labLevel->Visible = cbSkill->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TfEditItems::cbItTypeChange(TObject *Sender)
{
  tbProd->TabVisible = (cbItType->ItemIndex != 1);
  tbMProd->TabVisible = false; // !!!!!!
  tbArmor->TabVisible = (cbItType->ItemIndex == 5);
  tbWeapon->TabVisible = (cbItType->ItemIndex == 4);
  tbMonster->TabVisible = (cbItType->ItemIndex == 2);
  tbMount->TabVisible = (cbItType->ItemIndex == 3);
  tbMan->TabVisible = (cbItType->ItemIndex == 1);
  tbMove->TabVisible = (cbItType->ItemIndex != 4) && (cbItType->ItemIndex != 5);
}
//---------------------------------------------------------------------------

void __fastcall TfEditItems::btAdd1Click(TObject *Sender)
{
  fGetItem->cbInput->ItemIndex = 0;
  fGetItem->edNum->Value = 1;
  fGetItem->btOk->Caption = "Add";
  if(fGetItem->ShowModal() == mrOk)
  {
     if(!fGetItem->cbInput->ItemIndex) return;
     // Вначе нуна проверить бы.....
     TStringGrid *grTek;
     if(((TComponent*)Sender)->Tag == 1) grTek = grInput;
		else grTek = grInputMP;

     // А потом добавить
     if(grTek->Cells[0][0] != "") grTek->RowCount = grTek->RowCount + 1;
     grTek->Cells[0][grTek->RowCount-1] = fGetItem->cbInput->Text;
     grTek->Cells[1][grTek->RowCount-1] = fGetItem->edNum->Text;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfEditItems::btDel1Click(TObject *Sender)
{
  // Удалить строчку из таблицы
  TStringGrid *grTek;
  if(((TComponent*)Sender)->Tag == 1) grTek = grInput; else
  if(((TComponent*)Sender)->Tag == 3) grTek = grManSL;
	else grTek = grInputMP;
  grTek->Rows[grTek->Row]->Clear();
  for(int i=grTek->Row; i<grTek->RowCount-1; i++)
	grTek->Rows[i]->Assign(grTek->Rows[i+1]);
  grTek->RowCount = grTek->RowCount - 1;
}
//---------------------------------------------------------------------------

void __fastcall TfEditItems::grInputDblClick(TObject *Sender)
{
  TStringGrid *Grid = ((TStringGrid*)Sender);
  TPoint p = Grid->ScreenToClient(Mouse->CursorPos);
  int r, c;
  Grid->MouseToCell(p.x, p.y, c, r);
  if(r < 0) return;
  if(r > Grid->RowCount) return;
  if(Grid->Cells[0][r] == "") return;

  fGetItem->cbInput->ItemIndex = fGetItem->cbInput->Items->IndexOf(Grid->Cells[0][r]);
  try { fGetItem->edNum->Value = Grid->Cells[1][r].ToInt(); } catch (...) { fGetItem->edNum->Value = 1; }
  fGetItem->btOk->Caption = "Change";
  if(fGetItem->ShowModal() == mrOk)
  {
	 Grid->Cells[0][r] = fGetItem->cbInput->Text;
	 Grid->Cells[1][r] = fGetItem->edNum->Text;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfEditItems::cbWskil1Change(TObject *Sender)
{
  cbWskil2->Visible = cbWskil1->ItemIndex;
  lbSk2->Visible = cbWskil2->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TfEditItems::Button1Click(TObject *Sender)
{
  FoldItem->flags = 0;
  FoldItem->type  = 0;
  if(cbItType->ItemIndex == 6)
  {
	 // Unfinished Object !!!
	 FoldItem->type = AItemType::IT_UNFINISHED;
  }

  // Capasity
  FoldItem->walk = edWalk->Value;
  FoldItem->ride = edRide->Value;
  FoldItem->fly  = edFly->Value;
  FoldItem->swim = edSwim->Value;

  // Mans
  if(cbItType->ItemIndex == 1)
  {
	FMan->abr = FAbr;
	FMan->deflevel = edManDefL->ItemIndex + 1;
	FMan->skills->Clear();
	for(int i=1,endi=grManSL->RowCount;i<endi;i++)
	{
	  if(grManSL->Cells[0][i] != "")
	  FMan->SetLevel(SkillTypes->NameToAbr(grManSL->Cells[0][i]), grManSL->Cells[1][i].ToInt());
	}
	FoldItem->type|=AItemType::IT_MAN;
	if(!ManTypes->Get(FAbr)) ManTypes->AddManType(FMan);
  } else ManTypes->DelManType(FAbr);

  // Monsters
  if(cbItType->ItemIndex == 2)
  {
	FoldItem->type|=AItemType::IT_MONSTER;
	FMonster->item = FAbr;
	FMonster->attackLevel = edMonComb->Value;
	FMonster->tactics = edMonTact->Value;
	FMonster->numAttacks = edMonAtts->Value;
	FMonster->hits = edMonHits->Value;
	FMonster->regen = edMonRegen->Value;
	FMonster->stea = edMonStea->Value;
	FMonster->obse = edMonObse->Value;
	FMonster->spec = edMonAbil->Text; //todo: Abilities....
	FMonster->specLevel = edMonAbilLev->Value;
	if(!MonstrTypes->Get(FAbr)) MonstrTypes->AddMonstrType(FMonster);
  } else MonstrTypes->DelMonstrType(FAbr);

  // Mounts
  if(cbItType->ItemIndex == 3)
  {
	FoldItem->type|=AItemType::IT_MOUNT;
	if(FMount->items->IndexOf(FAbr) < 0) FMount->items->Add(FAbr);
	FMount->skill = SkillTypes->NameToAbr(cbMountS->Text);
	FMount->minBonus = edMouMinB->Value;
	FMount->maxBonus = edMouMaxB->Value;
	FMount->maxHamperedBonus = edMouHamB->Value;
	FMount->monster = cbMouMonstr->Checked;
	// cbMouSpec->ItemIndex
	if(!MountTypes->Get(FAbr)) MountTypes->AddMountType(FMount);
  } else MountTypes->DelMountType(FAbr);

  // Weapons
  if(cbItType->ItemIndex == 4)
  {
	FoldItem->type|=AItemType::IT_WEAPON;
	FWeapon->skill1 = SkillTypes->NameToAbr(cbWskil1->Text);
	if(cbWskil2->Visible) FWeapon->skill2 = SkillTypes->NameToAbr(cbWskil2->Text);
	  else FWeapon->skill2 = "";
	FWeapon->item = FAbr;
	switch (cbAttPR->ItemIndex) {
	  case 0 : FWeapon->numAttacks = edNumAtt->Value; break;
	  case 1 : FWeapon->numAttacks = -edNumAtt->Value; break;
	  case 2 : FWeapon->numAttacks = -10; break;
	  case 3 : FWeapon->numAttacks = -11;
	}

	FWeapon->weapClass = cbWeapClass->ItemIndex;
	FWeapon->attackType = cbAttType->ItemIndex;
	FWeapon->attackBonus = edAttBonus->Value;
	FWeapon->defenseBonus = edDefBonus->Value;
	FWeapon->mountBonus = edMountBonus->Value;
	FWeapon->flags = 0;
	if(cbALWAYSREADY->Checked) FWeapon->flags|=AWeaponType::ALWAYSREADY;
	if(cbNODEFENSE->Checked) FWeapon->flags|=AWeaponType::NODEFENSE;
	if(cbNOFOOT->Checked) FWeapon->flags|=AWeaponType::NOFOOT;
	if(cbNOMOUNT->Checked) FWeapon->flags|=AWeaponType::NOMOUNT;
	if(cbSHORT->Checked) FWeapon->flags|=AWeaponType::SHORT;
	if(cbLONG->Checked) FWeapon->flags|=AWeaponType::LONG;
	if(cbRANGED->Checked) FWeapon->flags|=AWeaponType::RANGED;
	if(cbNOATTACKERSKILL->Checked) FWeapon->flags|=AWeaponType::NOATTACKERSKILL;
	if(cbRIDINGBONUS->Checked) FWeapon->flags|=AWeaponType::RIDINGBONUS;
	if(cbRIDINGBONUSDEFENSE->Checked) FWeapon->flags|=AWeaponType::RIDINGBONUSDEFENSE;
	if(!WeaponTypes->Get(FAbr)) WeaponTypes->AddWeaponType(FWeapon);
  } else WeaponTypes->DelWeaponType(FAbr);

  // Armors
  if(cbItType->ItemIndex == 5)
  {
	FArmor->item = FAbr;
	FArmor->armorclass = cbAT->ItemIndex + 1;
	FArmor->flags = 0;
	if(cbAss->Checked) FArmor->flags|=AArmorType::USEINASS;
	FArmor->from = edFrom->Text.ToInt();
	FArmor->saves[0] = edAd1->Value;
	FArmor->saves[1] = edAd2->Value;
	FArmor->saves[2] = edAd3->Value;
	FArmor->saves[3] = edAd4->Value;
	FArmor->saves[4] = edAd5->Value;
	FArmor->saves[5] = edAd6->Value;
	FArmor->saves[6] = edAd7->Value;
	FArmor->saves[7] = edAd8->Value;
	FoldItem->type|=AItemType::IT_ARMOR;
	if(!ArmorTypes->Get(FAbr)) ArmorTypes->AddArmorType(FArmor);
  } else ArmorTypes->DelArmorType(FAbr);

  // Items Properties
  FoldItem->name  = edName->Text;
  FoldItem->names = edNames->Text;
  FoldItem->abr = edAbr->Text;
  if(cbCantGive->Checked) FoldItem->flags|=AItemType::CANTGIVE;
  FoldItem->combat = cbCombat->Checked;
  // Production - From Item Info
  FoldItem->skill = SkillTypes->NameToAbr(cbSkill->Text);
  FoldItem->level = cbLevel->ItemIndex + 1;
  //Inputs
  for(int i=0; i<AItemType::INPUTCOUNT; i++) { FoldItem->inputs[i] = ""; FoldItem->numbers[i] = 0; }
  for(int i=0,endi=grInput->RowCount;i<endi;i++)
	if(grInput->Cells[0][i] != "")
	{
	   FoldItem->inputs[i]  = ItemTypes->NameToAbr(grInput->Cells[0][i]);
	   FoldItem->numbers[i] = grInput->Cells[1][i].ToInt();
	}

  if(cbOrIn->Checked) FoldItem->type|=AItemType::IT_ORINPUTS;
  FoldItem->out = edOut->Value;
  FoldItem->months = edMonths->Value;
  FoldItem->weight = edWeight->Value;
  if(cbNormal->Checked) FoldItem->type|=AItemType::IT_NORMAL;
  if(cbAdvanced->Checked) FoldItem->type|=AItemType::IT_ADVANCED;
  if(cbTrade->Checked) FoldItem->type|=AItemType::IT_TRADE;
  if(cbMagic->Checked) FoldItem->type|=AItemType::IT_MAGIC;
  if(cbBattle->Checked) FoldItem->type|=AItemType::IT_BATTLE;
  if(cbFood->Checked) FoldItem->type|=AItemType::IT_FOOD;

  FoldItem->helpproditem = ItemTypes->NameToAbr(cbHelpProd->Text);
  FoldItem->helpprodmult = edHelpMult->Value;
}
//---------------------------------------------------------------------------



void __fastcall TfEditItems::SpeedButton3Click(TObject *Sender)
{
  TfGetSkilLv *fGetSkilLv = new TfGetSkilLv(this);
  fGetSkilLv->cbSkills->Items->Assign(cbSkill->Items);
  fGetSkilLv->cbSkills->ItemIndex = 0;
  if(fGetSkilLv->ShowModal() == mrOk)
  {
	  // Проверить на наличие
	  for(int i=1,endi=grManSL->RowCount;i<endi;i++)
	  {
		if(grManSL->Cells[0][i] == fGetSkilLv->cbSkills->Text)
		{
		  grManSL->Cells[1][i] = fGetSkilLv->edLev->Text;
		  fGetSkilLv->Free();
		  return;
		}
	  }
	  // Добавить если надо
	  grManSL->RowCount = grManSL->RowCount + 1;
	  grManSL->Cells[0][grManSL->RowCount-1] = fGetSkilLv->cbSkills->Text;
	  grManSL->Cells[1][grManSL->RowCount-1] = fGetSkilLv->edLev->Text;
  }
  fGetSkilLv->Free();
}
//---------------------------------------------------------------------------

