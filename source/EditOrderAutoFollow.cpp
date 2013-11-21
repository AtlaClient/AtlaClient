//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdlib.h>
#include <vector.h>
#include <algorithm>
#include "util.h"
#include "orders.h"
#include "items.h"
#include "unit.h"
#include "EditOrderAutoFollow.h"
#include "faction.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderAutoFollowForm *EditOrderAutoFollowForm;
//---------------------------------------------------------------------------
__fastcall TEditOrderAutoFollowForm::TEditOrderAutoFollowForm(TComponent* Owner, AOrderAutoFollow *_ord, AUnits *tars)
	: TForm(Owner),ord(_ord),targets(tars)
{
  curtarnum = ord->tarnum;
  cbTargets->Items->Clear();
  foreach(tars)
  {
	AUnit *un=*(AUnit**)iter;
	AnsiString s = un->FullName();
	s += ", ";
	s += un->faction->FullName();
	cbTargets->Items->AddObject(un->FullName(),(TObject*)un);
	if(curtarnum == un->num) cbTargets->ItemIndex = cbTargets->Items->Count - 1;
  }
  if( (cbTargets->Items->Count)&&(cbTargets->ItemIndex < 0) ) cbTargets->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderAutoFollowForm::bnOkClick(TObject *Sender)
{
  int indtar=cbTargets->ItemIndex;
  AUnit *un = 0;
  if(indtar < 0)
  {
	AnsiString str = cbTargets->Text;
	if(str.Length())
	{
	  if(isdigit(str[1])&&(str.Length()<2||isdigit(str[str.Length()-1])))
	  {
		int num = atoi(str.c_str());
		un = targets->GetUnit(num);
	}
  }
  if(!un)
	return;
  } else   un = (AUnit*)cbTargets->Items->Objects[indtar];

  if(ord->target != un)
  {
	ord->tarnum = un->num;
	ord->target = un;
  }

}
//---------------------------------------------------------------------------
