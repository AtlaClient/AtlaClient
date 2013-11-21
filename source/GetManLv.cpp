//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
//---------------------------------------------------------------------------
#include "GetManLv.h"
#include "util.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TfGetManLv *fGetSkilLv;
//---------------------------------------------------------------------------
__fastcall TfGetSkilLv::TfGetSkilLv(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfGetSkilLv::FormCreate(TObject *Sender)
{
  foreach(ManTypes)
  {
	AManType *man  = *(AManType**) iter;
	cbMans->Items->Add(ItemTypes->AbrToNames(man->abr));
  }
  cbMans->ItemIndex = 0;
}
//---------------------------------------------------------------------------
