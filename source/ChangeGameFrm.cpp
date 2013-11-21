//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "ChangeGameFrm.h"
#include "util.h"
#include "turn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TChangeGameForm *ChangeGameForm;
//---------------------------------------------------------------------------
__fastcall TChangeGameForm::TChangeGameForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TChangeGameForm::FormCreate(TObject *Sender)
{
 for(int i=0;i<Games->Count;i++){
  lbGames->Items->Add(Games->Strings[i]);
 }
 for(int i=0;i<lbGames->Items->Count;i++){
  if(lbGames->Items->Strings[i]==curGame){
   lbGames->ItemIndex=i;
   break;
  }
 }
}
//---------------------------------------------------------------------------
void __fastcall TChangeGameForm::bnOkClick(TObject *Sender)
{
 if(lbGames->ItemIndex==-1)return;
  AnsiString newGame=lbGames->Items->Strings[lbGames->ItemIndex];
 if(newGame==curGame)return;
 AnsiString oldGame=curGame;
 ATurns*oldTurns=curTurns;
 if(cbSaveOld->Checked)
  SaveGameBase();
 if(oldTurns)
   oldTurns->UnPlug(); 
 curTurns=0;
 curGame=newGame;
 try{
  LoadGameBase();
 }catch(Exception&e){
  Application->MessageBox(e.Message.c_str(),"Error loadind GameBase",MB_OK);
  delete curTurns;
  curTurns=oldTurns;
  curTurns->RestoreLastParam();
  curGame=oldGame;
  return;
 }
 delete oldTurns;
}
//---------------------------------------------------------------------------
void __fastcall TChangeGameForm::lbGamesDblClick(TObject *Sender)
{
 bnOk->Click();   
}
//---------------------------------------------------------------------------
