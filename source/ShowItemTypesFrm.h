//---------------------------------------------------------------------------
#ifndef ShowItemTypesFrmH
#define ShowItemTypesFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TShowItemTypesForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TButton *bnEdit;
    TListBox *List;
    TButton *bnCancel;
    TButton *bnRequery;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall bnEditClick(TObject *Sender);
    void __fastcall bnRequeryClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall bnCancelClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TShowItemTypesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TShowItemTypesForm *ShowItemTypesForm;
//---------------------------------------------------------------------------
#endif
