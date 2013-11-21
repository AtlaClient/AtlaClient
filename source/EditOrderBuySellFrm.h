//---------------------------------------------------------------------------
#ifndef EditOrderBuySellFrmH
#define EditOrderBuySellFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderBuySellForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TCheckBox *cbCommented;
    TCheckBox *cbRepeating;
    TComboBox *cbItems;
    TLabel *Label;
    TEdit *edCount;
        TButton *bnAll;
  TLabel *labPrice;
  TCheckBox *cbAll;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall edCountExit(TObject *Sender);
        void __fastcall bnAllClick(TObject *Sender);
  void __fastcall MoneyChange(TObject *Sender);
  void __fastcall cbAllClick(TObject *Sender);
private:	// User declarations
    AMarkets *markets;
public:		// User declarations
    __fastcall TEditOrderBuySellForm(TComponent* Owner, AOrderBuySell *_ord, AMarkets *marks);
    AOrderBuySell *ord;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderBuySellForm *EditOrderBuySellForm;
//---------------------------------------------------------------------------
#endif
