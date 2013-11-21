object EditOrderIntForm: TEditOrderIntForm
  Left = 192
  Top = 107
  BorderStyle = bsToolWindow
  Caption = 'Edit Int Order'
  ClientHeight = 83
  ClientWidth = 175
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  DesignSize = (
    175
    83)
  PixelsPerInch = 96
  TextHeight = 13
  object labText: TLabel
    Left = 8
    Top = 8
    Width = 81
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
    Left = 96
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
  object edValue: TEdit
    Left = 96
    Top = 8
    Width = 73
    Height = 21
    TabOrder = 0
  end
  object cbCommented: TCheckBox
    Left = 90
    Top = 32
    Width = 79
    Height = 17
    Caption = '&Commented'
    TabOrder = 2
  end
  object cbRepeating: TCheckBox
    Left = 10
    Top = 32
    Width = 71
    Height = 17
    Caption = '&Repeating'
    TabOrder = 1
  end
end
