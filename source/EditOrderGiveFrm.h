//---------------------------------------------------------------------------
#ifndef EditOrderGiveFrmH
#define EditOrderGiveFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderGiveForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TCheckBox *cbCommented;
    TCheckBox *cbRepeating;
    TComboBox *cbTargets;
    TLabel *Label1;
    TComboBox *cbItems;
    TEdit *edCount;
    TLabel *Label2;
    TButton *bnAll;
  TCheckBox *cbDiscard;
    TCheckBox *cbAll;
    TCheckBox *cbExcept;
  TComboBox *cbFacDiap;
  TLabel *Label3;
        TLabel *labCapacity;
        TLabel *labCapacity2;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall edCountExit(TObject *Sender);
    void __fastcall cbItemsChange(TObject *Sender);
    void __fastcall bnAllClick(TObject *Sender);
        void __fastcall cbTargetsChange(TObject *Sender);
    void __fastcall cbAllClick(TObject *Sender);
    void __fastcall cbExceptClick(TObject *Sender);
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
    void ShowCapacity();

public:		// User declarations
    __fastcall TEditOrderGiveForm(TComponent* Owner, AOrderGive *_ord, AItems *its, AUnits *tars);
    virtual __fastcall ~TEditOrderGiveForm();
    AOrderGive *ord;
    void MakeTargetList();
    bool CalcValues();
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderGiveForm *EditOrderGiveForm;
//---------------------------------------------------------------------------
#endif
