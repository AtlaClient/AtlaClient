object SaveMapForm: TSaveMapForm
  Left = 288
  Top = 465
  BorderStyle = bsDialog
  Caption = 'Save Map'
  ClientHeight = 134
  ClientWidth = 268
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object bnOk: TButton
    Left = 8
    Top = 104
    Width = 75
    Height = 25
    Caption = 'Ok'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = bnApplyClick
  end
  object bnCancel: TButton
    Left = 192
    Top = 104
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object cbAllRegion: TCheckBox
    Left = 8
    Top = 8
    Width = 97
    Height = 17
    Caption = 'All Regions'
    TabOrder = 2
  end
  object cbRegionInfo: TCheckBox
    Left = 8
    Top = 32
    Width = 97
    Height = 17
    Caption = 'Region Info'
    TabOrder = 3
  end
  object cbUserObject: TCheckBox
    Left = 8
    Top = 56
    Width = 97
    Height = 17
    Caption = 'User Objects'
    TabOrder = 4
  end
  object cbNonuserObject: TCheckBox
    Left = 8
    Top = 80
    Width = 97
    Height = 17
    Caption = 'Nonuser Objects'
    TabOrder = 5
  end
  object cbLocalUnit: TCheckBox
    Left = 128
    Top = 8
    Width = 97
    Height = 17
    Caption = 'Local Units'
    TabOrder = 6
    OnClick = cbLocalUnitClick
  end
  object cbLocalAsNonlocalUnit: TCheckBox
    Left = 128
    Top = 32
    Width = 137
    Height = 17
    Caption = 'Local As Non local Units'
    TabOrder = 7
  end
  object cbNonlocalUnit: TCheckBox
    Left = 128
    Top = 56
    Width = 97
    Height = 17
    Caption = 'Non local Units'
    TabOrder = 8
  end
  object cbAdvProducts: TCheckBox
    Left = 128
    Top = 80
    Width = 121
    Height = 17
    Caption = 'Advanced Products'
    TabOrder = 9
  end
  object bnApply: TButton
    Left = 96
    Top = 104
    Width = 75
    Height = 25
    Caption = 'Save Opts'
    TabOrder = 10
    OnClick = bnApplyClick
  end
end
