//---------------------------------------------------------------------------
#ifndef EditSkillTypeFrmH
#define EditSkillTypeFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TEditSkillTypeForm : public TForm
{
__published:	// IDE-managed Components
    TStringGrid *Table;
    TLabel *Label1;
    TEdit *edName;
    TLabel *Label2;
    TEdit *edAbr;
    TLabel *Label3;
    TEdit *edCost;
    TCheckBox *cbMagic;
    TCheckBox *cbCombat;
    TCheckBox *cbCast;
    TCheckBox *cbFoundation;
    TLabel *Label5;
    TComboBox *cbDep1;
    TLabel *Label6;
    TComboBox *cbDep2;
    TComboBox *cbDep3;
    TButton *bnOk;
    TButton *bnCancel;
    TComboBox *cbLev1;
    TComboBox *cbLev2;
    TComboBox *cbLev3;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall cbDep1Change(TObject *Sender);
    void __fastcall cbDep2Change(TObject *Sender);
    void __fastcall cbDep3Change(TObject *Sender);
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall TableDblClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
        //old skill do not deleted
    __fastcall TEditSkillTypeForm(TComponent* Owner,ASkillType*oldskill,ASkillType*newskill=0);
    virtual __fastcall ~TEditSkillTypeForm();
protected:
    ASkillType* FnewSkill, *FoldSkill;
    TStringList*list;
    void PrepareName(ASkillType* stype);
    void PrepareAbr(ASkillType* stype);
    void PrepareCost(ASkillType* stype);
    void PrepareMagic(ASkillType* stype);
    void PrepareCombat(ASkillType* stype);
    void PrepareCast(ASkillType* stype);
    void PrepareFoundation(ASkillType* stype);
    void PrepareDep1(ASkillType* stype);
    void PrepareDep2(ASkillType* stype);
    void PrepareDep3(ASkillType* stype);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditSkillTypeForm *EditSkillTypeForm;
//---------------------------------------------------------------------------
#endif
