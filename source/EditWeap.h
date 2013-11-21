//---------------------------------------------------------------------------

#ifndef EditWeapH
#define EditWeapH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "util.h"
#include <Mask.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfEditWeap : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TComboBox *cbSkill;
        TLabel *lbSk2;
        TComboBox *cbSkill2;
        TGroupBox *GroupBox1;
        TLabel *Label2;
        TMaskEdit *edAttBonus;
        TLabel *Label3;
        TMaskEdit *edDefBonus;
        TLabel *Label4;
        TMaskEdit *edMountBonus;
        TLabel *Label5;
        TMaskEdit *edNumAtt;
        TLabel *Label6;
        TLabel *Label7;
        TMaskEdit *edPerRound;
        TCheckBox *cbALWAYSREADY;
        TCheckBox *cbNODEFENSE;
        TCheckBox *cbNOFOOT;
        TCheckBox *cbNOMOUNT;
        TCheckBox *cbSHORT;
        TCheckBox *cbLONG;
        TCheckBox *cbRANGED;
        TCheckBox *cbNOATTACKERSKILL;
        TCheckBox *cbRIDINGBONUS;
        TCheckBox *cbRIDINGBONUSDEFENSE;
        TMemo *Memo;
        TLabel *Label8;
        TComboBox *cbWeapClass;
        TLabel *Label9;
        TComboBox *cbAttType;
        TButton *bnCancel;
        TButton *bnOk;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall cbSkillChange(TObject *Sender);
        void __fastcall bnOkClick(TObject *Sender);
        void __fastcall bnCancelClick(TObject *Sender);
private:	// User declarations
        bool bNewWeap;
public:		// User declarations
        AnsiString TekItem;
        __fastcall TfEditWeap(TComponent* Owner, 
                   AWeaponType *weap, AnsiString descr);
protected:
        AWeaponType *Fweap;
};
//---------------------------------------------------------------------------
extern PACKAGE TfEditWeap *fEditWeap;
//---------------------------------------------------------------------------
#endif
