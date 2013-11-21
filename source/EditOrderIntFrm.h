//---------------------------------------------------------------------------
#ifndef EditOrderIntFrmH
#define EditOrderIntFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderIntForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TEdit *edValue;
    TLabel *labText;
    TCheckBox *cbCommented;
    TCheckBox *cbRepeating;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TEditOrderIntForm(TComponent* Owner, AOrderInt *_ord);
    AOrderInt *ord;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderIntForm *EditOrderIntForm;
//---------------------------------------------------------------------------
#endif
