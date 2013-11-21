object fGetSkilLv: TfGetSkilLv
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = ' '
  ClientHeight = 65
  ClientWidth = 238
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
  object labNumber: TLabel
    Left = 139
    Top = 8
    Width = 37
    Height = 13
    AutoSize = False
    Caption = 'Level'
    Layout = tlBottom
  end
  object cbMans: TComboBox
    Left = 7
    Top = 4
    Width = 121
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
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
    TabOrder = 1
  end
  object btCan: TButton
    Left = 127
    Top = 32
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object edLev: TCSpinEdit
    Left = 178
    Top = 4
    Width = 39
    Height = 22
    MaxValue = 5
    MinValue = 1
    TabOrder = 3
    Value = 2
  end
end
