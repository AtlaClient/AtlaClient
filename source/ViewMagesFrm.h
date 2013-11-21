//---------------------------------------------------------------------------

#ifndef ViewMagesFrmH
#define ViewMagesFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <vector.h>
#include "util.h"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class MageInfo;
typedef vector<MageInfo> MageInfos;
class TViewMagesForm : public TForm
{
friend class MageInfo;
__published:	// IDE-managed Components
    TDrawGrid *Grid;
  TPopupMenu *PM;
  TMenuItem *miSetColWidth;
  TMenuItem *miLocalOnly;
  TMenuItem *miCurRegOnly;
  TMainMenu *MainMenu1;
  TMenuItem *miPrevTurn;
  TMenuItem *miNextTurn;
  TMenuItem *miEvents;
  TMenuItem *miRemoveMonthOrder;
    void __fastcall GridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall GridDblClick(TObject *Sender);
  void __fastcall miSetColWidthClick(TObject *Sender);
  void __fastcall miLocalOnlyClick(TObject *Sender);
  void __fastcall miCurRegOnlyClick(TObject *Sender);
  void __fastcall miPrevTurnClick(TObject *Sender);
  void __fastcall miNextTurnClick(TObject *Sender);
  void __fastcall miEventsClick(TObject *Sender);
  void __fastcall GridMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall miRemoveMonthOrderClick(TObject *Sender);
private:	// User declarations
    MageInfos *list;
    static TStringList *found,*magic;
    TList *regs;
    static int FSortRow;
    TStringList *headers;
    void __fastcall SetSortRow(int value);
public:		// User declarations
    static int FSortDir;
    static int ColsWidth;
    static bool LocalOnly;
    static bool CurRegOnly;

    __fastcall TViewMagesForm(TComponent* Owner);
    void Init();
    void MakeGrid();
    void RunOrders();
    void EditOrder(AOrders * ords, AOrder * ord, bool neworder);
    __property int SortRow  = { read=FSortRow, write=SetSortRow };
};
//---------------------------------------------------------------------------
//extern PACKAGE TViewMagesForm *ViewMagesForm;
//---------------------------------------------------------------------------
#endif
