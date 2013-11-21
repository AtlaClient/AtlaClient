//---------------------------------------------------------------------------

#ifndef ShowObjTypesFrmH
#define ShowObjTypesFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TShowObjTypesForm : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TButton *bnEdit;
        TButton *bnCancel;
        TButton *bnRequery;
        TListBox *List;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall bnCancelClick(TObject *Sender);
        void __fastcall bnRequeryClick(TObject *Sender);
        void __fastcall bnEditClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TShowObjTypesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TShowObjTypesForm *ShowObjTypesForm;
//---------------------------------------------------------------------------
#endif
