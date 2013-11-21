//---------------------------------------------------------------------------

#ifndef GetItemH
#define GetItemH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include "cspin.h"
//---------------------------------------------------------------------------
class TfGetItem : public TForm
{
__published:	// IDE-managed Components
        TComboBox *cbInput;
        TLabel *labNumber;
        TButton *btOk;
        TButton *btCan;
	TCSpinEdit *edNum;
private:	// User declarations
public:		// User declarations
        __fastcall TfGetItem(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfGetItem *fGetItem;
//---------------------------------------------------------------------------
#endif
