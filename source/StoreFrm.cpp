//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdlib.h>
#include "StoreFrm.h"
#include "util.h"
#include "turn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TStoreForm *StoreForm;
//---------------------------------------------------------------------------
__fastcall TStoreForm::TStoreForm(TComponent* Owner)
    : TForm(Owner)
{
 if(!curTurns) return;
 edBeginTurn->Text=curTurns->newbeginturn;
 if(curTurns->autobeginturn>0){
  rbLastTurn->Checked=true;
  edAutoBeginTurn->Text=curTurns->autobeginturn;
 }else{
  rbFirstTurn->Checked=true;
  edAutoBeginTurn->Text=-curTurns->autobeginturn;
 }
}
//---------------------------------------------------------------------------
void __fastcall TStoreForm::bnOkClick(TObject *Sender)
{
 if(rbLastTurn->Checked){
  int abt=atoi(edAutoBeginTurn->Text.c_str());
  if(abt<1){
   Application->MessageBox("Bad last turn number","",MB_OK);
   ModalResult=mrNone;
   return;
  }
  curTurns->autobeginturn=abt;
  return;
 }
 if(curTurns->autobeginturn>0)
  curTurns->autobeginturn=-curTurns->autobeginturn;
 int nbt=atoi(edBeginTurn->Text.c_str());
 if(nbt<1||nbt>curTurns->count-1){
  Application->MessageBox("Bad turn number","",MB_OK);
  ModalResult=mrNone;
  return;
 }
 curTurns->newbeginturn=nbt;
}
//---------------------------------------------------------------------------
void __fastcall TStoreForm::rbClick(TObject *Sender)
{
 if(rbFirstTurn->Checked){
  edBeginTurn->Enabled=true;
  edAutoBeginTurn->Enabled=false;
 }else{
  edBeginTurn->Enabled=false;
  edAutoBeginTurn->Enabled=true;
 }
}
//---------------------------------------------------------------------------

