object EditOrderWithdrawForm: TEditOrderWithdrawForm
  Left = 323
  Top = 220
  BorderStyle = bsToolWindow
  Caption = 'Edit Withdraw Order'
  ClientHeight = 103
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
    103)
  PixelsPerInch = 96
  TextHeight = 13
  object labUnclaimed: TLabel
    Left = 88
    Top = 32
    Width = 64
    Height = 13
    Caption = 'labUnclaimed'
  end
  object labAttempted: TLabel
    Left = 8
    Top = 32
    Width = 62
    Height = 13
    Caption = 'labAttempted'
  end
  object bnOk: TButton
    Left = 6
    Top = 74
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 2
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 94
    Top = 74
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
    OnClick = bnCancelClick
  end
  object cbItems: TComboBox
    Left = 64
    Top = 8
    Width = 175
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 0
    Sorted = True
    TabOrder = 1
    OnChange = edNumberChange
  end
  object edNumber: TEdit
    Left = 8
    Top = 8
    Width = 49
    Height = 21
    TabOrder = 0
    Text = '0'
    OnChange = edNumberChange
  end
  object cbRepeating: TCheckBox
    Left = 10
    Top = 54
    Width = 97
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Repeating'
    TabOrder = 4
  end
  object cbCommented: TCheckBox
    Left = 141
    Top = 54
    Width = 97
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Commented'
    TabOrder = 5
  end
end
