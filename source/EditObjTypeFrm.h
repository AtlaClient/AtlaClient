//---------------------------------------------------------------------------

#ifndef EditObjTypeFrmH
#define EditObjTypeFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "util.h"
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TEditObjTypeForm : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo;
        TLabel *Label1;
        TEdit *edName;
        TLabel *Label5;
        TComboBox *cbSkill;
        TComboBox *cbLevel;
        TLabel *labInput;
        TComboBox *cbInput;
        TLabel *labNumber;
        TEdit *edNumber;
        TLabel *labHelpProd;
        TEdit *edHelpMult;
        TLabel *labHelpFrom;
        TComboBox *cbHelpProd;
        TLabel *Label2;
        TLabel *Label3;
        TCheckBox *cbEnter;
        TLabel *lbSailors;
        TCheckBox *cbShip;
        TLabel *Label6;
        TMaskEdit *edProtect;
        TMaskEdit *edCapacity;
        TMaskEdit *edSailors;
        TComboBox *cbRoad;
        TButton *bnOk;
        TButton *bnCancel;
        TLabel *labLevel;
        TComboBox *cbIncProd;
        TLabel *Label7;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall cbSkillChange(TObject *Sender);
        void __fastcall cbInputChange(TObject *Sender);
        void __fastcall cbShipClick(TObject *Sender);
        void __fastcall bnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TEditObjTypeForm::TEditObjTypeForm(TComponent* Owner, AObjectType *oldObj,
        AnsiString descr);
    virtual __fastcall ~TEditObjTypeForm();
protected:
    AObjectType* FoldObj;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditObjTypeForm *EditObjTypeForm;
//---------------------------------------------------------------------------
#endif
