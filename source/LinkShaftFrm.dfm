object LinkShaftForm: TLinkShaftForm
  Left = 277
  Top = 190
  BorderStyle = bsSizeToolWin
  Caption = 'Shaft Link'
  ClientHeight = 201
  ClientWidth = 361
  Color = clBtnFace
  Constraints.MinHeight = 225
  Constraints.MinWidth = 184
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  DesignSize = (
    361
    201)
  PixelsPerInch = 96
  TextHeight = 13
  object List: TListBox
    Left = 8
    Top = 8
    Width = 346
    Height = 153
    Anchors = [akLeft, akTop, akRight, akBottom]
    ItemHeight = 13
    TabOrder = 0
  end
  object bnLink: TButton
    Left = 8
    Top = 168
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Link'
    Default = True
    TabOrder = 1
    OnClick = bnLinkClick
  end
  object bnCancel: TButton
    Left = 96
    Top = 168
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object bnAnalyze: TButton
    Left = 192
    Top = 168
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Analyze'
    TabOrder = 3
    OnClick = bnAnalyzeClick
  end
  object bnGoto: TButton
    Left = 280
    Top = 168
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Goto'
    TabOrder = 4
    OnClick = bnGotoClick
  end
end
