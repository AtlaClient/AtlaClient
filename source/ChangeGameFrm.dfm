object ChangeGameForm: TChangeGameForm
  Left = 410
  Top = 154
  BorderStyle = bsToolWindow
  Caption = 'Select Game'
  ClientHeight = 206
  ClientWidth = 149
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object lbGames: TListBox
    Left = 4
    Top = 4
    Width = 137
    Height = 144
    ItemHeight = 13
    Sorted = True
    TabOrder = 0
    OnDblClick = lbGamesDblClick
  end
  object bnOk: TButton
    Left = 8
    Top = 176
    Width = 65
    Height = 25
    Caption = 'Change'
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 80
    Top = 176
    Width = 65
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object cbSaveOld: TCheckBox
    Left = 8
    Top = 152
    Width = 137
    Height = 17
    Caption = 'Save old game'
    Checked = True
    State = cbChecked
    TabOrder = 3
  end
end
