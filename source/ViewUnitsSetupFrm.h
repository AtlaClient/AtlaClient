//---------------------------------------------------------------------------
#ifndef ViewUnitsSetupFrmH
#define ViewUnitsSetupFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TViewUnitsSetupForm : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label1;
  TListBox *lbColumns;
  TButton *bnOk;
  TButton *bnCancel;
  TListBox *lbItems;
  TLabel *Label2;
  TListBox *lbAvailItems;
  TButton *bnAddItem;
  TButton *bnDelItem;
  TButton *bnAddColumn;
  TButton *bnDelColumn;
  TButton *bnMoveUp;
  TButton *bnMoveDown;
  TButton *bnClearItems;
  TListBox *lbStoredColumns;
  TListBox *lbStoredSets;
  TLabel *Label4;
  TLabel *Label5;
  TButton *bnColsSave;
  TButton *bnColsLoad;
  TButton *bnColsRename;
  TButton *bnColsDelete;
  TButton *bnSetsSave;
  TButton *bnSetsLoad;
  TButton *bnSetsDelete;
  TButton *bnSetsRename;
  TTabControl *tabItems;
  void __fastcall bnOkClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall lbColumnsClick(TObject *Sender);
  void __fastcall bnAddItemClick(TObject *Sender);
  void __fastcall bnDelItemClick(TObject *Sender);
  void __fastcall bnAddColumnClick(TObject *Sender);
  void __fastcall bnDelColumnClick(TObject *Sender);
  void __fastcall lbColumnsDblClick(TObject *Sender);
  void __fastcall bnMoveUpClick(TObject *Sender);
  void __fastcall bnMoveDownClick(TObject *Sender);
  void __fastcall bnClearItemsClick(TObject *Sender);
  void __fastcall bnColsSaveClick(TObject *Sender);
  void __fastcall bnColsLoadClick(TObject *Sender);
  void __fastcall bnColsRenameClick(TObject *Sender);
  void __fastcall bnColsDeleteClick(TObject *Sender);
  void __fastcall bnSetsSaveClick(TObject *Sender);
  void __fastcall bnSetsLoadClick(TObject *Sender);
  void __fastcall bnSetsRenameClick(TObject *Sender);
  void __fastcall bnSetsDeleteClick(TObject *Sender);
  void __fastcall tabItemsChange(TObject *Sender);
  void __fastcall tabItemsChanging(TObject *Sender, bool &AllowChange);
private:	// User declarations
  VUColumnHeader *curcolumn;
public:		// User declarations
  __fastcall TViewUnitsSetupForm(TComponent* Owner);
  void MakeColumnList(int newind=-1);
  void MakeItemList();
  void MakeAvalItemList();
  void MakeStoredColumns();
  void MakeStoredSets();
};
//---------------------------------------------------------------------------
//extern PACKAGE TViewUnitsSetupForm *ViewUnitsSetupForm;
//---------------------------------------------------------------------------
#endif
