//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "orders.h"
#include "EditTurnOrder.h"
#include "EditOrdersFrm.h"
#include "unit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfEditTurnOrder *fEditTurnOrder;
//---------------------------------------------------------------------------
//
__fastcall TfEditTurnOrder::TfEditTurnOrder(TComponent* Owner, AOrders *orders)
		: TForm(Owner)
{
  Forders = new AOrders(0);
  FSaveOrders = new AOrders(0);
  foreach(orders){
	AOrder *ord = *(AOrder**)iter;

	AOrder *ord1 = ParseOrder(ord->WriteTemplate());
	if(!ord1) continue;
	ord1->turnorder = true;
	Forders->Add(ord1);
	ord1 = ParseOrder(ord->WriteTemplate());
	ord1->turnorder = true;
	FSaveOrders->Add(ord1);
  }
  UpdateOrders();
}
//---------------------------------------------------------------------------
void TfEditTurnOrder::UpdateOrders()
{
  Memo1->Clear();
  foreach(Forders)
  {
	AOrder *ord = *(AOrder**)iter;
	Memo1->Lines->Add(ord->WriteTemplate());
  }
}
//---------------------------------------------------------------------------

void __fastcall TfEditTurnOrder::EditOrders1Click(TObject *Sender)
{
  TEditOrdersForm *frm = new TEditOrdersForm(this);
  TStrings *list = frm->Editor->Lines;
  AnsiString s;
  foreach(Forders){
	AOrder *ord=*(AOrder**) iter;
	s = ord->WriteTemplate();
	int pos,oldpos=0;
	while((pos = s.SubString(oldpos+1,s.Length()).Pos("\n"))!=0)
	{
	  pos += oldpos;
	  s.Insert("\r",pos);
	  oldpos = pos+1;
	}
	if(!s.Length())continue;
	list->Add(s);
  }

  if(frm->ShowModal()==mrOk)
  {
	AOrder *ord;
	Forders->Clear();
	for(int i=0,endi=list->Count;i<endi;i++)
	{
	  s = list->Strings[i];
	  if(!s.Length()) continue;
	  ord = ParseOrder(s);
	  if(!ord)
	  {
		AddEvent("Error parsing order: " + s);
		continue;
	  }

	  if(ord->type==O_END||ord->type==O_UNIT||O_ENDTURN||O_TURN)
	  {
		AddEvent("This order cannot be used here: "+s);
		continue;
	  }
	  Forders->Add(ord);
	} // for
	UpdateOrders();
 }
 delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TfEditTurnOrder::FormCreate(TObject *Sender)
{
  miAddOrderCustom->Tag       = O_CUSTOMORDER;
  miAddOrderGive->Tag         = O_GIVE;
  miAddOrderCast->Tag         = O_CAST;
  miAddOrderTeach->Tag        = O_TEACH;
  miAddOrderBuild->Tag        = O_BUILD;
  miAddOrderNameDescribe->Tag = O_NAME;
  miAddOrderSteal->Tag        = O_STEAL;
  miAddOrderAssassinate->Tag  = O_ASSASSINATE;
  miAddOrderAttack->Tag       = O_ATTACK;

  miAddOrderDestroy->Tag      = O_DESTROY;
  miAddOrderEntertain->Tag    = O_ENTERTAIN;
  miAddOrderPillage->Tag      = O_PILLAGE;
  miAddOrderTax->Tag          = O_TAX;
  miAddOrderWork->Tag         = O_WORK;

  miAddOrderAutotax->Tag      = O_AUTOTAX;
  miAddOrderAvoid->Tag        = O_AVOID;
  miAddOrderBehind->Tag       = O_BEHIND;
  miAddOrderGuard->Tag        = O_GUARD;
  miAddOrderHold->Tag         = O_HOLD;
  miAddOrderNoaid->Tag        = O_NOAID;
  miAddOrderReveal->Tag       = O_REVEAL;
  miAddOrderConsume->Tag      = O_CONSUME;
  miAddOrderNoCross->Tag      = O_NOCROSS;
  miAddOrderNoSpoils->Tag     = O_NOSPOILS;
  miAddOrderSpoils->Tag       = O_SPOILS;
  miAddOrderSharing->Tag      = O_SHARING;

  miAddOrderCombat->Tag       = O_COMBAT;
  miAddOrderForget->Tag       = O_FORGET;
  miAddOrderStudy->Tag        = O_STUDY;
  miAddOrderProduce->Tag      = O_PRODUCE;

  miAddOrderBuy->Tag          = O_BUY;
  miAddOrderSell->Tag         = O_SELL;

  miAddOrderClaim->Tag        = O_CLAIM;
  miAddOrderWithdraw->Tag     = O_WITHDRAW;

  miAddOrderLeave->Tag        = O_LEAVE;
  miAddOrderEnter->Tag        = O_ENTER;
  miAddOrderPromote->Tag      = O_PROMOTE;
  miAddOrderMove->Tag         = O_MOVE;
  miAddOrderAdvance->Tag      = O_ADVANCE;
  miAddOrderSail->Tag         = O_SAIL;

}
//---------------------------------------------------------------------------
void __fastcall TfEditTurnOrder::btRestoreClick(TObject *Sender)
{
  Forders->Clear();
  foreach(FSaveOrders){
	AOrder *ord = *(AOrder**)iter;
	AOrder *ord1 = ParseOrder(ord->WriteTemplate());
	if(!ord1) continue;
	Forders->Add(ord1);
  }
  UpdateOrders();
}
//---------------------------------------------------------------------------
void __fastcall TfEditTurnOrder::Button1Click(TObject *Sender)
{
  AOrder *ord;
  Forders->Clear();
  TStrings *list = Memo1->Lines;
  AnsiString s;
  for(int i=0,endi=list->Count;i<endi;i++)
  {
	s = list->Strings[i];
	if(!s.Length()) continue;
	ord = ParseOrder(s);
	if(!ord) continue;

	if(ord->type==O_END||ord->type==O_UNIT||ord->type==O_ENDTURN||ord->type==O_TURN
		 ||ord->type==O_FORM||ord->type==O_FORMEND)
	{
	  AddEvent("This order cannot be used here: "+s);
	  delete(ord);
	  continue;
	}
	ord->turnorder = true;
	Forders->Add(ord);
  } // for
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

