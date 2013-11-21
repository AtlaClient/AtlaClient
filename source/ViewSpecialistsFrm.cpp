//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "ViewSpecialistsFrm.h"
#include "MainFrm.h"
#include "region.h"
#include "skills.h"
#include "items.h"
#include "orders.h"
#include "turn.h"
#include "faction.h"
#include "gameparams.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TViewSpecialistsForm *ViewSpecialistsForm;
bool TViewSpecialistsForm::NoMagic=true,TViewSpecialistsForm::AllReg=false;
bool TViewSpecialistsForm::ShortMode=true;
bool TViewSpecialistsForm::SortMode=true;
//---------------------------------------------------------------------------
TSkLev::TSkLev():units(new AUnits(0)){
  level=0;
  units->autodelete=false;
}
//---------------------------------------------------------------------------
TSkLev::TSkLev(const TSkLev&b):units(new AUnits(0))
{
  stype=b.stype;
  level=b.level;
  units->autodelete=false;
  AssignUnits(b.units);
}
//---------------------------------------------------------------------------
void TSkLev::operator =(const TSkLev&b)
{
  stype=b.stype;
  level=b.level;
  units->autodelete=false;
  AssignUnits(b.units);
}
//---------------------------------------------------------------------------
TSkLev::~TSkLev(){
  delete units;
}
//---------------------------------------------------------------------------
void TSkLev::AssignUnits(AUnits *uns)
{
  units->Clear();
  foreach(uns){
    AUnit *un=*(AUnit**)iter;
    units->Add(un);
  }
}
//---------------------------------------------------------------------------
__fastcall TViewSpecialistsForm::TViewSpecialistsForm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
TSkLev* TViewSpecialistsForm::GetSkLev(ASkillType* st, int lev)
{
  int addind=sklevs.size();
  int stind=st->sortindex;
  for(TSkLev* skl=sklevs.begin().operator ->(),*endskl=sklevs.end().operator ->();skl<endskl;skl++)
  {
    if(skl->stype->sortindex<stind) continue;
    if(skl->stype->sortindex==stind)
    {
      if(skl->level<lev) continue;
      if(skl->level==lev)
        return skl;
    }
	addind=skl-sklevs.begin().operator ->();
    break;
  }
  TSkLev skl;
  skl.stype=st;
  skl.level=lev;
  sklevs.insert(sklevs.begin()+addind,skl);
  return sklevs.begin().operator ->()+addind;
}
//---------------------------------------------------------------------------
void __fastcall TViewSpecialistsForm::FormShow(TObject *Sender)
{
  bnAllRegion->Down = AllReg;
  bnNoMagic->Down = NoMagic;
  bnShortMode->Down = ShortMode;
  bnSortMode->Down = SortMode;
  sklevs.clear();
  if(!AllReg)
  {
	ProcessRegion(curRegion);
  }
   else
  {
	if(!curTurn->AllOrdersRunned)
	{
	  DisableEvents();
	  curTurn->RunOrders();
	  EnableEvents();
	}
	foreach(curRegionList)
	{
	  ARegion *reg=*(ARegion**)iter;
	  if(!reg->hasInfo) continue;
	  ProcessRegion(reg);
	}
  }
  MakeTree();
}
//---------------------------------------------------------------------------
void TViewSpecialistsForm::ProcessRegion(ARegion * reg)
{
  foreach(reg)
  {
	AObject *o=*(AObject **) iter;
	foreach(o->units){
	  AUnit *u=*(AUnit**)iter;
	  if(!u->IsLocal()) continue;
      foreach(u->skills)
      {
        ASkill *sk=*(ASkill**)iter;
		if(NoMagic&&(sk->type->flags&ASkillType::MAGIC))
          continue;
        TSkLev* skl=GetSkLev(sk->type,sk->GetLevel());
        skl->units->Add(u);
      }
    }
  }
}
//---------------------------------------------------------------------------
void TViewSpecialistsForm::MakeTree()
{
  List->Items->Clear();
  TTreeNodes *items = Tree->Items;
  items->BeginUpdate();
  items->Clear();
  TTreeNode *lasttop=0;
  ASkillType *laststype=0;
  int l=0,m=0;
  for(TSkLev* skl=sklevs.begin().operator ->(),*endskl=sklevs.end().operator ->();skl<endskl;skl++)
  {
	skl->UpdateMensCount();
	if(skl->stype!=laststype)
	{
      if(lasttop)
      {
        lasttop->Text=lasttop->Text+":"+TSkLev::MakeMensStr(l,m);
        lasttop->Expand(true);

      }
      laststype=skl->stype;
      lasttop=items->Add(0,laststype->name);
      l=0;m=0;
    }
    AnsiString str=skl->level;
    str+=":";
    str+=skl->MakeMensStr();
    l+=skl->lead;
    m+=skl->man;
	TTreeNode *node=items->AddChild(lasttop,str);
	node->Data=skl;
  }
  if(lasttop)
  {
	lasttop->Text=lasttop->Text+":"+TSkLev::MakeMensStr(l,m);
	lasttop->Expand(true);
  }
  items->AlphaSort(false);
  items->EndUpdate();
}
//---------------------------------------------------------------------------
void TSkLev::UpdateMensCount()
{
  int l=0,m=0;
  foreach(units)
  {
    AUnit *u=*(AUnit**)iter;
    foreach(u->items){
      AItem *it=*(AItem**)iter;
      AItemType *itype=it->type;
      if(!(itype->type&AItemType::IT_MAN))
        continue;
      if(itype->type&AItemType::IT_LEADER)
        l+=it->count;
      else
        m+=it->count;
    }
  }
  lead=l;
  man=m;
}
//---------------------------------------------------------------------------
AnsiString TSkLev::MakeMensStr(int lead, int man)
{
  AnsiString str;
  if(lead)
  {
    str+=lead;
    if(lead==1)
      str+=" leader";
    else
      str+=" leaders";
  }
  if(man)
  {
    if(lead)
      str+=", ";
    str+=man;
    if(man==1)
      str+=" man";
    else
      str+=" men";
  }
  return str;
}
//---------------------------------------------------------------------------
static int __fastcall UnitSortMen(void *Item1, void *Item2){
 AUnit *un1=(AUnit*)Item1;
 AUnit *un2=(AUnit*)Item2;
 return un1->GetBegMen()-un2->GetBegMen();
}
//---------------------------------------------------------------------------
void __fastcall TViewSpecialistsForm::TreeChange(TObject *Sender,
      TTreeNode *Node)
{
  TSkLev *skl=0;
  if(Node&&Node->Data)
    skl=(TSkLev*)Node->Data;
  if(!skl) return;
  if(SortMode)
	skl->units->Sort(UnitSortMen);
  TStrings *list=List->Items;
  list->BeginUpdate();
  list->Clear();
  //int i=0;
  int maxsize=0;
  foreach(skl->units)
  {
    AUnit *u=*(AUnit**)iter;
    AnsiString str;
    {
      AOrderTeach *ord=dynamic_cast<AOrderTeach*>(u->orders->Find(O_TEACH,2));
      if(ord)
      {
        str="T(";
        int students=0;
        for(int i=0;i<ord->targets->Count;i++){
          AUnitPtr *ptr=ord->GetTarget(i);
          AUnit *un=ptr->ptr;
          if(!un) continue;
          students+=un->GetMen();
        }
        str+=students;
        str+="/";
        str+=u->GetMen()*TeachMaxStudents;
        str+=")";
      }
    }
    {
      AOrderAbr *ord=dynamic_cast<AOrderAbr*>(u->orders->Find(O_STUDY,2));
      if(ord)
      {
        str="S(";
        str+=SkillTypes->AbrToName(ord->abr);
        foreach(u->orders)
		{
          AOrder *_ord=*(AOrder**)iter;
          if(_ord->type!=O_MESSAGE)
            continue;
          AOrderMessage *ord=(AOrderMessage*)_ord;
          if(!ord->base) continue;
          if(ord->base->type!=O_TEACH) continue;
          str+="+";
        }
        str+=")";
      }
    }
    if(ShortMode)
    {
      if(str.Length())
        str+=" ";
      str+=u->FullName();
      if(u->faction->number){
        str+=", ";
        str+=u->faction->FullName();
      }
      foreach(u->items){
        AItem *it=*(AItem**)iter;
        if(!(it->type->type&AItemType::IT_MAN)) continue;
        str+=", "+it->Print();
      }
    }else
    {
      str+=u->Print();
      str.SetLength(str.Length()-1);
    }
    str+=KeyPhrases->Get(keyUnitSkills);
    str+=u->skills->Print();
    list->AddObject(str,(TObject*)u);
    TSize siz=List->Canvas->TextExtent(str);
    if(siz.cx>maxsize)maxsize=siz.cx;
	//i++;
  }
  list->EndUpdate();
  List->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
}
//---------------------------------------------------------------------------
void __fastcall TViewSpecialistsForm::ListDblClick(TObject *Sender)
{
  int ind=List->ItemIndex;
  if(ind<0) return;
  AUnit *un=(AUnit*)List->Items->Objects[ind];
  if(!un) return;
  AtlaForm->GotoUnit(un);
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TViewSpecialistsForm::bnAllRegionClick(TObject *Sender)
{
  AllReg=bnAllRegion->Down;
  FormShow(0);
}
//---------------------------------------------------------------------------
void __fastcall TViewSpecialistsForm::bnNoMagicClick(TObject *Sender)
{
  NoMagic=bnNoMagic->Down;
  FormShow(0);
}
//---------------------------------------------------------------------------
void __fastcall TViewSpecialistsForm::bnShortModeClick(TObject *Sender)
{
  ShortMode=bnShortMode->Down;
  TreeChange(0,Tree->Selected);
}
//---------------------------------------------------------------------------
void __fastcall TViewSpecialistsForm::bnSortModeClick(TObject *Sender)
{
  SortMode=bnSortMode->Down;
  TreeChange(0,Tree->Selected);
}
//---------------------------------------------------------------------------

