//---------------------------------------------------------------------------
#ifndef EditOrderAbrFrmH
#define EditOrderAbrFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderAbrForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TCheckBox *cbCommented;
    TCheckBox *cbRepeating;
    TComboBox *cbList;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
private:	// User declarations
    TStringList *namelist, *abrlist;
public:		// User declarations
    __fastcall TEditOrderAbrForm(TComponent* Owner, AOrderAbr *_ord,TStringList *names, TStringList *abrs);
    AOrderAbr *ord;
};
//---------------------------------------------------------------------------
//extern PACKAGE TEditOrderAbrForm *EditOrderAbrForm;
//---------------------------------------------------------------------------
#endif
