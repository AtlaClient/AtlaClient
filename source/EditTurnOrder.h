//---------------------------------------------------------------------------

#ifndef EditTurnOrderH
#define EditTurnOrderH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfEditTurnOrder : public TForm
{
__published:	// IDE-managed Components
        TMenuItem *miAddOrderCustom;
        TMenuItem *N5;
        TMenuItem *miAddOrderBuy;
        TMenuItem *miAddOrderSell;
        TMenuItem *miAddOrderGive;
        TMenuItem *miAddOrderCombat;
        TMenuItem *miAddOrderClaim;
        TMenuItem *miAddOrderPillage;
        TMenuItem *miAddOrderTax;
        TMenuItem *miAddOrderForget;
        TMenuItem *miAddOrderDestroy;
        TMenuItem *miAddOrderCast;
        TMenuItem *miAddOrderWithdraw;
        TMenuItem *N13;
        TMenuItem *miAddOrderStudy;
        TMenuItem *miAddOrderTeach;
        TMenuItem *miAddOrderBuild;
        TMenuItem *miAddOrderProduce;
        TMenuItem *miAddOrderWork;
        TMenuItem *miAddOrderEntertain;
        TMenuItem *miAddOrderAutotax;
        TMenuItem *miAddOrderAvoid;
        TMenuItem *miAddOrderBehind;
        TMenuItem *miAddOrderGuard;
        TMenuItem *miAddOrderHold;
        TMenuItem *miAddOrderNoaid;
        TMenuItem *miAddOrderReveal;
        TMenuItem *miAddOrderConsume;
        TMenuItem *miAddOrderNoCross;
        TMenuItem *miAddOrderNoSpoils;
        TMenuItem *miAddOrderSpoils;
        TMenuItem *miAddOrderSharing;
        TMenuItem *miAddOrderNameDescribe;
        TMenuItem *N14;
        TMenuItem *miAddOrderLeave;
        TMenuItem *miAddOrderEnter;
        TMenuItem *miAddOrderPromote;
        TMenuItem *miAddOrderMove;
        TMenuItem *miAddOrderAdvance;
        TMenuItem *miAddOrderSail;
        TMenuItem *N20;
        TMenuItem *miAddOrderSteal;
        TMenuItem *miAddOrderAssassinate;
        TMenuItem *miAddOrderAttack;
        TMainMenu *mMenu;
        TMenuItem *Addorder1;
        TPopupMenu *popMenu;
        TMenuItem *miEditOrder;
        TMenuItem *miDeleteOrder;
        TMenuItem *miAddOrder;
        TMenuItem *MenuItem1;
        TMenuItem *MenuItem2;
        TMenuItem *MenuItem3;
        TMenuItem *MenuItem4;
        TMenuItem *MenuItem5;
        TMenuItem *MenuItem6;
        TMenuItem *MenuItem7;
        TMenuItem *MenuItem8;
        TMenuItem *MenuItem9;
        TMenuItem *MenuItem10;
        TMenuItem *MenuItem11;
        TMenuItem *MenuItem12;
        TMenuItem *MenuItem13;
        TMenuItem *MenuItem14;
        TMenuItem *MenuItem15;
        TMenuItem *MenuItem16;
        TMenuItem *MenuItem17;
        TMenuItem *MenuItem18;
        TMenuItem *MenuItem19;
        TMenuItem *MenuItem20;
        TMenuItem *MenuItem21;
        TMenuItem *MenuItem22;
        TMenuItem *MenuItem23;
        TMenuItem *MenuItem24;
        TMenuItem *MenuItem25;
        TMenuItem *MenuItem26;
        TMenuItem *MenuItem27;
        TMenuItem *MenuItem28;
        TMenuItem *MenuItem29;
        TMenuItem *MenuItem30;
        TMenuItem *MenuItem31;
        TMenuItem *MenuItem32;
        TMenuItem *MenuItem33;
        TMenuItem *MenuItem34;
        TMenuItem *MenuItem35;
        TMenuItem *MenuItem36;
        TMenuItem *MenuItem37;
        TMenuItem *MenuItem38;
        TMenuItem *MenuItem39;
        TMenuItem *MenuItem40;
        TMenuItem *MenuItem41;
        TMenuItem *MenuItem42;
        TMenuItem *MenuItem43;
        TMenuItem *MenuItem44;
        TMenuItem *MenuItem45;
        TMenuItem *MenuItem46;
        TMenuItem *N8;
        TMenuItem *miCopyOrder;
        TMenuItem *miPasteOrder;
        TMenuItem *N7;
        TMenuItem *miAddOrderFormNew;
        TMenuItem *miCopyNewUnit;
        TMenuItem *miDeleteNewUnit;
        TPanel *Panel1;
        TButton *Button1;
        TButton *Button2;
        TMenuItem *EditOrders1;
	TPanel *Panel2;
	TMemo *Memo1;
	TSpeedButton *btRestore;
	TCheckBox *cbRep;
        void __fastcall EditOrders1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
	void __fastcall btRestoreClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
   AOrders *FSaveOrders;
   void UpdateOrders();
public:		// User declarations
   AOrders *Forders;
		__fastcall TfEditTurnOrder(TComponent* Owner, AOrders *orders);
};
//---------------------------------------------------------------------------
extern PACKAGE TfEditTurnOrder *fEditTurnOrder;
//---------------------------------------------------------------------------
#endif
