object SetFlagsForm: TSetFlagsForm
  Left = 465
  Top = 313
  BorderStyle = bsToolWindow
  Caption = 'Set Unit Flags'
  ClientHeight = 207
  ClientWidth = 271
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object cbGuard: TCheckBox
    Left = 8
    Top = 8
    Width = 73
    Height = 17
    Caption = '&Guard'
    TabOrder = 0
    OnClick = cbGuardClick
  end
  object cbAutoTax: TCheckBox
    Left = 104
    Top = 8
    Width = 73
    Height = 17
    Caption = 'Auto&Tax'
    TabOrder = 1
  end
  object cbAvoid: TCheckBox
    Left = 8
    Top = 32
    Width = 73
    Height = 17
    Caption = '&Avoid'
    TabOrder = 2
    OnClick = cbAvoidClick
  end
  object cbHold: TCheckBox
    Left = 104
    Top = 32
    Width = 73
    Height = 17
    Caption = '&Hold'
    TabOrder = 3
  end
  object cbNoAid: TCheckBox
    Left = 104
    Top = 56
    Width = 73
    Height = 17
    Caption = '&NoAid'
    TabOrder = 4
  end
  object cbBehind: TCheckBox
    Left = 8
    Top = 56
    Width = 73
    Height = 17
    Caption = '&Behind'
    TabOrder = 5
  end
  object rgReveal: TRadioGroup
    Left = 8
    Top = 96
    Width = 73
    Height = 73
    Caption = '&Reveal'
    Items.Strings = (
      'None'
      'Unit'
      'Faction')
    TabOrder = 6
  end
  object rgConsume: TRadioGroup
    Left = 104
    Top = 96
    Width = 73
    Height = 73
    Caption = '&Consume'
    Items.Strings = (
      'None'
      'Unit'
      'Faction')
    TabOrder = 7
  end
  object bnOk: TButton
    Left = 46
    Top = 177
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 8
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 174
    Top = 177
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 9
  end
  object cbNoCross: TCheckBox
    Left = 8
    Top = 80
    Width = 89
    Height = 17
    Caption = '&NoCrossWater'
    TabOrder = 10
    Visible = False
  end
  object cbNoSpoils: TCheckBox
    Left = 103
    Top = 80
    Width = 89
    Height = 17
    Caption = '&NoBattleSpoils'
    TabOrder = 11
    Visible = False
  end
  object rgSpoils: TRadioGroup
    Left = 200
    Top = 64
    Width = 65
    Height = 105
    Caption = '&Spoils'
    TabOrder = 12
    Visible = False
  end
  object cbSharing: TCheckBox
    Left = 190
    Top = 8
    Width = 73
    Height = 17
    Caption = 'Sharing'
    TabOrder = 13
  end
end
