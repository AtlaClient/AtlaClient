//---------------------------------------------------------------------------
#ifndef EditOrderTeachFrmH
#define EditOrderTeachFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <vector.h>
//---------------------------------------------------------------------------
class TEditOrderTeachForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TCheckBox *cbCommented;
    TCheckBox *cbRepeating;
    TListBox *lbTargets;
    TListBox *lbUnits;
    TButton *bnDelete;
    TButton *bnAdd;
    TLabel *Label1;
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall bnAddClick(TObject *Sender);
    void __fastcall bnDeleteClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    struct TUnitInfo{
      AUnit *un;
      AnsiString str;
      int sortord;
      bool operator<(const TUnitInfo&b) const {return sortord<b.sortord;}
    };

    __fastcall TEditOrderTeachForm(TComponent* Owner, AOrderTeach *_ord, AUnits *_units);
    AOrderTeach *ord;
    AUnits *targets;
    vector<TUnitInfo> units;
    int maxstudents;
    void MakeLists();
    AnsiString PrintUnit(AUnit * un);
    int TEditOrderTeachForm::GetSortOrd(AUnit * un);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderTeachForm *EditOrderTeachForm;
//---------------------------------------------------------------------------
#endif
