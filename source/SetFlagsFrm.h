//---------------------------------------------------------------------------

#ifndef SetFlagsFrmH
#define SetFlagsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TSetFlagsForm : public TForm
{
__published:	// IDE-managed Components
    TCheckBox *cbGuard;
    TCheckBox *cbAutoTax;
    TCheckBox *cbAvoid;
    TCheckBox *cbHold;
    TCheckBox *cbNoAid;
    TCheckBox *cbBehind;
    TRadioGroup *rgReveal;
    TRadioGroup *rgConsume;
    TButton *bnOk;
    TButton *bnCancel;
    TCheckBox *cbNoCross;
    TCheckBox *cbNoSpoils;
    TRadioGroup *rgSpoils;
        TCheckBox *cbSharing;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall cbGuardClick(TObject *Sender);
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall cbAvoidClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TSetFlagsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TSetFlagsForm *SetFlagsForm;
//---------------------------------------------------------------------------
#endif
