//---------------------------------------------------------------------------
#ifndef ProductsFrmH
#define ProductsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TProductsForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *PanTop;
  TStringGrid *Grid;
  TLabel *Label1;
  TComboBox *cbItems;
  TLabel *labTools;
  TComboBox *cbLevel;
  TLabel *Label2;
  TButton *bnToRegionList;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall cbItemsChange(TObject *Sender);
  void __fastcall GridDblClick(TObject *Sender);
  void __fastcall cbLevelChange(TObject *Sender);
  void __fastcall bnToRegionListClick(TObject *Sender);
private:	// User declarations
  static AnsiString Item;
  static int DefLevel;
  AItemType *IType;
  AItemType *ToolType;
  bool HasBonusObject;
  
public:		// User declarations
  __fastcall TProductsForm(TComponent* Owner);
  void CollectInRegion(ARegion*reg);
  void CollectData();
  void MakeTable();
  void MakeItemsList();
  void UpdateSumm();
};
//---------------------------------------------------------------------------
//extern PACKAGE TProductsForm *ProductsForm;
//---------------------------------------------------------------------------
#endif
