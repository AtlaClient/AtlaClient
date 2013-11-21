//---------------------------------------------------------------------------
#ifndef LinkShaftFrmH
#define LinkShaftFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <vector.h>
//---------------------------------------------------------------------------
class TLinkShaftForm : public TForm
{
__published:	// IDE-managed Components
    TListBox *List;
    TButton *bnLink;
    TButton *bnCancel;
  TButton *bnAnalyze;
  TButton *bnGoto;
    void __fastcall bnLinkClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
  void __fastcall bnAnalyzeClick(TObject *Sender);
  void __fastcall bnGotoClick(TObject *Sender);
private:	// User declarations
    AObject *obj;
    vector<int> xx,yy;
  void CalcXY(int levelto);
public:		// User declarations
    ALocation GotoLoc; //ModalResult==mrYes;

    __fastcall TLinkShaftForm(TComponent* Owner,AObject *_obj);
};
//---------------------------------------------------------------------------
extern PACKAGE TLinkShaftForm *LinkShaftForm;
//---------------------------------------------------------------------------
#endif
