//---------------------------------------------------------------------------

#ifndef FlagFiltrH
#define FlagFiltrH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
struct TarFlags
{
  unsigned guard;
  unsigned avoid;
  unsigned behind;
  unsigned reveal;
  unsigned autotax;
  unsigned hold;
  unsigned noaid;
  unsigned consume;
  unsigned spoils;
  unsigned nocroswater;
  unsigned sharing;
  unsigned nobattspoils;
};

class TfFlagFiltr : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TComboBox *cbGuard;
	TComboBox *cbAvoid;
	TLabel *Label2;
	TComboBox *cbReveal;
	TLabel *Label3;
	TLabel *Label4;
	TComboBox *cbBehind;
	TGroupBox *GroupBox2;
	TLabel *Label5;
	TComboBox *cbAutoTax;
	TComboBox *cbHold;
	TLabel *Label6;
	TComboBox *cbConsume;
	TLabel *Label7;
	TLabel *Label8;
	TComboBox *cbNoaid;
	TGroupBox *GroupBox3;
	TLabel *lbNoCross;
	TComboBox *cbNoCross;
	TLabel *lbNoSpoils;
	TComboBox *cbNoSpoils;
	TLabel *lbSpoils;
	TComboBox *cbSpoils;
	TComboBox *cbSharing;
	TLabel *lbSharing;
	TPanel *Panel2;
	TButton *bnCancel;
	TButton *bnOk;
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfFlagFiltr(TComponent* Owner);
	void __fastcall SetFlags(TarFlags af);
};
//---------------------------------------------------------------------------
extern PACKAGE TfFlagFiltr *fFlagFiltr;
//---------------------------------------------------------------------------
#endif
