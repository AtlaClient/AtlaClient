//---------------------------------------------------------------------------
#ifndef EditOrderBuildH
#define EditOrderBuildH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderBuildForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TCheckBox *cbRepeating;
    TCheckBox *cbCommented;
    TComboBox *cbBuilding;
    TLabel *labProd;
    TLabel *labCost;
    TLabel *labHave;
    TLabel *labCap;
    TLabel *labSail;
    TLabel *labProt;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall cbBuildingChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TEditOrderBuildForm(TComponent* Owner, AOrderBuild *_ord);
    AOrderBuild *ord;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderBuildForm *EditOrderBuildForm;
//---------------------------------------------------------------------------
#endif
