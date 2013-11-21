//---------------------------------------------------------------------------

#ifndef EditItemsH
#define EditItemsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <Mask.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "cspin.h"
//---------------------------------------------------------------------------
class TfEditItems : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label4;
        TEdit *edName;
        TEdit *edAbr;
        TEdit *edNames;
        TGroupBox *GroupBox2;
        TCheckBox *cbNormal;
        TCheckBox *cbMagic;
        TCheckBox *cbTrade;
        TCheckBox *cbAdvanced;
        TCheckBox *cbFood;
        TCheckBox *cbCantGive;
        TCheckBox *cbCombat;
        TPageControl *PageControl1;
        TTabSheet *tbProd;
        TTabSheet *tbMProd;
        TTabSheet *tbArmor;
        TTabSheet *tbWeapon;
        TTabSheet *tbMonster;
        TTabSheet *tbMount;
        TMemo *Memo;
        TComboBox *cbItType;
        TLabel *Label3;
        TLabel *Label5;
        TTabSheet *tbMan;
        TGroupBox *GroupBox3;
        TLabel *Label6;
        TLabel *labLevel;
        TComboBox *cbSkill;
        TComboBox *cbLevel;
        TGroupBox *GroupBox5;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TEdit *edFrom;
        TComboBox *cbAT;
        TLabel *Label16;
        TCheckBox *cbAss;
        TGroupBox *GroupBox4;
        TLabel *Label17;
        TLabel *lbSk2;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label24;
        TLabel *Label25;
        TComboBox *cbWskil1;
        TComboBox *cbWskil2;
        TGroupBox *GroupBox6;
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
        TComboBox *cbWeapClass;
        TComboBox *cbAttType;
	TComboBox *cbAttPR;
        TGroupBox *GroupBox7;
        TStringGrid *grManSL;
        TSpeedButton *SpeedButton4;
        TSpeedButton *SpeedButton3;
        TGroupBox *GroupBox8;
        TGroupBox *GroupBox9;
        TGroupBox *GroupBox11;
        TTabSheet *tbMove;
        TGroupBox *GroupBox10;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label23;
        TLabel *Label26;
        TButton *Button1;
        TButton *Button2;
	TComboBox *cbSpellMP;
        TLabel *Label27;
        TLabel *Label28;
	TStringGrid *grInputMP;
        TSpeedButton *btAdd2;
        TSpeedButton *btDel2;
        TLabel *Label29;
	TComboBox *cbLevMP;
        TCheckBox *cbLeader;
        TLabel *Label30;
        TComboBox *edManDefL;
        TLabel *Label31;
        TComboBox *cbMountS;
        TLabel *Label32;
        TLabel *Label33;
        TLabel *Label34;
        TCheckBox *cbMouMonstr;
        TLabel *Label35;
        TLabel *Label36;
        TComboBox *cbMouSpec;
        TGroupBox *GroupBox12;
        TLabel *Label37;
        TLabel *Label38;
        TLabel *Label39;
        TLabel *Label40;
        TLabel *Label41;
        TLabel *Label42;
        TLabel *Label43;
        TLabel *Label44;
        TLabel *Label45;
        TLabel *Label46;
        TLabel *Label47;
        TLabel *Label48;
        TLabel *Label49;
        TLabel *Label50;
        TLabel *Label51;
        TPanel *PanelProd;
        TComboBox *cbHelpProd;
        TLabel *labHelpFrom;
        TLabel *labHelpProd;
        TLabel *labPer;
        TLabel *labManMonth;
        TLabel *labRate;
        TPanel *PanelInput;
        TLabel *labInput;
        TCheckBox *cbOrIn;
        TStringGrid *grInput;
        TSpeedButton *btAdd1;
        TSpeedButton *btDel1;
        TCheckBox *cbBattle;
        TComboBox *edMonAbil;
        TLabel *Label52;
	TCheckBox *cbAnyMP;
	TCSpinEdit *edHelpMult;
	TCSpinEdit *edMonths;
	TCSpinEdit *edOut;
	TCSpinEdit *edAd1;
	TCSpinEdit *edAd2;
	TCSpinEdit *edAd3;
	TCSpinEdit *edAd4;
	TCSpinEdit *edAd5;
	TCSpinEdit *edAd6;
	TCSpinEdit *edAd7;
	TCSpinEdit *edAd8;
	TCSpinEdit *edAttBonus;
	TCSpinEdit *edDefBonus;
	TCSpinEdit *edMountBonus;
	TCSpinEdit *edMonComb;
	TCSpinEdit *edMonTact;
	TCSpinEdit *edMonAtts;
	TCSpinEdit *edMonHits;
	TCSpinEdit *edMonObse;
	TCSpinEdit *edMonStea;
	TCSpinEdit *edMonRegen;
	TCSpinEdit *edMonD2;
	TCSpinEdit *edMonD1;
	TCSpinEdit *edMonD4;
	TCSpinEdit *edMonD3;
	TCSpinEdit *edMonD8;
	TCSpinEdit *edMonD7;
	TCSpinEdit *edMonD6;
	TCSpinEdit *edMonD5;
	TCSpinEdit *edMouHamB;
	TCSpinEdit *edMouMaxB;
	TCSpinEdit *edMouMinB;
	TCSpinEdit *CSpinEdit32;
	TCSpinEdit *edSwim;
	TCSpinEdit *edFly;
	TCSpinEdit *edRide;
	TCSpinEdit *edWalk;
	TCSpinEdit *edWeight;
	TCSpinEdit *edMonAbilLev;
	TCSpinEdit *edNumAtt;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall cbSkillChange(TObject *Sender);
	void __fastcall cbItTypeChange(TObject *Sender);
	void __fastcall btAdd1Click(TObject *Sender);
	void __fastcall btDel1Click(TObject *Sender);
	void __fastcall grInputDblClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall cbWskil1Change(TObject *Sender);
	void __fastcall SpeedButton3Click(TObject *Sender);
private:	// User declarations
    AnsiString FAbr;
	AItemType* FnewItem, *FoldItem;
	AMountType* FMount;
    AMonstrType* FMonster;
    AWeaponType* FWeapon;
    AArmorType* FArmor;
    AManType* FMan;
public:		// User declarations
        __fastcall TfEditItems(TComponent* Owner,AItemType*oldItem
    ,AItemType*newItem,AnsiString descr);
};
//---------------------------------------------------------------------------
extern PACKAGE TfEditItems *fEditItems;
//---------------------------------------------------------------------------
#endif
