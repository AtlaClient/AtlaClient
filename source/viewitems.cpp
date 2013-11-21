//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <comctrls.hpp>

#include "viewitems.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

VTreeNode::VTreeNode(VTreeNodes*vtns)
 :type(tntUnknown),value(-1),node(0),data(0)
{
 vtns->Add(this);
}
void VTreeNode::SetText(AnsiString str)
{
  if(!node) return;
  if(node->Text==str) return;
  TCustomTreeView *tv=node->TreeView;
  TTreeNode *sel=tv->Selected;
  tv->Selected=0;
  node->Text=str;
  tv->Selected=sel;
}
VTreeNodes::VTreeNodes()
{
}
__fastcall VTreeNodes::~VTreeNodes()
{
 Clear();
}
void __fastcall VTreeNodes::Clear()
{
 for(int i=Count-1;i>=0;i--){
  VTreeNode *tn=Get(i);
//  Delete(i);
  delete tn;
 }
 TList::Clear();
}
VTreeNode* VTreeNodes::Get(int index)
{
 return (VTreeNode*)Items[index];
}
void VTreeNodes::Add(VTreeNode*node)
{
 TList::Add(node);
}
VListItem::VListItem(VListItems * vlis)
 :type(litUnknown),value(-1)
{
 vlis->Add(this);
}
VListItems::VListItems()
{
}
__fastcall VListItems::~VListItems()
{
 Clear();
}
void __fastcall VListItems::Clear()
{
 for(int i=Count-1;i>=0;i--){
  VListItem *li=Get(i);
//  Delete(i);
  delete li;
 }
 TList::Clear();
}
VListItem * VListItems::Get(int index)
{
 return (VListItem*)Items[index];
}
void VListItems::Add(VListItem* vli)
{
 TList::Add(vli);
}

