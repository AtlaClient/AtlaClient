//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "BattleSimFrm.h"
#include "unit.h"
#include "items.h"
#include "region.h"
#include "army.h"
#include "battle.h"
#include "skills.h"
#include "battlerep.h"
#include "exprlang.h"
#include "reports.h"
#include "gameparams.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBattleSimForm *BattleSimForm;
//---------------------------------------------------------------------------
__fastcall TBattleSimForm::TBattleSimForm(TComponent* Owner)
		: TForm(Owner)
{
 rep=new ABattleReport;
 lastunnum=0;
 Modified=false;
 Reports=new TEAPCustomMemo(this);
 Reports->ReadOnly=true;
 TRect r(0,281,792,281+252);
 Reports->BoundsRect=r;
 Reports->Align=alClient;
 Reports->Parent=this;
 Reports->OnMemoEvents=ReportMemoEvents;
 Reports->TabOrder=0;
}
__fastcall TBattleSimForm::~TBattleSimForm()
{
}
void __fastcall TBattleSimForm::FormShow(TObject *Sender)
{
static void *oldturns=0;
// static bool first=true;
 if(oldturns!=curTurns){
   oldturns=curTurns;
//  first=false;
  rep->Clear();
  rep->SetSides(0,0);
  Reports->Clear();
  foreach(TerrainTypes){
   ATerrainType*ttype=*(ATerrainType**)iter;
   if(!(ttype->mountstypes&ATerrainType::mtRiding)) continue;
   rep->reg->type=ttype->name;
   break;
  }
  if(!rep->reg->type.Length()){
   foreach(TerrainTypes){
    ATerrainType*ttype=*(ATerrainType**)iter;
    if(!(ttype->mountstypes&ATerrainType::mtFlying)) continue;
    rep->reg->type=ttype->name;
    break;
   }
  }
  if(!rep->reg->type.Length())
   rep->reg->type=TerrainTypes->Get(0)->name;
  UpdateComboBoxs();
 }

 if(rep->count)
  bnViewClick(0);
 else
  Draw();
}
//---------------------------------------------------------------------------
void TBattleSimForm::Draw()
{
 AnsiString str;//=reg->type;
 ATerrainType *ttype=TerrainTypes->Get(rep->reg->type);
 if(ttype->mountstypes&ATerrainType::mtRiding)
  str+="riding && flying";
 else if(ttype->mountstypes&ATerrainType::mtFlying)
  str+="flying only";
 else
  str+="no mounts";
 labMountType->Caption=str;
 cbRegType->ItemIndex=cbRegType->Items->IndexOf(rep->reg->type);

 UpdateList(lbAtts,rep->atts);
 UpdateList(lbDefs,rep->defs);

 str=rep->attcount;
 str+=":";
 str+=rep->defcount;
 if(rep->count){
  str+="\n";
  if(rep->wintype==wtWinAtt){
   str+=rep->loses2;
   str+=":";
   str+=rep->loses1;
  }else{
   str+=rep->loses1;
   str+=":";
   str+=rep->loses2;
  }
 }
 labAnalRes->Caption=str;

 DrawCurUnit2();
}
//---------------------------------------------------------------------------
void TBattleSimForm::DrawCurUnit2(){
 if(lastunnum){
  AUnit *un;
  int unind;
  un=rep->atts->GetUnit(lastunnum);
  if(!un){
   un=rep->defs->GetUnit(lastunnum);
   lbDefs->ItemIndex=rep->defs->IndexOf(un);
  }else{
   lbAtts->ItemIndex=rep->atts->IndexOf(un);
  }
  if(un){
   curUnit=un;
   DrawCurUnit();
  }else{
   lastunnum=0;
   curUnit=0;
  }
 }
 if(!lastunnum){
  labCurUnit->Caption="No unit selected";
  lbItems->Items->Clear();
  lbSkills->Items->Clear();
  DrawCurItem();
  DrawCurSkill();
 }
 bnSetItem->Enabled=lastunnum;
 bnSetSkill->Enabled=lastunnum;
}
//---------------------------------------------------------------------------
void TBattleSimForm::DrawCurUnit()
{
 labCurUnit->Caption=curUnit->BattleReport();
 TStrings *list=lbItems->Items;
 list->BeginUpdate();
 list->Clear();
 int maxsize=0;
 foreach(curUnit->items){
  AItem *it=*(AItem**) iter;
  AnsiString str=it->WriteReport(true);
  list->Add(str);
  TSize siz=lbItems->Canvas->TextExtent(str);
  if(siz.cx>maxsize)maxsize=siz.cx;
 }
 list->EndUpdate();
 lbItems->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
 lbItems->ItemIndex=0;
 DrawCurItem();

 list=lbSkills->Items;
 list->BeginUpdate();
 list->Clear();
 maxsize=0;
 foreach(curUnit->skills){
  ASkill *sk=*(ASkill**) iter;
  AnsiString str=sk->Print(5);
  list->Add(str);
  TSize siz=lbSkills->Canvas->TextExtent(str);
  if(siz.cx>maxsize)maxsize=siz.cx;
 }
 list->EndUpdate();
 lbSkills->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
 lbSkills->ItemIndex=0;
 DrawCurSkill();
 DrawCurCombat();
 cbBehind->Checked=curUnit->GetFlag(FLAG_BEHIND);  
}
//---------------------------------------------------------------------------
void TBattleSimForm::DrawCurItem()
{
 lbItemsClick(0);
}
//---------------------------------------------------------------------------
void TBattleSimForm::DrawCurSkill()
{
 lbSkillsClick(0);
}
//---------------------------------------------------------------------------
void TBattleSimForm::DrawCurCombat(){
 int ind=0;
 TStrings *list=cbCombatSpell->Items;
 list->BeginUpdate();
 list->Clear();
 list->Add("None");
 if(curUnit){
  int i=1;
  foreach(curUnit->skills){
   ASkill *sk=*(ASkill**) iter;
   if(!(sk->type->flags&ASkillType::COMBAT))
    continue;
   AnsiString str=sk->Print(1);
   list->Add(str);
   if(sk->type->abr==curUnit->combatspell)
    ind=i;
   i++;
  }
 }
 list->EndUpdate();
 cbCombatSpell->ItemIndex=ind;
}
//---------------------------------------------------------------------------
void TBattleSimForm::UpdateComboBoxs()
{
 TStrings *list=cbRegType->Items;
 list->BeginUpdate();
 list->Clear();
 foreach(TerrainTypes){
  ATerrainType *ttype=*(ATerrainType**)iter;
  AnsiString str=ttype->name;
/*  if(ttype->mountstypes){
   str+=" (";
   if(ttype->mountstypes&ATerrainType::mtRiding)
    str+="riding && flying";
   else if(ttype->mountstypes&ATerrainType::mtFlying)
    str+="flying only";
   str+=")";
  }*/
  list->Add(str);
 }
 list->EndUpdate();

 list=cbItems->Items;
 list->BeginUpdate();
 list->Clear();
 foreach(ItemTypes){
  AItemType *itype=*(AItemType**)iter;
  if(!itype->combat) continue;
  if(ItemTypes->IsIllusionAbr(itype->abr)) continue;
  AnsiString str=itype->names;
  list->Add(str);
 }
 list->EndUpdate();

 list=cbSkills->Items;
 list->BeginUpdate();
 list->Clear();
 foreach(SkillTypes){
  ASkillType *stype=*(ASkillType**)iter;
  if(stype->flags&ASkillType::MAGIC){
   if(!(stype->flags&ASkillType::COMBAT))
    continue;
  }else{
   if(stype->abr!=TactSkill
    &&stype->abr!=CombSkill
    &&stype->abr!=LbowSkill
    &&stype->abr!=XbowSkill
    &&stype->abr!=RidiSkill) continue;
  }
  AnsiString str=stype->name;
  list->Add(str);
 }
 list->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::cbRegTypeChange(TObject *Sender)
{
 rep->reg->type=cbRegType->Text;
 Draw();
 Modified=true;
}
//---------------------------------------------------------------------------
void TBattleSimForm::AddAttacker(int index)
{
 AUnit *un=curRegion->GetUnit(index);
 if(!un) return;
 AddAttacker(un);
}
//---------------------------------------------------------------------------
void TBattleSimForm::AddAttacker(AUnit *un)
{
 int num=ModifyNewUnitNum(un->num);
 if(rep->atts->GetUnit(num)) return;
 AUnit *un2=un->CreateBattleUnit();
 un2->baseobj=rep->reg->GetDummy();
 un2->num=num;
/* if(un2->num) //todo:verif
  un2->num=GetNewUnitNum();*/
 rep->atts->Add(un2);
}
//---------------------------------------------------------------------------
void TBattleSimForm::AddDefender(int index)
{
 AUnit *un=curRegion->GetUnit(index);
 if(!un) return;
 AddDefender(un);
}
//---------------------------------------------------------------------------
void TBattleSimForm::AddDefender(AUnit *un)
{
 int num=ModifyNewUnitNum(un->num);
 if(rep->defs->GetUnit(num)) return;
 AUnit *un2=un->CreateBattleUnit();
 un2->baseobj=rep->reg->GetDummy();
 un2->num=num;
 rep->defs->Add(un2);
}
//---------------------------------------------------------------------------
void TBattleSimForm::UpdateList(TListBox *lb, AUnits * uns)
{
 TStrings *list=lb->Items;
 int maxsize=0;
 list->BeginUpdate();
 list->Clear();
 foreach(uns){
  AUnit *un=*(AUnit**)iter;
  AnsiString str=un->BattleReport();
  list->AddObject(str,(TObject*)un->num);
  TSize siz=lb->Canvas->TextExtent(str);
  if(siz.cx>maxsize)maxsize=siz.cx;
 }
 lb->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
 list->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::bnRunClick(TObject *Sender)
{
 rep->CompactUnits();
 if(!rep->atts->count||!rep->defs->count){
  Draw();
  return;
 }
 Modified=false;
 AnsiString str=edMaxRounds->Text;
 if(str.Length())
   rep->roundlimit=atoi(str.c_str());
 else
   rep->roundlimit=-1;
 Screen->Cursor=crHourGlass;
 rep->Run();
 Screen->Cursor=crDefault;

 bnViewClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::lbAttsDefsClick(TObject *Sender)
{
 TListBox *lb=dynamic_cast<TListBox*>(Sender);
 int ind=lb->ItemIndex;
 if(ind<0){
  lastunnum=0;
 }else{
  lastunnum=int(lb->Items->Objects[ind]);
 }
 DrawCurUnit2();
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::bnSetItemClick(TObject *Sender)
{
 int num=atoi(edItem->Text.c_str());
 int ind=cbItems->ItemIndex;
 if(ind<0) return;
 AnsiString item=cbItems->Items->Strings[ind];
 AItemType *itype=ItemTypes->Get(item,15);
 curUnit->items->SetNum(itype->abr,num);
 if(!num)
  curUnit->items->DeleteEmpty();
 Draw();
 Modified=true;
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::bnSetSkillClick(TObject *Sender)
{
 int num=atoi(edSkill->Text.c_str());
 int ind=cbSkills->ItemIndex;
 if(ind<0) return;
 AnsiString skill=cbSkills->Items->Strings[ind];
 ASkillType *stype=SkillTypes->Get(skill);
 ASkill *sk=curUnit->skills->GetSkill(stype->abr);
 if(!sk){
  if(num){
   sk=new ASkill;
   sk->type=stype;
   sk->SetLevel(num);
   curUnit->skills->Add(sk);
  }
 }else{
  sk->SetLevel(num);
  if(!num)
   curUnit->skills->DeleteEmpty();  
 }
 Draw();
 Modified=true;
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::lbItemsClick(TObject *Sender)
{
 int ind=lbItems->ItemIndex;
 if(ind<0) return;
 AItem *it=curUnit->items->Get(ind);
// AItemType *itype=it->GetType();
 cbItems->ItemIndex=cbItems->Items->IndexOf(it->type->names);
 edItem->Text=it->count;
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::lbSkillsClick(TObject *Sender)
{
 int ind=lbSkills->ItemIndex;
 if(ind<0) return;
 ASkill *sk=curUnit->skills->Get(ind);
 cbSkills->ItemIndex=cbSkills->Items->IndexOf(sk->type->name);
 edSkill->Text=sk->GetLevel(); 
}
//---------------------------------------------------------------------------
int TBattleSimForm::PresentUnit(int num)
{
 return rep->PresentUnit(num);
}
//---------------------------------------------------------------------------
int TBattleSimForm::GetNewUnitNum()
{
 int num=100000;
 while(PresentUnit(num)) num++;
 return num;
}
//---------------------------------------------------------------------------
int TBattleSimForm::ModifyNewUnitNum(int num){
 if(num>=0) return num;
 return 100000+num;
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::bnAddAttClick(TObject *Sender)
{
 AUnit *un=new AUnit(0);
 un->num=GetNewUnitNum();
 rep->atts->Add(un);
 Draw();
 Modified=true;
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::bnAddDefClick(TObject *Sender)
{
 AUnit *un=new AUnit(0);
 un->num=GetNewUnitNum();
 rep->defs->Add(un);
 Draw();
 Modified=true;
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::bnDeleteClick(TObject *Sender)
{
 rep->atts->Delete(lastunnum);
 rep->defs->Delete(lastunnum);
 Draw();
 Modified=true;
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::bnSwapClick(TObject *Sender)
{
 int where=PresentUnit(lastunnum);
 if(!where) return;
 AUnit *un=curUnit->CreateBattleUnit();
 if(where==1){
  rep->defs->Add(un);
  rep->atts->Delete(lastunnum);
 }else{
  rep->atts->Add(un);
  rep->defs->Delete(lastunnum);
 }
 Draw();
 Modified=true;
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::bnSwapAllClick(TObject *Sender)
{
 AUnits *t=rep->atts;
 rep->atts=rep->defs;
 rep->defs=t;
 Draw();
 Modified=true;
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::cbBehindClick(TObject *Sender)
{
 curUnit->SetFlag(FLAG_BEHIND,cbBehind->Checked);
 Draw(); 
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::bnSaveClick(TObject *Sender)
{
 SaveDialog->Filter="Battle files (*.btl)|*.btl|All files|*.*";
 SaveDialog->DefaultExt="btl";
 if(!LoadBattlesDir.Length()){
  LoadBattlesDir=GetGameFileName("");
 }
 if(LoadBattlesDir.Length()){
  SaveDialog->InitialDir=LoadBattlesDir;
  if(ExtractFilePath(SaveDialog->FileName)!=LoadBattlesDir)
   SaveDialog->FileName="";
 }
 if(!SaveDialog->Execute()) return;
 LoadBattlesDir=ExtractFilePath(SaveDialog->FileName);
 AnsiString fname=SaveDialog->FileName;

 if(Modified)
  rep->Reset();
 TFileStream *strm;
 try{
  strm=new TFileStream(fname,fmCreate);
  TStringList *list=new TStringList;
  rep->Print(list,false);

  SaveRepStringList(strm,list);
// list->SaveToFile(fname);
  delete list;
 }__finally{
  delete strm;
 }
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::bnViewClick(TObject *Sender)
{
 if(Sender){
  if(Modified)
   rep->Reset();
  rep->CompactUnits();
 }
 rep->Check();
 TStringList *list=new TStringList;
 rep->Print(list,cbShortMode->Checked);
 Reports->Clear();
 Reports->Lines->Assign(list);
 Reports->ScrollTo(0,0);
 Reports->Invalidate();
 delete list;
 Draw();
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::bnAnalyzeClick(TObject *Sender)
{
 InAnalyze=!InAnalyze;
}
//---------------------------------------------------------------------------
void sortinsert(vector<int> &vect,int val){
 int *i,*endi;
 for(i=vect.begin().operator ->(),endi=vect.end().operator ->();i<endi;i++){
  if(*i>=val){
   break;
  }
 }
 vect.insert(i,val);
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::SetInAnalyze(bool value)
{
 if(FInAnalyze == value) return;
 rep->CompactUnits();
 if(!rep->atts->count||!rep->defs->count){
  value=false;
 }
 FInAnalyze = value;
 bnAnalyze->Caption=value?"Stop":"Analyze";
 value=!value;
 for(int i=Panel1->ControlCount-1;i>=0;i--){
  if(Panel1->Controls[i]->Name != "Panel4") Panel1->Controls[i]->Enabled=value;
 }
 bnAnalyze->Enabled=true;
 labAnalRes->Enabled=true;
 if(value){ //not in analyze
  DrawCurUnit2();
  return;
 }
 AnsiString str=edMaxRounds->Text;
 if(str.Length())
   rep->roundlimit=atoi(str.c_str());
 else
   rep->roundlimit=-1;
 int winatt=0,windef=0,tie=0;
/* int attminlos=65535,attmaxlos=0;
 int defminlos=65535,defmaxlos=0;
 float attsrlos,defsrlos;*/
 vector<int> attloses,defloses;

 for(int round=0;round<10000;round++){
  Application->ProcessMessages();
  if(!InAnalyze) break;
  Screen->Cursor=crHourGlass;
  rep->Run();
  Screen->Cursor=crDefault;
  switch(rep->wintype){
   case wtTie:tie++;break;
   case wtWinAtt:winatt++;break;
   case wtWinDef:windef++;break;
  }
  sortinsert(attloses,rep->AttLoses);
  sortinsert(defloses,rep->DefLoses);
/*  int los=rep->AttLoses;
  if(los<attminlos) attminlos=los;
  if(los>attmaxlos) attmaxlos=los;
  if(round){
   attsrlos=(attsrlos*round+los)/(round+1);
  }else{
   attsrlos=los;
  }
  int los=rep->DefLoses;
  if(los<defminlos) defminlos=los;
  if(los>defmaxlos) defmaxlos=los;
  if(round){
   defsrlos=(defsrlos*round+los)/(round+1);
  }else{
   defsrlos=los;
  }*/
  AnsiString str="Battles:";
  str+=round+1;
  str+=" Att:";
  str+=winatt;
  str+=" Tie:";
  str+=tie;
  str+=" Def:";
  str+=windef;
  str+=" AttLos:";
  str+=attloses[0];
  str+="/";
  str+=attloses[round/2];
  str+="/";
  str+=attloses[round];
  str+=" DefLos:";
  str+=defloses[0];
  str+="/";
  str+=defloses[round/2];
  str+="/";
  str+=defloses[round];
  labAnalRes->Caption=str;
 }
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::bnLoadClick(TObject *Sender)
{

 OpenDialog->Filter="Battle files (*.btl)|*.btl|All files|*.*";
 OpenDialog->DefaultExt="btl";
 if(!LoadBattlesDir.Length()){
  LoadBattlesDir=GetGameFileName("");
 }
 if(LoadBattlesDir.Length()){
  OpenDialog->InitialDir=LoadBattlesDir;
  if(ExtractFilePath(OpenDialog->FileName)!=LoadBattlesDir)
   OpenDialog->FileName="";
 }
 if(!OpenDialog->Execute()) return;
 LoadBattlesDir=ExtractFilePath(OpenDialog->FileName);
 AnsiString fname=OpenDialog->FileName;

 TMemoryStream*mem=LoadFile(fname);
 char*s=(char*)mem->Memory;
 bool ret=ProcessBattleReport(s,rep);
 delete mem;
 Draw();
 if(!ret)
  ShowMessage("Error loading battle");
 Modified=false;
 bnViewClick(0);
}
//---------------------------------------------------------------------------
bool TBattleSimForm::LoadFromList(TStringList * list, int ind)
{
 TMemoryStream *strm=new TMemoryStream;
 for(int i=ind,endi=list->Count;i<endi;i++){
  AnsiString s=list->Strings[i]+"\n";
  strm->Write(s.c_str(),s.Length());
 }
 int siz=strm->Size;
 strm->SetSize(siz+1);
 char *s=(char*)strm->Memory;
 s[siz]=0;
 bool ret=ProcessBattleReport(s,rep);
 delete strm;
 if(!ret)
  ShowMessage("Error loading battle");
 Modified=false;
 return ret;
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::cbCombatSpellClick(TObject *Sender)
{
 if(!curUnit) return;
 int ind=cbCombatSpell->ItemIndex;
 AnsiString spell;
 if(ind>0){ //==0 spell=0;
  spell=SkillTypes->NameToAbr(cbCombatSpell->Items->Strings[ind]);
 }
 if(curUnit->combatspell!=spell)
  Modified=true;
 curUnit->combatspell=spell;
}
//---------------------------------------------------------------------------
void TBattleSimForm::AddArmy(AUnit *lun,bool attacks){
 AFaction *fac=lun->faction;
 ARegion *breg=lun->baseobj->basereg;
 rep->SetRegion(breg);
 for(int i=-1;i<6;i++){
  ARegion *reg;
  if(i==-1){
   reg=breg;
  }else{
   reg=curRegionList->Get(breg->neighbors[i]);
   if(!reg) continue;
   reg->PrepareOrders();
   reg->RunFlagOrders();
   reg->PrepareConsume();
  }
  foreach(reg){
   AObject *obj=*(AObject**) iter;
   foreach(obj->units){
    AUnit *un=*(AUnit**)iter;
//
    if(un->faction!=fac) continue;
    if(!un->GetSoldiers()) continue;
    if(un!=lun)
     if((attacks||i!=-1)&&un->endguard==GUARD_AVOID) continue;
    if(i!=-1){
     if(un->GetEndFlag(FLAG_HOLDING)) continue;
     int movetype=un->MoveType();
     int movecost=breg->MoveCost(movetype);
     if(movecost>un->CalcMovePoints(movetype))
      continue;
    }
    if(attacks){
     AddAttacker(un);
    }else{
     AddDefender(un);
    }
   }
  }
  if(i!=-1){
   reg->RunOrders(false);
  }
 }
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::bnClearAttClick(TObject *Sender)
{
 rep->atts->Clear();
 Draw();
 Modified=true;
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::bnSoldiersClick(TObject *Sender)
{
  if(!curUnit) return;
  SHORT shift=GetKeyState(VK_SHIFT)&0x8000;
  TStringList *list=new TStringList;
  int ind=rep->atts->IndexOf(curUnit);
  AUnits *uns=ind>=0?rep->atts:rep->defs;
  foreach(uns)
  {
  AUnit *un=*(AUnit**)iter;
  if(shift)
    un=curUnit;
  un->enditems->CreateNew(un->items);
  foreach(un->enditems){
    AItem *it=*(AItem**)iter;
    if(!it->count) continue;
    if(!it->IsSoldier()) continue;
    AItemType *itype=it->type;//GetType();
    for(int i=0;i<it->count;i++){
      ASoldier sldr(un,un->baseobj,rep->reg->type,itype);
      AnsiString str=sldr.Print();
      bool locbehind=(itype->type&AItemType::IT_MAN)&&un->GetFlag(FLAG_BEHIND);
      AMonstrType *mtype=itype->GetMonstrType();
      if(mtype)
        locbehind=mtype->flags&AMonstrType::BEHIND;
      if(locbehind){
        str="behind "+str;
      }
      int index=list->IndexOf(str);
      if(index<0)
        list->AddObject(str,(TObject*)1);
      else
        list->Objects[index]=(TObject*)(((int)list->Objects[index])+1);
    }
  }
  if(shift)
    break;
	}
	Reports->Clear();
	TStrings *list0=Reports->Lines;
	list0->BeginUpdate();
	list0->Clear();
	for(int i=0;i<list->Count;i++)
	{
		int num=(int)list->Objects[i];
		AnsiString str=AnsiString(num)+" * "+list->Strings[i];
		list0->Add(str);
	}
	list0->EndUpdate();
	delete list;
	Reports->ScrollTo(0,0);
	Reports->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::cbShortModeClick(TObject *Sender)
{
  bnViewClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TBattleSimForm::ReportMemoEvents(TObject* Sender, Classes::TShiftState Shift
	, TEAPKeyEvent KeyEvent, Word Key, const TPoint &CaretPos, const TPoint &MousePos)
{
	if(KeyEvent!=k_Up) return;
	if(!Shift.Contains(ssCtrl)) return;
	if(Key==VK_INSERT||Key=='C')
	{
		Reports->CopyToClipboard();
		return;
	}
	if(Key=='A')
	{
	  Reports->SelectAll();
		return;
	}
}
//---------------------------------------------------------------------------

