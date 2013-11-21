//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "EAPCustomMemo.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static TPoint RepPos;

static inline void ValidCtrCheck(TEAPCustomMemo *)
{
    new TEAPCustomMemo(NULL);
}
static inline void ValidCtrCheck(TEAPMemo *)
{
    new TEAPMemo(NULL);
}
namespace Eapmemo
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TEAPMemo)};
         RegisterComponents("EAP", classes, 0);
    }
}
//---------------------------------------------------------------------------
__fastcall TEAPCustomMemo::TEAPCustomMemo(TComponent* Owner)
    : TCustomControl(Owner)
{
// ControlStyle=ControlStyle<<csReplicatable;
 FWindowHandle=AllocateHWnd(CaretBlinkingTimeProc);
 Width=320;
 Height=240;
 TabStop=true;
 FTracking=true;
 FFullRedraw=false;
 FScrollBars= ssNone;
 FVScrollVisible= true;
 FHScrollVisible= true;
 FBorderStyle= bsSingle;
 FLines=new TStringList;
 ParentColor=false;
 Color=clWindow;
 Font->Name="Courier New";//"Fixedsys";
 Font->Color=clWindowText;
 Font->Size=8;
 FTopLine=0;
 FMaxScrollH=0;
 Cursor=crIBeam;
 FSelected=false;
 FSelMouseDwn=false;
 CaretPos.x=0;
 CaretPos.y=0;
 oldCaretPos.x=-1;
 oldCaretPos.y=-1;
 PulseCaret=true;
 FDblClick=false;

 FTextColor=clWindowText;
// FTextBackGround=clWindow;
 FSelColor=clHighlightText;
 FSelBackGround=clHighlight;

 FModified=false;
 FVisLinesAlterCaret=0;
}
__fastcall TEAPCustomMemo::~TEAPCustomMemo(){
 delete FLines;
 delete ScrollTimer;

 KillTimer(FWindowHandle,1);
 DeallocateHWnd(FWindowHandle);

}
//---------------------------------------------------------------------------
void __fastcall TEAPCustomMemo::CreateParams(TCreateParams &Params){
static DWORD BorderStyles[bsSizeToolWin+1]={0, WS_BORDER};
 inherited::CreateParams(Params);
 Params.Style|=BorderStyles[FBorderStyle];
 Params.Style|=WS_CLIPCHILDREN|WS_HSCROLL|WS_VSCROLL;
 if(NewStyleControls&&Ctl3D&&FBorderStyle==bsSingle){
  Params.Style&=~WS_BORDER;
  Params.ExStyle|=WS_EX_CLIENTEDGE;
 }
 Params.WindowClass.style&=~(CS_HREDRAW||CS_VREDRAW);
}
void __fastcall TEAPCustomMemo::CreateWnd(void){
 inherited::CreateWnd();
 VPos=0;
 HPos=0;
 UpdateCharBounds();
 UpdateScrollBars();
 ScrollTimer=new TTimer(this);
 ScrollTimer->Interval=100;
 ScrollTimer->Enabled=false;
 ScrollTimer->OnTimer=ScrollOnTimer;
 if(ComponentState.Contains(csDesigning)){
  if(FLines->Count==0) FLines->Add(Name);
 }
 UpdateTimer();
}
MESSAGE void __fastcall TEAPCustomMemo::WMGetDlgCode(Messages::TWMNoParams &Message){
// inherited::WMGetDlgCode(Message);
 Message.Result|=/*DLGC_WANTTAB|*/DLGC_WANTARROWS;
}
void __fastcall TEAPCustomMemo::CaretBlinkingTimeProc(Messages::TMessage &Msg){
 if(Msg.Msg==WM_TIMER){
  __try{
   BlinkCaret();
  }catch(...){
   Application->HandleException(this);
  }
 }else
  DefWindowProc(FWindowHandle, Msg.Msg, Msg.WParam, Msg.LParam);
}
void __fastcall TEAPCustomMemo::UpdateTimer(void){
 KillTimer(FWindowHandle, 1);
 if(PulseCaret)
  if(SetTimer(FWindowHandle, 1, 450, 0)==0)
   throw EOutOfResources("CARET_TIME_ERROR");
}
void __fastcall TEAPCustomMemo::SetScrollBars(TScrollStyle value){
static bool V_SCROLL[]={false, false,true, true};
static bool H_SCROLL[]={false, true, false,true};
 if(value==FScrollBars) return;
 FVScrollVisible=V_SCROLL[value];
 FHScrollVisible=H_SCROLL[value];
 ShowScrollBar(Handle, SB_VERT, FVScrollVisible);
 ShowScrollBar(Handle, SB_HORZ, FHScrollVisible);
}
void __fastcall TEAPCustomMemo::SetBorderStyle(TBorderStyle Value){
 if(FBorderStyle==Value) return;
 FBorderStyle=Value;
 RecreateWnd();
}
void __fastcall TEAPCustomMemo::SetLines(TStrings* Value){
 FLines->Assign(Value);
 FModified=false;
 Invalidate();
}
int __fastcall TEAPCustomMemo::GetCaretVPos(){
 return CaretPos.x;
}
int __fastcall TEAPCustomMemo::GetCaretHPos(){
 return CaretPos.y;
}
void __fastcall TEAPCustomMemo::UpdateScrollBars(){
 TScrollInfo ScrollInfoV, ScrollInfoH;
 ScrollInfoV.cbSize= sizeof(ScrollInfoV);
 ScrollInfoV.fMask= SIF_ALL;
 ScrollInfoV.nMin= 0;
 ScrollInfoV.nPage= FVisLines - 1;
 ScrollInfoV.nMax= FLines->Count;
 ScrollInfoV.nPos= VPos;
 ScrollInfoV.nTrackPos= 0;
 SetScrollInfo(Handle, SB_VERT, &ScrollInfoV, true);

 ScrollInfoH.cbSize= sizeof(ScrollInfoH);
 ScrollInfoH.fMask= SIF_ALL;
 ScrollInfoH.nMin= 0;
 ScrollInfoH.nPage= FMaxScrollH/4;
 ScrollInfoH.nMax= FMaxScrollH;
 ScrollInfoH.nPos= HPos;
 ScrollInfoH.nTrackPos= 0;
 SetScrollInfo(Handle, SB_HORZ, &ScrollInfoH, true);

 if(!FVScrollVisible) ShowScrollBar(Handle, SB_VERT, FVScrollVisible);
 if(!FHScrollVisible) ShowScrollBar(Handle, SB_HORZ, FHScrollVisible);
}
void __fastcall TEAPCustomMemo::WMHScroll(Messages::TWMScroll &Message){
 switch(Message.ScrollCode){
  case SB_LINERIGHT: SetHPos(HPos + 1);break;
  case SB_LINELEFT: SetHPos(HPos - 1);break;
  case SB_PAGEUP: SetHPos(HPos - FVisCols);break;
  case SB_PAGEDOWN: SetHPos(HPos + FVisCols);break;
  case SB_THUMBPOSITION: SetHPos(Message.Pos);break;
  case SB_THUMBTRACK: if(FTracking)SetHPos(Message.Pos);break;
  case SB_TOP: SetHPos(0);break;
  case SB_BOTTOM: SetHPos(XSize);break;
 }
}
void __fastcall TEAPCustomMemo::WMVScroll(Messages::TWMScroll &Message){

 switch(Message.ScrollCode){
  case SB_LINEUP: SetVPos(VPos - 1);break;
  case SB_LINEDOWN: SetVPos(VPos + 1);break;
  case SB_PAGEUP: SetVPos(VPos - FVisLines);break;
  case SB_PAGEDOWN: SetVPos(VPos + FVisLines);break;
  case SB_THUMBPOSITION: SetVPos(GetThumbtrack_V());break;
  case SB_THUMBTRACK: if(FTracking)SetVPos(GetThumbtrack_V());break;
  case SB_TOP: SetVPos(0);break;
  case SB_BOTTOM: SetVPos(YSize);break;
 }
}
void __fastcall TEAPCustomMemo::SetVPos(int p){
 TScrollInfo ScrollInfo;
 int oldPos=VPos;
 TRect Rc;
 VPos=p;
 ScrollInfo.cbSize= sizeof(ScrollInfo);
 ScrollInfo.nPos=VPos;
 ScrollInfo.fMask=SIF_POS;
 SetScrollInfo(Handle,SB_VERT,&ScrollInfo, true);
 GetScrollInfo(Handle, SB_VERT,&ScrollInfo);
 VPos=ScrollInfo.nPos;
 Rc=ClientRect;
 if(oldPos-VPos!=0){
  FTopLine=VPos;
  if(FFullRedraw)
   Refresh();
  else if(FTopLine + FVisLines < FLines->Count)
   DrawVisible();
  else
   ScrollWindowEx(Handle, 0, (oldPos - VPos) * FChrH, 0, &Rc, 0, 0, SW_INVALIDATE);
  if(FOnScrolled_V) FOnScrolled_V(this);
  if(FOnTextScrolled) FOnTextScrolled(this, FTopLine, FTopLine + FVisLines + 1,HPos, FMaxScrollH);
 }
}
void __fastcall TEAPCustomMemo::SetHPos(int p){
 TScrollInfo ScrollInfo;
 int oldPos=HPos;
 TRect Rc;
 HPos=p;
 ScrollInfo.cbSize=sizeof(ScrollInfo);
 ScrollInfo.nPos=HPos;
 ScrollInfo.fMask=SIF_POS;
 SetScrollInfo(Handle,SB_HORZ,&ScrollInfo, true);
 GetScrollInfo(Handle, SB_HORZ,&ScrollInfo);
 HPos=ScrollInfo.nPos;
 Rc=ClientRect;
 if(oldPos-HPos!=0){
  if(FFullRedraw)
   Refresh();
  else
   DrawVisible();
  if(FOnScrolled_H) FOnScrolled_H(this);
  if(FOnTextScrolled) FOnTextScrolled(this, FTopLine, FTopLine + FVisLines, HPos, FMaxScrollH);
 }
}
void __fastcall TEAPCustomMemo::ScrollTo(int X, int Y){
 if(FChrH) SetVPos(Y/FChrH);
 if(FChrW) SetHPos(X/FChrW);
}
int __fastcall TEAPCustomMemo::GetVScrollPos(){
 return VPos;
}
void __fastcall TEAPCustomMemo::SetVScrollPos(int Pos){
 SetVPos(Pos);
}
int __fastcall TEAPCustomMemo::GetHScrollPos(){
 return HPos;
}
void __fastcall TEAPCustomMemo::SetHScrollPos(int Pos){
 SetHPos(Pos);
}
int __fastcall TEAPCustomMemo::GetVScrollMax(){
 TScrollInfo ScrollInfo;
 ScrollInfo.cbSize=sizeof(ScrollInfo);
 ScrollInfo.fMask=SIF_RANGE|SIF_PAGE;
 GetScrollInfo(Handle, SB_VERT,&ScrollInfo);
 return ScrollInfo.nMax-ScrollInfo.nPage+1;
}
int __fastcall TEAPCustomMemo::GetHScrollMax(){
 TScrollInfo ScrollInfo;
 ScrollInfo.cbSize=sizeof(ScrollInfo);
 ScrollInfo.fMask=SIF_RANGE|SIF_PAGE;
 GetScrollInfo(Handle, SB_HORZ,&ScrollInfo);
 return ScrollInfo.nMax-ScrollInfo.nPage+1;
}
void __fastcall TEAPCustomMemo::CMMouseWheel(Controls::TCMMouseWheel &Message){
 TPoint p(Message.Pos.x,Message.Pos.y);
 MouseWheel(Message.ShiftState,Message.WheelDelta,p);
}
void __fastcall TEAPCustomMemo::MouseWheel(Classes::TShiftState Shift, int WheelDelta, const TPoint&MousePos){
 int ScrollNotify;
 bool hasShift, hasCtrl, NotDoIt, IsNeg;
 NotDoIt= false;
 if(FOnMouseWheel)
  FOnMouseWheel(this, Shift, WheelDelta, MousePos, NotDoIt);
 if(NotDoIt) return;
 ScrollNotify=-1;
 hasShift=Shift.Contains(ssShift);
 hasCtrl=Shift.Contains(ssCtrl);
 if(hasCtrl){
  if(WheelDelta>0) ScrollNotify=SB_LINELEFT;
  if(WheelDelta<0) ScrollNotify=SB_LINERIGHT;
  if(ScrollNotify!=-1){
   Perform(WM_HSCROLL, ScrollNotify, 0);
   Perform(WM_HSCROLL, ScrollNotify, 0);
  }
  return;
 }
 if(hasShift){
  DrawVisible();
  HideCaret=false;
  if(WheelDelta>0) CaretPos.x=CaretPos.x-1;
  if(WheelDelta<0) CaretPos.x=CaretPos.x+1;
  if(CaretPos.x<0) CaretPos.x=0;
  if(CaretPos.x>=FLines->Count) CaretPos.x=FLines->Count-1;
  DrawCaret(CaretPos.x, CaretPos.y, HideCaret);
  return;
 }
 if(!hasShift&&!hasCtrl){
  if(WheelDelta>0) ScrollNotify=SB_LINEUP;
  if(WheelDelta<0) ScrollNotify=SB_LINEDOWN;
  if(ScrollNotify!=-1)
   Perform(WM_VSCROLL, ScrollNotify, 0);
 }
 FWheelAccumulator+=WheelDelta;
 while(abs(FWheelAccumulator)>=WHEEL_DELTA){
  IsNeg=FWheelAccumulator<0;
  FWheelAccumulator=abs(FWheelAccumulator)-WHEEL_DELTA;
  if(IsNeg){
   if(FWheelAccumulator!=0) FWheelAccumulator=-FWheelAccumulator;
   NotDoIt=MouseWheelDown(Shift, MousePos);
  }else
   NotDoIt=MouseWheelUp(Shift, MousePos);
 }
}
bool __fastcall TEAPCustomMemo::MouseWheelDown(Classes::TShiftState Shift, const TPoint &MousePos){
 bool Result=false;
 if(FOnMouseWheelDown) FOnMouseWheelDown(this, Shift, MousePos, Result);
 return Result;
}
bool __fastcall TEAPCustomMemo::MouseWheelUp(Classes::TShiftState Shift, const TPoint &MousePos){
 bool Result=false;
 if(FOnMouseWheelUp) FOnMouseWheelUp(this, Shift, MousePos, Result);
 return Result;
}
void __fastcall TEAPCustomMemo::UpdateCharBounds(void){
 Canvas->Font=Font;
 FChrW=Canvas->TextWidth("W");
 FChrH=Canvas->TextHeight("Wp");
 FVisLines=(ClientHeight/FChrH)+1;
 FVisCols=ClientWidth/FChrW;
}
void __fastcall TEAPCustomMemo::Paint(void){
 UpdateCharBounds();

 Canvas->Brush->Color=Color;
 Canvas->FillRect(Canvas->ClipRect);
 DrawVisible();
 if(FOnPaint) FOnPaint(this);
}
void __fastcall TEAPCustomMemo::FormatLine(int I, Graphics::TBitmap* &LGliph){
 int TI, Si, Ei, T;
 AnsiString ss;
 if(!FLines->Count) return;
 if(I<0||I>=FLines->Count) return;
 AnsiString s=FLines->Strings[I];
 for(int i=s.Length();i>=1;i--){
  if(s[i]=='\t') s[i]=' ';
 }
 LGliph->Width=FChrW * s.Length();
 LGliph->Height=FChrH;
 TCanvas *canv=LGliph->Canvas;
 TRect r(0,0,LGliph->Width,LGliph->Height);
 bool AllowDraw=true;
 if(FOnDrawLine) FOnDrawLine(this, s, I, LGliph, r, AllowDraw);
 if(AllowDraw)
  DrawText(canv->Handle,s.c_str(), s.Length(), &r, DT_DRAWLINE);
 if(FSelected){
  canv->Font->Color=FTextColor;
  canv->Brush->Color=Color;
  canv->Font->Style.Clear();
  if((I>StartNo)&&(I<EndNo)){
   canv->Brush->Color=FSelBackGround;
   canv->Font->Color=FSelColor;
   r=TRect(0, 0, s.Length() * FChrW, FChrH);
   DrawText(canv->Handle,s.c_str(),s.Length(), &r, DT_DRAWLINE);
  }else{
   Ei=EndOffs;
   Si=StartOffs;
   if((I==StartNo)&&(I==EndNo)){
    canv->Brush->Color=FSelBackGround;
    canv->Font->Color=FSelColor;
    if(Ei>s.Length())
     ss=s.SubString(Si + 1, s.Length() - Si);
    else
     ss=s.SubString(Si + 1, Ei - Si);
    r=TRect(Si * FChrW, 0, (Si * FChrW) + (ss.Length() * FChrW), FChrH);
    DrawText(canv->Handle, ss.c_str(), ss.Length(),&r, DT_DRAWLINE);
   }else if((I==StartNo)&&(I<EndNo)){
    canv->Brush->Color=FSelBackGround;
    canv->Font->Color=FSelColor;
    ss=s.SubString(Si + 1, s.Length() - Si);
    r=TRect(Si * FChrW, 0, (Si * FChrW) + (ss.Length() * FChrW), FChrH);
    DrawText(canv->Handle, ss.c_str(), ss.Length(),&r, DT_DRAWLINE);
   }else if((I>StartNo)&&(I==EndNo)){
    canv->Brush->Color=FSelBackGround;
    canv->Font->Color=FSelColor;
    if(Ei>s.Length()) Ei=s.Length();

    ss=s.SubString(1,Ei);
    r=TRect(0, 0, ss.Length() * FChrW, FChrH);
    DrawText(canv->Handle, ss.c_str(), ss.Length(),&r, DT_DRAWLINE);
   }
  }
 }
 if(s.Length()>FMaxScrollH) FMaxScrollH=s.Length();
 UpdateScrollBars();
}
void __fastcall TEAPCustomMemo::DrawLine(Graphics::TCanvas* CanvasSupport, int I, int Y){
 int L, Ei, Si;
 TRect Rc;
 Graphics::TBitmap *LGliph=new Graphics::TBitmap;
 TCanvas *canv=LGliph->Canvas;
 canv->Brush->Color=Color;
 canv->Font=Font;
 Font->Color=FTextColor;
 if(I<FLines->Count)
  FormatLine(I,LGliph);
 else
  LGliph->Width=0;
 CanvasSupport->Draw(-(HPos*FChrW),Y,LGliph);
 L=LGliph->Width-(HPos*FChrW);
 if(L<0) L=0;
 if(FSelected){
  Ei=EndOffs;
  Si=StartOffs;
  if((I>StartNo)&&(I<EndNo)){
   CanvasSupport->Brush->Color=FSelBackGround;
   Rc=TRect(L, Y, (FMaxScrollH * FChrW) + ClientWidth - (HPos * FChrW), Y + FChrH);
   CanvasSupport->FillRect(Rc);
  }else if((I==StartNo)&&(I<EndNo)){
   if(I<FLines->Count){
    if(FLines->Strings[I].Length()-1<Si){
     Si=FLines->Strings[I].Length()-1;
     StartOffs=Si;
    }
   }
   if(LGliph->Width<1){
    CanvasSupport->Brush->Color=Color;
    Rc=TRect(-(HPos * FChrW), Y, (Si * FChrW) - (HPos * FChrW) , Y + FChrH);
    CanvasSupport->FillRect(Rc);
    L=(Si * FChrW) - (HPos * FChrW);
   }else
    L=LGliph->Width - (HPos * FChrW);
   if(LGliph->Width < (Si * FChrW)){
    CanvasSupport->Brush->Color=Color;
    Rc=TRect(L, Y, (Si * FChrW) - (HPos * FChrW), Y + FChrH);
    CanvasSupport->FillRect(Rc);
    L=(Si * FChrW) - (HPos * FChrW);
   }
   CanvasSupport->Brush->Color=FSelBackGround;
   Rc=TRect(L, Y, (FMaxScrollH * FChrW) + ClientWidth - (HPos * FChrW), Y + FChrH);
   CanvasSupport->FillRect(Rc);
  }else if((I>StartNo)&&(I==EndNo)){
   CanvasSupport->Brush->Color=FSelBackGround;
   if(LGliph->Width<1){
    Rc=TRect(-(HPos * FChrW), Y, (Ei * FChrW) - (HPos * FChrW), Y + FChrH);
    CanvasSupport->FillRect(Rc);
   }else if(LGliph->Width < (Ei * FChrW)){
    L=LGliph->Width-(HPos * FChrW);
    Rc=TRect(L, Y, (Ei * FChrW) - (HPos * FChrW), Y + FChrH);
    CanvasSupport->FillRect(Rc);
   }
   L=(Ei * FChrW) - (HPos * FChrW);
   if(LGliph->Width-(HPos * FChrW)> L)
    L=LGliph->Width-(HPos * FChrW);
    CanvasSupport->Brush->Color=Color;
    Rc=TRect(L, Y, (FMaxScrollH * FChrW) + ClientWidth - (HPos * FChrW), Y + FChrH);
    CanvasSupport->FillRect(Rc);
  }else{
   CanvasSupport->Brush->Color=Color;
   Rc=TRect(L, Y, (FMaxScrollH * FChrW) + ClientWidth - (HPos * FChrW), Y + FChrH);
   CanvasSupport->FillRect(Rc);
  }
 }else{
  CanvasSupport->Brush->Color=Color;
  TRect Rc=Rect(L, Y, (FMaxScrollH * FChrW) + ClientWidth - (HPos * FChrW), Y + FChrH);
  CanvasSupport->FillRect(Rc);
 }
 delete LGliph;
}
void __fastcall TEAPCustomMemo::DrawVisible(){
 int y=0;
 GetSelBounds(StartNo, EndNo, StartOffs, EndOffs);
 for(int i=FTopLine,endi=FTopLine+FVisLines;i<endi;i++){
  DrawLine(Canvas,i,y);
  y+=FChrH;
 }
}
TPoint __fastcall TEAPCustomMemo::GetSelBegin(){
 TPoint Result;
 Result.x=FSelStartNo;
 Result.y=FSelStartOffs;
 return Result;
}
TPoint __fastcall TEAPCustomMemo::GetSelEnd(){
 TPoint Result;
 Result.x=FSelEndNo;
 Result.y=FSelEndOffs;
 return Result;
}
void __fastcall TEAPCustomMemo::SetSelBegin(TPoint Value){
 if(Value.x>FLines->Count) Value.x=FLines->Count;
 if(Value.x<0) Value.x=0;
 if(Value.y<0) Value.y=0;
 FSelStartNo=Value.x;
 FSelStartOffs=Value.y;
 FSelected=(FSelStartNo!=FSelEndNo)||(FSelStartOffs!=FSelEndOffs);
}
void __fastcall TEAPCustomMemo::SetSelEnd(TPoint Value){
 if(Value.x>FLines->Count) Value.x=FLines->Count;
 if(Value.x<0) Value.x=0;
 if(Value.y<0) Value.y=0;
 FSelEndNo=Value.x;
 FSelEndOffs=Value.y;
 CaretPos.x=Value.x;
 CaretPos.y=Value.y;
 FSelected=(FSelStartNo!=FSelEndNo)||(FSelStartOffs!=FSelEndOffs);
 MakeCaretVisible();
}
void __fastcall TEAPCustomMemo::GetSelBounds(int &StartNo, int &EndNo, int &StartOffs, int &EndOffs){
 if(FSelStartNo<=FSelEndNo){
  StartNo=FSelStartNo;
  EndNo=FSelEndNo;
  if(!((StartNo== EndNo)&&(FSelStartOffs>FSelEndOffs))){
   StartOffs=FSelStartOffs;
   EndOffs=FSelEndOffs;
  }else{
   StartOffs=FSelEndOffs;
   EndOffs=FSelStartOffs;
  }
 }else{
  StartNo=FSelEndNo;
  EndNo=FSelStartNo;
  StartOffs=FSelEndOffs;
  EndOffs=FSelStartOffs;
 }
}
void __fastcall TEAPCustomMemo::MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X,	int Y){
 int RNo,CNo;
 inherited::MouseDown(Button, Shift, X, Y);
 if(!Focused()){
  TCustomForm *frm=GetParentForm(this);
  if(frm&&frm->ActiveControl==this)
   frm->ActiveControl=0;
  SetFocus();
 }
 ShiftState=Shift;
 if(FOnMemoEvents)
  switch(Button){
   case mbRight: FOnMemoEvents(this, Shift, k_Mouse2, 0, CaretPos, Point(X, Y)); break;
   case mbMiddle: FOnMemoEvents(this, Shift, k_Mouse3, 0, CaretPos, Point(X, Y)); break;
  }
 if(Button!=mbLeft) return;
 if(FDblClick) return;
 GetRowColAtPos(X + HPos * FChrW, Y + VPos * FChrH, RNo, CNo);
 CaretPos.x=RNo;
 CaretPos.y=CNo;
 if(FOnMemoEvents) FOnMemoEvents(this, Shift, k_Mouse1, 0, CaretPos, Point(X, Y));
 if(!Shift.Contains(ssShift)){
  if(FSelected){
    // Erase old selection, if any...
   FSelected=false;
   DrawVisible();
  }else{
   PulseCaret=true;
   UpdateTimer();
  }
  FSelStartNo=RNo;
  FSelEndNo=FSelStartNo;
  FSelStartOffs=CNo;
  FSelEndOffs=FSelStartOffs;
  FSelected=true;
  FSelMouseDwn=true;
  ScrollTimer->Enabled=true;
 }else{
  FSelEndNo=RNo;
  FSelEndOffs=CNo;
  FSelected=true;
 }
 DrawVisible();
 DrawCaret(CaretPos.x, CaretPos.y, True);
}
void __fastcall TEAPCustomMemo::MouseUp(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y){
 int RNo, CNo;
 inherited::MouseUp(Button, Shift, X, Y);
 ShiftState=Shift;
 if(FOnMemoEvents)
  switch(Button){
   case mbLeft: FOnMemoEvents(this, Shift, k_Mouse1, 0, CaretPos, Point(X, Y));break;
   case mbRight: FOnMemoEvents(this, Shift, k_Mouse2, 0, CaretPos, Point(X, Y));break;
   case mbMiddle: FOnMemoEvents(this, Shift, k_Mouse3, 0, CaretPos, Point(X, Y));break;
  }
 if(Button!=mbLeft) return;
 if(FDblClick){
  FDblClick=false;
  return;
 }
 FSelMouseDwn=false;
 ScrollTimer->Enabled=false;
 GetRowColAtPos(X + HPos * FChrW, Y + VPos * FChrH, RNo, CNo);
 if(RNo!=FSelEndNo) FSelEndNo=RNo;
 if(CNo!=FSelEndOffs) FSelEndOffs=CNo;
 FSelected=(FSelStartNo!=FSelEndNo)||(FSelStartOffs!=FSelEndOffs);
 if(!FSelected &&(FSelStartNo==0)&&(FSelStartOffs==0)){
  FSelStartNo=0;
  FSelEndNo=FSelStartNo;
  FSelStartOffs=0;
  FSelEndOffs=FSelStartOffs;
  DrawVisible();
  DrawCaret(CaretPos.x,CaretPos.y,true);
  HideCaret=true;
  PulseCaret=true;
  UpdateTimer();
 }
}
void __fastcall TEAPCustomMemo::MouseMove(Classes::TShiftState Shift, int X, int Y){
 int RNo, CNo,verY;
 inherited::MouseMove(Shift, X, Y);
 ShiftState=Shift;
 MousePos=Point(X, Y);
 if(FOnMemoEvents) FOnMemoEvents(this, Shift, k_None, 0, CaretPos, MousePos);
 VScrollDelta=0;
 HScrollDelta=0;
 if(Y<0) VScrollDelta=-1;
 if(X<0) HScrollDelta=-1;
 if(Y>ClientHeight) VScrollDelta=1;
 if(X>ClientWidth) HScrollDelta=1;
 if(Y<-FChrH) VScrollDelta=-2;
 if(Y>(ClientHeight + FChrH)) VScrollDelta=2;
 if(X<-FChrW) HScrollDelta=-2;
 if(X>(ClientWidth + FChrW)) HScrollDelta=2;
 if(FSelected && FSelMouseDwn){
  XMouse=X;
  YMouse=Y;
  verY=Y;
  if(verY<0) verY=0;
  if(verY>ClientHeight) verY=ClientHeight;
  GetRowColAtPos(X + HPos * FChrW, verY + VPos * FChrH, RNo, CNo);
  FSelEndNo=RNo;
  FSelEndOffs=CNo;
  CaretPos.x=RNo;
  CaretPos.y=CNo;
  DrawVisible();
 }
}
bool IsValidChar(char a){
// ValidChars = ['a'..'z', 'A'..'Z', '0'..'9', '#'];
 if(a>='0'&&a<='9') return true;
 if(a>='a'&&a<='z') return true;
 if(a>='A'&&a<='Z') return true;
 if(a>='à'&&a<='ÿ') return true;
 if(a>='À'&&a<='ß') return true;
 return false;
}
bool GetNextWord(AnsiString SLine, int &PosX){
 int I, RetX;
 bool FindNext;
 bool Result=false;
 if(PosX>SLine.Length()) return Result;
 FindNext=false;
 RetX=0;
 for(I=PosX;I<=SLine.Length();I++){
  if(!FindNext&&!IsValidChar(SLine[I])){
   FindNext=true;
   continue;
  }
  if(FindNext&&IsValidChar(SLine[I])){
   RetX=I;
   Result=True;
   break;
  }
 }
 if(RetX<1) Result=false;
 PosX=RetX;
 return Result;
}
void __fastcall TEAPCustomMemo::DblClick(){
 AnsiString s;
 int XB, NextW;
 inherited::DblClick();
 ShiftState=ShiftState<<ssDouble;
 if(FOnMemoEvents) FOnMemoEvents(this, ShiftState, k_None, 0, CaretPos, MousePos);
 s=GetWordAtPos(CaretPos.y, CaretPos.x, XB);
 if((s.Length())&&(XB>-1)){
  FSelStartNo=CaretPos.x;
  FSelEndNo=CaretPos.x;
  FSelStartOffs=XB;
  FSelEndOffs=XB + s.Length();
  CaretPos.y=FSelStartOffs;
  FSelected=true;
  DrawVisible();
  FDblClick=true;
 }else if(CaretPos.x<FLines->Count){
  s=FLines->Strings[CaretPos.x];
  NextW=CaretPos.y;
  if(GetNextWord(s,NextW)){
   s=GetWordAtPos(NextW, CaretPos.x, XB);
   if(s.Length()&&(XB > -1)){
    FSelStartNo=CaretPos.x;
    FSelEndNo=CaretPos.x;
    FSelStartOffs=XB;
    FSelEndOffs=XB+s.Length();
    CaretPos.y=FSelStartOffs;
    FSelected=true;
    DrawVisible();
    FDblClick=true;
   }
  }
 }
}
AnsiString __fastcall TEAPCustomMemo::GetWordAtPos(int Col, int Row, int &XBegin){
 AnsiString S;
 char C;
 int I, Si, Ei, CrX;
 XBegin=-1;
 Si=0;
 Ei=0;
 if(Row>=FLines->Count) return "";
 S=FLines->Strings[Row];
 if(!S.Length()) return "";
 if(Col>S.Length()-1)
  Col=S.Length()-1;
 if(!IsValidChar(S[Col + 1])){
  CrX=Col-1;
  for(I=CrX;I>=1;I--){
   C=S[I+1];
   if(IsValidChar(C)){
    Col=I;
    break;
   }
  }
  if(Col==0) return "";
 }
 for(I=Col + 1;I>=1;I--){
  if(IsValidChar(S[I]))
   Si=I;
  else
   break;
 }
 for(I=Col + 1;I<=S.Length();I++){
  if(IsValidChar(S[I]))
   Ei=I+1;
  else
   break;
 }
 if(Ei>=Si){
  XBegin=Si-1;
  return S.SubString(Si, Ei - Si);
 }
 return "";
}
void __fastcall TEAPCustomMemo::GetRowColAtPos(int X, int Y, int &Row, int &Col){
 Row=Y/FChrH;
 if(Row>FLines->Count) Row=FLines->Count;
 if(X<0) X= 0;
 Col=(X+FChrW/2)/FChrW;
}
void __fastcall TEAPCustomMemo::ScrollOnTimer(TObject* Sender){
 if(VScrollDelta){
  ScrollPos_V+=VScrollDelta;
  MouseMove(TShiftState(), XMouse, YMouse);
 }
 if(HScrollDelta){
  ScrollPos_H+=HScrollDelta;
  MouseMove(TShiftState(), XMouse, YMouse);
 }
}
void __fastcall TEAPCustomMemo::BlinkCaret(){
 if(ComponentState.Contains(csDesigning)) return;
 HideCaret=!HideCaret;
 if((FSelStartNo==FSelEndNo)&&(FSelStartOffs==FSelEndOffs))
  FSelected=false;
 if(FSelected)
  HideCaret=false;
// HWND h=GetFocus();
 if(!Focused())
  HideCaret=false;
 DrawCaret(CaretPos.x, CaretPos.y, HideCaret);
}
void __fastcall TEAPCustomMemo::DrawCaret(int X, int Y, bool ShowCaret){
 int Xp, Yp;
 if(ComponentState.Contains(csDesigning)) return;
 if(CaretPos.x!=oldCaretPos.x||CaretPos.y!=oldCaretPos.y){
  if(OnCaretMove) OnCaretMove(this);
  oldCaretPos=CaretPos;
  OldHideCaret=HideCaret;
 }
 if(FSelected)return;
 if((X<FTopLine)||(X>FTopLine + FVisLines)) return;
 Yp=(X-FTopLine)*FChrH;
 Xp=(Y-HPos)*FChrW;
 if((Xp<0)||(Xp>ClientWidth)) return;
 if(OldHideCaret!=HideCaret)
 {
   Canvas->Pen->Mode=pmXor;
   Canvas->Pen->Color=clWhite;
   for(int x=Xp;x<Xp+FChrW;x++)
   {
     Canvas->MoveTo(x, Yp);
     Canvas->LineTo(x, Yp + FChrH);
   }
  Canvas->Pen->Mode=pmCopy;
 }
 OldHideCaret=HideCaret;
 FSelStartNo=CaretPos.x;
 FSelStartOffs=CaretPos.y;
}
AnsiString __fastcall TEAPCustomMemo::GetSelText(){
 int StartLine, StartPos, EndLine, EndPos, I, LineI;
 AnsiString FirstPart, LastPart, SLine, Result;
 if(!FSelected) return Result;
 if(FSelStartNo>FSelEndNo){
  StartLine=FSelEndNo;
  StartPos=FSelEndOffs;
  EndLine=FSelStartNo;
  EndPos=FSelStartOffs;
 }else if((FSelStartNo==FSelEndNo)&&(FSelEndOffs<FSelStartOffs)){
  StartLine=FSelStartNo;
  StartPos=FSelEndOffs;
  EndLine=StartLine;
  EndPos=FSelStartOffs;
 }else{
  StartLine=FSelStartNo;
  StartPos=FSelStartOffs;
  EndLine=FSelEndNo;
  EndPos=FSelEndOffs;
 }
 if(StartLine>=FLines->Count) return Result;
 if(EndLine>=FLines->Count)
 {
	 EndLine=FLines->Count-1;
	 EndPos=FLines->Strings[EndLine].Length();
 }
 SLine=FLines->Strings[StartLine];
 if(StartLine<EndLine){
  FirstPart=SLine.SubString(StartPos + 1,SLine.Length()-StartPos);
  SLine=FLines->Strings[EndLine];
  if(EndPos>SLine.Length()) EndPos=SLine.Length();
  LastPart=SLine.SubString(1,EndPos);
  LineI=StartLine+1;
  Result=FirstPart;
  for(I=LineI;I<EndLine;I++)
   Result+="\r\n"+FLines->Strings[I];
  Result+="\r\n"+LastPart;
 }else
   Result=SLine.SubString(StartPos+1,EndPos-StartPos);
 return Result;
}
void __fastcall TEAPCustomMemo::SetSelText(AnsiString str){
 DeleteSelection();
 InsertTextAtPos(str,CaretPos.y,CaretPos.x);
}

void __fastcall TEAPCustomMemo::CopyToClipboard(){
 if(!FSelected) return;
 Clipboard()->SetTextBuf(GetSelText().c_str());
}
void __fastcall TEAPCustomMemo::CutToClipboard(){
 if(!FSelected) return;
 CopyToClipboard();
 if(FReadOnly) return;
 DeleteSelection();
}
void __fastcall TEAPCustomMemo::PasteFromClipboard(){
 if(FReadOnly) return;
 if(FSelected) DeleteSelection();
 InsertTextAtPos(Clipboard()->AsText, CaretPos.y, CaretPos.x);
}
void __fastcall TEAPCustomMemo::InsertTextAtPos(AnsiString S, int Col, int Row){
 if(FReadOnly) return;
 AnsiString SLine, BufS1, BufS2, BufS;
 int I, L;
 if(!S.Length()) return;
 if(Row>FLines->Count) return;
 FModified=true;
 if(Row==FLines->Count) FLines->Add("");
 SLine=FLines->Strings[Row];
 if(Col>SLine.Length()){
  L=SLine.Length();
  for(I=L;I<=Col;I++) SLine+=" ";
 }
 BufS1=SLine.SubString(1,Col);
 BufS2=SLine.SubString(Col+1,SLine.Length()-Col);
 SLine=BufS1+S+BufS2;
 FSelected=true;
 I=SLine.Pos("\r\n");
 if(I>0){
  BufS="";
  FSelStartNo=Row;
  FSelStartOffs=BufS1.Length();
  while(I>0){
   BufS=SLine.SubString(1,I-1);
   FLines->Insert(Row, BufS);
   SLine.Delete(1,I+1);
   I=SLine.Pos("\r\n");
   CaretPos.x=Row;
   CaretPos.y=BufS.Length();
   FSelEndNo=Row;
   FSelEndOffs=CaretPos.y;
   Row++;
  }
  if(SLine.Length()){
   FLines->Strings[Row]=SLine;
   CaretPos.x=Row;
   CaretPos.y=SLine.Length()-BufS2.Length();
   FSelEndNo=Row;
   FSelEndOffs=CaretPos.y;
  }else{
   CaretPos.x=Row;
   CaretPos.y=0;
   FSelEndNo=Row;
   FSelEndOffs=0;
  }
  Invalidate();
 }else{
  I=SLine.Pos("\r");
  if(I>0){
   BufS="";
   FSelStartNo=Row;
   FSelStartOffs=BufS1.Length();
   while(I>0){
    BufS=SLine.SubString(1,I-1);
    FLines->Insert(Row, BufS);
    SLine.Delete(1, I);
    I=SLine.Pos("\r");
    CaretPos.x=Row;
    CaretPos.y=BufS.Length();
    FSelEndNo=Row;
    FSelEndOffs=CaretPos.y;
    Row++;
   }
   if(SLine.Length()){
    FLines->Strings[Row]=SLine;
    CaretPos.x=Row;
    CaretPos.y=SLine.Length()-BufS2.Length();
    FSelEndNo=Row;
    FSelEndOffs=CaretPos.y;
   }
   Invalidate();
  }else{
   CaretPos.x=Row;
   FLines->Strings[Row]= SLine;
   CaretPos.y=Col+S.Length();
   FSelStartNo=Row;
   FSelEndNo=Row;
   FSelStartOffs=BufS1.Length();
   FSelEndOffs=CaretPos.y;
   Invalidate();
  }
 }
 UpdateScrollBars();
 MakeCaretVisible();
}
void __fastcall TEAPCustomMemo::DeleteSelection(){
 if(FReadOnly) return;
 AnsiString FirstPart, LastPart, SLine;
 int StartLine, StartPos, EndLine, EndPos, I, DelLine;
 if(!FSelected) return;
 if(FReadOnly) return;
 FModified=true;
 if(FSelStartNo>FSelEndNo){
  StartLine=FSelEndNo;
  StartPos=FSelEndOffs;
  EndLine=FSelStartNo;
  EndPos=FSelStartOffs;
 }else if((FSelStartNo==FSelEndNo)&&(FSelEndOffs<FSelStartOffs)){
  StartLine=FSelStartNo;
  StartPos=FSelEndOffs;
  EndLine=StartLine;
  EndPos=FSelStartOffs;
 }else{
  StartLine=FSelStartNo;
  StartPos=FSelStartOffs;
  EndLine=FSelEndNo;
  EndPos=FSelEndOffs;
 }
//  { update Undo List }
 if(StartLine>=FLines->Count) return;
 if(EndLine>=FLines->Count)
 {
	 EndLine=FLines->Count-1;
	 EndPos=FLines->Strings[EndLine].Length();
 }
 SLine=FLines->Strings[StartLine];
 FirstPart=SLine.SubString(1,StartPos);
 SLine=FLines->Strings[EndLine];
 if(EndPos>SLine.Length()) EndPos=SLine.Length();
 LastPart=SLine.SubString(EndPos + 1, SLine.Length()-EndPos);
 DelLine=StartLine+1;
 for(I=DelLine;I<=EndLine;I++) FLines->Delete(DelLine);
 FLines->Strings[StartLine]=FirstPart+LastPart;

 CaretPos.x=StartLine;
 CaretPos.y=StartPos;
 FSelected=false;
 if(StartLine==EndLine){
  I=(CaretPos.x - FTopLine)*FChrH;
  DrawLine(Canvas, CaretPos.x, I);
 }else
  DrawVisible();
 DrawCaret(CaretPos.x, CaretPos.y, true);
 UpdateScrollBars();
}
void __fastcall TEAPCustomMemo::SelectAll(){
 FSelStartNo=0;
 FSelStartOffs=0;
 FSelEndNo=Lines->Count;
 FSelEndOffs=0;
 FSelected=true;
 DrawVisible();
}
void __fastcall TEAPCustomMemo::WMKeyDown(Messages::TWMKey &Message){
 int GliphY, Y;
 bool CaretScroll;
 AnsiString SLine, AddS, SLink;
 CaretScroll=false;
 ShiftState=KeyDataToShiftState(Message.KeyData);
 if(FOnMemoEvents)
  FOnMemoEvents(this,ShiftState,k_Down,Message.CharCode,CaretPos,MousePos);
 switch(Message.CharCode){
  case VK_LEFT: case VK_RIGHT: case VK_DOWN:case VK_UP:
  case VK_HOME: case VK_END: case VK_PRIOR: case VK_NEXT:
   KeyboardCaretNav(ShiftState, Message.CharCode);
   CaretScroll=true;
   break;
  case VK_DELETE:
   if(FReadOnly) return;
   if(FSelected){
    DeleteSelection();
    return;
   }
   if(CaretPos.x>=FLines->Count) return;
   FModified=true;
   SLine=FLines->Strings[CaretPos.x];
   if(SLine.Length()>=CaretPos.y+1){
    Y=CaretPos.y+1;
    SLine.Delete(Y,1);
    FLines->Strings[CaretPos.x]=SLine;
    GliphY=(CaretPos.x-FTopLine)*FChrH;
    DrawLine(Canvas,CaretPos.x,GliphY);
   }else{
    if(CaretPos.x+1>=FLines->Count) return;
    AddS=FLines->Strings[CaretPos.x+1];
    FLines->Strings[CaretPos.x]=SLine+AddS;
    FLines->Delete(CaretPos.x+1);
    DrawVisible();
   }
   DrawCaret(CaretPos.x,CaretPos.y,true);
  break;
 }
 if(CaretScroll){
/*  if(CaretPos.y>HPos+FVisCols) ScrollPos_H=CaretPos.y-FVisCols;
  if(CaretPos.y<HPos) ScrollPos_H=CaretPos.y;
  if(CaretPos.x<FTopLine) ScrollPos_V=CaretPos.x;
  if(CaretPos.x>FTopLine+FVisLines-2) ScrollPos_V=CaretPos.x-FVisLines+2;*/
  MakeCaretVisible();
 }
// inherited::WMKeyDown(Message);
}
void __fastcall TEAPCustomMemo::WMKeyUp(Messages::TWMKey &Message){
 ShiftState=KeyDataToShiftState(Message.KeyData);
 if(FOnMemoEvents)
  FOnMemoEvents(this,ShiftState,k_Up,Message.CharCode,CaretPos,MousePos);
// inherited::WMKeyUp(Message);
}
void __fastcall TEAPCustomMemo::KeyboardCaretNav(Classes::TShiftState ShiftState, Word Direction){
 int GliphY, SaveXCaret;
 switch(Direction){
  case  VK_LEFT: //Left Arrow key is pressed.
   CaretPos.y--;
   if(CaretPos.y<0){
    if(CaretPos.x>0){
     if(CaretPos.x<FLines->Count){
      GliphY=(CaretPos.x-FTopLine)*FChrH;
      DrawLine(Canvas, CaretPos.x, GliphY);
      DrawCaret(CaretPos.x, CaretPos.y + 1,false);
      if(ShiftState.Contains(ssCtrl)&&CaretPos.x>0){
       CaretPos.x--;
       CaretPos.y=FLines->Strings[CaretPos.x].Length();
       if(FSelected){
        FSelEndNo=CaretPos.x;
        FSelEndOffs=CaretPos.y;
        DrawVisible();
       }
       return;
      }
     }
     CaretPos.x--;
     CaretPos.y=FLines->Strings[CaretPos.x].Length();
     if(!FSelected){
      GliphY=(CaretPos.x-FTopLine)*FChrH;
      DrawLine(Canvas,CaretPos.x,GliphY);
     }else
      DrawVisible();
    }else{
     CaretPos.y=0;
     GliphY=(CaretPos.x-FTopLine)*FChrH;
     if(!FSelected)
      DrawLine(Canvas,CaretPos.x,GliphY);
     else
      DrawVisible();
    }
   }else{
    GliphY=(CaretPos.x-FTopLine)*FChrH;
    DrawLine(Canvas,CaretPos.x,GliphY);
    DrawCaret(CaretPos.x,CaretPos.y,true);
   }
   if(ShiftState.Contains(ssShift)){
    if(!FSelected){
     if(CaretPos.x<FLines->Count)
      if(CaretPos.y>FLines->Strings[CaretPos.x].Length())
       CaretPos.y=FLines->Strings[CaretPos.x].Length()-1;
     FSelected=true;
     FSelStartNo=CaretPos.x;
     FSelStartOffs=CaretPos.y+1;
     FSelEndNo=CaretPos.x;
     FSelEndOffs=CaretPos.y;
    }else{
     FSelEndNo=CaretPos.x;
     FSelEndOffs=CaretPos.y;
     if(FSelEndNo<FLines->Count){
      if(FSelEndOffs>FLines->Strings[FSelEndNo].Length()){
       FSelEndOffs=FLines->Strings[FSelEndNo].Length()-1;
       CaretPos.y=FSelEndOffs;
      }
     }else{
      FSelEndOffs=0;
      CaretPos.y=0;
     }
    }
    FSelected=(FSelStartNo!=FSelEndNo)||(FSelStartOffs!=FSelEndOffs);
    DrawVisible();
    return;
   }
   if(FSelected){
    FSelected=false;
    DrawVisible();
    DrawCaret(CaretPos.x, CaretPos.y, true);
   }
   FSelStartNo=CaretPos.x;
   FSelStartOffs=CaretPos.y;
  break;
  case VK_RIGHT: //Right Arrow key is pressed.
   CaretPos.y=CaretPos.y+1;
   if(CaretPos.y>FMaxScrollH){
    FMaxScrollH=FMaxScrollH+2;
    UpdateScrollBars();
   }
   if(CaretPos.x<FLines->Count){
    GliphY=(CaretPos.x - FTopLine) * FChrH;
    DrawLine(Canvas, CaretPos.x,GliphY);
   }
   if(ShiftState.Contains(ssShift)){
    if(!FSelected){
     FSelected=true;
     FSelStartNo=CaretPos.x;
     FSelStartOffs=CaretPos.y-1;
     FSelEndNo=CaretPos.x;
     FSelEndOffs=CaretPos.y;
    }else{
     FSelEndNo=CaretPos.x;
     FSelEndOffs=CaretPos.y;
    }
    FSelected=(FSelStartNo!=FSelEndNo)||(FSelStartOffs!=FSelEndOffs);
    DrawVisible();
    return;
   }
   if(FSelected){
    FSelected=false;
    DrawVisible();
    DrawCaret(CaretPos.x, CaretPos.y,true);
   }
   DrawCaret(CaretPos.x, CaretPos.y,true);
   FSelStartNo=CaretPos.x;
   FSelStartOffs=CaretPos.y;
  break;
  case VK_UP: //Up Arrow key is pressed.
   if(CaretPos.x==0) return;
   if(!ShiftState.Contains(ssShift)){
    CaretPos.x=CaretPos.x-1;
    if(FSelected){
     FSelected=false;
     DrawVisible();
     DrawCaret(CaretPos.x, CaretPos.y, true);
     return;
    }
    FSelStartNo=CaretPos.x;
    GliphY=(CaretPos.x - FTopLine + 1) * FChrH;
    DrawLine(Canvas, CaretPos.x + 1, GliphY);
    GliphY=(CaretPos.x - FTopLine) * FChrH;
    DrawLine(Canvas, CaretPos.x, GliphY);
    DrawCaret(CaretPos.x, CaretPos.y, True);
    return;
   }else{
    CaretPos.x=CaretPos.x-1;
    if(!FSelected){
     FSelStartNo=CaretPos.x + 1;
     FSelStartOffs=CaretPos.y;
     FSelEndNo=CaretPos.x;
     FSelEndOffs=CaretPos.y;
     FSelected=true;
    }else{
     FSelEndNo=CaretPos.x;
     FSelEndOffs=CaretPos.y;
     FSelected=(FSelStartNo!=FSelEndNo)||(FSelStartOffs!=FSelEndOffs);
    }
    DrawVisible();
   }
  break;
  case VK_DOWN: //Down Arrow key is pressed.
   if(CaretPos.x>=FLines->Count) return;
   if(!ShiftState.Contains(ssShift)){
    CaretPos.x=CaretPos.x+1;
    if(FSelected){
     FSelected=false;
     DrawVisible();
     DrawCaret(CaretPos.x, CaretPos.y, true);
     return;
    }
    FSelStartNo=CaretPos.x;
    GliphY=(CaretPos.x - FTopLine - 1) * FChrH;
    DrawLine(Canvas, CaretPos.x - 1, GliphY);
    GliphY=(CaretPos.x - FTopLine) * FChrH;
    DrawLine(Canvas, CaretPos.x, GliphY);
    DrawCaret(CaretPos.x, CaretPos.y, True);
    return;
   }else{
    CaretPos.x=CaretPos.x+1;
    if(!FSelected){
     FSelStartNo=CaretPos.x - 1;
     FSelStartOffs=CaretPos.y;
     FSelEndNo=CaretPos.x;
     FSelEndOffs=CaretPos.y;
     FSelected=true;
    }else{
     FSelEndNo=CaretPos.x;
     FSelEndOffs=CaretPos.y;
     FSelected=(FSelStartNo!=FSelEndNo)||(FSelStartOffs!=FSelEndOffs);
    }
    DrawVisible();
   }
  break;
  case VK_HOME: //Home key is pressed.
   if(!ShiftState.Contains(ssShift)){
    CaretPos.y=0;
    GliphY=(CaretPos.x - FTopLine) * FChrH;
    DrawLine(Canvas, CaretPos.x, GliphY);
    DrawCaret(CaretPos.x, CaretPos.y, true);
   }else{
    DrawCaret(CaretPos.x, CaretPos.y, false);
    if(!FSelected){
     FSelStartNo=CaretPos.x;
     FSelStartOffs=CaretPos.y;
     FSelected=true;
    }
    CaretPos.y=0;
    FSelEndNo=CaretPos.x;
    FSelEndOffs=0;
    if(FSelEndNo==FSelStartNo)
     FSelected=(FSelStartOffs!=FSelEndOffs);
    DrawVisible();
   }
  break;
  case VK_END: //End key is pressed.
   if(!ShiftState.Contains(ssShift)){
    if(CaretPos.x<FLines->Count)
     CaretPos.y=FLines->Strings[CaretPos.x].Length();
    else
     CaretPos.y=0;
    GliphY=(CaretPos.x-FTopLine)*FChrH;
    DrawLine(Canvas,CaretPos.x,GliphY);
    DrawCaret(CaretPos.x,CaretPos.y,true);
   }else{
    DrawCaret(CaretPos.x, CaretPos.y,false);
    if(FSelected){
     FSelStartNo=CaretPos.x;
     if(CaretPos.x<FLines->Count)
      if(CaretPos.y>FLines->Strings[CaretPos.x].Length())
       CaretPos.y=FLines->Strings[CaretPos.x].Length();
     FSelStartOffs=CaretPos.y;
     FSelected=true;
    }
    if(CaretPos.x<FLines->Count)
     CaretPos.y=FLines->Strings[CaretPos.x].Length();
    else
     CaretPos.y=0;
    FSelEndNo=CaretPos.x;
    FSelEndOffs=CaretPos.y;
    if(FSelEndNo==FSelStartNo)
     FSelected=(FSelStartOffs!=FSelEndOffs);
    DrawVisible();
   }
  break;
  case VK_PRIOR: case VK_NEXT: //Page Up key or Page Down key is pressed.
   if(!FSelected){
    FSelStartNo=CaretPos.x;
    FSelStartOffs=CaretPos.y;
   }
   SaveXCaret=CaretPos.x-FTopLine;
   if(Direction==VK_PRIOR){
    if(VPos==0){
     DrawCaret(CaretPos.x, CaretPos.y,false);
     CaretPos.x=0;
     CaretPos.y=0;
    }else{
     Perform(WM_VSCROLL, SB_PAGEUP, 0);
     CaretPos.x=FTopLine+SaveXCaret;
    }
   }else{
    if(VPos>FLines->Count-FVisLines){
     DrawCaret(CaretPos.x, CaretPos.y,false);
     CaretPos.x=FLines->Count-1;
     CaretPos.y=FLines->Strings[CaretPos.x].Length();
    }else{
     Perform(WM_VSCROLL, SB_PAGEDOWN, 0);
     CaretPos.x=FTopLine+SaveXCaret;
    }
   }
   DrawCaret(CaretPos.x, CaretPos.y,true);
   if(ShiftState.Contains(ssShift)){
    FSelEndNo=CaretPos.x;
    FSelEndOffs=CaretPos.y;
//    if(!FSelected)
     FSelected=true;
    DrawVisible();
   }
  break;
 }
}
void __fastcall TEAPCustomMemo::KeyPress(char &Key){
 AnsiString SLine, AddS;
 int Fill, Y, GliphY, T;
 inherited::KeyPress(Key);
 if(CaretPos.x>=FLines->Count) FLines->Add("");
 if(CaretPos.x>=FLines->Count) return;
 if((Key==VK_ESCAPE)||ShiftState.Contains(ssCtrl)) return;
 if(FSelected){
  DeleteSelection();
  if(Key==8) return;
 }
 SLine=FLines->Strings[CaretPos.x];
 switch(Key){
  case 8: // Backspace key
   if(FReadOnly) return;
   FModified=true;
   if(SLine.Length()>=CaretPos.y)
    Y=CaretPos.y;
   else{
    Y=SLine.Length();
    CaretPos.y=Y;
   }
   SLine.Delete(Y,1);
   FLines->Strings[CaretPos.x]=SLine;
   CaretPos.y=CaretPos.y-1;
   if(CaretPos.y<0){
    if(CaretPos.x>0){
     AddS=FLines->Strings[CaretPos.x];
     FLines->Delete(CaretPos.x);
     CaretPos.x=CaretPos.x-1;
     CaretPos.y=FLines->Strings[CaretPos.x].Length();
     if(AddS.Length())
      FLines->Strings[CaretPos.x]=FLines->Strings[CaretPos.x]+AddS;
     DrawVisible();
    }else{
     CaretPos.y=0;
     GliphY=(CaretPos.x-FTopLine)*FChrH;
     DrawLine(Canvas,CaretPos.x,GliphY);
    }
   }else{
    GliphY=(CaretPos.x-FTopLine) * FChrH;
    DrawLine(Canvas,CaretPos.x,GliphY);
   }
   FSelStartNo=CaretPos.x;
   FSelStartOffs=CaretPos.y;
  break;
  case 13: //Return key
// FIX-ME Undo update...
   if(FReadOnly) return;
   FModified=true;
   AddS="";
   if(SLine.Length()>CaretPos.y){
    AddS=SLine.SubString(CaretPos.y + 1, SLine.Length() - CaretPos.y + 1);
    SLine.Delete(CaretPos.y+1,SLine.Length()-CaretPos.y);
    FLines->Strings[CaretPos.x]=SLine;
   }
   if(CaretPos.x==FLines->Count-1)
    FLines->Add(AddS);
   else if(CaretPos.x<FLines->Count-1)
    FLines->Insert(CaretPos.x+1,AddS);
   else if(CaretPos.x>=FLines->Count)
    FLines->Add("");
   CaretPos.x=CaretPos.x+1;
   CaretPos.y=0;
   DrawVisible();
   FSelStartNo=CaretPos.x;
   FSelStartOffs=CaretPos.y;
  break;
  default:;
// FIX-ME Undo update...
   if(unsigned(Key)<32) return;
   if(FReadOnly) return;
   FModified=true;
   if(SLine.Length()<CaretPos.y+1)
    for(Fill=SLine.Length();Fill<=CaretPos.y+1;Fill++) SLine+=" ";
   AddS=Key;
   SLine.Insert(AddS,CaretPos.y+1);
   FLines->Strings[CaretPos.x]=SLine;
   CaretPos.y=CaretPos.y+1;
   GliphY=(CaretPos.x-FTopLine)*FChrH;
   DrawLine(Canvas,CaretPos.x,GliphY);
   FSelStartNo=CaretPos.x;
   FSelStartOffs=CaretPos.y;
 }
/* if(CaretPos.y>HPos+FVisCols)
  ScrollPos_H=CaretPos.y-FVisCols;
 if(CaretPos.y<HPos)
  ScrollPos_H=CaretPos.y;
 if(CaretPos.x<FTopLine)
  ScrollPos_V=CaretPos.x;
 if(CaretPos.x>FTopLine+FVisLines-2)
  ScrollPos_V=CaretPos.x-FVisLines+2;*/

 MakeCaretVisible();
 DrawCaret(CaretPos.x, CaretPos.y,true);
}
bool __fastcall TEAPCustomMemo::FindReplaceProc(AnsiString TextToFind, TEAPFindOptions FindOptions,
 bool Backward, bool ReplaceMode, bool &ReplaceText){
 int SrcBegin, SrcEnd, I, WordPos, ScrollX, ScrollY, Fill;
 AnsiString SLine, SrcWord;
 TPoint FindPos;
 bool AllowScroll, ContinueSrc;
 bool Result=false;
 if(FindOptions.Contains(foEntireScope)){
  SrcBegin=0;
  SrcEnd=FLines->Count-1;
 }else{
  SrcBegin=CaretPos_V;
  if(Backward)
   SrcEnd=0;
  else
   SrcEnd=FLines->Count-1;
 }
 if(!FindOptions.Contains(foMatchCase))
  SrcWord=UpperCase(TextToFind);
 else
  SrcWord=TextToFind;
 if(SrcBegin<=SrcEnd){
  for(I=SrcBegin;I<=SrcEnd;I++){
   SLine=FLines->Strings[I];
   if(!FindOptions.Contains(foMatchCase)) SLine=UpperCase(SLine);
   FindPos.x=0;
   WordPos=SLine.Pos(SrcWord);
   while(WordPos>0){
    if((I==CaretPos.x)&&(WordPos<CaretPos.y)){
     for(Fill=WordPos;Fill<WordPos+SrcWord.Length();Fill++)
      SLine[Fill]='*';
     FindPos.x+=WordPos;
     WordPos=SLine.Pos(SrcWord);
     continue;
    }
    FindPos.x=WordPos;
    FindPos.y=I;
    AllowScroll=true;
    ContinueSrc=false;
    if(FindOptions.Contains(foWholeWords)){
     if(WordPos>1){
//           if (SLine[WordPos - 1] in ['a'..'z', 'A'..'Z']) then
      if(IsValidChar(SLine[WordPos-1]))
       for(Fill=WordPos;Fill<WordPos+SrcWord.Length();Fill++)
        SLine[Fill]='*';
      FindPos.x+=WordPos;
      WordPos=SLine.Pos(SrcWord);
      continue;
     }
     if(WordPos+SrcWord.Length()<=SLine.Length())
      if(IsValidChar(SLine[WordPos+SrcWord.Length()])){
       for(Fill=WordPos;Fill<WordPos+SrcWord.Length();Fill++)
        SLine[Fill]='*';
       FindPos.x+=WordPos;
       WordPos=SLine.Pos(SrcWord);
       continue;
      }
    }
    FSelStartNo=I;
    FSelEndNo=I;
    FSelStartOffs=FindPos.x-1;
    FSelEndOffs=FindPos.x+SrcWord.Length()-1;
    FSelected=true;
    CaretPos.x=I;
    CaretPos.y=FindPos.x+SrcWord.Length()-1;
    if(AllowScroll){
     ScrollX=0;
     ScrollY=FTopLine*FChrH;
     if(((FindPos.x+SrcWord.Length())*FChrW)-FChrW>ClientWidth)
      ScrollX=(FindPos.x*FChrW)-2*FChrW;
     if(I>FTopLine+FVisLines-2)
      ScrollY=I*FChrH;
     ScrollTo(ScrollX, ScrollY);
     Invalidate();
    }else
     Invalidate();
    Result=true;
    if(ReplaceMode){
     if(FOnReplaceText)
      FOnReplaceText(this, FindPos, AllowScroll, ReplaceText);
     RepPos=FindPos;
    }else
     if(FOnFindText)
      FOnFindText(this, FindPos, AllowScroll);
    for(Fill=WordPos;Fill<WordPos+SrcWord.Length();Fill++)
     SLine[Fill]='*';
    WordPos=SLine.Pos(SrcWord);
    if(!ContinueSrc) return Result;
   }
  }
 }else{
  for(I=SrcBegin;I>=SrcEnd;I--){
   SLine=FLines->Strings[I];
   if(!FindOptions.Contains(foMatchCase))
    SLine=UpperCase(SLine);
   FindPos.x=0;
   WordPos=SLine.Pos(SrcWord);
   while(WordPos>0){
    if((I==CaretPos.x)&&(WordPos<CaretPos.y)){
     for(Fill=WordPos;Fill<WordPos+SrcWord.Length();Fill++)
      SLine[Fill]='*';
     FindPos.x+=WordPos;
     WordPos=SLine.Pos(SrcWord);
     continue;
    }
    FindPos.x=WordPos;
    FindPos.y=I;
    AllowScroll=true;
    ContinueSrc=false;
    if(FindOptions.Contains(foWholeWords)){
     if(WordPos>1)
      if(IsValidChar(SLine[WordPos - 1])){
       for(Fill=WordPos;Fill<WordPos+SrcWord.Length();Fill++)
        SLine[Fill]='*';
       FindPos.x+=WordPos;
       WordPos=SLine.Pos(SrcWord);
       continue;
      }
     if(WordPos+SrcWord.Length()<=SLine.Length())
      if(IsValidChar(SLine[WordPos+SrcWord.Length()])){
       for(Fill=WordPos;Fill<WordPos+SrcWord.Length();Fill++)
        SLine[Fill]='*';
       FindPos.x+=WordPos;
       WordPos=SLine.Pos(SrcWord);
       continue;
      }
    }
    FSelStartNo=I;
    FSelEndNo=I;
    FSelStartOffs=FindPos.x-1;
    FSelEndOffs=FindPos.x+SrcWord.Length()-1;
    FSelected=true;
    CaretPos.x=I;
    CaretPos.y=FindPos.x+SrcWord.Length()-1;
    if(AllowScroll){
     ScrollX=0;
     ScrollY=FTopLine*FChrH;
     if(((FindPos.x+SrcWord.Length())*FChrW)-FChrW>ClientWidth)
      ScrollX=(FindPos.x*FChrW)-2*FChrW;
     if(I>FTopLine+FVisLines-2)
      ScrollY=I*FChrH;
     ScrollTo(ScrollX,ScrollY);
     Invalidate();
    }else
     Invalidate();
    Result=true;
    if(ReplaceMode){
     if(FOnReplaceText)
      FOnReplaceText(this,FindPos,AllowScroll,ReplaceText);
     RepPos=FindPos;
    }else
     if(FOnFindText)
      FOnFindText(this, FindPos, AllowScroll);
    for(Fill=WordPos;Fill<WordPos+SrcWord.Length();Fill++)
     SLine[Fill]='*';
    WordPos=SLine.Pos(SrcWord);
    if(!ContinueSrc)
     return Result;
   }
  }
 }
 return Result;
}
void __fastcall TEAPCustomMemo::FindText(AnsiString TextToFind, TEAPFindOptions FindOptions, bool Backward){
 bool Rep, SrcRes;
 SrcRes=FindReplaceProc(TextToFind, FindOptions, Backward, false, Rep);
 if(FOnSerachEnd) FOnSerachEnd(this, SrcRes, false);
}
void __fastcall TEAPCustomMemo::ReplaceText(AnsiString TextToFind, AnsiString TextToReplace, TEAPFindOptions FindOptions, bool Backward){
 bool Rep, SrcRes;
 Rep=true;
 SrcRes=FindReplaceProc(TextToFind, FindOptions, Backward, true, Rep);
 if(SrcRes&&Rep){
  DeleteSelection();
  InsertTextAtPos(TextToReplace, CaretPos.y, CaretPos.x);
 }
 if(FOnSerachEnd) FOnSerachEnd(this, SrcRes, true);
}
void __fastcall TEAPCustomMemo::Clear(void){
 CaretPos.x=0;
 CaretPos.y=0;
 ScrollTo(0,0);
 FSelStartNo=0;
 FSelStartOffs=0;
 FSelEndNo=0;
 FSelEndOffs=0;
 FLines->Clear();
 FSelected=false;
 Invalidate();
 FModified=false;
}
void __fastcall TEAPCustomMemo::SaveToFile(const AnsiString FileName){
 TStrings *BuffList=new TStringList;
 bool usecopy=true;
 if(FLines->Count>100000)
 {
   delete BuffList;
   BuffList=FLines;
   usecopy=false;
 }else
   BuffList->Assign(FLines);
 for(int i=0,endi=BuffList->Count;i<endi;i++){
  AnsiString SLine=BuffList->Strings[i];
  int p=SLine.Length();
  if(!p) continue;
  char *s=SLine.c_str()+p-1;
  while(p>0&&*s==' '){
   s--;
   p--;
  }
  if(p==SLine.Length())
   continue;
  SLine.SetLength(p);
  BuffList->Strings[i]=SLine;
 }
 BuffList->SaveToFile(FileName);
 if(usecopy) delete BuffList;
 FModified=false;
}
void __fastcall TEAPCustomMemo::LoadFromFile(const AnsiString FileName){
 if(!FileExists(FileName))
  return;
 Clear();
 FLines->LoadFromFile(FileName);
 Invalidate();
 FModified=false;
}

void TEAPCustomMemo::MakeCaretVisible()
{
 if(CaretPos.y>HPos+FVisCols)
  ScrollPos_H=CaretPos.y-FVisCols;
 if(CaretPos.y<HPos)
  ScrollPos_H=CaretPos.y;
 if(CaretPos.x<FTopLine)
  ScrollPos_V=CaretPos.x;
 if(CaretPos.x>FTopLine+FVisLines-2-VisLinesAlterCaret)
  ScrollPos_V=CaretPos.x-FVisLines+2+VisLinesAlterCaret;
}



int __fastcall TEAPCustomMemo::GetThumbtrack_V()
{
 TScrollInfo ScrollInfo;
 ScrollInfo.cbSize=sizeof(ScrollInfo);
 ScrollInfo.fMask=SIF_TRACKPOS;
 GetScrollInfo(Handle, SB_VERT,&ScrollInfo);
 return ScrollInfo.nTrackPos;
}

