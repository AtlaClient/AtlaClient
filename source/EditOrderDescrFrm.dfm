object EditOrderDescrForm: TEditOrderDescrForm
  Left = 295
  Top = 197
  BorderStyle = bsToolWindow
  Caption = 'Edit Local Description'
  ClientHeight = 64
  ClientWidth = 244
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  DesignSize = (
    244
    64)
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
    Top = 35
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 88
    Top = 35
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
    OnClick = bnCancelClick
  end
  object edText: TEdit
    Left = 40
    Top = 8
    Width = 201
    Height = 21
    TabOrder = 0
  end
  object bnConvertComment: TButton
    Left = 168
    Top = 35
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'To Comment'
    TabOrder = 3
    OnClick = bnConvertCommentClick
  end
end
