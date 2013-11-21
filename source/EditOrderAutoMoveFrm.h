//---------------------------------------------------------------------------
#ifndef EditOrderAutoMoveFrmH
#define EditOrderAutoMoveFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TEditOrderAutoMoveForm : public TForm
{
__published:	// IDE-managed Components
    TButton *bnClose;
    TListBox *lbOrders;
    TPanel *Panel1;
    TCheckBox *cbCurrent;
    TCheckBox *cbCommented;
    TEdit *edX;
    TButton *bnApply;
    TLabel *labBegPos;
    TEdit *edY;
    TButton *bnEdit;
    TLabel *labMess;
    TLabel *labReg;
    TButton *bnGetPos;
    TCheckBox *cbSail;
    TButton *bnAdd;
    TButton *bnDelete;
    TButton *bnMoveUp;
    TButton *bnDown;
  TEdit *edZ;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall lbOrdersClick(TObject *Sender);
    void __fastcall cbCommentedClick(TObject *Sender);
    void __fastcall bnEditClick(TObject *Sender);
    void __fastcall bnApplyClick(TObject *Sender);
    void __fastcall bnGetPosClick(TObject *Sender);
    void __fastcall cbSailClick(TObject *Sender);
    void __fastcall bnAddClick(TObject *Sender);
    void __fastcall bnDeleteClick(TObject *Sender);
    void __fastcall bnMoveUpClick(TObject *Sender);
    void __fastcall bnDownClick(TObject *Sender);
private:	// User declarations
    AUnit *un;
    AOrders *ords;
    AOrderAutoMoving *curord;
    int lastindex;
    ARegion *curreg;
public:		// User declarations
    __fastcall TEditOrderAutoMoveForm(TComponent* Owner, AUnit *_un,AOrder *ord);
    bool Swap(int ind1,int ind2);
protected:
    void Show();
};
//---------------------------------------------------------------------------
extern PACKAGE TEditOrderAutoMoveForm *EditOrderAutoMoveForm;
//---------------------------------------------------------------------------
#endif
