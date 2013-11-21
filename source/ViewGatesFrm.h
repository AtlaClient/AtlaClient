//---------------------------------------------------------------------------
#ifndef ViewGatesFrmH
#define ViewGatesFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TViewGatesForm : public TForm
{
__published:	// IDE-managed Components
  TStringGrid *Grid;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall GridDblClick(TObject *Sender);
private:	// User declarations
  TList* locs;
public:		// User declarations
  __fastcall TViewGatesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TViewGatesForm *ViewGatesForm;
//---------------------------------------------------------------------------
#endif
