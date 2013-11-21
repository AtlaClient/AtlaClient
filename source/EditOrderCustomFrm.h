//---------------------------------------------------------------------------
#ifndef EditOrderCustomFrmH
#define EditOrderCustomFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderCustomForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TEdit *edText;
    TLabel *Label1;
    TButton *bnConvertComment;
    TButton *bnConvertOrder;
    TCheckBox *cbCommented;
    TCheckBox *cbRepeating;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall bnConvertCommentClick(TObject *Sender);
    void __fastcall bnConvertOrderClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TEditOrderCustomForm(TComponent* Owner, AOrderCustom *_ord);
    AOrderCustom *ord;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderCustomForm *EditOrderCustomForm;
//---------------------------------------------------------------------------
#endif
