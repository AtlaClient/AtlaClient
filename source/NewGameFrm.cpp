//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "NewGameFrm.h"
#include "util.h"
#include "turn.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TNewGameForm *NewGameForm;
//---------------------------------------------------------------------------
__fastcall TNewGameForm::TNewGameForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TNewGameForm::bnOkClick(TObject *Sender)
{
 if(!edName->Text.Length()){
  Application->MessageBox("Please enter game name","Error creating GameBase",MB_OK);
  ModalResult=0;
  return;
 }
 AnsiString oldGame=curGame;
 ATurns*oldTurns=curTurns;
 if(cbSaveOld->Checked)
  SaveGameBase();
 curTurns=0;
 if(cbCreateNew->Checked==false){
  curGame=edName->Text;
  try{
   LoadGameBase();
  }catch(Exception&e){
   Application->MessageBox(e.Message.c_str(),"Error loadind GameBase",MB_OK);
   delete curTurns;
   curTurns=oldTurns;
   curGame=oldGame;
   return;
  }
  delete oldTurns;
  if(Games->IndexOf(curGame)==-1){
   Games->Add(curGame);
   AtlaForm->SaveGameList();
  }
  AtlaForm->miGameChange->Enabled=(Games->Count>1);
  return;
 }
 AddEvent("Creating game \""+edName->Text+"\" ...");
 delete oldTurns;
 if(curGame.Length()){
  curGame="";
  LoadGameBase();
 }
 curGame=edName->Text;
 ForceDirectories(GetGameFileName(""));
 DeleteFiles(GetGameFileName("*.*"));
 curTurns=new ATurns;
 curTurns->CreateNew();
 curTurns->SetCurTurn(0);
 SaveGameBase();
 AddEvent("Game \""+curGame+"\" created");
 if(Games->IndexOf(curGame)==-1){
  Games->Add(curGame);
  AtlaForm->SaveGameList();
 }
 AtlaForm->miGameChange->Enabled=(Games->Count>1);
}
//---------------------------------------------------------------------------

