//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdlib.h>
#include <algorithm>
#include "util.h"
#include "eapfile.h"
#include "orders.h"
#include "unit.h"
#include "items.h"
#include "markets.h"
#include "region.h"
#include "faction.h"
#include "skills.h"
#include "EditOrderCustomFrm.h"
#include "EditOrderCommentFrm.h"
#include "EditOrderDescrFrm.h"
#include "EditOrderSimpleFrm.h"
#include "EditOrderByteFrm.h"
#include "EditOrderAbrFrm.h"
#include "EditOrderGiveFrm.h"
#include "EditOrderBuySellFrm.h"
#include "EditOrderTeachFrm.h"
#include "EditOrderIntFrm.h"
#include "EditOrderEnterFrm.h"
#include "EditOrderMovingFrm.h"
#include "EditOrderAutoGiveFrm.h"
#include "EditOrderBuild.h"
#include "EditOrderAutoMoveFrm.h"
#include "EditOrderCastFrm.h"
#include "exprlang.h"
#include "EditOrderWithdrawFrm.h"
#include "EditOrderAutoWarningFrm.h"
#include "EditOrderNameDescribeFrm.h"
#include "EditOrderStealFrm.h"
#include "EditOrderAutoFollow.h"
#include "EditOrderAssassinateFrm.h"
#include "EditOrderAttackFrm.h"
#include "plugdoc\plugins.h"
#include "acplugins.h"
#include "EditOrderPromoteFrm.h"
#include "turn.h"
#include "gameparams.h"
#include "EditTurnOrder.h"
#include "ChangeAtt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
struct OrderStr{
 OrderTypes index;
 char key[28];
}
OrderStrs[]={
 {O_END,"#end"},
 {O_UNIT,"unit "},
 {O_COMMENT," "},
 {O_LOCALDESCR,";"},
 {O_FORM,"form "},
 {O_FORMEND,"end"},
 {O_TURN,"turn"},
 {O_ENDTURN,"endturn"},

 {O_ADVANCE,"advance "},
 {O_ASSASSINATE,"assassinate "},
 {O_ATTACK,"attack "},
 {O_AUTOTAX,"autotax "},
 {O_AVOID,"avoid "},
 {O_BEHIND,"behind "},
 {O_BUILD,"build"},
 {O_BUY,"buy "},
 {O_CAST,"cast "},
 {O_CLAIM,"claim "},
 {O_COMBAT,"combat"},
 {O_CONSUME,"consume"},
 {O_OPTION,"option "},
 {O_DESTROY,"destroy"},
 {O_ENTERTAIN,"entertain"},
 {O_ENTER,"enter "},
 {O_FORGET,"forget "},
 {O_GIVE,"give "},
 {O_GUARD,"guard "},
 {O_HOLD,"hold "},
 {O_LEAVE,"leave"},
 {O_MOVE,"move "},
 {O_NOAID,"noaid "},
 {O_NOCROSS,"nocross "},
 {O_NOP,"NOP"},
 {O_NOSPOILS,"nospoils "},
 {O_PILLAGE,"pillage"},
 {O_PRODUCE,"produce "},
 {O_REVEAL,"reveal"},
 {O_SAIL,"sail"},
 {O_SELL,"sell "},
 {O_SPOILS,"spoils "},
 {O_STEAL,"steal "},
 {O_STUDY,"study "},
 {O_TAX,"tax"},
 {O_TEACH,"teach "},
 {O_WORK,"work"},
 {O_WITHDRAW,"withdraw "},
 {O_AUTOWARNING,"warning "},
 {O_NAME,"name "},
 {O_DESCRIBE,"describe "},
 {O_PROMOTE,"promote "},
 {O_SHARING,"share "},
 {O_DECLARE,"declare "},
 {O_AUTOFOLLOW,"follow "},
 {O_AUTOCOLLECT,"collect "},
 {O_AUTOBUY,"autobuy "},
};
AnsiString keyUnit="unit";
AnsiString keyFaction="faction";
AnsiString keyCity="city";
AnsiString keyShip="ship";
AnsiString keyBuilding="building";
AnsiString keyObject="object";
AnsiString keyIn="IN";
AnsiString keyOut="OUT";
AnsiString keyAutoOrder="@;!";
AnsiString keyAll="all";
AnsiString keyExcept="except";
AnsiString keySpoils[5]={
 "none","walk","ride","fly","all"
};
//---------------------------------------------------------------------------
AnsiString keyCust=" ;!c";
//---------------------------------------------------------------------------
OrderTypes FindOrderType(char *&s){
 for(int i=sizeof(OrderStrs)/sizeof(OrderStrs[0])-1;i>=0;i--){
  OrderStr &os=OrderStrs[i];
  if(!strncmpi(s,os.key,strlen(os.key))){
   s+=strlen(os.key);
   while(*s==' ')s++;
   return os.index;
  }
 }
 return O_NONE;
}
//---------------------------------------------------------------------------
AnsiString FindOrderKey(OrderTypes type){
 for(int i=sizeof(OrderStrs)/sizeof(OrderStrs[0])-1;i>=0;i--){
  if(OrderStrs[i].index==type)return OrderStrs[i].key;
 }
 return "";
}
//---------------------------------------------------------------------------
int ParseAlias(char *&s){
 int facnum=0;
 if(!MyCompare(s,"faction ")){
  s+=sizeof("faction ")-1;
  facnum=atoi(GetNextToken(s).c_str());
  while(*s==' ')s++;
  if(curTurn)
  {
    AFaction *fac=curTurn->factions->GetFaction(facnum);
    if(fac&&fac->local)
      facnum=0;
  }
 }
 int num;
 if(!MyCompare(s,"new ")){
  s+=sizeof("new ")-1;
  num=-atoi(GetNextToken(s).c_str());
  if(facnum)
    num-=facnum*FACTIONMULT;
 }else{
  num=atoi(GetNextToken(s).c_str());
 }
 return num;
}
//---------------------------------------------------------------------------
AnsiString GetAlias(int num,AFaction* faction)
{
  AUnit *un=new AUnit(0);
  if(num<-FACTIONMULT)
  {
    int facnum=-num/FACTIONMULT;
    num%=FACTIONMULT;
    if(faction)
      faction=faction->factions->GetFaction(facnum);
  }
  un->num=num;
  un->faction=faction;
  AnsiString s=un->Alias();
  delete un;
  return s;
}
//---------------------------------------------------------------------------
AOrder *ParseOrder(AnsiString str){
  for(int i=str.Length();i>=1;i--)
	if(str[i]=='\t') str[i]=' ';

  str = str.TrimLeft();
  bool custom = false;
  int pos = str.Pos(keyCust);
  if(pos>0)
  {
	custom = true;
	str.SetLength(pos-1);
	if(!str.Length()) return 0;
  }

  char *s = str.c_str();
  bool commented = false,repeating=false;
  bool _auto = false;
  if(!MyCompare(s, keyAutoOrder))
  {
	s += keyAutoOrder.Length();
	_auto=true;
	repeating=true;
  } else if(s[0]==';'&&s[1]=='!')
  {
	s+=2;
	_auto=true;
  }

  if(_auto)
  {
	int mn=-1,on;
	if(Plugins.FindOrderPlugin(PO_AUTO,s,mn,on))
	{
	  AOrderPlugin *ord=new AOrderPlugin(mn,on);
	  ord->OrderType=PO_AUTO;
	  ord->repeating=true;
	  if(ord->ParseOrder(s)) return ord;
	  delete ord;
	}

	AOrder *ord=0;

	OrderTypes ot=FindOrderType(s);
	switch(ot)
	{
	  case O_GIVE:
		ord = new AOrderAutoGive;
		if(!ord->ParseOrder(s)){
		AddEvent("Error: Bad auto order format: "+str);
		delete ord;
		ord = 0;
	  }
	  break;
	  case O_NOP:
		ord = new AOrderSimple;
		ord->type = O_NOP;
		ord->commented = false;
		ord->repeating = true;
	  break;
	  case O_MOVE:
	  case O_SAIL:
		ord = new AOrderAutoMoving;
		ord->issail = (ot==O_SAIL);
		if(!ord->ParseOrder(s))
		{
		  AddEvent("Error: Bad auto order format: "+str);
		  delete ord;
		  ord = 0;
		}
	  break;
	  case O_WITHDRAW:
		ord = new AOrderWithdraw(true);
		if(!ord->ParseOrder(s))
		{
		  AddEvent("Error: Bad auto order format: "+str);
		  delete ord;
		  ord = 0;
		}
	  break;
	  case O_AUTOWARNING:
		ord = new AOrderAutoWarning;
		if(!ord->ParseOrder(s))
		{
		  AddEvent("Error: Bad auto order format: "+str);
          delete ord;
		  ord = 0;
        }
	  break;
	  case O_AUTOFOLLOW:
		ord = new AOrderAutoFollow();
		if(!ord->ParseOrder(s))
		{
		  AddEvent("Error: Bad order format: "+str);
		  delete ord;
		  ord = 0;
		}
	break;
	  case O_AUTOCOLLECT:
		ord = new AOrderAutoCollect();
		if(!ord->ParseOrder(s))
		{
		  AddEvent("Error: Bad order format: "+str);
		  delete ord;
		  ord = 0;
		}
	break;
	  case O_AUTOBUY:
		ord = new AOrderAutoBuy();
		if(!ord->ParseOrder(s))
		{
		  AddEvent("Error: Bad order format: "+str);
		  delete ord;
		  ord = 0;
		}
	break;

	} // switch
    if(ord) return ord;
  }

  s = str.c_str();
  if(*s==';')
  {
    if(_auto) custom = true;
    else return 0;
  }
  if(*s=='@'){
	repeating=true;
	s++;
  }
  if(*s==';'){
	commented = true;
	s++;
  }

  AOrder *ord;
  if(custom)
  {
	ord = new AOrderCustom();
	ord->commented=commented;
	ord->repeating=repeating;
	if(!ord->ParseOrder(s)){delete ord; return 0;}
	return ord;
  }

  char *olds = s;
  {
	int mn=-1,on;
	if(Plugins.FindOrderPlugin(PO_NORMAL,s,mn,on))
	{
	  AOrderPlugin *ord=new AOrderPlugin(mn,on);
	  if(ord->ParseOrder(s))
	  return ord;
	  delete ord;
	}
  }

  OrderTypes ot = FindOrderType(s);
  switch(ot)
  {
	case O_END:
	case O_UNIT:
	case O_ENDTURN:
	  ord = new AOrder();
	  ord->type = ot;
	break;
	case O_COMMENT:
	  ord = new AOrderComment();
	  if(!ord->ParseOrder(s)){ delete ord; return 0; }
	  ord->repeating = repeating;
	break;
    case O_LOCALDESCR:
      ord = new AOrderLocDescr();
      if(!ord->ParseOrder(s)){ delete ord; return 0; }
    break;
    case O_DESTROY:
    case O_ENTERTAIN:
    case O_LEAVE:
    case O_PILLAGE:
	case O_TAX:
    case O_WORK:
    case O_FORMEND:
      ord = new AOrderSimple();
      ord->type = ot;
      ord->commented = commented;
      ord->repeating = repeating;
      if(!ord->ParseOrder(s)){ delete ord; return 0; }
	break;
    case O_AUTOTAX:
    case O_AVOID:
    case O_BEHIND:
    case O_GUARD:
    case O_HOLD:
    case O_NOAID:
    case O_REVEAL:
	case O_CONSUME:
	case O_SHARING:
	//case O_OPTION:
	case O_NOCROSS:
	case O_NOSPOILS:
	case O_SPOILS:
	  if(ot == O_NOCROSS&&!optServNoCross)
	  {
		int ret = Application->MessageBox("NOCROSS order found.\nActivate NOCROSS flag?","",MB_YESNO);
		if(ret == IDNO)
		{
		  ord = new AOrderCustom();
		  ord->commented = commented;
		  ord->repeating = repeating;
		  if(!ord->ParseOrder(olds)){ delete ord; return 0; }
		  return ord;
		}
		optServNoCross = true;
	  }
	  if(ot==O_NOSPOILS&&!optServNoSpoils)
	  {
		int ret = Application->MessageBox("NOSPOILS order found.\nActivate NOSPOILS flag?","",MB_YESNO);
		if(ret == IDNO)
		{
		  ord = new AOrderCustom();
		  ord->commented = commented;
		  ord->repeating = repeating;
		  if(!ord->ParseOrder(olds)){ delete ord; return 0; }
		  return ord;
		}
		optServNoSpoils=true;
	  }
	  if(ot == O_SPOILS&&!optServSpoils)
	  {
		int ret = Application->MessageBox("SPOILS order found.\nActivate SPOILS flags?","",MB_YESNO);
		if(ret == IDNO)
		{
		  ord = new AOrderCustom();
		  ord->commented = commented;
		  ord->repeating = repeating;
		  if(!ord->ParseOrder(olds)){ delete ord; return 0; }
		  return ord;
		}
		optServSpoils = true;
	  }
	  ord = new AOrderByte();
	  ord->type = ot;
	  ord->commented = commented;
	  ord->repeating = repeating;
	  if(!ord->ParseOrder(s)){ delete ord; return 0; }
	break;
	case O_COMBAT:
	case O_FORGET:
	case O_PRODUCE:
	case O_STUDY:
	  ord = new AOrderAbr();
	  ord->type = ot;
	  ord->commented = commented;
	  ord->repeating = repeating;
	  if(!ord->ParseOrder(s))
	  {
		AddEvent("Error: Bad order format: "+str);
		delete ord;
		ord = new AOrderCustom();
		ord->commented = commented;
		ord->repeating = repeating;
		if(!ord->ParseOrder(olds)){ delete ord; return 0; }
	  }
	break;
	case O_GIVE:
	  ord = new AOrderGive();
	  ord->commented = commented;
	  ord->repeating = repeating;
	  if(!ord->ParseOrder(s))
	  {
		AddEvent("Error: Bad order format: "+str);
		delete ord;
		ord = new AOrderCustom();
		ord->commented = commented;
		ord->repeating = repeating;
		if(!ord->ParseOrder(olds)){ delete ord; return 0; }
	  }
	break;
	case O_BUY:
	case O_SELL:
	  ord = new AOrderBuySell();
	  ord->type = ot;
	  ord->commented = commented;
	  ord->repeating = repeating;
	  if(!ord->ParseOrder(s))
	  {
		AddEvent("Error: Bad order format: "+str);
		delete ord;
		ord = new AOrderCustom();
		ord->commented = commented;
		ord->repeating = repeating;
		if(!ord->ParseOrder(olds)){ delete ord; return 0; }
	  }
	break;
	case O_TEACH:
	  ord = new AOrderTeach();
	  ord->commented=commented;
	  ord->repeating=repeating;
	  if(!ord->ParseOrder(s))
	  {
		AddEvent("Error: Bad order format: "+str);
		delete ord;
		ord=new AOrderCustom();
		ord->commented=commented;
		ord->repeating=repeating;
		if(!ord->ParseOrder(olds)){ delete ord; return 0; }
	  }
	break;
	case O_CLAIM:
	case O_ENTER:
	case O_PROMOTE:
	  ord = new AOrderInt();
	  ord->type = ot;
	  ord->commented = commented;
	  ord->repeating = repeating;
	  if(!ord->ParseOrder(s))
	  {
		AddEvent("Error: Bad order format: "+str);
		delete ord;
		ord=new AOrderCustom();
		ord->commented=commented;
		ord->repeating=repeating;
		if(!ord->ParseOrder(olds)){delete ord;return 0;}
	  }
	break;
	case O_MOVE:
	case O_ADVANCE:
	case O_SAIL:
	  ord=new AOrderMoving();
	  ord->type=ot;
      ord->commented=commented;
      ord->repeating=repeating;
      if(!ord->ParseOrder(s))
      {
        AddEvent("Error: Bad order format: "+str);
        delete ord;
        ord=new AOrderCustom();
        ord->commented=commented;
        ord->repeating=repeating;
        if(!ord->ParseOrder(olds)){delete ord;return 0;}
      }
    break;
    case O_BUILD:
      ord=new AOrderBuild();
      ord->commented=commented;
      ord->repeating=repeating;
      if(!ord->ParseOrder(s))
      {
        AddEvent("Error: Bad order format: "+str);
		delete ord;
        ord=new AOrderCustom();
        ord->commented=commented;
        ord->repeating=repeating;
        if(!ord->ParseOrder(olds)){delete ord;return 0;}
      }
    break;
    case O_CAST:
	  ord=new AOrderCast();
      ord->commented=commented;
      ord->repeating=repeating;
      if(!ord->ParseOrder(s))
      {
        AddEvent("Error: Bad order format: "+str);
        delete ord;
        ord=new AOrderCustom();
        ord->commented=commented;
        ord->repeating=repeating;
        if(!ord->ParseOrder(olds)){delete ord;return 0;}
      }
    break;
    case O_NAME:
    case O_DESCRIBE:
      ord=new AOrderNameDescribe();
      ord->type=ot;
      ord->commented=commented;
      ord->repeating=repeating;
      if(!ord->ParseOrder(s))
      {
        AddEvent("Error: Bad order format: "+str);
        delete ord;
        ord=new AOrderCustom();
        ord->commented=commented;
        ord->repeating=repeating;
		if(!ord->ParseOrder(olds)){delete ord;return 0;}
      }
    break;
    case O_STEAL:
      ord=new AOrderSteal();
      ord->commented=commented;
      ord->repeating=repeating;
      if(!ord->ParseOrder(s))
	  {
        AddEvent("Error: Bad order format: "+str);
        delete ord;
        ord=new AOrderCustom();
        ord->commented=commented;
        ord->repeating=repeating;
        if(!ord->ParseOrder(olds)){delete ord;return 0;}
      }
    break;
    case O_ASSASSINATE:
      ord=new AOrderAssassinate();
      ord->commented=commented;
      ord->repeating=repeating;
      if(!ord->ParseOrder(s))
      {
        AddEvent("Error: Bad order format: "+str);
        delete ord;
        ord=new AOrderCustom();
        ord->commented=commented;
        ord->repeating=repeating;
        if(!ord->ParseOrder(olds)){delete ord;return 0;}
      }
    break;
    case O_ATTACK:
      ord=new AOrderAttack();
      ord->commented=commented;
	  ord->repeating=repeating;
      if(!ord->ParseOrder(s))
      {
        AddEvent("Error: Bad order format: "+str);
        delete ord;
        ord=new AOrderCustom();
        ord->commented=commented;
        ord->repeating=repeating;
		if(!ord->ParseOrder(olds)){delete ord;return 0;}
      }
    break;
    case O_WITHDRAW:
      ord=new AOrderWithdraw(false);
      ord->commented=commented;
      ord->repeating=repeating;
      if(!ord->ParseOrder(s))
      {
        AddEvent("Error: Bad auto order format: "+str);
        delete ord;
        ord=0;
      }
    break;
    case O_TURN:
      ord = new AOrderTurn();
      ord->commented = commented;
      ord->repeating = repeating;
      if(!ord->ParseOrder(s))
      {
        AddEvent("Error: Bad order format: "+str);
        delete ord;
        ord=new AOrderCustom();
        ord->commented=commented;
        ord->repeating=repeating;
        if(!ord->ParseOrder(olds)){delete ord;return 0;}
	  }
    break;
    case O_FORM:
      ord = new AOrderForm();
      if(!ord->ParseOrder(s))
      {
        AddEvent("Error: Bad order format: "+str);
        delete ord;
		ord=new AOrderCustom();
        ord->commented=commented;
        ord->repeating=repeating;
        if(!ord->ParseOrder(olds)){delete ord; return 0;}
      }
    break;
    case O_DECLARE:
	  ord = new AOrderSetAtt();
	  ord->commented = commented;
	  ord->repeating = repeating;
      if(!ord->ParseOrder(s))
      {
        AddEvent("Error: Bad order format: "+str);
        delete ord;
        ord=new AOrderCustom();
        ord->commented=commented;
        ord->repeating=repeating;
        if(!ord->ParseOrder(olds)){delete ord;return 0;}
      }
    break;
	case O_AUTOFOLLOW:
	  ord=new AOrderAutoFollow();
	  if(!ord->ParseOrder(s))
	  {
		AddEvent("Error: Bad order format: "+str);
		delete ord;
		ord=new AOrderCustom();
		ord->commented=commented;
		ord->repeating=repeating;
        if(!ord->ParseOrder(olds)){delete ord;return 0;}
      }
    break;
	case   O_AUTOCOLLECT:
	  ord=new AOrderAutoCollect();
	  if(!ord->ParseOrder(s))
	  {
		AddEvent("Error: Bad order format: "+str);
		delete ord;
		ord=new AOrderCustom();
		ord->commented=commented;
		ord->repeating=repeating;
        if(!ord->ParseOrder(olds)){delete ord;return 0;}
      }
    break;
	case O_AUTOBUY:
	  ord=new AOrderAutoBuy();
	  if(!ord->ParseOrder(s))
	  {
		AddEvent("Error: Bad order format: "+str);
		delete ord;
		ord=new AOrderCustom();
		ord->commented=commented;
		ord->repeating=repeating;
        if(!ord->ParseOrder(olds)){delete ord;return 0;}
      }
    break;
	default:
      ord=new AOrderCustom();
      ord->commented=commented;
      ord->repeating=repeating;
      if(!ord->ParseOrder(olds)){delete ord;return 0;}
  } // switch

 return ord;
 
};
AOrder *ParseOrder1(AnsiString str){
  return ParseOrder(str);
}
//---------------------------------------------------------------------------
AOrder *ParseOrderAdd1(AnsiString str)
{
  return ParseOrder(str);
}
//---------------------------------------------------------------------------
AOrder *ReadOrder(AOrders *orders,TFile &in){
  int i;
  OrderTypes type;
  ReadIntAsByte(in,i);
  type=OrderTypes(i);
  AOrder *ord;
  switch(type){
   case O_CUSTOMORDER:
	ord=new AOrderCustom();
	ord->Read(in);
	return ord;
   case O_COMMENT:
	ord=new AOrderComment();
    ord->Read(in);
    return ord;
   case O_LOCALDESCR:
    ord=new AOrderLocDescr();
    ord->Read(in);
    return ord;
   case O_DESTROY:
   case O_ENTERTAIN:
   case O_LEAVE:
   case O_PILLAGE:
   case O_TAX:
   case O_WORK:
   case O_NOP:
   case O_FORMEND:
	ord=new AOrderSimple();
    ord->type=type;
    ord->Read(in);
    return ord;
   case O_AUTOTAX:
   case O_AVOID:
   case O_BEHIND:
   case O_GUARD:
   case O_HOLD:
   case O_NOAID:
   case O_REVEAL:
   case O_CONSUME:
   case O_SHARING:
//   case O_OPTION:
   case O_NOCROSS:
   case O_NOSPOILS:
   case O_SPOILS:
    ord=new AOrderByte();
    ord->type=type;
	ord->Read(in);
    return ord;
   case O_COMBAT:
   case O_FORGET:
   case O_PRODUCE:
   case O_STUDY:
    ord=new AOrderAbr();
    ord->type=type;
    ord->Read(in);
    return ord;
   case O_GIVE:
    ord=new AOrderGive();
    ord->Read(in);
    return ord;
   case O_BUY:
   case O_SELL:
    ord=new AOrderBuySell();
    ord->type=type;
    ord->Read(in);
    return ord;
   case O_TEACH:
    ord=new AOrderTeach();
    ord->Read(in);
    return ord;
   case O_CLAIM:
   case O_ENTER:
   case O_PROMOTE:
    ord=new AOrderInt;
    ord->type=type;
    ord->Read(in);
    return ord;
   case O_MOVE:
   case O_ADVANCE:
   case O_SAIL:
    ord=new AOrderMoving;
    ord->type=type;
    ord->Read(in);
    return ord;
   case O_AUTOGIVE:
    ord=new AOrderAutoGive;
    ord->Read(in);
    return ord;
   case O_BUILD:
    ord=new AOrderBuild;
    ord->Read(in);
    return ord;
   case O_AUTOMOVE:
    ord=new AOrderAutoMoving();
    ord->Read(in);
	return ord;
   case O_CAST:
    ord=new AOrderCast();
    ord->Read(in);
    return ord;
   case O_WITHDRAW:
    ord=new AOrderWithdraw(false);
    ord->Read(in);
    return ord;
   case O_AUTOWARNING:
    ord=new AOrderAutoWarning;
    ord->Read(in);
    return ord;
   case O_NAME:
   case O_DESCRIBE:
    ord=new AOrderNameDescribe();
    ord->type=type;
    ord->Read(in);
    return ord;
   case O_STEAL:
    ord=new AOrderSteal();
    ord->Read(in);
    return ord;
   case O_ASSASSINATE:
    ord=new AOrderAssassinate();
    ord->Read(in);
    return ord;
   case O_ATTACK:
    ord=new AOrderAttack();
    ord->Read(in);
    return ord;
   case O_PLUGIN:
    ord=new AOrderPlugin;
    ord->Read(in);
	return ord;
   case O_TURN:
    ord=new AOrderTurn;
    ord->Read(in);
    return ord;
   case O_DECLARE:
    ord = new AOrderSetAtt;
    ord->Read(in);
    return ord;
   case O_FORM:
    ord = new AOrderForm;
    ord->Read(in);
    return ord;
   case O_AUTOFOLLOW:
	ord=new AOrderAutoFollow();
	ord->Read(in);
	return ord;
   case O_AUTOCOLLECT:
	ord=new AOrderAutoCollect();
	ord->Read(in);
	return ord;
   case O_AUTOBUY:
	ord=new AOrderAutoBuy();
	ord->Read(in);
	return ord;

/*
   case O_:
    ord=new AOrder;
    ord->type=type;
    ord->Read(in);
    return ord;
*/
  }
  return 0;
};
//---------------------------------------------------------------------------
AOrder::AOrder()
 :type(O_NONE),orders(0),flags(0),isedited(false),turnorder(false)
// commented(false),repeating(false)
{   }
//---------------------------------------------------------------------------
AOrder::~AOrder() { }
//---------------------------------------------------------------------------
void AOrder::Read(TFile & in) { in>>flags; }
//---------------------------------------------------------------------------
void AOrder::Write(TFile & out)
{
 WriteIntAsByte(out,type);
 out<<flags;
}
//---------------------------------------------------------------------------
AnsiString AOrder::Print()
{
  if(!orders->unit)
  {
    if(type == O_FORMEND || type == O_FORM ) return FindOrderKey(type);
  }
  return "ERROR";
}
//---------------------------------------------------------------------------
AnsiString AOrder::WriteTemplate()
{
 return "ERROR";
}
//---------------------------------------------------------------------------
bool AOrder::ParseOrder(char * s)
{
 return false;
}
//---------------------------------------------------------------------------
void AOrder::Run(int value)
{
}
//---------------------------------------------------------------------------
AOrder * AOrder::Edit()
{
 return 0;
}
//---------------------------------------------------------------------------
AnsiString AOrder::Begin()
{
 AnsiString s;
 if(repeating)s+="@";
 if(commented)s+=";";
 return s;
}
//---------------------------------------------------------------------------
bool AOrder::IsMonthLong()
{
 return false;
}
//---------------------------------------------------------------------------
bool AOrder::IsTradeOrder()
{
 return false;
}
//---------------------------------------------------------------------------
int AOrder::Productivity()
{
 return 0;
}
//---------------------------------------------------------------------------
void AOrder::DeleteMessage(AOrderMessage *mes){
}
//---------------------------------------------------------------------------
void AOrder::Prepare(){
}
//---------------------------------------------------------------------------
AOrders::AOrders(AUnit *un)
 :unit(un),autodelete(true)
{
}
//---------------------------------------------------------------------------
__fastcall AOrders::~AOrders()
{
 Clear();
}
//---------------------------------------------------------------------------
void __fastcall AOrders::Clear()
{
 if(autodelete){
  for(int i=Count-1;i>=0;i--){
   AOrder *ord=Get(i);
 /*  if(ord->type==O_RECEIVED){
    AOrderMessage *o=dynamic_cast<AOrderMessage*>(ord);
    if(o->base){
     o->base->target=0;
    }
   }*/
   TList::Delete(i);
   delete ord;
  }
 }
 TList::Clear();
}
//---------------------------------------------------------------------------
void AOrders::Read(TFile & in)
{
 int kol;
 in>>kol;
 Capacity=kol;
 for(int i=0;i<kol;i++){
  AOrder *ord=ReadOrder(this,in);
  if(ord)Add(ord);
 }
}
//------------------------------------------------------------------
void AOrders::Write(TFile & out)
{
  Compact();
  int kol = count;
  out<<kol;
  foreach(this)
  {
	AOrder *ord=*(AOrder**)iter;
	ord->Write(out);
  } 
}
//------------------------------------------------------------------
AOrder* AOrders::Get(int index)
{
  return (AOrder*)Items[index];
}
//------------------------------------------------------------------
void AOrders::Add(AOrder* ord)
{
  if(autodelete) ord->orders = this;
  if(ord->type == O_TURN)
  {
	foreach(((AOrderTurn*)ord)->torders)
	{
	  AOrder *ord1 = *(AOrder**)iter;
	  ord1->turnorder = true;
	  ord1->orders->unit = this->unit;
	}
  }
  TList::Add(ord);
}
//------------------------------------------------------------------
void AOrders::Insert(int index,AOrder* ord)
{
  if(autodelete) ord->orders = this;
  TList::Insert(index, ord);
}
//------------------------------------------------------------------
void AOrders::Delete(AOrder * ord)
{
  for(int i=count-1;i>=0;i--)
  {
	AOrder *o=Get(i);
	if(o==ord)
	 {
	   TList::Delete(i);
	   if(autodelete) delete ord;
	   break;
	 }
  }
}
//------------------------------------------------------------------
void AOrders::Delete(int index)
{
 if(index<0||index>=count)return;
 AOrder *ord=Get(index);
 TList::Delete(index);
 if(autodelete) delete ord;
}
//---------------------------------------------------------------------------
void AOrders::Update(AOrders * orders)
{
 if(!autodelete)
  throw Exception("Can't update orders by autodelete=false");
 Clear();
 foreach(orders){
  AOrder *ord=*(AOrder**)iter;
  Add(ord);
 }
 bool oldad=orders->autodelete;
 orders->autodelete=false;
 orders->Clear();
 orders->autodelete=oldad;
}
//---------------------------------------------------------------------------
bool AOrders::Replace(AOrder * from, AOrder * to)
{
 for(int i=count-1;i>=0;i--){
  if(Get(i)==from){
   if(autodelete)to->orders=this;
   Items[i]=to;
   if(autodelete)delete from;
   return true;
  }
 }
 return false;
}
//---------------------------------------------------------------------------
AOrder * AOrders::Find(OrderTypes type, int mode)
{
 if(type==O_LOCALDESCR){
  foreach(this){
   AOrder *ord=*(AOrder**)iter;
   if(ord->type!=type)continue;
   return ord;
  }
  return 0;
 }
 foreachr(this){
  AOrder *ord=*(AOrder**)iter;
  if(ord->type!=type)continue;
  if(ord->commented==true){
   if(mode&1)return ord;
  }else{
   if(mode&2)return ord;
  }
  if(ord->repeating==true){
   if(mode&4)return ord;
  }else{
   if(mode&8)return ord;
  }
 }
 return 0;
}
//---------------------------------------------------------------------------
AOrder * AOrders::GetMonthLong()
{
 foreachr(this){
  AOrder *ord=*(AOrder**)iter;
  if(ord->commented) continue;
  if(!ord->IsMonthLong()) continue;
  return ord;
 }
 return 0;
}
//---------------------------------------------------------------------------
bool AOrders::Distribute(int amount, int attempted)
{
//если attemped<0, то они не нормируются (используется Pillage)
 if(attempted<0)
  attempted=-attempted;
 else
  if(attempted<amount) attempted=amount;
 foreach(this){
  AOrder *ord=*(AOrder**)iter;
  if(!amount)break;
  if(!attempted)break;
  int prod=ord->Productivity(),ubucks;
  if(prod==-1)return false;
  ubucks=int(double(amount)*prod/attempted);
  amount-=ubucks;
  attempted-=prod;
  ord->Run(ubucks);
 }
 return true;
}
//---------------------------------------------------------------------------
void AOrders::Compact()
{
 if(!unit)return;
 if(!count)return;
 for(int i=Count-1;i>=0;i--){
  AOrder *ord=Get(i);
  if(ord->type!=O_MESSAGE)continue;
  TList::Delete(i);
  if(autodelete) delete ord;
 }
 if(!count)return;
 bool autotax=unit->GetFlag(FLAG_AUTOTAX);
 AOrderByte *o=dynamic_cast<AOrderByte*>(Find(O_AUTOTAX,2));
 if(o) autotax=o->value;
 if(autotax){
  for(int i=Count-1;i>=0;i--){
   AOrder *ord=Get(i);
   if(ord->type!=O_TAX)continue;
   if(ord->repeating)continue;
   if(ord->commented)continue;
   if(autodelete)delete ord;
   TList::Delete(i);
  }
 }
 if(!count)return;
 {
  AOrder *ord=Get(0);
  if(ord->type!=O_LOCALDESCR){
   for(int i=count-1;i>0;i--){
    ord=Get(i);
    if(ord->type!=O_LOCALDESCR)continue;
    TList::Delete(i);
    TList::Insert(0,ord);
    break;
   }
  }
 }
}
//---------------------------------------------------------------------------
bool AOrders::Swap(int i, int j)
{
  if(i == j) return false;
  if(i<0 || i>=count) return false;
  if(j<0 || j>=count) return false;
  AOrder *ord1 = (AOrder*)Items[i];
  if(ord1->type==O_MESSAGE) return false;
  AOrder *ord2 = (AOrder*)Items[j];
  if(ord2->type==O_MESSAGE) return false;
  Items[i] = ord2;
  Items[j] = ord1;
  return true;
}
//---------------------------------------------------------------------------
AOrderCustom::AOrderCustom(){ type = O_CUSTOMORDER; }
//---------------------------------------------------------------------------
AOrderCustom::~AOrderCustom(){ }
//---------------------------------------------------------------------------
void AOrderCustom::Read(TFile & in)
{
 AOrder::Read(in);
 in>>text;
}
//---------------------------------------------------------------------------
void AOrderCustom::Write(TFile & out)
{
 AOrder::Write(out);
 out<<text;
}
//---------------------------------------------------------------------------
AnsiString AOrderCustom::Print()
{
 AnsiString s=Begin()+text+" (custom)";
 if(IsMonthLong())s+="+";
 return s;
}
//---------------------------------------------------------------------------
AnsiString AOrderCustom::WriteTemplate()
{
 return Begin()+text+keyCust;
}
//---------------------------------------------------------------------------
bool AOrderCustom::ParseOrder(char * s)
{
 text=s;
 return true;
}
//---------------------------------------------------------------------------
AOrder * AOrderCustom::Edit()
{
 TEditOrderCustomForm *frm=new TEditOrderCustomForm(0,this);
 frm->ShowModal();
 AOrder *ord=frm->ord;
 delete frm;
 return ord;
}
//---------------------------------------------------------------------------
bool AOrderCustom::IsMonthLong()
{
 if(commented)return false;
// if(!MyCompare(text.c_str(),FindOrderKey(O_BUILD)))return true;
// if(!MyCompare(text.c_str(),FindOrderKey(O_PRODUCE)))return true;
// if(!MyCompare(text.c_str(),FindOrderKey(O_TEACH)))return true;
// if(!MyCompare(text.c_str(),FindOrderKey(O_MOVE)))return true;
// if(!MyCompare(text.c_str(),FindOrderKey(O_ADVANCE)))return true;
// if(!MyCompare(text.c_str(),FindOrderKey(O_SAIL)))return true;
 return false;
}
//---------------------------------------------------------------------------
AOrderComment::AOrderComment()
{
 type=O_COMMENT;
 commented=true;
 repeating=true;
}
//---------------------------------------------------------------------------
AOrderComment::~AOrderComment()
{
}
//---------------------------------------------------------------------------
void AOrderComment::Read(TFile & in)
{
 AOrder::Read(in);
 in>>text;
 commented=true;
}
//---------------------------------------------------------------------------
void AOrderComment::Write(TFile & out)
{
 AOrder::Write(out);
 out<<text;
}
//---------------------------------------------------------------------------
AnsiString AOrderComment::Print()
{
 return Begin()+" "+text+" (comment)";
}
//---------------------------------------------------------------------------
AnsiString AOrderComment::WriteTemplate()
{
 return Begin()+" "+text;
}
//---------------------------------------------------------------------------
bool AOrderComment::ParseOrder(char * s)
{
 commented = true;
 text = s;
 return true;
}
//---------------------------------------------------------------------------
AOrder* AOrderComment::Edit()
{
 TEditOrderCommentForm *frm=new TEditOrderCommentForm(0,this);
 frm->ShowModal();
 AOrder *ord=frm->ord;
 delete frm;
 return ord;
}
//---------------------------------------------------------------------------
AOrderLocDescr::AOrderLocDescr()
{
 type=O_LOCALDESCR;
 commented=true;
 repeating=true;
}
//---------------------------------------------------------------------------
AOrderLocDescr::~AOrderLocDescr()
{
}
//---------------------------------------------------------------------------
void AOrderLocDescr::Read(TFile & in)
{
 AOrder::Read(in);
 in>>text;
 commented=true;
 repeating=true;
}
//---------------------------------------------------------------------------
void AOrderLocDescr::Write(TFile & out)
{
 AOrder::Write(out);
 out<<text;
}
//---------------------------------------------------------------------------
AnsiString AOrderLocDescr::Print()
{
 return "@;;"+text+" (local descr)";
}
//---------------------------------------------------------------------------
AnsiString AOrderLocDescr::WriteTemplate()
{
 return "@;;"+text;
}
//---------------------------------------------------------------------------
bool AOrderLocDescr::ParseOrder(char * s)
{
 commented=true;
 repeating=true;
 text=s;
 return true;
}
//---------------------------------------------------------------------------
AOrder * AOrderLocDescr::Edit()
{
 TEditOrderDescrForm *frm=new TEditOrderDescrForm(0,this);
 frm->ShowModal();
 AOrder *ord=frm->ord;
 delete frm;
 return ord;
}
//---------------------------------------------------------------------------
AOrderSimple::AOrderSimple()
 :number(-1)
{
}
//---------------------------------------------------------------------------
AOrderSimple::~AOrderSimple()
{
}
//---------------------------------------------------------------------------
AnsiString AOrderSimple::Print()
{
 if(type==O_NOP){
  return keyAutoOrder+FindOrderKey(O_NOP);
 }
 AnsiString s=Begin()+FindOrderKey(type);
 if(commented)return s;
 if(type==O_PILLAGE||type==O_TAX||type==O_ENTERTAIN||type==O_WORK){
  s+=" ;+$";
  s+=number;
 }
 return s;
}
//---------------------------------------------------------------------------
AnsiString AOrderSimple::WriteTemplate()
{
 if(type==O_NOP)
  return keyAutoOrder+FindOrderKey(O_NOP);
 return Begin()+FindOrderKey(type);
}
//---------------------------------------------------------------------------
bool AOrderSimple::ParseOrder(char * s)
{
 return true;
}
//---------------------------------------------------------------------------
AOrder * AOrderSimple::Edit()
{
  if(type==O_NOP) return this;
  TEditOrderSimpleForm *frm=new TEditOrderSimpleForm(0,this);
  switch(type)
  {
	case O_DESTROY:
	  frm->Caption = "Edit DESTROY order";
	  frm->labInfo->Caption = "";
	  break;
	case O_ENTERTAIN:
	  frm->Caption = "Edit ENTERTAIN order";
	  if(orders->unit->GetSkill(EnteSkill)==0) frm->labInfo->Caption="Warning: Unit cannot ENTERTAIN";
		else frm->labInfo->Caption="";
	  break;
	case O_LEAVE:
	  frm->Caption="Edit LEAVE order";
	  frm->labInfo->Caption="";
	  break;
	case O_PILLAGE:
	  frm->Caption="Edit PILLAGE order";
	  if(!curUnit->Taxers()) frm->labInfo->Caption="Warning: Unit cannot TAX";
	  else if(!curUnit->baseobj->basereg->money) frm->labInfo->Caption="Warning: No money";
	  else if(!curUnit->baseobj->basereg->CanTax(curUnit)) frm->labInfo->Caption="Warning: Unit on guard";
	  break;
	case O_TAX:
	  frm->Caption="Edit TAX order";
	  if(!curUnit->Taxers()) frm->labInfo->Caption="Warning: Unit cannot TAX";
	  else if(!curUnit->baseobj->basereg->money) frm->labInfo->Caption="Warning: No money";
	  else if(!curUnit->baseobj->basereg->CanTax(curUnit)) frm->labInfo->Caption="Warning: Unit on guard";
	  break;
	case O_WORK:
	  frm->Caption="Edit WORK order";
	  frm->labInfo->Caption="";
	  break;
  }
  frm->ShowModal();
  AOrder *ord = frm->ord;
  delete frm;
  return ord;
}
//---------------------------------------------------------------------------
void AOrderSimple::Run(int value)
{
 if(commented)return;
 AUnit *un=orders->unit;
 AnsiString mes;
 switch(type){
  case O_DESTROY:
  case O_NOP:
  break;
  case O_LEAVE:
  {
   if(TerrainTypes->IsOcean(un->baseobj->basereg->type)&&
	(!un->CanSwim()||un->GetEndFlag(FLAG_NOCROSS_WATER))){
    mes="Error MOVE: ";
    mes+=un->FullName();
    mes+=": Can't leave ship in the ocean";
    AddEvent(mes);
    break;
   }
   AObject *obj=un->endobj;
   if(obj->owner==un)
     obj->SetNextOwner();
   un->endobj=un->baseobj->basereg->GetDummy();
  }
  break;
  case O_PILLAGE:
  case O_ENTERTAIN:
  case O_TAX:
  case O_WORK:
  {
   number=value;
   un->SetMoney(un->GetMoney() + value);
   int t=-1;
   switch(type)
   {
     case O_PILLAGE:case O_TAX:t=3;break;
     case O_ENTERTAIN:t=1;break;
     case O_WORK:t=0;break;
   }
   LogItem(t,value,"SILV",un);
  }
  break;
 }
}
//---------------------------------------------------------------------------
bool AOrderSimple::IsMonthLong()
{
 if(commented)return false;
 if(type==O_ENTERTAIN||type==O_WORK||type==O_NOP)return true;
 if(optTaxIsMonthlong&&(type==O_TAX||type==O_PILLAGE)) return true;
 return false;
}
//---------------------------------------------------------------------------
int AOrderSimple::Productivity()
{
 AUnit *unit=orders->unit;
 ARegion *reg=unit->baseobj->basereg;
 if(type==O_TAX)return reg->curtaxprod*unit->Taxers();
 if(type==O_PILLAGE)
  return unit->Taxers();
//  return 100*unit->Taxers();
 if(type==O_ENTERTAIN){
  int lev=unit->GetSkill(EnteSkill);
  if(lev==0){
   AddEvent(unit->FullName()+" Error: Unit doesn't have ENTERTAIN skill");
   return 0;
  }
  int ei=reg->entincome;
  if(ei==0) ei=EntertainEffect;
  AItemType *itype=ItemTypes->Get("SILV",4);
  int mens=unit->GetMen();
  int prod=mens*lev;
  if(itype->helpprodmult){
    int bonus=mens;
    if(itype->helpproditem.Length()){
      bonus=unit->enditems->GetNum(itype->helpproditem);
    }
    if(bonus>mens) bonus=mens;
    prod+=bonus*itype->helpprodmult/EntertainEffect;
  }
  return ei*prod;
 }
 if(type==O_WORK){
  int wi=reg->workincome;
  if(wi==0) wi=reg->wages;
  return wi*orders->unit->GetMen();
 }
 return 0;
}
//---------------------------------------------------------------------------
void AOrderSimple::Prepare()
{
 switch(type){
  case O_ENTERTAIN:
  case O_TAX:
  case O_WORK:
  case O_PILLAGE:
   number=0;
  break;
 }
}
//---------------------------------------------------------------------------
AOrderByte::AOrderByte()
 :value(false)
{
}
//---------------------------------------------------------------------------
AOrderByte::~AOrderByte()
{
}
//---------------------------------------------------------------------------
void AOrderByte::Read(TFile & in)
{
 AOrder::Read(in);
 in>>value;
}
//---------------------------------------------------------------------------
void AOrderByte::Write(TFile & out)
{
 AOrder::Write(out);
 out<<value;
}
//---------------------------------------------------------------------------
AnsiString AOrderByte::Print()
{
 AnsiString s=Begin()+FindOrderKey(type);
 if(type==O_REVEAL||type==O_CONSUME){
  switch(value){
   case 0:
   break;
   case 1:
    s+=" "+keyUnit;
   break;
   case 2:
    s+=" "+keyFaction;
   break;
  }
 }else if(type==O_SPOILS){
  s+=keySpoils[value];
 }else{
  s+=(value?"1":"0");
 }
 return s;
}
//---------------------------------------------------------------------------
AnsiString AOrderByte::WriteTemplate()
{
 AnsiString s=Begin()+FindOrderKey(type);
 if(type==O_REVEAL||type==O_CONSUME){
  switch(value){
   case 0:
   break;
   case 1:
    s+=" "+keyUnit;
   break;
   case 2:
    s+=" "+keyFaction;
   break;
  }
 }else if(type==O_SPOILS){
  s+=keySpoils[value];
 }else{
  s+=(value?"1":"0");
 }
 return s;
}
//---------------------------------------------------------------------------
static AnsiString TrueStr="true";
//---------------------------------------------------------------------------
bool AOrderByte::ParseOrder(char *s)
{
 if(type==O_REVEAL||type==O_CONSUME){
  if(*s=='\0'){
   value=0;
  }else if(!MyCompare(s,keyUnit)){
   value=1;
  }else if(!MyCompare(s,keyFaction)){
   value=2;
  }else return false;
  return true;
 }
 if(type==O_SPOILS){
  for(int i=0;i<5;i++){
   if(MyCompare(s,keySpoils[i])) continue;
   value=i;
   return true;
  }
  return false;
 }
 if(*s=='1')value=true;
 else if(!MyCompare(s,TrueStr))value=true;
 return true;
}
//---------------------------------------------------------------------------
void AOrderByte::Run(int v)
{
 if(commented)return;
 AUnit *un=orders->unit;
 switch(type){
  case O_AUTOTAX:
   un->SetEndFlag(FLAG_AUTOTAX,value);
  break;
  case O_AVOID:
   if(value){
    un->endguard=GUARD_AVOID;
   }else if(un->endguard==GUARD_AVOID){
    un->endguard=GUARD_NONE;
   }
  break;
  case O_BEHIND:
   un->SetEndFlag(FLAG_BEHIND,value);
  break;
  case O_GUARD:
   if(value){
     un->endguard=GUARD_SET;
   }else if(un->endguard!=GUARD_AVOID){
    un->endguard=GUARD_NONE;
   }
  break;
  case O_HOLD:
   un->SetEndFlag(FLAG_HOLDING,value);
  break;
  case O_NOAID:
   un->SetEndFlag(FLAG_NOAID,value);
  break;
  case O_REVEAL:
   un->endreveal=value;
  break;
  case O_CONSUME:
   {
    bool confac=false,conun=false;
    if(value==1) conun=true;
    else if(value==2) confac=true;
    un->SetEndFlag(FLAG_CONSUMING_UNIT,conun);
    un->SetEndFlag(FLAG_CONSUMING_FACTION,confac);
   }
  break;
  case O_NOCROSS:
   if(optServNoCross) un->SetEndFlag(FLAG_NOCROSS_WATER,value);
  break;
  case O_SHARING:
    un->SetEndFlag(FLAG_SHARING,value);
  break;
  case O_NOSPOILS:
   if(optServNoSpoils){
    un->SetEndFlag(FLAG_FLYSPOILS,false);
    un->SetEndFlag(FLAG_RIDESPOILS,false);
    un->SetEndFlag(FLAG_WALKSPOILS,false);
    un->SetEndFlag(FLAG_NOSPOILS,value);
   }
  break;
  case O_SPOILS:
   if(optServSpoils){
    un->SetEndFlag(FLAG_FLYSPOILS,false);
    un->SetEndFlag(FLAG_RIDESPOILS,false);
    un->SetEndFlag(FLAG_WALKSPOILS,false);
    un->SetEndFlag(FLAG_NOSPOILS,false);
    switch(value){
     case 0: //none
      un->SetEndFlag(FLAG_NOSPOILS,true);
     break;
     case 1: //walk
      un->SetEndFlag(FLAG_WALKSPOILS,true);
     break;
     case 2: //ride
      un->SetEndFlag(FLAG_RIDESPOILS,true);
     break;
     case 3: //fly
      un->SetEndFlag(FLAG_FLYSPOILS,true);
     break;
/*     case 4: //all
     break;*/
    }
   }
  break;
 }
}
//---------------------------------------------------------------------------
AOrder * AOrderByte::Edit()
{
 TStringList *list=new TStringList;
 list->Add("O&ff");
 list->Add("O&n");
 AnsiString Caption,Info;
 switch(type){
  case O_AUTOTAX:
   Caption="Edit AUTOTAX order";
   if(!curUnit->Taxers()){
    Info="Warning: Unit cannot TAX";
   }else if(!curUnit->baseobj->basereg->money){
    Info="Warning: No money";
   }else if(!curUnit->baseobj->basereg->CanTax(curUnit)){
    Info="Warning: Unit on guard";
   }
  break;
  case O_AVOID:
   Caption="Edit AVOID order";
  break;
  case O_BEHIND:
   Caption="Edit BEHIND order";
  break;
  case O_GUARD:
   Caption="Edit GUARD order";
   if(!curUnit->baseobj->basereg->CanTax(curUnit)){
    Info="Warning: Unit on guard";
   }
  break;
  case O_SHARING:
   Caption="Edit SHARING order";
  break;
  case O_HOLD:
   Caption="Edit HOLD order";
  break;
  case O_NOAID:
   Caption="Edit NOAID order";
  break;
  case O_REVEAL:
  case O_CONSUME:
   list->Clear();
   list->Add("&None");
   list->Add("&"+keyUnit);
   list->Add("&"+keyFaction);
   if(type==O_REVEAL) Caption="Edit REVEAL order";
   else Caption="Edit CONSUME order";
  break;
  case O_NOCROSS:
   Caption="Edit NOCROSS order";
  break;
  case O_NOSPOILS:
   Caption="Edit NOSPOILS order";
  break;
  case O_SPOILS:
   list->Clear();
   for(int i=0;i<5;i++)
    list->Add("&"+keySpoils[i]);
   Caption="Edit SPOILS order";
  break;
 }
 TEditOrderByteForm *frm=new TEditOrderByteForm(0,this,list);
 frm->Caption=Caption;
 frm->labInfo->Caption=Info;
 frm->ShowModal();
 AOrder *ord=frm->ord;
 delete frm;
 delete list;
 return ord;
}
//---------------------------------------------------------------------------
AOrderAbr::AOrderAbr()
 :number(0)
{
}
//---------------------------------------------------------------------------
AOrderAbr::~AOrderAbr()
{
}
//---------------------------------------------------------------------------
void AOrderAbr::Read(TFile & in)
{
 AOrder::Read(in);
 if(BaseVer>=32)
   in>>abr;
 else
   ReadStringAsInt(in,abr);
}
//---------------------------------------------------------------------------
void AOrderAbr::Write(TFile & out)
{
 AOrder::Write(out);
 out<<abr;
}
//---------------------------------------------------------------------------
AnsiString AOrderAbr::Print()
{
 AnsiString name;
 switch(type){
  case O_COMBAT:
   name=" ";
//break;   
  case O_FORGET:
   {
    name+=SkillTypes->AbrToName(abr);
   }
  break;
  case O_PRODUCE:
   {
    name=ItemTypes->AbrToName(abr);
	if(!commented && number>=0 && !turnorder){
     name+=" ;+";
     name+=number;
     name+="/";
     name+=maxnum;
    }
   }
  break;
  case O_STUDY:
   {
    name=SkillTypes->AbrToName(abr);
    if(commented)break;
    ASkillType *stype=SkillTypes->Get(abr,2);
    if(!stype)break;
    AUnit *un=orders->unit;
    if(!un || turnorder)break;
    name+=" ;";
    if(un->faction->local)
    {
      name+="-$";
      name+=maxnum;
      name+=":";
    }
    int beg=un->GetBegRealSkill(abr);
    name+=beg;
    name+="+";
    name+=(un->GetRealSkill(abr)-beg);
   }
  break;
  default:
   name=abr;
 }
 return Begin()+FindOrderKey(type)+name;
}
//---------------------------------------------------------------------------
AnsiString AOrderAbr::WriteTemplate()
{
 AnsiString s=Begin()+FindOrderKey(type);
 if(type==O_COMBAT) s+=" ";
 return s+abr;
}
//---------------------------------------------------------------------------
bool AOrderAbr::ParseOrder(char * s)
{
 AnsiString str=GetNextToken(s);

 switch(type){
  case O_COMBAT:
   {
    if(!str.Length()){
     abr="";
     return true;
    }
    ASkillType *stype=SkillTypes->Get(str);
    if(!stype)return false;
    if((stype->flags&ASkillType::MAGICCOMBAT)!=ASkillType::MAGICCOMBAT)return false;
    abr=stype->abr;
   }
  break;
  case O_FORGET:
   {
    ASkillType *stype=SkillTypes->Get(str);
    if(!stype)return false;
    abr=stype->abr;
   }
  break;
  case O_PRODUCE:
   {
    AItemType *itype=ItemTypes->Get(str);
    if(!itype)return false;
    abr=itype->abr;
   }
  break;
  case O_STUDY:
   {
    ASkillType *stype=SkillTypes->Get(str);
    if(!stype)return false;
    abr=stype->abr;
   }
  break;
 }
 return true;
}
//---------------------------------------------------------------------------
void AOrderAbr::Run(int value)
{
 if(commented)return;
 AUnit *un=orders->unit;
 AnsiString mes;
 switch(type){
  case O_COMBAT:return;
  case O_FORGET:
   if(un->GetSkillDays(abr)){
    un->SetSkillDays(abr,0);
    un->endskills->DeleteEmpty();
   }else{
    mes="Error ";
    mes+=un->FullName();
    mes+=" FORGET: unit doesn't know skill";
    AddEvent(mes);
   }
  break;
  case O_PRODUCE:
   {
    AItemType *itype=ItemTypes->Get(abr,4);
    if(!itype) return;
    if(itype->needbuilding.Length())
    {
      if(un->endobj->type!=itype->needbuilding)
      {
        if((value==-1&&itype->inputs[0].Length())
          ||(value!=-1&&!itype->inputs[0].Length()))
        {
          mes="Error ";
          mes+=un->FullName();
          mes+=" PRODUCE: not in building ";
          mes+=itype->needbuilding;
          AddEvent(mes);
        }
        number=0;
        return;
      }
    }
    if(itype->inputs[0].Length()){
     if(value!=-1)
      throw Exception("Error in PRODUCE::run");
     value=Productivity();
     int inputno[AItemType::INPUTCOUNT];
     int present[AItemType::INPUTCOUNT];
     if(itype->type&AItemType::IT_ORINPUTS)
     {
       int count=0;
       for(int i=0;i<AItemType::INPUTCOUNT;i++)
       {
         if(!itype->inputs[i].Length())
           break;
         inputno[i]=itype->numbers[i];
         present[i]=un->enditems->GetNum(itype->inputs[i]);
         count += present[i]/inputno[i];
       }
       if(!count)
       {
         mes="Error ";
         mes+=un->FullName();
         mes+=" PRODUCE: has no materials";
         AddEvent(mes);
         number=0;
         return;
       }
       if(value>count)
         value=count;
       count=value;
       for(int i=0;i<AItemType::INPUTCOUNT&&count;i++)
       {
         if(!itype->inputs[i].Length())
           break;
         int num=present[i]/inputno[i];
         if(count<num)
           num=count;
         count-=num;
         un->enditems->SetNum(itype->inputs[i],present[i]-num*inputno[i]);
         LogItem(4,-num*inputno[i],itype->inputs[i],un);
       }
     }else{

       for(int i=0;i<AItemType::INPUTCOUNT;i++)
       {
         if(!itype->inputs[i].Length()) break;
         inputno[i]=itype->numbers[i];
         present[i]=un->enditems->GetNum(itype->inputs[i]);

         if(present[i] < inputno[i]){
           if(optServerSharing){
             value = curRegion->CheckFactionSharedItems(un, itype->inputs[i], value*inputno[i], 1);
             present[i] = value;
           } else value = 0;

           if(!value){
             mes="Error ";
             mes+=un->FullName();
             mes+=" PRODUCE: has no materials (";
             mes+=ItemTypes->AbrToName(itype->inputs[i]);
             mes+=")";
             AddEvent(mes);
           } else {
             if(value*inputno[i]>present[i]) value=present[i]/inputno[i];
           }
         }else if(value*inputno[i] > present[i]){
           // Есть, но мало
           if(optServerSharing){
             present[i] += curRegion->CheckFactionSharedItems(un, itype->inputs[i], (value*inputno[i]-present[i]), 1);
             if(value*inputno[i] > present[i]) value=present[i]/inputno[i];
           } else value=present[i]/inputno[i];
         }
       }
       for(int i=0;i<AItemType::INPUTCOUNT;i++)
       {
         if(!itype->inputs[i].Length())
           break;
         un->enditems->SetNum(itype->inputs[i],present[i]-value*inputno[i]);
         LogItem(4,-value*inputno[i],itype->inputs[i],un);
       }
     }
    }else if(value<0)return;
    value*=itype->out;
    un->enditems->SetNum(abr,un->enditems->GetNum(abr)+value);
    LogItem(4,value,itype,un);
    number=value;
   }
  break;
  case O_STUDY:
   {
     maxnum = 0;
     if(!un->GetMen()){
       number = 0;
       return;
     }
     if(optMagicDeep)
     {
      if(un->endskills->count>2&&!un->IsLeader()){
       bool err=true;
       foreach(un->endskills)
       {
         ASkill *sk=*(ASkill**)iter;
         if(sk->type->abr!=abr)
           continue;
         err=false;
         break;
       }
       if(err){
        mes="Error : ";
        mes+=un->FullName();
        mes+=" STUDY: Can know only 3 skill";
        AddEvent(mes);
        number=0;
        return;
       }
     }
     } else {
       if(un->endskills->count>0&&!un->IsLeader()){
         if(un->endskills->Get(0)->type->abr!=abr){
           mes = "Error : ";
           mes += un->FullName();
           mes += " STUDY: Can know only 1 skill";
           AddEvent(mes);
           number = 0;
           return;
         }
       }
     }
     if(!un->CanStudy(abr)){
       mes = "Error: ";
       mes += un->FullName();
       mes += " STUDY: Doesn't have the pre-requisite skills to study that";
       AddEvent(mes);
       number = 0;
       return;
     }
     ASkillType *stype=SkillTypes->Get(abr, 2);
     if(!stype)return;
     if((stype->flags&ASkillType::MAGIC)&&un->GetSkill(abr)>=2&&!un->endobj->Protect()){
       mes = "Warning: ";
       mes += un->FullName();
       mes += " STUDY: Magic study rate outside of a tower is cut in half above level 2.";
       AddEvent(mes);
       number /= 2;
     }
     int maxskill = un->GetMaxLevel(abr);
     if(un->GetSkill(abr) >= maxskill){
       mes = "Error : ";
       mes += un->FullName();
       mes += " STUDY: already knows this skills";
       AddEvent(mes);
       number = 0;
       return;
     }
     int cost = un->CalcStudyCost(abr);
     int lastmoney = un->GetMoney() - cost;
     if(lastmoney < 0){
       if(optServerSharing){
         lastmoney += curRegion->CheckFactionSharedItems(un, "SILV", -lastmoney, 0);
       }
       if(lastmoney < 0){
         mes = "Error : ";
         mes += un->FullName();
         mes += " STUDY: Not enough funds ";
         mes += -lastmoney;
         AddEvent(mes);
         return;
       }
     }
     maxnum = cost;
     un->SetMoney(lastmoney);
     LogItem(7, -cost, "SILV", un);
     number /= un->GetMen();
     number += un->GetSkillDays(abr);
     int maxdays = ASkill::LevelToDays(maxskill);
     if(number>maxdays) number = maxdays;
     un->SetSkillDays(abr,number);
   }
   break;
  }
}
//---------------------------------------------------------------------------
class levelsort{
public:
 AnsiString abr;
 AnsiString mes;
 int days;
 int sortindex;
 bool operator<(const levelsort&b) const
 {
   if(days<b.days) return false;
   if(days>b.days) return true;
   return sortindex<b.sortindex;
 }
};
//---------------------------------------------------------------------------
AOrder * AOrderAbr::Edit()
{
 TStringList *names=new TStringList, *abrs=new TStringList;
 AnsiString Caption,mes;
 AUnit *unit=orders->unit;
 switch(type){
  case O_COMBAT:
   Caption="Edit COMBAT order";
   abrs->Add("");
   names->Add("None");
   foreach(unit->skills){
    ASkill *skill=*(ASkill**)iter;
    ASkillType *stype=skill->type;
    if(!stype)throw Exception("Bad skill type");
    if((stype->flags&ASkillType::MAGICCOMBAT)!=ASkillType::MAGICCOMBAT)continue;
    abrs->Add(stype->abr);
    names->Add(skill->Print(1|4));
   }
  break;
  case O_FORGET:
   Caption="Edit FORGET order";
   isedited=true;
   unit->baseobj->basereg->RunOrders(true);
   isedited=false;
//   unit->endskills->DeleteEmpty();
   foreach(unit->endskills){
    ASkill *skill=*(ASkill**)iter;
    ASkillType *stype=skill->type;
    if(!stype)throw Exception("Bad skill type");
    abrs->Add(stype->abr);
    names->Add(stype->name);
   }
  break;
  case O_PRODUCE:
   Caption="Edit PRODUCE order";
   foreach(ItemTypes){
    AItemType *itype=*(AItemType**)iter;
    if(!itype->skill.Length())continue;
    if(itype->level>unit->GetSkill(itype->skill))continue;
    mes=itype->name;
    if(itype->inputs[0].Length()){
     mes+=" (from ";
     for(int i=0;i<AItemType::INPUTCOUNT;i++)
     {
       if(!itype->inputs[i].Length())
         break;
			 if(i)
				 if(!(itype->type&AItemType::IT_ORINPUTS))
					 mes+=" and ";
				 else
					 mes+=" or ";
			 AItem it;
       it.SetType(itype->inputs[i]);
       it.count=itype->numbers[i];
       mes+=it.Print(3);
     }
     mes+=")";
    }else{
		 AMarket *mark=orders->unit->endobj->endreg->products->GetMarket(itype->abr);
		 if(!mark)continue;
     if(!mark->amount)continue;
    }
    if(itype->helpprodmult){
     mes+=" +";
     mes+=itype->helpprodmult;
     if(itype->helpproditem.Length()){
      mes+="*"+ItemTypes->AbrToName(itype->helpproditem);
     }
    }
    abrs->Add(itype->abr);
    names->Add(mes);
   }
  break;
  case O_STUDY:
  {
   Caption="Edit STUDY order";
   DisableEvents();
   unit->baseobj->basereg->RunOrders(true, rsAfterPrepareConsume);
   EnableEvents();
   vector<levelsort> datas;
   foreach(SkillTypes){
    ASkillType *stype=*(ASkillType**)iter;
    AnsiString abr=stype->abr;
    if(!unit->CanStudy(abr))
      continue;
    int maxskilldays=ASkill::LevelToDays(unit->GetMaxLevel(abr));
    int days=unit->GetSkillDays(abr);
    if(days>=maxskilldays)
      continue;
    {
     ASkill sk;
     sk.type=stype;
     sk.days=days;
     mes=sk.Print(1+4+8);
    }
    if(stype->flags&ASkillType::FOUNDATION){
     mes+=" (foundation)";
    }else if(stype->flags&ASkillType::MAGIC){
     mes+=" (magic)";
    }
    levelsort ls;
    ls.abr=abr;
    ls.mes=mes;
    ls.days=days;
    ls.sortindex=stype->sortindex;
    datas.push_back(ls);
   }
   std::sort(datas.begin(),datas.end());
   for(levelsort *ls=datas.begin().operator ->(),*endls=datas.end().operator ->();ls<endls;ls++)
   {
    abrs->Add(ls->abr);
    names->Add(ls->mes);
   }
  }
  break;
  default:
   delete names;
   delete abrs;
   return 0;
 }
 TEditOrderAbrForm *frm=new TEditOrderAbrForm(0,this,names,abrs);
 frm->Caption=Caption;
 frm->ShowModal();
 AOrder *ord=frm->ord;
 delete frm;
 delete names;
 delete abrs;
 return ord;
}
//---------------------------------------------------------------------------
bool AOrderAbr::IsMonthLong()
{
 if(commented)return false;
 if(type==O_PRODUCE||type==O_STUDY)return true;
 return false;
}
//---------------------------------------------------------------------------
bool AOrderAbr::IsTradeOrder()
{
 if(commented)return false;
 if(type==O_PRODUCE)
 {
   AItemType *itype=ItemTypes->Get(abr,4);
   return !(itype->type&AItemType::IT_PRODNOTTRADEACTIVITY);
 }
 return false;
}
//---------------------------------------------------------------------------
int AOrderAbr::Productivity(){
 if(type!=O_PRODUCE)return 0;
 if(number==-1)return -1;
 AUnit *unit=orders->unit;
 AItemType *itype=ItemTypes->Get(abr,4);
 int mens=unit->GetMen();
 int prod=mens*unit->GetSkill(itype->skill);
 if(itype->helpprodmult){
  int bonus=mens;
  if(itype->helpproditem.Length()){
   bonus=unit->enditems->GetNum(itype->helpproditem);
  }
  if(bonus>mens) bonus=mens;
  prod+=bonus*itype->helpprodmult;
 }
 prod/=itype->months;
 maxnum=prod*itype->out;
 return prod;
}
//---------------------------------------------------------------------------
void AOrderAbr::Prepare(){
 number=0;
 if(commented)return;
 if(type!=O_PRODUCE)return;
 AItemType *itype=ItemTypes->Get(abr,4);
 AnsiString mes;
 if(!itype||itype->skill.Length()==0){
  mes=orders->unit->FullName();
  mes+=" PRODUCE: can't produce ";
  if(itype) mes+=itype->name;
  else mes+="nothing";
  AddEvent(mes);
  number=-1;
  return;
 }
 if(itype->inputs[0].Length()==0){
  AMarket *mark=orders->unit->baseobj->basereg->products->GetMarket(abr);
  if(!mark){
   mes=orders->unit->FullName();
   mes+=" PRODUCE: can't produce ";
   mes+=itype->name;
   mes+=" in this region";
   AddEvent(mes);
   number=-1;
   return;
  }
 }
}
//---------------------------------------------------------------------------
AOrderGive::AOrderGive()
 :target(0),count(0),tarnum(0),message(0),item("SILV"),except(0),added(0)
{
 type=O_GIVE;
}
//---------------------------------------------------------------------------
AOrderGive::~AOrderGive()
{
 DeleteMessage(message);
}
//---------------------------------------------------------------------------
void AOrderGive::DeleteMessage(AOrderMessage *mes){
 if(mes!=message)return;
 if(!mes)return;
 mes->base=0;
 mes->orders->Delete(mes);
 message=0;
 target=0;
}
//---------------------------------------------------------------------------
void AOrderGive::SetMessage(){
 if(!message){
  if(!target)return;
  message=new AOrderMessage(this);
  message->target=orders->unit;
  message->subtype=OM_RECEIVED;
  target->orders->Add(message);
 }
 AnsiString s="received ";
 if(item.Length()){
  s+=added;
  s+=" ";
  AnsiString str=ItemTypes->AbrToName(item);
  if(str.Length())s+=str;
  else s+="???";
  s+=" from";
 }else s+=keyUnit;
 s+=" ";
 s+=orders->unit->FullName();
 message->text=s;
}
//---------------------------------------------------------------------------
void AOrderGive::Prepare(){
 added=0;
 AnsiString mes;
 if(!target){
  if(tarnum) target=orders->unit->baseobj->basereg->GetUnit(tarnum);
  if(commented)return;
  if(!target){
   if(tarnum){
    mes="Error ";
    mes+=orders->unit->FullName();
    mes+=" GIVE: unit ";
    mes+=GetAlias(tarnum,orders->unit->faction);
    mes+=" is not found";
    AddEvent(mes);
   }
   return;
  }
 }
 if(message){
  if(commented){ //???
   DeleteMessage(message);
  }
  return;
 }
}
//---------------------------------------------------------------------------
void AOrderGive::Read(TFile & in)
{
 AOrder::Read(in);
 in>>tarnum;
 in>>count;
 if(BaseVer>=32)
   in>>item;
 else
   ReadStringAsInt(in,item);
 in>>except;
}
//---------------------------------------------------------------------------
void AOrderGive::Write(TFile & out)
{
 AOrder::Write(out);
 out<<tarnum;
// if(!tarnum)return;
 out<<count;
 out<<item;
 out<<except;
}
//---------------------------------------------------------------------------
AnsiString AOrderGive::Print()
{
  AnsiString s=Begin()+FindOrderKey(type);
  if(!orders->unit){
    s += GetAlias(tarnum, 0);
    s += " ";
  } else {
    AUnit *un=target;
    if(un)s+=un->FullName()+" "; else if(tarnum==0)
      {
        s += "0 ";
      }
      else
      {
        un=orders->unit->baseobj->basereg->GetUnit(tarnum);
        if(un)
        {
          s += un->FullName()+" ";
        }
        if(!un)
        {
          s+="?"+GetAlias(tarnum,orders->unit->faction)+" ";
        }
      }
  }
 if(item.Length()){
  if(count==-2)
   s+=keyAll;
  else
   s+=count;
  s+=" ";
  AnsiString ss=ItemTypes->AbrToName(item);
  if(ss.Length()) s+=ss;
  else{
   s+="???";
  }
  if(count==-2&&except!=0){
   s+=" "+keyExcept+" ";
   s+=except;
  }
  if(count==-2&&!commented)
  {
    s+=" ;";
    s+=added;
  }
 }else{
  s+=keyUnit;
 }
 return s;
}
//---------------------------------------------------------------------------
AnsiString AOrderGive::WriteTemplate()
{
 AnsiString s=Begin()+FindOrderKey(type);

 if( (!orders) || (!orders->unit)){
   s += GetAlias(tarnum, 0);
   s += " ";
 } else {

   if(!target){
     if(tarnum) target=orders->unit->baseobj->basereg->GetUnit(tarnum);
   }
   if(tarnum == 0){
    s+="0 ";
   }else if(!target){
    if(!commented&&!turnorder){
     AnsiString ss=orders->unit->FullName()+" GIVE: Unknown target ";
     AnsiString alias=GetAlias(tarnum,orders->unit->faction);
     s+=alias+" ";
     ss+=alias;
     AddEvent(ss);
    }else{
     s+=GetAlias(tarnum,orders->unit->faction)+" ";
    }
   }else s+=target->Alias()+" ";
 }

 if(item.Length()){
  if(count==-2)
   s+=keyAll;
  else
   s+=count;
  s+=" ";
  AItemType *itype=ItemTypes->Get(item,4);
  if(itype)s+=item;
  else{
   s=orders->unit->FullName()+" GIVE: unknown item "+item;
   AddEvent(s);
   throw Exception(s);
  }
  if(count==-2&&except!=0){
   s+=" "+keyExcept+" ";
   s+=except;
  }
 }else{
  s+=keyUnit;
 }
 return s;
}
//---------------------------------------------------------------------------
bool AOrderGive::ParseOrder(char * s)
{
 tarnum=ParseAlias(s);
// if(!tarnum)return false;
 while(*s==' ')s++;
 AnsiString str=GetNextToken(s);
 if(!CompareIStr(str,keyUnit)){
  item="";
  return true;
 }
 if(!str.Length())return false;
 if(!CompareIStr(str,keyAll)){
  count=-2;
 }else{
  if(str[1]<'0'||str[1]>'9') return false;
  count=atoi(str.c_str());
 }
 while(*s==' ')s++;
 AnsiString itemname=GetNextToken(s);
 AItemType *itype=ItemTypes->Get(itemname);
 if(!itype)return false;
 item=itype->abr;
 except=0;
 while(*s==' ')s++;
 str=GetNextToken(s);
 if(!CompareIStr(str,keyExcept)){
  if(count==-2){
   while(*s==' ')s++;
   str=GetNextToken(s);
   if(!str.Length())return false;
   if(str[1]<'0'||str[1]>'9') return false;
   except=atoi(str.c_str());
  }else{
   return false;
  }
 }
 if(!optServGiveAll&&count==-2){
  int ret=Application->MessageBox("Give All Except order found.\nActivate Give All Except order?","",MB_YESNO);
  if(ret==IDNO)
   return false;
  optServGiveAll=true; 
 }
 return true;
}
//---------------------------------------------------------------------------
void AOrderGive::Run(int value)
{
 if(commented)return;
 AUnit *un=orders->unit;
 AnsiString mes;
 if(!item.Length()){
  if(!tarnum){
   mes+="Error : ";
   mes+=un->FullName();
   mes+=" GIVE: Can't discard a whole unit";
   AddEvent(mes);
   return;
  }
  if(!target){
   return;
  }
  if(un->endfaction==target->endfaction){
   mes+="Warning : ";
   mes+=un->FullName();
   mes+=" GIVE: unit is given to its own faction";
   AddEvent(mes);
  }
  orders->unit->endfaction=target->endfaction;
  return;
 }
 added=count;
 if(added==-2){
  added=un->enditems->GetNum(item)-except;
  if(added<0){
   added=0;
   mes+="Error : ";
   mes+=un->FullName();
   mes+=" GIVE: EXCEPT value greater than amount on hand.";
   AddEvent(mes);
   return;
  }
 }
 int lastitem=un->enditems->GetNum(item)-added;
 if(lastitem<0){
  mes+="Error : ";
  mes+=un->FullName();
  mes+=" GIVE: Not enough ";
  mes+=-lastitem;
  mes+=" "+ItemTypes->AbrToName(item);
  AddEvent(mes);
  added+=lastitem;
  lastitem=0;
 }
 AItemType *itype=ItemTypes->Get(item,4);
 if(!itype)
  throw Exception("Give: bad type");
 if(tarnum==0){
  un->enditems->SetNum(item,lastitem);
  if(itype->type&AItemType::IT_MAN)
    if(!un->GetMen())
      un->endskills->Clear();
  if(!(optDontShowEvents&dseDiscard))
  {
   mes="Warning: ";
   mes+=un->FullName();
   mes+=" discards ";
   mes+=added;
   mes+=" "+ItemTypes->AbrToNames(item);
   AddEvent(mes);
  }
  return;
 }
 if(itype->flags&AItemType::CANTGIVE){
  mes="Error: ";
  mes+=un->FullName();
  mes+=" GIVE: Can't give ";
  mes+=itype->names;
  AddEvent(mes);
  return;
 }
 if(!target)return;
 if(target==un)
 {
   mes="Error: ";
   mes+=un->FullName();
   mes+=" GIVE to yourself";
   AddEvent(mes);
   return;
 }
 if(un->endfaction!=un->faction){
   mes="Error: ";
   mes+=un->FullName();
   mes+=" can't give after GIVE UNIT";
   AddEvent(mes);
   return;
 }
 if(itype->type&AItemType::IT_MAN){
  if(un->endfaction!=target->endfaction){
   mes="Error: ";
   mes+=un->FullName();
   mes+=" GIVE: Can't give men to another faction";
   AddEvent(mes);
   return;
  }
  int men=target->GetMen();
  foreach(target->endskills){
   ASkill *skill=*(ASkill**)iter;
   skill->days*=men;
  }
  foreach(un->endskills){
   ASkill *skill=*(ASkill**)iter;
   target->SetSkillDays(skill->type->abr,target->GetSkillDays(skill->type->abr)+skill->days*added);
  }
  men+=added;
  if(!men)
   un->endskills->Clear();
  else
  foreach(target->endskills){
   ASkill *skill=*(ASkill**)iter;
   skill->days/=men;
  }
 }
 un->enditems->SetNum(item,lastitem);
 target->enditems->SetNum(item,target->enditems->GetNum(item)+added);
 if(un->faction->local&&!target->endfaction->local&&!target->faction->local)
   LogItem(8,-added,itype,un);
 if(itype->type&AItemType::IT_MAN)
   if(!un->GetMen())
     un->endskills->Clear();
 SetMessage();
}
//---------------------------------------------------------------------------
AOrder * AOrderGive::Edit()
{
 AUnit *unit=orders->unit;
 if(!unit)return 0;
 ARegion *reg=unit->baseobj->basereg;
 if(!reg)return 0;
 isedited=true;
 reg->RunOrders(true);
 isedited=false;
 AItems *items=unit->enditems;
 items->DeleteEmpty(); 
 AUnits *units=new AUnits(orders->unit->baseobj);
 units->autodelete=false;

 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(un==unit)continue;
   units->Add(un);
  }
 }
 TEditOrderGiveForm *frm=new TEditOrderGiveForm(0,this,items,units);
 frm->ShowModal();
 AOrder *ord=frm->ord;
 delete frm;
 delete units;
 return ord;
}
//---------------------------------------------------------------------------
AOrderGive * AOrderGive::CreateCopy()
{
  AOrderGive *ord=new AOrderGive;
  ord->tarnum=tarnum;
  ord->count=count;
  ord->added=added;
  ord->except=except;
  ord->item=item;
  return ord;
}
//---------------------------------------------------------------------------
AOrderMessage::AOrderMessage(AOrder *b)
 :base(b),target(0),subtype(OM_UNK)
{
 type=O_MESSAGE;
}
//---------------------------------------------------------------------------
AOrderMessage::~AOrderMessage()
{
 if(base) base->DeleteMessage(this);
}
//---------------------------------------------------------------------------
void AOrderMessage::Read(TFile & in)
{
// AOrder::Read(in);
}
//---------------------------------------------------------------------------
void AOrderMessage::Write(TFile & out)
{
 WriteIntAsByte(out,type);
// AOrder::Write(out);
}
//---------------------------------------------------------------------------
AnsiString AOrderMessage::Print()
{
 return ";"+text;
}
//---------------------------------------------------------------------------
AnsiString AOrderMessage::WriteTemplate()
{
 return "";
}
//---------------------------------------------------------------------------
AOrderBuySell::AOrderBuySell()
 :amount(0),number(0)
{
}
//---------------------------------------------------------------------------
AOrderBuySell::~AOrderBuySell()
{
}
//---------------------------------------------------------------------------
void AOrderBuySell::Read(TFile & in)
{
 AOrder::Read(in);
 if(BaseVer>=32)
   in>>item;
 else
   ReadStringAsInt(in,item);
 in>>amount;
}
//---------------------------------------------------------------------------
void AOrderBuySell::Write(TFile & out)
{
 AOrder::Write(out);
 out<<item;
 out<<amount;
}
//---------------------------------------------------------------------------
AnsiString AOrderBuySell::Print()
{
 AnsiString s=Begin()+FindOrderKey(type);
 if(amount==-1)
   s+=keyAll;
 else
   s+=amount;
 s+=" ";
 s+=ItemTypes->AbrToName(item);
 if(commented) return s;
 if(!orders->unit) return s;
 ARegion *reg=orders->unit->baseobj->basereg;
 AMarkets *markets;
 if(type==O_BUY) markets=reg->saleds;
 else markets=reg->wanteds;
 int price=0;
 AMarket *mark=markets->GetMarket(item);
 if(mark) price=mark->price;
 price*=number;
 if(price){
  s+=(type==O_BUY?" ;-$":" ;+$");
  s+=price;
  s+="/";
  s+=number;
 }
 return s;
}
//---------------------------------------------------------------------------
AnsiString AOrderBuySell::WriteTemplate()
{
 AnsiString s=Begin()+FindOrderKey(type);
 if(amount==-1)
   s+=keyAll;
 else
   s+=amount;
 s+=" ";
 s+=item;
 return s;
}
//---------------------------------------------------------------------------
bool AOrderBuySell::ParseOrder(char * s)
{
 AnsiString str=GetNextToken(s);
 if(!CompareIStr(str,keyAll))
  amount=-1;
 else
  amount=atoi(str.c_str());
 if(!amount)return false;
 number=amount;
 while(*s==' ')s++;
 AnsiString itemname=GetNextToken(s);
 AItemType *itype=ItemTypes->Get(itemname);
 if(!itype)return false;
 item=itype->abr;
 return true;
}
//---------------------------------------------------------------------------
void AOrderBuySell::Run(int value)
{
 if(commented)return;
 AnsiString mes;
 AUnit *un=orders->unit;
 ARegion *reg=un->baseobj->basereg;
 AMarkets *markets;
 if(type==O_BUY) markets=reg->saleds;
 else markets=reg->wanteds;
 int price=0;
 AMarket *mark=markets->GetMarket(item);
 if(mark) price=mark->price;
 if(type==O_BUY){
  if(un->GetMoney()<price*value){
   int money=un->GetMoney();
   mes="Warning: ";
   mes+=un->FullName();
   mes+=" attempts to buy too many items : needed $";
   mes+=price*value-money;
   value=money/price;
   AddEvent(mes);
  }
  price*=value;
  AItemType *itype=ItemTypes->Get(item,4);
  if(!itype)
   throw Exception("Buy: bad type");
  if(itype->type&AItemType::IT_MAN){
   int men=un->GetMen();
   foreach(un->endskills){
    ASkill *skill=*(ASkill**)iter;
    skill->days*=men;
   }
   men+=value;
   if(!men)
     un->endskills->Clear();
   else foreach(un->endskills){
    ASkill *skill=*(ASkill**)iter;
    skill->days/=men;
   }
  }
  un->enditems->SetNum(item,un->enditems->GetNum(item)+value);
  un->SetMoney(un->GetMoney()-price);
  LogItem(5,value,itype,un);
  LogItem(5,-price,"SILV",un);
 }else{
  int last=un->enditems->GetNum(item)-value;
  price*=value;
  un->enditems->SetNum(item,last);
  un->SetMoney(un->GetMoney()+price);
  LogItem(6,-value,item,un);
  LogItem(6,price,"SILV",un);
 }
 number=value;
}
//---------------------------------------------------------------------------
AOrder * AOrderBuySell::Edit()
{
 AUnit *unit=orders->unit;
 if(!unit)return 0;
 ARegion *reg=unit->baseobj->basereg;
 if(!reg)return 0;
 AMarkets *markets=new AMarkets;
 AnsiString caption,label;
 isedited=true;
 reg->RunOrders(true,rsAfterBuy);
 isedited=false;
 int money=unit->GetMoney();
 if(type==O_BUY){
  caption="Edit BUY order";
  label="buy";
  markets->Update(reg->saleds);
  foreach(markets){
   AMarket *market=*(AMarket**)iter;
   int maxbuy = money/market->price;
   if(maxbuy<market->amount)market->amount=maxbuy;
  }
 }else{
  caption="Edit SELL order";
  label="sell";
  markets->Update(reg->wanteds);
  foreach(markets){
   AMarket *market=*(AMarket**)iter;
   int maxsell=unit->enditems->GetNum(market->type);
   if(maxsell<market->amount)market->amount=maxsell;
  }
  if(amount!=-1)
    markets->DeleteEmpty();
 }
 TEditOrderBuySellForm *frm=new TEditOrderBuySellForm(0,this,markets);
 frm->Label->Caption=label;
 frm->Caption=caption;
 frm->ShowModal();
 AOrder *ord=frm->ord;
 delete frm;
 delete markets;
 return ord;
}
//---------------------------------------------------------------------------
bool AOrderBuySell::IsTradeOrder(){
 if(commented)return false;
 if(type==O_SELL) return false;
 AItemType *itype=ItemTypes->Get(item,4);
 if(!itype) return false;
 if(!(itype->type&AItemType::IT_TRADE)) return false;
 return true;
}
//---------------------------------------------------------------------------
int AOrderBuySell::Productivity()
{
 if(isedited)return -1;
 return number;
}
//---------------------------------------------------------------------------
AOrderTeach::AOrderTeach()
 :targets(new TList),bases(0)
{
 type=O_TEACH;
}
//---------------------------------------------------------------------------
AOrderTeach::~AOrderTeach()
{
 delete bases;bases=0;
 Clear();
 delete targets;
}
//---------------------------------------------------------------------------
void AOrderTeach::Read(TFile & in)
{
 Clear();
 AOrder::Read(in);
 int kol,num;
 in>>kol;
 for(int i=0;i<kol;i++){
  in>>num;
  AddTarget(num,0);
 }
}
//---------------------------------------------------------------------------
void AOrderTeach::Write(TFile & out)
{
 AOrder::Write(out);
 int kol=targets->Count;
 out<<kol;
 for(int i=0;i<kol;i++){
  AUnitPtr *ptr=GetTarget(i);
  out<<ptr->num;
 }
}
//---------------------------------------------------------------------------
AnsiString AOrderTeach::Print()
{
 AnsiString s=Begin()+FindOrderKey(type);
 int kol=targets->Count;
 for(int i=0;i<kol;i++){
  AUnitPtr *ptr=GetTarget(i);
  AUnit *un=ptr->ptr;
  if(un)s+=un->FullName();
  else{
 //  if(commented){
    un=orders->unit->baseobj->basereg->GetUnit(ptr->num);
    if(un){
     s+=un->FullName();
    }
 //  }
   if(!un){
    s+="?"+GetAlias(ptr->num,orders->unit->faction);
   }
  }
  s+=" ";
 }
 s.SetLength(s.Length()-1);
 return s;
}
//---------------------------------------------------------------------------
AnsiString AOrderTeach::WriteTemplate()
{
  AnsiString s = Begin()+FindOrderKey(type);
  int oldlength = s.Length();
  int kol = targets->Count;
  for(int i=0;i<kol;i++)
  {
	if(s.Length()-oldlength>60)
	{
	  s.SetLength(s.Length()-1);
	  s+="\n"+Begin()+FindOrderKey(type);
	  oldlength=s.Length();
	}
	AUnitPtr *ptr=GetTarget(i);
	AUnit *target=ptr->ptr;
	if(!target)
	{
      if(orders)
	  if(orders->unit)
		if(ptr->num) target=orders->unit->baseobj->basereg->GetUnit(ptr->num);
	}
	if( (!target) && (orders) )
	{
	  if(!commented && !turnorder)
	  {
		AnsiString ss=orders->unit->FullName();
		if(type==O_ATTACK) ss += " ATTACK: Unknown target ";
		  else ss += " TEACH: Unknown target ";
		AnsiString alias = GetAlias(ptr->num,orders->unit->faction);
		ss += alias;
		s += alias;
		AddEvent(ss);
	  }else{
		if(orders->unit) s += GetAlias(ptr->num,orders->unit->faction);
		  else s += ptr->num;
	  }
	} else if (target) s += target->Alias();
	else s += ptr->num;
	s+=" ";
  }
  s.SetLength(s.Length()-1);
  return s;
}
//---------------------------------------------------------------------------
bool AOrderTeach::ParseOrder(char * s)
{
 int tarnum=ParseAlias(s);
 if(!tarnum)return false;
 while(1){
  AddTarget(tarnum,0);
  if(*s==0)break;
  if(*s!=' ') return false;
  while(*s==' ')s++;
  if(*s==0)break;
  tarnum=ParseAlias(s);
  if(!tarnum)return false;
 }
 return true;
}
//---------------------------------------------------------------------------
void AOrderTeach::Run(int value)
{
 AnsiString mes;
 AUnit *unit=orders->unit;
 if(!unit->IsLeader()){
  mes=unit->FullName()+" TEACH: Only leaders can teach";
  AddEvent(mes);
  return;
 }
 int students=0;
 for(int i=0;i<targets->Count;i++){
  AUnitPtr *ptr=GetTarget(i);
  AUnit *target=ptr->ptr;
  if(!target){
   targets->Delete(i);i--;
   delete ptr;
   continue;
  }
  AOrderAbr *study=dynamic_cast<AOrderAbr*>(target->orders->Find(O_STUDY,2));
  if(!study){
   if(!(optDontShowEvents&dseTeachNonLocNotStudy)||target->IsLocal())
   {
     mes=unit->FullName()+" TEACH error: unit ";
     mes+=target->FullName()+" is not studying";
     AddEvent(mes);
   }
   targets->Delete(i);i--;
   delete ptr;
   continue;
  }
  AnsiString skill=study->abr;
  if(unit->GetRealSkill(skill)<=target->GetRealSkill(skill)){
   mes=unit->FullName()+" TEACH error: unit ";
   mes+=target->FullName()+" is not studying a skill you can teach";
   AddEvent(mes);
   targets->Delete(i);i--;
   delete ptr;
   continue;
  }

  int maxdays=ASkill::LevelToDays(target->GetMaxLevel(skill));
  if(target->GetSkillDays(skill)>=maxdays-StudyAddDays){
   mes=unit->FullName()+" TEACH warning: unit ";
   mes+=target->FullName()+" does not need teaching";
   AddEvent(mes);
  }
  students+=target->GetMen();
 }
 if(!students)return;
 int maxstud=unit->GetMen()*TeachMaxStudents;
 if(students>maxstud){
  mes=unit->FullName()+" TEACH warning: too many students";
  AddEvent(mes);
 }
 int days=TeachAddDays*maxstud,dif;

 for(int i=0;i<targets->Count;i++){
  AUnitPtr *ptr=GetTarget(i);
  AUnit *u=ptr->ptr;
  int umen=u->GetMen();
  if(!umen)continue;
  int tempdays=(double(umen)*days)/students;
  if(tempdays>TeachAddDays*umen) tempdays=TeachAddDays*umen;
  days-=tempdays;
  students-=umen;
  AOrderAbr *o=dynamic_cast<AOrderAbr*>(u->orders->Find(O_STUDY,2));
  o->number+=tempdays;
  if(o->number>(TeachAddDays+StudyAddDays)*umen){
   dif=o->number-(TeachAddDays+StudyAddDays)*umen;
   if(tempdays==dif){
    mes=unit->FullName()+" TEACH error: unit ";
    mes+=u->FullName()+" is already teached";
    AddEvent(mes);
   }
   days+=dif;
   o->number=(TeachAddDays+StudyAddDays)*umen;
  }else dif=0;
  dif=(tempdays-dif)/umen;
  AOrderMessage *m=new AOrderMessage(0);
  if(bases){
   foreach(bases){
    AOrder *o=*(AOrder**)iter;
    AOrderTeach *ord=dynamic_cast<AOrderTeach*>(o);
    for(int i=0;i<ord->targets->Count;i++){
     AUnitPtr *ptr2=ord->GetTarget(i);
     if(ptr->num!=ptr2->num)continue;
     m->base=ord;
     ptr2->mes=m; 
     break;
    }
    if(m->base)break;
   }
  }
  mes="Taught by ";
  mes+=unit->FullName();
  mes+=" ";
  mes+=dif;
  mes+=" days";
  m->text=mes;
  m->target=unit; 
  m->subtype=OM_TAUGTH;
  u->orders->Add(m);
  
//
 }
}
//---------------------------------------------------------------------------
AOrder * AOrderTeach::Edit()
{
 AUnit *unit=orders->unit;
 if(!unit)return 0;
 ARegion *reg=unit->baseobj->basereg;
 if(!reg)return 0;
 AUnits *units=new AUnits(orders->unit->baseobj);
 units->autodelete=false;
 AUnits *newun=new AUnits(orders->unit->baseobj);
 isedited=true;
 reg->RunOrders(true);
 isedited=false;
 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(un==unit)continue;
/*   if(unit->endfaction){
    if(!un->endfaction)continue;*/
   if(unit->endfaction->local&&unit->endfaction->GetAttitude(un->endfaction->number)<A_FRIENDLY)continue;
//   }
   if(unit->endobj->endreg!=un->endobj->endreg) continue;
   AOrderAbr *study=dynamic_cast<AOrderAbr*>(un->orders->Find(O_STUDY,2));
   if(!study)continue;
   AnsiString skill=study->abr;
   if(unit->GetRealSkill(skill)<=un->GetRealSkill(skill)) continue;
   units->Add(un);
  }
 }
 for(int i=targets->Count-1;i>=0;i--){
  AUnitPtr *ptr=GetTarget(i);
  if(ptr->ptr) continue;
  if(newun->GetUnit(ptr->num))continue;
  AUnit *un=reg->GetUnit(ptr->num);
  if(!un){
   un=new AUnit(unit->turn);
   un->num=ptr->num;
   newun->Add(un);
  }
  ptr->ptr=un;
 }
 TEditOrderTeachForm *frm=new TEditOrderTeachForm(0,this,units);
 frm->ShowModal();
 AOrder *ord=frm->ord;
 delete frm;
 delete newun;
 for(int i=targets->Count-1;i>=0;i--){
  AUnitPtr *ptr=GetTarget(i);
  ptr->ptr=0;
 }
 delete units;
 return ord;
}
//---------------------------------------------------------------------------
void AOrderTeach::DeleteMessage(AOrderMessage *mes){
 AUnit *tar=mes->orders->unit;
 for(int i=targets->Count-1;i>=0;i--){
  AUnitPtr *ptr=GetTarget(i);
  if(ptr->ptr==tar){
   ptr->ptr=0;
   if(ptr->mes!=mes){
    ptr->mes->orders->Delete(ptr->mes);
   }
   ptr->mes=0;
   mes->base=0;
   mes->orders->Delete(mes);
  }
 }
}
void AOrderTeach::Clear()
{
 for(int i=targets->Count-1;i>=0;i--){
  AUnitPtr *ptr=GetTarget(i);
  targets->Delete(i);
  if(ptr->mes){
   ptr->mes->base=0;
   ptr->mes->orders->Delete(ptr->mes);
  }
  delete ptr;
 }
}
AUnitPtr * AOrderTeach::GetTarget(int index)
{
 return (AUnitPtr*)targets->Items[index];
}
void AOrderTeach::AddTarget(int num, AUnit *target)
{
 if(!target){
  if(orders)target=orders->unit->baseobj->basereg->GetUnit(num);
 }
 for(int i=targets->Count-1;i>=0;i--){
  AUnitPtr *ptr=GetTarget(i);
  if(target&&ptr->ptr==target)return;
  if(ptr->num==num){
   ptr->ptr=target;
   return;
  }
 }
 AUnitPtr *ptr=new AUnitPtr;
 ptr->ptr=target;
 ptr->num=num;
 ptr->mes=0;
 targets->Add(ptr);
}
void AOrderTeach::VerTargets()
{
 for(int i=targets->Count-1;i>=0;i--){
  AUnitPtr *ptr=GetTarget(i);
  if(ptr->ptr) continue;
  ptr->ptr=orders->unit->baseobj->basereg->GetUnit(ptr->num);
  if(ptr->ptr) continue;
  if(ptr->mes){
   ptr->mes->orders->Delete(ptr->mes);
   ptr->mes=0;
  }
  AnsiString mes;
  mes=orders->unit->FullName();
  if(type==O_ATTACK)
    mes+=" ATTACK Error: unit ";
  else
    mes+=" TEACH Error: unit ";
  mes+=GetAlias(ptr->num,orders->unit->faction)+" is not found";
  AddEvent(mes);
 }
}
bool AOrderTeach::IsMonthLong(){
 return !commented;
}
void AOrderTeach::Prepare(){
 VerTargets();
 int kol=0;
 for(int i=targets->Count-1;i>=0;i--){
  if(GetTarget(i)->ptr)kol++;
 }
 if(!kol){
  AddEvent(orders->unit->FullName()+" TEACH: No students");
 }
}
AOrderInt::AOrderInt()
 :value(0)
{
}
AOrderInt::~AOrderInt()
{
}
void AOrderInt::Read(TFile & in)
{
 AOrder::Read(in);
 in>>value;
}
void AOrderInt::Write(TFile & out)
{
 AOrder::Write(out);
 out<<value;
}
AnsiString AOrderInt::Print()
{
 AnsiString s=Begin()+FindOrderKey(type);
 if(type==O_CLAIM){
  s+=value;
  return s;
 }
 if(type==O_ENTER){
   if(!orders->unit){
     s+=value;
     return s;
   }
  AObject *obj=orders->unit->baseobj->basereg->GetObject(value);
  if(value&&obj) s+=obj->FullName();
  else{
   s+="?";
   s+=value;
  }
  return s;
 }
 if(type==O_PROMOTE)
 {
   AUnit *un=orders->unit->baseobj->basereg->GetUnit(value);
   if(un){
     s+=un->FullName();
   }else{
     s+="?"+GetAlias(value,orders->unit->faction);
   }
 }
 if(commented) return s;

 return s;
}
AnsiString AOrderInt::WriteTemplate()
{
 AnsiString s=Begin()+FindOrderKey(type);
 if(type==O_PROMOTE)
 {
    AUnit *un=orders->unit->baseobj->basereg->GetUnit(value);
    if(!un)
      s+=GetAlias(value,orders->unit->faction);
    else
      s+=un->Alias();
    return s;
 }
 if(type==O_CLAIM||type==O_ENTER){
  s+=value;
  return s;
 }

 return s;
}
bool AOrderInt::ParseOrder(char * s)
{
 if(type==O_PROMOTE)
 {
   value=ParseAlias(s);
   return true;
 }
 if(type==O_CLAIM||type==O_ENTER){
  AnsiString str=GetNextToken(s);
  if(!str.Length()) return false;
  value=atoi(str.c_str());
  return true;
 }
 return false;
}
void AOrderInt::Run(int val)
{
 AnsiString mes;
 if(type==O_CLAIM)
 {
   AUnit *u=orders->unit;
   u->SetMoney(u->GetMoney()+value);
   ARegion *reg=u->baseobj->basereg;
   int facnum=0;
   if(u->faction)
   {
     if(!u->faction->local) return;
     facnum=u->faction->number;
   }
   reg->claims[facnum]+=value;
   LogItem(11,value,"SILV",u);
   return;
 }
 if(type==O_ENTER){
  AUnit *un=orders->unit;
  AObject *obj=un->endobj;
  if(obj->owner==un)
    obj->SetNextOwner();
  obj=un->baseobj->basereg->GetObject(value);
  if(obj)
  {
    un->endobj=obj;
    if(!obj->owner)
      obj->owner=un;
  }
  else{
   mes="Error ";
   mes+=orders->unit->FullName();
   mes+=" ENTER: object №";
   mes+=value;
   mes+=" does not exist";
   AddEvent(mes);
  }
  return;
 }
 if(type==O_PROMOTE)
 {
   AObject *obj=orders->unit->endobj;
   if(obj->num==0)
   {
     mes="Error ";
     mes+=orders->unit->FullName();
     mes+=" PROMOTE: can't promote outside object";
     AddEvent(mes);
     return;
   }
   if(obj->GetOwner()!=orders->unit)
   {
     mes="Error PROMOTE: ";
     mes+=orders->unit->FullName();
     mes+=" is not owner";
     AddEvent(mes);
     return;
   }
   AUnit *targ=obj->basereg->GetUnit(value);
   if(!targ)
   {
     mes="Error ";
     mes+=orders->unit->FullName();
     mes+=" PROMOTE: unit ";
     mes+=GetAlias(value,orders->unit->faction);
     mes+=" is not found";
     AddEvent(mes);
     return;
   }
   if(targ->endobj!=obj)
   {
     mes="Error ";
     mes+=orders->unit->FullName();
     mes+=" PROMOTE: ";
     mes+=targ->FullName();
     mes+=" not in ";
     mes+=obj->FullName();
     AddEvent(mes);
     return;
   }
   obj->owner=targ;
   return;
 }
}
AOrder * AOrderInt::Edit()
{
 AnsiString text,caption;
 AOrder *ord=0;
 switch(type){
  case O_CLAIM:
  {
   TEditOrderIntForm *frm=new TEditOrderIntForm(0,this);
   frm->Caption="Edit CLAIM order";
   text.printf("Money (%d)",orders->unit->faction->unclaimed);
   frm->labText->Caption=text;
   frm->ShowModal();
   ord=frm->ord;
   delete frm;
  }
  break;
  case O_ENTER:
  {
	 TEditOrderEnterForm *frm=new TEditOrderEnterForm(0,this);
	 frm->ShowModal();
	 ord=frm->ord;
	 delete frm;
	}
  break;
  case O_PROMOTE:
  {
   AUnit *unit=orders->unit;
   if(!unit)return 0;
   ARegion *reg=unit->baseobj->basereg;
   if(!reg)return 0;
   reg->RunOrders(true,rsAfterEnter);
   AObject *bobj=unit->endobj;
   AUnits *units=new AUnits(bobj);
   units->autodelete=false;

   foreach(reg){
    AObject *obj=*(AObject**)iter;
    foreach(obj->units){
     AUnit *un=*(AUnit**)iter;
     if(un==unit)continue;
     if(un->endobj!=bobj) continue;
     units->Add(un);
    }
   }
   TEditOrderPromoteForm *frm=new TEditOrderPromoteForm(0,this,units);
   frm->ShowModal();
   ord=frm->ord;
   delete frm;
   delete units;
   break;
  }
 }
 return ord;
}
int AOrderInt::Productivity()
{
 return 0;
}
AOrderMoving::AOrderMoving()
  :moved(0)
{
 dirs=new TList;
}
AOrderMoving::~AOrderMoving()
{
 delete dirs;
}
void AOrderMoving::Read(TFile & in)
{
 AOrder::Read(in);
 int kol,dir;
 in>>kol;
 for(int i=0;i<kol;i++){
  in>>dir;
  Add(dir);
 }
 in>>custom;
}
void AOrderMoving::Write(TFile & out)
{
 AOrder::Write(out);
 int kol=count,dir;
 out<<kol;
 for(int i=0;i<kol;i++){
  dir=Get(i);
  out<<dir;
 }
 out<<custom;
}
AnsiString AOrderMoving::Print()
{
 return WriteTemplate();
}
AnsiString AOrderMoving::WriteTemplate()
{
 AnsiString s=Begin()+FindOrderKey(type);
 if(type==O_SAIL) s+=" ";
 for(int i=0;i<count;i++){
  if(i) s+=" ";
  int dir=Get(i);
  if(dir<MOVE_IN){
   ADirType *dtype=DirTypes->Get(dir);
   if(dtype)
    s+=dtype->abr; 
  }else if(dir==MOVE_IN){
   s+=keyIn;
  }else if(dir==MOVE_OUT){
   s+=keyOut;
  }else{
   s+=dir-MOVE_OUT;
  }
 }
 if(custom.Length()){
  s+=" "+custom;
 }
 return s.TrimRight();
}
bool AOrderMoving::ParseOrder(char *s)
{
 AnsiString str;
 int dir;
 while(*s){
  str=GetNextToken(s);
  if(!str.Length()) return false;
  if(isdigit(str[1])){
   dir=MOVE_OUT+atoi(str.c_str());
  }else if(!str.AnsiCompareIC(keyIn)){
   dir=MOVE_IN;
  }else if(!str.AnsiCompareIC(keyOut)){
   dir=MOVE_OUT;
  }else{
   dir=DirTypes->Find(str);
   if(dir<0) return false;
  }
  Add(dir);
  while(*s==' ') s++;
 }
 if(type!=O_SAIL&&!count) return false;
 return true;
}
int movingdx[NDIRS]={
 0, 1, 1, 0, -1, -1
};
int movingdy[NDIRS]={
 -2, -1, 1, 2, 1, -1
};
void AOrderMoving::Run(int value)
{
 AnsiString mes;
 int dir;
 AUnit *unit=orders->unit;
 AObject *curobj=unit->endobj, *nextobj;
 ARegion *nextreg, *curreg=curobj->endreg;
 int movecost;
 moved=0;
 int _type=type;
 if(_type==O_AUTOMOVE)
   _type=issail?O_SAIL:O_MOVE;
 if(_type==O_SAIL){
  if(!count){
   if(!isedited){
    mes="Error Sailing: ";
    mes+=unit->FullName();
    mes+=" has empty sailing order";
    AddEvent(mes);
   }
//   return;
  }
  AUnits *units=new AUnits(curobj);
  units->autodelete=false;
  int movepoints;
  if(type==O_AUTOMOVE)
  {
    movepoints=1000;
    curreg=curobj->endreg=curobj->basereg;
  }
  else
  {
    movepoints=unit->CalcMovePoints(M_SAIL);
    int wgt=0,slr=0;
    foreach(curobj->basereg)
    {
      AObject *obj=*(AObject**)iter;
      foreach(obj->units){
       AUnit *un=*(AUnit**) iter;
       if(un->endobj!=curobj) continue;
       wgt+=un->weight;
       if(!un->orders->Find(O_SAIL,2)) continue;
       slr+=un->GetSkill(SailSkill)*un->GetMen();
       units->Add(un);
      }
    }
    curobj->cursailors=slr;
    if(wgt>curobj->Capacity()){
     mes="Error SAIL: ";
     mes+=unit->FullName();
     mes+=": Ship is overloaded";
     AddEvent(mes);
     movepoints=0;
    }else  if(slr<curobj->Sailors()){
     mes="Error SAIL: ";
     mes+=unit->FullName();
     mes+=": Not enough sailors";
     AddEvent(mes);
     movepoints=0;
    }
  }
  if(movepoints) for(int i=0;i<count;i++){
   dir=Get(i);
   if(dir>=NDIRS){
    mes="Error SAIL: ";
    mes+=unit->FullName();
    mes+=" must be moved in normal direction";
    AddEvent(mes);
    break;
   }
   if(curreg->hasInfo){
    ALocation loc=curreg->neighbors[dir];
    if(loc.xloc==-1){
      mes="Error SAIL: ";
      mes+=unit->FullName();
      mes+=": Can't sail that way";
      AddEvent(mes);
      break;
    }
    nextreg=curRegionList->Get(loc);
   }else{
    nextreg=0;
   }
   if(!nextreg){
    int newx=curreg->xloc+movingdx[dir];
    int newy=curreg->yloc+movingdy[dir];
    ARegionArray *ar=curRegionList->GetRegionArray(curreg->zloc);
    newx=(newx+ar->x)%ar->x;
    newy=(newy+ar->y)%ar->y;
    nextreg=curRegionList->Get(newx, newy,curreg->zloc);
    if(!nextreg){
     mes="Error SAIL: ";
     mes+=unit->FullName();
     mes+=": region does not exist or is not explored";
     AddEvent(mes);
     break;
    }
   }
   if(curobj->type!="Balloon"&&!nextreg->IsCoastal()){
    mes="Error SAIL: ";
    mes+=unit->FullName();
    mes+=": can't sail inland";
    AddEvent(mes);
    break;
   }
   if(curobj->type!="Balloon"&&!TerrainTypes->IsOcean(curreg->type)&&!TerrainTypes->IsOcean(nextreg->type)){
    mes="Error SAIL: ";
    mes+=unit->FullName();
    mes+=": can't sail inland";
    AddEvent(mes);
    break;
   }
   movecost=Plugins.CalcMoveCost(unit,nextreg,dir,M_SAIL);
   if(movecost<0)
     movecost=nextreg->MoveCost(M_SAIL);
   movepoints-=movecost;
   if(movepoints<0){
    if(optDontShowEvents&dseCantReach)
      break;
    mes="Error SAIL: ";
    mes+=unit->FullName();
    mes+=" can't sail that far";
    AddEvent(mes);
    break;
   }
   curreg=nextreg;
   moved=i+1;
  }
  curobj->endreg=curreg;

//for sail editing
  foreach(units){
   AUnit *un=*(AUnit**) iter;
   un->movepoints=movepoints;
  }
  delete units;
  return;
 }
 int movetype=unit->MoveType();
 if(type==O_AUTOMOVE)
 {
   unit->movepoints=10000;
   curobj=unit->endobj=unit->baseobj;
 }else
   unit->movepoints=unit->CalcMovePoints(movetype);
 if(curobj->endreg!=curobj->basereg){
  unit->movepoints=0;
  mes="Error MOVE: ";
  mes+=unit->FullName();
  mes+=" can't move while aboard a sailing ship";
  AddEvent(mes);
  return;
 }
 if(!count){
  if(isedited) return;
  mes="Error MOVE: ";
  mes+=unit->FullName();
  mes+=" has empty MOVE order";
  AddEvent(mes);
  return;
 }
 if(type==O_ADVANCE)
 {
   if(unit->endfaction!=unit->faction)
   {
     mes="Warning ADVANCE: ";
     mes+=unit->FullName();
     mes+=" cannot advance after being given if no ally";
     AddEvent(mes);
   }
 }
 bool firstmove=true;
 for(int i=0;i<count;i++){
  dir=Get(i);
  if(dir<NDIRS||dir==MOVE_IN){
   if(dir==MOVE_IN){
    if(!curobj->inner){
     mes="Error MOVE: ";
     mes+=unit->FullName();
     mes+=": Can't move IN there";
     AddEvent(mes);
     break;
    }
    nextreg=curRegionList->Get(curobj->innerloc);
    if(!nextreg){
     mes="Error MOVE: ";
     mes+=unit->FullName();
     mes+=": an unknown inner region";
     AddEvent(mes);
     break;
    }
   }else{
    curreg=curobj->endreg;
    if(TerrainTypes->IsOcean(curreg->type)&&
     (!unit->CanSwim()||unit->GetEndFlag(FLAG_NOCROSS_WATER))){
      mes="Error MOVE: ";
      mes+=unit->FullName();
      mes+=": Can't move while in the ocean";
      AddEvent(mes);
      break;
    }
    if(curreg->hasInfo){
     ALocation loc=curreg->neighbors[dir];
     if(loc.xloc==-1){
       mes="Error MOVE: ";
       mes+=unit->FullName();
       mes+=": Can't move that direction";
       AddEvent(mes);
       break;
     }
     nextreg=curRegionList->Get(loc);
    }else{
     nextreg=0;
    }
    if(!nextreg){
     int newx=curobj->endreg->xloc+movingdx[dir];
     int newy=curobj->endreg->yloc+movingdy[dir];
     ARegionArray *ar=curRegionList->GetRegionArray(curobj->endreg->zloc);
     newx=(newx+ar->x)%ar->x;
     newy=(newy+ar->y)%ar->y;
     nextreg=curRegionList->Get(newx, newy,curobj->endreg->zloc);
     if(!nextreg){
      mes="Error MOVE: ";
      mes+=unit->FullName();
      mes+=": region does not exist or not explored";
      AddEvent(mes);
      break;
     }
    }
   }
   if(TerrainTypes->IsOcean(nextreg->type)&&
	(!unit->CanSwim()||unit->GetEndFlag(FLAG_NOCROSS_WATER))){
     mes="Error MOVE: ";
     mes+=unit->FullName();
     mes+=": Can't move to the ocean";
     AddEvent(mes);
     break;
   }
   movecost=Plugins.CalcMoveCost(unit,nextreg,dir,movetype);
   if(movecost<0)
   {
     movecost=nextreg->MoveCost(movetype);
     if(movecost>1&&movetype!=M_FLY&&curobj->endreg->HasExitRoad(dir) &&
              nextreg->HasConnectingRoad(dir))
     {
      movecost/=2;
     }
   }
   if((unit->movepoints-=movecost)<0){
    mes="Error MOVE: ";
    mes+=unit->FullName();
    if(firstmove)
      mes+=" can't move";
    else if(optDontShowEvents&dseCantReach)
      break;
    else
      mes+=" can't reach destination";
    AddEvent(mes);
    break;
   }
// to do
   firstmove=false;
   curobj=nextreg->GetDummy();
   moved=i+1;
  }else if(dir<MOVE_IN){
   mes="Error MOVE: ";
   mes+=unit->FullName();
   mes+=" unknown direction";
   AddEvent(mes);
   break;
  }else if(dir==MOVE_OUT){
   if(TerrainTypes->IsOcean(curobj->endreg->type)&&
	(!unit->CanSwim()||unit->GetEndFlag(FLAG_NOCROSS_WATER))){
    mes="Error MOVE: ";
    mes+=unit->FullName();
    mes+=": Can't leave ship in the ocean";
    AddEvent(mes);
    break;
   }
   curobj=curobj->endreg->GetDummy();
  }else{
   int bnum=dir-MOVE_OUT;
   nextobj=curobj->endreg->GetObject(bnum);
   if(!nextobj){
    mes="Error MOVE: ";
    mes+=unit->FullName();
    mes+=": object №";
    mes+=bnum;
    mes+=" does not exist";
    AddEvent(mes);
    break;
   }
   curobj=nextobj;
  }
 }
 unit->endobj=curobj;
 if(curobj->num==0&&TerrainTypes->IsOcean(curobj->endreg->type)){
  bool drown=true;
  switch(optServFlightOverWater){
   case WFLIGHT_UNLIMITED:
    drown=!unit->CanSwim();
   break;
   case WFLIGHT_NONE:case WFLIGHT_MUST_LAND:
    drown=!unit->CanReallySwim();
   break;
  }
  if(drown&&type!=O_AUTOMOVE){
   mes="Warning MOVE: ";
   mes+=unit->FullName();
   mes+=": Drowns in the ocean";
   AddEvent(mes);
  }
 }
 if(custom.Length()){
  mes="Warning MOVE: ";
  mes+=unit->FullName();
  mes+=" has custom MOVE order";
  AddEvent(mes);
 }
}
AOrder * AOrderMoving::Edit()
{
 AOrder *ord;
 TEditOrderMovingForm *frm=new TEditOrderMovingForm(0,this);
 frm->ShowModal();
 ord=frm->ord;
 delete frm;
 return ord;
}
int AOrderMoving::Get(int ind)
{
 return (int)dirs->Items[ind];
}
void AOrderMoving::Add(int dir)
{
 dirs->Add((void*)dir);
}
void AOrderMoving::Delete(int ind)
{
 dirs->Delete(ind);
}
bool AOrderMoving::IsMonthLong()
{
 return !commented;
}

AOrderAutoGive::AOrderAutoGive()
 :number(0),item("SILV")
{
 type=O_AUTOGIVE;
}
AOrderAutoGive::~AOrderAutoGive()
{
}
void AOrderAutoGive::Read(TFile & in)
{
// AOrder::Read(in);
 in>>number;
 if(BaseVer>=32)
   in>>item;
 else
   ReadStringAsInt(in,item);
 in>>begstr;
 in>>substr;
}
void AOrderAutoGive::Write(TFile & out)
{
 WriteIntAsByte(out,type);
// AOrder::Write(out);
 out<<number;
 out<<item;
 out<<begstr;
 out<<substr;
}
AnsiString AOrderAutoGive::Print()
{
 AnsiString s=keyAutoOrder+FindOrderKey(O_GIVE);
 if(begstr.Length()){
  s+='\"';
  s+=begstr+'\"';
 }
 if(substr.Length()){
  s+="+\""+substr+'\"';
 }
 s+=' ';
 if(number==0){
  s+=keyAll;
 }else if(number<0){
  s+=keyExcept+" ";
  s+=-number;
 }else{
  s+=number;
 }
 s+=' ';
 AItemType *itype=ItemTypes->Get(item,4);
 if(!itype){
  s+=item;
 }else{
  if(itype->names.Length())
   s+=itype->names;
  else
   s+=itype->name;
 }
 s+=" (autoorder)";
 return s;
}
AnsiString AOrderAutoGive::WriteTemplate()
{
 AnsiString s=keyAutoOrder+FindOrderKey(O_GIVE);
 if(begstr.Length()){
  s+='\"';
  s+=begstr+'\"';
 }else if(!substr.Length()){
  s=orders->unit->FullName()+": AUTOGIVE: no target selected";
  AddEvent(s);
  throw Exception(s);
 }
 if(substr.Length()){
  s+="+\""+substr+'\"';
 }
 s+=' ';
 s+=number;
 s+=' ';
/* AItemType *itype=ItemTypes->Get(item);
 if(!itype){
  s+=item;
 }else{*/
 s+=item;
// }
 return s;
}
bool AOrderAutoGive::ParseOrder(char * s)
{
 begstr.SetLength(0);
 if(*s=='\"'){
  begstr=GetNextToken(s);
 }
 if(*s=='+'&&s[1]=='\"'){
  s++;
  substr=GetNextToken(s);
 }else if(!begstr.Length()) return false;
 if(*s!=' ') return false;
 s++;
 number=atoi(GetNextToken(s).c_str());
 if(*s!=' ') return false;
 s++;
 AnsiString it=GetNextToken(s);
 AItemType *itype=ItemTypes->Get(it);
 if(!itype) return false;
 item=itype->abr;
 return true;
}
/*void AOrderAutoGive::Run(int value)
{
}*/
AOrder * AOrderAutoGive::Edit()
{
 AOrder *ord;
 TEditOrderAutoGiveForm *frm=new TEditOrderAutoGiveForm(0,this);
 frm->ShowModal();
 ord=frm->ord;
 delete frm;
 return ord;
}
AOrderBuild::AOrderBuild()
 :work(0)
{
 type=O_BUILD;
}
AOrderBuild::~AOrderBuild()
{
}
void AOrderBuild::Read(TFile & in)
{
 AOrder::Read(in);
 in>>object;
}
void AOrderBuild::Write(TFile & out)
{
 AOrder::Write(out);
 out<<object;
}
AnsiString AOrderBuild::Print()
{
 AnsiString s=WriteTemplate();
 if(!commented){
  s+=" ;";
  s+=work;
 }
 return s;
}
AnsiString AOrderBuild::WriteTemplate()
{
 AnsiString s=Begin()+FindOrderKey(O_BUILD);
 if(object.Length()==0) return s;
 s+=" \""+object+"\"";
 return s;
}
bool AOrderBuild::ParseOrder(char * s)
{
 if(*s==0){
  object="";
  return true;
 }
 AnsiString objname=GetNextToken(s);
 AObjectType *otype=ObjectTypes->Get(objname);
 if(!otype) return false;
 object=otype->name;
 while(*s==' ')s++;
 return *s==0;
}
void AOrderBuild::Run(int value)
{
 work=0;
 AnsiString mes;
 AObject *obj=0;
 AObjectType *otype;
 int needed;
 AUnit *unit=orders->unit;
 if(!object.Length()){
  obj=unit->endobj;
  otype=ObjectTypes->Get(obj->type);
  needed=obj->endincomplete;
 }else{
  otype=ObjectTypes->Get(object);
  needed=otype->cost;
  if(unit->endobj->endreg!=unit->baseobj->basereg)
  {
    mes="Error BUILD: ";
    mes+=unit->FullName();
    mes+=" can't build from sailing ship";
    AddEvent(mes);
    return;
  }
  if(unit->endobj->owner==unit)
    unit->endobj->SetNextOwner();
  unit->endobj=unit->baseobj->basereg->GetDummy();
 }
 if(!otype){
  mes="Error BUILD: ";
  mes+=unit->FullName();
  mes+=" internal error";
  AddEvent(mes);
  return;
 }
 if(otype->road&&needed<=0){
  needed=(24+3)/4;
 }
 AnsiString sk=otype->skill;
 if(!sk.Length()){
  mes="Error BUILD: ";
  mes+=unit->FullName();
  mes+=" can't build ";
  mes+=otype->name;
  AddEvent(mes);
  return;
 }
 int usk=unit->GetSkill(sk);
 if(usk<otype->level){
  mes="Error BUILD: ";
  mes+=unit->FullName();
  mes+=" not skilled to build ";
  mes+=otype->name;
  AddEvent(mes);
  return;
 }
 if(!needed){
  mes="Error BUILD: ";
  mes+=unit->FullName();
  mes+=": Object is finished";
  AddEvent(mes);
  return;
 }
 AnsiString it=otype->item;
 int itn;
 if(it==WoodOrStoneStr){
  itn=unit->enditems->GetNum("WOOD")+unit->enditems->GetNum("STON");
 }else{
  itn=unit->enditems->GetNum(it);
 }
 if(itn==0){
  mes="Error BUILD: ";
  mes+=unit->FullName();
  mes+=" don't have the required item "+it;
  AddEvent(mes);
  return;
 }
 int num=unit->GetMen()*usk+unit->GetBuildBonus(otype);
 if(num>needed) num=needed;
 if(itn<num){
  num=itn;
  mes="Warning BUILD: ";
  mes+=unit->FullName();
  mes+=" don't have enough materials";
  AddEvent(mes);
 }
 work=num;
 if(it==WoodOrStoneStr){
  int snum=unit->enditems->GetNum("STON");
  if(num>snum) {
   num-=snum;
   unit->enditems->SetNum("STON",0);
   unit->enditems->SetNum("WOOD",unit->enditems->GetNum("WOOD")-num);
   LogItem(10,-snum,"STON",unit);
   LogItem(10,-num,"WOOD",unit);
  }else{
   unit->enditems->SetNum("STON",snum - num);
   LogItem(10,-num,"STON",unit);
  }
 }else{
  unit->enditems->SetNum(it,itn - num);
  LogItem(10,-num,it,unit);
 }
 if(obj){
  obj->endincomplete-=num;
 }
}
AOrder * AOrderBuild::Edit()
{
 AOrder *ord;
 orders->unit->baseobj->basereg->RunOrders(true,rsBeforeBuild);
 TEditOrderBuildForm *frm=new TEditOrderBuildForm(0,this);
 frm->ShowModal();
 ord=frm->ord;
 delete frm;
 return ord;
}
bool AOrderBuild::IsMonthLong(){
 return true;
}
bool AOrderBuild::IsTradeOrder()
{
 if(commented)return false;
 AObjectType *otype;
 AUnit *unit=orders->unit;
 if(!object.Length()){
  otype=ObjectTypes->Get(unit->endobj->type);
 }else{
  otype=ObjectTypes->Get(object);
 }
 if(!otype) return false;
 return !(otype->flags&AObjectType::OT_BUILDNOTTRADEACTIVITY);
}
AOrderAutoMoving::AOrderAutoMoving()
{
 type=O_AUTOMOVE;
 issail=false;
}
AOrderAutoMoving::~AOrderAutoMoving()
{
}
void AOrderAutoMoving::Read(TFile & in)
{
 AOrder::Read(in);
 int kol,dir;
 in>>kol;
 for(int i=0;i<kol;i++){
  in>>dir;
  Add(dir);
 }
 if(BaseVer<28)
 {
   ReadIntAsShort(in,begloc.xloc);
   ReadIntAsShort(in,begloc.yloc);
   begloc.zloc=1;
 }
 else
   begloc.Read(in);
 if(commented) repeating=false;
}
void AOrderAutoMoving::Write(TFile &out)
{
 AOrder::Write(out);
 int kol=count,dir;
 out<<kol;
 for(int i=0;i<kol;i++){
  dir=Get(i);
  out<<dir;
 }
 begloc.Write(out);
}
AnsiString AOrderAutoMoving::Print()
{
 return WriteTemplate();
}
AnsiString AOrderAutoMoving::WriteTemplate()
{
 AnsiString s=keyAutoOrder;
 if(!issail)
  s+=FindOrderKey(O_MOVE);
 else
  s+=FindOrderKey(O_SAIL)+" ";
 if(repeating) s+="!";
 if(commented) s+=";";
 s+=begloc.xloc;
 s+=",";
 s+=begloc.yloc;
 s+=",";
 s+=begloc.zloc;
 for(int i=0;i<count;i++){
  s+=" ";
  int dir=Get(i);
  if(dir<MOVE_IN){
   ADirType *dtype=DirTypes->Get(dir);
   if(dtype)
    s+=dtype->abr;
  }else if(dir==MOVE_IN){
   s+=keyIn;
  }else if(dir==MOVE_OUT){
   s+=keyOut;
  }else{
   s+=dir-MOVE_OUT;
  }
 }
 return s;
}
bool AOrderAutoMoving::ParseOrder(char *s)
{
 AnsiString str;
 if(*s=='!'){
  repeating=true;
  s++;
 }
 if(*s==';'){
  commented=true;
  repeating=false;
  s++;
 }
 str=GetNextToken(s);
 begloc.xloc=atoi(str.c_str());
 if(*s!=',') return false;
 s++;
 str=GetNextToken(s);
 begloc.yloc=atoi(str.c_str());
 if(*s==',')
 {
   s++;
   str=GetNextToken(s);
   begloc.zloc=atoi(str.c_str());
 }else begloc.zloc=1;
 if(*s!=' ') return false;
 while(*s==' ') s++;
 return AOrderMoving::ParseOrder(s);
}
void AOrderAutoMoving::Run(int value)
{
 AOrderMoving::Run(value);
}
AOrder * AOrderAutoMoving::Edit()
{
/* AOrder *ord;
 TEditOrderMovingForm *frm=new TEditOrderMovingForm(0,this);
 frm->preparemode=true;
 frm->ShowModal();
 ord=frm->ord;
 return ord;*/
 AUnit *un=orders->unit;
 TEditOrderAutoMoveForm *frm=new TEditOrderAutoMoveForm(0,un,this);
 frm->ShowModal();
 delete frm;
 return 0;
}
bool AOrderAutoMoving::IsMonthLong()
{
 return false;
}
bool AOrderAutoMoving::Step(ALocation &loc,AObject *& obj,int dir)
{
  if(dir<NDIRS)
  {
    loc.xloc+=movingdx[dir];
    loc.yloc+=movingdy[dir];
    obj=0;
    ARegionArray *ar=curRegionList->GetRegionArray(loc.zloc);
    if(ar)
      ar->NormCoord(loc.xloc,loc.yloc);
    return true;
  }
  if(dir==MOVE_IN)
  {
    if(!obj)
      return false;
    if(!obj->inner)
      return false;
    if(obj->innerloc.xloc==-1)
      return false;
    loc=obj->innerloc;  
    return true;
  }
  if(dir==MOVE_OUT)
  {
    obj=0;
    return true;
  }
  if(dir>MOVE_OUT)
  {
    int num=dir-MOVE_OUT;
    ARegion *reg=curRegionList->Get(loc);
    if(reg)
      obj=reg->GetObject(num);
    return true;
  }
  return false;
}
ALocation AOrderAutoMoving::GetLoc(int movepoints){
 ALocation loc;
 if(begloc.xloc<0) return loc;
 loc=begloc;
 AObject *obj=0;
 if(movepoints>count) movepoints=count;
 for(int i=0;i<movepoints;i++){
  int dir=Get(i);
  Step(loc,obj,dir);
 }
 return loc;
}
int AOrderAutoMoving::GetIndex(ALocation targloc){
 if(begloc.xloc<0) return -1;
 ALocation loc=begloc;
 AObject *obj=0;
 for(int i=0,endi=count;i<endi;i++){
  if(loc.xloc==targloc.xloc&&loc.yloc==targloc.yloc)
   return i;
  int dir=Get(i);
  Step(loc,obj,dir);
 }
 if(loc.xloc==targloc.xloc&&loc.yloc==targloc.yloc)
   return count;
 return -1;
}
AOrderMoving *AOrderAutoMoving::CreateMoveOrder(int index){
 AOrderMoving *ord=new AOrderMoving;
 ord->type=issail?O_SAIL:O_MOVE;
 for(int i=index,endi=count;i<endi;i++){
  int dir=Get(i);
  ord->Add(dir);
 }
 return ord;
}
AOrderCast::AOrderCast()
{
 type=O_CAST;
}
AOrderCast::~AOrderCast()
{
}
void AOrderCast::Read(TFile & in)
{
 AOrder::Read(in);
 if(BaseVer>=32)
   in>>abr;
 else
   ReadStringAsInt(in,abr);
 in>>params;
}
void AOrderCast::Write(TFile & out)
{
 AOrder::Write(out);
 out<<abr;
 out<<params;
}
AnsiString AOrderCast::Print()
{
 AnsiString s=Begin()+FindOrderKey(type);
 s+=SkillTypes->AbrToName(abr);
 if(params.Length()){
  s+=" "+params;
 }
 if(!commented&&result.Length()){
  s+=" ;"+result;
 }
 return s;
}
AnsiString AOrderCast::WriteTemplate()
{
 AnsiString s=Begin()+FindOrderKey(type);
 s+=abr;
 if(params.Length()){
  s+=" "+params;
 }
 return s;
}
bool AOrderCast::ParseOrder(char * s)
{
 AnsiString str=GetNextToken(s);
 if(!str.Length()){
  return false;
 }
 ASkillType *stype=SkillTypes->Get(str);
 if(!stype)return false;
 if(!(stype->flags&ASkillType::CAST))return false;
 abr=stype->abr;
 while(*s==' ')s++;
 params=GetNextString(s);
 int pos=params.Pos(";");
 if(pos){
  params.SetLength(pos-1);
 }
 return true;
}
void AOrderCast::Run(int value)
{
 AnsiString mes;
 AUnit *un=orders->unit;
 if(!abr.Length()){
  if(!isedited){
   mes="Error ";
   mes+=un->FullName();
   mes+=" has empty CAST order";
   AddEvent(mes);
  }
  return;
 }
 int level=un->GetSkill(abr);
 result.SetLength(0);
 if(!level)
 {
  mes="Error: ";
  mes+=un->FullName();
  mes+=" don't know ";
  mes+=SkillTypes->AbrToName(abr);
  AddEvent(mes);
  return;
 }
 AnsiString res;
 ASpellType *stype=SpellTypes->Get(abr);
 if(!stype){
  mes="Error ";
  mes+=un->FullName();
  mes+=" has CAST order with no spell info";
  AddEvent(mes);
  return;
 }
 int num1=stype->GetCastCreateCount();
 if(!num1){
  res="level ";
  res+=level;
  result=res;
  return;
 }
 ExprSetRegionMode(un->endobj->endreg);
 EDataInt *e=new EDataInt;
 e->value=level;
 variables[7].data=e;

 bool used=false;
 for(int i=0;i<num1;i++){
  TStringList *params=new TStringList;
  AnsiString maxnum,param;
  stype->GetCastCreateParam(i,params,maxnum,param);
  if(param.Length()){
   if(MyCompare(this->params.c_str(),param)){
    delete params;
    continue;
   }
  }
  used=true;
  int count=params->Count/2;
  AnsiString targabr=params->Strings[0];
  int value=0;
  try{
   value=ExprProcessExpr(params->Strings[1]);
  }catch (Exception &exception){
   Application->ShowException(&exception);
  }
  if(value){
   if(maxnum.Length()){
    try{
     int mn=ExprProcessExpr(maxnum);
     int cur=un->enditems->GetNum(targabr);
     if(value+cur>mn){
      value=mn-cur;
     }
    }catch (Exception &exception){
     Application->ShowException(&exception);
    }
   }
   if(value<0) value=0;
   if(count>1){
    for(int j=1;j<count&&value;j++){
     AnsiString abr=params->Strings[j*2];
     int mul=1;
     try{
      mul=ExprProcessExpr(params->Strings[j*2+1]);
     }catch (Exception &exception){
      Application->ShowException(&exception);
     }
     params->Objects[j*2]=(TObject*)mul;
     if(mul<0){
      if(mul>un->enditems->GetNum(abr))
       value=0;
     }else{
      mul=un->enditems->GetNum(abr)/mul;
      if(value>mul)
       value=mul;
     }
    }
   }
   if(targabr.Length()){
    res+=" +";
    res+=value;
    res+=" "+ItemTypes->AbrToNames(targabr);
    if(!value)
    {
      mes="Warning: ";
      mes+=un->FullName();
      mes+=" can't create ";
      mes+=ItemTypes->AbrToNames(targabr);
      AddEvent(mes);
    }
   }
   if(value){
    if(targabr.Length()){
     un->enditems->SetNum(targabr,un->enditems->GetNum(targabr)+value);
     LogItem(9,value,targabr,un);
    }
    if(count>1){
     for(int j=1;j<count;j++){
      AnsiString abr=params->Strings[j*2];
      int mul=(int)params->Objects[j*2];
      mul*=value;
      res+=" -";
      res+=mul;
      res+=" "+ItemTypes->AbrToNames(abr);
      un->enditems->SetNum(abr,un->enditems->GetNum(abr)-mul);
      LogItem(9,-mul,abr,un);
     }
    }
   }
  }
  delete params;
 }
 if(!used)
 {
    mes="Warning: ";
    mes+=un->FullName();
    mes+=" has CAST order with empty or unknown parameter";
    AddEvent(mes);
 }
 result=res;
}
AOrder * AOrderCast::Edit()
{
 TStringList *names=new TStringList, *abrs=new TStringList;
 AUnit *unit=orders->unit;
 foreach(unit->skills){
  ASkill *skill=*(ASkill**)iter;
  ASkillType *stype=skill->type;
  if(!stype)throw Exception("Bad skill type");
  if(!(stype->flags&ASkillType::CAST))continue;
  abrs->Add(stype->abr);
  names->Add(skill->Print(1+4));
//  names->Add(stype->name);
 }
 TEditOrderCastForm *frm=new TEditOrderCastForm(0,this,names,abrs);
 frm->ShowModal();
 AOrder *ord=frm->ord;
 delete frm;
 delete names;
 delete abrs;
 return ord;
}
int AOrderCast::Productivity()
{
 return 0;
}
AOrderWithdraw::AOrderWithdraw(bool cl)
 :clientonly(cl),count(0)
{
 type=O_WITHDRAW;
}
AOrderWithdraw::~AOrderWithdraw()
{
}
void AOrderWithdraw::Read(TFile & in)
{
 AOrder::Read(in);
 in>>clientonly;
 in>>count;
 if(BaseVer>=32)
   in>>item;
 else
   ReadStringAsInt(in,item);
}
void AOrderWithdraw::Write(TFile & out)
{
 AOrder::Write(out);
 out<<clientonly;
 out<<count;
 out<<item;
}
AnsiString AOrderWithdraw::Print()
{
 AnsiString s=FindOrderKey(type);
 if(item.Length()){
  s+=count;
  s+=" ";
  AnsiString ss=ItemTypes->AbrToName(item);
  if(ss.Length()) s+=ss;
  else{
   s+="???";
  }
 }
 if(clientonly)
   s=keyAutoOrder+s+" (clientonly)";
 else
 {
   s=Begin()+s;
   if(!commented)
   {
     AItemType *itype=ItemTypes->Get(item,4);
     if(itype)
     {
       int cost=itype->withdrawcost*count;
       s+=" ;-$";
       s+=cost;
     }
   }
 }
 return s;
}
AnsiString AOrderWithdraw::WriteTemplate()
{
 AnsiString s=clientonly?keyAutoOrder:Begin();
 s+=FindOrderKey(type);
 if(item.Length()){
  s+=count;
  s+=" ";
  s+=item;
 }else{
  s=orders->unit->FullName()+" WITHDRAW: Empty order";
  AddEvent(s);
  throw Exception(s);
 }
 return s;
}
bool AOrderWithdraw::ParseOrder(char * s)
{
 AnsiString str=GetNextToken(s);
 if(!str.Length())return false;
 if((str[1]<'0'||str[1]>'9')&&str[1]!='-') return false;
 count=atoi(str.c_str());
 while(*s==' ')s++;
 AnsiString itemname=GetNextToken(s);
 AItemType *itype=ItemTypes->Get(itemname);
 if(!itype)return false;
 item=itype->abr;
 return true;
}
void AOrderWithdraw::Run(int value)
{
 if(value!=clientonly) return;
 AUnit *un=orders->unit;
 AnsiString mes;
 if(!item.Length()){
  mes+="Error : ";
  mes+=un->FullName();
  if(clientonly)
    mes+=" client";
  mes+=" WITHDRAW: empty order";
  AddEvent(mes);
  return;
 }
 int num=un->enditems->GetNum(item)+count;
 if(num<0) num=0;
 un->enditems->SetNum(item,num);
 if(!clientonly)
 {
   LogItem(12,count,item,un);
   int facnum=0;
   if(un->faction)
   {
     if(!un->faction->local) return;
     facnum=un->faction->number;
   }
   AItemType *itype=ItemTypes->Get(item,4);
   if(itype)
   {
     int cost=itype->withdrawcost*count;
     un->baseobj->basereg->claims[facnum]+=cost;
   }
   return;
 }
 if(!un->IsLocal())
   return;
 mes="Warning: ";
 mes+=un->FullName();
 mes+=" has client WITHDRAW order";
 AddEvent(mes);
}
AOrder * AOrderWithdraw::Edit()
{
 TEditOrderWithdrawForm *frm=new TEditOrderWithdrawForm(0,this);
 frm->ShowModal();
 AOrder *ord=frm->ord;
 delete frm;
 return ord;
// return 0;
}
AOrderAutoWarning::AOrderAutoWarning()
{
  type=O_AUTOWARNING;
}
AOrderAutoWarning::~AOrderAutoWarning()
{
}
void AOrderAutoWarning::Read(TFile & in)
{
//  AOrder::Read(in);
  in>>mess;
  in>>expr;
}
void AOrderAutoWarning::Write(TFile & out)
{
  WriteIntAsByte(out,type);
//  AOrder::Write(out);
  out<<mess;
  out<<expr;
}
AnsiString AOrderAutoWarning::Print()
{
  AnsiString s=WriteTemplate();
  s+=" (autoorder)";
  return s;
}
AnsiString AOrderAutoWarning::WriteTemplate()
{
  AnsiString s=keyAutoOrder+FindOrderKey(O_AUTOWARNING);
  if(mess.Length())
  {
    s+="\""+mess+"\" ";
  }
  s+=expr;
  return s;
}
bool AOrderAutoWarning::ParseOrder(char * s)
{
  mess.SetLength(0);
  if(*s=='\"')
  {
    mess=GetNextToken(s);
    while(*s==' ') s++;
  }
  expr=s;
  return true;
}
void AOrderAutoWarning::Run(int value)
{
  AUnit *un=orders->unit;
  ExprSetUnitMode(un);
  bool res;
  try{
    res=ExprProcessExpr(expr);
  }catch (Exception &exception){
    AnsiString str=un->FullName()+": ";
    exception.Message=str+exception.Message;
    Application->ShowException(&exception);
    return;
  }
  if(!res) return;
  AnsiString str="AutoWarning for "+un->FullName()+": ";
  if(mess.Length())
    str+=mess;
  else
    str+="\""+expr+"\" is true";
  AddEvent(str);  
}
AOrder * AOrderAutoWarning::Edit()
{
 AOrder *ord;
 TEditOrderAutoWarningForm *frm=new TEditOrderAutoWarningForm(0,this);
 frm->ShowModal();
 ord=frm->ord;
 delete frm;
 return ord;
}

AOrderNameDescribe::AOrderNameDescribe()
  :subtype(-1)
{
}
AOrderNameDescribe::~AOrderNameDescribe()
{
}
void AOrderNameDescribe::Read(TFile & in)
{
 AOrder::Read(in);
 ReadIntAsByte(in,subtype);
 in>>value;
}
void AOrderNameDescribe::Write(TFile & out)
{
 AOrder::Write(out);
 WriteIntAsByte(out,subtype);
 out<<value;
}
AnsiString AOrderNameDescribe::Print()
{
  return WriteTemplate();
}
AnsiString AOrderNameDescribe::WriteTemplate()
{
 AnsiString str=Begin()+FindOrderKey(type);
 switch((type==O_NAME?0:10)+subtype)
 {
   case 0:
     str+=keyUnit;
   break;
   case 1:
     str+=keyFaction;
   break;
   case 2:
     str+=keyObject;
   break;
   case 3:
     str+=keyCity;
   break;
   case 10:
     str+=keyUnit;
   break;
   case 11:
     str+=keyShip;
   break;
   case 12:
     str+=keyBuilding;
   break;
   case 13:
     str+=keyObject;
   break;
 }
 if(value.Length())
 {
   str+=" \"";
   str+=value;
   str+="\"";
 }
 return str;
}
bool AOrderNameDescribe::ParseOrder(char * s)
{
 if(type==O_NAME)
 {
   AnsiString str=GetNextToken(s);
   if(!str.AnsiCompareIC(keyUnit))
     subtype=0;
   else if(!str.AnsiCompareIC(keyFaction))
     subtype=1;
   else if(!str.AnsiCompareIC(keyObject))
     subtype=2;
   else if(!str.AnsiCompareIC(keyCity))
     subtype=3;
   else return false;
   while(*s==' ') s++;
   if(*s)
     value=GetNextToken(s);
   return true;
 }
 if(type==O_DESCRIBE)
 {
   AnsiString str=GetNextToken(s);
   if(!str.AnsiCompareIC(keyUnit))
     subtype=0;
   else if(!str.AnsiCompareIC(keyShip))
     subtype=1;
   else if(!str.AnsiCompareIC(keyBuilding))
     subtype=2;
   else if(!str.AnsiCompareIC(keyObject))
     subtype=3;
   else return false;
   while(*s==' ') s++;
   if(*s)
     value=GetNextToken(s);
   return true;
 }
 return false;
}
AOrder * AOrderNameDescribe::Edit()
{
 TEditOrderNameDescribeForm *frm=new TEditOrderNameDescribeForm(0,this);
 frm->ShowModal();
 AOrder *ord=frm->ord;
 delete frm;
 return ord;
}
AOrderSteal::AOrderSteal()
 :target(0),amount(0),tarnum(0),message(0),item("SILV")
{
  type=O_STEAL;
}
AOrderSteal::~AOrderSteal()
{
  DeleteMessage(message);
}
void AOrderSteal::DeleteMessage(AOrderMessage *mes){
  if(mes!=message)return;
  if(!mes)return;
  mes->base=0;
  mes->orders->Delete(mes);
  message=0;
  target=0;
}
void AOrderSteal::SetMessage(){
 if(!message){
  if(!target)return;
  message=new AOrderMessage(this);
  message->target=orders->unit;
  message->subtype=OM_STEALED;
  target->orders->Add(message);
 }
 AnsiString s;
 s+=amount;
 s+=" ";
 AnsiString str=ItemTypes->AbrToName(item);
 if(str.Length()) s+=str;
 else s+="???";
 s+=" is stolen by ";
 s+=orders->unit->FullName();
 message->text=s;
}
void AOrderSteal::Prepare(){
 amount=0;
 AnsiString mes;
 if(!target){
  if(tarnum) target=orders->unit->baseobj->basereg->GetUnit(tarnum);
  if(commented)return;
  if(!target){
   if(tarnum){
	mes="Error ";
	mes+=orders->unit->FullName();
	mes+=" STEAL: unit ";
	mes+=GetAlias(tarnum,orders->unit->faction);
	mes+=" is not found";
	AddEvent(mes);
   }
   return;
  }
 }
 if(message){
  if(commented){ //???
   DeleteMessage(message);
  }
  return;
 }
 if(item.Length())
 {
   if(item=="SILV")
	 amount=200;
   else
   {
	 AItemType *itype=ItemTypes->Get(item,4);
	 if(itype)
	 {
	   amount=1;
	   if(itype->weight&&!target->items->GetNum(item))
		 amount=0;
	 }
   }
 }
}
void AOrderSteal::Read(TFile & in)
{
 AOrder::Read(in);
 in>>tarnum;
 if(BaseVer>=32)
   in>>item;
 else
   ReadStringAsInt(in,item);
}
void AOrderSteal::Write(TFile & out)
{
 AOrder::Write(out);
 out<<tarnum;
 out<<item;
}
AnsiString AOrderSteal::Print()
{
 AnsiString s=Begin()+FindOrderKey(type);
 AUnit *un=target;
 if(un)s+=un->FullName()+" ";
 else{
  un=orders->unit->baseobj->basereg->GetUnit(tarnum);
  if(un){
   s+=un->FullName()+" ";
  }else{
   s+="?"+GetAlias(tarnum,orders->unit->faction)+" ";
  }
 }
 AnsiString ss=ItemTypes->AbrToName(item);
 if(ss.Length()) s+=ss;
 else s+="???";
 if(!commented)
 {
   s+=" ;";
   s+=amount;
 }
 return s;
}
AnsiString AOrderSteal::WriteTemplate()
{
 AnsiString s=Begin()+FindOrderKey(type);
 if(!target){
  if(tarnum) target=orders->unit->baseobj->basereg->GetUnit(tarnum);
 }
 if(!target){
  if(!commented){
   AnsiString ss=orders->unit->FullName()+" STEAL: Unknown target ";
   AnsiString alias=GetAlias(tarnum,orders->unit->faction);
   s+=alias+" ";
   ss+=alias;
   AddEvent(ss);
  }else{
   s+=GetAlias(tarnum,orders->unit->faction)+" ";
  }
 }else s+=target->Alias()+" ";

 AItemType *itype=ItemTypes->Get(item,4);
 if(itype)s+=item;
 else{
  s=orders->unit->FullName()+" STEAL: unknown item "+item;
  AddEvent(s);
  throw Exception(s);
 }
 return s;
}
bool AOrderSteal::ParseOrder(char * s)
{
 tarnum=ParseAlias(s);
// if(!tarnum)return false;
 while(*s==' ')s++;
 AnsiString itemname=GetNextToken(s);
 AItemType *itype=ItemTypes->Get(itemname);
 if(!itype)return false;
 item=itype->abr;
 amount=0;
 return true;
}
void AOrderSteal::Run(int value)
{
 if(commented)return;
 AUnit *un=orders->unit;
 AnsiString mes;
 int mens=un->GetBegMen();
 if(mens!=1)
 {
   mes="Error: ";
   mes+=un->FullName();
   mes+=" STEAL: Must be executed by a 1-man unit.";
   AddEvent(mes);
   amount=0;
   return;
 }
 AItemType *itype=ItemTypes->Get(item,4);
 if(!itype)
  throw Exception("Steal: bad type");
 if(itype->type&(AItemType::IT_MAN|AItemType::IT_MONSTER)){
  mes="Error: ";
  mes+=un->FullName();
  mes+=" STEAL: Can't steal ";
  mes+=itype->names;
  AddEvent(mes);
  amount=0;
  return;
 }
 if(!target)return;

  if( (un->endreveal != REVEAL_NONE) || (un->GetBegStealth() == 0) )
  {
	mes = "Error: ";
	mes += un->FullName();
	mes += " STEAL: is not hidden.";
	AddEvent(mes);
  } else
  if(un->GetBegStealth() < 5)
  {
	mes="Warning: ";
	mes+=un->FullName();
	mes+=" STEAL: Stealth level is low.";
	AddEvent(mes);
  }

 if(amount)
 {
   un->enditems->SetNum(itype,un->enditems->GetNum(itype)+amount);
   int last=target->enditems->GetNum(itype)-amount;
   if(last>=0)
     target->enditems->SetNum(itype,last);
   if(last==0)
     target->enditems->DeleteEmpty();
 }else
 {
   mes="Warning: ";
   mes+=un->FullName();
   mes+=" STEAL: target unit has no ";
   mes+=itype->names;
   AddEvent(mes);
 }
 SetMessage();
}
AOrder * AOrderSteal::Edit()
{
 AUnit *unit=orders->unit;
 if(!unit)return 0;
 ARegion *reg=unit->baseobj->basereg;
 if(!reg)return 0;
 isedited=true;
 reg->RunOrders(true);
 isedited=false;

 AItems *items=new AItems;
 foreach(ItemTypes)
 {
   AItemType *itype=*(AItemType**)iter;
   if(itype->type&(AItemType::IT_MAN|AItemType::IT_MONSTER))
     continue;
   items->SetNum(itype->abr,1);
 }
 items->Sort();
 AUnits *units=new AUnits(orders->unit->baseobj);
 units->autodelete=false;

 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(un==unit)continue;
   if(un->IsLocal()) continue;
   units->Add(un);
  }
 }
 TEditOrderStealForm *frm=new TEditOrderStealForm(0,this,items,units);
 frm->ShowModal();
 AOrder *ord=frm->ord;
 delete frm;
 delete units;
 delete items;
 return ord;
}
AOrderAssassinate::AOrderAssassinate()
 :target(0),tarnum(0),message(0)
{
 type=O_ASSASSINATE;
}
AOrderAssassinate::~AOrderAssassinate()
{
 DeleteMessage(message);
}
void AOrderAssassinate::DeleteMessage(AOrderMessage *mes){
 if(mes!=message)return;
 if(!mes)return;
 mes->base=0;
 mes->orders->Delete(mes);
 message=0;
 target=0;
}
void AOrderAssassinate::SetMessage(){
 if(!message){
  if(!target)return;
  message=new AOrderMessage(this);
  message->target=orders->unit;
  message->subtype=OM_ASSASSINATED;
  target->orders->Add(message);
 }
 AnsiString s;
 s+="Is assassinated by ";
 s+=orders->unit->FullName();
 message->text=s;
}
void AOrderAssassinate::Prepare(){
 AnsiString mes;
 if(!target){
  if(tarnum) target=orders->unit->baseobj->basereg->GetUnit(tarnum);
  if(commented)return;
  if(!target){
   if(tarnum){
    mes="Error ";
    mes+=orders->unit->FullName();
    mes+=" ASSASSINATE: unit ";
    mes+=GetAlias(tarnum,orders->unit->faction);
    mes+=" is not found";
    AddEvent(mes);
   }
   return;
  }
 }
 if(message){
  if(commented){ //???
   DeleteMessage(message);
  }
  return;
 }
}
void AOrderAssassinate::Read(TFile & in)
{
 AOrder::Read(in);
 in>>tarnum;
}
void AOrderAssassinate::Write(TFile & out)
{
 AOrder::Write(out);
 out<<tarnum;
}
AnsiString AOrderAssassinate::Print()
{
 AnsiString s=Begin()+FindOrderKey(type);
 AUnit *un=target;
 if(un)s+=un->FullName();
 else{
  un=orders->unit->baseobj->basereg->GetUnit(tarnum);
  if(un){
   s+=un->FullName();
  }else{
   s+="?"+GetAlias(tarnum,orders->unit->faction);
  }
 }
 return s;
}
AnsiString AOrderAssassinate::WriteTemplate()
{
 AnsiString s=Begin()+FindOrderKey(type);
 if(!target){
  if(tarnum) target=orders->unit->baseobj->basereg->GetUnit(tarnum);
 }
 if(!target){
  if(!commented){
   AnsiString ss=orders->unit->FullName()+" ASSASSINATE: Unknown target ";
   AnsiString alias=GetAlias(tarnum,orders->unit->faction);
   s+=alias;
   ss+=alias;
   AddEvent(ss);
  }else{
   s+=GetAlias(tarnum,orders->unit->faction);
  }
 }else s+=target->Alias();
 return s;
}
bool AOrderAssassinate::ParseOrder(char * s)
{
 tarnum=ParseAlias(s);
 return true;
}
void AOrderAssassinate::Run(int value)
{
 if(commented)return;
 AUnit *un=orders->unit;
 AnsiString mes;
 int mens=un->GetBegMen();
 if(mens!=1)
 {
   mes="Error: ";
   mes+=un->FullName();
   mes+=" ASSASSINATE: Must be executed by a 1-man unit.";
   AddEvent(mes);
   return;
 }
 if(!target)return;

  if( (un->endreveal != REVEAL_NONE) || (un->GetBegStealth() == 0) )
  {
	mes="Error: ";
	mes+=un->FullName();
	mes+=" ASSASSINATE: is not hidden.";
	AddEvent(mes);
  } else
  if(un->GetBegStealth() < 5)
  {
	mes="Warning: ";
	mes+=un->FullName();
	mes+=" ASSASSINATE: Stealth level is low.";
	AddEvent(mes);
  }

  AItems *items=target->enditems;
  foreach(items)
  {
   AItem *it=*(AItem**)iter;
   if(!(it->type->type&AItemType::IT_MAN))
     continue;
   it->count--;
   if(!it->count)
     items->DeleteEmpty();
   break;
 }
 SetMessage();
}
AOrder * AOrderAssassinate::Edit()
{
 AUnit *unit=orders->unit;
 if(!unit)return 0;
 ARegion *reg=unit->baseobj->basereg;
 if(!reg)return 0;
 isedited=true;
 reg->RunOrders(true);
 isedited=false;

 AUnits *units=new AUnits(orders->unit->baseobj);
 units->autodelete=false;

 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(un==unit)continue;
   if(un->IsLocal()) continue;
   units->Add(un);
  }
 }
 TEditOrderAssassinateForm *frm=new TEditOrderAssassinateForm(0,this,units);
 frm->ShowModal();
 AOrder *ord=frm->ord;
 delete frm;
 delete units;
 return ord;
}
AOrderAttack::AOrderAttack()
 :AOrderTeach()
{
 type=O_ATTACK;
}
AOrderAttack::~AOrderAttack()
{
}
void AOrderAttack::Run(int value)
{
 AnsiString mes;
 AUnit *unit=orders->unit;
 if(unit->num<0)
 {
   AddEvent(unit->FullName()+" ATTACK: new units can't attack");
   return;
 }
 int kol=0;
 for(int i=0;i<targets->Count;i++){
  AUnitPtr *ptr=GetTarget(i);
  AUnit *target=ptr->ptr;
  if(!target){
   targets->Delete(i);i--;
   delete ptr;
   continue;
  }
  kol++;
  AOrderMessage *message=new AOrderMessage(this);
  message->target=orders->unit;
  message->subtype=OM_ATTACKED;
  message->text="Is attacked by "+unit->FullName();;
  ptr->mes=message;
  target->orders->Add(message);
 }
 if(!kol){
  AddEvent(orders->unit->FullName()+" ATTACK: No targets");
 }
}
AOrder * AOrderAttack::Edit()
{
 AUnit *unit=orders->unit;
 if(!unit)return 0;
 ARegion *reg=unit->baseobj->basereg;
 if(!reg)return 0;
 VerTargets();

 AUnits *units=new AUnits(orders->unit->baseobj);
 units->autodelete=false;

 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(un==unit)continue;
   if(un->IsLocal()) continue;
   units->Add(un);
  }
 }
 TEditOrderAttackForm *frm=new TEditOrderAttackForm(0,this,units);
 frm->ShowModal();
 AOrder *ord=frm->ord;
 delete frm;
 delete units;
 for(int i=targets->Count-1;i>=0;i--){
  AUnitPtr *ptr=GetTarget(i);
  ptr->ptr=0;
 }
 return ord;
}
void AOrderAttack::DeleteMessage(AOrderMessage *mes){
 for(int i=targets->Count-1;i>=0;i--){
  AUnitPtr *ptr=GetTarget(i);
  if(ptr->mes!=mes)
    continue;
  ptr->ptr=0;
  ptr->mes=0;
  mes->base=0;
  mes->orders->Delete(mes);
 }
}
void AOrderAttack::AddTarget(int num, AUnit *target)
{
 if(!target){
  if(orders)target=orders->unit->baseobj->basereg->GetUnit(num);
 }
 AUnitPtr *ptr=new AUnitPtr;
 ptr->ptr=target;
 ptr->num=num;
 ptr->mes=0;
 targets->Add(ptr);
}
bool AOrderAttack::IsMonthLong(){
 return false;
}
void AOrderAttack::Prepare(){
 VerTargets();
}
static EvPluginOrder OrderParams;
AOrderPlugin::AOrderPlugin(int MN, int ON)
  :ModuleNumber(MN),OrderNumber(ON)
{
  type=O_PLUGIN;
  OrderType=PO_NORMAL;
}
AOrderPlugin::~AOrderPlugin()
{
  UnPlug();
}
void AOrderPlugin::UnPlug()
{
  if(ModuleNumber<0) return;
  FillOrderParams();
  if(Plugins.ProcessOrderEvent(OrderType,ModuleNumber,PE_ORDERDESTROY,&OrderParams,orders->unit)<0)
  {
    AnsiString str="Can't delete plugin order: ";
    str+=text;
    AddEvent(str);
    ShowMessage(str);
  }
  ModuleNumber=-1;
}
void AOrderPlugin::Read(TFile & in)
{
  AOrder::Read(in);
  in>>OrderType;
  in>>text;
  ModuleNumber=-1;
}
void AOrderPlugin::Write(TFile & out)
{
  AOrder::Write(out);
  out<<OrderType;
  out<<text;
}
AnsiString AOrderPlugin::Print()
{
  AnsiString s=OrderType==PO_AUTO?keyAutoOrder:Begin();
  VerifyPlugin();
  FillOrderParams();
  if(Plugins.ProcessOrderEvent(OrderType,ModuleNumber,PE_ORDERVIEWSTR,&OrderParams,orders->unit)<0
    ||OrderParams.retstr[0]==0)
    s+=text;
  else
    s+=OrderParams.retstr;
  if(ModuleNumber<0)
    s+=" ;unknown plugin";
  return s;
}
AnsiString AOrderPlugin::WriteTemplate()
{
  AnsiString s=OrderType==PO_AUTO?keyAutoOrder:Begin();
  VerifyPlugin();
  FillOrderParams();
  if(Plugins.ProcessOrderEvent(OrderType,ModuleNumber,PE_ORDERTEMPLATESTR,&OrderParams,orders->unit)>=0
    &&OrderParams.retstr[0]!=0)
    text=OrderParams.retstr;
  s+=text;
  return s;
}
bool AOrderPlugin::ParseOrder(char * s)
{
  if(ModuleNumber<0)
    return false;
  text=s;
  return VerifyPlugin(false);
}
void AOrderPlugin::Run(int value)
{
  if(!VerifyPlugin())
  {
    AnsiString str="Can't run plugin order: ";
    str+=text;
    AddEvent(str);
    return;
  }
  FillOrderParams();
  if(value)
    OrderParams.flags|=4;
  if(Plugins.ProcessOrderEvent(OrderType,ModuleNumber,PE_ORDERRUN,&OrderParams,orders->unit)<0)
  {
    AnsiString str="Can't run plugin order: ";
    str+=text;
    AddEvent(str);
  }
}
AOrder * AOrderPlugin::Edit()
{
  if(!VerifyPlugin(text.Length()))
  {
    AnsiString str="Can't edit plugin order: ";
    str+=text;
    AddEvent(str);
    return 0;
  }
  FillOrderParams();
  int ret=Plugins.ProcessOrderEvent(OrderType,ModuleNumber,PE_ORDEREDIT,&OrderParams,orders->unit);
  if(ret==1)
  {
    AnsiString oldtext=text;
    AOrder *_ord=AOrderCustom::Edit();
    if(_ord!=this)
      return _ord;
    if(oldtext==text)
      return _ord;
    int mn=ModuleNumber,on=OrderNumber;
    ModuleNumber=-1;
    if(!VerifyPlugin())
    {
      text=oldtext;
      ModuleNumber=mn;
      OrderNumber=on;
      ShowMessage("Can't parse custom plugin order");
      return 0;
    }
    return this;
  }
  if(ret<0)
  {
    AnsiString str="Can't edit plugin order: ";
    str+=text;
    AddEvent(str);
  }
  LoadParams();
  if(!text.Length())
    return 0;
  return this;
}
bool AOrderPlugin::VerifyPlugin(bool find)
{
  if((unsigned)ModuleNumber>=Plugins.size())
    find=true;
  if(find)
  {
    if((unsigned)ModuleNumber<Plugins.size())
      return true;
    if(!Plugins.FindOrderPlugin(OrderType,text.c_str(),ModuleNumber,OrderNumber))
      return false;
  }
  FillOrderParams(false);
  AUnit *un=0;
  if(orders) un=orders->unit;
  if(Plugins.ProcessOrderEvent(OrderType,ModuleNumber,PE_ORDERCREATE,&OrderParams,un)<0)
  {
    ModuleNumber=-1;
    return false;
  }
  memcpy(UUID,OrderParams.uuid,sizeof(UUID));
  LoadParams();
  return true;
}
void AOrderPlugin::FillOrderParams(bool withuuid)
{
  memset(&OrderParams,0,sizeof(EvPluginOrder));
  OrderParams.ordernum=OrderNumber;
  OrderParams.str=text.c_str();
  if(withuuid)
    memcpy(OrderParams.uuid,UUID,sizeof(UUID));
  OrderParams.flags=flags&3;
}
void AOrderPlugin::LoadParams()
{
  if(OrderParams.retstr[0])
    text=OrderParams.retstr;
  flags&=~3;
  flags|=OrderParams.flags&3;
}
/*
AOrderCustom::AOrderCustom()
{
 type=O_;
}
AOrderCustom::~AOrderCustom()
{
}
void AOrderCustom::Read(TFile & in)
{
 AOrder::Read(in);
}
void AOrderCustom::Write(TFile & out)
{
 AOrder::Write(out);
}
AnsiString AOrderCustom::Print()
{
 return Begin();
}
AnsiString AOrderCustom::WriteTemplate()
{
 return Begin();
}
bool AOrderCustom::ParseOrder(char * s)
{
 return false;
}
void AOrderCustom::Run(int value)
{
}
AOrder * AOrderCustom::Edit()
{
 return 0;
}
int AOrderCustom::Productivity()
{
 return 0;
}
*/


int AOrder::GetSize()
{
 int siz=sizeof(*this);
// siz+=Capacity*4;
 return siz;
}
int AOrders::GetSize()
{
 int siz=sizeof(*this);
 siz+=Capacity*4;
 foreach(this){
  AOrder *ord=*(AOrder**)iter;
  siz+=ord->GetSize()+0;
 }
 return siz;
}
int AOrderCustom::GetSize(){
 int siz=sizeof(*this);
 siz+=text.Length()+1;
 return siz;
}
int AOrderComment::GetSize(){
 int siz=sizeof(*this);
 siz+=text.Length()+1;
 return siz;
}
int AOrderLocDescr::GetSize(){
 int siz=sizeof(*this);
 siz+=text.Length()+1;
 return siz;
}
int AOrderSimple::GetSize(){
 int siz=sizeof(*this);
 return siz;
}
int AOrderByte::GetSize(){
 int siz=sizeof(*this);
 return siz;
}
int AOrderAbr::GetSize(){
 int siz=sizeof(*this);
 siz+=abr.Length()+1;
 return siz;
}
int AOrderGive::GetSize(){
 int siz=sizeof(*this);
 siz+=item.Length()+1;
 return siz;
}
int AOrderMessage::GetSize(){
 int siz=sizeof(*this);
 siz+=text.Length()+1;
 return siz;
}
int AOrderBuySell::GetSize(){
 int siz=sizeof(*this);
 siz+=item.Length()+1;
 return siz;
}
int AOrderTeach::GetSize(){
 int siz=sizeof(*this);
 siz+=sizeof(TList);
 siz+=targets->Capacity*4;
 siz+=targets->Count*sizeof(AUnitPtr);
 return siz;
}
int AOrderInt::GetSize(){
 int siz=sizeof(*this);
 return siz;
}
int AOrderMoving::GetSize(){
 int siz=sizeof(*this);
 siz+=sizeof(TList);
 siz+=dirs->Capacity*4;
 siz+=custom.Length()+1;
 return siz;
}
int AOrderAutoGive::GetSize(){
 int siz=sizeof(*this);
 siz+=item.Length()+1;
 siz+=begstr.Length()+1;
 siz+=substr.Length()+1;
 return siz;
}
int AOrderBuild::GetSize(){
 int siz=sizeof(*this);
 siz+=object.Length()+1;
 return siz;
}
int AOrderAutoMoving::GetSize(){
 int siz=AOrderMoving::GetSize();
 siz+=2*sizeof(int);
 return siz;
}
int AOrderCast::GetSize(){
 int siz=sizeof(*this);
 siz+=abr.Length()+1;
 siz+=params.Length()+1;
 siz+=result.Length()+1;

 return siz;
}
int AOrderWithdraw::GetSize(){
 int siz=sizeof(*this);
 return siz;
}
int AOrderAutoWarning::GetSize(){
 int siz=sizeof(*this);
 siz+=mess.Length()+1;
 siz+=expr.Length()+1;
 return siz;
}
int AOrderNameDescribe::GetSize(){
 int siz=sizeof(*this);
 siz+=value.Length()+1;
 return siz;
}
int AOrderSteal::GetSize(){
 int siz=sizeof(*this);
 siz+=item.Length()+1;
 return siz;
}
int AOrderAssassinate::GetSize(){
 int siz=sizeof(*this);
 return siz;
}
int AOrderAttack::GetSize(){
 int siz=sizeof(*this);
 siz+=sizeof(TList);
 siz+=targets->Capacity*4;
 siz+=targets->Count*sizeof(AUnitPtr);
 return siz;
}
int AOrderPlugin::GetSize(){
 int siz=sizeof(*this);
 siz+=text.Length()+1;
 return siz;
}
//------------------------------------------------------------------
AOrderSetAtt::AOrderSetAtt() { type = O_DECLARE; repeating = true; }
AOrderSetAtt::~AOrderSetAtt() {}
void AOrderSetAtt::Read(TFile & in)
{
 AOrder::Read(in);
 in>>fac;
 in>>att;
}
void AOrderSetAtt::Write(TFile & out)
{
 AOrder::Write(out);
 out<<fac;
 out<<att;
}
AnsiString AOrderSetAtt::Print()
{
  AnsiString str = Begin() + FindOrderKey(type);
  if(fac) { str += fac; str += " "; } else str += "default ";
  if(att != 5) str += AttitudeStrs[att];
  //str = AttitudeStrs[curFaction->defattitude];
  if(fac == 0) str += "; Default attidute";
  else {
    AFaction *Fact = curFactions->GetFaction(fac);
    if(Fact){
      str += "; ";
      str += Fact->name;
      str += " (";
      str += Fact->number;
      str += ")";
    }
  }
  return str;
}

AnsiString AOrderSetAtt::WriteTemplate()
{
  AnsiString str = Begin() + FindOrderKey(type);
  str += fac;
  if(att == 5) return str;
  str += " ";
  str += AttitudeStrs[att];
  return str;
}

bool AOrderSetAtt::ParseOrder(char * s)
{
  while(*s==' ')s++;
  AnsiString str = GetNextToken(s);
  if(!str.AnsiCompareIC("default")) fac = 0;
    else try { fac = str.ToInt(); } catch (...) { return false; }
  while(*s==' ')s++;
  str = GetNextToken(s);
  att = AttitudeStrToInt(str);
  if(att < 0) return false;
  if(!curUnit)return true;
  if(curUnit->faction->local && (curUnit->faction->number != curFaction->number)){
	AUnit *SavcurUnit = curUnit;
	curFactions->SetCurFaction(curUnit->faction->number);
	curUnit = SavcurUnit;
  }
  curFaction->SetAttitude(fac, att);
  return true;
}

AOrder * AOrderSetAtt::Edit()
{
  fChAtt->cbAtt->ItemIndex = att;//curFaction->defattitude;
  fChAtt->Caption = "";
  if(fChAtt->ShowModal() != mrOk) return this;
  att = fChAtt->cbAtt->ItemIndex;
  return this;
}
int AOrderSetAtt::GetSize()
{
  //int siz = sizeof(*this);
  AnsiString str = WriteTemplate();
  return str.Length();
}
//---------------------------------------------------------------------------

// TURN order
AOrderTurn::AOrderTurn()
{
  torders = new AOrders(0);
  type = O_TURN;
  tplus = 1;
}

AOrderTurn::~AOrderTurn() { delete torders; }

void AOrderTurn::Read(TFile & in)
{
  // Как-то нужно прочитать....
  AOrder::Read(in);
  in>>tplus;
  torders->Clear();
  torders->Read(in);
  foreach(torders){
	AOrder *ord = *(AOrder**) iter;
	ord->turnorder = true;
  }
}

void AOrderTurn::Write(TFile & out)
{
  // как-то нужно записать...
  AOrder::Write(out);
  out<<tplus;
  torders->Write(out);
}

AnsiString AOrderTurn::Print()
{
  // Печать ордера....
  if(!torders->unit && orders->unit) torders->unit = orders->unit;
  AnsiString str = Begin() + FindOrderKey(type); // TURN
  str += "; Orders for next turn";
  //str += torders->count;
  return str;
}

AnsiString AOrderTurn::WriteTemplate()
{
  // Запись ордера
  AnsiString str = Begin() + FindOrderKey(type);
  str += "\n";
  foreach(torders)
  {
    AOrder *ord = *(AOrder**) iter;
    str += ord->WriteTemplate();
    str += "\n";
  }
  str += "endturn";
  return str;
}

bool AOrderTurn::ParseOrder(char * s)
{
  // Парсер ордера
  foreach(torders){
    AOrder *ord = *(AOrder**) iter;
    ord->turnorder = true;
  }
  return true;
}

AOrder * AOrderTurn::Edit()
{
  //Редактирование ордера
  TfEditTurnOrder *frm = new TfEditTurnOrder(0, torders);
  frm->cbRep->Checked = repeating;
  if(frm->ShowModal() == mrOk)
  {
	repeating = frm->cbRep->Checked;
	torders->Clear();
	foreach(frm->Forders)
	{
	  AOrder *ord = *(AOrder**) iter;
	  AOrder *ord1 = ParseOrderAdd1(ord->WriteTemplate());
	  if(!ord1) continue;
	  ord1->turnorder = true; 
	  torders->Add(ord1);
	}
  delete frm;
  return this;
  }
  delete frm;
  return 0;
}

int AOrderTurn::GetSize()
{
  //int siz = sizeof(*this);
  AnsiString str = WriteTemplate();
  return str.Length();
}
//--------------------------
// FORM order
AOrderForm::AOrderForm(){ type = O_FORM; }
AOrderForm::~AOrderForm() { }

void AOrderForm::Read(TFile & in){ AOrder::Read(in); in>>num; }
void AOrderForm::Write(TFile & out){ AOrder::Write(out); out<<num; }
AnsiString AOrderForm::Print(){ return WriteTemplate(); }
AnsiString AOrderForm::WriteTemplate()
{
  // Запись ордера
  AnsiString str = FindOrderKey(type);
  str += num;
  return str;
}

bool AOrderForm::ParseOrder(char * s){
  while(*s==' ')s++;
  AnsiString str = GetNextToken(s);
  num = atoi(str.c_str());
  return (num > 0);
}

AOrder * AOrderForm::Edit() { return this; }
int AOrderForm::GetSize()
{
  AnsiString str = WriteTemplate();
  return str.Length();
}
//--------------------------
AOrderAutoFollow::AOrderAutoFollow()
 :target(0),tarnum(0)
{
  type=O_AUTOFOLLOW;
}

void AOrderAutoFollow::Read(TFile & in)
{
 AOrder::Read(in);
 in>>tarnum;
}
void AOrderAutoFollow::Write(TFile & out)
{
 AOrder::Write(out);
 out<<tarnum;
}
AnsiString AOrderAutoFollow::Print()
{
  AnsiString s = keyAutoOrder + FindOrderKey(O_AUTOFOLLOW);
  AUnit *un = target;
  if(un) s += un->FullName()+" ";
   else{
	 un = orders->unit->baseobj->basereg->GetUnit(tarnum);
	 if(un){
	   s += un->FullName()+" ";
	 } else {
	   s += "?" + GetAlias(tarnum,orders->unit->faction)+" ";
	 }
  }
 return s;
}
AnsiString AOrderAutoFollow::WriteTemplate()
{
  AnsiString s = keyAutoOrder + FindOrderKey(O_AUTOFOLLOW);
  if(!target){
	if(tarnum) target = orders->unit->baseobj->basereg->GetUnit(tarnum);
  }
  if(!target){
	if(!commented){
	  AnsiString ss=orders->unit->FullName()+" FOLLOW: Unknown target ";
	  AnsiString alias = GetAlias(tarnum,orders->unit->faction);
	  s += alias+" ";
	  ss += alias;
	  AddEvent(ss);
	} else {
	  s += GetAlias(tarnum,orders->unit->faction)+" ";
	}
  } else s += target->Alias()+" ";

 return s;
}
bool AOrderAutoFollow::ParseOrder(char * s)
{
 tarnum = ParseAlias(s);
 // if(!tarnum)return false;
 return true;
}
AOrder * AOrderAutoFollow::Edit()
{
  AUnit *unit = orders->unit;
  if(!unit)return 0;
  ARegion *reg = unit->baseobj->basereg;
  if(!reg)return 0;
  isedited = true;
  reg->RunOrders(true);
  isedited = false;

  AUnits *units = new AUnits(orders->unit->baseobj);
  units->autodelete = false;

  foreach(reg){
	AObject *obj=*(AObject**)iter;
	foreach(obj->units){
	  AUnit *un=*(AUnit**)iter;
	  if(un == unit)continue;
	  if(!un->IsLocal()) continue;
	  units->Add(un);
	}
  }

  TEditOrderAutoFollowForm *frm=new TEditOrderAutoFollowForm(0,this,units);
  frm->ShowModal();
  AOrder *ord = frm->ord;
  delete frm;
  delete units;
  return ord;
}
void AOrderAutoFollow::RunFollow()
{
  if(!target){
	if(tarnum) target = orders->unit->baseobj->basereg->GetUnit(tarnum);
  }
  if(!target)
  {
	  AnsiString ss=orders->unit->FullName()+" FOLLOW: Unknown target ";
	  AnsiString alias = GetAlias(tarnum,orders->unit->faction);
	  ss += alias;
	  AddEvent(ss);
	  return;
  }
  AnsiString ord_str = "";
  // 1. найти move / advance / sail ордер у target
  for(int i=0;i<target->orders->count;i++){
	AOrder *ord = target->orders->Get(i);
	if(ord->commented) continue;
	if( (ord->type!= O_MOVE) && (ord->type!= O_ADVANCE) && (ord->type!= O_SAIL) ) continue;
	AOrderMoving *ord_m = dynamic_cast<AOrderMoving*>(ord);
	ord_str = ord_m->WriteTemplate();
	break;
  }
  if(ord_str == "")
  {
	  AnsiString ss = orders->unit->FullName()+" FOLLOW: Target ";
	  ss += target->FullName();
	  ss += " not moved!";
	  AddEvent(ss);
	  return;
  }
  // 2. найти такой же у себя.
  AOrderMoving *ord_m = 0;
  int ind = 0;
  for(int i=0;i<orders->count;i++)
  {
	AOrder *ord = orders->Get(i);
	if(ord->commented) continue;
	if( (ord->type!= O_MOVE) && (ord->type!= O_ADVANCE) && (ord->type!= O_SAIL) ) continue;
	ord_m = dynamic_cast<AOrderMoving*>(ord);
	ind = i;
  }
  if(ord_m){
	 // есть такой, проверить надо
	 AnsiString s = ord_m->WriteTemplate();
	 if(s == ord_str) return;
	 orders->Delete(ind);
	 AOrder *ord = ParseOrder1(ord_str);
	 orders->unit->orders->Add(ord);
	 //ord_m = dynamic_cast<AOrderMoving*>(ord);

  }
  else
  {
	 // надо добавить
	//ord_m = new AOrderMoving;
	//ord_m = ParseOrder(ord_str);
	//orders->unit->orders->Add(ord_m);
	AOrder *ord = ParseOrder1(ord_str);
	orders->unit->orders->Add(ord);

  }
}
// Auto Collect Items

AOrderAutoCollect::AOrderAutoCollect() : item("")
{
}
void AOrderAutoCollect::Read(TFile & in)
{
  AOrder::Read(in);
  in>>item;
}

void AOrderAutoCollect::Write(TFile & out)
{
  AOrder::Write(out);
  out<<item;
}

AnsiString AOrderAutoCollect::Print()
{
  AnsiString s = keyAutoOrder + FindOrderKey(O_AUTOCOLLECT);
  if(item.Length())
  {
	AnsiString ss = ItemTypes->AbrToName(item);
	if(ss.Length()) s += ss; else s+="???";
  }
  return s;
}
AnsiString AOrderAutoCollect::WriteTemplate()
{
  AnsiString s = keyAutoOrder + FindOrderKey(O_AUTOCOLLECT);
  s += item;
  return s;
}

bool AOrderAutoCollect::ParseOrder(char * s)
{
  AnsiString itemname=GetNextToken(s);
  AItemType *itype = ItemTypes->Get(itemname);
  if(!itype)return false;
  item = itype->abr;
  return true;
}

AOrder * AOrderAutoCollect::Edit()
{

}

void AOrderAutoCollect::RunCollect()
{

}
// -------- Auto Order Buy
AOrderAutoBuy::AOrderAutoBuy(): item(""), num(0), max(0)
{  }
void AOrderAutoBuy::Read(TFile & in)
{
  AOrder::Read(in);
  in>>item;
  in>>num;
  in>>max;
}
void AOrderAutoBuy::Write(TFile & out)
{
  AOrder::Write(out);
  out<<item;
  out<<num;
  out<<max;
}
AnsiString AOrderAutoBuy::Print()
{
  AnsiString s = keyAutoOrder + FindOrderKey(O_AUTOCOLLECT);
  if(num < 0) s += keyAll; else s += num;
  s += " ";
  if(item.Length())
  {
	AnsiString ss = ItemTypes->AbrToName(item);
	if(ss.Length()) s += ss; else s+="???";
  }
  if(max) { s += " (max "; s += max; s += ")"; }
  return s;
}
AnsiString AOrderAutoBuy::WriteTemplate()
{
  AnsiString s = keyAutoOrder + FindOrderKey(O_AUTOCOLLECT);
  if(num < 0) s += keyAll; else s += num;
  s += " ";
  s += item;
  s += " ";
  s += max;
  return s;
}
bool AOrderAutoBuy::ParseOrder(char * s)
{
  AnsiString str = GetNextToken(s);
  if(!CompareIStr(str,keyAll)) num = -1; else num = atoi(str.c_str());
  //if(!amount)return false;
  while(*s == ' ') s++;
  AnsiString itemname = GetNextToken(s);
  AItemType *itype = ItemTypes->Get(itemname);
  if(!itype)return false;
  item = itype->abr;
  while(*s == ' ') s++;
  str = GetNextToken(s);
  max = atoi(str.c_str());
  return true;
}
AOrder * AOrderAutoBuy::Edit()
{

}
void AOrderAutoBuy::RunAutoBuy()
{

}
// -----------------------------------
