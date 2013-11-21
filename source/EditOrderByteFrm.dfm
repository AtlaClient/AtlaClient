object EditOrderByteForm: TEditOrderByteForm
  Left = 214
  Top = 142
  BorderStyle = bsToolWindow
  Caption = 'Edit Byte Order'
  ClientHeight = 103
  ClientWidth = 325
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  DesignSize = (
    325
    103)
  PixelsPerInch = 96
  TextHeight = 13
  object labInfo: TLabel
    Left = 168
    Top = 8
    Width = 153
    Height = 41
    Alignment = taCenter
    Anchors = [akLeft, akTop, akRight, akBottom]
    AutoSize = False
    Layout = tlCenter
    WordWrap = True
  end
  object bnOk: TButton
    Left = 49
    Top = 74
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
    Top = 74
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
    Left = 194
    Top = 56
    Width = 97
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Commented'
    TabOrder = 1
  end
  object cbRepeating: TCheckBox
    Left = 50
    Top = 56
    Width = 97
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Repeating'
    TabOrder = 0
  end
end
