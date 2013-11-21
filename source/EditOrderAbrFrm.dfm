object EditOrderAbrForm: TEditOrderAbrForm
  Left = 252
  Top = 172
  BorderStyle = bsToolWindow
  Caption = 'Edit Abr Order'
  ClientHeight = 81
  ClientWidth = 460
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  DesignSize = (
    460
    81)
  PixelsPerInch = 96
  TextHeight = 13
  object bnOk: TButton
    Left = 14
    Top = 50
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 118
    Top = 50
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
    OnClick = bnCancelClick
  end
  object cbCommented: TCheckBox
    Left = 119
    Top = 32
    Width = 79
    Height = 17
    Caption = '&Commented'
    TabOrder = 2
  end
  object cbRepeating: TCheckBox
    Left = 15
    Top = 32
    Width = 71
    Height = 17
    Caption = '&Repeating'
    TabOrder = 1
  end
  object cbList: TComboBox
    Left = 8
    Top = 8
    Width = 445
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 0
    TabOrder = 0
  end
end
