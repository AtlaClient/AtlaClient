//---------------------------------------------------------------------------
#ifndef ViewUnitsFrmH
#define ViewUnitsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <vector.h>
#include "util.h"
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
extern AnsiString *ItemGroups;
extern int ItemGroupCount;
class VUColumnHeader{
public:
  TStringList *abrs;
  AnsiString name;
  int width;
  bool skills;

  VUColumnHeader();
  VUColumnHeader(const VUColumnHeader& b);
  ~VUColumnHeader();
  void operator=(const VUColumnHeader& b);

  static int GetFlags(AnsiString itemgroup);
  void Add(AnsiString abr);
  void Delete(AnsiString abr);
  void Clear();
  int CalcCount(AUnit *un, AnsiString &str);
  void operator=(AnsiString str);
  operator AnsiString();
};
class VUColumnHeaders{
public:
  vector<VUColumnHeader> headers;
  AnsiString name;

  VUColumnHeaders(){};
  void operator=(AnsiString str);
  operator AnsiString();
};
class UnitInfo;
typedef vector<UnitInfo> UnitInfos;
const int InfoCols=5;
class TViewUnitsForm : public TForm
{
friend class UnitInfo;
__published:	// IDE-managed Components
  TToolBar *ToolBar1;
  TStringGrid *Grid;
  TLabel *Label1;
  TComboBox *cbRegDiap;
  TLabel *labUnits;
  TLabel *Label2;
  TComboBox *cbFacDiap;
  TToolButton *ToolButton2;
  TToolButton *ToolButton3;
  TToolButton *tbSetup;
  TCheckBox *cbHideObjects;
  TComboBox *cbColumnSets;
  TToolButton *ToolButton1;
  TCheckBox *cbExpression;
  TEdit *edExpr;
  TButton *bnEditExpr;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall GridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
  void __fastcall GridDblClick(TObject *Sender);
  void __fastcall cbRegDiapChange(TObject *Sender);
  void __fastcall cbFacDiapChange(TObject *Sender);
  void __fastcall bnSetupClick(TObject *Sender);
  void __fastcall cbHideObjectsClick(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall GridMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
  void __fastcall cbFacDiapEnter(TObject *Sender);
  void __fastcall cbFacDiapKeyPress(TObject *Sender, char &Key);
  void __fastcall cbColumnSetsClick(TObject *Sender);
  void __fastcall cbExpressionClick(TObject *Sender);
  void __fastcall edExprExit(TObject *Sender);
  void __fastcall edExprKeyPress(TObject *Sender, char &Key);
  void __fastcall bnEditExprClick(TObject *Sender);
        void __fastcall GridMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
  UnitInfos *units;
  static int FSortCol;
  TStringList *headers;
  vector<int> counts;
  int facnum_search;

  void __fastcall SetSortCol(int value);
public:		// User declarations
  static VUColumnHeaders Headers;
  static int FSortDir;
  enum TRegDiap{
    rdtCurrent=0,
    rdtNearest,
    rdtAll,
    rdtRegionList,
  };
  static TRegDiap regdiap;
  static int facdiap; //-2 - all, -3 - local, -4 - nonlocal, -5 - friendly, -6 -unfriendly
  static int height;
  static int ColWidths[InfoCols];
  static int InfoRows;

  __fastcall TViewUnitsForm(TComponent* Owner);
  virtual __fastcall ~TViewUnitsForm();
  void CollectUnitsInRegion(ARegion *reg, vector<int>* summcounts);
  void ReMake(bool newtable=false);
  void CollectUnits();
  void SortUnits();
  void MakeGrid();
  void MakeFacList();
  static AnsiString GetColWidthsStr();
  static void SetColWidthsStr(AnsiString str);
  void SaveColWidths();
  void UpdateUnit(AUnit * un);
  void MakeColumnSetList();
  __property int SortCol  = { read=FSortCol, write=SetSortCol };
};
//---------------------------------------------------------------------------
extern PACKAGE TViewUnitsForm *ViewUnitsForm;
//---------------------------------------------------------------------------
#endif
