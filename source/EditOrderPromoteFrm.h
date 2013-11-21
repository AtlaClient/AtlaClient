//---------------------------------------------------------------------------
#ifndef EditOrderPromoteFrmH
#define EditOrderPromoteFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderPromoteForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TCheckBox *cbCommented;
    TCheckBox *cbRepeating;
    TComboBox *cbTargets;
    TLabel *Label2;
  TComboBox *cbFacDiap;
  TLabel *Label3;
  TLabel *labError;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
        void __fastcall cbTargetsChange(TObject *Sender);
  void __fastcall cbFacDiapChange(TObject *Sender);
  void __fastcall cbFacDiapEnter(TObject *Sender);
  void __fastcall cbFacDiapKeyPress(TObject *Sender, char &Key);
private:	// User declarations
    AUnits *targets;
    int curtarnum;
    AnsiString curitem;
    TStringList *slist;
    int facdiap;
    int facnum_search;

public:		// User declarations
    __fastcall TEditOrderPromoteForm(TComponent* Owner, AOrderInt *_ord, AUnits *tars);
    virtual __fastcall ~TEditOrderPromoteForm();
    AOrderInt *ord;
    void MakeTargetList();
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderPromoteForm *EditOrderPromoteForm;
//---------------------------------------------------------------------------
#endif
