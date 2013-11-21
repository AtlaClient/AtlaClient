//---------------------------------------------------------------------------
#ifndef ordersH
#define ordersH
//---------------------------------------------------------------------------
#include "region.h"
enum OrderTypes{
  O_NONE,
  O_END,
  O_UNIT,
  O_CUSTOMORDER,
  O_LOCALDESCR,
  O_COMMENT,
  O_ADDRESS,
  O_ADVANCE,
  O_ASSASSINATE,
  O_ATTACK,
  O_AUTOTAX,
  O_AVOID,
  O_BEHIND,
  O_BUILD,
  O_BUY,
  O_CAST,
  O_CLAIM,
  O_COMBAT,
  O_CONSUME,
  O_DECLARE,
  O_DESCRIBE,
  O_DESTROY,
  O_ENDFORM,
  O_ENTER,
  O_ENTERTAIN,
  O_FACTION,
  O_FIND,
  O_FORGET,
  O_FORM,
  O_FORMEND,
  O_GIVE,
  O_MESSAGE,
  O_GUARD,
  O_HOLD,
  O_LEAVE,
  O_MOVE,
  O_NAME,
  O_NOAID,
  O_OPTION,
  O_PASSWORD,
  O_PILLAGE,
  O_PRODUCE,
  O_PROMOTE,
  O_QUIT,
  O_RESTART,
  O_REVEAL,
  O_SAIL,
  O_SELL,
  O_SHOW,
  O_STEAL,
  O_STUDY,
  O_TAX,
  O_TEACH,
  O_TEACHED,
  O_WORK,
  O_AUTOGIVE,
  O_NOP,
  O_AUTOMOVE,
  O_WITHDRAW,
  O_NOCROSS,
  O_NOSPOILS,
  O_SPOILS,
  O_AUTOWARNING,
  O_TURN,
  O_ENDTURN,
  O_PLUGIN,
  NORDERS,
  O_SHARING,
  O_AUTOFOLLOW,
  O_AUTOCOLLECT,
  O_AUTOBUY,
};
class AOrderMessage;
class AOrder{
public:
    OrderTypes type;
    AOrders *orders;
    bool turnorder;
	bool isedited; //not saved
//    int index;
    union{
     struct{
      bool commented:1;
      bool repeating:1;
      bool issail:1; //for automove;
      unsigned reserved:5;
     };
     unsigned char flags;
    };
    AOrder();
    virtual ~AOrder();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
	virtual void Run(int value);
    virtual AOrder * Edit();
    AnsiString Begin();
    virtual bool IsMonthLong();
    virtual bool IsTradeOrder();
    virtual int Productivity();
    virtual void DeleteMessage(AOrderMessage *mes);
    virtual void Prepare();
    virtual int GetSize();
};
AOrder *ParseOrder(AnsiString str);
AOrder *ParseOrderAdd1(AnsiString str);
class AFaction;
const int FACTIONMULT=10000;
AnsiString GetAlias(int num,AFaction* faction);

class AOrders:TList{
public:
    bool autodelete;

    AUnit *unit;
    AOrders(AUnit *un);
    __fastcall ~AOrders();
    void __fastcall Clear();
    void Read(TFile & in);
    void Write(TFile & out);
    AOrder* Get(int index);
    void Add(AOrder* ord);
    void Insert(int index,AOrder* ord);
    void Update(AOrders * orders);
    bool Replace(AOrder * from, AOrder * to);
//mode = 1 -comm=true, 2 - comm =false , 4 rep=true, 8 rep= false
    AOrder * Find(OrderTypes type, int mode = 15);
    AOrder * GetMonthLong();
    int IndexOf(AOrder *ord){
	return TList::IndexOf(ord);
	}
	void Delete(int index);
    void Delete(AOrder * ord);
    bool Distribute(int amount, int attempted); //exit when producivity<=0
    void Compact();
    int GetSize();
    bool Swap(int i, int j);
    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}

};
class AOrderCustom:public AOrder{
public:
    AnsiString text;

    AOrderCustom();
    virtual ~AOrderCustom();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual AOrder * Edit();
    virtual bool IsMonthLong();
    virtual int GetSize();
};
class AOrderComment:public AOrder{
public:
    AnsiString text;

    AOrderComment();
    virtual ~AOrderComment();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual AOrder * Edit();
    virtual int GetSize();
};
class AOrderLocDescr:public AOrder{
public:
    AnsiString text;

    AOrderLocDescr();
    virtual ~AOrderLocDescr();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual AOrder * Edit();
    virtual int GetSize();
};
//destroy-, entertain, leave-, pillage-, Tax, Work
//autoNOP
class AOrderSimple:public AOrder{
private:
    int number;
public:
    AOrderSimple();
    virtual ~AOrderSimple();
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual bool IsMonthLong();
    virtual int Productivity();
    virtual void Prepare();
    virtual int GetSize();
};
//autotax, avoid, behind, guard, hold, noaid
//consume, option-, reveal,nocross,nospoils, sharing
class AOrderByte:public AOrder{
public:
    unsigned char value;
    AOrderByte();
    virtual ~AOrderByte();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual int GetSize();
};
//combat, forget, produce, study
class AOrderAbr:public AOrder{
public:
    int number,maxnum;
    AnsiString abr;

    AOrderAbr();
    virtual ~AOrderAbr();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual bool IsMonthLong();
    virtual bool IsTradeOrder();
    virtual int Productivity();
    virtual void Prepare();
    virtual int GetSize();
};
class AOrderGive:public AOrder{
public:
	int tarnum;
	AUnit *target;
	int count,added,except;
    AnsiString item;
    AOrderMessage *message;

    AOrderGive();
    virtual ~AOrderGive();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual void DeleteMessage(AOrderMessage *mes);
    void SetMessage();
    virtual void Prepare();
    virtual int GetSize();
  AOrderGive * CreateCopy();
};
enum OrderMessageSubType{
 OM_UNK=0,
 OM_RECEIVED,
 OM_TAUGTH,
 OM_BORROWSFROM,
 OM_BORROWSTO,
 OM_STEALED,
 OM_ASSASSINATED,
 OM_ATTACKED,
};
class AOrderMessage:public AOrder{
public:
    AOrder *base;
    AUnit *target;
    AnsiString text;
    int subtype;
    int ivalue1;
    AnsiString svalue1;

    AOrderMessage(AOrder *b);
    virtual ~AOrderMessage();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual int GetSize();
};
class AOrderBuySell:public AOrder{
public:
    int number;


    AnsiString item;
    int amount;

    AOrderBuySell();
    virtual ~AOrderBuySell();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual bool IsTradeOrder();
    virtual int Productivity();
    virtual int GetSize();
};
struct AUnitPtr{
 int num;
 AUnit *ptr;
 AOrderMessage *mes;
};
class AOrderTeach:public AOrder{
//children: AOrderAttack
public:
    TList *targets;  //*AUnitPtr

    AOrders *bases;
    AOrderTeach();
    virtual ~AOrderTeach();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
	virtual void Run(int value);
    virtual AOrder * Edit();
    virtual void DeleteMessage(AOrderMessage *mes);
    void Clear();
    AUnitPtr * GetTarget(int index);
    virtual void AddTarget(int num, AUnit *target);
    void VerTargets();
    virtual bool IsMonthLong();
    virtual void Prepare();
    virtual int GetSize();
};
//claim, enter-, assasinate-, promote-
class AOrderInt:public AOrder{
public:
    int value;

    AOrderInt();
    virtual ~AOrderInt();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual int Productivity();
    virtual int GetSize();
};
enum {
  M_NONE,
  M_WALK,
  M_RIDE,
  M_FLY,
  M_SAIL
};
const MOVE_IN=10;
const MOVE_OUT=11;
/* Enter is MOVE_ENTER + num of object */
const MOVE_ENTER=12;

class AOrderMoving:public AOrder{
friend class TEditOrderMovingForm;
protected:
    TList *dirs;
    AnsiString custom;
public:
    int moved; //number of real moves

    AOrderMoving();
    virtual ~AOrderMoving();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder * Edit();
    int Get(int ind);
    void Add(int dir);
    void Delete(int ind);
    virtual bool IsMonthLong();
    virtual int GetSize();

    __property int count  = { read = Getcount };
    __property int* List={read =GetList};
private:
    int __fastcall Getcount(){return dirs->Count;}
    int* __fastcall GetList(){return (int*)dirs->List;}
};
class AOrderAutoGive:public AOrder{
public:
    int number;
    AnsiString item;
    AnsiString begstr;
    AnsiString substr;

    AOrderAutoGive();
    virtual ~AOrderAutoGive();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
//    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual int GetSize();
};
class AOrderBuild:public AOrder{
public:
    AnsiString object;
    int work;

    AOrderBuild();
    virtual ~AOrderBuild();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual bool IsMonthLong();
    virtual bool IsTradeOrder();
    virtual int GetSize();
};
class AOrderAutoMoving:public AOrderMoving{
friend class TEditOrderAutoMoveForm;
//repeating use as current
protected:
    ALocation begloc;
public:
    AOrderAutoMoving();
    virtual ~AOrderAutoMoving();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual bool IsMonthLong();
    virtual int GetSize();
    bool Step(ALocation &loc,AObject *& obj,int dir);
    ALocation GetLoc(int movepoints);
    int GetIndex(ALocation loc);
    AOrderMoving *CreateMoveOrder(int index);
/*    __property int count  = { read = Getcount };
    __property int* List={read =GetList};
private:
    int __fastcall Getcount(){return dirs->Count;}
    int* __fastcall GetList(){return (int*)dirs->List;}*/
};
class AOrderCast:public AOrder{
public:
    AnsiString abr;
    AnsiString params;
    AnsiString result;

    AOrderCast();
    virtual ~AOrderCast();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual int Productivity();
    virtual int GetSize();
};
class AOrderWithdraw:public AOrder{
public:
    bool clientonly;
    int count;
    AnsiString item;


    AOrderWithdraw(bool cl);
    virtual ~AOrderWithdraw();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual int GetSize();
};
class AOrderAutoWarning:public AOrder{
public:
    AnsiString mess;
    AnsiString expr;

    AOrderAutoWarning();
    virtual ~AOrderAutoWarning();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder *Edit();
    virtual int GetSize();
};

class AOrderSetAtt:public AOrder{
public:
    int att;//Hostyle-Ally, Default
            //DESCRIBE: 0 - UNIT, 1 - SHIP, 2 - BUILDING, 3 - OBJECT
    int fac;

    AOrderSetAtt();
    virtual ~AOrderSetAtt();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual AOrder * Edit();
    virtual int GetSize();
};

class AOrderNameDescribe:public AOrder{
public:
    int subtype;//NAME: 0 - UNIT, 1 - FACTION, 2 - OBJECT, 3 - CITY
                //DESCRIBE: 0 - UNIT, 1 - SHIP, 2 - BUILDING, 3 - OBJECT
    AnsiString value;

    AOrderNameDescribe();
    virtual ~AOrderNameDescribe();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual AOrder * Edit();
    virtual int GetSize();
};
class AOrderSteal:public AOrder{
public:
    int tarnum;
	AUnit *target;
	AnsiString item;
	AOrderMessage *message;
	int amount;

	AOrderSteal();
	virtual ~AOrderSteal();
	virtual void Read(TFile & in);
	virtual void Write(TFile & out);
	virtual AnsiString Print();
	virtual AnsiString WriteTemplate();
	virtual bool ParseOrder(char * s);
	virtual void Run(int value);
	virtual AOrder * Edit();
	virtual void DeleteMessage(AOrderMessage *mes);
	void SetMessage();
	virtual void Prepare();
	virtual int GetSize();
};
class AOrderAssassinate:public AOrder{
public:
	int tarnum;
	AUnit *target;
	AOrderMessage *message;

    AOrderAssassinate();
    virtual ~AOrderAssassinate();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual void DeleteMessage(AOrderMessage *mes);
    void SetMessage();
    virtual void Prepare();
    virtual int GetSize();
};
class AOrderAttack:public AOrderTeach{
public:
    AOrderAttack();
    virtual ~AOrderAttack();
    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual void DeleteMessage(AOrderMessage *mes);
    virtual void AddTarget(int num, AUnit *target);
    virtual bool IsMonthLong();
    virtual void Prepare();
    virtual int GetSize();
};
class AOrderPlugin:public AOrderCustom{
public:
    int OrderType;
    int ModuleNumber;
    int OrderNumber;
	char UUID[8];

	int runtime; //used only in ARegion::RunPluginOrders
                 //and ARegion::RunPluginAutoOrders

    AOrderPlugin(int MN=-1, int ON=-1);
    virtual ~AOrderPlugin();
    void UnPlug(); //delete order in plugin
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual int GetSize();
    bool VerifyPlugin(bool find=true);
    void FillOrderParams(bool withuuid=true);
    void LoadParams();
};
/*
class AOrderCustom:public AOrder{
public:

    AOrderCustom();
    virtual ~AOrderCustom();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual void Run(int value);
    virtual AOrder * Edit();
    virtual int Productivity();
	virtual int GetSize();
};
*/

class AOrderTurn:public AOrder{
public:
    int tplus;// This Turn +1 xxx
    AOrders *torders;

    AOrderTurn();
    virtual ~AOrderTurn();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
    virtual bool ParseOrder(char * s);
    virtual AOrder * Edit();
    virtual int GetSize();
};

class AOrderForm:public AOrder{
public:
    int num;
    AOrderForm();
	virtual ~AOrderForm();
    virtual void Read(TFile & in);
    virtual void Write(TFile & out);
    virtual AnsiString Print();
    virtual AnsiString WriteTemplate();
	virtual bool ParseOrder(char * s);
	virtual AOrder * Edit();
	virtual int GetSize();
};

class AOrderAutoFollow:public AOrder{
public:
	int tarnum;
	AUnit *target;

	AOrderAutoFollow();
	virtual void Read(TFile & in);
	virtual void Write(TFile & out);
	virtual AnsiString Print();
	virtual AnsiString WriteTemplate();
	virtual bool ParseOrder(char * s);
	virtual AOrder * Edit();
	//virtual int GetSize();
	virtual void RunFollow();
};

class AOrderAutoCollect:public AOrder{
public:
	AnsiString item;

	AOrderAutoCollect();

	virtual void Read(TFile & in);
	virtual void Write(TFile & out);
	virtual AnsiString Print();
	virtual AnsiString WriteTemplate();
	virtual bool ParseOrder(char * s);
	virtual AOrder * Edit();
	virtual void RunCollect();
};

class AOrderAutoBuy:public AOrder{
public:
	AnsiString item;
	int num, max;

	AOrderAutoBuy();
	virtual void Read(TFile & in);
	virtual void Write(TFile & out);
	virtual AnsiString Print();
	virtual AnsiString WriteTemplate();
	virtual bool ParseOrder(char * s);
	virtual AOrder * Edit();
	//virtual int GetSize();
	virtual void RunAutoBuy();
};

#endif
