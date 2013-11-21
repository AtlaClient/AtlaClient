object EditOrderTeachForm: TEditOrderTeachForm
  Left = 214
  Top = 142
  BorderStyle = bsToolWindow
  Caption = 'Edit Teach Order'
  ClientHeight = 260
  ClientWidth = 413
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnDestroy = FormDestroy
  DesignSize = (
    413
    260)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 88
    Top = 192
    Width = 257
    Height = 13
    Alignment = taCenter
    AutoSize = False
  end
  object bnOk: TButton
    Left = 49
    Top = 231
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 265
    Top = 231
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
    OnClick = bnCancelClick
  end
  object cbCommented: TCheckBox
    Left = 250
    Top = 212
    Width = 97
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Commented'
    TabOrder = 3
  end
  object cbRepeating: TCheckBox
    Left = 50
    Top = 212
    Width = 97
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Repeating'
    TabOrder = 2
  end
  object lbTargets: TListBox
    Left = 8
    Top = 8
    Width = 177
    Height = 176
    Anchors = [akLeft, akTop, akBottom]
    ItemHeight = 13
    TabOrder = 4
    OnDblClick = bnDeleteClick
  end
  object lbUnits: TListBox
    Left = 232
    Top = 8
    Width = 169
    Height = 177
    ItemHeight = 13
    TabOrder = 5
    OnDblClick = bnAddClick
  end
  object bnDelete: TButton
    Left = 192
    Top = 48
    Width = 33
    Height = 33
    Caption = '>'
    TabOrder = 6
    OnClick = bnDeleteClick
  end
  object bnAdd: TButton
    Left = 192
    Top = 88
    Width = 33
    Height = 33
    Caption = '<'
    TabOrder = 7
    OnClick = bnAddClick
  end
end
