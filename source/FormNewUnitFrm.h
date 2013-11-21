//---------------------------------------------------------------------------
#ifndef FormNewUnitFrmH
#define FormNewUnitFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include "cspin.h"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormNewUnitForm : public TForm
{
__published:	// IDE-managed Components
        TPanel *pn2;
        TRadioGroup *rgSpoils;
        TRadioGroup *rgConsume;
        TRadioGroup *rgReveal;
        TPanel *pn1;
        TPanel *pn3;
        TButton *bnCancel;
        TButton *bnOk;
        TGroupBox *gb2;
        TButton *bnAllB;
        TCheckBox *cbRepB;
		TCheckBox *cbCommB;
        TCheckBox *cbAllB;
        TComboBox *cbItemsB;
        TGroupBox *gb3;
        TComboBox *cbListS;
        TCheckBox *cbRepS;
        TCheckBox *cbCommS;
        TCheckBox *cbStudy;
        TGroupBox *gb4;
        TComboBox *cbTargetsU;
        TButton *btAllS;
        TLabel *Label5;
        TCheckBox *cbRepR;
        TGroupBox *gb6;
        TLabel *lbNeedB;
        TLabel *lbNeedS;
        TLabel *lbM;
        TLabel *Label8;
        TLabel *lbNeedM;
        TLabel *Label10;
        TLabel *Label11;
        TBevel *Bevel1;
        TLabel *lbNeedAll;
        TLabel *Label6;
        TGroupBox *gbC;
        TLabel *Label7;
        TLabel *Label9;
        TPanel *p0;
        TGroupBox *gb1;
        TLabel *Label4;
        TLabel *Label3;
        TLabel *Label2;
        TLabel *Label1;
        TEdit *edDescr;
        TEdit *edLocalDescr;
        TEdit *edName;
        TEdit *edAlias;
        TGroupBox *gb5;
        TCheckBox *cbGuard;
        TCheckBox *cbAvoid;
        TCheckBox *cbBehind;
        TCheckBox *cbNoCross;
        TCheckBox *cbNoSpoils;
        TCheckBox *cbNoAid;
        TCheckBox *cbHold;
        TCheckBox *cbAutoTax;
        TCheckBox *cbSharing;
	TCSpinEdit *edReceive;
	TCSpinEdit *edCopy;
	TCSpinEdit *edMonth;
	TCSpinEdit *edMonS;
	TSpeedButton *btAllNeedS;
	TCSpinEdit *edCountB;
    void __fastcall edAliasKeyPress(TObject *Sender, char &Key);
    void __fastcall cbGuardClick(TObject *Sender);
    void __fastcall cbAvoidClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
        void __fastcall cbTargetsUChange(TObject *Sender);
        void __fastcall btAllSClick(TObject *Sender);
        void __fastcall bnAllBClick(TObject *Sender);
        void __fastcall cbAllBClick(TObject *Sender);
        void __fastcall ChangeInfo(TObject *Sender);
	void __fastcall cbListSChange(TObject *Sender);
	void __fastcall btAllNeedSClick(TObject *Sender);
private:	// User declarations
        int CalcMoney();
public:		// User declarations
  __fastcall TFormNewUnitForm(TComponent* Owner);
  void __fastcall PrepareReg();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormNewUnitForm *FormNewUnitForm;
//---------------------------------------------------------------------------
#endif
