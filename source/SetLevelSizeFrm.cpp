//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "SetLevelSizeFrm.h"
#include "util.h"
#include "turn.h"
#include "region.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TSetLevelSizeForm *SetLevelSizeForm;
//---------------------------------------------------------------------------
__fastcall TSetLevelSizeForm::TSetLevelSizeForm(TComponent* Owner)
  : TForm(Owner)
{
  TStrings *list=cbLevel->Items;
  foreach(curRegionList->RegionArrays)
  {
    ARegionArray *ra=*(ARegionArray**)iter;
    AnsiString name=ra->GetName();
    list->AddObject(name,(TObject*)ra->levelnum);
  }
  cbLevel->ItemIndex=1;
  cbLevelChange(0);
}
//---------------------------------------------------------------------------
void __fastcall TSetLevelSizeForm::cbLevelChange(TObject *Sender)
{
  int ind=cbLevel->ItemIndex;
  if(ind<0) return;
  ARegionArray *ra=curRegionList->RegionArrays->Get((int)cbLevel->Items->Objects[ind]);
  edX->Text=ra->x;
  edY->Text=ra->y;
}
//---------------------------------------------------------------------------
void __fastcall TSetLevelSizeForm::bnSetClick(TObject *Sender)
{
  int ind=cbLevel->ItemIndex;
  if(ind<0) return;
  int levelnum=(int)cbLevel->Items->Objects[ind];
  int x=atoi(edX->Text.c_str());
  int y=atoi(edY->Text.c_str());
  if(!x||!y) return;

  panSettings->Enabled=false;
  ATurn *turn;
  TList *delturns=new TList;
  for(int i=0;i<curTurns->count;i++)
  {
    turn=curTurns->Get(i);
    if(!turn)
    {
      if(!curTurns->PreloadTurn(i)) continue;
      turn=curTurns->Get(i);
      if(!turn) continue;
      delturns->Add((void*)i);
      if(delturns->Count>12)
      {
        for(int i=0;i<4;i++)
        {
          int num=(int)delturns->Items[0];
          delturns->Delete(0);
          curTurns->Delete(num);
        }
      }
    }
    ARegionList *rl=turn->RegionList;
    bool needdeleteregs=false;
    foreach(rl->RegionArrays)
    {
      ARegionArray *ra=*(ARegionArray**)iter;
      if(ra->levelnum!=levelnum) continue;
      if(ra->x==x&&ra->y==y)
        continue;
      if(ra->x>x||ra->y>y)
        needdeleteregs=true;
      ra->x=x;
      ra->y=y;
      turn->Modified=true;
    }
    if(needdeleteregs)
    {
      foreachr(rl)
      {
        ARegion *reg=*(ARegion**)iter;
        if(reg->zloc!=levelnum) continue;
        if(reg->xloc>=x||reg->yloc>=y)
        {
          rl->Delete(reg);
          turn->Modified=true;
          continue;
        }
        for(int i=0;i<NDIRS;i++)
        {
          ALocation &loc=reg->neighbors[i];
          if(loc.zloc!=levelnum) continue;
          if(loc.xloc>=x||loc.yloc>=y)
          {
            loc.xloc=-1;
            turn->Modified=true;
          }
        }
      }
    }
    turn->Write();
  }
  for(int i=0;i<delturns->Count;i++)
  {
    int num=(int)delturns->Items[i];
    curTurns->Delete(num);
  }

  delete delturns;
  panSettings->Enabled=true;
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TSetLevelSizeForm::edXKeyPress(TObject *Sender, char &Key)
{
  if(Key!=13) return;
  bnSet->Click();
}
//---------------------------------------------------------------------------
