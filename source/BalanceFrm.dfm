object BalanceForm: TBalanceForm
  Left = 195
  Top = 107
  Caption = 'Balance'
  ClientHeight = 513
  ClientWidth = 774
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  WindowState = wsMaximized
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Grid: TDrawGrid
    Left = 0
    Top = 23
    Width = 774
    Height = 490
    Align = alClient
    ColCount = 6
    DefaultColWidth = 100
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goThumbTracking]
    TabOrder = 0
    OnDrawCell = GridDrawCell
  end
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 774
    Height = 23
    ButtonHeight = 21
    ButtonWidth = 60
    Caption = 'ToolBar'
    ShowCaptions = True
    TabOrder = 1
    object cbCurRegionOnly: TCheckBox
      Left = 0
      Top = 2
      Width = 121
      Height = 21
      Caption = 'Current &Region Only'
      TabOrder = 0
    end
    object Label2: TLabel
      Left = 121
      Top = 2
      Width = 47
      Height = 21
      AutoSize = False
      Caption = '  &Faction'
      FocusControl = cbFacDiap
      Layout = tlCenter
    end
    object cbFacDiap: TComboBox
      Left = 168
      Top = 2
      Width = 145
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      Sorted = True
      TabOrder = 1
      OnChange = cbFacDiapChange
    end
    object ToolButton3: TToolButton
      Left = 313
      Top = 0
      Width = 30
      Caption = 'ToolButton3'
      ImageIndex = 2
      Style = tbsSeparator
    end
    object bnPrevTurn: TToolButton
      Left = 343
      Top = 0
      AutoSize = True
      Caption = '&Prev Turn'
      ImageIndex = 0
      OnClick = bnPrevTurnClick
    end
    object bnNextTurn: TToolButton
      Left = 401
      Top = 0
      AutoSize = True
      Caption = '&Next Turn'
      ImageIndex = 1
      OnClick = bnNextTurnClick
    end
  end
end
