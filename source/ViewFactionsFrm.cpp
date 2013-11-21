//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <clipbrd.hpp>

#include "util.h"
#include "ViewFactionsFrm.h"
#include "faction.h"
#include "turn.h"
#include "MainFrm.h"
#include "reports.h"
#include "unit.h"
#include "orders.h"
#include "ChangeAtt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TViewFactionsForm *ViewFactionsForm;
//---------------------------------------------------------------------------

int TViewFactionsForm::SortCol,TViewFactionsForm::SortType;

int __fastcall DoCompareListItem(void * Item1, void * Item2){
  return ((TFactListItem*)Item1)->CompareWith((TFactListItem*)Item2);
}

__fastcall TViewFactionsForm::TViewFactionsForm(TComponent* Owner)
    : TForm(Owner),FListItems(new TList),headers(new TStringList)
{
  SortCol = 0;
  SortType = 0;
  headers->Add("Name");
  headers->Add("Num");
  headers->Add("Address");
  headers->Add("Attitudes");
  headers->Add("Color");
  headers->Add("Comment");
  Grid->ColCount = 6;
}
//---------------------------------------------------------------------------

__fastcall TViewFactionsForm::~TViewFactionsForm()
{
  ClearList();
  delete FListItems;
  delete headers;
}
//---------------------------------------------------------------------------

TFactListItem::TFactListItem()
 :FText(new TStringList)
{
}
//---------------------------------------------------------------------------

TFactListItem::~TFactListItem()
{
  delete FText;
}
//---------------------------------------------------------------------------

void TFactListItem::Draw(TDrawGrid *Grid,int ACol,TRect &Rect, TGridDrawState State)
{
  AnsiString str;
  if(ACol == 4)
    str = "Unit";
    else str = Texts[ACol];
  TCanvas *canv=Grid->Canvas;
  if(fac->local){
    canv->Font->Style=canv->Font->Style<<fsBold;
  }else{
    canv->Font->Style=canv->Font->Style>>fsBold;
  }
  if(ACol == 4) canv->Font->Color = fac->color;
  if((ACol == 3) && optColorUnit){
    if(curFactions->GetFaction(fac->number)->color != clWindowText)
        canv->Font->Color = curFactions->GetFaction(fac->number)->color;
        else canv->Font->Color = infocolors[curFaction->GetAttitude(fac->number)];
  }
  canv->TextRect(Rect, Rect.Left+2, Rect.Top+2, str);
}
//---------------------------------------------------------------------------

AnsiString __fastcall TFactListItem::GetTexts(int Col)
{
  //if(Col == 4) return 'Unit';
  return FText->Strings[Col];
}
//---------------------------------------------------------------------------

void TFactListItem::UpdateFactionInfo()
{
  FText->Clear();
  if(!fac) return;
  FText->Add(fac->name);
  FText->Add(fac->number);
  FText->Add(fac->address);
  if(!curFaction || curFaction->number == fac->number) FText->Add("");
  else {
    AnsiString str;
    int att = curFaction->GetAttitude(fac->number, true);
    if(att==5) {
      str = AttitudeStrs[curFaction->defattitude];
      str += "(def)";
    } else str = AttitudeStrs[att];
    FText->Add(str);
  }
  FText->Add(fac->color);
  FText->Add(curTurns->FacComments[fac->number]);
}
//---------------------------------------------------------------------------

int TFactListItem::CompareWith(TFactListItem * b)
{
  if(fac->local&&!b->fac->local) return -1;
  if(!fac->local&&b->fac->local) return 1;
  int res=0;
  switch(TViewFactionsForm::SortCol){
    case 0:
    case 2:
    case 3:
    case 4:
      res = Texts[TViewFactionsForm::SortCol].AnsiCompareIC(b->Texts[TViewFactionsForm::SortCol]);
    break;
    case 1:
     res = fac->number-b->fac->number;
    break;
  }
  return TViewFactionsForm::SortType?-res:res;
}
//---------------------------------------------------------------------------

void __fastcall TViewFactionsForm::GridDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
  AnsiString str;
  if(ARow>0){
    ARow--;
    TFactListItem *li=ListItems[ARow];
    if(li) li->Draw(Grid,ACol,Rect,State);
    return;
  }
  str = headers->Strings[ACol];
  TCanvas *Canv = Grid->Canvas;
  if(SortCol == ACol){
    Canv->Pen->Color=clRed;
    Canv->MoveTo(Rect.left+5,Rect.top+4);
    Canv->LineTo(Rect.left+5,Rect.bottom-8);
    if(SortType==1){
      Canv->MoveTo(Rect.left+2,Rect.bottom-14);
      Canv->LineTo(Rect.left+5,Rect.bottom-8);
      Canv->LineTo(Rect.left+8,Rect.bottom-14-1);
    } else {
      Canv->MoveTo(Rect.left+2,Rect.top+10);
      Canv->LineTo(Rect.left+5,Rect.top+4);
      Canv->LineTo(Rect.left+8,Rect.top+10+1);
    }
  }
  Rect.left+=10;
  Canv->TextRect(Rect, Rect.Left, Rect.Top+2, str);
}
//---------------------------------------------------------------------------

void TViewFactionsForm::ClearList()
{
  for(int i=FListItems->Count-1;i>=0;i--){
    TFactListItem *li=(TFactListItem*)FListItems->Items[i];
    delete li;
  }
  FListItems->Clear();
}
//---------------------------------------------------------------------------

TFactListItem * __fastcall TViewFactionsForm::GetListItems(int index)
{
  return (TFactListItem*)FListItems->Items[index];
}
//---------------------------------------------------------------------------

void TViewFactionsForm::AddListItem(TFactListItem * li)
{
  FListItems->Add(li);
}
//---------------------------------------------------------------------------

void TViewFactionsForm::CreateList()
{
  ClearList();

  int facnum = 0;
  foreach(curFactions){
    AFaction *fac=*(AFaction**)iter;
    facnum++;

    TFactListItem *li = new TFactListItem;
    li->fac = fac;
    li->UpdateFactionInfo();
    AddListItem(li);
  }
  Grid->RowCount=facnum+1;
  SortList();
}
//---------------------------------------------------------------------------

void TViewFactionsForm::SortList()
{
  FListItems->Sort(DoCompareListItem);
  bool b;
  GridSelectCell(0,1,1,b);
  Grid->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TViewFactionsForm::FormCreate(TObject *Sender)
{
  CreateList();
  AnsiString  str = "Default attitude : ";
  str += AttitudeStrs[curFaction->defattitude];
  btAttDef->Caption = str;
}
//---------------------------------------------------------------------------

void ExistOrd(AUnit *LastUnit, AOrderSetAtt *ord)
{
  curTurn->Modified=true;

  foreach(LastUnit->orders)
  {
    AOrder *o = *(AOrder**)iter;
    if(o->type != O_DECLARE) continue;
    if(((AOrderSetAtt*)o)->fac == ord->fac)
    {
      ((AOrderSetAtt*)o)->att = ord->att;
      delete ord;
      return;
    }
  }
  
  LastUnit->orders->Add(ord);
}
//---------------------------------------------------------------------------

void __fastcall TViewFactionsForm::GridDblClick(TObject *Sender)
{
  TPoint p = Grid->ScreenToClient(Mouse->CursorPos);
  int r,c;
  Grid->MouseToCell(p.x, p.y, c, r);
  if(r<0) return;
  if(r==0){
    if(SortCol==c){
      SortType=!SortType;
    } else {
      SortCol=c;
      SortType=0;
    }
    SortList();
    return;
  }
  r--;
  TFactListItem *li=ListItems[r];
  if(!li) return;
  AnsiString str, facname=li->fac->FullName();
  bool changed = false;
  switch(c){
    case 2:
      str=li->fac->address;
      if(!InputQuery("Address for "+facname,"",str))return;
      li->fac->address=str;
      changed=true;
      break;
    case 5:
      str=curTurns->FacComments[li->fac->number];
      if(!InputMemo("Comments for "+facname,str))return;
      curTurns->FacComments[li->fac->number]=str;
      changed=true;
      break;
    case 4:
      // Color for Faction
      //ColorDialog->Color
      ColorDialog->Color = li->fac->color;
      if(ColorDialog->Execute()){
        li->fac->color = ColorDialog->Color;
        changed = true;
      }
      break;
    case 3:
      // Change Attitudes
      fChAtt->cbAtt->ItemIndex = curFaction->GetAttitude(li->fac->number, true);
      fChAtt->Caption = facname;
      if(fChAtt->ShowModal() != mrOk) return;

      AUnit *LastUnit = GetLastUnit();
      if(!LastUnit) return;

      curFaction->SetAttitude(li->fac->number, fChAtt->cbAtt->ItemIndex);
      AOrderSetAtt *ord = new AOrderSetAtt();
      ord->att = fChAtt->cbAtt->ItemIndex;
      ord->fac = li->fac->number;
      ExistOrd(LastUnit, ord);

      curTurn->Modified=true;

      changed = true;
      break;
  }
  if(changed){
    curTurn->Modified=true;
    li->UpdateFactionInfo();
    Grid->Invalidate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TViewFactionsForm::GridSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
 if(ARow<=0) return;
 ARow--;
 TFactListItem *li=ListItems[ARow];
 if(!li) return;
 inselectcell=true;
 bool local=li->fac->local;
 bnSetCurrent->Enabled=local&&li->fac!=curFaction;
 bnSendMail->Enabled=((optNoServerMail==false)||li->fac->address.Length())&&li->fac!=curFaction;
 bnSendOrders->Enabled=local;

 cbFacDeleted->Checked=li->fac->deleted;

 inselectcell=false;
}
//---------------------------------------------------------------------------

void __fastcall TViewFactionsForm::bnSetCurrentClick(TObject *Sender)
{
  int r=Grid->Row;
  if(r<=0) return;
  r--;
  TFactListItem *li=ListItems[r];
  if(!li) return;
  curFactions->SetCurFaction(li->fac->number);
  CreateList();
  Grid->Invalidate();
  bool b;
  GridSelectCell(0,Grid->Col,Grid->Row,b);
}
//---------------------------------------------------------------------------

void __fastcall TViewFactionsForm::bnSendMailClick(TObject *Sender)
{
 int r=Grid->Row;
 if(r<=0) return;
 r--;
 TFactListItem *li=ListItems[r];
 if(!li) return;
 bool servmail=!optNoServerMail;
// if(li->fac->address.Length()) servmail=false;
// else if(!servmail) return;
 if(!li->fac->address.Length()&&!servmail) return;
 AnsiString str="mailto:";
 if(!servmail){
  str+=li->fac->address;
  str+="?subject=";
  str+=StrForURL("["+curGame+"]");
  str+="&body=";
 }else{
  str+=optServEMail;
  str+="?subject=";
  str+=StrForURL(optServMailSubj);
  str+="&body=";
  str+=StrForURL(AnsiString("#mail ")+li->fac->number+" "+curFaction->number+
   +" \""+curFaction->password+"\"\n");
 }
 str+=StrForURL("  Hi,"+li->fac->name+"!\n\n  "+curFaction->FullName()+".");
 ShellExecute(0,"open",str.c_str(),0,0,SW_SHOW);
}
//---------------------------------------------------------------------------

void __fastcall TViewFactionsForm::cbFacDeletedClick(TObject *Sender)
{
 if(inselectcell) return;
 int r=Grid->Row;
 if(r<=0) return;
 r--;
 TFactListItem *li=ListItems[r];
 if(!li) return;
 li->fac->deleted=cbFacDeleted->Checked;
 curTurn->Modified=true;
 Grid->SetFocus(); 
}
//---------------------------------------------------------------------------

void __fastcall TViewFactionsForm::bnSendOrdersClick(TObject *Sender)
{
 int ARow=Grid->Row;
 if(ARow<=0) return;
 ARow--;
 TFactListItem *li=ListItems[ARow];
 if(!li) return;
 AFaction *fac=li->fac;
 if(!fac->local) return;
 int facnum=fac->number;
 AnsiString fName=GetOrderName(facnum);
 if(!SaveOrder(fName,facnum)){
  AnsiString str="Error saving order for faction ";
  str+=fac->FullName();
  AddEvent(str);
  return;
 }
 TStringList *list=new TStringList;
 try{
  list->LoadFromFile(fName);
  Clipboard()->AsText=list->Text;
  AnsiString str="mailto:";
  str+=optServEMail;
  str+="?subject=";
  str+=StrForURL(optServOrdersSubj);
  ShellExecute(0,"open",str.c_str(),0,0,SW_SHOW);
  str="Orders for "+fac->FullName()+" copied to Clipboard";
  Application->MessageBox(str.c_str(),"Atlaclient",MB_OK);
 }catch(Exception&c){
 }
 delete list;
}
//---------------------------------------------------------------------------

void __fastcall TViewFactionsForm::bnFindAddressClick(TObject *Sender)
{
  if(!curUnit) return;
  if(!curUnit->faction->local) return;
  int ARow=Grid->Row;
  if(ARow<=0) return;
  ARow--;
  TFactListItem *li=ListItems[ARow];
  if(!li) return;
  AFaction *fac=li->fac;
  if(fac->local) return;
  int facnum=fac->number;
  AOrderCustom *ord=new AOrderCustom;
  ord->text=AnsiString("find ")+facnum;
  curUnit->orders->Add(ord);
  curTurn->Modified=true;
}
//---------------------------------------------------------------------------

AUnit * TViewFactionsForm::GetLastUnit()
{
   // Последняя карта
   /*
   ARegionArray *ra = curRegionList->RegionArrays->Get(curRegionList->RegionArrays->count-1);
   ARegion *reg = ra->Get(ra->x, ra->y);
   Caption = reg->type;
   reg->PresentLocals
   */
   AUnit *LastUnit = 0;

   for(int i = curRegionList->count-1 ; i>=0; i--){
     ARegion *reg = curRegionList->Get(i);
     if(!reg->PresentLocals()) continue;
     foreach(reg){
        AObject *obj=*(AObject **) iter;
        foreach(obj->units){
          AUnit *unit=*(AUnit**)iter;
          if((unit->faction->number == curFaction->number) && (unit->num > 0))
             LastUnit = unit;
        }
     }
     if(LastUnit) return LastUnit;
   }
   return 0;
}
//---------------------------------------------------------------------------

void __fastcall TViewFactionsForm::btAttDefClick(TObject *Sender)
{
  fChAtt->cbAtt->ItemIndex = curFaction->defattitude;
  fChAtt->Caption = "Default";
  if(fChAtt->ShowModal() != mrOk) return;
  if(fChAtt->cbAtt->ItemIndex == 5) return;

  AUnit *LastUnit = GetLastUnit();
  if(!LastUnit) return;
  curFaction->defattitude = fChAtt->cbAtt->ItemIndex;
  AOrderSetAtt *ord = new AOrderSetAtt();
  ord->att = curFaction->defattitude;
  ord->fac = 0;
  ExistOrd(LastUnit, ord);

  CreateList();
  Grid->Invalidate();
  AnsiString  str = "Default attitude : ";
  str += AttitudeStrs[curFaction->defattitude];
  btAttDef->Caption = str;

}
//---------------------------------------------------------------------------

