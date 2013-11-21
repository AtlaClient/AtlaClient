object ReqForm: TReqForm
  Left = 318
  Top = 230
  ClientHeight = 110
  ClientWidth = 375
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 35
    Top = 12
    Width = 305
    Height = 53
    AutoSize = False
    WordWrap = True
  end
  object bt1: TButton
    Left = 10
    Top = 77
    Width = 116
    Height = 25
    Caption = 'Set '#39'weapon'#39' flag'
    ModalResult = 1
    TabOrder = 0
  end
  object bt2: TButton
    Left = 140
    Top = 77
    Width = 138
    Height = 25
    Caption = 'Remove from weapon list'
    ModalResult = 6
    TabOrder = 1
  end
  object btCancel: TButton
    Left = 290
    Top = 77
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Stop'
    ModalResult = 2
    TabOrder = 2
  end
end
