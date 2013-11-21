//---------------------------------------------------------------------------
#ifndef EventsFrmH
#define EventsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEventsForm : public TForm
{
__published:	// IDE-managed Components
    TMemo *Memo;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
 void __fastcall FormDestroy(TObject *Sender);
    void __fastcall MemoDblClick(TObject *Sender);
  void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
private:	// User declarations
    bool begin;
    int nobegincount;
public:		// User declarations
    bool Disabled;

    __fastcall TEventsForm(TComponent* Owner);
    void AddEvent(AnsiString ev);
    void ClearEvents();
    void Begin();
    void AddNoClearOnBegin();
};
//---------------------------------------------------------------------------
extern PACKAGE TEventsForm *EventsForm;
//---------------------------------------------------------------------------
#endif
