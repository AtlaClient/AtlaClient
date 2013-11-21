//---------------------------------------------------------------------------

#ifndef EditScriptH
#define EditScriptH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfEditScript : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo1;
	TPanel *Panel1;
	TButton *bnEdit;
	TButton *Button1;
	TPanel *Panel2;
	TLabel *Label1;
	TEdit *edName;
	TCheckBox *cbClear;
private:	// User declarations
public:		// User declarations
	__fastcall TfEditScript(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfEditScript *fEditScript;
//---------------------------------------------------------------------------
#endif
