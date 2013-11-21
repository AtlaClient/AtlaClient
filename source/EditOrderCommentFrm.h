//---------------------------------------------------------------------------
#ifndef EditOrderCommentFrmH
#define EditOrderCommentFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderCommentForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TEdit *edText;
    TLabel *Label1;
    TButton *bnConvertCustom;
    TButton *bnConvertLocal;
    TCheckBox *cbRepeating;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall bnConvertLocalClick(TObject *Sender);
    void __fastcall bnConvertCustomClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TEditOrderCommentForm(TComponent* Owner, AOrderComment *_ord);
    AOrderComment *ord;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderCommentForm *EditOrderCommentForm;
//---------------------------------------------------------------------------
#endif
