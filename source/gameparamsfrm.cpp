//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "gameparamsfrm.h"
#include "gameparams.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGameParamsForm *GameParamsForm;
//---------------------------------------------------------------------------
__fastcall TGameParamsForm::TGameParamsForm(TComponent* Owner)
  : TForm(Owner)
{
  CreateList();
}
//---------------------------------------------------------------------------
void TGameParamsForm::CreateList()
{
  TStrings *list=List->Items;
  int maxsize=0;
  list->BeginUpdate();
  list->Clear();
  for(TGameParam *gp=GameParams.params.begin().operator ->(),*endgp=GameParams.params.end().operator ->();gp<endgp;gp++)
  {
    AnsiString str=gp->comment+" = "+gp->GetString();
	list->AddObject(str,(TObject*)(gp-GameParams.params.begin().operator ->()));
    TSize siz=List->Canvas->TextExtent(str);
    if(siz.cx>maxsize)maxsize=siz.cx;
  }
  list->EndUpdate();
  List->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
  if(List->Items->Count)
    List->ItemIndex=0;
  ListClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TGameParamsForm::ListClick(TObject *Sender)
{
  int ind=List->ItemIndex;
  TGameParam *gp=0;
  if(ind>=0)
  {
    ind=(int)List->Items->Objects[ind];
    if(unsigned(ind)<GameParams.params.size())
      gp=&GameParams.params[ind];
  }
  if(!gp||!gp->IsPtrSet())
  {
    labComment->Caption="";
    edValue->Text="";
    bnSet->Enabled=false;
  }
  labComment->Caption=gp->comment;
  edValue->Text=gp->GetString();
  bnSet->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TGameParamsForm::bnSetClick(TObject *Sender)
{
  int ind=List->ItemIndex;
  TGameParam *gp=0;
  if(ind>=0)
  {
    ind=(int)List->Items->Objects[ind];
    if(unsigned(ind)<GameParams.params.size())
      gp=&GameParams.params[ind];
  }
  if(!gp||!gp->IsPtrSet())
    return;
  gp->SetString(edValue->Text);
  ind=List->ItemIndex;
  CreateList();
  try{
    List->ItemIndex=ind;
  }catch(...){}
  ListClick(0);
}
//---------------------------------------------------------------------------

