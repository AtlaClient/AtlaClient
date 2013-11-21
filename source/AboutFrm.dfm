object AboutForm: TAboutForm
  Left = 539
  Top = 219
  BorderStyle = bsToolWindow
  Caption = 'About'
  ClientHeight = 150
  ClientWidth = 225
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
  object labName: TLabel
    Left = 8
    Top = 8
    Width = 42
    Height = 13
    Caption = 'labName'
  end
  object Label2: TLabel
    Left = 8
    Top = 80
    Width = 59
    Height = 13
    Caption = 'Home Page:'
  end
  object labHomePage: TLabel
    Left = 80
    Top = 80
    Width = 130
    Height = 13
    Cursor = crHandPoint
    Caption = 'http://www.atlaclient.tu1.ru'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsUnderline]
    ParentFont = False
    OnClick = labHomePageClick
  end
  object Label3: TLabel
    Left = 8
    Top = 32
    Width = 114
    Height = 13
    Caption = 'Author: Evgeny Pashkin'
  end
  object labEMail: TLabel
    Left = 128
    Top = 31
    Width = 79
    Height = 13
    Cursor = crHandPoint
    Caption = 'eap@newmail.ru'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsUnderline]
    ParentFont = False
    OnClick = labEMailClick
  end
  object labRegCount: TLabel
    Left = 8
    Top = 104
    Width = 42
    Height = 13
    Caption = '              '
  end
  object Label1: TLabel
    Left = 8
    Top = 56
    Width = 114
    Height = 13
    Caption = 'Current support: Loudeo'
  end
  object Label4: TLabel
    Left = 132
    Top = 56
    Width = 76
    Height = 13
    Cursor = crHandPoint
    Caption = 'Loudeo@ngs.ru'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsUnderline]
    ParentFont = False
    OnClick = labEMailClick
  end
  object bnClose: TButton
    Left = 75
    Top = 120
    Width = 75
    Height = 25
    Caption = 'Ok'
    ModalResult = 2
    TabOrder = 0
  end
end
