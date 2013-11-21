//---------------------------------------------------------------------------
#ifndef EditOrderAttackFrmH
#define EditOrderAttackFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderAttackForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TCheckBox *cbCommented;
    TCheckBox *cbRepeating;
  TComboBox *cbCanTargets;
    TLabel *Label2;
  TComboBox *cbFacDiap;
  TLabel *Label3;
  TListBox *lbTargets;
  TButton *bnAdd;
  TButton *bnDelete;
  TButton *bnAddAll;
    void __fastcall bnAddClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
        void __fastcall cbCanTargetsChange(TObject *Sender);
  void __fastcall cbFacDiapChange(TObject *Sender);
  void __fastcall cbFacDiapEnter(TObject *Sender);
  void __fastcall cbFacDiapKeyPress(TObject *Sender, char &Key);
  void __fastcall bnOkClick(TObject *Sender);
  void __fastcall bnDeleteClick(TObject *Sender);
  void __fastcall bnAddAllClick(TObject *Sender);
private:	// User declarations
    AUnits *cantargets;
    TList *targets;
    int curtarnum;
    AnsiString curitem;
    TStringList *slist;
    int facdiap;
    int facnum_search;

public:		// User declarations
    __fastcall TEditOrderAttackForm(TComponent* Owner, AOrderAttack *_ord, AUnits *tars);
    virtual __fastcall ~TEditOrderAttackForm();
    AOrderAttack *ord;
    void MakeCanTargetList();
    void MakeTargetList();
    AnsiString PrintUnit(AUnit * un);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderAttackForm *EditOrderAttackForm;
//---------------------------------------------------------------------------
#endif
