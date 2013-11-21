//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
//---------------------------------------------------------------------------
#include "util.h"
#include "SkillsTree.h"
#include "unit.h"
#include "faction.h"
#include "orders.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TfSkillsTree *fSkillsTree;
//---------------------------------------------------------------------------
__fastcall TfSkillsTree::TfSkillsTree(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfSkillsTree::FormCreate(TObject *Sender)
{
  if(!curUnit) btQall->Enabled = false;
  if(!SkillTypes)return;

  TStringList* list = new TStringList;
  list->Clear();
  SkillTypes->MakeSkillList(list);
  list->Sort();

  cbSkill->Items->Clear();
  cbSkill->Items->Add("None");
  cbSkill->Items->AddStrings(list);
  cbSkill2->Items->Assign(cbSkill->Items);
  cbSkill3->Items->Assign(cbSkill->Items);
  delete list;

  TTreeNodes *tns = TreeS->Items;
  tns->Clear();
  tnsF = tns->Add(0, "Foundations");
  tnsM = tns->Add(0, "Magic Skills");
  tnsS = tns->Add(0, "Simple Skills");
  TTreeNode *tnsT;

  foreach(SkillTypes)
  {
	ASkillType *si = *(ASkillType**)iter;
	if(si->flags&ASkillType::FOUNDATION) tnsT = tnsF; else
	if(si->flags&ASkillType::MAGIC) tnsT = tnsM; else tnsT = tnsS;
	tns->AddChildObject(tnsT, si->name, si);
  }

  tnsF->AlphaSort();
  tnsM->AlphaSort();
  tnsS->AlphaSort();
  FChanged = false;
}
//---------------------------------------------------------------------------
void __fastcall TfSkillsTree::TreeSExpanding(TObject *Sender, TTreeNode *Node, bool &AllowExpansion)
{
  TTreeNode *tnsT = Node->getFirstChild();
  while (tnsT)
  {
	if(!tnsT->HasChildren)
	{
	  ASkillType *siT = (ASkillType*)tnsT->Data;
	  foreach(SkillTypes)
	  {
		ASkillType *si = *(ASkillType**)iter;
		for(int i=0;i<3;i++)
		{
		  if(si->depends[i].Length() == 0) break;
		  if(si->depends[i] == siT->abr) TreeS->Items->AddChildObject(tnsT, si->name, si);
		}
	  }
	}
	tnsT = Node->GetNextChild(tnsT);
  }
}
//---------------------------------------------------------------------------
void __fastcall TfSkillsTree::TreeSChange(TObject *Sender, TTreeNode *Node)
{
  ASkillType *siT = (ASkillType*)Node->Data;
  if(!siT) { gbInf->Enabled = false; btQ->Enabled = false; return; }
  gbInf->Enabled = true;

  lbAbr->Caption = siT->abr;
  if(siT->flags&ASkillType::FOUNDATION) cbTypeS->ItemIndex = 0; else
  if(siT->flags&ASkillType::MAGIC) cbTypeS->ItemIndex = 1; else cbTypeS->ItemIndex = 2;
  edCost->Value = siT->cost;
  cbCast->Checked = (siT->flags&ASkillType::CAST);
  cbComb->Checked = (siT->flags&ASkillType::COMBAT);

  cbSkill2->ItemIndex = 0;
  cbSkill3->ItemIndex = 0;
  if(siT->depends[0].Length())
  {
	 cbSkill->ItemIndex = cbSkill->Items->IndexOf(SkillTypes->AbrToName(siT->depends[0]));
	 edLev->Value = siT->levels[0];
	 if(siT->depends[1].Length())
	 {
	   cbSkill2->ItemIndex = cbSkill2->Items->IndexOf(SkillTypes->AbrToName(siT->depends[1]));
	   edLev2->Value = siT->levels[1];
	   if(siT->depends[2].Length())
	   {
		 cbSkill3->ItemIndex = cbSkill3->Items->IndexOf(SkillTypes->AbrToName(siT->depends[2]));
		 edLev3->Value = siT->levels[2];
	   } else cbSkill3->ItemIndex = 0;
	 } else cbSkill2->ItemIndex = 0;
  } else cbSkill->ItemIndex = 0;
  cbSkillChange(0);
  cbSkill2Change(0);
  cbSkill3Change(0);

  Memo1->Clear();
  for(int i=1;i<6;i++)
  {
	ASkillInfo *s = SkillInfos->Get(siT->abr, i);
	Memo1->Lines->Add(siT->name + ", Level " + i);
	if(s && s->text.Length()) Memo1->Lines->Add(s->text);
	  else Memo1->Lines->Add("No report");
	Memo1->Lines->Add("");
  }

  grSpec->RowCount = 1;
  grSpec->Cells[0][0] = "";
  grSpec->Cells[1][0] = "";
  int i = 0;
  int ind;
  foreach(ManTypes)
  {
	AManType *man  = *(AManType**) iter;
	ind = man->skills->IndexOf(siT->abr);
	if(ind >= 0)
	{
	  i++;
	  if(grSpec->RowCount < i) grSpec->RowCount = i;
		  grSpec->Cells[0][i-1] = ItemTypes->AbrToNames(man->abr);
	  grSpec->Cells[1][i-1] = (int)man->skills->Objects[ind];
	}
  }
  FChanged = False;
  btSave->Enabled = false;
  btQ->Enabled = btQall->Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TfSkillsTree::cbSkillChange(TObject *Sender)
{
  edLev->Visible = (cbSkill->ItemIndex > 0);
  lbAnd1->Visible = edLev->Visible;
  cbSkill2->Visible = edLev->Visible;
  FChanged = true;
  btSave->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfSkillsTree::cbSkill2Change(TObject *Sender)
{
  edLev2->Visible = (cbSkill2->ItemIndex > 0);
  lbAnd2->Visible = edLev2->Visible;
  cbSkill3->Visible = edLev2->Visible;
  FChanged = true;
  btSave->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfSkillsTree::cbSkill3Change(TObject *Sender)
{
  edLev3->Visible = (cbSkill3->ItemIndex > 0);
  FChanged = true;
  btSave->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfSkillsTree::edLevChange(TObject *Sender)
{
  FChanged = true;
  btSave->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfSkillsTree::TreeSChanging(TObject *Sender, TTreeNode *Node, bool &AllowChange)
{
  if(FChanged)
  {
	switch (MessageDlg("", mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo<<mbCancel, 0))
	{
	  mrYes : btSaveClick(0); return;
	  mrCancel : AllowChange = false; break;
	  default : FChanged = false; btSave->Enabled = false;
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TfSkillsTree::btSaveClick(TObject *Sender)
{
  ASkillType *siT = (ASkillType*)TreeS->Selected->Data;
  if(!siT) return;
  siT->flags = 0;
  if(cbTypeS->ItemIndex == 0) siT->flags|=ASkillType::FOUNDATION;
  if(cbTypeS->ItemIndex == 1) siT->flags|=ASkillType::MAGIC;
  siT->cost = edCost->Value;
  if(cbCast->Checked) siT->flags|=ASkillType::CAST;
  if(cbComb->Checked) siT->flags|=ASkillType::COMBAT;
  for(int i=0; i<3; i++) siT->depends[0] = "";
  if(cbSkill->ItemIndex > 0) { siT->depends[0] = SkillTypes->NameToAbr(cbSkill->Text); siT->levels[0] = edLev->Value; }
  if(cbSkill2->Visible && cbSkill2->ItemIndex > 0) { siT->depends[1] = SkillTypes->NameToAbr(cbSkill2->Text); siT->levels[1] = edLev2->Value; }
  if(cbSkill3->Visible && cbSkill3->ItemIndex > 0) { siT->depends[1] = SkillTypes->NameToAbr(cbSkill3->Text); siT->levels[1] = edLev3->Value; }
  FChanged = false; btSave->Enabled = false;
}
//---------------------------------------------------------------------------
void TfSkillsTree::RequeryInfo(ASkillType * si)
{
  if(!si) return;
  if(!curUnit)return;
  if(!curUnit->faction->local) return;
  for(int i=1;i<6;i++)
  {
	AOrderCustom *ord = new AOrderCustom;
	ord->text = "show skill " + si->abr + " " + i;
	curUnit->orders->Add(ord);
  }
}
//---------------------------------------------------------------------------
void __fastcall TfSkillsTree::btQClick(TObject *Sender)
{
  ASkillType *si = (ASkillType*)TreeS->Selected->Data;
  if(!si) return;
  RequeryInfo(si);
  curRegion->runned = false;
  AtlaForm->ViewShow();
}
//---------------------------------------------------------------------------
void __fastcall TfSkillsTree::btQallClick(TObject *Sender)
{
  if(!curUnit)return;
  if(!curUnit->faction->local)return;
  foreach(SkillTypes)
  {
	ASkillType *si = *(ASkillType**)iter;
	RequeryInfo(si);
  }
  curRegion->runned = false;
  AtlaForm->ViewShow();
}
//---------------------------------------------------------------------------
void __fastcall TfSkillsTree::FormClose(TObject *Sender, TCloseAction &Action)
{
  Free();	
}
//---------------------------------------------------------------------------

void __fastcall TfSkillsTree::FormDestroy(TObject *Sender)
{
  fSkillsTree = 0;	
}
//---------------------------------------------------------------------------

