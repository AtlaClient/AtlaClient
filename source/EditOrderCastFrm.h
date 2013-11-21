//---------------------------------------------------------------------------
#ifndef EditOrderCastFrmH
#define EditOrderCastFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderCastForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TCheckBox *cbCommented;
    TCheckBox *cbRepeating;
    TComboBox *cbList;
    TEdit *edParams;
    TLabel *Label1;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
private:	// User declarations
    TStringList *namelist, *abrlist;
public:		// User declarations
    __fastcall TEditOrderCastForm(TComponent* Owner, AOrderCast *_ord,TStringList *names, TStringList *abrs);
    AOrderCast *ord;
};
//---------------------------------------------------------------------------
//extern PACKAGE TEditOrderCastForm *EditOrderCastForm;
//---------------------------------------------------------------------------
#endif
