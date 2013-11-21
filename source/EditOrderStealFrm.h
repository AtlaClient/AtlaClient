//---------------------------------------------------------------------------
#ifndef EditOrderStealFrmH
#define EditOrderStealFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderStealForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TCheckBox *cbCommented;
    TCheckBox *cbRepeating;
    TComboBox *cbTargets;
    TLabel *Label1;
    TComboBox *cbItems;
    TLabel *Label2;
  TComboBox *cbFacDiap;
  TLabel *Label3;
	TLabel *lbAll;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall cbItemsChange(TObject *Sender);
        void __fastcall cbTargetsChange(TObject *Sender);
  void __fastcall cbFacDiapChange(TObject *Sender);
  void __fastcall cbFacDiapEnter(TObject *Sender);
  void __fastcall cbFacDiapKeyPress(TObject *Sender, char &Key);
private:	// User declarations
    AItems *items;
    AUnits *targets;
    int curtarnum;
    AnsiString curitem;
    TStringList *slist;
    int facdiap;
    int facnum_search;

public:		// User declarations
    __fastcall TEditOrderStealForm(TComponent* Owner, AOrderSteal *_ord, AItems *its, AUnits *tars);
    virtual __fastcall ~TEditOrderStealForm();
    AOrderSteal *ord;
    void MakeTargetList();
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderStealForm *EditOrderStealForm;
//---------------------------------------------------------------------------
#endif
