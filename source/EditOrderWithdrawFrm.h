//---------------------------------------------------------------------------
#ifndef EditOrderWithdrawFrmH
#define EditOrderWithdrawFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
//---------------------------------------------------------------------------
class TEditOrderWithdrawForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TComboBox *cbItems;
    TEdit *edNumber;
  TCheckBox *cbRepeating;
  TCheckBox *cbCommented;
  TLabel *labUnclaimed;
  TLabel *labAttempted;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
  void __fastcall edNumberChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TEditOrderWithdrawForm(TComponent* Owner, AOrderWithdraw *_ord);
    AOrderWithdraw *ord;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderWithdrawForm *EditOrderWithdrawForm;
//---------------------------------------------------------------------------
#endif
