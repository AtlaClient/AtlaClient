object fFlagFiltr: TfFlagFiltr
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Flags Filter'
  ClientHeight = 159
  ClientWidth = 432
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 432
    Height = 125
    Align = alClient
    TabOrder = 0
    ExplicitLeft = 40
    ExplicitTop = 120
    ExplicitWidth = 955
    ExplicitHeight = 225
    object GroupBox1: TGroupBox
      Left = 1
      Top = 1
      Width = 128
      Height = 123
      Align = alLeft
      TabOrder = 0
      ExplicitHeight = 223
      object Label1: TLabel
        Left = 8
        Top = 16
        Width = 29
        Height = 13
        Caption = 'Guard'
      end
      object Label2: TLabel
        Left = 8
        Top = 42
        Width = 27
        Height = 13
        Caption = 'Avoid'
      end
      object Label3: TLabel
        Left = 8
        Top = 94
        Width = 33
        Height = 13
        Caption = 'Reveal'
      end
      object Label4: TLabel
        Left = 8
        Top = 68
        Width = 32
        Height = 13
        Caption = 'Behind'
      end
      object cbGuard: TComboBox
        Left = 49
        Top = 12
        Width = 71
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 0
        Text = '*'
        Items.Strings = (
          '*'
          '0'
          '1')
      end
      object cbAvoid: TComboBox
        Left = 49
        Top = 38
        Width = 71
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 1
        Text = '*'
        Items.Strings = (
          '*'
          '0'
          '1')
      end
      object cbReveal: TComboBox
        Left = 49
        Top = 90
        Width = 71
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 2
        Text = '*'
        Items.Strings = (
          '*'
          'None'
          'Unit'
          'Faction')
      end
      object cbBehind: TComboBox
        Left = 49
        Top = 64
        Width = 71
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 3
        Text = '*'
        Items.Strings = (
          '*'
          '0'
          '1')
      end
    end
    object GroupBox2: TGroupBox
      Left = 129
      Top = 1
      Width = 137
      Height = 123
      Align = alLeft
      TabOrder = 1
      ExplicitHeight = 223
      object Label5: TLabel
        Left = 8
        Top = 16
        Width = 41
        Height = 13
        Caption = 'AutoTax'
      end
      object Label6: TLabel
        Left = 8
        Top = 42
        Width = 21
        Height = 13
        Caption = 'Hold'
      end
      object Label7: TLabel
        Left = 8
        Top = 94
        Width = 44
        Height = 13
        Caption = 'Consume'
      end
      object Label8: TLabel
        Left = 8
        Top = 68
        Width = 28
        Height = 13
        Caption = 'NoAid'
      end
      object cbAutoTax: TComboBox
        Left = 58
        Top = 12
        Width = 71
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 0
        Text = '*'
        Items.Strings = (
          '*'
          '0'
          '1')
      end
      object cbHold: TComboBox
        Left = 58
        Top = 38
        Width = 71
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 1
        Text = '*'
        Items.Strings = (
          '*'
          '0'
          '1')
      end
      object cbConsume: TComboBox
        Left = 58
        Top = 90
        Width = 71
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 2
        Text = '*'
        Items.Strings = (
          '*'
          'None'
          'Unit'
          'Faction')
      end
      object cbNoaid: TComboBox
        Left = 58
        Top = 64
        Width = 71
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 3
        Text = '*'
        Items.Strings = (
          '*'
          '0'
          '1')
      end
    end
    object GroupBox3: TGroupBox
      Left = 266
      Top = 1
      Width = 165
      Height = 123
      Align = alClient
      TabOrder = 2
      ExplicitLeft = 456
      ExplicitTop = 24
      ExplicitWidth = 185
      ExplicitHeight = 105
      object lbNoCross: TLabel
        Left = 8
        Top = 42
        Width = 70
        Height = 13
        Caption = 'NoCrossWater'
      end
      object lbNoSpoils: TLabel
        Left = 9
        Top = 94
        Width = 68
        Height = 13
        Caption = 'NoBattleSpoils'
        Visible = False
      end
      object lbSpoils: TLabel
        Left = 8
        Top = 15
        Width = 27
        Height = 13
        Caption = 'Spoils'
        Visible = False
      end
      object lbSharing: TLabel
        Left = 8
        Top = 68
        Width = 36
        Height = 13
        Caption = 'Sharing'
      end
      object cbNoCross: TComboBox
        Left = 82
        Top = 38
        Width = 71
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 0
        Text = '*'
        Items.Strings = (
          '*'
          '0'
          '1')
      end
      object cbNoSpoils: TComboBox
        Left = 83
        Top = 90
        Width = 71
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 1
        Text = '*'
        Visible = False
        Items.Strings = (
          '*'
          '0'
          '1')
      end
      object cbSpoils: TComboBox
        Left = 82
        Top = 11
        Width = 71
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 2
        Visible = False
      end
      object cbSharing: TComboBox
        Left = 83
        Top = 64
        Width = 71
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 3
        Text = '*'
        Items.Strings = (
          '*'
          '0'
          '1')
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 125
    Width = 432
    Height = 34
    Align = alBottom
    TabOrder = 1
    ExplicitTop = 132
    ExplicitWidth = 430
    object bnCancel: TButton
      Left = 241
      Top = 5
      Width = 75
      Height = 24
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 0
    end
    object bnOk: TButton
      Left = 113
      Top = 5
      Width = 75
      Height = 24
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 1
    end
  end
end
