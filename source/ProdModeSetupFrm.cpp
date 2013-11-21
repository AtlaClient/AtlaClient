//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "ProdModeSetupFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TProdModeSetupForm *ProdModeSetupForm;
//---------------------------------------------------------------------------
__fastcall TProdModeSetupForm::TProdModeSetupForm(TComponent* Owner, unsigned char _mode)
    : TForm(Owner),mode(_mode)
{
 cbFood->Checked=mode&1;
 cbNormal->Checked=mode&2;
 cbAdvanced->Checked=mode&4;
 cbNull->Checked=mode&8;
 cbRace->Checked=mode&16;
}

void __fastcall TProdModeSetupForm::bnOkClick(TObject *Sender)
{
 mode=0;
 if(cbFood->Checked)mode|=1;
 if(cbNormal->Checked)mode|=2;
 if(cbAdvanced->Checked)mode|=4;
 if(cbNull->Checked)mode|=8;
 if(cbRace->Checked)mode|=16;
}
//---------------------------------------------------------------------------

