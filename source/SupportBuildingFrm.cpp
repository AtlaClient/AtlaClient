//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "SupportBuildingFrm.h"
#include "util.h"
#include "region.h"
#include "markets.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TSupportBuildingForm *SupportBuildingForm;
//---------------------------------------------------------------------------
__fastcall TSupportBuildingForm::TSupportBuildingForm(TComponent* Owner)
  : TForm(Owner)
{
  sgCalc->Cells[0][0]="Buildings";
  sgCalc->Cells[1][0]="Production";
  sgReg->Cells[0][0]="Buildings";
  sgReg->Cells[1][0]="Production";
}
//---------------------------------------------------------------------------
void TSupportBuildingForm::MakeCalc(int begprod)
{
  sgCalc->RowCount=2;
  sgCalc->RowCount=21;
  int add=begprod/4,i;
  for(i=0;i<20&&add!=0;i++)
  {
    if(i!=0)
    {
      begprod+=add;
      add/=2;
    }
   sgCalc->Cells[0][i+1]=i;
   sgCalc->Cells[1][i+1]=begprod;
  }
  sgCalc->Cells[0][i+1]=i;
  sgCalc->Cells[1][i+1]=begprod;
  sgCalc->RowCount=i+2;
}
//---------------------------------------------------------------------------
void __fastcall TSupportBuildingForm::bnCalcClick(TObject *Sender)
{
  int prods=atoi(edProds->Text.c_str());
  if(prods<0) prods=0;
  int builds=atoi(edBuilds->Text.c_str());
  if(builds<0) builds=0;
  if(builds>20) builds=20;
  if(builds)
  {
    prods=CalcBegProd(prods,builds);
  }
  MakeCalc(prods);
}
//---------------------------------------------------------------------------
int TSupportBuildingForm::CalcBegProd(int prod, int builds)
{
  for(int pr=prod/2;pr<prod;pr++)
  {
    int pr2=CalcProd(pr,builds);
    if(pr2!=prod) continue;
    return pr;
  }
  return 0;
}
//---------------------------------------------------------------------------
int TSupportBuildingForm::CalcProd(int prods, int builds)
{
  int add=prods/4,i;
  for(i=0;i<builds;i++)
  {
    prods+=add;
    add/=2;
  }
  return prods;
}
//---------------------------------------------------------------------------
int TSupportBuildingForm::CalcMaxProd(int prods)
{
  int add=prods/4;
  while(add)
  {
    prods+=add;
    add/=2;
  }
  return prods;
}
//---------------------------------------------------------------------------
void __fastcall TSupportBuildingForm::FormShow(TObject *Sender)
{
  if(!curRegion)
  {
    sgReg->RowCount=2;
    return;
  }
  sgReg->RowCount=20;
  int row=1,firstdatarow=0;
  AMarkets *marks=new AMarkets;
  marks->Update(curRegion->products);
  {
    AMarket *mark=new AMarket;
    mark->type=ItemTypes->Get("SILV",4);
    mark->amount=curRegion->entertainment;
    if(mark->type)
      marks->Add(mark);
    else
      delete mark;
  }
  foreach(marks)
  {
    AMarket *mark=*(AMarket**)iter;
    if(!mark->amount) continue;
    sgReg->Cells[1][row]=mark->Print(1+2);
    AnsiString item=mark->type->abr;
    AnsiString otype;
    foreach(ObjectTypes)
    {
      AObjectType *_otype=*(AObjectType**)iter;
      if(_otype->production!=item) continue;
      otype=_otype->name;
      break;
    }
    if(otype.Length())
    {
      if(firstdatarow==0)
        firstdatarow=row;
      int count=0;
      foreach(curRegion)
      {
        AObject *obj=*(AObject**)iter;
        if(obj->type!=otype) continue;
        if(obj->incomplete>0) continue;
        count++;
      }
      sgReg->Cells[0][row]=AnsiString(count)+" "+otype;
    }
    row++;
  }
  delete marks;
  sgReg->RowCount=row<2?2:row;
  if(row<2) return;
  if(firstdatarow==0)
    firstdatarow=1;
  sgReg->Row=firstdatarow;
  sgRegDblClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TSupportBuildingForm::sgRegDblClick(TObject *Sender)
{
  int prods=atoi(sgReg->Cells[1][sgReg->Row].c_str());
  int builds=atoi(sgReg->Cells[0][sgReg->Row].c_str());
  edBuilds->Text=builds;
  edProds->Text=prods;
  bnCalcClick(0);
}
//---------------------------------------------------------------------------

