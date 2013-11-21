//---------------------------------------------------------------------------
#ifndef ViewChangesFrmH
#define ViewChangesFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TViewChangesForm : public TForm
{
__published:	// IDE-managed Components
  TStringGrid *sgAdded;
  TStringGrid *sgDeleted;
  TStringGrid *sgChanged;
  TSplitter *splitHorz;
  TSplitter *splitVert;
  TMainMenu *MainMenu1;
  TMenuItem *miChanges;
  TMenuItem *miFaction;
  TMenuItem *miItems;
  TMenuItem *miLocation;
  TMenuItem *miAdvanced;
  TMenuItem *miAdded;
  TPanel *Panel1;
  TLabel *Label1;
  TLabel *Label2;
  TComboBox *cbRegDiap;
  TComboBox *cbFacDiap;
  TButton *bnSetBaseReg;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall sgAddedDblClick(TObject *Sender);
  void __fastcall sgDeletedDblClick(TObject *Sender);
  void __fastcall sgChangedDblClick(TObject *Sender);
  void __fastcall miFactionClick(TObject *Sender);
  void __fastcall miItemsClick(TObject *Sender);
  void __fastcall miLocationClick(TObject *Sender);
  void __fastcall miAddedClick(TObject *Sender);
  void __fastcall cbRegDiapChange(TObject *Sender);
  void __fastcall bnSetBaseRegClick(TObject *Sender);
  void __fastcall cbFacDiapChange(TObject *Sender);
  void __fastcall cbFacDiapEnter(TObject *Sender);
  void __fastcall cbFacDiapKeyPress(TObject *Sender, char &Key);
private:	// User declarations
  int mode;  //для Changed
  vector<TChanges::TElem*> addeds,deleteds,changeds;
  ARegion *baseReg;
  int facnum_search;
  
public:		// User declarations
  int facdiap; //-2 - all, -3 - local, -4 - nonlocal, -5 - friendly, -6 -unfriendly
  
  __fastcall TViewChangesForm(TComponent* Owner);
  void ShowAll();
  void ShowAdded();
  void ShowDeleted();
  void ShowChanged();
  AnsiString PrintUnit(AUnit * un);
  bool PrintUnits(TChanges::TElem &elem, AnsiString &basestr, AnsiString & changesstr);
  void UpdateMode();
  void FillRegionLocs(vector<unsigned> &locs);
  void MakeFacList();
};
//---------------------------------------------------------------------------
extern PACKAGE TViewChangesForm *ViewChangesForm;
//---------------------------------------------------------------------------
#endif
