object SetLevelSizeForm: TSetLevelSizeForm
  Left = 509
  Top = 310
  AutoSize = True
  BorderStyle = bsDialog
  Caption = 'Set Level Size'
  ClientHeight = 73
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
  object panSettings: TPanel
    Left = 0
    Top = 0
    Width = 225
    Height = 73
    BevelOuter = bvNone
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 11
      Width = 26
      Height = 13
      Caption = 'Level'
    end
    object Label2: TLabel
      Left = 8
      Top = 44
      Width = 20
      Height = 13
      Caption = 'Size'
    end
    object Label3: TLabel
      Left = 96
      Top = 44
      Width = 7
      Height = 13
      Caption = 'X'
    end
    object cbLevel: TComboBox
      Left = 40
      Top = 8
      Width = 177
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      TabOrder = 0
      OnChange = cbLevelChange
    end
    object edX: TEdit
      Left = 40
      Top = 40
      Width = 49
      Height = 21
      TabOrder = 1
      OnKeyPress = edXKeyPress
    end
    object edY: TEdit
      Left = 112
      Top = 40
      Width = 49
      Height = 21
      TabOrder = 2
      OnKeyPress = edXKeyPress
    end
    object bnSet: TButton
      Left = 176
      Top = 40
      Width = 41
      Height = 21
      Caption = 'Set'
      TabOrder = 3
      OnClick = bnSetClick
    end
  end
end
