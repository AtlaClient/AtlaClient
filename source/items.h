//---------------------------------------------------------------------------
#ifndef itemsH
#define itemsH
//---------------------------------------------------------------------------
class AItem{
public:
    AItemType *type;
    int count;
    AItem();
    virtual __fastcall ~AItem();
    void Read(TFile& in);
    void Write(TFile& out);
    void CreateNew(AItem * newit);
    void SetType(AnsiString abr, bool addunknown=false);
    AnsiString Print(int mode=7);
    AnsiString WriteReport(bool seeillusions);
    int GetSize();
    bool IsSoldier();
    int GetSortType();

};
class AItems:TList{
public:
    AItems();
    virtual __fastcall ~AItems();
    void __fastcall Clear();
    void Read(TFile& in);
    void Write(TFile & out);
    AItem* Get(int index);
    AItem* GetItem(AnsiString type);
    AItem* GetItem(AItemType *type);
    void Add(AItem * item);
    void CreateNew(AItems * prvitems);
    void Update(AItems * newitems);
    void UpdateFromPrev(AItems * prvitems);
    AnsiString Print();
    AnsiString WriteReport(int obs,bool seeillusions,bool nofirstcomma);
    AnsiString BattleReport(bool nofirstcomma=true);
    int GetNum(AnsiString type);
    void SetNum(AnsiString type, int num);
    int GetNum(AItemType *type);
    void SetNum(AItemType *type, int num);
    void DeleteEmpty();
    int GetSize();
    void DeleteNonCombat();
    void Sort();

    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};


#endif
