object fEditArmor: TfEditArmor
  Left = 205
  Top = 107
  Caption = 'Edit Armor '
  ClientHeight = 265
  ClientWidth = 613
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  DesignSize = (
    613
    265)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 50
    Height = 13
    Caption = 'Armor type'
  end
  object Label10: TLabel
    Left = 8
    Top = 33
    Width = 60
    Height = 13
    Caption = 'Protect from '
  end
  object Memo: TMemo
    Left = 200
    Top = 55
    Width = 406
    Height = 163
    Anchors = [akLeft, akTop, akRight]
    ReadOnly = True
    TabOrder = 4
  end
  object cbAT: TComboBox
    Left = 79
    Top = 5
    Width = 73
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 0
    Items.Strings = (
      '1'
      '2'
      '3'
      '4')
  end
  object cbAss: TCheckBox
    Left = 176
    Top = 7
    Width = 125
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Used in assassination'
    TabOrder = 1
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 51
    Width = 185
    Height = 210
    TabOrder = 3
    object Label2: TLabel
      Left = 8
      Top = 16
      Width = 76
      Height = 13
      Caption = 'slashing attacks'
    end
    object Label3: TLabel
      Left = 8
      Top = 38
      Width = 75
      Height = 13
      Caption = 'piercing attacks'
    end
    object Label4: TLabel
      Left = 8
      Top = 65
      Width = 78
      Height = 13
      Caption = 'crushing attacks'
    end
    object Label5: TLabel
      Left = 8
      Top = 89
      Width = 78
      Height = 13
      Caption = 'cleaving attacks'
    end
    object Label6: TLabel
      Left = 8
      Top = 113
      Width = 104
      Height = 13
      Caption = 'armor-piercing attacks'
    end
    object Label7: TLabel
      Left = 8
      Top = 137
      Width = 70
      Height = 13
      Caption = 'energy attacks'
    end
    object Label8: TLabel
      Left = 8
      Top = 162
      Width = 59
      Height = 13
      Caption = 'spirit attacks'
    end
    object Label9: TLabel
      Left = 8
      Top = 186
      Width = 76
      Height = 13
      Caption = 'weather attacks'
    end
    object ed1: TEdit
      Left = 119
      Top = 12
      Width = 47
      Height = 21
      TabOrder = 0
      Text = '0'
    end
    object ed2: TEdit
      Left = 119
      Top = 34
      Width = 47
      Height = 21
      TabOrder = 1
      Text = '0'
    end
    object ed4: TEdit
      Left = 119
      Top = 85
      Width = 47
      Height = 21
      TabOrder = 3
      Text = '0'
    end
    object ed3: TEdit
      Left = 119
      Top = 61
      Width = 47
      Height = 21
      TabOrder = 2
      Text = '0'
    end
    object ed6: TEdit
      Left = 119
      Top = 133
      Width = 47
      Height = 21
      TabOrder = 5
      Text = '0'
    end
    object ed5: TEdit
      Left = 119
      Top = 109
      Width = 47
      Height = 21
      TabOrder = 4
      Text = '0'
    end
    object ed8: TEdit
      Left = 119
      Top = 182
      Width = 47
      Height = 21
      TabOrder = 7
      Text = '0'
    end
    object ed7: TEdit
      Left = 119
      Top = 158
      Width = 47
      Height = 21
      TabOrder = 6
      Text = '0'
    end
  end
  object bnCancel: TButton
    Left = 406
    Top = 230
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 6
  end
  object bnOk: TButton
    Left = 294
    Top = 230
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 5
    OnClick = bnOkClick
  end
  object edFrom: TEdit
    Left = 79
    Top = 29
    Width = 74
    Height = 21
    TabOrder = 2
    Text = '0'
  end
end
