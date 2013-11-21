//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdlib.h>
#include "util.h"
#include "orders.h"
#include "EditOrderAutoWarningFrm.h"
#include "exprlang.h"
#include "turn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TEditOrderAutoWarningForm *EditOrderAutoWarningForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderAutoWarningForm::TEditOrderAutoWarningForm(TComponent* Owner, AOrderAutoWarning *_ord)
    : TForm(Owner),ord(_ord)
{
  edMess->Text=_ord->mess;
  edExpr->Text=_ord->expr;
  ExprSetUnitMode(_ord->orders->unit);
  CalcExpr();
  TMenuItem *mi;
  mi=new TMenuItem(PopupMenu);
  PopupMenu->Items->Add(mi);
  mi->Caption="-";
  ExprCreateMenu(PopupMenu->Items,emUnit,ExprClick);
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
void __fastcall TEditOrderAutoWarningForm::bnOkClick(TObject *Sender)
{
  if(!CalcExpr()){
    ModalResult=mrNone;
    return;
  }
  ord->mess=edMess->Text;
  ord->expr=edExpr->Text;
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
void __fastcall TEditOrderAutoWarningForm::bnCancelClick(TObject *Sender)
{
  ord=0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoWarningForm::bnVerifyClick(TObject *Sender)
{
  CalcExpr();
}
//---------------------------------------------------------------------------
bool TEditOrderAutoWarningForm::CalcExpr()
{
  int i;
  try{
    bool res=ExprProcessExpr(edExpr->Text);
    labValue->Caption=res?"true":"false";
  }catch (Exception &exception){
    Application->ShowException(&exception);
    if(Visible) edExpr->SetFocus();
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoWarningForm::bnExprClick(TObject *Sender)
{
  TPoint p(bnExpr->Left,bnExpr->Top+bnExpr->Height);
  p=ClientToScreen(p);
  PopupMenu->Popup(p.x,p.y);
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoWarningForm::ExprClick(TObject *Sender)
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
void __fastcall TEditOrderAutoWarningForm::bnSetClick(TObject *Sender)
{
 edExpr->Text=Grid->Cells[0][Grid->Row];
 edMess->Text=Grid->Cells[1][Grid->Row];
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoWarningForm::bnGetClick(TObject *Sender)
{
 Grid->Cells[0][Grid->Row]=edExpr->Text;
 Grid->Cells[1][Grid->Row]=edMess->Text;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoWarningForm::bnInsertClick(TObject *Sender)
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
void __fastcall TEditOrderAutoWarningForm::bnAddClick(TObject *Sender)
{
  Grid->RowCount=Grid->RowCount+1;
  Grid->Row=Grid->RowCount-1;
  AnsiString str;
  for(int j=0;j<2;j++)
    Grid->Cells[j][Grid->Row]=str;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoWarningForm::bnDelClick(TObject *Sender)
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
void __fastcall TEditOrderAutoWarningForm::labValueDblClick(
      TObject *Sender)
{
  AnsiString str=edExpr->Text;
  AnsiString sign="turnnumber!=";
  int pos=str.Pos(sign);
  if(!pos) return;
  pos+=sign.Length();
  char *s=str.c_str()+pos-1,*ss=s;
  GetNextToken(s);
  int l=s-ss;
  str.Delete(pos,l);
  str.Insert(AnsiString(curTurnNumber),pos);
  edExpr->Text=str;
  bnVerifyClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoWarningForm::edMessKeyUp(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  if(Key==VK_F1)
  {
    labValueDblClick(0);
  }
}
//---------------------------------------------------------------------------

