//---------------------------------------------------------------------------
#ifndef SetLevelSizeFrmH
#define SetLevelSizeFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TSetLevelSizeForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *panSettings;
  TLabel *Label1;
  TComboBox *cbLevel;
  TLabel *Label2;
  TEdit *edX;
  TLabel *Label3;
  TEdit *edY;
  TButton *bnSet;
  void __fastcall cbLevelChange(TObject *Sender);
  void __fastcall bnSetClick(TObject *Sender);
  void __fastcall edXKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
  __fastcall TSetLevelSizeForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TSetLevelSizeForm *SetLevelSizeForm;
//---------------------------------------------------------------------------
#endif
