//---------------------------------------------------------------------------

#ifndef TownTradeH
#define TownTradeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TfTownTrade : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TLabel *Label1;
        TComboBox *cbItems;
        TGroupBox *gb2;
        TGroupBox *gb1;
        TStringGrid *grSale;
        TStringGrid *grWanted;
        TPanel *Panel2;
        TLabel *lb1;
        TLabel *lb2;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall cbItemsChange(TObject *Sender);
        void __fastcall grSaleDblClick(TObject *Sender);
        void __fastcall grWantedDblClick(TObject *Sender);
        void __fastcall grSaleSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall grWantedClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
        void UpdateInfo();
public:		// User declarations
        __fastcall TfTownTrade(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfTownTrade *fTownTrade;
//---------------------------------------------------------------------------
#endif
