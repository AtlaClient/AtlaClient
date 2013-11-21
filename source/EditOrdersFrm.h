//---------------------------------------------------------------------------

#ifndef EditOrdersFrmH
#define EditOrdersFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
//---------------------------------------------------------------------------
class TEditOrdersForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TButton *bnOk;
    TButton *bnCancel;
    TMemo *Editor;
  TActionList *ActionList1;
  TAction *OkAction;
    void __fastcall FormShow(TObject *Sender);
  void __fastcall OkActionExecute(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
    __fastcall TEditOrdersForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
