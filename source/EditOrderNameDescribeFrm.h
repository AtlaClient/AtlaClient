//---------------------------------------------------------------------------
#ifndef EditOrderNameDescribeFrmH
#define EditOrderNameDescribeFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TEditOrderNameDescribeForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TCheckBox *cbCommented;
  TEdit *edValue;
  TRadioGroup *RadioGroup;
  TButton *bnGet;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
  void __fastcall bnGetClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TEditOrderNameDescribeForm(TComponent* Owner, AOrderNameDescribe *_ord);
    AOrderNameDescribe *ord;
};
//---------------------------------------------------------------------------
#endif
