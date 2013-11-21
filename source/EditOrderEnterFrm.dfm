object EditOrderEnterForm: TEditOrderEnterForm
  Left = 252
  Top = 172
  BorderStyle = bsSizeToolWin
  Caption = 'Edit ENTER Order'
  ClientHeight = 303
  ClientWidth = 630
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    630
    303)
  PixelsPerInch = 96
  TextHeight = 13
  object labUnit: TLabel
    Left = 8
    Top = 8
    Width = 33
    Height = 13
    Caption = 'labUnit'
  end
  object bnOk: TButton
    Left = 24
    Top = 271
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 3
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 128
    Top = 271
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
    OnClick = bnCancelClick
  end
  object cbCommented: TCheckBox
    Left = 129
    Top = 249
    Width = 79
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Commented'
    TabOrder = 2
  end
  object cbRepeating: TCheckBox
    Left = 25
    Top = 249
    Width = 71
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Repeating'
    TabOrder = 1
  end
  object Grid: TStringGrid
    Left = 8
    Top = 24
    Width = 616
    Height = 217
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 6
    FixedCols = 0
    RowCount = 10
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect]
    TabOrder = 0
    OnDblClick = bnOkClick
    OnDrawCell = GridDrawCell
    ColWidths = (
      170
      40
      100
      100
      90
      90)
  end
end
