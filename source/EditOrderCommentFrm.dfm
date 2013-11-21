object EditOrderCommentForm: TEditOrderCommentForm
  Left = 295
  Top = 197
  BorderStyle = bsToolWindow
  Caption = 'Edit Commentary'
  ClientHeight = 88
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
    88)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 21
    Height = 17
    AutoSize = False
    Caption = 'Text'
    Layout = tlBottom
  end
  object bnOk: TButton
    Left = 8
    Top = 59
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
    Left = 88
    Top = 59
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
    OnClick = bnCancelClick
  end
  object edText: TEdit
    Left = 40
    Top = 8
    Width = 281
    Height = 21
    TabOrder = 0
  end
  object bnConvertCustom: TButton
    Left = 168
    Top = 59
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'To Custom'
    TabOrder = 4
    OnClick = bnConvertCustomClick
  end
  object bnConvertLocal: TButton
    Left = 248
    Top = 59
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'To Description'
    TabOrder = 5
    OnClick = bnConvertLocalClick
  end
  object cbRepeating: TCheckBox
    Left = 40
    Top = 32
    Width = 97
    Height = 17
    Caption = '&Repeating'
    TabOrder = 1
  end
end
