//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdlib.h>
#include "util.h"
#include "orders.h"
#include "EditOrderEnterFrm.h"
#include "region.h"
#include "unit.h"
#include "faction.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditOrderEnterForm *EditOrderEnterForm;
AnsiString noinfostr="-/-";
//---------------------------------------------------------------------------
__fastcall TEditOrderEnterForm::TEditOrderEnterForm(TComponent* Owner, AOrderInt *_ord)
    : TForm(Owner),ord(_ord)
{
 cbCommented->Checked=ord->commented;
 cbRepeating->Checked=ord->repeating;

 Grid->Cells[0][0]="Name";
 Grid->Cells[1][0]="#";
 Grid->Cells[2][0]="Type";
 Grid->Cells[3][0]="Owner Faction";
 Grid->Cells[4][0]="Protection";
 Grid->Cells[5][0]="Capacity";
 blockedrow=0;
 int row=0;
 int currow=0;
 AUnit *un=_ord->orders->unit;
 ARegion *reg=un->baseobj->basereg;
 int curobjnum=un->baseobj->num;
 int unitweight;
 int unitmens;
 {
	 AnsiString str;
	 str=un->FullName(false);
	 unitweight=un->weight;
	 unitmens=un->GetMen();
	 str+=", weight ";
	 str+=unitweight;
	 str+=", ";
	 str+=unitmens;
	 str+=" men";
	 labUnit->Caption=str;
 }

 foreach(reg)
 {
	 AObject *obj=*(AObject**)iter;
	 if(!obj->num) continue;
	 AObjectType *otype=ObjectTypes->Get(obj->type);
	 if(!otype||!otype->canenter) continue;
	 row++;
	 if(obj->num==curobjnum)
		 blockedrow=row;
	 Grid->RowCount=row+1;
	 if(obj->num==_ord->value)
		 currow=row;
	 Grid->Cells[0][row]=obj->name;
	 Grid->Cells[1][row]=obj->num;
	 Grid->Cells[2][row]=obj->type;
	 AnsiString ownerstr;
	 AUnit *owner=obj->owner;
	 if(owner)
		 ownerstr=owner->endfaction->FullName();
	 Grid->Cells[3][row]=ownerstr;
	 int usedmens=0;
	 int usedweight=0;
	 foreach(reg)
	 {
		 AObject *_obj=*(AObject**)iter;
		 foreach(_obj->units)
		 {
			 AUnit *un=*(AUnit**)iter;
			 if(un->endobj!=obj) continue;
			 usedmens+=un->GetMen();
			 usedweight+=un->weight;
		 }
	 }
	 if(!_ord->commented&&obj->num==_ord->value)
	 {
		 usedmens-=unitmens;
		 usedweight-=unitweight;
	 }
	 AnsiString mensstr;
	 AnsiString weightstr;
	 if(otype->protect)
	 {
		 mensstr=usedmens;
		 mensstr+="(";
		 mensstr+=usedmens+unitmens;
		 mensstr+=")/";
		 mensstr+=otype->protect;
	 }else
		 mensstr=noinfostr;
	 if(otype->capacity)
	 {
		 weightstr=usedweight;
		 weightstr+="(";
		 weightstr+=usedweight+unitweight;
		 weightstr+=")/";
		 weightstr+=otype->capacity;
	 }else
		 weightstr=noinfostr;
	 Grid->Cells[4][row]=mensstr;
	 Grid->Cells[5][row]=weightstr;
 }
 if(!row)
 {
	 Grid->RowCount=2;
	 for(int i=0;i<Grid->ColCount;i++)
		 Grid->Cells[i][0]=EmptyStr;
	 blockedrow=1;	 
 }
 if(currow)
 {
	 Grid->Row=currow;
	 Grid->TopRow=currow;
 }
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderEnterForm::bnOkClick(TObject *Sender)
{
 ord->commented=cbCommented->Checked;
 ord->repeating=cbRepeating->Checked;

 int r=Grid->Row;
 if(r==blockedrow) return;
 int ind=atoi(Grid->Cells[1][r].c_str());
 if(ind<=0) return;
 ord->value=ind;
 ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderEnterForm::bnCancelClick(TObject *Sender)
{
 ord=0;
}
//---------------------------------------------------------------------------
void __fastcall TEditOrderEnterForm::GridDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
 if(ARow!=blockedrow)
   return;
 AnsiString str=Grid->Cells[ACol][ARow];
 TCanvas *Canv=Grid->Canvas;
 Canv->Brush->Color=clBtnFace;
 Canv->TextRect(Rect, Rect.Left+2, Rect.Top+2, str);
}
//---------------------------------------------------------------------------

