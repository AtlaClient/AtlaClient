object EditOrderBuySellForm: TEditOrderBuySellForm
  Left = 252
  Top = 172
  BorderStyle = bsToolWindow
  Caption = 'Edit BUYSELL Order'
  ClientHeight = 107
  ClientWidth = 232
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    232
    107)
  PixelsPerInch = 96
  TextHeight = 13
  object Label: TLabel
    Left = 8
    Top = 8
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'BuySell'
    Layout = tlBottom
  end
  object labPrice: TLabel
    Left = 128
    Top = 8
    Width = 105
    Height = 17
    AutoSize = False
    Caption = 'labPrice'
    Layout = tlBottom
  end
  object bnOk: TButton
    Left = 24
    Top = 78
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 5
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 128
    Top = 78
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 6
    OnClick = bnCancelClick
  end
  object cbCommented: TCheckBox
    Left = 97
    Top = 56
    Width = 79
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Commented'
    TabOrder = 3
  end
  object cbRepeating: TCheckBox
    Left = 25
    Top = 56
    Width = 71
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Repeating'
    TabOrder = 2
  end
  object cbItems: TComboBox
    Left = 40
    Top = 32
    Width = 185
    Height = 21
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 1
    OnChange = MoneyChange
  end
  object edCount: TEdit
    Left = 40
    Top = 8
    Width = 53
    Height = 21
    TabOrder = 0
    OnChange = MoneyChange
    OnExit = edCountExit
  end
  object bnAll: TButton
    Left = 96
    Top = 8
    Width = 22
    Height = 21
    Caption = '&All'
    TabOrder = 4
    TabStop = False
    OnClick = bnAllClick
  end
  object cbAll: TCheckBox
    Left = 184
    Top = 56
    Width = 49
    Height = 17
    Caption = 'A&ll'
    TabOrder = 7
    OnClick = cbAllClick
  end
end
