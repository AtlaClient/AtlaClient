//---------------------------------------------------------------------------

#ifndef ChangeAttH
#define ChangeAttH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfChAtt : public TForm
{
__published:	// IDE-managed Components
        TComboBox *cbAtt;
        TButton *Button1;
        TButton *Button2;
        TLabel *Label1;
private:	// User declarations
public:		// User declarations
        __fastcall TfChAtt(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfChAtt *fChAtt;
//---------------------------------------------------------------------------
#endif
