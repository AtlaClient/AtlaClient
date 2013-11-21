//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <algorithm>

#include "ViewUnitsFrm.h"
#include "ViewUnitsSetupFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TViewUnitsSetupForm *ViewUnitsSetupForm;
static VUColumnHeaders headers;
//---------------------------------------------------------------------------
__fastcall TViewUnitsSetupForm::TViewUnitsSetupForm(TComponent* Owner)
  : TForm(Owner),curcolumn(0)
{
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnOkClick(TObject *Sender)
{
  headers.name.SetLength(0);
  TViewUnitsForm::Headers=headers;
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::FormShow(TObject *Sender)
{
  headers=TViewUnitsForm::Headers;
  MakeColumnList();
  MakeAvalItemList();
  MakeStoredColumns();
  MakeStoredSets();
}
//---------------------------------------------------------------------------
void TViewUnitsSetupForm::MakeColumnList(int newindex)
{
  TStrings *list=lbColumns->Items;
  list->BeginUpdate();
  list->Clear();
  for(VUColumnHeader *ch=headers.headers.begin().operator ->(), *endch=headers.headers.end().operator ->()
    ;ch<endch;ch++)
  {
    list->Add(ch->name);
  }
  list->EndUpdate();
  if(list->Count)
  {
    lbColumns->ItemIndex=newindex>=0?newindex:0;
  }
  MakeItemList();
}
//---------------------------------------------------------------------------
void TViewUnitsSetupForm::MakeItemList()
{
  int maxsize=0;
  TStrings *list=lbItems->Items;
  list->BeginUpdate();
  list->Clear();
  int ind=lbColumns->ItemIndex;
  curcolumn=0;
  int size=headers.headers.size();
  bool tabChanged=false;
  if(ind>=0&&ind<size)
  {
    VUColumnHeader &ch=headers.headers[ind];
    curcolumn=&ch;
    bool skills=ch.skills;
    lbItems->Sorted=!skills;
    if(tabItems->TabIndex!=skills)
      tabChanged=true;
    tabItems->TabIndex=skills;
    for(int i=0;i<ch.abrs->Count;i++)
    {
      AnsiString abr=ch.abrs->Strings[i];
      AnsiString str;
      if(skills){
        ASkillType *stype=SkillTypes->Get(abr,2);
        if(stype)
          str=stype->FullName();
        else
          str=" Bad skill ["+abr+"]";
      }else
      {
        AItemType *itype=ItemTypes->Get(abr,4);
        if(itype)
          str=itype->FullName();
        else
          str=" Bad item ["+abr+"]";
      }
      list->Add(str);
      TSize siz=lbItems->Canvas->TextExtent(str);
      if(siz.cx>maxsize)maxsize=siz.cx;
    }
  }
  list->EndUpdate();
  lbItems->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
  bnMoveUp->Enabled=size&&ind;
  bnMoveDown->Enabled=size&&ind<size-1&&ind>=0;
  if(tabChanged)
    MakeAvalItemList();
}
//---------------------------------------------------------------------------
void TViewUnitsSetupForm::MakeAvalItemList()
{
  int maxsize=0;
  bool skills=tabItems->TabIndex;
  TStrings *list=lbAvailItems->Items;
  list->BeginUpdate();
  list->Clear();
  lbAvailItems->Sorted=!skills;
  if(skills)
  {
    for(int i=0;i<SkillTypes->count;i++)
    {
      ASkillType *stype=SkillTypes->GetSorted(i);
      AnsiString str=stype->FullName();
      list->Add(str);
      TSize siz=lbAvailItems->Canvas->TextExtent(str);
      if(siz.cx>maxsize)maxsize=siz.cx;
    }
  }else{
    for(int i=0;i<ItemGroupCount;i++)
    {
      list->Add(ItemGroups[i]);
    }
    foreach(ItemTypes)
    {
      AItemType *itype=*(AItemType**)iter;
      if(itype->abr=="XXXX") continue;
      AnsiString str=itype->FullName();
      list->Add(str);
      TSize siz=lbAvailItems->Canvas->TextExtent(str);
      if(siz.cx>maxsize)maxsize=siz.cx;
    }
  }
  list->EndUpdate();
  lbAvailItems->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::lbColumnsClick(TObject *Sender)
{
  MakeItemList();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnAddItemClick(TObject *Sender)
{
  if(!curcolumn) return;
  int ind=lbAvailItems->ItemIndex;
  if(ind<0) return;
  AnsiString name=lbAvailItems->Items->Strings[ind];
  AnsiString abr;
  if(name[1]==':')
    abr=name;
  else
  {
    int pos=name.Pos("[");
    int pos2=name.Pos("]");
    if(pos==0||pos2<pos) return;
    abr=name.SubString(pos+1,pos2-pos-1);
  }
  if(!curcolumn->abrs->Count)
    curcolumn->skills=tabItems->TabIndex;
  curcolumn->Add(abr);
  MakeItemList();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnDelItemClick(TObject *Sender)
{
  if(!curcolumn) return;
  int ind=lbItems->ItemIndex;
  if(ind<0) return;
  AnsiString name=lbItems->Items->Strings[ind];
  int pos=name.Pos("[");
  int pos2=name.Pos("]");
  if(pos==0||pos2<pos) return;
  AnsiString abr=name.SubString(pos+1,pos2-pos-1);
  curcolumn->Delete(abr);
  MakeItemList();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnAddColumnClick(TObject *Sender)
{
  VUColumnHeader ch;
  ch.name="User column";
  headers.headers.push_back(ch);
  MakeColumnList(headers.headers.size()-1);
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnDelColumnClick(TObject *Sender)
{
  int ind=lbColumns->ItemIndex;
  if(ind<0||ind>=(int)headers.headers.size())
    return;
  curcolumn=headers.headers.begin().operator ->()+ind;
  AnsiString promt="Delete column \""+curcolumn->name+"\"?";
  if(Application->MessageBox(promt.c_str(),"",MB_YESNO)!=IDYES)
    return;
  headers.headers.erase(curcolumn);
  MakeColumnList();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::lbColumnsDblClick(TObject *Sender)
{
  if(!curcolumn) return;
  AnsiString str=curcolumn->name;
  AnsiString query="Enter new name for \""+str+"\"";
  if(!InputQuery("Rename column",query,str)) return;
  curcolumn->name=str;
  MakeColumnList(lbColumns->ItemIndex);
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnMoveUpClick(TObject *Sender)
{
  if(!curcolumn) return;
  swap(curcolumn[0],curcolumn[-1]);
  MakeColumnList(lbColumns->ItemIndex-1);
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnMoveDownClick(TObject *Sender)
{
  if(!curcolumn) return;
  swap(curcolumn[0],curcolumn[1]);
  MakeColumnList(lbColumns->ItemIndex+1);
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnClearItemsClick(TObject *Sender)
{
  if(!curcolumn) return;
  curcolumn->Clear();
  MakeItemList();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnColsSaveClick(TObject *Sender)
{
  if(!curcolumn) return;
  ViewUnitsCols->Add(AnsiString(*curcolumn));
  MakeStoredColumns();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnColsLoadClick(TObject *Sender)
{
  int index=lbStoredColumns->ItemIndex;
  if(index<0) return;
  AnsiString str=ViewUnitsCols->Strings[index];
  try{
    VUColumnHeader ch;
    ch=str;
    headers.headers.push_back(ch);
  }catch(...){
  }
  MakeColumnList(headers.headers.size()-1);
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnColsRenameClick(TObject *Sender)
{
  int index=lbStoredColumns->ItemIndex;
  if(index<0) return;
  AnsiString str=ViewUnitsCols->Strings[index];
  try{
    VUColumnHeader ch;
    ch=str;
    if(InputQuery("Rename stored Column","Enter new name",ch.name))
    {
      str=ch;
      ViewUnitsCols->Strings[index]=str;
    }
  }catch(...){
  }
  MakeStoredColumns();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnColsDeleteClick(TObject *Sender)
{
  int index=lbStoredColumns->ItemIndex;
  if(index<0) return;
  AnsiString str=ViewUnitsCols->Strings[index];
  char *s=str.c_str();
  AnsiString name=GetNextToken(s);
  AnsiString promt="Delete stored Column \""+name+"\"?";
  if(Application->MessageBox(promt.c_str(),"",MB_YESNO)!=IDYES)
    return;
  ViewUnitsCols->Delete(index);
  MakeStoredColumns();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnSetsSaveClick(TObject *Sender)
{
  AnsiString name="new set";
  if(!InputQuery("Save ColumnSet","Enter set name",name)) return;
  headers.name=name;
  ViewUnitsSets->Add(AnsiString(headers));
  MakeStoredSets();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnSetsLoadClick(TObject *Sender)
{
  int index=lbStoredSets->ItemIndex;
  if(index<0) return;
  AnsiString str=ViewUnitsSets->Strings[index];
  try{
    VUColumnHeaders h;
    h=str;
    AnsiString promt="Load ColumnSet \""+h.name+"\"?";
    if(Application->MessageBox(promt.c_str(),"",MB_YESNO)!=IDYES)
      return;
    headers=h;
  }catch(...){
  }
  MakeColumnList();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnSetsRenameClick(TObject *Sender)
{
  int index=lbStoredSets->ItemIndex;
  if(index<0) return;
  AnsiString str=ViewUnitsSets->Strings[index];
  try{
    VUColumnHeaders ch;
    ch=str;
    if(InputQuery("Rename stored ColumnSet","Enter new name",ch.name))
    {
      str=ch;
      ViewUnitsSets->Strings[index]=str;
    }
  }catch(...){
  }
  MakeStoredSets();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::bnSetsDeleteClick(TObject *Sender)
{
  int index=lbStoredSets->ItemIndex;
  if(index<0) return;
  AnsiString str=ViewUnitsSets->Strings[index];
  char *s=str.c_str();
  AnsiString name=GetNextToken(s);
  AnsiString promt="Delete stored ColumnSet \""+name+"\"?";
  if(Application->MessageBox(promt.c_str(),"",MB_YESNO)!=IDYES)
    return;
  ViewUnitsSets->Delete(index);
  MakeStoredSets();
}
//---------------------------------------------------------------------------
void TViewUnitsSetupForm::MakeStoredColumns()
{
  TStrings *list=lbStoredColumns->Items;
  list->BeginUpdate();
  list->Clear();
  AnsiString str;
  for(int i=0;i<ViewUnitsCols->Count;i++)
  {
    str=ViewUnitsCols->Strings[i];
    char *s=str.c_str();
    list->Add(GetNextToken(s));
  }
  list->EndUpdate();
}
//---------------------------------------------------------------------------
void TViewUnitsSetupForm::MakeStoredSets()
{
  TStrings *list=lbStoredSets->Items;
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
void __fastcall TViewUnitsSetupForm::tabItemsChange(TObject *Sender)
{
  MakeAvalItemList();
}
//---------------------------------------------------------------------------
void __fastcall TViewUnitsSetupForm::tabItemsChanging(TObject *Sender,
      bool &AllowChange)
{
  if(!curcolumn)
    return;
  if(!curcolumn->abrs->Count)
    return;
  ShowMessage("Can't mix skills and items");
  AllowChange=false;
}
//---------------------------------------------------------------------------

