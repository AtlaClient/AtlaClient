//---------------------------------------------------------------------------
#ifndef EditOrderAutoWarningFrmH
#define EditOrderAutoWarningFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TEditOrderAutoWarningForm : public TForm
{
__published:	// IDE-managed Components
  TButton *bnOk;
  TButton *bnCancel;
  TEdit *edMess;
  TLabel *Label1;
  TEdit *edExpr;
  TLabel *Label2;
  TButton *bnVerify;
  TLabel *labValue;
  TButton *bnExpr;
  TPopupMenu *PopupMenu;
  TMenuItem *N1;
  TMenuItem *N2;
  TMenuItem *N3;
  TMenuItem *N4;
  TMenuItem *N5;
  TMenuItem *N6;
  TMenuItem *N7;
  TStringGrid *Grid;
  TButton *bnInsert;
  TButton *bnAdd;
  TButton *bnDel;
  TBitBtn *bnSet;
  TBitBtn *bnGet;
  void __fastcall bnOkClick(TObject *Sender);
  void __fastcall bnCancelClick(TObject *Sender);
  void __fastcall bnVerifyClick(TObject *Sender);
  void __fastcall bnExprClick(TObject *Sender);
  void __fastcall ExprClick(TObject *Sender);
  void __fastcall bnSetClick(TObject *Sender);
  void __fastcall bnGetClick(TObject *Sender);
  void __fastcall bnInsertClick(TObject *Sender);
  void __fastcall bnAddClick(TObject *Sender);
  void __fastcall bnDelClick(TObject *Sender);
  void __fastcall labValueDblClick(TObject *Sender);
  void __fastcall edMessKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
  bool CalcExpr();
public:		// User declarations
  __fastcall TEditOrderAutoWarningForm(TComponent* Owner, AOrderAutoWarning*_ord);
  AOrderAutoWarning*ord;
};
//---------------------------------------------------------------------------
//extern PACKAGE TEditOrderAutoWarningForm *EditOrderAutoWarningForm;
//---------------------------------------------------------------------------
#endif
