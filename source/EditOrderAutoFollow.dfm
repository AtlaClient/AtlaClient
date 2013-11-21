object EditOrderAutoFollowForm: TEditOrderAutoFollowForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Edit AutoFollow Order'
  ClientHeight = 93
  ClientWidth = 428
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 17
    Top = 14
    Width = 32
    Height = 13
    Caption = 'Target'
  end
  object bnOk: TButton
    Left = 124
    Top = 48
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 228
    Top = 48
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object cbTargets: TComboBox
    Left = 63
    Top = 10
    Width = 338
    Height = 21
    DropDownCount = 16
    ItemHeight = 13
    TabOrder = 2
  end
end
