object EditOrdersForm: TEditOrdersForm
  Left = 319
  Top = 242
  Caption = 'Edit Orders'
  ClientHeight = 232
  ClientWidth = 300
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesktopCenter
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 191
    Width = 300
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      300
      41)
    object bnOk: TButton
      Left = 64
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop]
      Caption = '&Ok'
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
    object bnCancel: TButton
      Left = 176
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop]
      Cancel = True
      Caption = '&Cancel'
      ModalResult = 2
      TabOrder = 1
    end
  end
  object Editor: TMemo
    Left = 0
    Top = 0
    Width = 300
    Height = 191
    Align = alClient
    TabOrder = 1
    WordWrap = False
  end
  object ActionList1: TActionList
    Left = 48
    Top = 40
    object OkAction: TAction
      Caption = 'OkAction'
      ShortCut = 16397
      OnExecute = OkActionExecute
    end
  end
end
