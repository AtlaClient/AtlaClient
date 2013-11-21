//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "EditObjTypeFrm.h"
#include "util.h"
#include "ShowObjTypesFrm.h"
#include "unit.h"
#include "orders.h"
#include "faction.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TShowObjTypesForm *ShowObjTypesForm;
//---------------------------------------------------------------------------
__fastcall TShowObjTypesForm::TShowObjTypesForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TShowObjTypesForm::FormCreate(TObject *Sender)
{
  if(!ObjectTypes)return;

 int maxsize=0;
 foreach(ObjectTypes){
  AObjectType*ttype=*(AObjectType**)iter;
  AnsiString str=ttype->Print(false);
  List->Items->Add(str);
  TSize siz=List->Canvas->TextExtent(str);
  if(siz.cx>maxsize)maxsize=siz.cx;
 }
 List->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
 bnRequery->Enabled=(curUnit!=0);
}
//---------------------------------------------------------------------------
void __fastcall TShowObjTypesForm::bnCancelClick(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TShowObjTypesForm::bnRequeryClick(TObject *Sender)
{
 if(!curUnit)return;
 if(!curUnit->faction->local)return;
 int num=List->ItemIndex;
 if(num==-1)return;
 AObjectType *otype=ObjectTypes->Get(num);
 AOrderCustom *ord=new AOrderCustom;
 ord->text="show object \""+otype->name+"\"";
 curUnit->orders->Add(ord);
 curRegion->runned=false;
 AtlaForm->ViewShow();
}
//---------------------------------------------------------------------------
void __fastcall TShowObjTypesForm::bnEditClick(TObject *Sender)
{
  int num = List->ItemIndex;
  if(num == -1)return;
  AObjectType *stype = ObjectTypes->Get(num);
  AnsiString descr;
  AItObjInfo *ioi = ItObjInfos->Get(stype->name, 0);
  if(ioi) descr = ioi->text;

  TEditObjTypeForm *frm = new TEditObjTypeForm(this, stype, descr);
  if(frm->ShowModal() == mrOk){
    AnsiString str=stype->Print(false);
    List->Items->Strings[num]=str;
    int maxsize=List->Perform(LB_GETHORIZONTALEXTENT,0,0)-5;
    TSize siz=List->Canvas->TextExtent(str);
    if(siz.cx>maxsize)maxsize=siz.cx;
    List->Perform(LB_SETHORIZONTALEXTENT,maxsize+5,0);
  }
  delete frm;

}
//---------------------------------------------------------------------------
void __fastcall TShowObjTypesForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 Action=caFree;
}
//---------------------------------------------------------------------------

void __fastcall TShowObjTypesForm::FormDestroy(TObject *Sender)
{
  ShowObjTypesForm = 0;
}
//---------------------------------------------------------------------------

