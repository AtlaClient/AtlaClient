//---------------------------------------------------------------------------
#ifndef OptionsFrmH
#define OptionsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TOptionsForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
    TPageControl *PageControl;
  TTabSheet *tsColors;
  TLabel *labcurregColor;
  TButton *bnOk;
  TButton *bnCancel;
  TButton *bnApply;
  TLabel *labgridColor;
  TLabel *labwallColor;
  TLabel *labtextColor;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TColorDialog *ColorDialog;
  TLabel *labWedge0;
  TLabel *labWedge5;
  TLabel *labWedge4;
  TLabel *labWedge3;
  TLabel *labWedge2;
  TLabel *labWedge1;
  TLabel *Wedges;
    TTabSheet *tsOptions;
 TCheckBox *cbShowEvents;
 TCheckBox *cbRunRegionOrders;
 TLabel *Label5;
 TEdit *edTownNameLimit;
    TCheckBox *cbMaximizeWindow;
    TTabSheet *tsEvents;
    TCheckBox *cbdseRegDataFound;
    TTabSheet *tsServerMail;
    TLabel *Label6;
    TEdit *edServEMail;
    TCheckBox *cbNoServerMail;
    TLabel *Label7;
    TEdit *edOrdersSubj;
    TLabel *Label8;
    TEdit *edTimesSubj;
    TEdit *edTimesBody;
    TLabel *Label9;
    TEdit *edMailSubj;
    TTabSheet *tsServer;
    TCheckBox *cbNoCross;
    TCheckBox *cbNoSpoils;
    TCheckBox *cbdseIncome;
    TCheckBox *cbdseDiscard;
    TCheckBox *cbdseNoMonth;
    TCheckBox *cbShowLDescFirst;
    TCheckBox *cbGiveAll;
    TCheckBox *cbSpoils;
    TComboBox *cbServFlightOverWater;
    TLabel *labFlightOverWater;
    TLabel *labroadColor;
    TLabel *Label11;
  TCheckBox *cbCopyOnSaveOrders;
  TCheckBox *cbdseCantReach;
  TCheckBox *cbdseBorrows;
  TEdit *edAutoSaveTimer;
  TLabel *Label10;
  TLabel *Label12;
  TCheckBox *cbTaxIsMonthlong;
  TButton *bnChangeGameParams;
  TCheckBox *cbdseTeachNonLocNotStudy;
        TCheckBox *cbSharing;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *lbAlly;
        TLabel *lbFriend;
        TLabel *lbNeutral;
        TLabel *lbUndetect;
        TLabel *lbUnfrien;
        TLabel *lbHostile;
        TLabel *lbLocF;
        TCheckBox *cbColorUnit;
  void __fastcall bnApplyClick(TObject *Sender);
  void __fastcall labColorDblClick(TObject *Sender);
  void __fastcall bnChangeGameParamsClick(TObject *Sender);
private:
  bool loaded; //game is loaded
    void Load();	// User declarations
public:		// User declarations
  __fastcall TOptionsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOptionsForm *OptionsForm;
//---------------------------------------------------------------------------
#endif
