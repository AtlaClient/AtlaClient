//---------------------------------------------------------------------------

#ifndef GetManLvH
#define GetManLvH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "cspin.h"
//---------------------------------------------------------------------------
class TfGetSkilLv : public TForm
{
__published:	// IDE-managed Components
	TLabel *labNumber;
	TComboBox *cbMans;
	TButton *btOk;
	TButton *btCan;
	TCSpinEdit *edLev;
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfGetSkilLv(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfGetSkilLv *fGetSkilLv;
//---------------------------------------------------------------------------
#endif
