//---------------------------------------------------------------------------

#ifndef ViewTaxRegionsFrmH
#define ViewTaxRegionsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "util.h"
//---------------------------------------------------------------------------
class TViewTaxRegionsForm : public TForm
{
__published:	// IDE-managed Components
    TDrawGrid *Grid;
    void __fastcall GridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
    void __fastcall GridDblClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TViewTaxRegionsForm(TComponent* Owner);
    __fastcall ~TViewTaxRegionsForm();
    void Init();
    void MakeGrid();
};
//---------------------------------------------------------------------------
//extern PACKAGE TViewTaxRegionsForm *ViewTaxRegionsForm;
//---------------------------------------------------------------------------
#endif
