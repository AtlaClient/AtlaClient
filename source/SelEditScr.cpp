//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "SelEditScr.h"
#include "util.h"
#include "EditScript.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfSelEditScr *fSelEditScr;
//---------------------------------------------------------------------------
__fastcall TfSelEditScr::TfSelEditScr(TComponent* Owner)
	: TForm(Owner)
{
  Changed = false;
  for(int i = 0; i < AllScripts->count; i++)
  {
	List->Items->Add(AllScripts->Get(i)->caption);
  }
  SetEnabledBt();
}
//---------------------------------------------------------------------------
void TfSelEditScr::SetEnabledBt()
{
  if( (List->ItemIndex<0) && (List->Items->Count) ) List->ItemIndex = 0;
  bnEdit->Enabled = (List->ItemIndex>=0);
  btDel->Enabled = bnEdit->Enabled;
}
//---------------------------------------------------------------------------

void __fastcall TfSelEditScr::btDelClick(TObject *Sender)
{
  int num = List->ItemIndex;
  if(num == -1) return;
  if(Application->MessageBox("Delete this script ?", "Warning", MB_YESNO)!=IDYES) return;
  List->DeleteSelected();
  AllScripts->Delete(num);
  if(List->Items->Count > num) List->ItemIndex = num; else
  if(List->Items->Count) List->ItemIndex = (num-1);
  SetEnabledBt();
  Changed = true;
}
//---------------------------------------------------------------------------
void __fastcall TfSelEditScr::Button1Click(TObject *Sender)
{
  fEditScript->Memo1->Clear();
  fEditScript->Caption = "";
  fEditScript->cbClear->Checked = false;
  fEditScript->edName->Text = "New Script";
  if(fEditScript->ShowModal()!=mrOk) return;
  AScript *scr = new AScript;
  scr->caption  = fEditScript->edName->Text;
  scr->CountStr = fEditScript->Memo1->Lines->Count;
  scr->NeedClear= fEditScript->cbClear->Checked;
  scr->Strings->Text = fEditScript->Memo1->Text;
  AllScripts->Add(scr);
  List->Items->Add(fEditScript->edName->Text);
  List->ItemIndex = (List->Items->Count-1);
  SetEnabledBt();
  Changed = true;
}
//---------------------------------------------------------------------------
void __fastcall TfSelEditScr::bnEditClick(TObject *Sender)
{
  int num = List->ItemIndex;
  if(num == -1) return;
  AScript *scr = AllScripts->Get(num);
  if(!scr) return;

  fEditScript->cbClear->Checked = scr->NeedClear;
  fEditScript->edName->Text = scr->caption;
  fEditScript->Memo1->Text = scr->Strings->Text;

  if(fEditScript->ShowModal()!=mrOk) return;

  scr->caption  = fEditScript->edName->Text;
  scr->CountStr = fEditScript->Memo1->Lines->Count;
  scr->NeedClear= fEditScript->cbClear->Checked;
  scr->Strings->Text = fEditScript->Memo1->Text;

  Changed = true;
}
//---------------------------------------------------------------------------

