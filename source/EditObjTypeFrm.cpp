//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "EditObjTypeFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditObjTypeForm *EditObjTypeForm;
//---------------------------------------------------------------------------
__fastcall TEditObjTypeForm::TEditObjTypeForm(TComponent* Owner, AObjectType *oldObj,
        AnsiString descr)
        : TForm(Owner)
{
 FoldObj = oldObj;
 if(!oldObj) return;
 if(!ItemTypes) return;
 if(!SkillTypes) return;
 Memo->Visible = descr.Length();
 Memo->Lines->Text = descr;
}

__fastcall TEditObjTypeForm::~TEditObjTypeForm()
{
}

//---------------------------------------------------------------------------

void __fastcall TEditObjTypeForm::FormCreate(TObject *Sender)
{
  if(!FoldObj) return;
  if(!ItemTypes) return;
  if(!SkillTypes) return;

  AnsiString str;
  TStringList*list = new TStringList;
  ItemTypes->MakeItemList(list,AItemType::IT_TRADE|AItemType::IT_MONSTER|AItemType::IT_MAN);
  list->Sort();

  cbHelpProd->Items->Clear();
  cbHelpProd->Items->Add("None");
  cbHelpProd->Items->AddStrings(list);
  cbHelpProd->ItemIndex = 0;

  cbInput->Items->Clear();
  cbInput->Items->Add("None");
  cbInput->Items->Add("Wood or Stone");
  cbInput->Items->AddStrings(list);
  cbInput->ItemIndex = 0;

  cbIncProd->Items->Clear();
  cbIncProd->Items->Add("None");
  cbIncProd->Items->AddStrings(list);
  cbIncProd->ItemIndex = 0;

  list->Clear();
  SkillTypes->MakeSkillList(list);
  list->Sort();
  cbSkill->Items->Clear();
  cbSkill->Items->Add("None");
  cbSkill->Items->AddStrings(list);
  cbSkill->ItemIndex = 0;

  list->Clear();
  DirTypes->MakeSkillList(list);
  //list->Sort();
  cbRoad->Items->Clear();
  cbRoad->Items->Add("None");
  cbRoad->Items->AddStrings(list);
  delete list;

  edName->Text = FoldObj->name;
  edProtect->Text = FoldObj->protect;
  edCapacity->Text = FoldObj->capacity;
  cbEnter->Checked = FoldObj->canenter;
  cbShip->Checked = FoldObj->sailors;
  edSailors->Text = FoldObj->sailors;
  edSailors->Visible = cbShip->Checked;
  lbSailors->Visible = edSailors->Visible;

  int ind = cbSkill->Items->IndexOf(SkillTypes->AbrToName(FoldObj->skill));
  if(ind == -1) ind = 0;
  cbSkill->ItemIndex = ind;
  if(ind){
    cbLevel->ItemIndex = FoldObj->level-1;
  }
  cbSkillChange(this);

  if(cbInput->Visible){
    if(FoldObj->item == WoodOrStoneStr) cbInput->ItemIndex = 1;
    else {
      ind = cbInput->Items->IndexOf(ItemTypes->AbrToName(FoldObj->item));
      if(ind == -1)ind = 0;
      cbInput->ItemIndex = ind;
    }
    edNumber->Text = FoldObj->cost;
    cbInputChange(this);
  }

  ind = cbIncProd->Items->IndexOf(ItemTypes->AbrToName(FoldObj->production));
  if(ind == -1)ind = 0;
  cbIncProd->ItemIndex = ind;

  ind = cbHelpProd->Items->IndexOf(ItemTypes->AbrToName(FoldObj->helpproditem));
  if(ind == -1) ind = 0;
  cbHelpProd->ItemIndex = ind;
  edHelpMult->Text = FoldObj->helpprodmult;

  ind = 0;
  switch(FoldObj->road){
    case 1: ind = 1; break;
    case 2: ind = 2; break;
    case 4: ind = 3; break;
    case 8: ind = 4; break;
    case 16: ind = 5; break;
    case 32: ind = 6; break;
  }
  cbRoad->ItemIndex = ind;
}
//---------------------------------------------------------------------------

void __fastcall TEditObjTypeForm::cbSkillChange(TObject *Sender)
{
  cbLevel->Visible = (cbSkill->ItemIndex > 0);
  labLevel->Visible = cbLevel->Visible;
  cbInput->Visible = cbLevel->Visible;
  labInput->Visible = cbLevel->Visible;
  labHelpProd->Visible = cbLevel->Visible;
  edHelpMult->Visible = cbLevel->Visible;
  labHelpFrom->Visible = cbLevel->Visible;
  cbHelpProd->Visible = cbLevel->Visible;

  if(cbSkill->ItemIndex<=0){
    cbLevel->ItemIndex = 0;
    cbInput->ItemIndex = 0;
    cbInputChange(Sender);
 }
}
//---------------------------------------------------------------------------

void __fastcall TEditObjTypeForm::cbInputChange(TObject *Sender)
{
  if(cbInput->ItemIndex<=0){
    edNumber->Visible = false;
    edNumber->Text = 0;
    labNumber->Visible = false;
  }else{
    edNumber->Visible = true;
    labNumber->Visible = true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TEditObjTypeForm::cbShipClick(TObject *Sender)
{
  lbSailors->Visible = cbShip->Checked;
  edSailors->Visible = cbShip->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TEditObjTypeForm::bnOkClick(TObject *Sender)
{
  FoldObj->name = edName->Text;
  FoldObj->protect = edProtect->Text.ToInt();
  FoldObj->capacity = edCapacity->Text.ToInt();
  FoldObj->canenter = cbEnter->Checked;
  FoldObj->sailors = edSailors->Text.ToInt();

  int ind = cbSkill->ItemIndex;
  if(ind == 0){
    FoldObj->skill="";
  } else {
    FoldObj->skill = SkillTypes->NameToAbr(cbSkill->Items->Strings[ind]);
    FoldObj->level = cbLevel->ItemIndex + 1;
  }

  ind = cbInput->ItemIndex;
  if(ind == 0){
    FoldObj->item = "";
  } else {
    if(ind == 1) FoldObj->item = WoodOrStoneStr;
      else FoldObj->item = ItemTypes->NameToAbr(cbInput->Items->Strings[ind]);
    FoldObj->cost = edNumber->Text.ToInt();
  }

  ind = cbIncProd->ItemIndex;
  if(ind == 0){
    FoldObj->production = "";
  } else {
    FoldObj->production = ItemTypes->NameToAbr(cbIncProd->Items->Strings[ind]);
  }

  ind = cbHelpProd->ItemIndex;
  if(ind == 0){
    FoldObj->helpproditem = "";
  } else {
    FoldObj->helpproditem = ItemTypes->NameToAbr(cbHelpProd->Items->Strings[ind]);
    FoldObj->helpprodmult = edHelpMult->Text.ToInt();
  }

  ind = cbRoad->ItemIndex;
  switch(cbRoad->ItemIndex){
    case 3: ind = 4; break;
    case 4: ind = 8; break;
    case 5: ind = 16; break;
    case 6: ind = 32; break;
  }
  FoldObj->road = ind;
}
//---------------------------------------------------------------------------

