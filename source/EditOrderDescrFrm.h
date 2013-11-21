//---------------------------------------------------------------------------
#ifndef EditOrderDescrFrmH
#define EditOrderDescrFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderDescrForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TEdit *edText;
    TLabel *Label1;
    TButton *bnConvertComment;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall bnConvertCommentClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TEditOrderDescrForm(TComponent* Owner, AOrderLocDescr *_ord);
    AOrderLocDescr *ord;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderDescrForm *EditOrderDescrForm;
//---------------------------------------------------------------------------
#endif
