object OptionsForm: TOptionsForm
  Left = 357
  Top = 112
  BorderStyle = bsDialog
  Caption = 'Options'
  ClientHeight = 233
  ClientWidth = 271
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 195
    Width = 271
    Height = 38
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object bnOk: TButton
      Left = 10
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Ok'
      ModalResult = 1
      TabOrder = 0
      OnClick = bnApplyClick
    end
    object bnCancel: TButton
      Left = 98
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
    end
    object bnApply: TButton
      Left = 186
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Apply'
      TabOrder = 2
      OnClick = bnApplyClick
    end
  end
  object PageControl: TPageControl
    Left = 0
    Top = 0
    Width = 271
    Height = 195
    ActivePage = tsServer
    Align = alClient
    TabOrder = 1
    object tsColors: TTabSheet
      Caption = 'Colors'
      object labcurregColor: TLabel
        Left = 8
        Top = 1
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object labgridColor: TLabel
        Left = 8
        Top = 25
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object labwallColor: TLabel
        Left = 8
        Top = 49
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object labtextColor: TLabel
        Left = 8
        Top = 73
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object Label1: TLabel
        Left = 29
        Top = 1
        Width = 121
        Height = 17
        AutoSize = False
        Caption = 'Current Region'
        Layout = tlCenter
      end
      object Label2: TLabel
        Left = 29
        Top = 25
        Width = 121
        Height = 17
        AutoSize = False
        Caption = 'Grid'
        Layout = tlCenter
      end
      object Label3: TLabel
        Left = 29
        Top = 49
        Width = 121
        Height = 17
        AutoSize = False
        Caption = 'Wall'
        Layout = tlCenter
      end
      object Label4: TLabel
        Left = 29
        Top = 73
        Width = 121
        Height = 17
        AutoSize = False
        Caption = 'Text'
        Layout = tlCenter
      end
      object labWedge0: TLabel
        Left = 184
        Top = 21
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object labWedge5: TLabel
        Left = 160
        Top = 29
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object labWedge4: TLabel
        Left = 160
        Top = 53
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object labWedge3: TLabel
        Left = 184
        Top = 61
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object labWedge2: TLabel
        Left = 208
        Top = 53
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object labWedge1: TLabel
        Left = 208
        Top = 29
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object Wedges: TLabel
        Left = 171
        Top = 3
        Width = 40
        Height = 13
        Caption = 'Wedges'
      end
      object labroadColor: TLabel
        Left = 8
        Top = 97
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object Label11: TLabel
        Left = 29
        Top = 97
        Width = 57
        Height = 17
        AutoSize = False
        Caption = 'Road'
        Layout = tlCenter
      end
      object Label13: TLabel
        Left = 197
        Top = 143
        Width = 56
        Height = 13
        Caption = 'Undetected'
      end
      object Label14: TLabel
        Left = 29
        Top = 143
        Width = 66
        Height = 13
        Caption = 'Local factions'
      end
      object Label15: TLabel
        Left = 127
        Top = 99
        Width = 16
        Height = 13
        Caption = 'Ally'
      end
      object Label16: TLabel
        Left = 127
        Top = 121
        Width = 36
        Height = 13
        Caption = 'Friendly'
      end
      object Label17: TLabel
        Left = 197
        Top = 99
        Width = 47
        Height = 13
        Caption = 'Unfriendly'
      end
      object Label18: TLabel
        Left = 127
        Top = 143
        Width = 34
        Height = 13
        Caption = 'Neutral'
      end
      object Label19: TLabel
        Left = 197
        Top = 121
        Width = 32
        Height = 13
        Caption = 'Hostile'
      end
      object lbAlly: TLabel
        Left = 109
        Top = 97
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object lbFriend: TLabel
        Left = 109
        Top = 119
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object lbNeutral: TLabel
        Left = 109
        Top = 141
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object lbUndetect: TLabel
        Left = 178
        Top = 141
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object lbUnfrien: TLabel
        Left = 178
        Top = 97
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object lbHostile: TLabel
        Left = 178
        Top = 119
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object lbLocF: TLabel
        Left = 8
        Top = 141
        Width = 16
        Height = 16
        AutoSize = False
        Color = clBlack
        ParentColor = False
        OnDblClick = labColorDblClick
      end
      object cbColorUnit: TCheckBox
        Left = 8
        Top = 120
        Width = 97
        Height = 17
        Caption = 'Color units'
        TabOrder = 0
      end
    end
    object tsOptions: TTabSheet
      Caption = 'Options'
      ImageIndex = 1
      object Label5: TLabel
        Left = 8
        Top = 101
        Width = 129
        Height = 17
        AutoSize = False
        Caption = 'Town name limit (0 - none)'
        Layout = tlBottom
      end
      object Label10: TLabel
        Left = 9
        Top = 124
        Width = 104
        Height = 17
        AutoSize = False
        Caption = 'Backup orders every'
        Layout = tlBottom
      end
      object Label12: TLabel
        Left = 136
        Top = 124
        Width = 97
        Height = 17
        AutoSize = False
        Caption = '  min (0 - no backup)'
        Layout = tlBottom
      end
      object cbShowEvents: TCheckBox
        Left = 8
        Top = 4
        Width = 161
        Height = 17
        Caption = 'Show closed Events window'
        TabOrder = 0
      end
      object cbRunRegionOrders: TCheckBox
        Left = 8
        Top = 24
        Width = 249
        Height = 17
        Caption = 'Run Orders on change'
        TabOrder = 1
      end
      object edTownNameLimit: TEdit
        Left = 144
        Top = 101
        Width = 81
        Height = 21
        TabOrder = 5
      end
      object cbMaximizeWindow: TCheckBox
        Left = 8
        Top = 44
        Width = 137
        Height = 17
        Caption = 'Maximize Client on start'
        TabOrder = 2
      end
      object cbShowLDescFirst: TCheckBox
        Left = 8
        Top = 64
        Width = 153
        Height = 17
        Caption = 'Show Local Description first'
        TabOrder = 3
      end
      object cbCopyOnSaveOrders: TCheckBox
        Left = 8
        Top = 84
        Width = 225
        Height = 17
        Caption = 'Copy to Clipboard on Save Orders'
        TabOrder = 4
      end
      object edAutoSaveTimer: TEdit
        Left = 112
        Top = 124
        Width = 25
        Height = 21
        MaxLength = 2
        TabOrder = 6
      end
    end
    object tsEvents: TTabSheet
      Caption = 'Events'
      ImageIndex = 2
      object TLabel
        Left = 8
        Top = 8
        Width = 55
        Height = 13
        Caption = 'Don'#39't Show'
      end
      object cbdseRegDataFound: TCheckBox
        Left = 8
        Top = 60
        Width = 113
        Height = 17
        Caption = 'Region data found'
        TabOrder = 0
      end
      object cbdseIncome: TCheckBox
        Left = 8
        Top = 80
        Width = 129
        Height = 17
        Caption = 'Tax, Ent, Work Income'
        TabOrder = 1
      end
      object cbdseDiscard: TCheckBox
        Left = 8
        Top = 20
        Width = 97
        Height = 17
        Caption = 'Discard Warning'
        TabOrder = 2
      end
      object cbdseNoMonth: TCheckBox
        Left = 8
        Top = 40
        Width = 121
        Height = 17
        Caption = 'No Monthlong Error'
        TabOrder = 3
      end
      object cbdseCantReach: TCheckBox
        Left = 8
        Top = 100
        Width = 169
        Height = 17
        Caption = 'Move/Sail "can'#39't reach" Error'
        TabOrder = 4
      end
      object cbdseBorrows: TCheckBox
        Left = 144
        Top = 20
        Width = 105
        Height = 17
        Caption = 'Borrows in region'
        TabOrder = 5
      end
      object cbdseTeachNonLocNotStudy: TCheckBox
        Left = 8
        Top = 120
        Width = 209
        Height = 17
        Caption = 'Not Study Teach errors for nonlocals'
        TabOrder = 6
      end
    end
    object tsServer: TTabSheet
      Caption = 'Server'
      ImageIndex = 4
      object labFlightOverWater: TLabel
        Left = 8
        Top = 107
        Width = 89
        Height = 17
        AutoSize = False
        Caption = 'Flight Over Water'
        Layout = tlBottom
      end
      object cbNoCross: TCheckBox
        Left = 8
        Top = 8
        Width = 161
        Height = 17
        Caption = 'presents NOCROSS flag'
        TabOrder = 0
      end
      object cbNoSpoils: TCheckBox
        Left = 8
        Top = 24
        Width = 161
        Height = 17
        Caption = 'presents NOSPOILS flag'
        TabOrder = 1
      end
      object cbGiveAll: TCheckBox
        Left = 8
        Top = 40
        Width = 161
        Height = 17
        Caption = 'presents Give All Except order'
        TabOrder = 2
      end
      object cbSpoils: TCheckBox
        Left = 8
        Top = 56
        Width = 129
        Height = 17
        Caption = 'presents SPOILS order'
        TabOrder = 3
      end
      object cbServFlightOverWater: TComboBox
        Left = 96
        Top = 107
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 4
        Items.Strings = (
          'None'
          'Must Land'
          'Unlimited')
      end
      object cbTaxIsMonthlong: TCheckBox
        Left = 8
        Top = 89
        Width = 185
        Height = 17
        Caption = 'TAX, PILLAGE orders is monthlong'
        TabOrder = 5
      end
      object bnChangeGameParams: TButton
        Left = 8
        Top = 131
        Width = 121
        Height = 25
        Caption = 'Change Game Params'
        TabOrder = 6
        OnClick = bnChangeGameParamsClick
      end
      object cbSharing: TCheckBox
        Left = 8
        Top = 72
        Width = 133
        Height = 17
        Caption = 'presents SHARING flag'
        TabOrder = 7
      end
    end
    object tsServerMail: TTabSheet
      Caption = 'Server Mail'
      ImageIndex = 3
      object Label6: TLabel
        Left = 8
        Top = 8
        Width = 63
        Height = 17
        AutoSize = False
        Caption = 'Server E-Mail'
        Layout = tlBottom
      end
      object Label7: TLabel
        Left = 8
        Top = 32
        Width = 65
        Height = 17
        AutoSize = False
        Caption = 'Orders Subj'
        Layout = tlBottom
      end
      object Label8: TLabel
        Left = 8
        Top = 56
        Width = 71
        Height = 17
        AutoSize = False
        Caption = 'Times Request'
        Layout = tlBottom
      end
      object Label9: TLabel
        Left = 8
        Top = 80
        Width = 65
        Height = 17
        AutoSize = False
        Caption = 'Mail Subj'
        Layout = tlBottom
      end
      object edServEMail: TEdit
        Left = 80
        Top = 8
        Width = 121
        Height = 21
        TabOrder = 0
      end
      object cbNoServerMail: TCheckBox
        Left = 16
        Top = 104
        Width = 97
        Height = 17
        Caption = 'No Server Mail'
        TabOrder = 1
      end
      object edOrdersSubj: TEdit
        Left = 80
        Top = 32
        Width = 121
        Height = 21
        TabOrder = 2
      end
      object edTimesSubj: TEdit
        Left = 80
        Top = 56
        Width = 65
        Height = 21
        TabOrder = 3
      end
      object edTimesBody: TEdit
        Left = 152
        Top = 56
        Width = 97
        Height = 21
        TabOrder = 4
      end
      object edMailSubj: TEdit
        Left = 80
        Top = 80
        Width = 121
        Height = 21
        TabOrder = 5
      end
    end
  end
  object ColorDialog: TColorDialog
    Options = [cdFullOpen, cdSolidColor]
    Left = 88
    Top = 192
  end
end
