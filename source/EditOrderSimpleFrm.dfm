object EditOrderSimpleForm: TEditOrderSimpleForm
  Left = 192
  Top = 107
  BorderStyle = bsToolWindow
  Caption = 'Edit Simple Order'
  ClientHeight = 83
  ClientWidth = 325
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    325
    83)
  PixelsPerInch = 96
  TextHeight = 13
  object labInfo: TLabel
    Left = 8
    Top = 8
    Width = 313
    Height = 17
    Alignment = taCenter
    AutoSize = False
    Layout = tlBottom
  end
  object bnOk: TButton
    Left = 49
    Top = 54
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 2
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 201
    Top = 54
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
    OnClick = bnCancelClick
  end
  object cbCommented: TCheckBox
    Left = 186
    Top = 32
    Width = 97
    Height = 17
    Caption = '&Commented'
    TabOrder = 1
  end
  object cbRepeating: TCheckBox
    Left = 42
    Top = 32
    Width = 97
    Height = 17
    Caption = '&Repeating'
    TabOrder = 0
  end
end
