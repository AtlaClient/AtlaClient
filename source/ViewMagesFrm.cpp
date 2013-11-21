//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <algorithm>
#include "ViewMagesFrm.h"
#include "region.h"
#include "unit.h"
#include "turn.h"
#include "faction.h"
#include "skills.h"
#include "orders.h"
#include "MainFrm.h"
#include "gameparams.h"
#include "reports.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TViewMagesForm *ViewMagesForm;

const int SkillsRow=5;
int TViewMagesForm::FSortRow=-1,TViewMagesForm::FSortDir=true;
int TViewMagesForm::ColsWidth=150;
bool TViewMagesForm::LocalOnly=false;
bool TViewMagesForm::CurRegOnly=false;
TStringList *TViewMagesForm::found,*TViewMagesForm::magic;

class MageInfo{
public:
 AUnit *unit;
 AnsiString name;
 AnsiString regname;
 AnsiString monthorder;
 AnsiString castorder;
 int teachernum;
 AnsiString teacherstr;
 void Update();
 virtual void Draw(TDrawGrid *Grid,int ARow,TRect &Rect, TGridDrawState State);
 int CompareWith(const MageInfo &b) const;

 friend int operator<(const MageInfo&a,const MageInfo&b){
  return a.CompareWith(b)<0;
 }
};
AnsiString FindOrderKey(OrderTypes type); //in orders.cpp

void MageInfo::Update(){
 name=unit->FullName(false);
 regname=unit->endobj->endreg->FullName(false);
 AOrder *ord;
 ord=unit->orders->GetMonthLong();
 if(ord){
	if(ord->type==O_STUDY){
	 monthorder=ord->Print();
/*
	 AOrderAbr *_ord=(AOrderAbr*)ord;
	 int days=unit->GetSkillDays(_ord->abr)-unit->GetBegSkillDays(_ord->abr);
	 if(days==0)
		 monthorder="-"+monthorder;
	 else if(days>StudyAddDays)
		 monthorder="+"+monthorder;
*/		 
  }else if(ord->type==O_TEACH){
   monthorder=(ord->repeating?"@":"");
   monthorder+=FindOrderKey(O_TEACH);
  }else{
   monthorder=ord->Print();
  }
 }else{
  monthorder.SetLength(0);
 }
 ord=unit->orders->Find(O_CAST,2);
 if(ord){
  castorder=ord->Print();
 }else{
  castorder.SetLength(0);
 }
 teachernum=0;
 AnsiString beginstr="Taught by ";
 foreach(unit->orders)
 {
	 AOrder *_ord=*(AOrder**)iter;
	 if(_ord->type!=O_MESSAGE) continue;
	 AOrderMessage *ord=(AOrderMessage*)_ord;
	 if(MyCompare(ord->text.c_str(),beginstr)) continue;
	 char *s=ord->text.c_str()+beginstr.Length();
	 int num=0;
	 if(!ExtractBattleUnitName(s,num,0)) continue;
	 teachernum=num;
	 break;
 }
 teacherstr=EmptyStr;
 if(teachernum)
 {
	 AUnit *t=unit->baseobj->basereg->GetUnit(teachernum);
	 if(t)
		teacherstr=t->FullName(false);
 }		
}
void MageInfo::Draw(TDrawGrid *Grid,int ARow,TRect &Rect, TGridDrawState State){
 AnsiString str;
 TCanvas *Canv=Grid->Canvas;
 if(ARow>=SkillsRow){
  AnsiString abr=TViewMagesForm::magic->Strings[ARow-SkillsRow];
  ASkill *bsk=unit->skills->GetSkill(abr);
  ASkill *sk=unit->endskills->GetSkill(abr);
  int days=0,lev=0;
  if(bsk){
   str=bsk->Print(4+8);
   str.Delete(1,1);
   days=bsk->days;
   lev=bsk->GetLevel();
   if(lev>=unit->GetMaxLevel(abr))
    Canv->Brush->Color=clBtnFace;
  }
  if(sk){
   days=sk->days-days;
   if(days>0){
    lev=sk->GetLevel()-lev;
    str+="+";
    str+=lev;
    str+="(";
    str+=days;
    str+=")";
   }
  }
  if(!unit->BegCanStudy(abr))
   Canv->Brush->Color=clBtnFace;
 }else{
  switch(ARow){
   case 0:str=name;break;
   case 1:str=regname;break;
	 case 2:str=monthorder;break;
	 case 3:str=castorder;break;
	 case 4:str=teacherstr;break;
  }
 }
 Canv->TextRect(Rect, Rect.Left+2, Rect.Top+2, str);
}
int MageInfo::CompareWith(const MageInfo &b) const{
 int res;
 switch(TViewMagesForm::FSortRow){
  case 0:
   res=name.AnsiCompare(b.name);
  break;
  case 1:
   res=regname.AnsiCompare(b.regname);
  break;
  case 2:
   res=monthorder.AnsiCompare(b.monthorder);
  break;
  case 3:
	 res=castorder.AnsiCompare(b.castorder);
	break;
	case 4:
	 res=teacherstr.AnsiCompare(b.teacherstr);
	break;
  default:{
   AnsiString curSkill=TViewMagesForm::magic->Strings[TViewMagesForm::FSortRow-SkillsRow];
   int alev=unit->GetBegSkillDays(curSkill);
   int blev=b.unit->GetBegSkillDays(curSkill);
   res=alev-blev;
   if(res==0)
     res=unit->GetSkillDays(curSkill)-b.unit->GetSkillDays(curSkill);
  }
 }
 return TViewMagesForm::FSortDir?res:-res;
}
//---------------------------------------------------------------------------
__fastcall TViewMagesForm::TViewMagesForm(TComponent* Owner)
    : TForm(Owner),list(new MageInfos)
    ,regs(new TList),headers(new TStringList)
{
 found=new TStringList;magic=new TStringList;
 miLocalOnly->Checked=LocalOnly;
 miCurRegOnly->Checked=CurRegOnly;

/* Grid->ColWidths[1]=150;
 Grid->ColWidths[2]=100;
 Grid->ColWidths[3]=100;*/
 headers->Add("Name");
 headers->Add("EndRegion");
 headers->Add("MonthOrder");
 headers->Add("CastOrder");
 headers->Add("Teacher");
 Grid->FixedRows=SkillsRow;
 Init();
}
void __fastcall TViewMagesForm::FormDestroy(TObject *Sender)
{
 delete list;list=0;
 delete found;found=0;
 delete magic;magic=0;
 delete regs;regs=0;
 delete headers;headers=0;
}
void TViewMagesForm::Init()
{
 list->clear();
 found->Clear();
 magic->Clear();
 regs->Clear();

 TStringList *extra_magic_skills=curTurns->extra_magic_skills;
 foreach(SkillTypes){
  ASkillType *stype=*(ASkillType**)iter;
  if(stype->flags&ASkillType::FOUNDATION) found->Add(stype->abr);
  if(stype->flags&ASkillType::MAGIC) magic->Add(stype->abr);
  else if(extra_magic_skills->IndexOf(stype->abr)>=0) magic->Add(stype->abr);
 }

 foreach(curRegionList){
  ARegion *reg=*(ARegion**)iter;
	if(CurRegOnly&&reg!=curRegion)
    continue;
  bool presentmage=false;
  foreach(reg){
   AObject *obj=*(AObject**)iter;
   foreach(obj->units){
    AUnit *un=*(AUnit**) iter;
    if(!un->IsLocal()) continue;
    if(LocalOnly&&!un->faction->local) continue;
		bool ismage=false;
    for(int i=0;i<found->Count;i++){
     if(!un->GetSkill(found->Strings[i])) continue;
     ismage=true;
     break;
    }
    if(!ismage) continue;
    presentmage=true;
    MageInfo mi;
    mi.unit=un;
    list->push_back(mi);
   }
  }
  if(!presentmage) continue;
  regs->Add(reg);
 }
 if(!list->size())
 {
   if(!CurRegOnly)
		 PostMessage(Handle,WM_CLOSE,0,0);
   else{
     CurRegOnly=false;
     miCurRegOnly->Checked=CurRegOnly;
     Init();
   }
   return;
 }
 RunOrders();
 if(SortRow<0||SortRow>=SkillsRow+magic->Count)
   SortRow=0;
 else
 {
   int sr=SortRow;
   FSortRow=-1;
   SortRow=sr;
 }

 miPrevTurn->Enabled=AtlaForm->bnPrevTurn->Enabled;
 miNextTurn->Enabled=AtlaForm->bnNextTurn->Enabled;
}

void TViewMagesForm::MakeGrid()
{
 Grid->RowCount=SkillsRow+magic->Count;
 int siz=list->size();
 if(siz==0){
  Grid->ColCount=2;
	return;
 }
 if(ColsWidth<30) ColsWidth=30;
 Grid->DefaultColWidth=ColsWidth;
 Grid->ColWidths[0]=70;
 Grid->ColCount=siz+1;
}
void __fastcall TViewMagesForm::SetSortRow(int value)
{
 if(FSortRow == value) return;
 FSortRow = value;
 if(value==-1) return;
 stable_sort(list->begin(),list->end());
 MakeGrid();
 Grid->Invalidate();
}
void __fastcall TViewMagesForm::GridDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
 AnsiString str;
 if(ACol>0){
  MageInfo &w=list->at(ACol-1);
  w.Draw(Grid,ARow,Rect,State);
  return;
 }
 if(ARow<SkillsRow)
  str=headers->Strings[ARow];
 else
  str=magic->Strings[ARow-SkillsRow];
 TCanvas *Canv=Grid->Canvas;
 if(ARow==SortRow){
  Canv->Pen->Color=clRed;
  Canv->MoveTo(Rect.left+4,Rect.top+4);
  Canv->LineTo(Rect.left+4,Rect.bottom-8);
  if(FSortDir){
   Canv->MoveTo(Rect.left+1,Rect.bottom-14);
   Canv->LineTo(Rect.left+4,Rect.bottom-8);
   Canv->LineTo(Rect.left+7,Rect.bottom-14-1);
  }else{
   Canv->MoveTo(Rect.left+1,Rect.top+10);
   Canv->LineTo(Rect.left+4,Rect.top+4);
   Canv->LineTo(Rect.left+7,Rect.top+10+1);
  }
 }
 Rect.left+=10;
 Canv->TextRect(Rect, Rect.Left, Rect.Top+2, str);
}
static int __fastcall MagicSort(TStringList *List, int Index1, int Index2){
 int val1=(int)List->Objects[Index1];
 int val2=(int)List->Objects[Index2];
 return val2-val1;
}
void TViewMagesForm::RunOrders()
{
 DisableEvents();
 try{
   for(int i=0;i<regs->Count;i++){
    ARegion *reg=(ARegion*)regs->Items[i];
    reg->RunOrders(false);
   }
 }catch(...){
 }
 EnableEvents();
 TStringList *first_magic_skills=curTurns->first_magic_skills;
 TStringList *last_magic_skills=curTurns->last_magic_skills;
 for(MageInfo *mi=list->begin().operator ->(),*endmi=list->end().operator ->();mi<endmi;mi++)
   mi->Update();
 for(int i=0;i<magic->Count;i++){
  int s=0;
  AnsiString abr=magic->Strings[i];
  int ind=first_magic_skills->IndexOf(abr);
  if(ind>=0)
    s=1000000-ind;
  else
  {
    ind=last_magic_skills->IndexOf(abr);
    if(ind>=0)
      s=-1000000-ind;
    else  
    {
	  for(MageInfo *mi=list->begin().operator ->(),*endmi=list->end().operator ->();mi<endmi;mi++){
       int lev=mi->unit->GetSkill(abr);
       if(lev)
        s++;
      }
      s=s*1000-i;
    }
  }
  magic->Objects[i]=(TObject*)s;
 }
 magic->CustomSort(MagicSort);
}
void TViewMagesForm::EditOrder(AOrders * orders, AOrder * ord, bool neworder)
{
 DisableEvents();
 if(neworder){
  orders->Add(ord);
 }
 AOrder *neword=ord->Edit();
 if(neworder){
  if(ord!=neword){
   orders->Delete(ord);
   if(neword)
    orders->Add(neword);
  }
 }else{
  if(neword)
   if(ord!=neword)
    orders->Replace(ord,neword);
 }
 if(neword)
 {
  curTurn->Modified=true;
  neword->orders->unit->baseobj->basereg->runned=false;
 }
 EnableEvents();
 RunOrders();
 for(MageInfo *mi=list->begin().operator ->(),*endmi=list->end().operator ->();mi<endmi;mi++){
  mi->Update();
 }
 int c=SortRow;
 FSortRow=-1;
 SortRow=c;
}

void __fastcall TViewMagesForm::GridDblClick(TObject *Sender)
{
 TPoint p=Grid->ScreenToClient(Mouse->CursorPos);
 int ACol,ARow;
 Grid->MouseToCell(p.x,p.y,ACol,ARow);
 if(ACol<0) return;
 if(ACol==0){
  if(SortRow==ARow){
   FSortDir=!FSortDir;
   FSortRow=-1;
  }else{
   FSortDir=ARow<SkillsRow;
  }
  SortRow=ARow;
 }else if(ARow==0){
  MageInfo &w=list->at(ACol-1);
  AtlaForm->GotoUnit(w.unit);
  ModalResult=mrOk;
  return;
 }else if(ARow==1){
  MageInfo &w=list->at(ACol-1);
	AtlaForm->GotoRegion(w.unit->baseobj->basereg);
	ModalResult=mrOk;
	return;
 }else if(ARow==2){
	MageInfo &w=list->at(ACol-1);
	AOrders *orders=w.unit->orders;
	AOrder *ord=orders->GetMonthLong();
	if(ord){
	 EditOrder(orders,ord,false);
	}else{
	 int ret=Application->MessageBox("Add STUDY or TEACH order?","Adding monthorder",MB_YESNOCANCEL);
	 if(ret==IDYES){
		AOrderAbr *nord=new AOrderAbr;
		nord->type=O_STUDY;
		EditOrder(orders,nord,true);
	 }else if(ret==IDNO)
		EditOrder(orders,new AOrderTeach,true);
	}
 }else if(ARow==3){
	MageInfo &w=list->at(ACol-1);
	AOrders *orders=w.unit->orders;
	AOrder *ord=orders->Find(O_CAST,2);
	if(ord){
	 EditOrder(orders,ord,false);
	}else{
	 EditOrder(orders,new AOrderCast,true);
	}
 }else if(ARow==4){
		MageInfo &w=list->at(ACol-1);
		if(!w.teachernum) return;
		AUnit *un=w.unit->baseobj->basereg->turn->RegionList->GetUnit(w.teachernum);
		if(!un) return;
		AtlaForm->GotoUnit(un);
		ModalResult=mrOk;
 }else if(ARow>=SkillsRow){
	 AnsiString abr=TViewMagesForm::magic->Strings[ARow-SkillsRow];
   MageInfo &w=list->at(ACol-1);
   AOrders *orders=w.unit->orders;
   AOrder *ord=orders->GetMonthLong();
   if(!ord)
   {
     AOrderAbr *nord=new AOrderAbr;
     nord->type=O_STUDY;
     nord->abr=abr;
     orders->Add(nord);
   }
   else
   {
     if(ord->type!=O_STUDY)
       return;
     AOrderAbr *bord=(AOrderAbr*)ord;
     if(bord->abr==abr)
       orders->Delete(ord);
     else
       bord->abr=abr;
   }
   curTurn->Modified=true;
   w.unit->baseobj->basereg->runned=false;
   RunOrders();
	 for(MageInfo *mi=list->begin().operator ->(),*endmi=list->end().operator ->();mi<endmi;mi++){
     mi->Update();
   }
   int c=SortRow;
   FSortRow=-1;
   SortRow=c;
 }
}
//---------------------------------------------------------------------------
void __fastcall TViewMagesForm::miSetColWidthClick(TObject *Sender)
{
  AnsiString value=ColsWidth;
  if(!InputQuery("","Enter new columns width",value)) return;
  ColsWidth=atoi(value.c_str());
  MakeGrid();
}
//---------------------------------------------------------------------------
void __fastcall TViewMagesForm::miLocalOnlyClick(TObject *Sender)
{
  LocalOnly=!LocalOnly;
  miLocalOnly->Checked=LocalOnly;
  Init();
}
//---------------------------------------------------------------------------
void __fastcall TViewMagesForm::miCurRegOnlyClick(TObject *Sender)
{
  CurRegOnly=!CurRegOnly;
  miCurRegOnly->Checked=CurRegOnly;
	Init();
}
//---------------------------------------------------------------------------
void __fastcall TViewMagesForm::miPrevTurnClick(TObject *Sender)
{
  AtlaForm->bnPrevTurn->Click();
  Init();
}
//---------------------------------------------------------------------------
void __fastcall TViewMagesForm::miNextTurnClick(TObject *Sender)
{
  AtlaForm->bnNextTurn->Click();
  Init();
}
//---------------------------------------------------------------------------
void __fastcall TViewMagesForm::miEventsClick(TObject *Sender)
{
  miEvents->Clear();
  MageInfo &w=list->at(Grid->Col-1);
  AUnit *un=w.unit;
  AFaction *fac=un->faction;
  if(!fac->local)
    return;
  TStringList *slist=new TStringList;
  un->FillEventsList(slist);
  for(int i=0;i<slist->Count;i++){
    TMenuItem *mi=new TMenuItem(this);
    mi->Caption=slist->Strings[i];
    miEvents->Add(mi);
  }
  delete slist;
}
//---------------------------------------------------------------------------
void __fastcall TViewMagesForm::GridMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(Button!=mbRight) return;
  int c,r;
  Grid->MouseToCell(X,Y,c,r);
  if(c>=Grid->FixedCols)
  {
    if(r<Grid->FixedRows)
      r=Grid->FixedRows;
    Grid->Col=c;
    Grid->Row=r;
  }
}
//---------------------------------------------------------------------------
void __fastcall TViewMagesForm::miRemoveMonthOrderClick(TObject *Sender)
{
  MageInfo &w=list->at(Grid->Col-1);
  AUnit *un=w.unit;
  AOrder *ord=un->orders->GetMonthLong();
  if(!ord) return;
  un->orders->Delete(ord);
  curTurn->Modified=true;
  un->baseobj->basereg->runned=false;
  RunOrders();
  for(MageInfo *mi=list->begin().operator ->(),*endmi=list->end().operator ->();mi<endmi;mi++){
    mi->Update();
  }
  int c=SortRow;
  FSortRow=-1;
  SortRow=c;
}
//---------------------------------------------------------------------------

