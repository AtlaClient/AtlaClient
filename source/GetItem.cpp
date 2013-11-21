//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "GetItem.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TfGetItem *fGetItem;
//---------------------------------------------------------------------------
__fastcall TfGetItem::TfGetItem(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
