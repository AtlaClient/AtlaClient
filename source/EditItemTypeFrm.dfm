object EditItemTypeForm: TEditItemTypeForm
  Left = 171
  Top = 115
  Caption = 'Item Editor'
  ClientHeight = 356
  ClientWidth = 799
  Color = clBtnFace
  Constraints.MaxHeight = 383
  Constraints.MinHeight = 383
  Constraints.MinWidth = 469
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  ShowHint = True
  WindowState = wsMaximized
  OnCreate = FormCreate
  OnShow = FormShow
  DesignSize = (
    799
    356)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 0
    Top = 112
    Width = 31
    Height = 17
    AutoSize = False
    Caption = 'Name'
    Layout = tlBottom
  end
  object Label2: TLabel
    Left = 0
    Top = 176
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'Abr'
    Layout = tlBottom
  end
  object Label3: TLabel
    Left = 272
    Top = 296
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'Flying'
    Layout = tlBottom
  end
  object Label5: TLabel
    Left = 240
    Top = 112
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'Skill'
    Layout = tlBottom
  end
  object labLevel: TLabel
    Left = 400
    Top = 112
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'Level'
    Layout = tlBottom
  end
  object Label4: TLabel
    Left = 0
    Top = 144
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'Plural'
    Layout = tlBottom
  end
  object labInput: TLabel
    Left = 240
    Top = 144
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'Input'
    Layout = tlBottom
  end
  object labNumber: TLabel
    Left = 240
    Top = 168
    Width = 41
    Height = 17
    AutoSize = False
    Caption = 'Number'
    Layout = tlBottom
  end
  object Label9: TLabel
    Left = 184
    Top = 296
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'Riding'
    Layout = tlBottom
  end
  object Label10: TLabel
    Left = 0
    Top = 296
    Width = 41
    Height = 17
    AutoSize = False
    Caption = 'Weight'
    Layout = tlBottom
  end
  object Label11: TLabel
    Left = 96
    Top = 296
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'Walk'
    Layout = tlBottom
  end
  object labHelpProd: TLabel
    Left = 240
    Top = 224
    Width = 97
    Height = 17
    AutoSize = False
    Caption = 'Production bonus +'
    Layout = tlBottom
  end
  object Label13: TLabel
    Left = 368
    Top = 296
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'Swim'
    Layout = tlBottom
  end
  object labInputPlus: TLabel
    Left = 392
    Top = 144
    Width = 6
    Height = 17
    AutoSize = False
    Caption = '+'
    Layout = tlBottom
  end
  object labRate: TLabel
    Left = 240
    Top = 195
    Width = 35
    Height = 13
    Caption = 'Rate of'
  end
  object labManMonth: TLabel
    Left = 412
    Top = 195
    Width = 52
    Height = 13
    Caption = 'man-month'
  end
  object labPer: TLabel
    Left = 340
    Top = 195
    Width = 15
    Height = 13
    Caption = 'per'
  end
  object labWithdrawCost: TLabel
    Left = 8
    Top = 267
    Width = 68
    Height = 13
    Caption = 'Withdraw cost'
  end
  object labHelpFrom: TLabel
    Left = 372
    Top = 224
    Width = 29
    Height = 17
    AutoSize = False
    Caption = 'from'
    Layout = tlBottom
  end
  object Table: TStringGrid
    Left = 0
    Top = 8
    Width = 798
    Height = 97
    Anchors = [akLeft, akTop, akRight]
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goThumbTracking]
    TabOrder = 0
    OnDblClick = TableDblClick
  end
  object edName: TEdit
    Left = 32
    Top = 112
    Width = 121
    Height = 21
    TabOrder = 1
  end
  object edAbr: TEdit
    Left = 32
    Top = 176
    Width = 41
    Height = 21
    TabOrder = 3
  end
  object edFly: TEdit
    Left = 312
    Top = 296
    Width = 49
    Height = 21
    TabOrder = 31
  end
  object cbNormal: TCheckBox
    Left = 160
    Top = 112
    Width = 65
    Height = 16
    Caption = 'Normal'
    TabOrder = 7
  end
  object cbSkill: TComboBox
    Left = 272
    Top = 112
    Width = 121
    Height = 21
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 18
    OnChange = cbSkillChange
  end
  object bnOk: TButton
    Left = 97
    Top = 328
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 33
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 289
    Top = 328
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 34
  end
  object cbLevel: TComboBox
    Left = 432
    Top = 112
    Width = 57
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 19
    Visible = False
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5')
  end
  object edNames: TEdit
    Left = 32
    Top = 144
    Width = 121
    Height = 21
    TabOrder = 2
  end
  object cbInput: TComboBox
    Left = 272
    Top = 144
    Width = 121
    Height = 21
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 20
    OnChange = cbInputChange
  end
  object edNumber: TEdit
    Left = 288
    Top = 168
    Width = 49
    Height = 21
    TabOrder = 21
  end
  object cbBattle: TCheckBox
    Left = 160
    Top = 272
    Width = 65
    Height = 16
    Hint = 'Battle Item'
    Caption = 'Battle'
    TabOrder = 17
  end
  object cbMount: TCheckBox
    Left = 160
    Top = 256
    Width = 65
    Height = 16
    Hint = 'It is mounted item'
    Caption = 'Mount'
    TabOrder = 16
  end
  object cbWeapon: TCheckBox
    Left = 160
    Top = 224
    Width = 65
    Height = 16
    Caption = 'Weapon'
    TabOrder = 14
  end
  object cbMagic: TCheckBox
    Left = 160
    Top = 208
    Width = 65
    Height = 16
    Caption = 'Magic'
    TabOrder = 13
  end
  object cbMonster: TCheckBox
    Left = 160
    Top = 176
    Width = 65
    Height = 16
    Hint = 'It is Creature'
    Caption = 'Monster'
    TabOrder = 11
  end
  object cbMan: TCheckBox
    Left = 160
    Top = 160
    Width = 65
    Height = 16
    Hint = 'It is man'
    Caption = 'Man'
    TabOrder = 10
  end
  object cbTrade: TCheckBox
    Left = 160
    Top = 144
    Width = 65
    Height = 16
    Caption = 'Trade'
    TabOrder = 9
  end
  object cbAdvanced: TCheckBox
    Left = 160
    Top = 128
    Width = 73
    Height = 16
    Caption = 'Advanced'
    TabOrder = 8
  end
  object cbArmor: TCheckBox
    Left = 160
    Top = 240
    Width = 65
    Height = 16
    Caption = 'Armor'
    TabOrder = 15
  end
  object edRide: TEdit
    Left = 216
    Top = 296
    Width = 49
    Height = 21
    TabOrder = 30
  end
  object edWeight: TEdit
    Left = 40
    Top = 296
    Width = 49
    Height = 21
    TabOrder = 28
  end
  object edWalk: TEdit
    Left = 128
    Top = 296
    Width = 49
    Height = 21
    TabOrder = 29
  end
  object cbCantGive: TCheckBox
    Left = 32
    Top = 208
    Width = 97
    Height = 17
    Caption = 'Can'#39't Give'
    TabOrder = 4
  end
  object cbFood: TCheckBox
    Left = 160
    Top = 192
    Width = 65
    Height = 16
    Hint = 'It is Consumed ite'
    Caption = 'Food'
    TabOrder = 12
  end
  object cbHelpProd: TComboBox
    Left = 400
    Top = 224
    Width = 121
    Height = 21
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 27
  end
  object cbCombat: TCheckBox
    Left = 32
    Top = 232
    Width = 97
    Height = 17
    Caption = 'Show in combat'
    TabOrder = 5
  end
  object Memo: TMemo
    Left = 528
    Top = 112
    Width = 270
    Height = 201
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 35
  end
  object edSwim: TEdit
    Left = 400
    Top = 296
    Width = 49
    Height = 21
    TabOrder = 32
  end
  object cbInput2: TComboBox
    Left = 400
    Top = 144
    Width = 121
    Height = 21
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 22
    OnChange = cbInput2Change
  end
  object edNumber2: TEdit
    Left = 416
    Top = 168
    Width = 49
    Height = 21
    TabOrder = 23
  end
  object edOut: TEdit
    Left = 288
    Top = 192
    Width = 49
    Height = 21
    TabOrder = 24
  end
  object edMonths: TEdit
    Left = 360
    Top = 192
    Width = 49
    Height = 21
    TabOrder = 25
  end
  object edWithdrawCost: TEdit
    Left = 80
    Top = 264
    Width = 57
    Height = 21
    TabOrder = 6
  end
  object edHelpMult: TEdit
    Left = 336
    Top = 224
    Width = 33
    Height = 21
    TabOrder = 26
  end
end
