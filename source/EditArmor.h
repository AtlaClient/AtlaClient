//---------------------------------------------------------------------------

#ifndef EditArmorH
#define EditArmorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "util.h"
//---------------------------------------------------------------------------
class TfEditArmor : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo;
        TLabel *Label1;
        TComboBox *cbAT;
        TCheckBox *cbAss;
        TGroupBox *GroupBox1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TEdit *ed1;
        TEdit *ed2;
        TEdit *ed4;
        TEdit *ed3;
        TEdit *ed6;
        TEdit *ed5;
        TEdit *ed8;
        TEdit *ed7;
        TButton *bnCancel;
        TButton *bnOk;
        TLabel *Label10;
        TEdit *edFrom;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall bnOkClick(TObject *Sender);
private:	// User declarations
        AArmorType *Farm;
        bool bNewArm;
public:		// User declarations
        AnsiString TekItem;
        __fastcall TfEditArmor(TComponent* Owner,
                   AArmorType *arm, AnsiString descr);
};
//---------------------------------------------------------------------------
extern PACKAGE TfEditArmor *fEditArmor;
//---------------------------------------------------------------------------
#endif
