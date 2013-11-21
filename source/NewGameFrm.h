//---------------------------------------------------------------------------
#ifndef NewGameFrmH
#define NewGameFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TNewGameForm : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TEdit *edName;
    TCheckBox *cbCreateNew;
    TButton *bnOk;
    TButton *Button1;
 TCheckBox *cbSaveOld;
    void __fastcall bnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TNewGameForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TNewGameForm *NewGameForm;
//---------------------------------------------------------------------------
#endif
