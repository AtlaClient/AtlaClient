//---------------------------------------------------------------------------
#ifndef EditOrderByteFrmH
#define EditOrderByteFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderByteForm : public TForm
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
    __fastcall TEditOrderByteForm(TComponent* Owner, AOrderByte *_ord, TStringList *list);
    AOrderByte *ord;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderByteForm *EditOrderByteForm;
//---------------------------------------------------------------------------
#endif
