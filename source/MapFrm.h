//---------------------------------------------------------------------------
#ifndef MapFrmH
#define MapFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMapForm : public TForm
{
__published:	// IDE-managed Components
        void __fastcall ImageMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ImageMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
  void __fastcall FormPaint(TObject *Sender);
  void __fastcall ImageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
    Graphics::TBitmap *bmp;
        int oldzloc;
    bool FShowMap;
    int FMul;
    TRect currect;
    bool moving;
    TPoint movestart;
    int docktype; //1-left,2-top,4-right,8-bottom

    void __fastcall SetShowMap(bool value);
    void __fastcall SetMul(int value);
public:		// User declarations
        __fastcall TMapForm(TComponent* Owner);
    void UpdateMap(int zloc);
    void ResetMap();
    void PointToCoord(int x, int y, int & xloc, int& yloc);
    void ReCalcRect();
    virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
  void ApplyDockType();

    __property bool ShowMap  = { read = FShowMap, write = SetShowMap };
    __property int Mul  = { read = FMul, write = SetMul };
protected:
    void CreateMap(int zloc);
};
//---------------------------------------------------------------------------
extern PACKAGE TMapForm *MapForm;
//---------------------------------------------------------------------------
#endif
