//---------------------------------------------------------------------------
#ifndef SupportBuildingFrmH
#define SupportBuildingFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TSupportBuildingForm : public TForm
{
__published:	// IDE-managed Components
  TStringGrid *sgCalc;
  TEdit *edProds;
  TLabel *Label1;
  TLabel *Label2;
  TEdit *edBuilds;
  TStringGrid *sgReg;
  void __fastcall bnCalcClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall sgRegDblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TSupportBuildingForm(TComponent* Owner);
  void MakeCalc(int begprod);
  static int CalcBegProd(int prod, int builds);
  static int CalcProd(int prods, int builds);
  static int CalcMaxProd(int prods);
};
//---------------------------------------------------------------------------
//extern PACKAGE TSupportBuildingForm *SupportBuildingForm;
//---------------------------------------------------------------------------
#endif
