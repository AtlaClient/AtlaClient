//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "util.h"
#include "orders.h"
#include "EditOrderAutoMoveFrm.h"
#include "unit.h"
#include "region.h"
#include "EditOrderMovingFrm.h"
#include "turn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderAutoMoveForm *EditOrderAutoMoveForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderAutoMoveForm::TEditOrderAutoMoveForm(TComponent* Owner, AUnit *_un,AOrder *ord)
    : TForm(Owner),un(_un)
{
 Left=0;
 lastindex=0;
 ords=new AOrders(_un);
 ords->autodelete=false;
 foreach(_un->orders){
  AOrder *o=*(AOrder**)iter;
  if(o->type!=O_AUTOMOVE)continue;
  if(o==ord) lastindex=ords->count;
  ords->Add(o);
 }
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoMoveForm::FormShow(TObject *Sender)
{
 Show();
}
//---------------------------------------------------------------------------
void TEditOrderAutoMoveForm::Show()
{
 curord=0;
 labMess->Caption="";
 int maxsize=0;
 lbOrders->Items->BeginUpdate();
 lbOrders->Items->Clear();
 AOrderAutoMoving *prev=0;
 bool hascurrent=false,issail=false;
 foreach(ords){
  AOrderAutoMoving *o=*(AOrderAutoMoving**)iter;
  if(o->begloc.xloc<0){
   if(!prev){
    ARegion *reg=un->baseobj->basereg;
    o->begloc=*reg;
   }else{
    ALocation loc=prev->GetLoc(1000);
    o->begloc=loc;
   }
  }
  if(!prev){
   cbSail->Checked=issail=o->issail;
  }else{
   o->issail=issail;
  }
  if(o->repeating){
   if(hascurrent){
    ShowMessage("Too many current orders");
    o->repeating=false;
   }else
    hascurrent=true;
  }
  prev=o;
  AnsiString str=o->Print();
  lbOrders->Items->Add(str);
  TSize siz=lbOrders->Canvas->TextExtent(str);
  if(siz.cx>maxsize)maxsize=siz.cx;
 }
 if(!hascurrent){
  labMess->Caption="Warning: no current order";
 }
 lbOrders->Items->EndUpdate();
 lbOrders->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
 lbOrders->ItemIndex=lastindex;
 lbOrdersClick(0);
}
void __fastcall TEditOrderAutoMoveForm::lbOrdersClick(TObject *Sender)
{
 int ind= lbOrders->ItemIndex;
 if(ind<0){
  curord=0;
  return;
 }
 lastindex=ind;
 curord=(AOrderAutoMoving*)ords->Get(ind);
 cbCurrent->Checked=curord->repeating;
 cbCommented->Checked=curord->commented;
 edX->Text=curord->begloc.xloc;
 edY->Text=curord->begloc.yloc;
 edZ->Text=curord->begloc.zloc;

 curreg=curRegionList->Get(curord->begloc);
 if(!curreg){
  labReg->Caption="unknown reg";
 }else{
  labReg->Caption=curreg->FullName(false);
 }
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoMoveForm::cbCommentedClick(TObject *Sender)
{
 cbCurrent->Enabled=!cbCommented->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoMoveForm::bnEditClick(TObject *Sender)
{
 if(!curord) return;
 AOrder *ord;
 AObject *baseobj=un->baseobj;
 ARegion *basereg=baseobj->basereg;
 if(!curreg){
  AnsiString mes="Base region does not exist or not explored";
  ShowMessage(mes);
  return;
 }
 if(!curord->issail)
 {
   AObject *curobj=curreg->GetDummy();
   un->baseobj=curobj;
 }
 else
 {
   baseobj->basereg=curreg;
 }
 TEditOrderMovingForm *frm=new TEditOrderMovingForm(0,curord);
 frm->preparemode=true;
 frm->ShowModal();
 ord=frm->ord;
 delete frm;
 if(curord->issail)
 {
   baseobj->endreg=basereg;
   baseobj->basereg=basereg;
 }
 else
   un->baseobj=baseobj;
 if(ord){
  Show();
 }
}
//---------------------------------------------------------------------------

void __fastcall TEditOrderAutoMoveForm::bnApplyClick(TObject *Sender)
{
 if(cbCurrent->Checked&&cbCurrent->Enabled){
  foreach(ords){
   AOrderAutoMoving *o=*(AOrderAutoMoving**)iter;
   o->repeating=(o==curord);
  }
 }else{
  curord->repeating=false;
 }
 curord->commented=cbCommented->Checked;
 curord->begloc.xloc=atoi(edX->Text.c_str());
 curord->begloc.yloc=atoi(edY->Text.c_str());
 curord->begloc.zloc=atoi(edZ->Text.c_str());
 Show();
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoMoveForm::bnGetPosClick(TObject *Sender)
{
 edX->Text="-1";
 bnApplyClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoMoveForm::cbSailClick(TObject *Sender)
{
 if(!curord) return;
 AOrder *ord=ords->Get(0);
 ord->issail=cbSail->Checked;
 Show();
}
//---------------------------------------------------------------------------

void __fastcall TEditOrderAutoMoveForm::bnAddClick(TObject *Sender)
{
 AOrder *ord=new AOrderAutoMoving;
 un->orders->Add(ord);
 lastindex=ords->count;
 ords->Add(ord);
 Show();
}
//---------------------------------------------------------------------------


void __fastcall TEditOrderAutoMoveForm::bnDeleteClick(TObject *Sender)
{
 if(!curord) return;
 AnsiString q="Delete order\n"+curord->Print();
 if(Application->MessageBox(q.c_str(),"Deleting",MB_OKCANCEL)!=IDOK) return;
 un->orders->Delete(curord);
 ords->Delete(curord);
 int count=ords->count;
 if(!count){
  bnClose->Click();
  return;
 }
 if(lastindex>=count) lastindex=count-1;
 Show();
}
//---------------------------------------------------------------------------
bool TEditOrderAutoMoveForm::Swap(int ind1, int ind2)
{
 if(ind1<0||ind2<0) return false;
 if(ind1>=ords->count||ind2>=ords->count) return false;
 AOrders *bords=un->orders;
 int bind1=bords->IndexOf(ords->Get(ind1));
 int bind2=bords->IndexOf(ords->Get(ind2));
 if(bind1<0||bind2<0) return false;
 ords->Swap(ind1,ind2);
 return bords->Swap(bind1,bind2);
}

void __fastcall TEditOrderAutoMoveForm::bnMoveUpClick(TObject *Sender)
{
 if(Swap(lastindex-1,lastindex))
  lastindex--;
 Show();
}
//---------------------------------------------------------------------------

void __fastcall TEditOrderAutoMoveForm::bnDownClick(TObject *Sender)
{
 if(Swap(lastindex,lastindex+1))
  lastindex++;
 Show();
}
//---------------------------------------------------------------------------

