object fSelEditScr: TfSelEditScr
  Left = 0
  Top = 0
  Caption = 'Scripts'
  ClientHeight = 293
  ClientWidth = 274
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 252
    Width = 274
    Height = 41
    Align = alBottom
    TabOrder = 0
    object bnEdit: TButton
      Left = 17
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Edit'
      Default = True
      TabOrder = 0
      OnClick = bnEditClick
    end
    object Button1: TButton
      Left = 100
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Add'
      Default = True
      TabOrder = 1
      OnClick = Button1Click
    end
    object btDel: TButton
      Left = 181
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Delete'
      Default = True
      TabOrder = 2
      OnClick = btDelClick
    end
  end
  object List: TListBox
    Left = 0
    Top = 0
    Width = 274
    Height = 252
    Align = alClient
    ItemHeight = 13
    TabOrder = 1
  end
end
