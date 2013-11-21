object EditOrderCastForm: TEditOrderCastForm
  Left = 252
  Top = 172
  BorderStyle = bsToolWindow
  Caption = 'Edit Cast Order'
  ClientHeight = 102
  ClientWidth = 213
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    213
    102)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 32
    Width = 38
    Height = 17
    AutoSize = False
    Caption = 'Params:'
    Layout = tlBottom
  end
  object bnOk: TButton
    Left = 14
    Top = 74
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 4
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 118
    Top = 74
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 5
    OnClick = bnCancelClick
  end
  object cbCommented: TCheckBox
    Left = 119
    Top = 56
    Width = 79
    Height = 17
    Caption = '&Commented'
    TabOrder = 3
  end
  object cbRepeating: TCheckBox
    Left = 15
    Top = 56
    Width = 71
    Height = 17
    Caption = '&Repeating'
    TabOrder = 2
  end
  object cbList: TComboBox
    Left = 8
    Top = 8
    Width = 201
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 0
    TabOrder = 0
  end
  object edParams: TEdit
    Left = 48
    Top = 32
    Width = 161
    Height = 21
    TabOrder = 1
  end
end
