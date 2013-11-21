//---------------------------------------------------------------------------
#ifndef ShowItObjInfosFrmH
#define ShowItObjInfosFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class AItObjInfo;
class TShowItObjInfosForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TListBox *List;
    TButton *bnCancel;
    TMemo *Memo;
    TSplitter *Splitter1;
  TButton *bnRequery;
  TButton *bnRequeryAll;
  TCheckBox *cbItems;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
 void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
 void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall ListClick(TObject *Sender);
  void __fastcall bnRequeryClick(TObject *Sender);
  void __fastcall bnRequeryAllClick(TObject *Sender);
  void __fastcall cbItemsClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TShowItObjInfosForm(TComponent* Owner);
  void RequeryInfo(AItObjInfo * ioi);
};
//---------------------------------------------------------------------------
extern PACKAGE TShowItObjInfosForm *ShowItObjInfosForm;
//---------------------------------------------------------------------------
#endif
