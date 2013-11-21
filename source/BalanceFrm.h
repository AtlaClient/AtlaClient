//---------------------------------------------------------------------------
#ifndef BalanceFrmH
#define BalanceFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TBalanceForm : public TForm
{
__published:	// IDE-managed Components
  TDrawGrid *Grid;
  TToolBar *ToolBar;
  TToolButton *bnPrevTurn;
  TCheckBox *cbCurRegionOnly;
  TLabel *Label2;
  TComboBox *cbFacDiap;
  TToolButton *bnNextTurn;
  TToolButton *ToolButton3;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall GridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
  void __fastcall cbCurRegionOnlyClick(TObject *Sender);
  void __fastcall cbFacDiapChange(TObject *Sender);
  void __fastcall bnPrevTurnClick(TObject *Sender);
  void __fastcall bnNextTurnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  static bool curregonly;
  static bool logging;
  static int facdiap;

  __fastcall TBalanceForm(TComponent* Owner);
  void UpdateGridWidths(bool withnextturn);
  void CollectUnit(AUnit * un);
  void CollectInRegion(ARegion * reg);
  void CollectNextUnit(AUnit * un);
  void CollectNextInRegion(ARegion * reg);
  void CollectData();
  void MakeTable();
  void MakeFacList();
};
//---------------------------------------------------------------------------
//extern PACKAGE TBalanceForm *BalanceForm;
//---------------------------------------------------------------------------
#endif

