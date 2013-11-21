//---------------------------------------------------------------------------
#ifndef RegionListFrmH
#define RegionListFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <inifiles.hpp>
#include "util.h"
#include <Menus.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
struct PluginVariable;
class TRegionListForm : public TForm
{
__published:	// IDE-managed Components
  TListBox *List;
  TPopupMenu *PopupMenu;
  TMenuItem *miSaveList;
  TMenuItem *miLoadList;
  TOpenDialog *OpenDialog;
  TSaveDialog *SaveDialog;
  TMenuItem *N1;
  TMenuItem *miDelete;
  TMenuItem *miAddByExpression;
  TMenuItem *miAddCurrent;
  TMenuItem *N2;
  TMenuItem *miSaveMap;
  TMenuItem *miClearList;
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall miSaveListClick(TObject *Sender);
  void __fastcall miLoadListClick(TObject *Sender);
  void __fastcall miDeleteClick(TObject *Sender);
  void __fastcall miAddByExpressionClick(TObject *Sender);
  void __fastcall ListDblClick(TObject *Sender);
  void __fastcall miAddCurrentClick(TObject *Sender);
  void __fastcall miSaveMapClick(TObject *Sender);
  void __fastcall miClearListClick(TObject *Sender);
private:
  bool internal;
  int FindInsertIndex(int key);
  AnsiString expr;
  TStringList *list;
  int __fastcall GetRegionCount();
  ARegion* __fastcall GetRegions(int index);
public:		// User declarations
  __fastcall TRegionListForm(TComponent* Owner);
  __fastcall ~TRegionListForm();
  static int CoordsToInt(int x, int y, int z);
  static void IntToCoords(int key,int *x, int *y, int *z);
  bool InRegionList(int x, int y, int z);
  void AddRegion(ARegion * reg);
  void DeleteRegion(ARegion * reg);
  void UpdateListWidth();
  bool FillList(PluginVariable *list);
  __property int RegionCount  = { read=GetRegionCount };
  __property ARegion* Regions[int index]  = { read=GetRegions };
};
//---------------------------------------------------------------------------
extern PACKAGE TRegionListForm *RegionListForm;
//---------------------------------------------------------------------------
#endif



