//---------------------------------------------------------------------------

#ifndef GetSkilLvH
#define GetSkilLvH
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
	TComboBox *cbSkills;
	TButton *btOk;
	TButton *btCan;
	TCSpinEdit *edLev;
private:	// User declarations
public:		// User declarations
	__fastcall TfGetSkilLv(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfGetSkilLv *fGetSkilLv;
//---------------------------------------------------------------------------
#endif
