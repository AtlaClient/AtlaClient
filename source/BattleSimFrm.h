//---------------------------------------------------------------------------

#ifndef BattleSimFrmH
#define BattleSimFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include "EAPCustomMemo.h"
//---------------------------------------------------------------------------
class IntList:TList{
public:
     IntList():TList(){}
     TList::Clear;
     void Add(int val){TList::Add((TObject*)val);};
     TList::Delete;
     int IndexOf(int val){return TList::IndexOf((TObject*)val);}

    __property Count;
    __property int Items[int ind] = { read=GetItems, write=SetItems };
private:
    void __fastcall SetItems(int ind, int value){TList::Items[ind]=(TObject*)value;}
    int __fastcall GetItems(int ind){return (int)TList::Items[ind];}
};

class TBattleSimForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TListBox *lbAtts;
    TOpenDialog *OpenDialog;
    TSaveDialog *SaveDialog;
	TPanel *Panel2;
	TComboBox *cbRegType;
	TLabel *labMountType;
	TButton *bnRun;
	TButton *bnAddAtt;
	TButton *bnDelete;
	TButton *bnClearAtt;
	TButton *bnSwap;
	TButton *bnSwapAll;
	TButton *bnAddDef;
	TButton *bnSave;
	TButton *bnLoad;
	TPanel *Panel3;
	TSplitter *Splitter1;
	TPanel *Panel4;
	TLabel *labCurUnit;
	TLabel *labAnalRes;
	TLabel *Label1;
	TLabel *Label2;
	TListBox *lbItems;
	TListBox *lbSkills;
	TComboBox *cbItems;
	TComboBox *cbSkills;
	TEdit *edItem;
	TEdit *edSkill;
	TButton *bnSetItem;
	TButton *bnSetSkill;
	TCheckBox *cbBehind;
	TButton *bnView;
	TButton *bnAnalyze;
	TComboBox *cbCombatSpell;
	TButton *bnSoldiers;
	TCheckBox *cbShortMode;
	TEdit *edMaxRounds;
	TListBox *lbDefs;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall cbRegTypeChange(TObject *Sender);
    void __fastcall bnRunClick(TObject *Sender);
    void __fastcall lbAttsDefsClick(TObject *Sender);
    void __fastcall bnSetItemClick(TObject *Sender);
    void __fastcall bnSetSkillClick(TObject *Sender);
    void __fastcall lbItemsClick(TObject *Sender);
    void __fastcall lbSkillsClick(TObject *Sender);
    void __fastcall bnAddAttClick(TObject *Sender);
    void __fastcall bnAddDefClick(TObject *Sender);
    void __fastcall bnDeleteClick(TObject *Sender);
    void __fastcall bnSwapClick(TObject *Sender);
    void __fastcall bnSwapAllClick(TObject *Sender);
    void __fastcall cbBehindClick(TObject *Sender);
    void __fastcall bnSaveClick(TObject *Sender);
    void __fastcall bnViewClick(TObject *Sender);
    void __fastcall bnAnalyzeClick(TObject *Sender);
    void __fastcall bnLoadClick(TObject *Sender);
    void __fastcall cbCombatSpellClick(TObject *Sender);
    void __fastcall bnClearAttClick(TObject *Sender);
  void __fastcall bnSoldiersClick(TObject *Sender);
	void __fastcall cbShortModeClick(TObject *Sender);
private:	// User declarations
		ABattleReport *rep;
		TEAPCustomMemo *Reports;
//    ARegion *reg;
//    AUnits *atts,*defs;
		int lastunnum;
		AUnit *curUnit;
		bool Modified;
		bool FInAnalyze;
		void __fastcall SetInAnalyze(bool value);
		void __fastcall ReportMemoEvents(TObject* Sender, Classes::TShiftState Shift
			, TEAPKeyEvent KeyEvent, Word Key, const TPoint &CaretPos, const TPoint &MousePos);
public:		// User declarations
		__fastcall TBattleSimForm(TComponent* Owner);
		__fastcall ~TBattleSimForm();
		void UpdateComboBoxs();
    void AddAttacker(int index);
    void AddDefender(int index);
    void AddAttacker(AUnit *un);
    void AddDefender(AUnit *un);
    void UpdateList(TListBox * lb, AUnits * uns);
    int PresentUnit(int num);//0-not,1-att,2-def
    int GetNewUnitNum();
    int ModifyNewUnitNum(int num);
    bool LoadFromList(TStringList * list, int ind);
    void AddArmy(AUnit *un,bool attacks);
    void AddAttackers(AUnit *un){AddArmy(un,true);}
    void AddDefenders(AUnit *un){AddArmy(un,false);}

    __property bool InAnalyze  = { read=FInAnalyze, write=SetInAnalyze };
protected:
    void Draw();
    void DrawCurUnit2();
    void DrawCurUnit();
    void DrawCurItem();
    void DrawCurSkill();
    void DrawCurCombat();
};
//---------------------------------------------------------------------------
extern PACKAGE TBattleSimForm *BattleSimForm;
//---------------------------------------------------------------------------
#endif
