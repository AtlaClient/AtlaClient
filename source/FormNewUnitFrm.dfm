object FormNewUnitForm: TFormNewUnitForm
  Left = 255
  Top = 69
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Form new unit'
  ClientHeight = 424
  ClientWidth = 475
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pn2: TPanel
    Left = 297
    Top = 106
    Width = 178
    Height = 277
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 1
    object rgSpoils: TRadioGroup
      Left = 79
      Top = 73
      Width = 98
      Height = 105
      Caption = ' &Spoils '
      TabOrder = 2
    end
    object rgConsume: TRadioGroup
      Left = 88
      Top = 0
      Width = 89
      Height = 69
      Caption = ' &Consume '
      ItemIndex = 0
      Items.Strings = (
        'None'
        'Unit'
        'Faction')
      TabOrder = 1
    end
    object rgReveal: TRadioGroup
      Left = 0
      Top = 0
      Width = 81
      Height = 69
      Caption = ' &Reveal '
      ItemIndex = 0
      Items.Strings = (
        'None'
        'Unit'
        'Faction')
      TabOrder = 0
    end
    object gbC: TGroupBox
      Left = 0
      Top = 243
      Width = 178
      Height = 34
      Align = alBottom
      TabOrder = 3
      object Label7: TLabel
        Left = 8
        Top = 12
        Width = 31
        Height = 13
        Caption = 'Create'
      end
      object Label9: TLabel
        Left = 96
        Top = 12
        Width = 31
        Height = 13
        Caption = 'copies'
      end
      object edCopy: TCSpinEdit
        Left = 52
        Top = 9
        Width = 42
        Height = 22
        MaxValue = 100
        MinValue = 1
        TabOrder = 0
        Value = 1
      end
    end
  end
  object pn1: TPanel
    Left = 0
    Top = 106
    Width = 297
    Height = 277
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object gb2: TGroupBox
      Left = 0
      Top = 0
      Width = 297
      Height = 58
      Align = alTop
      Caption = ' Buy '
      TabOrder = 0
      object bnAllB: TButton
        Left = 239
        Top = 14
        Width = 22
        Height = 21
        Caption = '&All'
        TabOrder = 2
        TabStop = False
        OnClick = bnAllBClick
      end
      object cbRepB: TCheckBox
        Left = 46
        Top = 37
        Width = 71
        Height = 17
        Caption = '&Repeating'
        TabOrder = 3
      end
      object cbCommB: TCheckBox
        Left = 118
        Top = 37
        Width = 79
        Height = 17
        Caption = '&Commented'
        TabOrder = 4
      end
      object cbAllB: TCheckBox
        Left = 205
        Top = 38
        Width = 38
        Height = 17
        Caption = 'A&ll'
        TabOrder = 5
        OnClick = cbAllBClick
      end
      object cbItemsB: TComboBox
        Left = 58
        Top = 14
        Width = 177
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 1
        OnChange = ChangeInfo
      end
      object edCountB: TCSpinEdit
        Left = 9
        Top = 15
        Width = 39
        Height = 22
        MaxValue = 20000
        TabOrder = 0
        OnChange = ChangeInfo
      end
    end
    object gb3: TGroupBox
      Left = 0
      Top = 58
      Width = 297
      Height = 63
      Align = alTop
      Caption = ' Study '
      TabOrder = 1
      DesignSize = (
        297
        63)
      object Label6: TLabel
        Left = 138
        Top = 41
        Width = 34
        Height = 13
        Caption = 'months'
      end
      object cbListS: TComboBox
        Left = 24
        Top = 14
        Width = 268
        Height = 21
        Style = csDropDownList
        Anchors = [akLeft, akTop, akRight]
        ItemHeight = 13
        TabOrder = 1
        OnChange = cbListSChange
      end
      object cbRepS: TCheckBox
        Left = 24
        Top = 39
        Width = 71
        Height = 17
        Caption = '&Repeating'
        TabOrder = 2
      end
      object cbCommS: TCheckBox
        Left = 182
        Top = 39
        Width = 79
        Height = 17
        Caption = '&Commented'
        TabOrder = 4
      end
      object cbStudy: TCheckBox
        Left = 6
        Top = 16
        Width = 15
        Height = 17
        TabOrder = 0
        OnClick = ChangeInfo
      end
      object edMonS: TCSpinEdit
        Left = 96
        Top = 38
        Width = 39
        Height = 22
        MaxValue = 20
        MinValue = 1
        TabOrder = 3
        Value = 1
        OnChange = ChangeInfo
      end
    end
    object gb4: TGroupBox
      Left = 0
      Top = 121
      Width = 297
      Height = 64
      Align = alTop
      Caption = ' Get Silver '
      TabOrder = 2
      DesignSize = (
        297
        64)
      object Label5: TLabel
        Left = 8
        Top = 18
        Width = 40
        Height = 13
        Caption = 'Receive'
      end
      object btAllNeedS: TSpeedButton
        Left = 132
        Top = 13
        Width = 54
        Height = 22
        AllowAllUp = True
        GroupIndex = 1
        Caption = 'All &needs'
        OnClick = btAllNeedSClick
      end
      object cbTargetsU: TComboBox
        Left = 5
        Top = 38
        Width = 288
        Height = 21
        Style = csDropDownList
        Anchors = [akLeft, akTop, akRight]
        DropDownCount = 16
        ItemHeight = 13
        TabOrder = 3
        OnChange = cbTargetsUChange
      end
      object btAllS: TButton
        Left = 104
        Top = 14
        Width = 22
        Height = 21
        Caption = '&All'
        TabOrder = 1
        TabStop = False
        OnClick = btAllSClick
      end
      object cbRepR: TCheckBox
        Left = 190
        Top = 16
        Width = 71
        Height = 16
        Caption = '&Repeating'
        TabOrder = 2
      end
      object edReceive: TCSpinEdit
        Left = 54
        Top = 13
        Width = 42
        Height = 22
        MaxValue = 100000000
        TabOrder = 0
      end
    end
    object gb6: TGroupBox
      Left = 0
      Top = 185
      Width = 297
      Height = 92
      Align = alClient
      Caption = ' Need silver '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      object lbNeedB: TLabel
        Left = 191
        Top = 33
        Width = 6
        Height = 13
        Caption = '0'
      end
      object lbNeedS: TLabel
        Left = 191
        Top = 51
        Width = 6
        Height = 13
        Caption = '0'
      end
      object lbM: TLabel
        Left = 8
        Top = 14
        Width = 77
        Height = 13
        Caption = 'Maintenance for'
      end
      object Label8: TLabel
        Left = 145
        Top = 14
        Width = 34
        Height = 13
        Caption = 'months'
      end
      object lbNeedM: TLabel
        Left = 191
        Top = 15
        Width = 6
        Height = 13
        Caption = '0'
      end
      object Label10: TLabel
        Left = 133
        Top = 33
        Width = 46
        Height = 13
        Caption = 'for buying'
      end
      object Label11: TLabel
        Left = 130
        Top = 51
        Width = 49
        Height = 13
        Caption = 'for studing'
      end
      object Bevel1: TBevel
        Left = 123
        Top = 68
        Width = 129
        Height = 2
      end
      object lbNeedAll: TLabel
        Left = 191
        Top = 74
        Width = 6
        Height = 13
        Caption = '0'
      end
      object edMonth: TCSpinEdit
        Left = 95
        Top = 10
        Width = 39
        Height = 22
        TabOrder = 0
        OnChange = ChangeInfo
      end
    end
  end
  object pn3: TPanel
    Left = 0
    Top = 383
    Width = 475
    Height = 41
    Align = alBottom
    TabOrder = 2
    object bnCancel: TButton
      Left = 264
      Top = 8
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
    end
    object bnOk: TButton
      Left = 136
      Top = 8
      Width = 75
      Height = 25
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
      OnClick = bnOkClick
    end
  end
  object p0: TPanel
    Left = 0
    Top = 0
    Width = 475
    Height = 106
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 3
    object gb1: TGroupBox
      Left = 0
      Top = 0
      Width = 268
      Height = 106
      Align = alLeft
      TabOrder = 0
      object Label4: TLabel
        Left = 18
        Top = 83
        Width = 53
        Height = 13
        Caption = 'Description'
        Layout = tlBottom
      end
      object Label3: TLabel
        Left = 13
        Top = 60
        Width = 58
        Height = 13
        Caption = 'Local descr.'
        Layout = tlBottom
      end
      object Label2: TLabel
        Left = 43
        Top = 37
        Width = 28
        Height = 13
        Caption = 'Name'
        Layout = tlBottom
      end
      object Label1: TLabel
        Left = 5
        Top = 14
        Width = 66
        Height = 13
        Caption = 'Alias (number)'
        Layout = tlBottom
      end
      object edDescr: TEdit
        Left = 78
        Top = 79
        Width = 185
        Height = 21
        TabOrder = 3
      end
      object edLocalDescr: TEdit
        Left = 78
        Top = 56
        Width = 185
        Height = 21
        TabOrder = 2
      end
      object edName: TEdit
        Left = 78
        Top = 33
        Width = 185
        Height = 21
        TabOrder = 1
      end
      object edAlias: TEdit
        Left = 78
        Top = 10
        Width = 65
        Height = 21
        TabOrder = 0
        OnKeyPress = edAliasKeyPress
      end
    end
    object gb5: TGroupBox
      Left = 268
      Top = 0
      Width = 207
      Height = 106
      Align = alClient
      Caption = ' Flags '
      TabOrder = 1
      object cbGuard: TCheckBox
        Left = 8
        Top = 14
        Width = 73
        Height = 17
        Caption = '&Guard'
        TabOrder = 0
        OnClick = cbGuardClick
      end
      object cbAvoid: TCheckBox
        Left = 8
        Top = 31
        Width = 73
        Height = 17
        Caption = '&Avoid'
        TabOrder = 1
        OnClick = cbAvoidClick
      end
      object cbBehind: TCheckBox
        Left = 8
        Top = 48
        Width = 73
        Height = 17
        Caption = '&Behind'
        TabOrder = 2
      end
      object cbNoCross: TCheckBox
        Left = 8
        Top = 65
        Width = 89
        Height = 17
        Caption = '&NoCrossWater'
        TabOrder = 3
      end
      object cbNoSpoils: TCheckBox
        Left = 104
        Top = 65
        Width = 89
        Height = 17
        Caption = '&NoBattleSpoils'
        TabOrder = 8
      end
      object cbNoAid: TCheckBox
        Left = 104
        Top = 48
        Width = 73
        Height = 17
        Caption = '&NoAid'
        TabOrder = 7
      end
      object cbHold: TCheckBox
        Left = 104
        Top = 31
        Width = 73
        Height = 17
        Caption = '&Hold'
        TabOrder = 6
      end
      object cbAutoTax: TCheckBox
        Left = 104
        Top = 14
        Width = 73
        Height = 17
        Caption = 'Auto&Tax'
        TabOrder = 5
      end
      object cbSharing: TCheckBox
        Left = 8
        Top = 82
        Width = 73
        Height = 17
        Caption = 'Sharing'
        TabOrder = 4
      end
    end
  end
end
