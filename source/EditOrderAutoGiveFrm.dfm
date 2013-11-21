object EditOrderAutoGiveForm: TEditOrderAutoGiveForm
  Left = 323
  Top = 220
  BorderStyle = bsToolWindow
  Caption = 'Edit AutoGive Order'
  ClientHeight = 185
  ClientWidth = 246
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    246
    185)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 96
    Width = 41
    Height = 17
    AutoSize = False
    Caption = '&Begin str'
    FocusControl = edBegStr
    Layout = tlBottom
  end
  object Label2: TLabel
    Left = 8
    Top = 120
    Width = 41
    Height = 17
    AutoSize = False
    Caption = '&Sub str'
    FocusControl = edSubStr
    Layout = tlBottom
  end
  object Label3: TLabel
    Left = 8
    Top = 64
    Width = 49
    Height = 17
    AutoSize = False
    Caption = '&Item'
    FocusControl = cbItems
    Layout = tlBottom
  end
  object bnOk: TButton
    Left = 6
    Top = 154
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 7
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 94
    Top = 154
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 8
    OnClick = bnCancelClick
  end
  object cbItems: TComboBox
    Left = 64
    Top = 64
    Width = 175
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 0
    Sorted = True
    TabOrder = 4
  end
  object rbExcept: TRadioButton
    Left = 8
    Top = 8
    Width = 57
    Height = 17
    Caption = '&Except'
    TabOrder = 0
    OnClick = rbExceptClick
  end
  object rbAll: TRadioButton
    Left = 8
    Top = 24
    Width = 57
    Height = 17
    Caption = '&All'
    TabOrder = 1
    OnClick = rbAllClick
  end
  object rbNumber: TRadioButton
    Left = 8
    Top = 40
    Width = 57
    Height = 17
    Caption = '&Number'
    TabOrder = 2
    OnClick = rbNumberClick
  end
  object edNumber: TEdit
    Left = 88
    Top = 16
    Width = 121
    Height = 21
    TabOrder = 3
    Text = '0'
  end
  object edBegStr: TEdit
    Left = 64
    Top = 96
    Width = 177
    Height = 21
    TabOrder = 5
  end
  object edSubStr: TEdit
    Left = 64
    Top = 120
    Width = 177
    Height = 21
    TabOrder = 6
  end
end
