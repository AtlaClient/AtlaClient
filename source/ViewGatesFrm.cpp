//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "ViewGatesFrm.h"
#include "util.h"
#include "region.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TViewGatesForm *ViewGatesForm;
//---------------------------------------------------------------------------
__fastcall TViewGatesForm::TViewGatesForm(TComponent* Owner)
  : TForm(Owner),locs(new TList)
{
}
//---------------------------------------------------------------------------
void __fastcall TViewGatesForm::FormDestroy(TObject *Sender)
{
  delete locs;
}
//---------------------------------------------------------------------------
static int __fastcall GateCompare(void * Item1, void * Item2)
{
  return ((ARegion*)Item1)->gate-((ARegion*)Item2)->gate;
}
//---------------------------------------------------------------------------
void __fastcall TViewGatesForm::FormShow(TObject *Sender)
{
  locs->Clear();
  foreach(curRegionList)
  {
    ARegion *reg=*(ARegion**)iter;
    if(!reg->gate) continue;
    locs->Add((TObject*) reg);
  }
  if(!locs->Count)
  {
     ShowMessage("No gates found");
     PostMessage(Handle,WM_CLOSE,0,0);
     return;
  }
  locs->Sort(GateCompare);
  Grid->RowCount=locs->Count+1;
  Grid->Cells[0][0]="Number";
  Grid->Cells[1][0]="Region";
  for(int i=0;i<locs->Count;i++)
  {
    ARegion *reg=(ARegion*)locs->Items[i];
    Grid->Cells[0][i+1]=reg->gate;
    Grid->Cells[1][i+1]=reg->FullName(false);
  }
}
//---------------------------------------------------------------------------
void __fastcall TViewGatesForm::GridDblClick(TObject *Sender)
{
  TPoint p=Grid->ScreenToClient(Mouse->CursorPos);
  int ACol,ARow;
  Grid->MouseToCell(p.x,p.y,ACol,ARow);
  if(ARow<1) return;
  ARegion *reg=(ARegion*)locs->Items[ARow-1];
  AtlaForm->GotoRegion(reg);
}
//---------------------------------------------------------------------------

