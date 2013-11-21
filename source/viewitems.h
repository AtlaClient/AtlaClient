//---------------------------------------------------------------------------
#ifndef viewitemsH
#define viewitemsH
//---------------------------------------------------------------------------
enum VListItemTypes{
 litUnknown=0,

 litFactHeader,
 litFactErrors,
 litFactBattles,
 litFactEvents,
 litFactErrorsHeader,
 litFactError,
 litFactBattlesHeader,
 litFactBattleHeader,
 litFactBattle,
 litFactEventsHeader,
 litFactEvent,

 litRegHeader,
 litRegWeather,
 litRegWages,
 litRegWanted,
 litRegSaled,
 litRegEnte,
 litRegProd,
 litRegExit,
 litRegGate,
 litRegObj,

 litObjHeader,
 litObjUnit,

 litUnitHeader,
 litUnitInfo,
 litUnitOrder,
 litUnitEvents,
 litUnitEvent,
};
enum VTreeNodeTypes{
 tntUnknown=0,
 tntFaction,
 tntFactionErrors,
 tntFactionBattles,
 tntFactionBattle,
 tntFactionEvents,
 tntRegion,
 tntObject,
 tntUnit,
};
class VTreeNodes;
class VTreeNode{
public:
    TTreeNode *node;
    VTreeNodeTypes type;
    int value;
    void *data;
    VTreeNode(VTreeNodes*vtns);
    void SetText(AnsiString str);
};
class VTreeNodes:TList{
public:
    VTreeNodes();
    virtual __fastcall ~VTreeNodes();
    void __fastcall Clear();
    VTreeNode* Get(int index);
    void Add(VTreeNode*node);
    __property int count  = { read = Getcount };
private:
    int __fastcall Getcount(){return Count;}

};
class VListItems;
class VListItem{
public:
    AnsiString text;
    VListItemTypes type;
    int value;
    VListItem(VListItems * vlis);
};
class VListItems:TList{
public:
    VListItems();
    virtual __fastcall ~VListItems();
    void __fastcall Clear();
    VListItem * Get(int index);
    void Add(VListItem* vli);
    __property int count  = { read = Getcount };
private:
    int __fastcall Getcount(){return Count;}

};

#endif
