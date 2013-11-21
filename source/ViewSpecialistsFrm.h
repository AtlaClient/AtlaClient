//---------------------------------------------------------------------------
#ifndef ViewSpecialistsFrmH
#define ViewSpecialistsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <vector.h>
#include "util.h"
#include "unit.h"
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TSkLev{
public:
  ASkillType *stype;
  int level;
  AUnits *units;
  int lead,man;

  TSkLev();
  TSkLev(const TSkLev&b);
  void operator =(const TSkLev&b);
  ~TSkLev();
  void AssignUnits(AUnits *uns);
  void UpdateMensCount();
  static AnsiString MakeMensStr(int lead, int man);
  AnsiString MakeMensStr(){return MakeMensStr(lead,man);}

};
class TViewSpecialistsForm : public TForm
{
__published:	// IDE-managed Components
  TTreeView *Tree;
  TSplitter *Splitter;
  TToolBar *ToolBar1;
  TToolButton *bnAllRegion;
  TToolButton *bnNoMagic;
  TToolButton *ToolButton1;
  TListBox *List;
  TToolButton *ToolButton2;
  TToolButton *bnShortMode;
  TToolButton *ToolButton3;
  TToolButton *bnSortMode;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall TreeChange(TObject *Sender, TTreeNode *Node);
  void __fastcall ListDblClick(TObject *Sender);
  void __fastcall bnAllRegionClick(TObject *Sender);
  void __fastcall bnNoMagicClick(TObject *Sender);
  void __fastcall bnShortModeClick(TObject *Sender);
  void __fastcall bnSortModeClick(TObject *Sender);
private:	// User declarations
  vector<TSkLev> sklevs;
  static bool NoMagic,AllReg;
  static bool ShortMode;
  static bool SortMode;

  TSkLev* GetSkLev(ASkillType* st, int lev);
public:		// User declarations
  __fastcall TViewSpecialistsForm(TComponent* Owner);
  void ProcessRegion(ARegion * reg);
  void MakeTree();
};
//---------------------------------------------------------------------------
//extern PACKAGE TViewSpecialistsForm *ViewSpecialistsForm;
//---------------------------------------------------------------------------
#endif
