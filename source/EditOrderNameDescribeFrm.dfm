object EditOrderNameDescribeForm: TEditOrderNameDescribeForm
  Left = 252
  Top = 172
  BorderStyle = bsToolWindow
  Caption = 'Edit Name/Describe Order'
  ClientHeight = 184
  ClientWidth = 191
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object bnOk: TButton
    Left = 6
    Top = 154
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 110
    Top = 154
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
    OnClick = bnCancelClick
  end
  object cbCommented: TCheckBox
    Left = 7
    Top = 136
    Width = 79
    Height = 17
    Caption = '&Commented'
    TabOrder = 5
  end
  object edValue: TEdit
    Left = 8
    Top = 112
    Width = 161
    Height = 21
    TabOrder = 1
  end
  object RadioGroup: TRadioGroup
    Left = 8
    Top = 8
    Width = 177
    Height = 97
    Caption = 'Name                  Describe'
    Columns = 2
    Items.Strings = (
      '&Unit'
      '&Faction'
      '&Object'
      'C&ity'
      'U&nit'
      '&Ship'
      '&Building'
      'Ob&ject')
    TabOrder = 0
  end
  object bnGet: TButton
    Left = 168
    Top = 112
    Width = 22
    Height = 21
    Caption = '&Get'
    TabOrder = 2
    TabStop = False
    OnClick = bnGetClick
  end
end
