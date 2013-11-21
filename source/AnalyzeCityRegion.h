//---------------------------------------------------------------------------

#ifndef AnalyzeCityRegionH
#define AnalyzeCityRegionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TAnalyzeCityRegionFrm : public TForm
{
__published:	// IDE-managed Components
  TButton *bnLoad;
  TCheckBox *cbSaveOnClose;
  TButton *bnSave;
  TLabel *labLocation;
  TButton *bnAddTurn;
  TEdit *edDefBasePop;
  TStringGrid *sgMarkets;
  TStringGrid *sgMarket;
  TEdit *edBasePop;
  TEdit *edMaxAmt;
  TLabel *labMarketItem;
  TLabel *labError;
  TCheckBox *cbConfirmed;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  void __fastcall bnLoadClick(TObject *Sender);
  void __fastcall bnSaveClick(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall bnAddTurnClick(TObject *Sender);
  void __fastcall edDefBasePopExit(TObject *Sender);
  void __fastcall sgMarketsDblClick(TObject *Sender);
  void __fastcall edBasePopExit(TObject *Sender);
  void __fastcall edMaxAmtExit(TObject *Sender);
  void __fastcall edBasePopKeyPress(TObject *Sender, char &Key);
  void __fastcall edMaxAmtKeyPress(TObject *Sender, char &Key);
  void __fastcall cbConfirmedClick(TObject *Sender);
private:	// User declarations
  AnsiString curItem;

  bool Init();
public:		// User declarations
  __fastcall TAnalyzeCityRegionFrm(TComponent* Owner);
  void ShowInfo();
  void ShowMarket();
};
//---------------------------------------------------------------------------
//extern PACKAGE TAnalyzeCityRegionFrm *AnalyzeCityRegionFrm;
//---------------------------------------------------------------------------
class ALocation;
bool FillConfirmedMarketMaxAmounts(const ALocation &loc,map<AnsiString,int> &amounts, int &basepop);
#endif
