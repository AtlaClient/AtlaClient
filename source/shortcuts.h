//---------------------------------------------------------------------------
#ifndef shortcutsH
#define shortcutsH
//---------------------------------------------------------------------------
#include <vector.h>
//#include <map>
class TShortCutOwner{
public:
  TMenuItem *mi;
  AnsiString prefix;
  int level;
  AnsiString name;
  int sortorder;
  AnsiString code;
  TTreeNode *node;
  TShortCutOwner():level(0),mi(0),sortorder(0),node(0){}
  bool operator<(const TShortCutOwner &b) const{return sortorder<b.sortorder;}
  bool operator==(const TMenuItem *mi2) const{return mi==mi2;}
  bool operator==(const TTreeNode *node2) const{return node==node2;}
  bool operator!=(const TTreeNode *node2) const{return node!=node2;}
  AnsiString GetNameStr() const;
  AnsiString GetPrefixedStr() const;
};
class TShortCuts{
  class TShortCutEqualGroupElem{
  public:
    TMenuItem *mi;
    int groupindex;
    TShortCutEqualGroupElem():mi(0),groupindex(-1){}
    bool operator<(const TShortCutEqualGroupElem &b) const{return groupindex<b.groupindex;}
    bool operator==(const TMenuItem *mi2) const{return mi==mi2;}
    bool operator!=(const TMenuItem *mi2) const{return mi!=mi2;}
  };
  class TPrefixGroupElem{
  public:
    int groupindex;
    AnsiString prefix;
    TPrefixGroupElem():groupindex(-1){}
    bool operator<(const TPrefixGroupElem &b) const{return groupindex<b.groupindex;}
    bool operator==(const AnsiString &pr2) const{return prefix==pr2;}
    bool operator!=(const AnsiString &pr2) const{return prefix!=pr2;}
  };
  class TEqualPrefixGroupElem{
  public:
    int pg1,pg2;
    TEqualPrefixGroupElem():pg1(-1),pg2(-1){}
    bool operator==(const TEqualPrefixGroupElem &b) const{
      return (pg1==b.pg1&&pg2==b.pg2)||(pg1==b.pg2&&pg2==b.pg1);
    }
    bool operator!=(const TEqualPrefixGroupElem &b) const{
      return !(pg1==b.pg1&&pg2==b.pg2)||(pg1==b.pg2&&pg2==b.pg1);
    }
  };
  class TMyShortCut{
  public:
    TShortCut shortcut;
    AnsiString str;
    TMyShortCut():shortcut(0){}
    bool operator==(const AnsiString &str2) const{return str==str2;}
    bool operator!=(const AnsiString &str2) const{return str!=str2;}
    bool operator<(const TMyShortCut &b) const{
      if(shortcut<b.shortcut)
        return true;
      if(shortcut>b.shortcut)
        return false;
      return str<b.str;
    }
  };
private:
//список "активных" шорткатов (для которых есть меню и т.п.)
  vector<TShortCutOwner> shortcutowners;
//список "заблокированных" пунктов меню для AddOwnerMenu
  vector<TMenuItem*> blockedowners;
//для пунктов меню с одинаковым номером группы можно поставить
//один шорткат даже есть у них разный Action
  vector<TShortCutEqualGroupElem> equalgroups;
  int lastequalgroup;
//список всех установленных или измененных шорткатов
//даже тех, которые временно не используются (например, нет плагина)
  vector<TMyShortCut> shortcuts;
//можно создавать группы префиксов и говорить, что пункты меню в две
//данные группы могут иметь одинаковые шорткаты
  vector<TPrefixGroupElem> prefixgroups;
  int lastprefixgroup;
  vector<TEqualPrefixGroupElem> equalprefixgroups;

  bool IsBlockedOwner(TMenuItem *mi);
public:
  TShortCuts();
  ~TShortCuts();

  bool AddOwner(TMenuItem *mi,AnsiString Prefix, int level,int sortorder);
  bool AddOwnerMenu(TMenuItem *root, AnsiString Prefix, AnsiString RootName,int sortorder);
  bool AddOwnerWithCode(TMenuItem *mi,AnsiString code,AnsiString Prefix, int level,int sortorder);
  bool AddOwnerWithCodeRoot(AnsiString Prefix, AnsiString RootName,int sortorder);
  void DeleteOwnersWithPrefix(AnsiString Prefix);
  void SortOwners();

  bool MakeTree(TTreeNodes * nodes);
  void ClearTreeInfo();
  const TShortCutOwner* GetOwnerFromTree(const TTreeNode *node) const;

  void ClearBlockedOwners();
  void AddBlockedOwner(TMenuItem *mi);

  int CreateNewEqualGroup();
  void AddToEqualGroup(int groupindex, TMenuItem *mi);
  void SortEqualGroups();
  int GetEqualGroupIndex(TMenuItem *mi) const;

  int CreateNewPrefixGroup();
  void AddToPrefixGroup(int groupindex, AnsiString Prefix);
  void SortPrefixGroups();
  int GetPrefixGroupIndex(AnsiString prefix) const;
  void AddEqualPrefixGroups(int pg1,int pg2);
  bool IsEqualPrefixGroups(int pg1,int pg2);

  bool ChangeShortCut(const TShortCutOwner *sco, TShortCut shortcut, AnsiString &error);
  void SetShortCut(TShortCut shortcut,AnsiString str);
  void ClearShortCut(TShortCut shortcut,AnsiString str);
  void ClearAllShortCuts(TShortCut shortcut);
  TShortCut GetShortCut(AnsiString str); //65535 for not found
  void SortShortCuts();

  void FillShortCutsList(TStrings *list) const;
  bool Save();
  bool Load();
};

extern TShortCuts ShortCuts;
#endif
