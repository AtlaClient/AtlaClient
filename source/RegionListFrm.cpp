//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "RegionListFrm.h"
#include "region.h"
#include "MainFrm.h"
#include "turn.h"
#include "exprlang.h"
#include "SaveMapFrm.h"
#include "reports.h"
#include "acplugins.h"
#include "ExpressionEditorFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRegionListForm *RegionListForm;
static AnsiString capt;
//---------------------------------------------------------------------------
int TRegionListForm::CoordsToInt(int x, int y, int z){
  return x+y*1000+z*1000*1000;
}
void TRegionListForm::IntToCoords(int key,int *x, int *y, int *z)
{
  if(x)
    *x=key%1000;
  key/=1000;
  if(y)
    *y=key%1000;
  if(z)
    *z=key/1000;
}

static AnsiString IniSect="RegionListWin";
__fastcall TRegionListForm::TRegionListForm(TComponent* Owner)
  : TForm(Owner),internal(false),list(new TStringList)
{
  TIniFile *ini=new TIniFile(IniName);
  Left=atoi(ini->ReadString(IniSect,"Left",Left).c_str());
  Top=atoi(ini->ReadString(IniSect,"Top",Top).c_str());
  Width=atoi(ini->ReadString(IniSect,"Width",Width).c_str());
  Height=atoi(ini->ReadString(IniSect,"Height",Height).c_str());
  delete ini;
  expr="distance(curxloc,curyloc,curzloc)<=2";
  capt=Caption;
}
//---------------------------------------------------------------------------
__fastcall TRegionListForm::~TRegionListForm()
{
  delete list;
}
//---------------------------------------------------------------------------
void __fastcall TRegionListForm::FormDestroy(TObject *Sender)
{
 TIniFile *ini=new TIniFile(IniName);
 ini->WriteString(IniSect,"Left",Left);
 ini->WriteString(IniSect,"Top",Top);
 ini->WriteString(IniSect,"Width",Width);
 ini->WriteString(IniSect,"Height",Height);
 delete ini;
}
//---------------------------------------------------------------------------
bool TRegionListForm::InRegionList(int x, int y, int z)
{
  return list->IndexOfObject((TObject*)CoordsToInt(x,y,z))>=0;
}
//---------------------------------------------------------------------------
void TRegionListForm::AddRegion(ARegion * reg)
{
  if(!reg||!reg->hasInfo) return;
  AnsiString str=reg->FullName();
  int key=CoordsToInt(reg->xloc,reg->yloc,reg->zloc);
  int ind=FindInsertIndex(key);
  if(ind<0) return;
  list->InsertObject(ind,str,(TObject*)key);
  if(!internal)
    UpdateListWidth();
}
//---------------------------------------------------------------------------
void TRegionListForm::DeleteRegion(ARegion * reg)
{
  int key=CoordsToInt(reg->xloc,reg->yloc,reg->zloc);
  int ind=list->IndexOfObject((TObject*)key);
  if(ind<0) return;
  list->Delete(ind);
  if(!internal)
    UpdateListWidth();
}
//---------------------------------------------------------------------------
int TRegionListForm::FindInsertIndex(int key)
{
  for(int i=0,endi=list->Count;i<endi;i++){
    register int delta=(int)list->Objects[i]-key;
    if(delta<0) continue;
    if(delta>0)
      return i;
    return -i-1;
  }
  return list->Count;
}
//---------------------------------------------------------------------------
void TRegionListForm::UpdateListWidth()
{
  if(list->Count)
    Caption=capt+" ("+list->Count+")";
  else
    Caption=capt;
  int maxsize=0;
  TCanvas *Canv=List->Canvas;
  for(int i=0,endi=list->Count;i<endi;i++){
    TSize siz=Canv->TextExtent(list->Strings[i]);
    if(siz.cx>maxsize)maxsize=siz.cx;
 }
 List->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
 List->Items->Assign(list);
}
//---------------------------------------------------------------------------
void __fastcall TRegionListForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 AtlaForm->actShowRegionListExecute(0);
}
//---------------------------------------------------------------------------
void __fastcall TRegionListForm::miSaveListClick(TObject *Sender)
{
 SaveDialog->Filter="Region list files (*.regl)|*.regl|All files|*.*";
 SaveDialog->DefaultExt="regl";
 if(!LoadRegionListDir.Length()){
  LoadRegionListDir=GetGameFileName("");
 }
 if(LoadRegionListDir.Length()){
  SaveDialog->InitialDir=LoadRegionListDir;
  if(ExtractFilePath(SaveDialog->FileName)!=LoadRegionListDir)
   SaveDialog->FileName="";
 }
 if(!SaveDialog->Execute()) return;
 LoadRegionListDir=ExtractFilePath(SaveDialog->FileName);
 AnsiString fname=SaveDialog->FileName;

 TStringList *flist=new TStringList;
 try{
  for(int i=0,endi=list->Count;i<endi;i++){
    flist->Add((int)list->Objects[i]);
  }
  flist->SaveToFile(fname);
 }__finally{
  delete flist;
 }
}
//---------------------------------------------------------------------------
void __fastcall TRegionListForm::miLoadListClick(TObject *Sender)
{
 OpenDialog->Filter="Region list files (*.regl)|*.regl|All files|*.*";
 OpenDialog->DefaultExt="regl";
 if(!LoadRegionListDir.Length()){
  LoadRegionListDir=GetGameFileName("");
 }
 if(LoadRegionListDir.Length()){
  OpenDialog->InitialDir=LoadRegionListDir;
  if(ExtractFilePath(OpenDialog->FileName)!=LoadRegionListDir)
   OpenDialog->FileName="";
 }
 if(!OpenDialog->Execute()) return;
 LoadRegionListDir=ExtractFilePath(OpenDialog->FileName);
 AnsiString fname=OpenDialog->FileName;

 TStringList *flist=new TStringList;
 internal=true;
 try{
  flist->LoadFromFile(fname);
  for(int i=0,endi=flist->Count;i<endi;i++){
    int x,y,z;
    int key=atoi(flist->Strings[i].c_str());
    IntToCoords(key,&x,&y,&z);
    ARegion *reg=curRegionList->Get(x,y,z);
    AddRegion(reg);
  }
 }__finally{
  delete flist;
  internal=false;
 }
 UpdateListWidth();
 AtlaForm->DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TRegionListForm::miDeleteClick(TObject *Sender)
{
  int ind=List->ItemIndex;
  if(ind<0) return;
  list->Delete(ind);
  UpdateListWidth();
  if(ind>=list->Count)
    ind=list->Count-1;
  List->ItemIndex=ind;
  AtlaForm->DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TRegionListForm::miAddByExpressionClick(TObject *Sender)
{
  TExpressionEditorForm *frm=new TExpressionEditorForm(this);
  frm->mode=emRegion;
  frm->expr=expr;
  frm->Caption="Add regions by expression";
  int ret=frm->ShowModal();
  if(ret==mrOk)
  {
    expr=frm->expr;
  }
  delete frm;
  if(ret!=mrOk) return;
  internal=true;
  foreach(curRegionList)
  {
    ARegion *reg=*(ARegion**)iter;
    if(!reg->hasInfo) continue;
    try{
      ExprSetRegionMode(reg);
      bool res=ExprProcessExpr(expr);
      if(!res) continue;
      AddRegion(reg);
    }catch (Exception &exception){
      Application->ShowException(&exception);
      break;
    }
  }
  internal=false;
  UpdateListWidth();
  AtlaForm->DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TRegionListForm::ListDblClick(TObject *Sender)
{
  int ind=List->ItemIndex;
  if(ind<0) return;
  int x,y,z;
  int key=(int)list->Objects[ind];
  IntToCoords(key,&x,&y,&z);
  ARegion *reg=curRegionList->Get(x,y,z);
  if(!reg) return;
  AtlaForm->GotoRegion(reg);
}
//---------------------------------------------------------------------------
void __fastcall TRegionListForm::miAddCurrentClick(TObject *Sender)
{
  AddRegion(curRegion);
  UpdateListWidth();
  AtlaForm->DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TRegionListForm::miSaveMapClick(TObject *Sender)
{
  {
    TSaveMapForm *frm=new TSaveMapForm(this);
    if(frm->ShowModal()!=mrOk) return;
    delete frm;
  }
  AnsiString fName=GetGameFileName(ReportsDir+"map."+curTurn->num);
  if(!SaveMap(fName,optSaveMap|roRegionList)){
    AnsiString str="Error saving map";
    AddEvent(str);
  }
}
//---------------------------------------------------------------------------
//declared in util.h
bool InRegionList(ARegion *reg)
{
  if(!reg) return false;
  return RegionListForm->InRegionList(reg->xloc,reg->yloc,reg->zloc);
}
//---------------------------------------------------------------------------
void __fastcall TRegionListForm::miClearListClick(TObject *Sender)
{
  list->Clear();
  UpdateListWidth();
  AtlaForm->DrawMap();
}
//---------------------------------------------------------------------------
bool TRegionListForm::FillList(PluginVariable *l)
{
  for(int i=0,endi=list->Count;i<endi;i++){
    int x,y,z;
    int key=(int)list->Objects[i];
    IntToCoords(key,&x,&y,&z);
    ARegion *reg=curRegionList->Get(x,y,z);
    if(!reg) return false;
    if(!ListAddReg(l,reg)) return false;
  }
  return true;
}
//---------------------------------------------------------------------------
int __fastcall TRegionListForm::GetRegionCount()
{
  return list->Count;
}
//---------------------------------------------------------------------------
ARegion* __fastcall TRegionListForm::GetRegions(int index)
{
  int x,y,z;
  int key=(int)list->Objects[index];
  IntToCoords(key,&x,&y,&z);
  return curRegionList->Get(x,y,z);
}
//---------------------------------------------------------------------------

