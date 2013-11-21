//---------------------------------------------------------------------------
#ifndef ChangeGameFrmH
#define ChangeGameFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TChangeGameForm : public TForm
{
__published:	// IDE-managed Components
    TListBox *lbGames;
    TButton *bnOk;
    TButton *bnCancel;
 TCheckBox *cbSaveOld;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall lbGamesDblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TChangeGameForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TChangeGameForm *ChangeGameForm;
//---------------------------------------------------------------------------
#endif
