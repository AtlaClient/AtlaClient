//---------------------------------------------------------------------------
#ifndef gameparamsfrmH
#define gameparamsfrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TGameParamsForm : public TForm
{
__published:	// IDE-managed Components
  TListBox *List;
  TEdit *edValue;
  TLabel *labComment;
  TButton *bnSet;
  void __fastcall ListClick(TObject *Sender);
  void __fastcall bnSetClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TGameParamsForm(TComponent* Owner);
  void CreateList();
};
//---------------------------------------------------------------------------
extern PACKAGE TGameParamsForm *GameParamsForm;
//---------------------------------------------------------------------------
#endif
