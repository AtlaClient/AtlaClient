//---------------------------------------------------------------------------
#ifndef StoreFrmH
#define StoreFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TStoreForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TEdit *edBeginTurn;
        TRadioButton *rbFirstTurn;
        TRadioButton *rbLastTurn;
        TEdit *edAutoBeginTurn;
    void __fastcall bnOkClick(TObject *Sender);
        void __fastcall rbClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TStoreForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TStoreForm *StoreForm;
//---------------------------------------------------------------------------
#endif
