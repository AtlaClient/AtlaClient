//---------------------------------------------------------------------------
#ifndef EAPCustomMemoH
#define EAPCustomMemoH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <clipbrd.hpp>
//---------------------------------------------------------------------------
//компонент - текстовый редактор

typedef void __fastcall (__closure *TEAPTextScroledEvent)(TObject* Sender, int TopRow, int BottomRow
	, int LeftCol, int MaxCols);
typedef void __fastcall (__closure *TEAPDrawLineEvent)(TObject* Sender, AnsiString LineText,
	int LineIndex, Graphics::TBitmap* LineGlyph, const TRect &TextRect, bool &AllowSelfDraw);
enum TEAPKeyEvent {k_Down, k_Press, k_Up, k_Mouse1, k_Mouse2, k_Mouse3, k_DoubleClick, k_None};

typedef void __fastcall (__closure *TEAPMemoEvents)(TObject* Sender, Classes::TShiftState Shift
	, TEAPKeyEvent KeyEvent, Word Key, const TPoint &CaretPos, const TPoint &MousePos);

typedef void __fastcall (__closure *TEAPFindText)(TObject* Sender, const TPoint &FindPos
	, bool &ScrollToWord);

typedef void __fastcall (__closure *TEAPReplaceText)(TObject* Sender, const TPoint &
	FindPos, bool &ScrollToWord, bool &ReplaceText);

typedef void __fastcall (__closure *TEAPOnSearchEnd)(System::TObject* Sender, bool FindIt, bool ReplaceMode);

enum TEAPFindOption{ foMatchCase, foWholeWords, foEntireScope };

typedef Set<TEAPFindOption, foMatchCase, foEntireScope>  TEAPFindOptions;

class PACKAGE TEAPCustomMemo : public TCustomControl
{
	typedef Controls::TCustomControl inherited;
private:
    HWND FWindowHandle;
	bool FTracking;
	bool FFullRedraw;
	bool FVScrollVisible;
	bool FHScrollVisible;
	Classes::TNotifyEvent FOnScrolled_V;
	Classes::TNotifyEvent FOnScrolled_H;
	TEAPTextScroledEvent FOnTextScrolled;
	Controls::TMouseWheelEvent FOnMouseWheel;
	Controls::TMouseWheelUpDownEvent FOnMouseWheelDown;
	Controls::TMouseWheelUpDownEvent FOnMouseWheelUp;
	Classes::TNotifyEvent FOnPaint;
    TScrollStyle FScrollBars;
	TFormBorderStyle FBorderStyle;
	int FMaxScrollH;
	int FWheelAccumulator;

	TStrings* FLines;

	int FChrW;
	int FChrH;
	int FTopLine;
	int FVisLines;
	int FVisCols;
	int FSelStartNo;
	int FSelEndNo;
	int FSelStartOffs;
	int FSelEndOffs;
	bool FSelected;
	bool FSelMouseDwn;
	int VScrollDelta;
	int HScrollDelta;
	int XMouse;
	int YMouse;
	Extctrls::TTimer* ScrollTimer;
	int HPos;
	int VPos;
	int XSize;
	int YSize;
	int StartNo;
	int EndNo;
	int StartOffs;
	int EndOffs;
	TPoint CaretPos,oldCaretPos;
	TPoint MousePos;
	Classes::TShiftState ShiftState;
	bool HideCaret;
  bool OldHideCaret;
	bool PulseCaret;
	bool FDblClick;

    bool FModified;

	TEAPDrawLineEvent FOnDrawLine;
	TEAPMemoEvents FOnMemoEvents;
	TEAPFindText FOnFindText;
	TEAPReplaceText FOnReplaceText;
	TEAPOnSearchEnd FOnSerachEnd;
    Classes::TNotifyEvent FOnCaretMove;

//my
//    TColor FTextBackGround;
    TColor FTextColor;
    TColor FSelBackGround;
    TColor FSelColor;
    bool FReadOnly;
    int FVisLinesAlterCaret;


	int __fastcall GetVScrollPos();
	void __fastcall SetVScrollPos(int Pos);
	int __fastcall GetVScrollMax();
	int __fastcall GetHScrollPos();
	void __fastcall SetHScrollPos(int Pos);
	int __fastcall GetHScrollMax();
	void __fastcall SetScrollBars(TScrollStyle Value);
	void __fastcall SetBorderStyle(TBorderStyle Value);
	void __fastcall SetLines(TStrings* Value);

	void __fastcall CaretBlinkingTimeProc(Messages::TMessage &Msg);

	int __fastcall GetCaretVPos();
	int __fastcall GetCaretHPos();


	TPoint __fastcall GetSelBegin();
	TPoint __fastcall GetSelEnd();
	void __fastcall SetSelBegin(TPoint Value);
	void __fastcall SetSelEnd(TPoint Value);
    int __fastcall GetThumbtrack_V();
protected:
	virtual void __fastcall CreateParams(TCreateParams &Params);
	virtual void __fastcall CreateWnd();
	void __fastcall UpdateTimer();
	void __fastcall UpdateScrollBars();
	HIDESBASE MESSAGE void __fastcall WMHScroll(Messages::TWMScroll &Message);
	HIDESBASE MESSAGE void __fastcall WMVScroll(Messages::TWMScroll &Message);
	MESSAGE void __fastcall WMGetDlgCode(Messages::TWMNoParams &Message);
	HIDESBASE MESSAGE void __fastcall CMMouseWheel(Controls::TCMMouseWheel &Message);
	DYNAMIC void __fastcall MouseWheel(Classes::TShiftState Shift, int WheelDelta, const TPoint&MousePos);
	DYNAMIC bool __fastcall MouseWheelDown(Classes::TShiftState Shift, const TPoint &MousePos);
	DYNAMIC bool __fastcall MouseWheelUp(Classes::TShiftState Shift, const TPoint &MousePos);

	DYNAMIC void __fastcall MouseUp(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X,	int Y);
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);

	DYNAMIC void __fastcall DblClick(void);

	void __fastcall SetVPos(int p);
	void __fastcall SetHPos(int p);

	void __fastcall UpdateCharBounds(void);

	void __fastcall FormatLine(int I, Graphics::TBitmap* &LGliph);
	virtual void __fastcall Paint(void);
	void __fastcall DrawLine(Graphics::TCanvas* CanvasSupport, int I, int Y);
	void __fastcall DrawVisible(void);
	void __fastcall GetSelBounds(int &StartNo, int &EndNo, int &StartOffs, int &EndOffs);
	void __fastcall ScrollOnTimer(TObject* Sender);
	void __fastcall BlinkCaret();
	void __fastcall DrawCaret(int X, int Y, bool ShowCaret);
	void __fastcall KeyboardCaretNav(Classes::TShiftState ShiftState, Word Direction);

	HIDESBASE MESSAGE void __fastcall WMKeyDown(Messages::TWMKey &Message);
	HIDESBASE MESSAGE void __fastcall WMKeyUp(Messages::TWMKey &Message);
	DYNAMIC void __fastcall KeyPress(char &Key);

	bool __fastcall FindReplaceProc(AnsiString TextToFind, TEAPFindOptions FindOptions, bool Backward, bool
		ReplaceMode, bool &ReplaceText);
public:
    __fastcall TEAPCustomMemo(TComponent* Owner);
    __fastcall ~TEAPCustomMemo();
	AnsiString __fastcall GetSelText();
    void __fastcall SetSelText(AnsiString str);
	AnsiString __fastcall GetWordAtPos(int Col, int Row, int &XBegin);
	void __fastcall GetRowColAtPos(int X, int Y, int &Row, int &Col);
	void __fastcall InsertTextAtPos(AnsiString S, int Col, int Row);
	void __fastcall ScrollTo(int X, int Y);

	void __fastcall CopyToClipboard();
	void __fastcall CutToClipboard();
	void __fastcall PasteFromClipboard();
	void __fastcall DeleteSelection();
    void __fastcall SelectAll();
	void __fastcall FindText(AnsiString TextToFind, TEAPFindOptions FindOptions, bool Backward);
	void __fastcall ReplaceText(AnsiString TextToFind, AnsiString TextToReplace, TEAPFindOptions FindOptions	, bool Backward);
	void __fastcall Clear();
	void __fastcall SaveToFile(const AnsiString FileName);
	void __fastcall LoadFromFile(const AnsiString FileName);
    void MakeCaretVisible();

	__property TPoint SelBegin = {read=GetSelBegin, write=SetSelBegin};
	__property TPoint SelEnd = {read=GetSelEnd, write=SetSelEnd};
	__property bool Selected = {read=FSelected, nodefault};
	__property int CaretPos_H = {read=GetCaretHPos, nodefault};
	__property int CaretPos_V = {read=GetCaretVPos, nodefault};
	__property int FontHeight = {read=FChrH, nodefault};
	__property int FontWidth = {read=FChrW, nodefault};

	__property bool Modified = {read=FModified, write=FModified, nodefault};
	__property TScrollStyle ScrollBars = {read=FScrollBars, write=SetScrollBars, default=1};
	__property int ScrollMax_H = {read=GetHScrollMax, nodefault};
	__property int ScrollMax_V = {read=GetVScrollMax, nodefault};
	__property int ScrollPos_H = {read=GetHScrollPos, write=SetHScrollPos, nodefault};
	__property int ScrollPos_V = {read=GetVScrollPos, write=SetVScrollPos, nodefault};
	__property int TopLine = {read=FTopLine, nodefault};
	__property int VisibleLines = {read=FVisLines, nodefault};

	__property Forms::TBorderStyle BorderStyle = {read=FBorderStyle, write=SetBorderStyle, default=1};

	__property bool FullRedraw = {read=FFullRedraw, write=FFullRedraw, default=0};

	__property Classes::TStrings* Lines = {read=FLines, write=SetLines};

	__property bool Tracking = {read=FTracking, write=FTracking, default=0};

	__property TEAPDrawLineEvent OnDrawLine = {read=FOnDrawLine, write=FOnDrawLine};

	__property TEAPFindText OnFindText = {read=FOnFindText, write=FOnFindText};

	__property TEAPMemoEvents OnMemoEvents = {read=FOnMemoEvents, write=FOnMemoEvents};
	__property Controls::TMouseWheelEvent OnMouseWheel = {read=FOnMouseWheel, write=FOnMouseWheel};
	__property Controls::TMouseWheelUpDownEvent OnMouseWheelDown = {read=FOnMouseWheelDown, write=FOnMouseWheelDown};
	__property Controls::TMouseWheelUpDownEvent OnMouseWheelUp = {read=FOnMouseWheelUp, write=FOnMouseWheelUp};
	__property Classes::TNotifyEvent OnPaint = {read=FOnPaint, write=FOnPaint};
	__property TEAPReplaceText OnReplaceText = {read=FOnReplaceText, write=FOnReplaceText};
	__property Classes::TNotifyEvent OnScrolled_H = {read=FOnScrolled_H, write=FOnScrolled_H};
	__property Classes::TNotifyEvent OnScrolled_V = {read=FOnScrolled_V, write=FOnScrolled_V};
	__property TEAPOnSearchEnd OnSearchEnd = {read=FOnSerachEnd, write=FOnSerachEnd};
	__property TEAPTextScroledEvent OnTextScrolled = {read=FOnTextScrolled, write=FOnTextScrolled};
    __property Classes::TNotifyEvent OnCaretMove = {read=FOnCaretMove, write=FOnCaretMove};
    __property bool ReadOnly  = { read=FReadOnly, write=FReadOnly};
    __property AnsiString SelText  = { read=GetSelText, write=SetSelText };
    __property int VisLinesAlterCaret  = { read=FVisLinesAlterCaret, write=FVisLinesAlterCaret };
__published:
BEGIN_MESSAGE_MAP
  VCL_MESSAGE_HANDLER(WM_HSCROLL, Messages::TWMScroll, WMHScroll)
  VCL_MESSAGE_HANDLER(WM_VSCROLL, Messages::TWMScroll, WMVScroll)
  VCL_MESSAGE_HANDLER(WM_GETDLGCODE, Messages::TWMNoParams, WMGetDlgCode)
  VCL_MESSAGE_HANDLER(CM_MOUSEWHEEL, Controls::TCMMouseWheel, CMMouseWheel)
  VCL_MESSAGE_HANDLER(WM_KEYDOWN, Messages::TWMKey, WMKeyDown)
  VCL_MESSAGE_HANDLER(WM_KEYUP, Messages::TWMKey, WMKeyUp)
END_MESSAGE_MAP(inherited);
};
class PACKAGE TEAPMemo: public TEAPCustomMemo{
	typedef TEAPCustomMemo inherited;
private:
protected:
public:
    __fastcall TEAPMemo(TComponent* Owner):TEAPCustomMemo(Owner){}

    __property Font;
    __property Color;
__published:

};
static const Word DT_DRAWLINE = 0x824;

//---------------------------------------------------------------------------
#endif
