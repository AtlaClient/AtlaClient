//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "WedgeModeSetupFrm.h"
#include "util.h"
#include "exprlang.h"
#include "region.h"
#include "turn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TWedgeModeSetupForm *WedgeModeSetupForm;
//---------------------------------------------------------------------------
AnsiString wedgeexpr[6]={
 "presentfaction==localfaction",
 "guardfaction==localfaction",
 "presentfaction==10"
};
extern TColor wedgecolors[6];
__fastcall TWedgeModeSetupForm::TWedgeModeSetupForm(TComponent* Owner)
    : TForm(Owner)
{
 ExprSetRegionMode(curRegion);
 LabelReg->Caption=curRegion->FullName(false);
 edits[0]=Edit1;
 edits[1]=Edit2;
 edits[2]=Edit3;
 edits[3]=Edit4;
 edits[4]=Edit5;
 edits[5]=Edit6;

 labels[0]=Label1;
 labels[1]=Label2;
 labels[2]=Label3;
 labels[3]=Label4;
 labels[4]=Label5;
 labels[5]=Label6;

 colors[0]=Label11;
 colors[1]=Label12;
 colors[2]=Label13;
 colors[3]=Label14;
 colors[4]=Label15;
 colors[5]=Label16;
 for(int i=0;i<6;i++){
  edits[i]->Text=wedgeexpr[i];
  colors[i]->Color=wedgecolors[i];
 }
 CalcExpr();
 TMenuItem *mi;
 mi=new TMenuItem(PopupMenu);
 PopupMenu->Items->Add(mi);
 mi->Caption="-";
 ExprCreateMenu(PopupMenu->Items,emRegion,ExprClick);
 Grid->Cells[0][0]="Expressions";
 Grid->Cells[1][0]="Comments";
 int rowcount=curTurns->expr_comms->Count/2;
 Grid->RowCount=rowcount?rowcount+1:2;
 for(int i=0;i<rowcount;i++){
  Grid->Cells[0][i+1]=curTurns->expr_comms->Strings[i*2];
  Grid->Cells[1][i+1]=curTurns->expr_comms->Strings[i*2+1];
 }
}
//---------------------------------------------------------------------------
void __fastcall TWedgeModeSetupForm::bnOkClick(TObject *Sender)
{
 if(!CalcExpr()){
  ModalResult=mrNone;
  return;
 }
 for(int i=0;i<6;i++){
  wedgeexpr[i]=edits[i]->Text;
 }
 int rc=Grid->RowCount-1;
 TStringList *list=new TStringList;
 for(int i=1;i<=rc;i++){
  AnsiString expr=Grid->Cells[0][i];
  AnsiString rem=Grid->Cells[1][i];
  if(!expr.Length()&&!rem.Length()) continue;
  list->Add(expr);
  list->Add(rem);
 }
 curTurns->expr_comms->Assign(list);
 delete list;
}
//---------------------------------------------------------------------------
bool TWedgeModeSetupForm::CalcExpr()
{
 int i;
 try{
  ExprSetRegionMode(curRegion);
  for(i=0;i<6;i++){
   bool res=ExprProcessExpr(edits[i]->Text);
   labels[i]->Caption=res?"true":"false";
  }
 }catch (Exception &exception){
  Application->ShowException(&exception);
  if(Visible) edits[i]->SetFocus();
  return false;
 }
 return true;
}
void __fastcall TWedgeModeSetupForm::EditEnter(TObject *Sender)
{
 lastedit=((TComponent*)Sender)->Tag;
}
void __fastcall TWedgeModeSetupForm::EditExit(TObject *Sender)
{
// CalcExpr();
}
void __fastcall TWedgeModeSetupForm::EditKeyPress(TObject *Sender,
      char &Key)
{
 if(Key==13){
  EditExit(Sender);
  Key=0;
 }
}
void __fastcall TWedgeModeSetupForm::bnExprClick(TObject *Sender)
{
 TPoint p(bnExpr->Left,bnExpr->Top+bnExpr->Height);
 p=ClientToScreen(p);
 PopupMenu->Popup(p.x,p.y);
}
void __fastcall TWedgeModeSetupForm::ExprClick(TObject *Sender)
{
 TMenuItem *mi=dynamic_cast<TMenuItem*>(Sender);
 AnsiString str=mi->Caption;
 if(str=="&&&&") str="&&";
 edits[lastedit]->SelText=str;
 int start=edits[lastedit]->SelStart;
 edits[lastedit]->SetFocus();
 edits[lastedit]->SelStart=start;
}
void __fastcall TWedgeModeSetupForm::bnVerifyClick(TObject *Sender)
{
 CalcExpr();
}
void __fastcall TWedgeModeSetupForm::bnInsertClick(TObject *Sender)
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
void __fastcall TWedgeModeSetupForm::bnAddClick(TObject *Sender)
{
 Grid->RowCount=Grid->RowCount+1;
 Grid->Row=Grid->RowCount-1;
 AnsiString str;
 for(int j=0;j<2;j++)
  Grid->Cells[j][Grid->Row]=str;
}
void __fastcall TWedgeModeSetupForm::bnDelClick(TObject *Sender)
{
 if(Grid->RowCount<=2) return;
 int newrc=Grid->RowCount-1;
 for(int i=Grid->Row;i<newrc;i++){
  Grid->Cells[0][i]=Grid->Cells[0][i+1];
  Grid->Cells[1][i]=Grid->Cells[1][i+1];
 }
 Grid->RowCount=newrc;
}
void __fastcall TWedgeModeSetupForm::bnSetClick(TObject *Sender)
{
 TComponent*cmp=dynamic_cast<TComponent*>(Sender);
 if(!cmp) return;
 int ind=cmp->Tag;
 AnsiString str=Grid->Cells[0][Grid->Row];
 if(str==":") return;
 edits[ind]->Text=str;
}
void __fastcall TWedgeModeSetupForm::bnGetClick(TObject *Sender)
{
 TComponent*cmp=dynamic_cast<TComponent*>(Sender);
 if(!cmp) return;
 int ind=cmp->Tag;
 Grid->Cells[0][Grid->Row]=edits[ind]->Text;
}
void TWedgeModeSetupForm::FillGroupList(TStrings * list)
{
 list->BeginUpdate();
 list->Clear();
 int rowcount=curTurns->expr_comms->Count/2;
 for(int i=0;i<rowcount;i++){
  AnsiString str=curTurns->expr_comms->Strings[i*2];
  if(str!=":") continue;
  str=curTurns->expr_comms->Strings[i*2+1];
  list->AddObject(str,(TObject*)i);
 }

 list->EndUpdate();
}
void __fastcall TWedgeModeSetupForm::bnLoadClick(TObject *Sender)
{
 TPoint p(bnLoad->Left,bnLoad->Top+bnLoad->Height);
 p=ClientToScreen(p);
 TStringList *list=new TStringList;
 FillGroupList(list);
 pmLoad->Items->Clear();
 for(int i=0;i<list->Count;i++){
  TMenuItem *mi=new TMenuItem(this);
  mi->Caption=list->Strings[i];
  mi->Tag=(int)list->Objects[i];
  mi->OnClick=pmLoadClick;
  pmLoad->Items->Add(mi);
 }
 delete list;
 pmLoad->Popup(p.x,p.y);
}
void __fastcall TWedgeModeSetupForm::pmLoadClick(TObject *Sender)
{
 TMenuItem *mi=dynamic_cast<TMenuItem*>(Sender);
 if(!mi) return;
 int ind=mi->Tag+1;
 for(int i=0;i<6;i++){
  edits[i]->Text="";
 }
 for(int i=0;i<6&&ind<curTurns->expr_comms->Count/2;i++,ind++){
  AnsiString str=curTurns->expr_comms->Strings[ind*2];
  if(str==":") break;
  edits[i]->Text=str;
 }
}
void __fastcall TWedgeModeSetupForm::SaveClick(TObject *Sender)
{
 AnsiString str;
 if(!InputQuery("Saving Wedge Group","Enter name",str)) return;
 if(!str.Length()) return;
 int ind=Grid->RowCount;
 Grid->RowCount=ind+7;
 Grid->Row=ind;
 Grid->Cells[0][ind]=":";
 Grid->Cells[1][ind]=str;
 for(int i=0;i<6;i++){
  str=edits[i]->Text;
  if(str.Length())
   Grid->Cells[0][ind+i+1]=str;
  else
   Grid->Cells[1][ind+i+1]=" ";
 }
}

