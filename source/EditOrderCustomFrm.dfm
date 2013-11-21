object EditOrderCustomForm: TEditOrderCustomForm
  Left = 192
  Top = 107
  BorderStyle = bsToolWindow
  Caption = 'Edit Custom Order'
  ClientHeight = 83
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
    83)
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
    Top = 54
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 88
    Top = 54
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
    OnClick = bnCancelClick
  end
  object edText: TEdit
    Left = 40
    Top = 8
    Width = 281
    Height = 21
    TabOrder = 0
  end
  object bnConvertComment: TButton
    Left = 168
    Top = 54
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'To Comment'
    TabOrder = 5
    OnClick = bnConvertCommentClick
  end
  object bnConvertOrder: TButton
    Left = 248
    Top = 54
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'To &Order'
    TabOrder = 6
    OnClick = bnConvertOrderClick
  end
  object cbCommented: TCheckBox
    Left = 186
    Top = 32
    Width = 97
    Height = 17
    Caption = '&Commented'
    TabOrder = 2
  end
  object cbRepeating: TCheckBox
    Left = 42
    Top = 32
    Width = 97
    Height = 17
    Caption = '&Repeating'
    TabOrder = 1
  end
end
