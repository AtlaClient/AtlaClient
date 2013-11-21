//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <algorithm>
#include <inifiles.hpp>
#include "shortcuts.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

TShortCuts ShortCuts;
static const AnsiString IniSect="ShortCuts";
static const AnsiString noneStr="none";
extern AnsiString IniName; //in MainFrm.cpp & util.h
//---------------------------------------------------------------------------
AnsiString TShortCutOwner::GetNameStr() const
{
  AnsiString str=name;
  AnsiString shortcut;
  if(mi)
    shortcut=ShortCutToText(mi->ShortCut);
  if(shortcut.Length())
    str+=" ("+shortcut+")";
  return str;
}
//---------------------------------------------------------------------------
AnsiString TShortCutOwner::GetPrefixedStr() const
{
  AnsiString str=prefix+":";
  if(code.Length())
    str+=code;
  else if(mi)
    str+=mi->Name;
  return str;
}
//---------------------------------------------------------------------------
TShortCuts::TShortCuts()
  :lastequalgroup(0),lastprefixgroup(0)
{
}
//---------------------------------------------------------------------------
TShortCuts::~TShortCuts()
{
}
//---------------------------------------------------------------------------
bool TShortCuts::AddOwner(TMenuItem *mi,AnsiString Prefix, int level, int sortorder)
{
  if(mi->IsLine()) //skip separators
    return true;
  if(IsBlockedOwner(mi))
    return true;
  TShortCutOwner sco;
  sco.mi=mi;
  sco.prefix=Prefix;
  sco.level=level;
  sco.name=StripHotkey(mi->Caption);
  sco.sortorder=sortorder;
  shortcutowners.push_back(sco);
  if(mi&&mi->ShortCut)
    SetShortCut(mi->ShortCut,sco.GetPrefixedStr());
  int count=mi->Count;
  if(!count) return true;
  level++;
  for(int i=0;i<count;i++)
  {
    TMenuItem *mi2=mi->Items[i];
    if(!AddOwner(mi2,Prefix,level,sortorder))
      return false;
  }
  return true;
}
//---------------------------------------------------------------------------
bool TShortCuts::AddOwnerMenu(TMenuItem *root, AnsiString Prefix, AnsiString RootName, int sortorder)
{
  int count=root->Count;
  if(!count) return false;
  TShortCutOwner sco;
  sco.mi=0;
  sco.prefix=Prefix;
  sco.level=0;
  sco.name=RootName;
  sco.sortorder=sortorder;
  shortcutowners.push_back(sco);
  for(int i=0;i<count;i++)
  {
    TMenuItem *mi2=root->Items[i];
    if(!AddOwner(mi2,Prefix,1,sortorder))
      return false;
  }
  return true;
}
//---------------------------------------------------------------------------
bool TShortCuts::AddOwnerWithCode(TMenuItem *mi,AnsiString code,AnsiString Prefix, int level,int sortorder)
{
  if(mi->IsLine()) //skip separators
    return false;
  if(mi->Count)
    return false;
  if(IsBlockedOwner(mi))
    return false;

  TShortCutOwner sco;
  sco.mi=mi;
  sco.prefix=Prefix;
  sco.level=level;
  sco.name=StripHotkey(mi->Caption);
  sco.sortorder=sortorder;
  sco.code=code;
  shortcutowners.push_back(sco);
  if(mi)
  {
    TShortCut sc=GetShortCut(sco.GetPrefixedStr());
    if(sc!=65535)
    {
	  TShortCutOwner *sco=shortcutowners.end().operator ->()-1;
      AnsiString error;
      ChangeShortCut(sco,sc,error);
    }else if(mi->ShortCut)
      SetShortCut(mi->ShortCut,sco.GetPrefixedStr());
  }
  return true;
}
//---------------------------------------------------------------------------
bool TShortCuts::AddOwnerWithCodeRoot(AnsiString Prefix, AnsiString RootName,int sortorder)
{
  TShortCutOwner sco;
  sco.mi=0;
  sco.prefix=Prefix;
  sco.level=0;
  sco.name=RootName;
  sco.sortorder=sortorder;
  shortcutowners.push_back(sco);
  return true;
}
//---------------------------------------------------------------------------
bool TShortCuts::MakeTree(TTreeNodes * nodes)
{
  TTreeNode *node=0;
  nodes->BeginUpdate();
  nodes->Clear();
  for(TShortCutOwner *sco=shortcutowners.begin().operator ->(),*endsco=shortcutowners.end().operator ->();sco<endsco;sco++){
    while(node&&node->Level>sco->level-1){
      node=node->Parent;
    }
    node=nodes->AddChild(node,sco->GetNameStr());
    sco->node=node;
  }
  nodes->EndUpdate();
  return true;
}
//---------------------------------------------------------------------------
void TShortCuts::ClearTreeInfo()
{
  for(TShortCutOwner *sco=shortcutowners.begin().operator ->(),*endsco=shortcutowners.end().operator ->();sco<endsco;sco++)
    sco->node=0;
}
//---------------------------------------------------------------------------
const TShortCutOwner* TShortCuts::GetOwnerFromTree(const TTreeNode *node) const
{
  const TShortCutOwner *sco=find(shortcutowners.begin().operator ->(),shortcutowners.end().operator ->(),node);
  if(sco==shortcutowners.end().operator ->())
    return 0;
  return sco;
}
//---------------------------------------------------------------------------
void TShortCuts::DeleteOwnersWithPrefix(AnsiString Prefix)
{
  if(!shortcutowners.size())
    return;
  for(TShortCutOwner *sco=shortcutowners.end().operator ->()-1,*endsco=shortcutowners.begin().operator ->();sco>=endsco;sco--)
  {
    if(sco->prefix!=Prefix) continue;
    shortcutowners.erase(sco);
  }
}
//---------------------------------------------------------------------------
void TShortCuts::SortOwners()
{
  stable_sort(shortcutowners.begin(),shortcutowners.end());
}
//---------------------------------------------------------------------------
void TShortCuts::ClearBlockedOwners()
{
  blockedowners.clear();
}
//---------------------------------------------------------------------------
void TShortCuts::AddBlockedOwner(TMenuItem *mi)
{
  blockedowners.push_back(mi);
}
//---------------------------------------------------------------------------
bool TShortCuts::IsBlockedOwner(TMenuItem *mi)
{
  TMenuItem **i=find(blockedowners.begin().operator ->(),blockedowners.end().operator ->(),mi);
  if(i==blockedowners.end().operator ->())
    return false;
  return true;
}
//---------------------------------------------------------------------------
int TShortCuts::CreateNewEqualGroup()
{
  return ++lastequalgroup;
}
//---------------------------------------------------------------------------
void TShortCuts::AddToEqualGroup(int groupindex, TMenuItem *mi)
{
  if(groupindex<0||groupindex>lastequalgroup)
    return;
  TShortCutEqualGroupElem elem;
  elem.mi=mi;
  elem.groupindex=groupindex;
  equalgroups.push_back(elem);
}
//---------------------------------------------------------------------------
void TShortCuts::SortEqualGroups()
{
  stable_sort(equalgroups.begin(),equalgroups.end());
}
//---------------------------------------------------------------------------
int TShortCuts::GetEqualGroupIndex(TMenuItem *mi) const
{
  const TShortCutEqualGroupElem *elem=find(equalgroups.begin().operator ->(),equalgroups.end().operator ->(),mi);
  if(elem==equalgroups.end().operator ->())
    return -1;
  return elem->groupindex;
}
//---------------------------------------------------------------------------
int TShortCuts::CreateNewPrefixGroup()
{
  return ++lastprefixgroup;
}
//---------------------------------------------------------------------------
void TShortCuts::AddToPrefixGroup(int groupindex, AnsiString Prefix)
{
  if(groupindex<0||groupindex>lastprefixgroup)
    return;
  TPrefixGroupElem elem;
  elem.prefix=Prefix;
  elem.groupindex=groupindex;
  prefixgroups.push_back(elem);
}
//---------------------------------------------------------------------------
void TShortCuts::SortPrefixGroups()
{
  stable_sort(prefixgroups.begin(),prefixgroups.end());
}
//---------------------------------------------------------------------------
int TShortCuts::GetPrefixGroupIndex(AnsiString prefix) const
{
  const TPrefixGroupElem *elem=find(prefixgroups.begin().operator ->(),prefixgroups.end().operator ->(),prefix);
  if(elem==prefixgroups.end().operator ->())
    return -1;
  return elem->groupindex;
}
//---------------------------------------------------------------------------
void TShortCuts::AddEqualPrefixGroups(int pg1,int pg2)
{
  TEqualPrefixGroupElem pge;
  pge.pg1=pg1;
  pge.pg2=pg2;
  equalprefixgroups.push_back(pge);
}
//---------------------------------------------------------------------------
bool TShortCuts::IsEqualPrefixGroups(int pg1,int pg2)
{
  if(pg1<0||pg2<0) return false;
  TEqualPrefixGroupElem pge;
  pge.pg1=pg1;
  pge.pg2=pg2;
  TEqualPrefixGroupElem *pge2=find(equalprefixgroups.begin().operator ->(),equalprefixgroups.end().operator ->(),pge);
  if(pge2==equalprefixgroups.end().operator ->())
    return false;
  return true;
}
//---------------------------------------------------------------------------
bool TShortCuts::ChangeShortCut(const TShortCutOwner *sco, TShortCut shortcut, AnsiString &error)
{
  error.SetLength(0);
  if(!sco) return false;
  TMenuItem *mi=sco->mi;
  if(!mi) return false;
  if(mi->ShortCut==shortcut) return true;
  int oldshortcut=mi->ShortCut;
  TBasicAction *act=mi->Action;
  int groupindex=-2;
  AnsiString prefix=sco->prefix;
  int pgroupindex=-2;
  if(shortcut)
  for(TShortCutOwner *sco=shortcutowners.begin().operator ->(),*endsco=shortcutowners.end().operator ->();sco<endsco;sco++)
  {
    TMenuItem *mi2=sco->mi;
    if(!mi2) continue;
    if(mi2->ShortCut!=shortcut) continue;
    if(mi2==mi) continue;
    if(mi2->Action&&mi2->Action==act) continue;
    if(groupindex<0)
      groupindex=GetEqualGroupIndex(mi);
    if(groupindex>=0)
    {
      int groupindex2=GetEqualGroupIndex(mi2);
      if(groupindex==groupindex2) continue;
    }
    if(pgroupindex<0)
      pgroupindex=GetPrefixGroupIndex(prefix);
    if(pgroupindex>=0)
    {
      int pgroupindex2=GetPrefixGroupIndex(sco->prefix);
      if(IsEqualPrefixGroups(pgroupindex,pgroupindex2)) continue;
    }
    error=sco->name;
    return false;
  }
  TCustomAction *act2=dynamic_cast<TCustomAction*>(act);
  if(act2)
  {
    act2->ShortCut=shortcut;
	for(TShortCutOwner *sco=shortcutowners.begin().operator ->(),*endsco=shortcutowners.end().operator ->();sco<endsco;sco++)
    {
      TMenuItem *mi2=sco->mi;
      if(!mi2) continue;
      if(!mi2->Action||mi2->Action!=act) continue;
      if(sco->node)
        sco->node->Text=sco->GetNameStr();
      AnsiString pref=sco->GetPrefixedStr();
      ClearShortCut(oldshortcut,pref);
      SetShortCut(shortcut,pref);
    }
  }else{
    sco->mi->ShortCut=shortcut;
    if(sco->node)
      sco->node->Text=sco->GetNameStr();
    AnsiString pref=sco->GetPrefixedStr();
    ClearShortCut(oldshortcut,pref);
    SetShortCut(shortcut,pref);
  }
  return true;
}
//---------------------------------------------------------------------------
void TShortCuts::SetShortCut(TShortCut shortcut,AnsiString str)
{
  TMyShortCut sc;
  sc.shortcut=shortcut;
  sc.str=str;  
  TMyShortCut *begin=shortcuts.begin().operator ->(),*end=shortcuts.end().operator ->();
  TMyShortCut *msc=lower_bound(begin,end,sc);
  if(msc!=end)
    if(msc->shortcut==shortcut&&msc->str==str)
      return;
  shortcuts.insert(msc,sc);
}
//---------------------------------------------------------------------------
void TShortCuts::ClearShortCut(TShortCut shortcut,AnsiString str)
{
  TMyShortCut sc;
  sc.shortcut=shortcut;
  sc.str=str;
  TMyShortCut *begin=shortcuts.begin().operator ->(),*end=shortcuts.end().operator ->();
  TMyShortCut *msc=lower_bound(begin,end,sc);
  if(msc==end)
    return;
  if(msc->shortcut!=shortcut||msc->str!=str)
    return;
  shortcuts.erase(msc);
}
//---------------------------------------------------------------------------
void TShortCuts::ClearAllShortCuts(TShortCut shortcut)
{
  if(!shortcut) return;
  AnsiString error;
  for(TShortCutOwner *sco=shortcutowners.begin().operator ->(),*endsco=shortcutowners.end().operator ->();sco<endsco;sco++)
  {
    TMenuItem *mi2=sco->mi;
    if(!mi2) continue;
    if(mi2->ShortCut!=shortcut) continue;
    ChangeShortCut(sco,0,error);
  }
  TMyShortCut sc;
  sc.shortcut=shortcut;
  TMyShortCut *begin=shortcuts.begin().operator ->(),*end=shortcuts.end().operator ->();
  TMyShortCut *msc=lower_bound(begin,end,sc);
  if(msc==end)
    return;
  sc.str="\xFF\xFF\xFF";
  TMyShortCut *msc2=lower_bound(begin,end,sc);
  shortcuts.erase(msc,msc2);
}
//---------------------------------------------------------------------------
TShortCut TShortCuts::GetShortCut(AnsiString str)
{
  TMyShortCut *begin=shortcuts.begin().operator ->(),*end=shortcuts.end().operator ->();
  TMyShortCut *msc=find(begin,end,str);
  if(msc==end)
    return 65535;
  return msc->shortcut;
}
//---------------------------------------------------------------------------
void TShortCuts::SortShortCuts()
{
  stable_sort(shortcuts.begin(),shortcuts.end());
}
//---------------------------------------------------------------------------
void TShortCuts::FillShortCutsList(TStrings *list) const
{
  list->BeginUpdate();
  list->Clear();
  for(const TMyShortCut *msc=shortcuts.begin().operator ->(),*endmsc=shortcuts.end().operator ->();msc<endmsc;msc++)
  {
    AnsiString str=msc->str;
    str+=" ("+ShortCutToText(msc->shortcut)+")";
    list->Add(str);
  }
  list->EndUpdate();
}
//---------------------------------------------------------------------------
bool TShortCuts::Save()
{
  try{
    TIniFile *ini=new TIniFile(IniName);
    ini->EraseSection(IniSect);
    int num=0;
    ini->WriteString(IniSect,"Count",shortcuts.size());
	for(const TMyShortCut *msc=shortcuts.begin().operator ->(),*endmsc=shortcuts.end().operator ->();msc<endmsc;msc++,num++)
    {
      TShortCut sc=msc->shortcut;
      ini->WriteString(IniSect,AnsiString("Name")+num,"\""+msc->str+"\"");
			ini->WriteString(IniSect,AnsiString("ShortCut")+num,sc);
		}
    delete ini;
  }catch(...){
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
bool TShortCuts::Load()
{
  TStringList *list0=new TStringList;
  TStringList *list=new TStringList;
  try{
    TIniFile *ini=new TIniFile(IniName);
    ini->ReadSectionValues(IniSect,list0);
    if(list0->Count)
    {
      int count=atoi(list0->Values["Count"].c_str());
      for(int num=0;num<count;num++)
      {
        AnsiString pref=list0->Values[AnsiString("Name")+num];
        AnsiString sc=list0->Values[AnsiString("ShortCut")+num];
        if(pref.Length()&&sc.Length())
        {
          list->Add(pref);
          list->Add(sc);
        }
      }
      list0->Clear();
    }
    if(list->Count)
    {
      AnsiString error,nullstr;
      for(TShortCutOwner *sco=shortcutowners.begin().operator ->(),*endsco=shortcutowners.end().operator ->();sco<endsco;sco++)
      {
        AnsiString pref=sco->GetPrefixedStr();
        int ind=list->IndexOf(pref);
        if(ind<0||ind>=list->Count-1) continue;
        AnsiString str=list->Strings[ind+1];
				TShortCut sc=0;
				if(isdigit(*str.c_str())){
					sc=atoi(str.c_str());
				}else if(str!=noneStr)
        {
          try{
            sc=TextToShortCut(str);
          }catch(...){}
        }
        list->Delete(ind);
        list->Delete(ind);
        if(!ChangeShortCut(sco,sc,error))
        {
          ClearAllShortCuts(sc);
          ChangeShortCut(sco,sc,error);
        }
      }
      for(int i=0;i<list->Count;i+=2)
      {
        AnsiString pref=list->Strings[i];
        AnsiString str=list->Strings[i+1];
        TShortCut sc=0;
				if(isdigit(*str.c_str())){
					sc=atoi(str.c_str());
				}else if(str!=noneStr)
        {
          try{
            sc=TextToShortCut(str);
          }catch(...){}
        }
        SetShortCut(sc,pref);
      }
    }
    delete ini;
  }catch(...){
    delete list;
    delete list0;
    return false;
  }
  delete list;
  delete list0;
  return true;
}
//---------------------------------------------------------------------------

