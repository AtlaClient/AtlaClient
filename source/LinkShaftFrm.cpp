//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "region.h"
#include "reports.h"
#include "LinkShaftFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLinkShaftForm *LinkShaftForm;
//---------------------------------------------------------------------------
__fastcall TLinkShaftForm::TLinkShaftForm(TComponent* Owner,AObject *_obj)
    : TForm(Owner),obj(_obj)
{
}
void __fastcall TLinkShaftForm::FormCreate(TObject *Sender)
{
 AnsiString str;
 ARegion *basereg=obj->basereg;
 int zloc=basereg->zloc;
 List->Items->BeginUpdate();
 AnsiString indexstr;
 foreach(curRegionList){
  ARegion *reg=*(ARegion**)iter;
  if(reg->zloc==zloc)continue;
  foreach(reg)
  {
    AObject *obj2=*(AObject**)iter;
    if(!obj2->inner) continue;
    str=reg->FullName(false)+". "+obj2->num;
    if(obj2->innerloc.xloc!=-1)str+=" (linked)";
    List->Items->AddObject(str,(TObject*)obj2);
    if(obj->innerloc==*reg
     &&obj2->innerloc==*basereg)
      indexstr=str;
  }
 }
 List->Items->EndUpdate();
 List->ItemIndex=List->Items->IndexOf(indexstr);
}
void __fastcall TLinkShaftForm::bnLinkClick(TObject *Sender)
{
 int ind=List->ItemIndex;
 if(ind<0)return;
 AObject *obj2=(AObject*)List->Items->Objects[ind];
 ARegion *reg2=obj2->basereg,*reg;
 reg=obj->basereg;
 if(obj->innerloc.xloc!=-1)
 {
   if(obj->innerloc==*reg2)
   {
     ModalResult=mrOk;
     return;
   }
   ARegion *reg2=curRegionList->Get(obj->innerloc);
   if(reg2) foreach(reg2)
   {
     AObject *obj2=*(AObject**)iter;
     if(!obj2->inner) continue;
     if(!(obj2->innerloc==*reg)) continue;
     obj2->innerloc.xloc=-1;
     break;
   }
 }
 obj->innerloc.xloc=-1;
 if(obj2->innerloc.xloc!=-1)
 {
   ARegion *reg3=curRegionList->Get(obj2->innerloc);
   if(reg3) foreach(reg3)
   {
     AObject *obj3=*(AObject**)iter;
     if(!obj3->inner) continue;
     if(!(obj3->innerloc==*reg2)) continue;
     obj3->innerloc.xloc=-1;
     break;
   }
 }
 obj->innerloc=*reg2;
 obj2->innerloc=*reg;

 ModalResult=mrOk;
}
void __fastcall TLinkShaftForm::bnAnalyzeClick(TObject *Sender)
{
  int lastzloc=obj->basereg->zloc;
  for(int i=List->Items->Count-1;i>=0;i--)
  {
    AObject *obj=(AObject*)List->Items->Objects[i];
    ARegion *reg=obj->basereg;
    if(reg->zloc!=lastzloc)
      CalcXY(lastzloc=reg->zloc);
    bool okx=false,oky=false;
    for(int i=0;i<(int)xx.size();i++)
    {
      if(reg->xloc!=xx[i]) continue;
      okx=true;
      break;
    }
    for(int i=0;i<(int)yy.size();i++)
    {
      if(reg->yloc!=yy[i]) continue;
      oky=true;
      break;
    }
    bool ok=okx&&oky;
    if(!ok)
      List->Items->Delete(i);
  }
}
//---------------------------------------------------------------------------
static bool PassKoord(int x1,int x2,int xto,int sto)
{
  for(;x1<=x2;x1++)
  {
    int x=(x1+sto)%sto;
    if(xto==x)
      return true;
  }
  return false;
}
//---------------------------------------------------------------------------
void TLinkShaftForm::CalcXY(int levelto)
{
  ARegion *basereg=obj->basereg;
  int levelfrom=basereg->zloc;
  ARegionArray *rafrom=curRegionList->GetRegionArray(levelfrom);
  ARegionArray *rato=curRegionList->GetRegionArray(levelto);
  int x,y,dx,dy;
  xx.clear();
  yy.clear();
  if(rato->x>=rafrom->x)
  {
    x=basereg->xloc*rato->x/rafrom->x;
    dx=rato->x/rafrom->x-1;
    for(int _dx=0;_dx<=dx;_dx++)
    {
      int _x=(x+_dx+rato->x)%rato->x;
      xx.push_back(_x);
    }
  }
  if(rato->x<=rafrom->x)
  {
    dx=rafrom->x/rato->x-1;
    for(int _x=0;_x<rato->x;_x++)
    {
      x=_x*rafrom->x/rato->x;
      if(!PassKoord(x,x+dx,basereg->xloc,rafrom->x))
        continue;
      xx.push_back(_x);
    }
  }
  if(rato->y>=rafrom->y)
  {
    y=basereg->yloc*rato->y/rafrom->y;
    dy=rato->y/rafrom->y;
    for(int _dy=0;_dy<=dy;_dy++)
    {
      int _y=(y+_dy+rato->y)%rato->y;
      yy.push_back(_y);
    }
  }
  if(rato->y<=rafrom->y)
  {
    dy=rafrom->y/rato->y;
    for(int _y=0;_y<rato->y;_y++)
    {
      y=_y*rafrom->y/rato->y;
      if(!PassKoord(y,y+dy,basereg->yloc,rafrom->y))
        continue;
      yy.push_back(_y);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TLinkShaftForm::bnGotoClick(TObject *Sender)
{
  int ind=List->ItemIndex;
  if(ind<0)return;
  AObject *obj2=(AObject*)List->Items->Objects[ind];
  ARegion *reg2=obj2->basereg;
  GotoLoc=*reg2;
  ModalResult=mrYes;
}
//---------------------------------------------------------------------------

