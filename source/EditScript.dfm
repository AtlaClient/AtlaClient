object fEditScript: TfEditScript
  Left = 0
  Top = 0
  ClientHeight = 378
  ClientWidth = 426
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
  object Memo1: TMemo
    Left = 0
    Top = 34
    Width = 426
    Height = 303
    Align = alClient
    Lines.Strings = (
      '')
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 337
    Width = 426
    Height = 41
    Align = alBottom
    TabOrder = 1
    object bnEdit: TButton
      Left = 116
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Ok'
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
    object Button1: TButton
      Left = 235
      Top = 8
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 426
    Height = 34
    Align = alTop
    TabOrder = 2
    object Label1: TLabel
      Left = 7
      Top = 10
      Width = 56
      Height = 13
      Caption = 'Script name'
    end
    object edName: TEdit
      Left = 69
      Top = 6
      Width = 188
      Height = 21
      TabOrder = 0
    end
    object cbClear: TCheckBox
      Left = 318
      Top = 8
      Width = 80
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Clear orders'
      TabOrder = 1
    end
  end
end
