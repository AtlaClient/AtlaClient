object fGetItem: TfGetItem
  Left = 278
  Top = 299
  BorderIcons = []
  BorderStyle = bsDialog
  ClientHeight = 63
  ClientWidth = 233
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object labNumber: TLabel
    Left = 135
    Top = 8
    Width = 37
    Height = 13
    AutoSize = False
    Caption = 'Number'
    Layout = tlBottom
  end
  object cbInput: TComboBox
    Left = 7
    Top = 4
    Width = 121
    Height = 21
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 0
  end
  object btOk: TButton
    Left = 31
    Top = 32
    Width = 75
    Height = 25
    Caption = 'Add'
    Default = True
    ModalResult = 1
    TabOrder = 2
  end
  object btCan: TButton
    Left = 127
    Top = 32
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object edNum: TCSpinEdit
    Left = 178
    Top = 4
    Width = 39
    Height = 22
    TabOrder = 3
  end
end
