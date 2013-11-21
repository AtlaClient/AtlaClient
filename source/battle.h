//---------------------------------------------------------------------------

#ifndef battleH
#define battleH
//---------------------------------------------------------------------------
class ASoldier;
class AArmy;
class ABattleReport;
class ABattleRepRound;
class ABattle{
public:
/*    AArmy *attarmy;
    AArmy *defarmy;
    TMemoryStream *report;

    ARegion *reg;*/

    ABattle();
    ~ABattle();
    void FreeRound(ABattleRepRound *rep,AArmy *att,AArmy *def);
    void NormalRound(ABattleRepRound *rep,AArmy *a,AArmy *b);
    void DoAttack(ABattleRepRound *rep,int round, ASoldier * a, AArmy * atts, AArmy * defs, bool behind);
    void Run(ABattleReport *rep/*ARegion * reg, AUnits * atts, AUnits * defs*/);
//    void AddLine(AnsiString str);
//    void WriteSides(AUnits *atts,AUnits *defs);
    void GetSpoils(AUnits * losers, AItems * spoils);
    void DoSpecialAttack(ABattleRepRound *rep,int round, ASoldier * a, AArmy * atts, AArmy * defs, bool behind);
    void UpdateShields(ABattleRepRound *rep,AArmy *a);

};

#endif
