//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdlib.h>
#include "util.h"
#include "MapFrm.h"
#include "region.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMapForm *MapForm;
static AnsiString IniSect="MapWin";
//---------------------------------------------------------------------------
__fastcall TMapForm::TMapForm(TComponent* Owner)
        : TForm(Owner),oldzloc(-1),bmp(new Graphics::TBitmap)
        ,moving(false),docktype(0)
{
 TIniFile *ini=new TIniFile(IniName);
 Left=atoi(ini->ReadString(IniSect,"Left",Left).c_str());
 Top=atoi(ini->ReadString(IniSect,"Top",Top).c_str());
 docktype=atoi(ini->ReadString(IniSect,"DockType",docktype).c_str());
 delete ini;
 DoubleBuffered=true;
}
//---------------------------------------------------------------------------
void __fastcall TMapForm::FormDestroy(TObject *Sender)
{
 TIniFile *ini=new TIniFile(IniName);
 ini->WriteString(IniSect,"Left",Left);
 ini->WriteString(IniSect,"Top",Top);
 ini->WriteString(IniSect,"DockType",docktype);
 delete ini;
 delete bmp;
 MapForm=0;
}
//---------------------------------------------------------------------------
void TMapForm::UpdateMap(int zloc)
{
 if(!ShowMap) return;
 if(zloc==oldzloc) return;
 CreateMap(zloc);
}
//---------------------------------------------------------------------------
void TMapForm::ResetMap()
{
 oldzloc=-1;
}
//---------------------------------------------------------------------------
void TMapForm::CreateMap(int zloc)
{
 ARegionArray *ra=curRegionList->RegionArrays->Get(zloc);
 if(!ra) return;
 oldzloc=zloc;
 int ex=ra->x,ey=ra->y;
 if(zloc==0||ex==1){
  Hide();
  return;
 }
// Caption="Map "+ra->name;
 FMul=2;
 if(ex*FMul<120) FMul*=2;
 int mul=FMul;
 int w=ex*mul,h=(ey+1)*(mul/2);

 bmp->Width=w;
 bmp->Height=h;
 ClientWidth=w+2;
 ClientHeight=h+2;
 TCanvas *canv=bmp->Canvas;
 TRect r(0,0,w+mul,h+mul);
 canv->Brush->Color=clWhite;
 canv->FillRect(r);
 ARegion **beg,**end;
 beg=(ARegion**)curRegionList->List;
 end=beg+curRegionList->count;
 while(beg<end&&(*beg)->zloc!=zloc)
   beg++;
 while(beg<end&&(*beg)->zloc==zloc)
 {
   ARegion *reg=*beg++;
   ATerrainType *ttype=TerrainTypes->Get(reg->type);
   if(!ttype) return;
   r.right=(r.left=reg->xloc*mul)+mul;
   r.bottom=(r.top=reg->yloc*(mul/2))+mul;
   canv->Brush->Color=ttype->color;
   canv->FillRect(r);
 }
 ReCalcRect();
 ApplyDockType();
 if(!Visible) Show();
}
//---------------------------------------------------------------------------
void __fastcall TMapForm::SetShowMap(bool value)
{
 if(FShowMap== value) return;
 FShowMap = value;
 if(value){
  if(curRegion)
   UpdateMap(curRegion->zloc);
 }else{
  ResetMap();
  Hide();
 }
}
//---------------------------------------------------------------------------
void __fastcall TMapForm::ImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if(Button==mbRight)
 {
   moving=true;
   movestart=ClientToScreen(TPoint(X,Y));
   Mouse->Capture=Handle;
   ImageMouseMove(Sender,Shift,X,Y);
   return;
 }
 int xloc,yloc;
 PointToCoord(X,Y,xloc,yloc);
 ARegionArray *ra=curRegionList->RegionArrays->Get(oldzloc);
 if(!ra) return;
 ra->NormCoord(xloc,yloc);
 AnsiString mes;
 AtlaForm->xcentr=xloc;
 AtlaForm->ycentr=yloc;
 ReCalcRect();
 AtlaForm->DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TMapForm::ImageMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  if(Button!=mbRight||!moving) return;
  ImageMouseMove(Sender,Shift,X,Y);
  moving=false;
  Mouse->Capture=0;
  ApplyDockType();
}
//---------------------------------------------------------------------------
void TMapForm::PointToCoord(int x, int y, int & xloc, int& yloc)
{
 int mul=FMul;
 xloc=x/mul;
 yloc=y/(mul/2);
}
//---------------------------------------------------------------------------
void __fastcall TMapForm::SetMul(int value)
{
  FMul = value;
}
//---------------------------------------------------------------------------
void __fastcall TMapForm::FormClose(TObject *Sender, TCloseAction &Action)
{
 AtlaForm->actShowMap->Checked=false;
 ShowMap=false;
 Action=caHide;
}
//---------------------------------------------------------------------------
void __fastcall TMapForm::ImageMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
 if(moving)
 {
   TPoint p=ClientToScreen(TPoint(X,Y));
   Left=Left+p.x-movestart.x;
   Top=Top+p.y-movestart.y;
   movestart=p;
   TPoint lt=AtlaForm->PanelMap->ClientToScreen(TPoint(0,0));
   TPoint rb=AtlaForm->PanelMap->ClientToScreen(
     TPoint(AtlaForm->PanelMap->Width,AtlaForm->PanelMap->Height));
   docktype=0;
   if(abs(lt.x-Left)<10)
     docktype|=1;
   if(abs(lt.y-Top)<10)
     docktype|=2;
   if(abs(rb.x-Left-Width)<10)
     docktype|=4;
   if(abs(rb.y-Top-Height)<10)
     docktype|=8;
   Invalidate();
   return;
 }
 int xloc,yloc;
 PointToCoord(X,Y,xloc,yloc);
 AtlaForm->SetlbCoords(xloc,yloc); 
}
//---------------------------------------------------------------------------
void __fastcall TMapForm::FormPaint(TObject *Sender)
{
  Canvas->Brush->Style=bsClear;
//  Canvas->Pen->Color=clActiveBorder;
//  Canvas->Rectangle(BoundsRect);
  Canvas->Draw(0,0,bmp);
  Canvas->Pen->Color=clRed;
  Canvas->Rectangle(currect);
  if(!moving) return;
  TRect r=ClientRect;
  if(docktype&1) //left
  {
    int y=(r.top+r.bottom)/2;
    int x=(r.left+r.right)/8;
    Canvas->MoveTo(x,3*y/4);
    Canvas->LineTo(0,y);
    Canvas->LineTo(x,5*y/4);
  }
  if(docktype&2) //top
  {
    int y=(r.top+r.bottom)/8;
    int x=(r.left+r.right)/2;
    Canvas->MoveTo(3*x/4,y);
    Canvas->LineTo(x,0);
    Canvas->LineTo(5*x/4,y);
  }
  if(docktype&4) //right
  {
    int y=(r.top+r.bottom)/2;
    int x=r.right-(r.left+r.right)/8;
    Canvas->MoveTo(x,3*y/4);
    Canvas->LineTo(r.right,y);
    Canvas->LineTo(x,5*y/4);
  }
  if(docktype&8)
  {
    int y=r.bottom-(r.top+r.bottom)/8;
    int x=(r.left+r.right)/2;
    Canvas->MoveTo(3*x/4,y);
    Canvas->LineTo(x,r.bottom);
    Canvas->LineTo(5*x/4,y);
  }
}
//---------------------------------------------------------------------------
void TMapForm::ReCalcRect()
{
  int xc=AtlaForm->ImageMap->Width/2,yc=AtlaForm->ImageMap->Height/2;
  int xloc=AtlaForm->xcentr,yloc=AtlaForm->ycentr;
  int bx=xloc-xc/MapDX,by=yloc-yc/MapDY;
  int ex=xloc+xc/MapDX+1,ey=yloc+yc/MapDY+1;
  int mul=FMul;
  currect.left=bx*mul-1;
  currect.top=by*mul/2-1;
  currect.right=ex*mul+1;
  currect.bottom=ey*mul/2+1;
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMapForm::CreateParams(Controls::TCreateParams &Params)
{
  TCustomForm::CreateParams(Params);
  Params.Style|=WS_BORDER;
}
//---------------------------------------------------------------------------
void TMapForm::ApplyDockType()
{
  TPoint lt=AtlaForm->PanelMap->ClientToScreen(TPoint(0,0));
  TPoint rb=AtlaForm->PanelMap->ClientToScreen(
    TPoint(AtlaForm->PanelMap->Width,AtlaForm->PanelMap->Height));
  if(docktype&1)
  {
    docktype&=~4;
    Left=lt.x;
  }
  if(docktype&2)
  {
    docktype&=~8;
    Top=lt.y;
  }
  if(docktype&4)
  {
    Left=rb.x-Width;
  }
  if(docktype&8)
  {
    Top=rb.y-Height;
  }
}
//---------------------------------------------------------------------------

