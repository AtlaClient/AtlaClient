//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <algorithm>
#include "ViewUnitsFrm.h"
#include "region.h"
#include "unit.h"
#include "items.h"
#include "faction.h"
#include "MainFrm.h"
#include "ViewUnitsSetupFrm.h"
#include "skills.h"
#include "exprlang.h"
#include "RegionListFrm.h"
#include "ExpressionEditorFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TViewUnitsForm *ViewUnitsForm;
VUColumnHeaders TViewUnitsForm::Headers;
int TViewUnitsForm::FSortCol=-2,TViewUnitsForm::FSortDir=true;
TViewUnitsForm::TRegDiap TViewUnitsForm::regdiap=TViewUnitsForm::rdtCurrent;
int TViewUnitsForm::facdiap=-3;
int TViewUnitsForm::height=180;
int TViewUnitsForm::ColWidths[InfoCols]={100,45,100,50,134};
int TViewUnitsForm::InfoRows=2;
static AnsiString defHint;
//---------------------------------------------------------------------------
static AnsiString _ItemGroups[]={
 ":ADVANCED",
 ":ARMOR",
 ":BATTLE",
 ":FOOD",
 ":MAGIC",
 ":MAN",
 ":MONSTER",
 ":MOUNT",
 ":NORMAL",
 ":TRADE",
 ":WEAPON",
};
static int _ItemGroupsFlags[]={
  AItemType::IT_ADVANCED,
  AItemType::IT_ARMOR,
  AItemType::IT_BATTLE,
  AItemType::IT_FOOD,
  AItemType::IT_MAGIC,
  AItemType::IT_MAN,
  AItemType::IT_MONSTER,
  AItemType::IT_MOUNT,
  AItemType::IT_NORMAL,
  AItemType::IT_TRADE,
  AItemType::IT_WEAPON,
};
AnsiString *ItemGroups=_ItemGroups;
int ItemGroupCount=sizeof(_ItemGroups)/sizeof(_ItemGroups[0]);
#if sizeof(_ItemGroups)/sizeof(_ItemGroups[0])!=(sizeof(_ItemGroupsFlags)/sizeof(_ItemGroupsFlags[0]))
#error ItemGroupCount!=ItemGroupFlagsCount
#endif
//---------------------------------------------------------------------------
VUColumnHeader::VUColumnHeader()
  :abrs(new TStringList),width(100)
  ,skills(false)
{
}
VUColumnHeader::VUColumnHeader(const VUColumnHeader& b)
  :abrs(new TStringList)
{
  *this=b;
}
VUColumnHeader::~VUColumnHeader()
{
  delete abrs;
}
void VUColumnHeader::operator=(const VUColumnHeader& b)
{
  name=b.name;
  width=b.width;
  skills=b.skills;
  abrs->Assign(b.abrs);
}
int VUColumnHeader::GetFlags(AnsiString itemgroup)
{
  for(int i=0;i<ItemGroupCount;i++)
  {
    if(itemgroup==_ItemGroups[i])
      return _ItemGroupsFlags[i];
  }
  return 0;
}
void VUColumnHeader::Add(AnsiString abr)
{
  if(!abr.Length()) return;
  if(!skills)
  if(abr[1]==':')
  {
     int typeflags=GetFlags(abr);
     if(typeflags)
     {
       foreach(ItemTypes)
       {
         AItemType *itype=*(AItemType**)iter;
         if(!(itype->type&typeflags)) continue;
         Add(itype->abr);
       }
     }

     return;
  }
  int ind=abrs->IndexOf(abr);
  if(ind<0)
    abrs->Add(abr);
}
void VUColumnHeader::Delete(AnsiString abr)
{
  int ind=abrs->IndexOf(abr);
  if(ind>=0)
    abrs->Delete(ind);
}
void VUColumnHeader::Clear()
{
  abrs->Clear();
}
int VUColumnHeader::CalcCount(AUnit *un,AnsiString &s)
{
  int count=0;
  AnsiString str;
  if(skills)
  {
    for(int i=0,endi=abrs->Count;i<endi;i++)
    {
      AnsiString abr=abrs->Strings[i];
      ASkill *sk=un->skills->GetSkill(abr);
      if(!sk||!sk->GetLevel()) continue;
      count+=sk->days;
      if(str.Length())
        str+=",";
      str+=sk->type->abr;
      str+=" ";
      str+=sk->GetLevel();
    }
    s=str;
    return count;
  }
  for(int i=0,endi=abrs->Count;i<endi;i++)
  {
    AnsiString abr=abrs->Strings[i];
    AItem *it=un->items->GetItem(abr);
    if(!it||!it->count) continue;
    count+=it->count;
    if(abrs->Count>1)
    {
      if(str.Length())
        str+=",";
      str+=it->Print(3);
    }
  }
  s=count;
  if(str.Length())
   s+="="+str;
  return count;
}
void VUColumnHeader::operator=(AnsiString str)
{
  abrs->Clear();
  char *s=str.c_str();
  if(*s!='\"')
    throw 1;
  name=GetNextToken(s);
  if(*s==',')
  {
    s++;
    width=atoi(GetNextToken(s).c_str());
  }
  if(*s=='!')
  {
    skills=true;
    s++;
  }else
    skills=false;
  if(*s!='{')
    throw 1;
  s++;
  char *ss=s;
  while(*s&&*s!='}') s++;
  if(!*s)
    throw 1;
  AnsiString _str(ss,s-ss);
  abrs->CommaText=_str;
}
VUColumnHeader::operator AnsiString()
{
  AnsiString str;
  str="\""+name+"\",";
  str+=width;
  if(skills)
    str+="!";
  str+="{"+abrs->CommaText+"}";
  return str;
}
void VUColumnHeaders::operator=(AnsiString str)
{
  headers.clear();
  char *s=str.c_str();
  if(*s!='\"')
    return;
  name=GetNextToken(s);
  while(*s)
  {
    VUColumnHeader ch;
    if(*s!='\"')
      break;
    char *ss=s;
    while(*s&&*s!='}') s++;
    if(!*s) break;
    s++;
    try{
      AnsiString str(ss,s-ss);
      ch=str;
      headers.push_back(ch);
    }catch(...)
    {
      break;
    }
  }
}
VUColumnHeaders::operator AnsiString()
{
  AnsiString str="\""+name+"\"";
  for(VUColumnHeader *h=headers.begin().operator ->();h<headers.end().operator ->();h++)
  {
    str+=AnsiString(*h);
  }
  return str;
}
//---------------------------------------------------------------------------
class UnitInfo{
public:
 AUnit *unit;
 AnsiString name;
 AnsiString facname;
 AnsiString flags;
 AnsiString regname;
 vector<int> counts;
 vector<AnsiString> countstrs;

 void Update(vector<int>* summcounts);
 virtual void Draw(TDrawGrid *Grid,int ACol,TRect &Rect, TGridDrawState State);
 int CompareWith(const UnitInfo &b) const;

 friend int operator<(const UnitInfo&a,const UnitInfo&b){
  return a.CompareWith(b)<0;
 }
};
void UnitInfo::Update(vector<int>* summcounts){
  {
    AnsiString ld=unit->GetLocalDescr();
    if(ld.Length()&&optShowLDescFirst)
      name=ld;
    else if(unit->name.Length())
      name=unit->name;
    else if(ld.Length())
      name=ld;
    else
      name="Unit";
  }
  facname=unit->faction->FullName();
  {
    AnsiString str;
    int guard=unit->endguard;
    if(guard==GUARD_GUARD)
      str+="G";
    else if(guard==GUARD_AVOID)
      str+="A";
    if(unit->GetEndFlag(FLAG_BEHIND))
      str+="B";
    if(unit->GetEndFlag(FLAG_AUTOTAX))
      str+="T";
    if(unit->GetEndFlag(FLAG_HOLDING))
      str+="H";
    if(unit->GetEndFlag(FLAG_NOAID))
      str+="Na";
    int reveal=unit->endreveal;
    if(reveal==REVEAL_UNIT)
      str+="Ru";
    else if(reveal==REVEAL_FACTION)
      str+="Rf";
    flags=str;
  }
  {
    AnsiString str;
    AObject *baseobj=unit->baseobj;
    if(baseobj->num)
      str=baseobj->FullName();
    if(TViewUnitsForm::regdiap!=TViewUnitsForm::rdtCurrent)
    {
      if(baseobj->num)
        str+=" in ";
      str+=baseobj->basereg->FullName(false);
    }
    regname=str;
  }
  counts.resize(TViewUnitsForm::Headers.headers.size());
  countstrs.resize(TViewUnitsForm::Headers.headers.size());
  int i=0;
  for(VUColumnHeader *ch=TViewUnitsForm::Headers.headers.begin().operator ->(),
	*endch=TViewUnitsForm::Headers.headers.end().operator ->();ch<endch;ch++,i++)
  {
    counts[i]=ch->CalcCount(unit,countstrs[i]);
  }
  if(summcounts)
  {
	for(int *from=counts.begin().operator ->(),*to=summcounts->begin().operator ->();from<counts.end().operator ->();from++,to++)
      *to+=*from;
  }
}
void UnitInfo::Draw(TDrawGrid *Grid,int ACol,TRect &Rect, TGridDrawState State){
 AnsiString str;
 TCanvas *Canv=Grid->Canvas;
 if(ACol>=InfoCols){
   int ind=ACol-InfoCols;
   if(ind<(int)counts.size())
   {
     str=countstrs[ind];
   }
 }else{
  switch(ACol){
   case 0:str=name;break;
   case 1:str=unit->num;break;
   case 2:str=facname;break;
   case 3:str=flags;break;
   case 4:str=regname;break;
  }
 }
 Canv->TextRect(Rect, Rect.Left+2, Rect.Top+2, str);
}
int __fastcall RegionListCompare(void * Item1, void * Item2){
 ARegion *reg1=(ARegion*)Item1,*reg2=(ARegion*)Item2;
 return reg1->key-reg2->key;
}
int UnitInfo::CompareWith(const UnitInfo &b) const{
 int res=0;
 switch(TViewUnitsForm::FSortCol){
  case 0:
   res=name.AnsiCompare(b.name);;
  break;
  case 1:
   res=unit->num-b.unit->num;
  break;
  case 2:
  {
    AFaction *fac=unit->faction, *bfac=b.unit->faction;
    if(fac->local&&!bfac->local)
      res=-1;
    else if(!fac->local&&bfac->local)
      res=1;
    else
      res=fac->number-bfac->number;
  }
  break;
  case 3:
  {
    int val1=(unit->endguard==GUARD_GUARD)*4+(unit->endguard!=GUARD_AVOID)*2+(!unit->GetEndFlag(FLAG_BEHIND));
    int val2=(b.unit->endguard==GUARD_GUARD)*4+(b.unit->endguard!=GUARD_AVOID)*2+(!b.unit->GetEndFlag(FLAG_BEHIND));
    res=val1-val2;
  }
  break;
  case 4:
   res=RegionListCompare(unit->baseobj->basereg,b.unit->baseobj->basereg);
   if(!res)
     res=unit->baseobj->num-b.unit->baseobj->num;
  break;
  default:{
    int ind=TViewUnitsForm::FSortCol-InfoCols;
    if(unsigned(ind)<counts.size())
    {
      res=counts[ind]-b.counts[ind];
    }
  }
 }
 return TViewUnitsForm::FSortDir?res:-res;
}
//---------------------------------------------------------------------------
__fastcall TViewUnitsForm::TViewUnitsForm(TComponent* Owner)
  : TForm(Owner),headers(new TStringList),units(new UnitInfos)
{
  ViewUnitsForm=this;
  Height=height;
  Grid->RowHeights[0]=22;
  defHint=Grid->Hint;
  headers->Add("Name");
  headers->Add("ID");
  headers->Add("Faction");
  headers->Add("Flags");
  headers->Add("Object/Region");
}
//---------------------------------------------------------------------------
__fastcall TViewUnitsForm::~TViewUnitsForm()
{
  delete headers;
  delete units;
  ViewUnitsForm=0;
}
//---------------------------------------------------------------------------
void TViewUnitsForm::CollectUnitsInRegion(ARegion *reg, vector<int>* summcounts)
{
  AnsiString expr;
  if(cbExpression->Checked) expr=edExpr->Text;
  foreach(reg)
  {
    AObject *obj=*(AObject**)iter;
    foreach(obj->units)
    {
      AUnit *un=*(AUnit**)iter;
      if(!inFacDiap(facdiap,un->faction))
        continue;
      if(expr.Length())
      {
        ExprSetUnitMode(un);
        bool res=false;
        try{
          res=ExprProcessExpr(expr);
        }catch (Exception &exception){
          AnsiString str=un->FullName()+": ";
          exception.Message=str+exception.Message;
          Application->ShowException(&exception);
          expr.SetLength(0);
        }
        if(!res) continue;
      }
      UnitInfo ui;
      ui.unit=un;
      ui.Update(summcounts);
      units->push_back(ui);
    }
  }
}
//---------------------------------------------------------------------------
void TViewUnitsForm::CollectUnits()
{
  units->clear();
  counts.clear();
  counts.resize(Headers.headers.size(),0);
  vector<int> *summcounts=&counts;
  switch(regdiap)
  {
    case rdtCurrent:
      CollectUnitsInRegion(curRegion,summcounts);
    break;
    case rdtNearest:
      CollectUnitsInRegion(curRegion,summcounts);
      for(int i=0;i<NDIRS;i++)
      {
        ALocation loc=curRegion->neighbors[i];
        if(loc.xloc==-1) continue;
        ARegion *reg=curRegionList->Get(loc);
        if(!reg) continue;
        CollectUnitsInRegion(reg,summcounts);
      }
    break;
    case rdtAll:
      foreach(curRegionList)
      {
        ARegion *reg=*(ARegion**)iter;
        CollectUnitsInRegion(reg,summcounts);
      }
    break;
    case rdtRegionList:
      for(int i=0,endi=RegionListForm->RegionCount;i<endi;i++)
      {
        ARegion *reg=RegionListForm->Regions[i];
        CollectUnitsInRegion(reg,summcounts);
      }
    break;
  }
}
//---------------------------------------------------------------------------
void TViewUnitsForm::SortUnits()
{
  stable_sort(units->begin(),units->end());
}
//---------------------------------------------------------------------------
void TViewUnitsForm::MakeGrid()
{
  int count=units->size();
  Grid->ColWidths[0]=ColWidths[0];
  Grid->ColWidths[1]=ColWidths[1];
  Grid->ColWidths[2]=ColWidths[2];
  Grid->ColWidths[3]=ColWidths[3];
  if(cbHideObjects->Checked)
  {
    Grid->ColWidths[4]=0;
  }else
  {
    Grid->ColWidths[4]=ColWidths[4];
  }
  Grid->ColCount=InfoCols+Headers.headers.size();
  for(int i=InfoCols;i<Grid->ColCount;i++)
  {
    Grid->ColWidths[i]=Headers.headers[i-InfoCols].width;
  }
  Grid->FixedRows=InfoRows;
  if(!count)
  {
    Grid->RowCount=InfoRows+1;
    Grid->Invalidate();
    labUnits->Caption=" No units";
    return;
  }
  labUnits->Caption=AnsiString(" ")+count+" units";
  Grid->RowCount=count+InfoRows;
  Grid->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::FormShow(TObject *Sender)
{
  MakeFacList();
  MakeColumnSetList();
  cbRegDiap->ItemIndex=regdiap;
  if(!Headers.headers.size())
  {
    {
      VUColumnHeader ch;
      ch.name="Men";
      ch.Add(":MAN");
      Headers.headers.push_back(ch);
    }
    {
      VUColumnHeader ch;
      ch.name="Weapons";
      ch.Add(":WEAPON");
      Headers.headers.push_back(ch);
    }
    {
      VUColumnHeader ch;
      ch.name="Armors";
      ch.Add(":ARMOR");
      Headers.headers.push_back(ch);
    }
    {
      VUColumnHeader ch;
      ch.name="Horses";
      ch.Add(":MOUNT");
      Headers.headers.push_back(ch);
    }
  }

  CollectUnits();
  if(SortCol==-1) SortCol=0;
  else{
    SortUnits();
    MakeGrid();
  }
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::GridDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
  if(ARow>=InfoRows)
  {
    ARow-=InfoRows;
    if(ARow>=(int)units->size()) return;
    UnitInfo &ui=units->at(ARow);
    ui.Draw(Grid,ACol,Rect,State);
    return;
  }
  TCanvas *Canv=Grid->Canvas;
  AnsiString str;
  if(ARow==0)
  {
    if(ACol<InfoCols)
      str=headers->Strings[ACol];
    else
      str=Headers.headers[ACol-InfoCols].name;
    if(ACol==SortCol){
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
  }else if(ARow==1)
  {
      if(ACol>=InfoCols)
        if(Headers.headers[ACol-InfoCols].skills==false)
          str=counts[ACol-InfoCols];
    Rect.left+=2;
  }
  Canv->TextRect(Rect, Rect.Left, Rect.Top+2, str);
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::SetSortCol(int value)
{
 if(FSortCol == value) return;
 if(FSortCol!=-2)
   SaveColWidths();
 FSortCol = value;
 if(value==-1) return;
 SortUnits();
 MakeGrid();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::GridDblClick(TObject *Sender)
{
  TPoint p = Grid->ScreenToClient(Mouse->CursorPos);
  int ACol,ARow;
  Grid->MouseToCell(p.x,p.y,ACol,ARow);
  if(ACol<0) return;
  if(ARow<InfoRows){
    if(SortCol==ACol){
      FSortDir=!FSortDir;
      FSortCol=-1;
    }else{
      FSortDir=ACol<InfoCols;
    }
    SortCol=ACol;
  }else{
    ARow-=InfoRows;
    if(ARow>=(int)units->size()) return;
    UnitInfo &ui=units->at(ARow);
    AtlaForm->GotoUnit(ui.unit);
    //Grid->Col = ACol;
    //Grid->Row = ARow;
    return;
  }
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::cbRegDiapChange(TObject *Sender)
{
  if(regdiap==(TRegDiap)cbRegDiap->ItemIndex) return;
  regdiap=(TRegDiap)cbRegDiap->ItemIndex;
  ReMake();
}
//---------------------------------------------------------------------------
void TViewUnitsForm::MakeFacList()
{
  TStrings *list=cbFacDiap->Items;
  list->BeginUpdate();
  list->Clear();
  list->AddObject(" all",(TObject*)-2);
  list->AddObject(" local",(TObject*)-3);
  list->AddObject(" nonlocal",(TObject*)-4);
  list->AddObject(" friendly",(TObject*)-5);
  list->AddObject(" nonfriendly",(TObject*)-7);
  list->AddObject(" unfriendly",(TObject*)-6);
  foreach(curFactions)
  {
    AFaction *fac=*(AFaction**)iter;
    list->AddObject(fac->FullName(),(TObject*)fac->number);
  }
  list->EndUpdate();
  int ind=list->IndexOfObject((TObject*)facdiap);
  cbFacDiap->ItemIndex=ind;
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::cbFacDiapChange(TObject *Sender)
{
  int ind=cbFacDiap->ItemIndex;
  if(ind<0) return;
  ind=(int)cbFacDiap->Items->Objects[ind];
  if(facdiap==ind) return;
  facdiap=ind;
  ReMake();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::bnSetupClick(TObject *Sender)
{
  TViewUnitsSetupForm *frm=new TViewUnitsSetupForm(this);
  int res=frm->ShowModal();
  delete frm;
  if(res!=mrOk) return;
  ReMake(true);
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::cbHideObjectsClick(TObject *Sender)
{
  MakeGrid();
}
//---------------------------------------------------------------------------
void TViewUnitsForm::ReMake(bool newtable)
{
  if(!newtable)
    SaveColWidths();
  else
    MakeColumnSetList();
  CollectUnits();
  SortUnits();
  MakeGrid();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::FormResize(TObject *Sender)
{
  height=Height;
}
//---------------------------------------------------------------------------
AnsiString TViewUnitsForm::GetColWidthsStr()
{
  AnsiString str;
  for(int i=0;i<InfoCols;i++)
  {
    if(i)
      str+=",";
    str+=ColWidths[i];
  }
  return str;
}
//---------------------------------------------------------------------------
void TViewUnitsForm::SetColWidthsStr(AnsiString str)
{
  char *s=str.c_str();
  for(int i=0;i<InfoCols&&*s;i++)
  {
    if(i)
    {
      if(*s!=',')
        return;
      s++;
    }
    int w=atoi(GetNextToken(s).c_str());
    if(w<10) w=10;
    ColWidths[i]=w;
  }
}
//---------------------------------------------------------------------------
void TViewUnitsForm::SaveColWidths()
{
  for(int i=0;i<InfoCols;i++)
  {
    if(i==4&&cbHideObjects->Checked) continue;
    ColWidths[i]=Grid->ColWidths[i];
  }
  for(int i=InfoCols;i<Grid->ColCount;i++)
  {
    Headers.headers[i-InfoCols].width=Grid->ColWidths[i];
  }
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::FormHide(TObject *Sender)
{
  SaveColWidths();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::GridMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  int ARow,ACol;
  Grid->MouseToCell(X,Y,ACol,ARow);
  AnsiString str;
  if(ACol>=0&&ARow>=InfoRows)
  {
    ARow-=InfoRows;
    if(ARow>=(int)units->size()) return;
    UnitInfo &ui=units->at(ARow);
    if(ACol>=InfoCols)
      str=ui.countstrs[ACol-InfoCols];
    else
    {
      switch(ACol){
       case 0:str=ui.name;break;
       case 1:str=ui.unit->num;break;
       case 2:str=ui.facname;break;
       case 3:str=ui.flags;break;
       case 4:str=ui.regname;break;
      }
    }
  }else
    str=defHint;
  if(Grid->Hint==str)
    return;
  Application->CancelHint();
  Grid->Hint=str;
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::cbFacDiapEnter(TObject *Sender)
{
  facnum_search=0;
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::cbFacDiapKeyPress(TObject *Sender,
      char &Key)
{
  if(Key>='0'&&Key<='9')
  {
    facnum_search*=10;
    facnum_search+=Key-'0';
    Key=0;
    int ind=cbFacDiap->Items->IndexOfObject((TObject*)facnum_search);
    if(ind<0)
      return;
    cbFacDiap->ItemIndex=ind;
    cbFacDiapChange(0);
  }else
  {
    facnum_search=0;
  }
}
//---------------------------------------------------------------------------
void TViewUnitsForm::UpdateUnit(AUnit * un)
{
  if(!un) return;
  for(UnitInfo *ui=units->begin().operator ->(),*endui=units->end().operator ->();ui<endui;ui++)
  {
    if(ui->unit!=un) continue;
    ui->Update(0);
    if(SortCol==3)
    {
      SortUnits();
      MakeGrid();
    }else
      Grid->Invalidate();
    break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::cbColumnSetsClick(TObject *Sender)
{
  int ind=cbColumnSets->ItemIndex;
  if(ind<0) return;
  cbColumnSets->ItemIndex=-1;
  AnsiString str=ViewUnitsSets->Strings[ind];
  try{
    VUColumnHeaders h;
    h=str;
    Headers=h;
  }catch(...){
  }
  ReMake(true);
}
//---------------------------------------------------------------------------
void TViewUnitsForm::MakeColumnSetList()
{
  TStrings *list=cbColumnSets->Items;
  list->BeginUpdate();
  list->Clear();
  AnsiString str;
  for(int i=0;i<ViewUnitsSets->Count;i++)
  {
    str=ViewUnitsSets->Strings[i];
    char *s=str.c_str();
    list->Add(GetNextToken(s));
  }
  list->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::cbExpressionClick(TObject *Sender)
{
  ReMake();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::edExprExit(TObject *Sender)
{
  if(!edExpr->Modified) return;
  edExpr->Modified=false;
  if(!cbExpression->Checked) return;
  ReMake();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::edExprKeyPress(TObject *Sender, char &Key)
{
  if(Key!=13) return;
  edExpr->Modified=true;
  edExprExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsForm::bnEditExprClick(TObject *Sender)
{
  AUnit *un = 0;
  int ARow = Grid->Row;
  if(ARow < InfoRows) return;
  ARow-=InfoRows;
  if(ARow<(int)units->size())
  {
    UnitInfo &ui=units->at(ARow);
    un=ui.unit;
  }
  if(!un&&!curUnit)
  {
    ShowMessage("No unit selected");
    return;
  }
  TExpressionEditorForm *frm=new TExpressionEditorForm(this);
  frm->mode=emUnit;
  frm->expr=edExpr->Text;
  frm->unit=un;
  if(frm->ShowModal()==mrOk)
  {
    edExpr->Text=frm->expr;
    edExpr->Modified=true;
    edExprExit(0);
  }
  delete frm;
}
//---------------------------------------------------------------------------

void __fastcall TViewUnitsForm::GridMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(Button != mbRight) return;
  GridDblClick(0);
  if(curUnit && TreeViewPopupMenu)
  {
    TPoint mp = Grid->ScreenToClient(Mouse->CursorPos);
    int ACol,ARow;
    Grid->MouseToCell(mp.x, mp.y, ACol, ARow);
    if(ARow < 2) return;
    Grid->Col = ACol;
    Grid->Row = ARow;
    mp.y += this->Top*2;
    TreeViewPopupMenu->PopupComponent = AtlaForm->TreeView;
    TreeViewPopupMenu->Popup(mp.x, mp.y);
  }
}
//---------------------------------------------------------------------------

