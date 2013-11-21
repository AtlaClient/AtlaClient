//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "ExpressionEditorFrm.h"
#include "exprlang.h"
#include "unit.h"
#include "turn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TExpressionEditorForm *ExpressionEditorForm;
//---------------------------------------------------------------------------
__fastcall TExpressionEditorForm::TExpressionEditorForm(TComponent* Owner)
  : TForm(Owner),unit(0),reg(0),numbermode(false)
{
}
//---------------------------------------------------------------------------
void __fastcall TExpressionEditorForm::FormShow(TObject *Sender)
{
  edExpr->Text=expr;
  if(mode&emUnit)
  {
    if(!unit) unit=curUnit;
    if(!unit)
    {
      PostMessage(Handle,WM_CLOSE,0,0);
      return;
    }
    ExprSetUnitMode(unit);
  }else
  {
    if(!reg) reg=curRegion;
    if(!reg)
    {
      PostMessage(Handle,WM_CLOSE,0,0);
      return;
    }
    ExprSetRegionMode(reg);
  }
  CalcExpr();
  TMenuItem *mi;
  mi=new TMenuItem(PopupMenu);
  PopupMenu->Items->Add(mi);
  mi->Caption="-";
  ExprCreateMenu(PopupMenu->Items,mode,ExprClick);
  Grid->Cells[0][0]="Expressions";
  Grid->Cells[1][0]="Messages";
  int rowcount=curTurns->warning_exprs->Count/2;
  Grid->RowCount=rowcount?rowcount+1:2;
  for(int i=0;i<rowcount;i++){
    Grid->Cells[0][i+1]=curTurns->warning_exprs->Strings[i*2];
    Grid->Cells[1][i+1]=curTurns->warning_exprs->Strings[i*2+1];
  }
}
//---------------------------------------------------------------------------
void __fastcall TExpressionEditorForm::bnOkClick(TObject *Sender)
{
  if(!CalcExpr()){
    ModalResult=mrNone;
    return;
  }
  expr=edExpr->Text;
  int rc=Grid->RowCount-1;
  TStringList *list=new TStringList;
  for(int i=1;i<=rc;i++){
    AnsiString expr=Grid->Cells[0][i];
    AnsiString rem=Grid->Cells[1][i];
    if(!expr.Length()&&!rem.Length()) continue;
    list->Add(expr);
    list->Add(rem);
  }
  curTurns->warning_exprs->Assign(list);
  delete list;
}
//---------------------------------------------------------------------------
void __fastcall TExpressionEditorForm::bnVerifyClick(TObject *Sender)
{
  CalcExpr();
}
//---------------------------------------------------------------------------
bool TExpressionEditorForm::CalcExpr()
{
  int i;
  try{
    int res=ExprProcessExpr(edExpr->Text);
    if(numbermode)
      labValue->Caption=res;
    else
      labValue->Caption=res!=0?"true":"false";
  }catch (Exception &exception){
    Application->ShowException(&exception);
    if(Visible) edExpr->SetFocus();
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TExpressionEditorForm::bnExprClick(TObject *Sender)
{
  TPoint p(bnExpr->Left,bnExpr->Top+bnExpr->Height);
  p=ClientToScreen(p);
  PopupMenu->Popup(p.x,p.y);
}
//---------------------------------------------------------------------------
void __fastcall TExpressionEditorForm::ExprClick(TObject *Sender)
{
  TMenuItem *mi=dynamic_cast<TMenuItem*>(Sender);
  AnsiString str=mi->Caption;
  if(str=="&&&&") str="&&";
  edExpr->SelText=str;
  int start=edExpr->SelStart;
  edExpr->SetFocus();
  edExpr->SelStart=start;
}
//---------------------------------------------------------------------------
void __fastcall TExpressionEditorForm::bnSetClick(TObject *Sender)
{
 edExpr->Text=Grid->Cells[0][Grid->Row];
}
//---------------------------------------------------------------------------
void __fastcall TExpressionEditorForm::bnGetClick(TObject *Sender)
{
 Grid->Cells[0][Grid->Row]=edExpr->Text;
}
//---------------------------------------------------------------------------
void __fastcall TExpressionEditorForm::bnInsertClick(TObject *Sender)
{
  Grid->RowCount=Grid->RowCount+1;
  for(int i=Grid->RowCount-1,endi=Grid->Row;i>endi;i--){
    for(int j=0;j<2;j++)
     Grid->Cells[j][i]=Grid->Cells[j][i-1];
  }
  AnsiString str;
  for(int j=0;j<2;j++)
    Grid->Cells[j][Grid->Row]=str;
}
//---------------------------------------------------------------------------
void __fastcall TExpressionEditorForm::bnAddClick(TObject *Sender)
{
  Grid->RowCount=Grid->RowCount+1;
  Grid->Row=Grid->RowCount-1;
  AnsiString str;
  for(int j=0;j<2;j++)
    Grid->Cells[j][Grid->Row]=str;
}
//---------------------------------------------------------------------------
void __fastcall TExpressionEditorForm::bnDelClick(TObject *Sender)
{
  if(Grid->RowCount<=2) return;
  int newrc=Grid->RowCount-1;
  for(int i=Grid->Row;i<newrc;i++){
    Grid->Cells[0][i]=Grid->Cells[0][i+1];
    Grid->Cells[1][i]=Grid->Cells[1][i+1];
  }
  Grid->RowCount=newrc;
}
//---------------------------------------------------------------------------

