object ShortCutsForm: TShortCutsForm
  Left = 243
  Top = 107
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'ShortCuts'
  ClientHeight = 357
  ClientWidth = 342
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object panRight: TPanel
    Left = 208
    Top = 0
    Width = 134
    Height = 240
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 0
    object labCaption: TLabel
      Left = 16
      Top = 16
      Width = 15
      Height = 13
      Caption = '     '
    end
    object HotKey: THotKey
      Left = 16
      Top = 40
      Width = 113
      Height = 19
      Hint = 'Del or BackSpace to clear'
      HotKey = 0
      Modifiers = []
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
    end
    object bnSet: TButton
      Left = 16
      Top = 64
      Width = 75
      Height = 25
      Caption = 'Set'
      TabOrder = 1
      OnClick = bnSetClick
    end
    object bnClearShortCuts: TButton
      Left = 16
      Top = 96
      Width = 113
      Height = 25
      Caption = 'Clear same ShortCuts'
      TabOrder = 2
      OnClick = bnClearShortCutsClick
    end
  end
  object List: TListBox
    Left = 0
    Top = 240
    Width = 342
    Height = 117
    Align = alBottom
    ItemHeight = 13
    TabOrder = 1
  end
  object TreeView: TTreeView
    Left = 0
    Top = 0
    Width = 208
    Height = 240
    Align = alClient
    Indent = 19
    ReadOnly = True
    TabOrder = 2
    OnChange = TreeViewChange
  end
end
