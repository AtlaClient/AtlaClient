//---------------------------------------------------------------------------
#ifndef EditOrderSimpleFrmH
#define EditOrderSimpleFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderSimpleForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TLabel *labInfo;
    TCheckBox *cbCommented;
    TCheckBox *cbRepeating;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TEditOrderSimpleForm(TComponent* Owner, AOrderSimple *_ord);
    AOrderSimple *ord;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderSimpleForm *EditOrderSimpleForm;
//---------------------------------------------------------------------------
#endif
