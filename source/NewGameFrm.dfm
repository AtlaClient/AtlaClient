object NewGameForm: TNewGameForm
  Left = 192
  Top = 107
  BorderStyle = bsToolWindow
  Caption = 'New Game'
  ClientHeight = 119
  ClientWidth = 136
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 0
    Width = 87
    Height = 13
    Caption = 'Enter Game Name'
  end
  object edName: TEdit
    Left = 8
    Top = 16
    Width = 121
    Height = 21
    TabOrder = 0
  end
  object cbCreateNew: TCheckBox
    Left = 8
    Top = 40
    Width = 129
    Height = 17
    Caption = 'Create New Database'
    TabOrder = 1
  end
  object bnOk: TButton
    Left = 8
    Top = 88
    Width = 57
    Height = 25
    Caption = 'Create'
    Default = True
    ModalResult = 1
    TabOrder = 2
    OnClick = bnOkClick
  end
  object Button1: TButton
    Left = 72
    Top = 88
    Width = 57
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
  object cbSaveOld: TCheckBox
    Left = 7
    Top = 64
    Width = 137
    Height = 17
    Caption = 'Save old game'
    Checked = True
    State = cbChecked
    TabOrder = 4
  end
end
