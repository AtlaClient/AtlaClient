//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "ShowItemTypesFrm.h"
#include "util.h"
#include "unit.h"
#include "orders.h"
#include "faction.h"
#include "MainFrm.h"
#include "EditItems.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TShowItemTypesForm *ShowItemTypesForm;
//---------------------------------------------------------------------------
__fastcall TShowItemTypesForm::TShowItemTypesForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TShowItemTypesForm::FormCreate(TObject *Sender)
{
 if(!ItemTypes)return;
 int maxsize=0;
 foreach(ItemTypes){
  AItemType*ttype=*(AItemType**)iter;
  AnsiString str=ttype->Print();
  List->Items->Add(str);
  TSize siz=List->Canvas->TextExtent(str);
  if(siz.cx>maxsize)maxsize=siz.cx;
 }
 List->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
 bnRequery->Enabled=(curUnit!=0);
}
//---------------------------------------------------------------------------
void __fastcall TShowItemTypesForm::FormShow(TObject *Sender)
{
 if(!ItemTypes)ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TShowItemTypesForm::bnEditClick(TObject *Sender)
{
  int num = List->ItemIndex;
  if(num == -1)return;
  AItemType*stype = ItemTypes->Get(num);

  AnsiString descr = "";
  AItObjInfo *ioi = ItObjInfos->Get(stype->abr,AItObjInfo::ITEM);
  if(ioi) descr=ioi->text;
  TfEditItems *frm = new TfEditItems(this, stype, 0, descr);
  if(frm->ShowModal()==mrOk){
	// Нужно бы сохранить это всё.

  }
  delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TShowItemTypesForm::bnRequeryClick(TObject *Sender)
{
  if(!curUnit)return;
  if(!curUnit->faction->local)return;
  int num=List->ItemIndex;
  if(num==-1)return;
  AItemType *itype=ItemTypes->Get(num);
  if(ItemTypes->IsIllusionAbr(itype->abr)) return;
  AOrderCustom *ord=new AOrderCustom;
  ord->text="show item "+itype->abr;
  curUnit->orders->Add(ord);
  curRegion->runned=false;
  AtlaForm->ViewShow();
}


void __fastcall TShowItemTypesForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 Action=caFree;
}
//---------------------------------------------------------------------------

void __fastcall TShowItemTypesForm::bnCancelClick(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------

void __fastcall TShowItemTypesForm::FormDestroy(TObject *Sender)
{
  ShowItemTypesForm = 0;        
}
//---------------------------------------------------------------------------

