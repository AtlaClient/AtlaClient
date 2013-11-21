//---------------------------------------------------------------------------

#ifndef EditOrderAutoFollowH
#define EditOrderAutoFollowH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderAutoFollowForm : public TForm
{
__published:	// IDE-managed Components
	TButton *bnOk;
	TButton *bnCancel;
	TLabel *Label1;
	TComboBox *cbTargets;
	void __fastcall bnOkClick(TObject *Sender);
private:	// User declarations
    AUnits *targets;
    int curtarnum;
public:		// User declarations
	__fastcall TEditOrderAutoFollowForm(TComponent* Owner, AOrderAutoFollow *_ord, AUnits *tars);
	AOrderAutoFollow *ord;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderAutoFollowForm *EditOrderAutoFollowForm;
//---------------------------------------------------------------------------
#endif
