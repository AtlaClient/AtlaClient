object AtlaForm: TAtlaForm
  Left = 172
  Top = 107
  Caption = 'tra'
  ClientHeight = 918
  ClientWidth = 995
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBtnText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  WindowState = wsMaximized
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShortCut = FormShortCut
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter3: TSplitter
    Left = 0
    Top = 915
    Width = 995
    Height = 3
    Cursor = crVSplit
    Align = alBottom
    Visible = False
    ExplicitTop = 312
    ExplicitWidth = 820
  end
  object PanelMain: TPanel
    Left = 0
    Top = 0
    Width = 995
    Height = 915
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object Splitter1: TSplitter
      Left = 350
      Top = 34
      Height = 881
      ResizeStyle = rsLine
      ExplicitLeft = 274
      ExplicitTop = 36
      ExplicitHeight = 276
    end
    object ToolBar: TToolBar
      Left = 0
      Top = 0
      Width = 995
      Height = 34
      AutoSize = True
      ButtonHeight = 30
      ButtonWidth = 31
      Caption = 'ToolBar'
      Color = clBtnFace
      EdgeBorders = [ebTop, ebBottom]
      Images = ToolBarImages
      Indent = 2
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      object bnShowEvents: TToolButton
        Left = 2
        Top = 0
        Action = actShowEvents
        PopupMenu = pmShowEvents
      end
      object bnRunOrders: TToolButton
        Left = 33
        Top = 0
        Action = actRunOrders
      end
      object bnRunAutoOrders: TToolButton
        Left = 64
        Top = 0
        Action = actRunAutoOrders
      end
      object sep1: TToolButton
        Left = 95
        Top = 0
        Width = 8
        Caption = 'sep1'
        ImageIndex = 2
        Style = tbsSeparator
      end
      object Label1: TLabel
        Left = 103
        Top = 0
        Width = 26
        Height = 30
        AutoSize = False
        Caption = 'Find'
        Layout = tlCenter
      end
      object edFindUnit: TEdit
        Left = 129
        Top = 0
        Width = 72
        Height = 30
        Hint = 'Find unit by number and region by coords'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnKeyPress = edFindUnitKeyPress
      end
      object sep2: TToolButton
        Left = 201
        Top = 0
        Width = 8
        Caption = 'sep2'
        ImageIndex = 1
        Style = tbsSeparator
      end
      object bnPrevUnit: TToolButton
        Left = 209
        Top = 0
        Action = actPrevExprUnit
      end
      object bnNextUnit: TToolButton
        Left = 240
        Top = 0
        Action = actNextExprUnit
      end
      object sep3: TToolButton
        Left = 271
        Top = 0
        Width = 8
        Caption = 'sep3'
        ImageIndex = 7
        Style = tbsSeparator
      end
      object Label2: TLabel
        Left = 279
        Top = 0
        Width = 32
        Height = 30
        AutoSize = False
        Caption = 'Turn'
        Layout = tlCenter
      end
      object sep4: TToolButton
        Left = 311
        Top = 0
        Width = 8
        Caption = 'sep4'
        ImageIndex = 3
        Style = tbsSeparator
      end
      object edTurnNumber: TEdit
        Left = 319
        Top = 0
        Width = 46
        Height = 30
        Hint = 'Go to turn under number'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnKeyPress = edTurnNumberKeyPress
      end
      object bnPrevTurn: TToolButton
        Left = 365
        Top = 0
        Action = actPrevTurn
      end
      object bnNextTurn: TToolButton
        Left = 396
        Top = 0
        Action = actNextTurn
      end
      object sep5: TToolButton
        Left = 427
        Top = 0
        Width = 8
        Caption = 'sep5'
        ImageIndex = 4
        Style = tbsSeparator
      end
      object bnTownMode: TToolButton
        Left = 435
        Top = 0
        Action = actTownMode
        Grouped = True
        Style = tbsCheck
      end
      object bnProdMode: TToolButton
        Left = 466
        Top = 0
        Action = actProdMode
        Grouped = True
        PopupMenu = pmProdMode
        Style = tbsCheck
      end
      object bnWedgeMode: TToolButton
        Left = 497
        Top = 0
        Action = actWedgeMode
        Grouped = True
        PopupMenu = pmWedgeMode
        Style = tbsCheck
      end
      object btInfMode: TToolButton
        Left = 528
        Top = 0
        Caption = 'btInfMode'
        Grouped = True
        ImageIndex = 0
        PopupMenu = pmInfMapM
        Style = tbsCheck
        OnClick = btInfModeClick
      end
      object ToolButton1: TToolButton
        Left = 559
        Top = 0
        Width = 2
        Caption = 'ToolButton1'
        ImageIndex = 7
        Style = tbsSeparator
      end
      object cbWedgeGroup: TComboBox
        Left = 561
        Top = 0
        Width = 90
        Height = 21
        Style = csDropDownList
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        TabOrder = 2
        OnChange = cbWedgeGroupChange
      end
      object sep6: TToolButton
        Left = 651
        Top = 0
        Width = 8
        Caption = 'sep6'
        ImageIndex = 2
        Style = tbsSeparator
      end
      object bnLevelUp: TToolButton
        Left = 659
        Top = 0
        Action = actLevelUp
        PopupMenu = pmLevelChange
      end
      object bnLevelDown: TToolButton
        Left = 690
        Top = 0
        Action = actLevelDown
        PopupMenu = pmLevelChange
      end
      object sep7: TToolButton
        Left = 721
        Top = 0
        Width = 8
        Caption = 'sep7'
        ImageIndex = 6
        Style = tbsSeparator
      end
      object lbCoord: TLabel
        Left = 729
        Top = 0
        Width = 58
        Height = 30
        Alignment = taCenter
        AutoSize = False
        Caption = '         '
        Color = clBtnFace
        ParentColor = False
        Layout = tlCenter
        WordWrap = True
      end
    end
    object PanelMap: TPanel
      Left = 353
      Top = 34
      Width = 642
      Height = 881
      Align = alClient
      BevelOuter = bvLowered
      BevelWidth = 2
      TabOrder = 0
      OnResize = PanelMapResize
      object ImageMap: TImage
        Left = 2
        Top = 2
        Width = 638
        Height = 877
        Align = alClient
        ParentShowHint = False
        ShowHint = True
        OnMouseDown = ImageMapMouseDown
        OnMouseMove = ImageMapMouseMove
        ExplicitLeft = 3
        ExplicitHeight = 763
      end
    end
    object PanelViewer: TPanel
      Left = 0
      Top = 34
      Width = 350
      Height = 881
      Align = alLeft
      BevelOuter = bvLowered
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      object Splitter2: TSplitter
        Left = 1
        Top = 156
        Width = 348
        Height = 3
        Cursor = crVSplit
        Align = alTop
        ResizeStyle = rsLine
        ExplicitTop = 129
        ExplicitWidth = 272
      end
      object View: TListBox
        Left = 1
        Top = 159
        Width = 348
        Height = 721
        Style = lbOwnerDrawFixed
        Align = alClient
        ItemHeight = 13
        TabOrder = 1
        OnClick = ViewClick
        OnDblClick = ViewDblClick
        OnDrawItem = ViewDrawItem
        OnMouseDown = ViewMouseDown
      end
      object TreeView: TTreeView
        Left = 1
        Top = 28
        Width = 348
        Height = 128
        Align = alTop
        ChangeDelay = 1
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        HideSelection = False
        Indent = 27
        ParentFont = False
        ReadOnly = True
        SortType = stText
        TabOrder = 0
        ToolTips = False
        OnChange = TreeViewChange
        OnCompare = TreeViewCompare
        OnCustomDraw = TreeViewCustomDraw
        OnCustomDrawItem = TreeViewCustomDrawItem
        OnDblClick = TreeViewDblClick
        OnKeyDown = TreeViewKeyDown
        OnMouseDown = TreeViewMouseUp
      end
      object Panel1: TPanel
        Left = 1
        Top = 1
        Width = 348
        Height = 27
        Align = alTop
        TabOrder = 2
        object btAllFoff: TSpeedButton
          Left = 2
          Top = 2
          Width = 23
          Height = 22
          Hint = 'Set off all filters'
          Enabled = False
          Glyph.Data = {
            F6000000424DF600000000000000760000002800000010000000100000000100
            0400000000008000000000000000000000001000000010000000000000000000
            8000008000000080800080000000800080008080000080808000C0C0C0000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
            8888888888888888888888888817888888878888881178888817888888711888
            8718888888811788117888880007117711888888060811111888888808087111
            8888888808771111788888808111188117888808811678881178806FF8886088
            8118800000000088888888888888888888888888888888888888}
          ParentShowHint = False
          ShowHint = True
          OnClick = btAllFoffClick
        end
        object btFiltLead: TSpeedButton
          Left = 235
          Top = 3
          Width = 23
          Height = 22
          Hint = 'Leaders/not Leaders'
          AllowAllUp = True
          GroupIndex = 4
          Glyph.Data = {
            F6000000424DF600000000000000760000002800000010000000100000000100
            0400000000008000000000000000000000001000000010000000000000000000
            8000008000000080800080000000800080008080000080808000C0C0C0000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
            8888888888888881888888888888881178880888880881111888088000881118
            1188800880888188811888080008888888118008888088888881000088808888
            8888000888880888888800088800888888880000088088888888000000080888
            8888800000000888888888000008888888888888888888888888}
          ParentShowHint = False
          ShowHint = True
          OnClick = btFiltLeadClick
        end
        object SpeedButton3: TSpeedButton
          Left = 258
          Top = 3
          Width = 12
          Height = 22
          Hint = 'Leaders/not Leaders'
          Glyph.Data = {
            5A000000424D5A000000000000003E0000002800000007000000070000000100
            0100000000001C0000000000000000000000020000000000000000000000FFFF
            FF00FE000000FE000000EE000000C600000082000000FE000000FE000000}
          Margin = 0
          ParentShowHint = False
          ShowHint = True
          OnClick = SpeedButton3Click
        end
        object btFF: TSpeedButton
          Left = 273
          Top = 3
          Width = 23
          Height = 22
          Hint = 'Flags'
          AllowAllUp = True
          GroupIndex = 5
          Glyph.Data = {
            F6000000424DF600000000000000760000002800000010000000100000000100
            0400000000008000000000000000000000001000000010000000000000000000
            8000008000000080800080000000800080008080000080808000C0C0C0000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
            88880888888888888888808888888870088888088888800CC08888808888044C
            CC08888808880444CCC08888808804444CC088888800CC44400888888880CCC4
            0888888888880CCC08888888888880CC08888888888888008888888888888880
            8888888888888888088888888888888888888888888888888888}
          ParentShowHint = False
          ShowHint = True
          OnClick = btFFClick
        end
        object SpeedButton5: TSpeedButton
          Left = 296
          Top = 3
          Width = 12
          Height = 22
          Hint = 'Flags'
          Glyph.Data = {
            5A000000424D5A000000000000003E0000002800000007000000070000000100
            0100000000001C0000000000000000000000020000000000000000000000FFFF
            FF00FE000000FE000000EE000000C600000082000000FE000000FE000000}
          Margin = 0
          ParentShowHint = False
          ShowHint = True
          OnClick = SpeedButton5Click
        end
        object btFiltMO: TSpeedButton
          Left = 310
          Top = 3
          Width = 23
          Height = 22
          Hint = 'Monthlong order'
          AllowAllUp = True
          GroupIndex = 5
          Glyph.Data = {
            F6000000424DF600000000000000760000002800000010000000100000000100
            0400000000008000000000000000000000001000000010000000000000000000
            8000008000000080800080000000800080008080000080808000C0C0C0000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
            8888888800000000000088880F8F8F8F8F80800808F8F80000F08B000F8F8F0F
            F08087B00800080000F087FB008F8F8F8F80887FB008F80000F00000FB008F0F
            F0807FBBBBB0080000F087FB008F8F8F8F80887FB00444444444887FBB004444
            44448887FBB008888888888878BB008888888888888888888888}
          ParentShowHint = False
          ShowHint = True
          OnClick = btFiltMOClick
        end
        object SpeedButton2: TSpeedButton
          Left = 333
          Top = 3
          Width = 12
          Height = 22
          Hint = 'Monthlong order'
          Glyph.Data = {
            5A000000424D5A000000000000003E0000002800000007000000070000000100
            0100000000001C0000000000000000000000020000000000000000000000FFFF
            FF00FE000000FE000000EE000000C600000082000000FE000000FE000000}
          Margin = 0
          ParentShowHint = False
          ShowHint = True
          OnClick = SpeedButton2Click
        end
        object cbFacDiap: TComboBox
          Left = 26
          Top = 2
          Width = 105
          Height = 21
          Hint = 'Faction filter'
          Style = csDropDownList
          ItemHeight = 13
          ParentShowHint = False
          ShowHint = True
          Sorted = True
          TabOrder = 0
          OnChange = cbFacDiapChange
        end
        object cbProfSel: TComboBox
          Left = 130
          Top = 2
          Width = 105
          Height = 21
          Hint = 'Skills filter'
          Style = csDropDownList
          ItemHeight = 13
          ParentShowHint = False
          ShowHint = True
          Sorted = True
          TabOrder = 1
          OnChange = cbProfSelChange
        end
      end
    end
  end
  object MainMenu1: TMainMenu
    AutoHotkeys = maManual
    AutoLineReduction = maManual
    Left = 64
    Top = 200
    object miGame: TMenuItem
      Caption = '&Game'
      object miLoadReport: TMenuItem
        Caption = 'Load &report(s)'
        ShortCut = 116
        OnClick = miLoadReportClick
      end
      object miLoadOrder: TMenuItem
        Caption = '&Load order(s)'
        ShortCut = 114
        OnClick = miLoadOrderClick
      end
      object miLoadFromClipboard: TMenuItem
        Caption = 'Load from Clipboard'
        ShortCut = 115
        OnClick = miLoadFromClipboardClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object miSaveOrder: TMenuItem
        Caption = '&Save orders'
        ShortCut = 113
        OnClick = miSaveOrderClick
      end
      object miSaveReport: TMenuItem
        Caption = 'Save reports'
        OnClick = miSaveReportClick
      end
      object miSaveMap: TMenuItem
        Caption = 'Save &map'
        OnClick = miSaveMapClick
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object miCF: TMenuItem
        Caption = 'Local faction'
      end
      object miCFr: TMenuItem
        Caption = '-'
      end
      object miGameNew: TMenuItem
        Caption = '&New Game'
        OnClick = miGameNewClick
      end
      object miGameChange: TMenuItem
        Caption = '&Change Game'
        OnClick = miGameChangeClick
      end
      object miGameSave: TMenuItem
        Caption = 'S&ave Game'
        OnClick = miGameSaveClick
      end
      object miStoreTurns: TMenuItem
        Caption = 'S&tore turns'
        OnClick = miStoreTurnsClick
      end
      object miImport: TMenuItem
        Caption = '&Import'
        object miAtlaCart: TMenuItem
          Caption = '&AtlaCart'
          OnClick = miAtlaCartClick
        end
        object miCrystalBall: TMenuItem
          Caption = '&CrystalBall (regions only)'
          OnClick = miCrystalBallClick
        end
      end
      object miExprot: TMenuItem
        Caption = 'Ex&port'
        object miExportMages: TMenuItem
          Caption = '&Mages'
          OnClick = miExportMagesClick
        end
        object miExportMage: TMenuItem
          Caption = 'M&age'
          OnClick = miExportMageClick
        end
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = '&Exit'
        OnClick = Exit1Click
      end
      object N9: TMenuItem
        Caption = '-'
      end
      object Exitwithoutsaving1: TMenuItem
        Caption = 'E&xit without saving'
        ShortCut = 32856
        OnClick = Exitwithoutsaving1Click
      end
    end
    object miView: TMenuItem
      Caption = '&View'
      OnClick = miViewClick
      object miLevelUp: TMenuItem
        Action = actLevelUp
      end
      object miLevelDown: TMenuItem
        Action = actLevelDown
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object miPrevUnit: TMenuItem
        Action = actPrevUnit
      end
      object miNextUnit: TMenuItem
        Action = actNextUnit
      end
      object miPrevExprUnit: TMenuItem
        Action = actPrevExprUnit
      end
      object miNextExprUnit: TMenuItem
        Action = actNextExprUnit
      end
      object miSetUnitExpr: TMenuItem
        Caption = 'Set Unit Expr'
        OnClick = miSetUnitExprClick
      end
      object N12: TMenuItem
        Caption = '-'
      end
      object miPrevTurn: TMenuItem
        Action = actPrevTurn
      end
      object miNextTurn: TMenuItem
        Action = actNextTurn
      end
      object N6: TMenuItem
        Caption = '-'
      end
      object miShowRegionInfo: TMenuItem
        Action = actShowRegInfo
        ShortCut = 32850
      end
      object miShowMap: TMenuItem
        Action = actShowMap
        ShortCut = 32845
      end
      object miShowRegionList: TMenuItem
        Action = actShowRegionList
      end
      object N10: TMenuItem
        Caption = '-'
      end
      object miViewFactions: TMenuItem
        Caption = '&Factions'
        OnClick = miViewFactionsClick
      end
      object miViewMages: TMenuItem
        Caption = 'Ma&ges'
        OnClick = miViewMagesClick
      end
      object miViewTaxRegions: TMenuItem
        Caption = '&Tax Regions'
        OnClick = miViewTaxRegionsClick
      end
      object miViewGates: TMenuItem
        Caption = 'Gates'
        OnClick = miViewGatesClick
      end
      object miSpecialists: TMenuItem
        Caption = '&Specialists'
        OnClick = miSpecialistsClick
      end
      object miViewUnits: TMenuItem
        Caption = '&Units'
        ShortCut = 32853
        OnClick = miViewUnitsClick
      end
      object TownTrade1: TMenuItem
        Caption = 'Town Trade'
        OnClick = TownTrade1Click
      end
      object N17: TMenuItem
        Caption = '-'
      end
      object miProdModeSetup: TMenuItem
        Caption = '&Products mode settings'
        OnClick = miProdModeSetupClick
      end
      object miWedgemodesettings: TMenuItem
        Caption = '&Wedge mode settings'
        ShortCut = 112
        OnClick = miWedgemodesettingsClick
      end
    end
    object miAnalyze: TMenuItem
      Caption = '&Analyze'
      object miAdvProds: TMenuItem
        Caption = 'Advanced &products'
        OnClick = miAdvProdsClick
      end
      object miBattleSim: TMenuItem
        Caption = '&Battle Simulator'
        ShortCut = 49218
        OnClick = miBattleSimClick
      end
      object miChanges: TMenuItem
        Caption = '&Changes'
        object miChangesCollect: TMenuItem
          Caption = '&Collect'
          ShortCut = 49219
          OnClick = miChangesCollectClick
        end
        object miChangesView: TMenuItem
          Caption = '&View'
          ShortCut = 49238
          OnClick = miChangesViewClick
        end
      end
      object miSupportBuilding: TMenuItem
        Caption = '&Support Building'
        OnClick = miSupportBuildingClick
      end
      object miAnalyzeBalance: TMenuItem
        Caption = 'Ba&lance'
        OnClick = miAnalyzeBalanceClick
      end
      object miAnalyzeProducts: TMenuItem
        Caption = 'P&roducts'
        OnClick = miAnalyzeProductsClick
      end
    end
    object Help1: TMenuItem
      Caption = '&Settings'
      object miSkillsTree: TMenuItem
        Caption = 'Skills Tree'
        OnClick = miSkillsTreeClick
      end
      object miShowItemTypes: TMenuItem
        Caption = '&Item types'
        OnClick = miShowItemTypesClick
      end
      object Objecttypes1: TMenuItem
        Caption = 'Object types'
        OnClick = Objecttypes1Click
      end
      object miShowWeatherRegions: TMenuItem
        Caption = '&Weather regions'
        OnClick = miShowWeatherRegionsClick
      end
      object miShowItObjReports: TMenuItem
        Caption = 'I&tem && Object reports'
        OnClick = miShowItObjReportsClick
      end
      object N16: TMenuItem
        Caption = '-'
      end
      object miVerifyTypes: TMenuItem
        Caption = '&Verify Types'
        OnClick = miVerifyTypesClick
      end
      object N11: TMenuItem
        Caption = '-'
      end
      object miOptions: TMenuItem
        Caption = '&Options'
        ShortCut = 123
        OnClick = miOptionsClick
      end
      object miShortCuts: TMenuItem
        Caption = 'ShortCuts'
        OnClick = miShortCutsClick
      end
    end
    object miFix: TMenuItem
      Caption = 'Fix'
      object miAdvancedProductsFix: TMenuItem
        Caption = 'Advanced Products'
        OnClick = miAdvancedProductsFixClick
      end
      object miVerifynextturn: TMenuItem
        Caption = 'Verify next turn'
        OnClick = miVerifynextturnClick
      end
      object miCountLoadedTurns: TMenuItem
        Caption = 'Count Loaded Turns'
        OnClick = miCountLoadedTurnsClick
      end
      object miMinimizeTurns: TMenuItem
        Caption = 'Minimize Turns'
        OnClick = miMinimizeTurnsClick
      end
      object miUpdateTurns: TMenuItem
        Caption = 'Update Turns'
        OnClick = miUpdateTurnsClick
      end
      object miDeleteLastTurn: TMenuItem
        Caption = 'Delete Last Turn'
        OnClick = miDeleteLastTurnClick
      end
      object miSetLevelSize: TMenuItem
        Caption = 'Set Level Size'
        OnClick = miSetLevelSizeClick
      end
      object miUpdateNonLocal: TMenuItem
        Caption = 'Update NonLocal'
        OnClick = miUpdateNonLocalClick
      end
      object miClearImportedMages: TMenuItem
        Caption = 'Clear imported mages'
        OnClick = miClearImportedMagesClick
      end
    end
    object miTemporary: TMenuItem
      Caption = '&Temporary'
      object VerTime1: TMenuItem
        Caption = 'VerTime'
        Enabled = False
        OnClick = VerTime1Click
      end
      object miSavereginfo: TMenuItem
        Caption = '&Save reginfo'
        Enabled = False
        OnClick = miSavereginfoClick
      end
      object miAnalyzeCityRegion: TMenuItem
        Caption = 'Analyze &City Region'
        OnClick = miAnalyzeCityRegionClick
      end
      object miAnalyzeRegionPop: TMenuItem
        Caption = 'Analyze &Population'
        OnClick = miAnalyzeRegionPopClick
      end
      object miPluginsUnload: TMenuItem
        Caption = '&Unload Plugins'
        OnClick = miPluginsUnloadClick
      end
      object miPluginsReload: TMenuItem
        Caption = '&Reload Plugins'
        OnClick = miPluginsReloadClick
      end
    end
    object miMail: TMenuItem
      Caption = 'Ma&il'
      OnClick = miMailClick
      object miSendTimesRequest: TMenuItem
        Caption = 'Send &Times Request'
        OnClick = miSendTimesRequestClick
      end
      object miSendOrders: TMenuItem
        Caption = 'Send &Orders'
        OnClick = miSendOrdersClick
      end
    end
    object miPlugins: TMenuItem
      Caption = '&Plugins'
      object miConfigure: TMenuItem
        Caption = '&Configure'
      end
      object miPluginsRoot: TMenuItem
        Caption = '-'
      end
    end
    object Scripts1: TMenuItem
      Caption = 'Scripts'
      object miExecScr: TMenuItem
        Caption = 'Execute'
      end
      object N21: TMenuItem
        Caption = '-'
      end
      object Editor1: TMenuItem
        Caption = 'Editor'
        OnClick = Editor1Click
      end
    end
    object miAbout: TMenuItem
      Caption = 'About'
      OnClick = miAboutClick
    end
    object Test1: TMenuItem
      Caption = 'Test'
      Visible = False
    end
  end
  object MapImages: TImageList
    DrawingStyle = dsTransparent
    Left = 8
    Top = 232
  end
  object InfoImages: TImageList
    DrawingStyle = dsTransparent
    Height = 9
    Width = 9
    Left = 72
    Top = 232
  end
  object OpenDialog: TOpenDialog
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 104
    Top = 232
  end
  object popupViewRegObject: TPopupMenu
    AutoHotkeys = maManual
    AutoLineReduction = maManual
    OnPopup = popupViewRegObjectPopup
    Left = 360
    Top = 144
    object miLinkInner: TMenuItem
      Caption = '&Link shaft'
      OnClick = miLinkInnerClick
    end
    object miGotoLinked: TMenuItem
      Caption = 'G&oto linked'
      OnClick = miGotoLinkedClick
    end
    object miCopyToClipboard1: TMenuItem
      Action = actCopyToClipboard
    end
  end
  object popupViewUnit: TPopupMenu
    AutoHotkeys = maManual
    AutoLineReduction = maManual
    OnPopup = popupViewUnitPopup
    Left = 392
    Top = 144
    object miEditOrder: TMenuItem
      Caption = '&Edit order'
      OnClick = miEditOrderClick
    end
    object miDeleteOrder: TMenuItem
      Caption = '&Delete order'
      ShortCut = 46
      OnClick = miDeleteOrderClick
    end
    object miEditOrders: TMenuItem
      Caption = 'Edit Orders'
      ShortCut = 16397
      OnClick = miEditOrdersClick
    end
    object miAddOrder: TMenuItem
      Caption = 'Add &Order'
      object miAddOrderCustom: TMenuItem
        Caption = '&Custom Order'
        ShortCut = 45
        OnClick = miAddOrderCustomClick
      end
      object miAddOrderComment: TMenuItem
        Caption = 'Commentary'
        OnClick = miAddOrderCommentClick
      end
      object miAddOrderLocDescr: TMenuItem
        Caption = '&Local descripion'
        OnClick = miAddOrderLocDescrClick
      end
      object TURNorder1: TMenuItem
        Caption = 'TURN order'
        OnClick = TURNorder1Click
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object miAddOrderBuy: TMenuItem
        Caption = 'Buy'
        OnClick = miAddOrderBuySellClick
      end
      object miAddOrderSell: TMenuItem
        Caption = 'Sell'
        OnClick = miAddOrderBuySellClick
      end
      object miAddOrderGive: TMenuItem
        Caption = '&Give'
        OnClick = miAddOrderGiveClick
      end
      object miAddOrderCombat: TMenuItem
        Caption = 'Combat'
        OnClick = miAddOrderAbrClick
      end
      object miAddOrderClaim: TMenuItem
        Caption = 'Claim'
        OnClick = miAddOrderInt
      end
      object miAddOrderPillage: TMenuItem
        Caption = 'Pillage'
        OnClick = miAddOrderSimpleClick
      end
      object miAddOrderTax: TMenuItem
        Caption = 'Tax'
        OnClick = miAddOrderSimpleClick
      end
      object miAddOrderForget: TMenuItem
        Caption = 'Forget'
        OnClick = miAddOrderAbrClick
      end
      object miAddOrderDestroy: TMenuItem
        Caption = 'Destroy'
        OnClick = miAddOrderSimpleClick
      end
      object miAddOrderCast: TMenuItem
        Caption = 'Cast'
        OnClick = miAddOrderCastClick
      end
      object miAddOrderWithdraw: TMenuItem
        Caption = 'Withdraw'
        OnClick = miAddOrderWithdrawClick
      end
      object N13: TMenuItem
        Caption = '-'
      end
      object miAddOrderStudy: TMenuItem
        Caption = '&Study'
        OnClick = miAddOrderAbrClick
      end
      object miAddOrderTeach: TMenuItem
        Caption = '&Teach'
        OnClick = miAddOrderTeachClick
      end
      object miAddOrderBuild: TMenuItem
        Caption = 'Build'
        OnClick = miAddOrderBuildClick
      end
      object miAddOrderProduce: TMenuItem
        Caption = '&Produce'
        OnClick = miAddOrderAbrClick
      end
      object miAddOrderWork: TMenuItem
        Caption = 'Work'
        OnClick = miAddOrderSimpleClick
      end
      object miAddOrderEntertain: TMenuItem
        Caption = 'Entertain'
        OnClick = miAddOrderSimpleClick
      end
      object miAddOrderAutotax: TMenuItem
        Break = mbBarBreak
        Caption = 'Autotax'
        OnClick = miAddOrderByteClick
      end
      object miAddOrderAvoid: TMenuItem
        Caption = 'Avoid'
        OnClick = miAddOrderByteClick
      end
      object miAddOrderBehind: TMenuItem
        Caption = 'Behind'
        OnClick = miAddOrderByteClick
      end
      object miAddOrderGuard: TMenuItem
        Caption = 'Guard'
        OnClick = miAddOrderByteClick
      end
      object miAddOrderHold: TMenuItem
        Caption = 'Hold'
        OnClick = miAddOrderByteClick
      end
      object miAddOrderNoaid: TMenuItem
        Caption = 'No Aid'
        OnClick = miAddOrderByteClick
      end
      object miAddOrderReveal: TMenuItem
        Caption = 'Reveal'
        OnClick = miAddOrderByteClick
      end
      object miAddOrderConsume: TMenuItem
        Caption = 'Consume'
        OnClick = miAddOrderByteClick
      end
      object miAddOrderNoCross: TMenuItem
        Caption = 'No Cross Water'
        OnClick = miAddOrderByteClick
      end
      object miAddOrderNoSpoils: TMenuItem
        Caption = 'No Battle Spoils'
        OnClick = miAddOrderByteClick
      end
      object miAddOrderSpoils: TMenuItem
        Caption = 'Spoils'
        OnClick = miAddOrderByteClick
      end
      object miAddOrderSharing: TMenuItem
        Caption = 'Sharing'
        OnClick = miAddOrderByteClick
      end
      object miAddOrderNameDescribe: TMenuItem
        Caption = 'Name/Describe'
        OnClick = miAddOrderNameDescribeClick
      end
      object N14: TMenuItem
        Caption = '-'
      end
      object miAddOrderLeave: TMenuItem
        Caption = 'Leave'
        OnClick = miAddOrderSimpleClick
      end
      object miAddOrderEnter: TMenuItem
        Caption = 'Enter'
        OnClick = miAddOrderIntClick
      end
      object miAddOrderPromote: TMenuItem
        Caption = 'Promote'
        OnClick = miAddOrderIntClick
      end
      object miAddOrderMove: TMenuItem
        Caption = '&Move'
        OnClick = miAddOrderMovingClick
      end
      object miAddOrderAdvance: TMenuItem
        Caption = 'Advance'
        OnClick = miAddOrderMovingClick
      end
      object miAddOrderSail: TMenuItem
        Caption = 'Sail'
        OnClick = miAddOrderMovingClick
      end
      object N20: TMenuItem
        Caption = '-'
      end
      object miAddOrderSteal: TMenuItem
        Caption = 'Steal'
        OnClick = miAddOrderStealClick
      end
      object miAddOrderAssassinate: TMenuItem
        Caption = 'Assassinate'
        OnClick = miAddOrderAssassinateClick
      end
      object miAddOrderAttack: TMenuItem
        Caption = 'Attack'
        OnClick = miAddOrderAttackClick
      end
    end
    object miAddOrderAuto: TMenuItem
      Caption = 'Add &Auto Order'
      object miAddOrderAutoGive: TMenuItem
        Caption = 'Auto &Give'
        OnClick = miAddOrderAutoGiveClick
      end
      object miAddOrderNOP: TMenuItem
        Caption = '&NOP (Do nothing)'
        OnClick = miAddOrderSimpleClick
      end
      object miAddOrderAutoMove: TMenuItem
        Caption = 'Auto &Move'
        OnClick = miAddOrderAutoMoveClick
      end
      object miAddOrderAutoWarning: TMenuItem
        Caption = 'Auto &Warning'
        OnClick = miAddOrderAutoWarningClick
      end
      object miAddOrderAutoFollow: TMenuItem
        Caption = 'Auto Follow'
        OnClick = miAddOrderAutoFollowClick
      end
    end
    object miAddClientOrder: TMenuItem
      Caption = 'Add C&lient Order'
      object miReceive: TMenuItem
        Caption = '&Receive'
        OnClick = miReceiveClick
      end
      object miSetFlags: TMenuItem
        Caption = 'Set &flags'
        OnClick = miSetFlagsClick
      end
      object miAddOrderWithdrawClient: TMenuItem
        Caption = '&Withdraw (clientonly)'
        OnClick = miAddOrderWithdrawClientClick
      end
    end
    object miGotoOrderTarget: TMenuItem
      Caption = '&Goto order target'
      OnClick = miGotoOrderTargetClick
    end
    object N8: TMenuItem
      Caption = '-'
    end
    object miCopyOrder: TMenuItem
      Caption = '&Copy order'
      ShortCut = 16429
      OnClick = miCopyOrderClick
    end
    object miPasteOrder: TMenuItem
      Caption = '&Paste order'
      ShortCut = 8237
      OnClick = miPasteOrderClick
    end
    object miCopyToClipboard2: TMenuItem
      Action = actCopyToClipboard
    end
    object miMoveOrderUp: TMenuItem
      Caption = 'Move Order Up'
      ShortCut = 109
      OnClick = miMoveOrderUpClick
    end
    object miMoveOrderDown: TMenuItem
      Caption = 'Move Order Down'
      ShortCut = 107
      OnClick = miMoveOrderDownClick
    end
    object N7: TMenuItem
      Caption = '-'
    end
    object miAddOrderFormNew: TMenuItem
      Caption = '&Form new unit'
      OnClick = miAddOrderFormNewClick
    end
    object miCopyNewUnit: TMenuItem
      Caption = 'Cop&y new unit'
      OnClick = miCopyNewUnitClick
    end
    object miDeleteNewUnit: TMenuItem
      Caption = 'Delete &new unit'
      OnClick = miDeleteNewUnitClick
    end
    object N15: TMenuItem
      Caption = '-'
    end
    object miAddAttacker: TMenuItem
      Caption = 'Add as attacker'
      OnClick = miAddAttackerClick
    end
    object miAddDefender: TMenuItem
      Caption = 'Add as defender'
      OnClick = miAddDefenderClick
    end
    object miAddAttackers: TMenuItem
      Caption = 'Add attackers'
      OnClick = miAddAttackersClick
    end
    object miAddDefenders: TMenuItem
      Caption = 'Add defenders'
      OnClick = miAddDefendersClick
    end
  end
  object popupFactionMessages: TPopupMenu
    AutoHotkeys = maManual
    AutoLineReduction = maManual
    OnPopup = popupFactionMessagesPopup
    Left = 424
    Top = 144
    object miFacClearErrors: TMenuItem
      Caption = '&Clear errors'
      OnClick = miFacClearErrorsClick
    end
    object miFacClearError: TMenuItem
      Caption = '&Delete error'
      OnClick = miFacClearErrorClick
    end
    object miFacClearBattles: TMenuItem
      Caption = '&Clear battles'
      OnClick = miFacClearBattlesClick
    end
    object miFacClearBattle: TMenuItem
      Caption = '&Delete battle'
      OnClick = miFacClearBattleClick
    end
    object miFacGotoBattle: TMenuItem
      Caption = '&Goto battle'
      OnClick = miFacGotoBattleClick
    end
    object miFacBattleToSim: TMenuItem
      Caption = '&To Simulator'
      Default = True
      OnClick = miFacBattleToSimClick
    end
    object miFacBattleToClipboard: TMenuItem
      Caption = 'Cop&y Battle To Clipboard'
      ShortCut = 16451
      OnClick = miFacBattleToClipboardClick
    end
    object miFacClearEvents: TMenuItem
      Caption = '&Clear events'
      OnClick = miFacClearEventsClick
    end
    object miFacClearEvent: TMenuItem
      Caption = '&Delete event'
      OnClick = miFacClearEventClick
    end
    object miCopyToClipboard3: TMenuItem
      Action = actCopyToClipboard
    end
  end
  object popupViewRegion: TPopupMenu
    AutoHotkeys = maManual
    AutoLineReduction = maManual
    OnPopup = popupViewRegionPopup
    Left = 456
    Top = 144
    object miSetTaxInc: TMenuItem
      Caption = 'Set &Tax income'
      OnClick = miSetTaxIncClick
    end
    object miSetWorkInc: TMenuItem
      Caption = 'Set &Work income'
      OnClick = miSetWorkIncClick
    end
    object miSetEntInc: TMenuItem
      Caption = 'Set &Entertain income'
      OnClick = miSetEntIncClick
    end
    object miSetRegDescription: TMenuItem
      Caption = 'Set &Description'
      OnClick = miSetRegDescriptionClick
    end
    object miSetRegionUserInfo: TMenuItem
      Caption = 'Set &User Info'
      OnClick = miSetRegionUserInfoClick
    end
    object miGotoLastInfo: TMenuItem
      Caption = '&Goto last info'
      OnClick = miGotoLastInfoClick
    end
    object miWeather: TMenuItem
      Caption = 'We&ather'
      object t11: TMenuItem
        Caption = 't+1'
      end
      object t21: TMenuItem
        Caption = 't+2'
      end
      object t31: TMenuItem
        Caption = 't+3'
      end
      object t41: TMenuItem
        Caption = 't+4'
      end
      object r51: TMenuItem
        Caption = 't+5'
      end
      object t61: TMenuItem
        Caption = 't+6'
      end
      object t71: TMenuItem
        Caption = 't+7'
      end
      object t81: TMenuItem
        Caption = 't+8'
      end
      object t91: TMenuItem
        Caption = 't+9'
      end
      object t101: TMenuItem
        Caption = 't+10'
      end
      object t111: TMenuItem
        Caption = 't+11'
      end
      object t121: TMenuItem
        Caption = 't+12'
      end
    end
    object miCopyToClipboard4: TMenuItem
      Action = actCopyToClipboard
    end
    object miCopyRegToClipboard: TMenuItem
      Caption = 'Copy &region to clipboard'
      OnClick = miCopyRegToClipboardClick
    end
    object miAddToRegionList: TMenuItem
      Caption = 'Add to Region &List'
      ShortCut = 45
      OnClick = miAddToRegionListClick
    end
    object miDeleteFromRegionList: TMenuItem
      Caption = 'Delete &from Region List'
      ShortCut = 46
      OnClick = miDeleteFromRegionListClick
    end
  end
  object ToolBarImages: TImageList
    Height = 24
    Width = 24
    Left = 8
    Top = 264
  end
  object ActionList1: TActionList
    Left = 136
    Top = 232
    object actEnableTemp: TAction
      Caption = 'actEnableTemp'
      ShortCut = 122
      OnExecute = actEnableTempExecute
    end
    object actLevelUp: TAction
      Category = 'View'
      Caption = 'Level Up'
      Hint = 'Level Up'
      OnExecute = actLevelUpExecute
    end
    object actLevelDown: TAction
      Category = 'View'
      Caption = 'Level Down'
      Hint = 'Level Down'
      OnExecute = actLevelDownExecute
    end
    object actPrevTurn: TAction
      Category = 'View'
      Caption = 'Prev Turn'
      Hint = 'Go to Prev Turn'
      OnExecute = actPrevTurnExecute
    end
    object actNextTurn: TAction
      Category = 'View'
      Caption = 'Next Turn'
      Hint = 'Go to Next Turn'
      OnExecute = actNextTurnExecute
    end
    object actShowRegInfo: TAction
      Category = 'View'
      Caption = 'S&how region info'
      OnExecute = actShowRegInfoExecute
    end
    object actShowMap: TAction
      Category = 'View'
      Caption = 'Show &Map'
      OnExecute = actShowMapExecute
    end
    object actPrevUnit: TAction
      Category = 'View'
      Caption = 'Prev Local Unit'
      Hint = 'Go to Prev Local Unit'
      OnExecute = actPrevUnitExecute
    end
    object actNextUnit: TAction
      Category = 'View'
      Caption = 'Next Local Unit'
      Hint = 'Go to Next Local Unit'
      OnExecute = actNextUnitExecute
    end
    object actPrevExprUnit: TAction
      Category = 'View'
      Caption = 'Prev Expr Unit'
      Hint = 'Go to Prev Expr Unit'
      ShortCut = 32956
      OnExecute = actPrevExprUnitExecute
    end
    object actCopyToClipboard: TAction
      Caption = 'Co&py to clipboard'
      ShortCut = 16451
      OnExecute = actCopyToClipboardClick
    end
    object actHideClient: TAction
      Caption = 'actHideClient'
      ShortCut = 49242
      OnExecute = actHideClientExecute
    end
    object actShowEvents: TAction
      Category = 'ToolBar'
      Caption = 'Show &Events'
      Hint = 'Show event window'
      OnExecute = bnShowEventsClick
    end
    object actRunOrders: TAction
      Category = 'ToolBar'
      Caption = 'Run &Orders'
      Hint = 'Run orders'
      OnExecute = bnRunOrdersClick
    end
    object actRunAutoOrders: TAction
      Category = 'ToolBar'
      Caption = 'Run &AutoOrders'
      Hint = 'Run autoorders'
      OnExecute = bnRunAutoOrdersClick
    end
    object actTownMode: TAction
      Category = 'ToolBar'
      Caption = '&Town Map Mode'
      Hint = 'Town Map Mode'
      OnExecute = bnTownModeClick
    end
    object actProdMode: TAction
      Category = 'ToolBar'
      Caption = '&Product Map Mode'
      Hint = 'Product Map Mode'
      OnExecute = bnProdModeClick
    end
    object actWedgeMode: TAction
      Category = 'ToolBar'
      Caption = '&Wedge Map Mode'
      Hint = 'Wedge Map Mode'
      OnExecute = bnWedgeModeClick
    end
    object actNextExprUnit: TAction
      Category = 'View'
      Caption = 'Next Expr Unit'
      Hint = 'Go to Next Expr Unit'
      ShortCut = 32958
      OnExecute = actNextExprUnitExecute
    end
    object actShowRegionList: TAction
      Category = 'View'
      Caption = 'Show Region &List'
      ShortCut = 32844
      OnExecute = actShowRegionListExecute
    end
  end
  object popupViewDefault: TPopupMenu
    AutoHotkeys = maManual
    AutoLineReduction = maManual
    OnPopup = popupViewDefaultPopup
    Left = 488
    Top = 144
    object miCopyToClipboard0: TMenuItem
      Action = actCopyToClipboard
    end
  end
  object TrayIcon: TTrayIcon
    Left = 176
    Top = 232
  end
  object pmProdMode: TPopupMenu
    Left = 32
    Top = 108
    object miProdModeSetup2: TMenuItem
      Caption = 'Settings'
      OnClick = miProdModeSetupClick
    end
  end
  object pmWedgeMode: TPopupMenu
    Left = 72
    Top = 108
    object miWedgemodesettings2: TMenuItem
      Caption = 'Settings'
      OnClick = miWedgemodesettingsClick
    end
  end
  object pmShowEvents: TPopupMenu
    OnPopup = pmShowEventsPopup
    Left = 112
    Top = 108
    object miRunOrdersOnChange: TMenuItem
      Caption = 'Run Orders On Change'
      OnClick = miRunOrdersOnChangeClick
    end
    object N18: TMenuItem
      Caption = '-'
    end
    object miShowDiscardWarnings: TMenuItem
      Caption = 'Show "DISCARD" warnings'
      OnClick = miShowDiscardWarningsClick
    end
    object miShowNoMonthLongErrors: TMenuItem
      Caption = 'Show "no month-long" errors'
      OnClick = miShowNoMonthLongErrorsClick
    end
    object miShowTaxWarnings: TMenuItem
      Caption = 'Show tax/ent./work income warnings'
      OnClick = miShowTaxWarningsClick
    end
    object miShowMoveError: TMenuItem
      Caption = 'Show Move/Sail "can'#39't reach" errors'
      OnClick = miShowMoveErrorClick
    end
    object miShowBorrowsWarnings: TMenuItem
      Caption = 'Show Borrows in region warning'
      OnClick = miShowBorrowsWarningsClick
    end
    object miShowTeachNonLocNotStudy: TMenuItem
      Caption = 'Show Not Study Teach errors for nonlocals'
      OnClick = miShowTeachNonLocNotStudyClick
    end
    object N19: TMenuItem
      Caption = '-'
    end
    object miShowRegionDataFound: TMenuItem
      Caption = 'Show Region data found'
      OnClick = miShowRegionDataFoundClick
    end
  end
  object pmLevelChange: TPopupMenu
    OnPopup = pmLevelChangePopup
    Left = 152
    Top = 108
  end
  object AutoSaveTimer: TTimer
    Enabled = False
    OnTimer = AutoSaveTimerTimer
    Left = 72
    Top = 268
  end
  object pmToolBar: TPopupMenu
    Left = 192
    Top = 108
    object miShowEvents: TMenuItem
      Action = actShowEvents
    end
    object miRunOrders: TMenuItem
      Action = actRunOrders
    end
    object miRunAutoOrders: TMenuItem
      Action = actRunAutoOrders
    end
    object miTownMode: TMenuItem
      Action = actTownMode
    end
    object miProdMode: TMenuItem
      Action = actProdMode
    end
    object miWedgeMode: TMenuItem
      Action = actWedgeMode
    end
  end
  object pmInfMapM: TPopupMenu
    Left = 541
    Top = 144
    object Wages1: TMenuItem
      Caption = 'Wages'
      Checked = True
      GroupIndex = 1
      RadioItem = True
      OnClick = Entertaimens1Click
    end
    object Work1: TMenuItem
      Tag = 2
      Caption = 'Max wages'
      GroupIndex = 1
      RadioItem = True
      OnClick = Entertaimens1Click
    end
    object Taxincome1: TMenuItem
      Tag = 1
      Caption = 'Tax available'
      GroupIndex = 1
      RadioItem = True
      OnClick = Entertaimens1Click
    end
    object Entertaimens1: TMenuItem
      Tag = 3
      Caption = 'Entertaimen available'
      GroupIndex = 1
      RadioItem = True
      OnClick = Entertaimens1Click
    end
  end
  object pmSelFlead: TPopupMenu
    Left = 216
    Top = 64
    object miFlead0: TMenuItem
      AutoCheck = True
      Caption = 'All'
      Checked = True
      GroupIndex = 1
      RadioItem = True
      OnClick = miFlead2Click
    end
    object miFlead1: TMenuItem
      Tag = 1
      AutoCheck = True
      Caption = 'Only Leaders'
      GroupIndex = 1
      RadioItem = True
      OnClick = miFlead2Click
    end
    object miFlead2: TMenuItem
      Tag = 2
      AutoCheck = True
      Caption = 'Not leaders'
      GroupIndex = 1
      RadioItem = True
      OnClick = miFlead2Click
    end
  end
  object pmSelFflag: TPopupMenu
    Left = 248
    Top = 64
    object miNoFF: TMenuItem
      AutoCheck = True
      Caption = 'No filter'
      Checked = True
      GroupIndex = 2
      RadioItem = True
      OnClick = miNoFFClick
    end
    object miSetFF: TMenuItem
      Caption = 'Set Filter'
      GroupIndex = 2
      RadioItem = True
      OnClick = miSetFFClick
    end
  end
  object pmSelMO: TPopupMenu
    Left = 288
    Top = 64
    object miNofilterMO: TMenuItem
      AutoCheck = True
      Caption = 'No filter'
      Checked = True
      GroupIndex = 8
      RadioItem = True
      OnClick = miFiltMO
    end
    object miNoOrder: TMenuItem
      Tag = -1
      AutoCheck = True
      Caption = 'No monthlong order'
      GroupIndex = 8
      RadioItem = True
      OnClick = miFiltMO
    end
    object miAdvMO: TMenuItem
      AutoCheck = True
      Caption = 'Advance'
      GroupIndex = 8
      RadioItem = True
      OnClick = miFiltMO
    end
    object miBuiMO: TMenuItem
      AutoCheck = True
      Caption = 'Build'
      GroupIndex = 8
      RadioItem = True
      OnClick = miFiltMO
    end
    object miEntMO: TMenuItem
      AutoCheck = True
      Caption = 'Entertain'
      GroupIndex = 8
      RadioItem = True
      OnClick = miFiltMO
    end
    object miMovMO: TMenuItem
      AutoCheck = True
      Caption = 'Move'
      GroupIndex = 8
      RadioItem = True
      OnClick = miFiltMO
    end
    object miProMO: TMenuItem
      AutoCheck = True
      Caption = 'Produce'
      GroupIndex = 8
      RadioItem = True
      OnClick = miFiltMO
    end
    object miSaiMO: TMenuItem
      AutoCheck = True
      Caption = 'Sail'
      GroupIndex = 8
      RadioItem = True
      OnClick = miFiltMO
    end
    object miStuMO: TMenuItem
      AutoCheck = True
      Caption = 'Study'
      GroupIndex = 8
      RadioItem = True
      OnClick = miFiltMO
    end
    object miTeaMO: TMenuItem
      AutoCheck = True
      Caption = 'Teach'
      GroupIndex = 8
      RadioItem = True
      OnClick = miFiltMO
    end
    object miWorMO: TMenuItem
      AutoCheck = True
      Caption = 'Work'
      GroupIndex = 8
      RadioItem = True
      OnClick = miFiltMO
    end
  end
end
