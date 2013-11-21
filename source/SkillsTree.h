//---------------------------------------------------------------------------

#ifndef SkillsTreeH
#define SkillsTreeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "cspin.h"
#include <Grids.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfSkillsTree : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TTreeView *TreeS;
	TPanel *Panel3;
	TGroupBox *gbInf;
	TLabel *Label1;
	TLabel *lbAbr;
	TLabel *Label3;
	TComboBox *cbTypeS;
	TLabel *Label2;
	TCSpinEdit *edCost;
	TComboBox *cbSkill;
	TComboBox *cbSkill2;
	TComboBox *cbSkill3;
	TLabel *Label4;
	TLabel *lbAnd1;
	TLabel *lbAnd2;
	TCSpinEdit *edLev;
	TCSpinEdit *edLev2;
	TCSpinEdit *edLev3;
	TLabel *Label5;
	TStringGrid *grSpec;
	TCheckBox *cbCast;
	TCheckBox *cbComb;
	TLabel *Label6;
	TMemo *Memo1;
	TSpeedButton *btSave;
	TSpeedButton *btQ;
	TSpeedButton *btQall;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TreeSExpanding(TObject *Sender, TTreeNode *Node, bool &AllowExpansion);
	void __fastcall TreeSChanging(TObject *Sender, TTreeNode *Node, bool &AllowChange);
	void __fastcall TreeSChange(TObject *Sender, TTreeNode *Node);
	void __fastcall cbSkillChange(TObject *Sender);
	void __fastcall cbSkill2Change(TObject *Sender);
	void __fastcall cbSkill3Change(TObject *Sender);
	void __fastcall edLevChange(TObject *Sender);
	void __fastcall btSaveClick(TObject *Sender);
	void __fastcall btQClick(TObject *Sender);
	void __fastcall btQallClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
	TTreeNode *tnsF;
	TTreeNode *tnsM;
	TTreeNode *tnsS;
	bool FChanged;
	void RequeryInfo(ASkillType *si);
public:		// User declarations
	__fastcall TfSkillsTree(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfSkillsTree *fSkillsTree;
//---------------------------------------------------------------------------
#endif
