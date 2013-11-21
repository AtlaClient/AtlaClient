object ShowWeatherRegionsForm: TShowWeatherRegionsForm
  Left = 482
  Top = 306
  BorderStyle = bsDialog
  Caption = 'Weather Regions'
  ClientHeight = 96
  ClientWidth = 178
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesktopCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object labMapSize: TLabel
    Left = 64
    Top = 8
    Width = 44
    Height = 13
    Caption = 'Map Size'
  end
  object labX: TLabel
    Left = 80
    Top = 32
    Width = 17
    Height = 17
    AutoSize = False
    Caption = 'X'
    Layout = tlBottom
  end
  object edX: TEdit
    Left = 32
    Top = 32
    Width = 41
    Height = 21
    TabOrder = 0
  end
  object edY: TEdit
    Left = 96
    Top = 32
    Width = 41
    Height = 21
    TabOrder = 1
  end
  object bnOk: TButton
    Left = 8
    Top = 64
    Width = 75
    Height = 25
    Caption = 'Ok'
    Default = True
    TabOrder = 2
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 96
    Top = 64
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
end
