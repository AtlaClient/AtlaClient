//---------------------------------------------------------------------------
#ifndef WedgeModeSetupFrmH
#define WedgeModeSetupFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
extern AnsiString wedgeexpr[6];
class TWedgeModeSetupForm : public TForm
{
__published:	// IDE-managed Components
    TEdit *Edit1;
    TButton *bnOk;
    TEdit *Edit2;
    TEdit *Edit3;
    TEdit *Edit4;
    TEdit *Edit5;
    TEdit *Edit6;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TButton *bnCancel;
    TButton *bnExpr;
    TPopupMenu *PopupMenu;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *N3;
    TMenuItem *N4;
    TMenuItem *N5;
    TMenuItem *N6;
    TMenuItem *N7;
    TButton *bnVerify;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *LabelReg;
    TButton *bnSet1;
    TButton *bnGet1;
    TButton *bnSet2;
    TButton *bnGet2;
    TButton *bnSet3;
    TButton *bnGet3;
    TButton *bnSet4;
    TButton *bnGet4;
    TButton *bnSet5;
    TButton *bnGet5;
    TButton *bnSet6;
    TButton *bnGet6;
    TStringGrid *Grid;
    TButton *bnAdd;
    TButton *bnDel;
    TBevel *Bevel1;
    TButton *bnLoad;
    TPopupMenu *pmLoad;
    TButton *Save;
    TButton *bnInsert;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall EditEnter(TObject *Sender);
    void __fastcall EditExit(TObject *Sender);
    void __fastcall EditKeyPress(TObject *Sender, char &Key);
    void __fastcall bnExprClick(TObject *Sender);
    void __fastcall ExprClick(TObject *Sender);
    void __fastcall bnVerifyClick(TObject *Sender);
    void __fastcall bnAddClick(TObject *Sender);
    void __fastcall bnDelClick(TObject *Sender);
    void __fastcall bnSetClick(TObject *Sender);
    void __fastcall bnGetClick(TObject *Sender);
    void __fastcall bnLoadClick(TObject *Sender);
    void __fastcall pmLoadClick(TObject *Sender);
    void __fastcall SaveClick(TObject *Sender);
    void __fastcall bnInsertClick(TObject *Sender);
private:	// User declarations
    TLabel *labels[6];
    TLabel *colors[6];
    TEdit *edits[6];
    int lastedit;

    bool CalcExpr();
public:		// User declarations
    __fastcall TWedgeModeSetupForm(TComponent* Owner);
    static void FillGroupList(TStrings * list);
};
//---------------------------------------------------------------------------
//extern PACKAGE TWedgeModeSetupForm *WedgeModeSetupForm;
//---------------------------------------------------------------------------
#endif
