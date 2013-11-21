//---------------------------------------------------------------------------
#ifndef marketsH
#define marketsH
//---------------------------------------------------------------------------
class AMarket{
public:
//    AnsiString type;
    AItemType *type;
    int amount;
    int price; //also used for prod level
    AMarket();
    virtual __fastcall ~AMarket();
    void Read(TFile& in);
    void Write(TFile & out);
    void CreateNew(AMarket * newmark);
    void SetType(AnsiString type, bool addunknown=false);
    AnsiString Print(int mode=15);
//    AItemType* GetType();
    int GetSize();

};
class AMarkets:TList{
public:
    AMarkets();
    virtual __fastcall ~AMarkets();
    void __fastcall Clear();
    void Read(TFile& in);
    void Write(TFile& out);
    AMarket * Get(int index);
    void Add(AMarket * mark);
    AMarket * GetMarket(AnsiString type);
    AMarket * GetMarket(AItemType *type);
    int GetNum(AnsiString type);
    void CreateNew(AMarkets * prvmarks);
    void Update(AMarkets * newmarks);
    void UpdateFromPrev(AMarkets * prvmarks);
    AnsiString Print(int mode=15);
    void Assign(AItems * list);
    void DeleteEmpty();
    void Delete(int ind);
    int GetSize();

    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};

#endif
