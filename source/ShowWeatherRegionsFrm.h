//---------------------------------------------------------------------------

#ifndef ShowWeatherRegionsFrmH
#define ShowWeatherRegionsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TShowWeatherRegionsForm : public TForm
{
__published:	// IDE-managed Components
    TEdit *edX;
    TEdit *edY;
    TButton *bnOk;
    TButton *bnCancel;
    TLabel *labMapSize;
    TLabel *labX;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall bnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TShowWeatherRegionsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TShowWeatherRegionsForm *ShowWeatherRegionsForm;
//---------------------------------------------------------------------------
#endif
