//---------------------------------------------------------------------------
#ifndef ShortCutsFrmH
#define ShortCutsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TShortCutOwner;
class TShortCutsForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *panRight;
  TListBox *List;
  THotKey *HotKey;
  TLabel *labCaption;
  TButton *bnSet;
  TTreeView *TreeView;
  TButton *bnClearShortCuts;
  void __fastcall bnSetClick(TObject *Sender);
  void __fastcall TreeViewChange(TObject *Sender, TTreeNode *Node);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall bnClearShortCutsClick(TObject *Sender);
private:	// User declarations
  const TShortCutOwner *sco;
public:		// User declarations
  __fastcall TShortCutsForm(TComponent* Owner);

  void ShowShortCut();
};
//---------------------------------------------------------------------------
//extern PACKAGE TShortCutsForm *ShortCutsForm;
//---------------------------------------------------------------------------
#endif
