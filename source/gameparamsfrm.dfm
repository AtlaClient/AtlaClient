object GameParamsForm: TGameParamsForm
  Left = 243
  Top = 191
  BorderStyle = bsDialog
  Caption = 'Change Game Params'
  ClientHeight = 308
  ClientWidth = 467
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object labComment: TLabel
    Left = 296
    Top = 8
    Width = 169
    Height = 33
    AutoSize = False
    Caption = 'labComment'
    WordWrap = True
  end
  object List: TListBox
    Left = 8
    Top = 8
    Width = 281
    Height = 297
    ItemHeight = 13
    TabOrder = 0
    OnClick = ListClick
  end
  object edValue: TEdit
    Left = 296
    Top = 48
    Width = 121
    Height = 21
    TabOrder = 1
    Text = 'edValue'
  end
  object bnSet: TButton
    Left = 296
    Top = 80
    Width = 121
    Height = 25
    Caption = 'Set'
    TabOrder = 2
    OnClick = bnSetClick
  end
end
