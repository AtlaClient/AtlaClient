//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "ShowItObjInfosFrm.h"
#include "util.h"
#include "unit.h"
#include "faction.h"
#include "orders.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TShowItObjInfosForm *ShowItObjInfosForm;
//---------------------------------------------------------------------------
__fastcall TShowItObjInfosForm::TShowItObjInfosForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TShowItObjInfosForm::FormCreate(TObject *Sender)
{
 if(!SkillInfos)return;
 int maxsize=0;
 int type=cbItems->Checked;
 List->Sorted=true;
 TStrings *list=List->Items;
 list->BeginUpdate();
 list->Clear();
 foreach(ItObjInfos){
  AItObjInfo*ioi=*(AItObjInfo**)iter;
  if(ioi->type!=type) continue;
  AnsiString str=ioi->text;
  list->AddObject(str,(TObject*)ioi);
  TSize siz=List->Canvas->TextExtent(str);
  if(siz.cx>maxsize)maxsize=siz.cx;
 }
 list->EndUpdate();
 List->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
 Memo->Lines->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TShowItObjInfosForm::FormShow(TObject *Sender)
{
 if(!ItObjInfos)ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TShowItObjInfosForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TShowItObjInfosForm::bnCancelClick(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TShowItObjInfosForm::ListClick(TObject *Sender)
{
 int ind=List->ItemIndex;
 if(ind<0) return;
 AItObjInfo *ioi=(AItObjInfo*)List->Items->Objects[ind];
 Memo->Lines->Text=ioi->text;
}
//---------------------------------------------------------------------------
void __fastcall TShowItObjInfosForm::bnRequeryClick(TObject *Sender)
{
 int ind=List->ItemIndex;
 if(ind<0) return;
 AItObjInfo *ioi=(AItObjInfo*)List->Items->Objects[ind];
 RequeryInfo(ioi);
 curRegion->runned=false;
 AtlaForm->ViewShow();
}
//---------------------------------------------------------------------------
void TShowItObjInfosForm::RequeryInfo(AItObjInfo * ioi)
{
  if(!ioi) return;
  if(!curUnit)return;
  if(!curUnit->faction->local)return;
  AOrderCustom *ord=new AOrderCustom;
  AnsiString str;
  if(ioi->type==AItObjInfo::OBJECT)
    str="object";
  else if(ioi->type==AItObjInfo::ITEM)
    str="item";
  else
    return;
  ord->text="show "+str+" "+ioi->abr;
  curUnit->orders->Add(ord);
}
//---------------------------------------------------------------------------
void __fastcall TShowItObjInfosForm::bnRequeryAllClick(TObject *Sender)
{
  if(!curUnit)return;
  if(!curUnit->faction->local)return;
  int type=cbItems->Checked;
  foreach(ItObjInfos)
  {
    AItObjInfo *ioi=*(AItObjInfo**)iter;
    if(ioi->type!=type) continue;
    RequeryInfo(ioi);
  }
  curRegion->runned=false;
  AtlaForm->ViewShow();
}
//---------------------------------------------------------------------------
void __fastcall TShowItObjInfosForm::cbItemsClick(TObject *Sender)
{
  FormCreate(0);
}
//---------------------------------------------------------------------------

