//---------------------------------------------------------------------------
#ifndef EditOrderEnterFrmH
#define EditOrderEnterFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TEditOrderEnterForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TCheckBox *cbCommented;
    TCheckBox *cbRepeating;
	TStringGrid *Grid;
	TLabel *labUnit;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
	void __fastcall GridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
private:	// User declarations
		int blockedrow;
public:		// User declarations
    __fastcall TEditOrderEnterForm(TComponent* Owner, AOrderInt *_ord);
    AOrderInt *ord;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderEnterForm *EditOrderEnterForm;
//---------------------------------------------------------------------------
#endif
