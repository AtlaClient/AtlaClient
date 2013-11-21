object fEditTurnOrder: TfEditTurnOrder
  Left = 205
  Top = 103
  ActiveControl = Memo1
  ClientHeight = 271
  ClientWidth = 265
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  Menu = mMenu
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 236
    Width = 265
    Height = 35
    Align = alBottom
    TabOrder = 0
    object Button1: TButton
      Left = 43
      Top = 5
      Width = 75
      Height = 25
      Caption = '&Ok'
      Default = True
      TabOrder = 0
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 147
      Top = 5
      Width = 75
      Height = 25
      Cancel = True
      Caption = '&Cancel'
      ModalResult = 2
      TabOrder = 1
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 265
    Height = 236
    Align = alClient
    TabOrder = 1
    object btRestore: TSpeedButton
      Left = 198
      Top = 5
      Width = 61
      Height = 22
      Caption = 'R&estore'
      OnClick = btRestoreClick
    end
    object Memo1: TMemo
      Left = 1
      Top = 30
      Width = 263
      Height = 205
      Align = alBottom
      TabOrder = 1
      ExplicitLeft = 0
    end
    object cbRep: TCheckBox
      Left = 3
      Top = 8
      Width = 64
      Height = 17
      Caption = '&Repeating'
      TabOrder = 0
    end
  end
  object mMenu: TMainMenu
    Left = 144
    Top = 8
    object EditOrders1: TMenuItem
      Caption = 'Edit Orders'
      Visible = False
      OnClick = EditOrders1Click
    end
    object Addorder1: TMenuItem
      Caption = 'Add order'
      Visible = False
      object miAddOrderCustom: TMenuItem
        Caption = '&Custom Order'
        ShortCut = 45
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object miAddOrderBuy: TMenuItem
        Caption = 'Buy'
      end
      object miAddOrderSell: TMenuItem
        Caption = 'Sell'
      end
      object miAddOrderGive: TMenuItem
        Caption = '&Give'
      end
      object miAddOrderCombat: TMenuItem
        Caption = 'Combat'
      end
      object miAddOrderClaim: TMenuItem
        Caption = 'Claim'
      end
      object miAddOrderPillage: TMenuItem
        Caption = 'Pillage'
      end
      object miAddOrderTax: TMenuItem
        Caption = 'Tax'
      end
      object miAddOrderForget: TMenuItem
        Caption = 'Forget'
      end
      object miAddOrderDestroy: TMenuItem
        Caption = 'Destroy'
      end
      object miAddOrderCast: TMenuItem
        Caption = 'Cast'
      end
      object miAddOrderWithdraw: TMenuItem
        Caption = 'Withdraw'
      end
      object N13: TMenuItem
        Caption = '-'
      end
      object miAddOrderStudy: TMenuItem
        Caption = '&Study'
      end
      object miAddOrderTeach: TMenuItem
        Caption = '&Teach'
      end
      object miAddOrderBuild: TMenuItem
        Caption = 'Build'
      end
      object miAddOrderProduce: TMenuItem
        Caption = '&Produce'
      end
      object miAddOrderWork: TMenuItem
        Caption = 'Work'
      end
      object miAddOrderEntertain: TMenuItem
        Caption = 'Entertain'
      end
      object miAddOrderAutotax: TMenuItem
        Break = mbBarBreak
        Caption = 'Autotax'
      end
      object miAddOrderAvoid: TMenuItem
        Caption = 'Avoid'
      end
      object miAddOrderBehind: TMenuItem
        Caption = 'Behind'
      end
      object miAddOrderGuard: TMenuItem
        Caption = 'Guard'
      end
      object miAddOrderHold: TMenuItem
        Caption = 'Hold'
      end
      object miAddOrderNoaid: TMenuItem
        Caption = 'No Aid'
      end
      object miAddOrderReveal: TMenuItem
        Caption = 'Reveal'
      end
      object miAddOrderConsume: TMenuItem
        Caption = 'Consume'
      end
      object miAddOrderNoCross: TMenuItem
        Caption = 'No Cross Water'
      end
      object miAddOrderNoSpoils: TMenuItem
        Caption = 'No Battle Spoils'
      end
      object miAddOrderSpoils: TMenuItem
        Caption = 'Spoils'
      end
      object miAddOrderSharing: TMenuItem
        Caption = 'Sharing'
      end
      object miAddOrderNameDescribe: TMenuItem
        Caption = 'Name/Describe'
      end
      object N14: TMenuItem
        Caption = '-'
      end
      object miAddOrderLeave: TMenuItem
        Caption = 'Leave'
      end
      object miAddOrderEnter: TMenuItem
        Caption = 'Enter'
      end
      object miAddOrderPromote: TMenuItem
        Caption = 'Promote'
      end
      object miAddOrderMove: TMenuItem
        Caption = '&Move'
      end
      object miAddOrderAdvance: TMenuItem
        Caption = 'Advance'
      end
      object miAddOrderSail: TMenuItem
        Caption = 'Sail'
      end
      object N20: TMenuItem
        Caption = '-'
      end
      object miAddOrderSteal: TMenuItem
        Caption = 'Steal'
      end
      object miAddOrderAssassinate: TMenuItem
        Caption = 'Assassinate'
      end
      object miAddOrderAttack: TMenuItem
        Caption = 'Attack'
      end
    end
  end
  object popMenu: TPopupMenu
    AutoHotkeys = maManual
    AutoLineReduction = maManual
    Left = 144
    Top = 72
    object miEditOrder: TMenuItem
      Caption = '&Edit order'
    end
    object miDeleteOrder: TMenuItem
      Caption = '&Delete order'
      ShortCut = 46
    end
    object miAddOrder: TMenuItem
      Caption = 'Add &Order'
      object MenuItem1: TMenuItem
        Caption = '&Custom Order'
        ShortCut = 45
      end
      object MenuItem2: TMenuItem
        Caption = 'Commentary'
      end
      object MenuItem3: TMenuItem
        Caption = '&Local descripion'
      end
      object MenuItem4: TMenuItem
        Caption = '-'
      end
      object MenuItem5: TMenuItem
        Caption = 'Buy'
      end
      object MenuItem6: TMenuItem
        Caption = 'Sell'
      end
      object MenuItem7: TMenuItem
        Caption = '&Give'
      end
      object MenuItem8: TMenuItem
        Caption = 'Combat'
      end
      object MenuItem9: TMenuItem
        Caption = 'Claim'
      end
      object MenuItem10: TMenuItem
        Caption = 'Pillage'
      end
      object MenuItem11: TMenuItem
        Caption = 'Tax'
      end
      object MenuItem12: TMenuItem
        Caption = 'Forget'
      end
      object MenuItem13: TMenuItem
        Caption = 'Destroy'
      end
      object MenuItem14: TMenuItem
        Caption = 'Cast'
      end
      object MenuItem15: TMenuItem
        Caption = 'Withdraw'
      end
      object MenuItem16: TMenuItem
        Caption = '-'
      end
      object MenuItem17: TMenuItem
        Caption = '&Study'
      end
      object MenuItem18: TMenuItem
        Caption = '&Teach'
      end
      object MenuItem19: TMenuItem
        Caption = 'Build'
      end
      object MenuItem20: TMenuItem
        Caption = '&Produce'
      end
      object MenuItem21: TMenuItem
        Caption = 'Work'
      end
      object MenuItem22: TMenuItem
        Caption = 'Entertain'
      end
      object MenuItem23: TMenuItem
        Break = mbBarBreak
        Caption = 'Autotax'
      end
      object MenuItem24: TMenuItem
        Caption = 'Avoid'
      end
      object MenuItem25: TMenuItem
        Caption = 'Behind'
      end
      object MenuItem26: TMenuItem
        Caption = 'Guard'
      end
      object MenuItem27: TMenuItem
        Caption = 'Hold'
      end
      object MenuItem28: TMenuItem
        Caption = 'No Aid'
      end
      object MenuItem29: TMenuItem
        Caption = 'Reveal'
      end
      object MenuItem30: TMenuItem
        Caption = 'Consume'
      end
      object MenuItem31: TMenuItem
        Caption = 'No Cross Water'
      end
      object MenuItem32: TMenuItem
        Caption = 'No Battle Spoils'
      end
      object MenuItem33: TMenuItem
        Caption = 'Spoils'
      end
      object MenuItem34: TMenuItem
        Caption = 'Sharing'
      end
      object MenuItem35: TMenuItem
        Caption = 'Name/Describe'
      end
      object MenuItem36: TMenuItem
        Caption = '-'
      end
      object MenuItem37: TMenuItem
        Caption = 'Leave'
      end
      object MenuItem38: TMenuItem
        Caption = 'Enter'
      end
      object MenuItem39: TMenuItem
        Caption = 'Promote'
      end
      object MenuItem40: TMenuItem
        Caption = '&Move'
      end
      object MenuItem41: TMenuItem
        Caption = 'Advance'
      end
      object MenuItem42: TMenuItem
        Caption = 'Sail'
      end
      object MenuItem43: TMenuItem
        Caption = '-'
      end
      object MenuItem44: TMenuItem
        Caption = 'Steal'
      end
      object MenuItem45: TMenuItem
        Caption = 'Assassinate'
      end
      object MenuItem46: TMenuItem
        Caption = 'Attack'
      end
    end
    object N8: TMenuItem
      Caption = '-'
    end
    object miCopyOrder: TMenuItem
      Caption = '&Copy order'
      ShortCut = 16429
    end
    object miPasteOrder: TMenuItem
      Caption = '&Paste order'
      ShortCut = 8237
    end
    object N7: TMenuItem
      Caption = '-'
    end
    object miAddOrderFormNew: TMenuItem
      Caption = '&Form new unit'
    end
    object miCopyNewUnit: TMenuItem
      Caption = 'Cop&y new unit'
    end
    object miDeleteNewUnit: TMenuItem
      Caption = 'Delete &new unit'
    end
  end
end
