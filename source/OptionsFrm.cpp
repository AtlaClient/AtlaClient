//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "util.h"
#include "OptionsFrm.h"
#include "MainFrm.h"
#include "gameparamsfrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptionsForm *OptionsForm;
//---------------------------------------------------------------------------
extern TColor curregColor,wallColor;
extern TColor gridColor,textColor,roadColor;
extern TColor wedgecolors[6];
extern TColor infocolors[7];

__fastcall TOptionsForm::TOptionsForm(TComponent* Owner)
  : TForm(Owner)
{
 Load();
 PageControl->ActivePageIndex=-1;
 PageControl->SelectNextPage(true);
}
//---------------------------------------------------------------------------
void __fastcall TOptionsForm::bnApplyClick(TObject *Sender)
{
 if(loaded){
  curregColor=labcurregColor->Color;
  wallColor=labwallColor->Color;
  gridColor=labgridColor->Color;
  textColor=labtextColor->Color;
  roadColor=labroadColor->Color;
  wedgecolors[0]=labWedge0->Color;
  wedgecolors[1]=labWedge1->Color;
  wedgecolors[2]=labWedge2->Color;
  wedgecolors[3]=labWedge3->Color;
  wedgecolors[4]=labWedge4->Color;
  wedgecolors[5]=labWedge5->Color;

  infocolors[0] = lbHostile->Color;
  infocolors[1] = lbUnfrien->Color;
  infocolors[2] = lbNeutral->Color;
  infocolors[3] = lbFriend->Color;
  infocolors[4] = lbAlly->Color;
  infocolors[5] = lbLocF->Color;
  infocolors[6] = lbUndetect->Color;
 }
 
 optShowEvents=cbShowEvents->Checked;
 optColorUnit = cbColorUnit->Checked;

 optRunRegionOrders=cbRunRegionOrders->Checked;
 optMaximizeWindow=cbMaximizeWindow->Checked;
 optTownNameLimit=atoi(edTownNameLimit->Text.c_str());
 optAutoSaveTimer=atoi(edAutoSaveTimer->Text.c_str());
 optShowLDescFirst=cbShowLDescFirst->Checked;
 SetBits(optDontShowEvents,dseDiscard,cbdseDiscard->Checked);
 SetBits(optDontShowEvents,dseNoMonth,cbdseNoMonth->Checked);
 SetBits(optDontShowEvents,dseRegDataFound,cbdseRegDataFound->Checked);
 SetBits(optDontShowEvents,dseIncome,cbdseIncome->Checked);
 SetBits(optDontShowEvents,dseCantReach,cbdseCantReach->Checked);
 SetBits(optDontShowEvents,dseBorrows,cbdseBorrows->Checked);
 SetBits(optDontShowEvents,dseTeachNonLocNotStudy,cbdseTeachNonLocNotStudy->Checked);
 optCopyOnSaveOrders=cbCopyOnSaveOrders->Checked;

 if(loaded){
  optServNoCross=cbNoCross->Checked;
  optServNoSpoils=cbNoSpoils->Checked;
  optServGiveAll=cbGiveAll->Checked;
  optServSpoils=cbSpoils->Checked;
  optTaxIsMonthlong=cbTaxIsMonthlong->Checked;
  optServerSharing=cbSharing->Checked;

  optServEMail=edServEMail->Text;
  optServOrdersSubj=edOrdersSubj->Text;
  optServTimesSubj=edTimesSubj->Text;
  optServTimesBody=edTimesBody->Text;
  optServMailSubj=edMailSubj->Text;
  optNoServerMail=cbNoServerMail->Checked;
  optServFlightOverWater=cbServFlightOverWater->ItemIndex;
 }

 if(loaded)
  AtlaForm->OnSetCurRegion();
}
//---------------------------------------------------------------------------
void TOptionsForm::Load()
{
 loaded=true;
 if(!curGame.Length())loaded=false;
 if(!curTurns)loaded=false;
 if(loaded){
  labcurregColor->Color=curregColor;
  labwallColor->Color=wallColor;
  labgridColor->Color=gridColor;
  labtextColor->Color=textColor;
  labroadColor->Color=roadColor;

  labWedge0->Color=wedgecolors[0];
  labWedge1->Color=wedgecolors[1];
  labWedge2->Color=wedgecolors[2];
  labWedge3->Color=wedgecolors[3];
  labWedge4->Color=wedgecolors[4];
  labWedge5->Color=wedgecolors[5];

  lbHostile->Color = infocolors[0];
  lbUnfrien->Color = infocolors[1];
  lbNeutral->Color = infocolors[2];
  lbFriend->Color = infocolors[3];
  lbAlly->Color = infocolors[4];
  lbLocF->Color = infocolors[5];
  lbUndetect->Color = infocolors[6];

 }else{
  tsColors->TabVisible=false; 
 }


 cbColorUnit->Checked = optColorUnit;
 cbShowEvents->Checked=optShowEvents;
 cbRunRegionOrders->Checked=optRunRegionOrders;
 cbMaximizeWindow->Checked=optMaximizeWindow;
 edTownNameLimit->Text=optTownNameLimit;
 edAutoSaveTimer->Text=optAutoSaveTimer;
 cbShowLDescFirst->Checked=optShowLDescFirst;

 cbdseDiscard->Checked=optDontShowEvents&dseDiscard;
 cbdseNoMonth->Checked=optDontShowEvents&dseNoMonth;
 cbdseRegDataFound->Checked=optDontShowEvents&dseRegDataFound;
 cbdseIncome->Checked=optDontShowEvents&dseIncome;
 cbdseCantReach->Checked=optDontShowEvents&dseCantReach;
 cbdseBorrows->Checked=optDontShowEvents&dseBorrows;
 cbdseTeachNonLocNotStudy->Checked=optDontShowEvents&dseTeachNonLocNotStudy;

 cbCopyOnSaveOrders->Checked=optCopyOnSaveOrders;

 if(loaded){
  cbNoCross->Checked=optServNoCross;
  cbNoSpoils->Checked=optServNoSpoils;
  cbGiveAll->Checked=optServGiveAll;
  cbSpoils->Checked=optServSpoils;
  cbTaxIsMonthlong->Checked=optTaxIsMonthlong;
  cbSharing->Checked=optServerSharing;

  edServEMail->Text=optServEMail;
  edOrdersSubj->Text=optServOrdersSubj;
  edTimesSubj->Text=optServTimesSubj;
  edTimesBody->Text=optServTimesBody;
  edMailSubj->Text=optServMailSubj;
  cbNoServerMail->Checked=optNoServerMail;
  cbServFlightOverWater->ItemIndex=optServFlightOverWater;
 }else{
  tsServer->TabVisible=false;
  tsServerMail->TabVisible=false;
 }
}
void __fastcall TOptionsForm::labColorDblClick(TObject *Sender)
{
 TLabel *lab=dynamic_cast<TLabel*>(Sender);
 if(!lab) return;
 AnsiString str=ColorToString(lab->Color);
 if(str[1]=='0'){
  ColorDialog->CustomColors->Values["ColorA"]=str.SubString(5,str.Length());
 }
 ColorDialog->Color=lab->Color;
 if(ColorDialog->Execute()){
  lab->Color=ColorDialog->Color;
 }
}
//---------------------------------------------------------------------------
void __fastcall TOptionsForm::bnChangeGameParamsClick(TObject *Sender)
{
  TGameParamsForm *frm=new TGameParamsForm(this);
  frm->ShowModal();
  delete frm;
}
//---------------------------------------------------------------------------

