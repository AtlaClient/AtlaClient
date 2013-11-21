//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <vector.h>
#include <inifiles.hpp>

#include "ShortCutsFrm.h"
#include "util.h"
#include "orders.h"
#include "shortcuts.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TShortCutsForm *ShortCutsForm;
//---------------------------------------------------------------------------
__fastcall TShortCutsForm::TShortCutsForm(TComponent* Owner)
  : TForm(Owner),sco(0)
{
  ShortCuts.FillShortCutsList(List->Items);
  ShortCuts.MakeTree(TreeView->Items);
  ShowShortCut();
}
//---------------------------------------------------------------------------
void __fastcall TShortCutsForm::bnSetClick(TObject *Sender)
{
  if(!sco) return;
  if(!bnSet->Visible) return;
  TShortCut sc=HotKey->HotKey;
  AnsiString error;
  if(!ShortCuts.ChangeShortCut(sco,sc,error))
  {
    ShowMessage("Shortcut already used in: "+error);
    return;
  }
  ShortCuts.FillShortCutsList(List->Items);
}
//---------------------------------------------------------------------------
void TShortCutsForm::ShowShortCut()
{
  bool set=true;
  if(!sco)
    set=false;
  else if(!sco->mi)
    set=false;
  else if(sco->mi->Count)
    set=false;
  labCaption->Visible=set;
  HotKey->Visible=set;
  bnSet->Visible=set;
  bnClearShortCuts->Visible=set;
  if(!set)
    return;

  labCaption->Caption=sco->name;
  HotKey->HotKey=sco->mi->ShortCut;
}
//---------------------------------------------------------------------------
void __fastcall TShortCutsForm::TreeViewChange(TObject *Sender,
      TTreeNode *Node)
{
  sco=ShortCuts.GetOwnerFromTree(Node);
  ShowShortCut();
}
//---------------------------------------------------------------------------
void __fastcall TShortCutsForm::FormDestroy(TObject *Sender)
{
  ShortCuts.ClearTreeInfo();
}
//---------------------------------------------------------------------------
void __fastcall TShortCutsForm::bnClearShortCutsClick(TObject *Sender)
{
  ShortCuts.ClearAllShortCuts(HotKey->HotKey);
  ShortCuts.FillShortCutsList(List->Items);
}
//---------------------------------------------------------------------------

