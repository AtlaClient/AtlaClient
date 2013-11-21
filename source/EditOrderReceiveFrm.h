//---------------------------------------------------------------------------
#ifndef EditOrderReceiveFrmH
#define EditOrderReceiveFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditOrderReceiveForm : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label1;
  TLabel *Label2;
  TButton *bnOk;
  TButton *bnCancel;
  TComboBox *cbTargets;
  TComboBox *cbItems;
  TEdit *edCount;
  TButton *bnAll;
  TButton *bnAdd;
    TCheckBox *cbRepeating;
  TLabel *Label3;
  TLabel *labCapacity;
	TLabel *lbNeed;
  void __fastcall cbItemsChange(TObject *Sender);
  void __fastcall cbTargetsChange(TObject *Sender);
  void __fastcall FormPaint(TObject *Sender);
  void __fastcall bnAllClick(TObject *Sender);
  void __fastcall bnOkClick(TObject *Sender);
  void __fastcall bnCancelClick(TObject *Sender);
private:	// User declarations
    TStringList *items;
    AUnits *targets;
    AUnit *curtarget;
    AnsiString curitem;
    void MakeTargetsList();
    void MakeItemsList();
    void Init();
    bool added;
public:		// User declarations
  __fastcall TEditOrderReceiveForm(TComponent* Owner);
    virtual __fastcall ~TEditOrderReceiveForm();
  void UpdateCapacity();
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderReceiveForm *EditOrderReceiveForm;
//---------------------------------------------------------------------------
#endif
