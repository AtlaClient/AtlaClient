//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
USEFORM("source\WedgeModeSetupFrm.cpp", WedgeModeSetupForm);
USEFORM("source\AboutFrm.cpp", AboutForm);
USEFORM("source\AnalyzeCityRegion.cpp", AnalyzeCityRegionFrm);
USEFORM("source\BalanceFrm.cpp", BalanceForm);
USEFORM("source\BattleSimFrm.cpp", BattleSimForm);
USEFORM("source\ChangeAtt.cpp", fChAtt);
USEFORM("source\ChangeGameFrm.cpp", ChangeGameForm);
USEFORM("source\EditArmor.cpp", fEditArmor);
USEFORM("source\EditObjTypeFrm.cpp", EditObjTypeForm);
USEFORM("source\EditOrderAbrFrm.cpp", EditOrderAbrForm);
USEFORM("source\EditOrderAssassinateFrm.cpp", EditOrderAssassinateForm);
USEFORM("source\EditOrderAttackFrm.cpp", EditOrderAttackForm);
USEFORM("source\EditOrderAutoGiveFrm.cpp", EditOrderAutoGiveForm);
USEFORM("source\EditOrderAutoMoveFrm.cpp", EditOrderAutoMoveForm);
USEFORM("source\EditOrderAutoWarningFrm.cpp", EditOrderAutoWarningForm);
USEFORM("source\EditOrderBuild.cpp", EditOrderBuildForm);
USEFORM("source\EditOrderBuySellFrm.cpp", EditOrderBuySellForm);
USEFORM("source\EditOrderByteFrm.cpp", EditOrderByteForm);
USEFORM("source\EditOrderCastFrm.cpp", EditOrderCastForm);
USEFORM("source\EditOrderCommentFrm.cpp", EditOrderCommentForm);
USEFORM("source\EditOrderCustomFrm.cpp", EditOrderCustomForm);
USEFORM("source\EditOrderDescrFrm.cpp", EditOrderDescrForm);
USEFORM("source\EditOrderEnterFrm.cpp", EditOrderEnterForm);
USEFORM("source\EditOrderGiveFrm.cpp", EditOrderGiveForm);
USEFORM("source\EditOrderIntFrm.cpp", EditOrderIntForm);
USEFORM("source\EditOrderMovingFrm.cpp", EditOrderMovingForm);
USEFORM("source\EditOrderNameDescribeFrm.cpp", EditOrderNameDescribeForm);
USEFORM("source\EditOrderPromoteFrm.cpp", EditOrderPromoteForm);
USEFORM("source\EditOrderReceiveFrm.cpp", EditOrderReceiveForm);
USEFORM("source\EditOrdersFrm.cpp", EditOrdersForm);
USEFORM("source\EditOrderSimpleFrm.cpp", EditOrderSimpleForm);
USEFORM("source\EditOrderStealFrm.cpp", EditOrderStealForm);
USEFORM("source\EditOrderTeachFrm.cpp", EditOrderTeachForm);
USEFORM("source\EditOrderWithdrawFrm.cpp", EditOrderWithdrawForm);
USEFORM("source\EditSkillTypeFrm.cpp", EditSkillTypeForm);
USEFORM("source\EditTurnOrder.cpp", fEditTurnOrder);
USEFORM("source\EditWeap.cpp", fEditWeap);
USEFORM("source\EventsFrm.cpp", EventsForm);
USEFORM("source\ExpressionEditorFrm.cpp", ExpressionEditorForm);
USEFORM("source\FormNewUnitFrm.cpp", FormNewUnitForm);
USEFORM("source\gameparamsfrm.cpp", GameParamsForm);
USEFORM("source\GetItem.cpp", fGetItem);
USEFORM("source\LinkShaftFrm.cpp", LinkShaftForm);
USEFORM("source\MainFrm.cpp", AtlaForm);
USEFORM("source\MapFrm.cpp", MapForm);
USEFORM("source\NewGameFrm.cpp", NewGameForm);
USEFORM("source\OptionsFrm.cpp", OptionsForm);
USEFORM("source\ProdModeSetupFrm.cpp", ProdModeSetupForm);
USEFORM("source\ProductsFrm.cpp", ProductsForm);
USEFORM("source\RegInfoFrm.cpp", RegInfoForm);
USEFORM("source\RegionListFrm.cpp", RegionListForm);
USEFORM("source\ReqFrm.cpp", ReqForm);
USEFORM("source\SaveMapFrm.cpp", SaveMapForm);
USEFORM("source\SetFlagsFrm.cpp", SetFlagsForm);
USEFORM("source\SetLevelSizeFrm.cpp", SetLevelSizeForm);
USEFORM("source\ShortCutsFrm.cpp", ShortCutsForm);
USEFORM("source\ShowItemTypesFrm.cpp", ShowItemTypesForm);
USEFORM("source\ShowItObjInfosFrm.cpp", ShowItObjInfosForm);
USEFORM("source\ShowObjTypesFrm.cpp", ShowObjTypesForm);
USEFORM("source\ShowWeatherRegionsFrm.cpp", ShowWeatherRegionsForm);
USEFORM("source\StoreFrm.cpp", StoreForm);
USEFORM("source\SupportBuildingFrm.cpp", SupportBuildingForm);
USEFORM("source\TownTrade.cpp", fTownTrade);
USEFORM("source\ViewChangesFrm.cpp", ViewChangesForm);
USEFORM("source\ViewFactionsFrm.cpp", ViewFactionsForm);
USEFORM("source\ViewGatesFrm.cpp", ViewGatesForm);
USEFORM("source\ViewMagesFrm.cpp", ViewMagesForm);
USEFORM("source\ViewSpecialistsFrm.cpp", ViewSpecialistsForm);
USEFORM("source\ViewTaxRegionsFrm.cpp", ViewTaxRegionsForm);
USEFORM("source\ViewUnitsFrm.cpp", ViewUnitsForm);
USEFORM("source\ViewUnitsSetupFrm.cpp", ViewUnitsSetupForm);
USEFORM("source\FlagFiltr.cpp", fFlagFiltr);
USEFORM("source\EditItems.cpp", fEditItems);
USEFORM("source\SkillsTree.cpp", fSkillsTree);
USEFORM("source\GetSkilLv.cpp", fGetSkilLv);
USEFORM("source\EditItemTypeFrm.cpp", EditItemTypeForm);
USEFORM("source\SelEditScr.cpp", fSelEditScr);
USEFORM("source\EditScript.cpp", fEditScript);
USEFORM("source\EditOrderAutoFollow.cpp", EditOrderAutoFollowForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->Title = "AtlaClient";
		Application->CreateForm(__classid(TAtlaForm), &AtlaForm);
		Application->CreateForm(__classid(TEventsForm), &EventsForm);
		Application->CreateForm(__classid(TRegInfoForm), &RegInfoForm);
		Application->CreateForm(__classid(TMapForm), &MapForm);
		Application->CreateForm(__classid(TBattleSimForm), &BattleSimForm);
		Application->CreateForm(__classid(TViewChangesForm), &ViewChangesForm);
		Application->CreateForm(__classid(TRegionListForm), &RegionListForm);
		Application->CreateForm(__classid(TReqForm), &ReqForm);
		Application->CreateForm(__classid(TfChAtt), &fChAtt);
		Application->CreateForm(__classid(TfGetItem), &fGetItem);
		Application->CreateForm(__classid(TRegInfoForm), &RegInfoForm);
		Application->CreateForm(__classid(TBattleSimForm), &BattleSimForm);
		Application->CreateForm(__classid(TfChAtt), &fChAtt);
		Application->CreateForm(__classid(TfGetItem), &fGetItem);
		Application->CreateForm(__classid(TMapForm), &MapForm);
		Application->CreateForm(__classid(TReqForm), &ReqForm);
		Application->CreateForm(__classid(TViewChangesForm), &ViewChangesForm);
		Application->CreateForm(__classid(TEditItemTypeForm), &EditItemTypeForm);
		Application->CreateForm(__classid(TfEditScript), &fEditScript);
		Application->Run();
  }
  catch (Exception &exception)
  {
    Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------
