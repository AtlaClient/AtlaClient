//---------------------------------------------------------------------------
#ifndef ExpressionEditorFrmH
#define ExpressionEditorFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class AUnit;
class ARegion;
class TExpressionEditorForm : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label2;
  TLabel *labValue;
  TButton *bnOk;
  TButton *bnCancel;
  TEdit *edExpr;
  TButton *bnVerify;
  TButton *bnExpr;
  TStringGrid *Grid;
  TButton *bnInsert;
  TButton *bnAdd;
  TButton *bnDel;
  TBitBtn *bnSet;
  TBitBtn *bnGet;
  TPopupMenu *PopupMenu;
  TMenuItem *N1;
  TMenuItem *N2;
  TMenuItem *N3;
  TMenuItem *N4;
  TMenuItem *N5;
  TMenuItem *N6;
  TMenuItem *N7;
  void __fastcall bnOkClick(TObject *Sender);
  void __fastcall bnVerifyClick(TObject *Sender);
  void __fastcall bnExprClick(TObject *Sender);
  void __fastcall ExprClick(TObject *Sender);
  void __fastcall bnSetClick(TObject *Sender);
  void __fastcall bnGetClick(TObject *Sender);
  void __fastcall bnInsertClick(TObject *Sender);
  void __fastcall bnAddClick(TObject *Sender);
  void __fastcall bnDelClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
  bool CalcExpr();
public:		// User declarations
  int mode;
  AnsiString expr;
  bool numbermode;
  AUnit *unit;
  ARegion *reg;

  __fastcall TExpressionEditorForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TExpressionEditorForm *ExpressionEditorForm;
//---------------------------------------------------------------------------
#endif
