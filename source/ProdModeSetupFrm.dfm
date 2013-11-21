object ProdModeSetupForm: TProdModeSetupForm
  Left = 400
  Top = 282
  BorderStyle = bsDialog
  Caption = 'Products Mode Settings'
  ClientHeight = 122
  ClientWidth = 165
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object cbFood: TCheckBox
    Left = 8
    Top = 8
    Width = 97
    Height = 17
    Caption = '&Food items'
    TabOrder = 0
  end
  object cbNormal: TCheckBox
    Left = 8
    Top = 32
    Width = 97
    Height = 17
    Caption = '&Normal items'
    TabOrder = 1
  end
  object cbAdvanced: TCheckBox
    Left = 8
    Top = 56
    Width = 97
    Height = 17
    Caption = '&Advanced items'
    TabOrder = 2
  end
  object cbNull: TCheckBox
    Left = 8
    Top = 80
    Width = 97
    Height = 17
    Caption = 'No present items'
    TabOrder = 3
  end
  object bnOk: TButton
    Left = 112
    Top = 8
    Width = 49
    Height = 25
    Caption = 'Ok'
    Default = True
    ModalResult = 1
    TabOrder = 4
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 112
    Top = 40
    Width = 49
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 5
  end
  object cbRace: TCheckBox
    Left = 8
    Top = 104
    Width = 97
    Height = 17
    Caption = 'Race'
    TabOrder = 6
  end
end
