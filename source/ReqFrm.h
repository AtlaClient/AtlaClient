//---------------------------------------------------------------------------

#ifndef ReqFrmH
#define ReqFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TReqForm : public TForm
{
__published:	// IDE-managed Components
        TButton *bt1;
        TButton *bt2;
        TButton *btCancel;
        TLabel *Label1;
private:	// User declarations
public:		// User declarations
        __fastcall TReqForm(TComponent* Owner);
        int __fastcall ShowFrm(AnsiString l, AnsiString s1, AnsiString s2);
};
//---------------------------------------------------------------------------
extern PACKAGE TReqForm *ReqForm;
//---------------------------------------------------------------------------
#endif
