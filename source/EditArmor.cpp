//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "EditArmor.h"
#include "util.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfEditArmor *fEditArmor;
//---------------------------------------------------------------------------
__fastcall TfEditArmor::TfEditArmor(TComponent* Owner,
                   AArmorType *arm, AnsiString descr): TForm(Owner)
{
  if(!arm){
    bNewArm = true;
    Farm = new AArmorType;
  } else {
    bNewArm = false;
    Farm = arm;
  }

  Memo->Visible = descr.Length();
  Memo->Lines->Text = descr;
}
//---------------------------------------------------------------------------
void __fastcall TfEditArmor::FormCreate(TObject *Sender)
{
  if(Farm->armorclass < 1) Farm->armorclass = 1;
  cbAT->ItemIndex = Farm->armorclass - 1;
  cbAss->Checked = (Farm->flags&&AArmorType::USEINASS);
  edFrom->Text = Farm->from;
  ed1->Text = Farm->saves[0];
  ed2->Text = Farm->saves[1];
  ed3->Text = Farm->saves[2];
  ed4->Text = Farm->saves[3];
  ed5->Text = Farm->saves[4];
  ed6->Text = Farm->saves[5];
  ed7->Text = Farm->saves[6];
  ed8->Text = Farm->saves[7];
}
//---------------------------------------------------------------------------
void __fastcall TfEditArmor::bnOkClick(TObject *Sender)
{
  Farm->armorclass = cbAT->ItemIndex + 1;
  Farm->flags = 0;
  if(cbAss->Checked) Farm->flags|= AArmorType::USEINASS;
  Farm->from = edFrom->Text.ToInt();
  Farm->saves[0] = ed1->Text.ToInt();
  Farm->saves[1] = ed2->Text.ToInt();
  Farm->saves[2] = ed3->Text.ToInt();
  Farm->saves[3] = ed4->Text.ToInt();
  Farm->saves[4] = ed5->Text.ToInt();
  Farm->saves[5] = ed6->Text.ToInt();
  Farm->saves[6] = ed7->Text.ToInt();
  Farm->saves[7] = ed8->Text.ToInt();
  if(bNewArm){
    Farm->item = TekItem;
    ArmorTypes->AddArmorType(Farm);
  }
}
//---------------------------------------------------------------------------
