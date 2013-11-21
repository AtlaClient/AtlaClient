//---------------------------------------------------------------------------
#ifndef AboutFrmH
#define AboutFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TAboutForm : public TForm
{
__published:	// IDE-managed Components
  TLabel *labName;
  TLabel *Label2;
  TLabel *labHomePage;
  TLabel *Label3;
  TLabel *labEMail;
  TButton *bnClose;
  TLabel *labRegCount;
        TLabel *Label1;
        TLabel *Label4;
  void __fastcall labHomePageClick(TObject *Sender);
  void __fastcall labEMailClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TAboutForm(TComponent* Owner);
  static void UpdateVersionString();
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutForm *AboutForm;
//---------------------------------------------------------------------------
#endif
