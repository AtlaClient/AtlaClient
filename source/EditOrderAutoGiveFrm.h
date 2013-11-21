//---------------------------------------------------------------------------
#ifndef EditOrderAutoGiveFrmH
#define EditOrderAutoGiveFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
//---------------------------------------------------------------------------
class TEditOrderAutoGiveForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TComboBox *cbItems;
    TRadioButton *rbExcept;
    TRadioButton *rbAll;
    TRadioButton *rbNumber;
    TEdit *edNumber;
    TEdit *edBegStr;
    TLabel *Label1;
    TEdit *edSubStr;
    TLabel *Label2;
    TLabel *Label3;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall rbExceptClick(TObject *Sender);
    void __fastcall rbAllClick(TObject *Sender);
    void __fastcall rbNumberClick(TObject *Sender);
private:	// User declarations
    TStringList *namelist, *abrlist;
public:		// User declarations
    __fastcall TEditOrderAutoGiveForm(TComponent* Owner, AOrderAutoGive *_ord);
    AOrderAutoGive *ord;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderAutoGiveForm *EditOrderAutoGiveForm;
//---------------------------------------------------------------------------
#endif
