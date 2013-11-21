object StoreForm: TStoreForm
  Left = 299
  Top = 198
  BorderStyle = bsToolWindow
  Caption = 'Store Turns'
  ClientHeight = 130
  ClientWidth = 189
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object bnOk: TButton
    Left = 8
    Top = 104
    Width = 75
    Height = 25
    Caption = 'Ok'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 96
    Top = 104
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object edBeginTurn: TEdit
    Left = 128
    Top = 8
    Width = 57
    Height = 21
    TabOrder = 2
  end
  object rbFirstTurn: TRadioButton
    Left = 8
    Top = 8
    Width = 113
    Height = 25
    Caption = 'First Turn'
    TabOrder = 3
    OnClick = rbClick
  end
  object rbLastTurn: TRadioButton
    Left = 8
    Top = 48
    Width = 113
    Height = 17
    Caption = 'Last N Turns'
    TabOrder = 4
    OnClick = rbClick
  end
  object edAutoBeginTurn: TEdit
    Left = 128
    Top = 48
    Width = 57
    Height = 21
    TabOrder = 5
  end
end
