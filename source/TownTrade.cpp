//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "TownTrade.h"
#include "util.h"
#include "region.h"
#include "markets.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfTownTrade *fTownTrade;
//---------------------------------------------------------------------------
__fastcall TfTownTrade::TfTownTrade(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfTownTrade::FormCreate(TObject *Sender)
{
  if(!ItemTypes)return;
  TStringList*list = new TStringList;
  ItemTypes->MakeItemList(list, AItemType::IT_MONSTER|AItemType::IT_MAN);
  list->Sort();

  cbItems->Items->Clear();
  cbItems->Items->AddStrings(list);
  delete list;

  grSale->Cells[0][0] = "Town";
  grSale->Cells[1][0] = "num";
  grSale->Cells[2][0] = "price";
  grWanted->Cells[0][0] = "Town";
  grWanted->Cells[1][0] = "num";
  grWanted->Cells[2][0] = "price";
}
//---------------------------------------------------------------------------

int CoordsToInt(int x, int y, int z){
  return x+y*1000+z*1000*1000;
}

void IntToCoords(int key,int *x, int *y, int *z)
{
  if(x)
    *x=key%1000;
  key/=1000;
  if(y)
    *y=key%1000;
  if(z)
    *z=key/1000;
}

void __fastcall TfTownTrade::cbItemsChange(TObject *Sender)
{
  grSale->RowCount = 1;
  grWanted->RowCount = 1;
  grSale->Cells[0][1] = ""; grSale->Cells[1][1] = ""; grSale->Cells[2][1] = "";
  grWanted->Cells[0][1] = ""; grWanted->Cells[2][1] = ""; grWanted->Cells[2][1] = "";
  AnsiString str = ItemTypes->NameToAbr(cbItems->Text);
  foreach(curRegionList){
    ARegion *reg = *(ARegion**)iter;
    ATownType *tt = TownTypes->Get(reg->towntype);
    if(!tt->name.Length()) continue;

    foreach(reg->saleds)
    {
      AMarket *mark = *(AMarket**)iter;
      if(CompareIStr(mark->type->abr, str)) continue;
	  grSale->RowCount++;
	  grSale->Cells[0][grSale->RowCount-1] = reg->townname + reg->GetLocation();;
	  grSale->Cells[1][grSale->RowCount-1] = mark->amount;
	  grSale->Cells[2][grSale->RowCount-1] = mark->price;
	  int key = CoordsToInt(reg->xloc, reg->yloc, reg->zloc);
	  grSale->Objects[0][grSale->RowCount-1] = ((TObject*)key);
	}
	foreach(reg->wanteds)
	{
	  AMarket *mark = *(AMarket**)iter;
	  if(CompareIStr(mark->type->abr, str)) continue;
	  grWanted->RowCount++;
	  grWanted->Cells[0][grWanted->RowCount-1] = reg->townname + reg->GetLocation();
	  grWanted->Cells[1][grWanted->RowCount-1] = mark->amount;
	  grWanted->Cells[2][grWanted->RowCount-1] = mark->price;
	  int key = CoordsToInt(reg->xloc, reg->yloc, reg->zloc);
	  grWanted->Objects[0][grWanted->RowCount-1] = ((TObject*)key);
    }
  }
  if(grSale->RowCount < 2) grSale->RowCount = 2;
  if(grWanted->RowCount < 2) grWanted->RowCount = 2;
  grSale->FixedRows = 1;
  grWanted->FixedRows = 1;
  grSale->Row = 1;
  grWanted->Row = 1;

  UpdateInfo();
}
//---------------------------------------------------------------------------
void __fastcall TfTownTrade::grSaleDblClick(TObject *Sender)
{
  if(grSale->Row < 1) return;
  if(grSale->Cells[0][grSale->Row] == "") return;
  int x,y,z;
  int key = (int)grSale->Objects[0][grSale->Row];
  IntToCoords(key, &x, &y, &z);
  ARegion *reg = curRegionList->Get(x, y, z);
  if(!reg) return;
  AtlaForm->GotoRegion(reg);
}
//---------------------------------------------------------------------------
void __fastcall TfTownTrade::grWantedDblClick(TObject *Sender)
{
  if(grWanted->Row < 1) return;
  if(grWanted->Cells[0][grWanted->Row] == "") return;
  int x,y,z;
  int key = (int)grWanted->Objects[0][grWanted->Row];
  IntToCoords(key, &x, &y, &z);
  ARegion *reg = curRegionList->Get(x, y, z);
  if(!reg) return;
  AtlaForm->GotoRegion(reg);
}
//---------------------------------------------------------------------------
void __fastcall TfTownTrade::grSaleSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
  UpdateInfo();
}

void TfTownTrade::UpdateInfo()
{
  lb1->Caption = "";
  lb2->Caption = "";
  if(grSale->Row < 1) return;
  if(grSale->Cells[0][grSale->Row] == "") return;
  if(grWanted->Row < 1) return;
  if(grWanted->Cells[0][grWanted->Row] == "") return;
  AnsiString str = grSale->Cells[0][grSale->Row];
  str += " --> ";
  str += grWanted->Cells[0][grWanted->Row];
  lb1->Caption = str;

  int n1 = grSale->Cells[1][grSale->Row].ToInt(), n2 = grWanted->Cells[1][grWanted->Row].ToInt();
  int p1 = grSale->Cells[2][grSale->Row].ToInt(), p2 = grWanted->Cells[2][grWanted->Row].ToInt();
  n1 = min(n1, n2);
  str = n1;
  str += " * ( ";
  str += p2;
  str += " - ";
  str += p1;
  str += " ) = ";
  n2 = n1 * (p2 - p1);
  str += n2;

  int x, y, z;
  int key = (int)grSale->Objects[0][grSale->Row];
  IntToCoords(key, &x, &y, &z);
  ARegion *reg = curRegionList->Get(x, y, z);
  if(reg)
  {
	key = (int)grWanted->Objects[0][grWanted->Row];
	IntToCoords(key, &x, &y, &z);
	key = reg->GetDistance(x, y, z);
	if(key < 10000000)
	{
	  str += ". Distance=";
	  str += key;
	}
  }
  lb2->Caption = str;
  if(n2 < 0) Panel2->Font->Color = clRed; else Panel2->Font->Color = clGreen;
}
//---------------------------------------------------------------------------
void __fastcall TfTownTrade::grWantedClick(TObject *Sender)
{
  UpdateInfo();
}
//---------------------------------------------------------------------------
void __fastcall TfTownTrade::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TfTownTrade::FormDestroy(TObject *Sender)
{
  fTownTrade = 0;        
}
//---------------------------------------------------------------------------
