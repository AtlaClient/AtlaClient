//---------------------------------------------------------------------------
#ifndef skillsH
#define skillsH
//---------------------------------------------------------------------------
class ASkill{
public:
    ASkillType *type;
    int days;
    ASkill();
    virtual __fastcall ~ASkill();
    void Read(TFile& in);
    void Write(TFile & out);
    void CreateNew(ASkill * newskill);
    void SetType(AnsiString abr, bool addunknown=false);
    AnsiString Print(int mode=15);
    int GetLevel();
    void SetLevel(int lev);
    int GetSize();
    int static LevelToDays(int);
    int static DaysToLevel(int);

};
class ASkills:TList{
public:
    ASkills();
    virtual __fastcall ~ASkills();
    void __fastcall Clear();
    void Read(TFile& in);
    void Write(TFile& out);
    ASkill * Get(int index);
    void Add(ASkill * mark);
    ASkill * GetSkill(AnsiString type);
    ASkill * GetSkill(ASkillType *type);
    void CreateNew(ASkills * prvmarks);
    void Update(ASkills * newmarks);
    void UpdateFromPrev(ASkills * prvmarks);
    AnsiString Print(int mode=15);
    void DeleteEmpty();
    int GetSize();

    __property int count  = { read = Getcount };
    __property List;
private:
    int __fastcall Getcount(){return Count;}
};


#endif
