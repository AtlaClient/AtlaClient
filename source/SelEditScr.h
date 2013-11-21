//---------------------------------------------------------------------------

#ifndef SelEditScrH
#define SelEditScrH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <inifiles.hpp>
//---------------------------------------------------------------------------
class TfSelEditScr : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TListBox *List;
	TButton *bnEdit;
	TButton *Button1;
	TButton *btDel;
	void __fastcall btDelClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall bnEditClick(TObject *Sender);
private:	// User declarations
    void SetEnabledBt();
public:		// User declarations
    bool Changed;
	__fastcall TfSelEditScr(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfSelEditScr *fSelEditScr;
//---------------------------------------------------------------------------
#endif
