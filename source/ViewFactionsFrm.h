//---------------------------------------------------------------------------

#ifndef ViewFactionsFrmH
#define ViewFactionsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFactListItem{
public:
    AFaction *fac;

    TFactListItem();
    virtual ~TFactListItem();
    virtual void Draw(TDrawGrid *Grid,int ACol,TRect &Rect, TGridDrawState State);
    void UpdateFactionInfo();
    int CompareWith(TFactListItem * b);

    __property AnsiString Texts[int Col]  = { read=GetTexts/*, write=SetTexts */};
private:
    TStringList *FText;
    AnsiString __fastcall GetTexts(int Col);
};
int __fastcall DoCompareListItem(void * Item1, void * Item2);

class TViewFactionsForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TDrawGrid *Grid;
    TButton *bnSetCurrent;
    TButton *bnSendMail;
    TCheckBox *cbFacDeleted;
    TButton *bnSendOrders;
  TButton *bnFindAddress;
        TButton *btAttDef;
        TColorDialog *ColorDialog;
    void __fastcall GridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall GridDblClick(TObject *Sender);
    void __fastcall GridSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
    void __fastcall bnSetCurrentClick(TObject *Sender);
    void __fastcall bnSendMailClick(TObject *Sender);
    void __fastcall cbFacDeletedClick(TObject *Sender);
    void __fastcall bnSendOrdersClick(TObject *Sender);
  void __fastcall bnFindAddressClick(TObject *Sender);
        void __fastcall btAttDefClick(TObject *Sender);
private:	// User declarations
    TList *FListItems;
    TStringList *headers;
    bool inselectcell;

    AUnit * GetLastUnit();
    TFactListItem * __fastcall GetListItems(int index);
public:		// User declarations
    static int SortCol,SortType;

    __fastcall TViewFactionsForm(TComponent* Owner);
    __fastcall ~TViewFactionsForm();
    void ClearList();
    void AddListItem(TFactListItem * li);
    void CreateList();
    void SortList();
    __property TFactListItem * ListItems[int index]  = { read=GetListItems };
};
//---------------------------------------------------------------------------
//extern PACKAGE TViewFactionsForm *ViewFactionsForm;
//---------------------------------------------------------------------------
#endif
