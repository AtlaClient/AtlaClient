//---------------------------------------------------------------------------
#ifndef ProdModeSetupFrmH
#define ProdModeSetupFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TProdModeSetupForm : public TForm
{
__published:	// IDE-managed Components
    TCheckBox *cbFood;
    TCheckBox *cbNormal;
    TCheckBox *cbAdvanced;
    TCheckBox *cbNull;
    TButton *bnOk;
    TButton *bnCancel;
    TCheckBox *cbRace;
    void __fastcall bnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TProdModeSetupForm(TComponent* Owner, unsigned char _mode);
    unsigned char mode;
};
//---------------------------------------------------------------------------
extern PACKAGE TProdModeSetupForm *ProdModeSetupForm;
//---------------------------------------------------------------------------
#endif
