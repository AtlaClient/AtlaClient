//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "EditScript.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfEditScript *fEditScript;
//---------------------------------------------------------------------------
__fastcall TfEditScript::TfEditScript(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
