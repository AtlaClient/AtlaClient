//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "FlagFiltr.h"
#include "util.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfFlagFiltr *fFlagFiltr;
extern AnsiString keySpoils[5]; //from orders.cpp
//---------------------------------------------------------------------------
__fastcall TfFlagFiltr::TfFlagFiltr(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfFlagFiltr::FormCreate(TObject *Sender)
{
 if(optServSpoils)
 {
	cbSpoils->Items->Add('*');
	for(int i=0;i<5;i++)
	{
	  cbSpoils->Items->Add(keySpoils[i]);
	}
	cbSpoils->ItemIndex = 0;
	cbSpoils->Visible = true;
	lbSpoils->Visible = true;
 } else {
	cbNoSpoils->Visible = optServNoSpoils;
	lbNoSpoils->Visible = optServNoSpoils;
 }

  cbSharing->Visible = optServerSharing;
  cbNoCross->Visible = optServNoCross;
  lbSharing->Visible = optServerSharing;
  lbNoCross->Visible = optServNoCross;
}
//---------------------------------------------------------------------------
void __fastcall TfFlagFiltr::SetFlags(TarFlags af)
{
  cbGuard->ItemIndex = af.guard;
  cbAvoid->ItemIndex = af.avoid;
  cbReveal->ItemIndex = af.reveal;
  cbBehind->ItemIndex = af.behind;
  cbAutoTax->ItemIndex = af.autotax;
  cbHold->ItemIndex = af.hold;
  cbConsume->ItemIndex = af.consume;
  cbNoaid->ItemIndex = af.noaid;
  cbNoCross->ItemIndex = af.nocroswater;
  cbNoSpoils->ItemIndex = af.nobattspoils;
  cbSpoils->ItemIndex = af.spoils;
  cbSharing->ItemIndex = af.sharing;
}
//---------------------------------------------------------------------------

