object ViewSpecialistsForm: TViewSpecialistsForm
  Left = 205
  Top = 107
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'View Specialists'
  ClientHeight = 513
  ClientWidth = 798
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  WindowState = wsMaximized
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter: TSplitter
    Left = 225
    Top = 21
    Height = 492
    ExplicitTop = 23
    ExplicitHeight = 490
  end
  object Tree: TTreeView
    Left = 0
    Top = 21
    Width = 225
    Height = 492
    Align = alLeft
    HideSelection = False
    Indent = 19
    ReadOnly = True
    TabOrder = 0
    OnChange = TreeChange
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 798
    Height = 21
    AutoSize = True
    ButtonHeight = 21
    ButtonWidth = 80
    Caption = 'ToolBar1'
    ShowCaptions = True
    TabOrder = 1
    object bnAllRegion: TToolButton
      Left = 0
      Top = 0
      Caption = 'All Regions'
      ImageIndex = 0
      Style = tbsCheck
      OnClick = bnAllRegionClick
    end
    object ToolButton1: TToolButton
      Left = 80
      Top = 0
      Width = 8
      Caption = 'ToolButton1'
      ImageIndex = 2
      Style = tbsSeparator
    end
    object bnNoMagic: TToolButton
      Left = 88
      Top = 0
      Caption = 'No Magic Skills'
      ImageIndex = 1
      Style = tbsCheck
      OnClick = bnNoMagicClick
    end
    object ToolButton2: TToolButton
      Left = 168
      Top = 0
      Width = 8
      Caption = 'ToolButton2'
      ImageIndex = 2
      Style = tbsSeparator
    end
    object bnShortMode: TToolButton
      Left = 176
      Top = 0
      Caption = 'Short Mode'
      ImageIndex = 2
      Style = tbsCheck
      OnClick = bnShortModeClick
    end
    object ToolButton3: TToolButton
      Left = 256
      Top = 0
      Width = 8
      Caption = 'ToolButton3'
      ImageIndex = 3
      Style = tbsSeparator
    end
    object bnSortMode: TToolButton
      Left = 264
      Top = 0
      Caption = 'Sort by Mens'
      ImageIndex = 3
      Style = tbsCheck
      OnClick = bnSortModeClick
    end
  end
  object List: TListBox
    Left = 228
    Top = 21
    Width = 570
    Height = 492
    Hint = 'DblClick to goto'
    Align = alClient
    ItemHeight = 13
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    OnDblClick = ListDblClick
  end
end
