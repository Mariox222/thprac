﻿#include "thprac_utils.h"

namespace THPrac {
namespace TH13 {
    using std::pair;
    struct THPracParam {
        int32_t mode;
        int32_t stage;
        int32_t section;
        int32_t phase;
        int64_t score;
        int32_t life;
        int32_t extend;
        int32_t life_fragment;
        int32_t bomb;
        int32_t bomb_fragment;
        int32_t power;
        int32_t value;
        int32_t graze;
        int32_t trance_meter;

        bool bossrush_midboss;

        void Reset()
        {
            memset(this, 0, sizeof(THPracParam));
            DataBatchResetRem();
        }
        bool ReadJson(std::string& json)
        {
            ParseJson();

            ForceJsonValue(game, "th13");
            GetJsonValue(mode);
            GetJsonValue(stage);
            GetJsonValue(section);
            GetJsonValue(phase);

            GetJsonValue(score);
            GetJsonValue(life);
            GetJsonValue(extend);
            GetJsonValue(life_fragment);
            GetJsonValue(bomb);
            GetJsonValue(bomb_fragment);
            GetJsonValue(power);
            GetJsonValue(value);
            GetJsonValue(graze);
            GetJsonValue(trance_meter);

            return true;
        }
        std::string GetJson()
        {
            CreateJson();

            AddJsonValueEx(version, GetVersionStr(), jalloc);
            AddJsonValueEx(game, "th13", jalloc);
            AddJsonValue(mode);
            AddJsonValue(stage);
            if (section)
                AddJsonValue(section);
            if (phase)
                AddJsonValue(phase);

            AddJsonValue(score);
            AddJsonValue(life);
            AddJsonValue(extend);
            AddJsonValue(life_fragment);
            AddJsonValue(bomb);
            AddJsonValue(bomb_fragment);
            AddJsonValue(power);
            AddJsonValue(value);
            AddJsonValue(graze);
            AddJsonValue(trance_meter);

            ReturnJson();
        }
    };
    THPracParam thPracParam {};

    class THGuiPrac : public Gui::GameGuiWnd {
        THGuiPrac() noexcept
        {
            *mMode = 1;
            *mLife = 9;
            *mBomb = 9;
            *mPower = 400;
            *mValue = 10000;
            *mTranceMeter = 200;

            //SetWndFlag(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
            SetFade(0.8f, 0.1f);
            SetStyle(ImGuiStyleVar_WindowRounding, 0.0f);
            SetStyle(ImGuiStyleVar_WindowBorderSize, 0.0f);
        }
        SINGLETON(THGuiPrac);
    public:

        int GetStage()
        {
            return *mStage;
        }

        __declspec(noinline) void State(int state)
        {
            switch (state) {
            case 0:
                break;
            case 1:
                mDiffculty = *((int32_t*)0x4be7c4);
                SetFade(0.8f, 0.1f);
                Open();
                thPracParam.Reset();
            case 2:
                break;
            case 3:
                SetFade(0.8f, 0.1f);
                Close();

                // Fill Param
                thPracParam.mode = *mMode;
                thPracParam.stage = *mStage;
                thPracParam.section = CalcSection();
                thPracParam.phase = SpellPhase() ? *mPhase : 0;

                thPracParam.score = *mScore;
                thPracParam.life = *mLife;
                thPracParam.extend = *mExtend;
                thPracParam.life_fragment = *mLifeFragment;
                thPracParam.bomb = *mBomb;
                thPracParam.bomb_fragment = *mBombFragment;
                thPracParam.power = *mPower;
                thPracParam.value = *mValue;
                thPracParam.graze = *mGraze;
                thPracParam.trance_meter = *mTranceMeter;

                thPracParam.bossrush_midboss = *mBRMidBoss;
                break;
            case 4:
                Close();
                *mNavFocus = 0;
                break;
            default:
                break;
            }
        }

        bool LoadParam(std::string& json)
        {
            ParseJsonNoReset();

            ForceJsonValue(game, "th13");
            GetJsonValueAlt(mode, *mMode, Int);
            GetJsonValueAlt(stage, *mStage, Int);
            GetJsonValueAlt(warp, *mWarp, Int);
            GetJsonValueAlt(section, *mSection, Int);
            GetJsonValueAlt(phase, *mPhase, Int);
            GetJsonValueAlt(chapter, *mChapter, Int);

            GetJsonValueAlt(score, *mScore, Int64);
            GetJsonValueAlt(life, *mLife, Int);
            GetJsonValueAlt(extend, *mExtend, Int);
            GetJsonValueAlt(life_fragment, *mLifeFragment, Int);
            GetJsonValueAlt(bomb, *mBomb, Int);
            GetJsonValueAlt(bomb_fragment, *mBombFragment, Int);
            GetJsonValueAlt(power, *mPower, Int);
            GetJsonValueAlt(value, *mValue, Int);
            GetJsonValueAlt(graze, *mGraze, Int);
            GetJsonValueAlt(trance_meter, *mTranceMeter, Int);

            return true;
        }
        std::string GetParam()
        {
            CreateJson();

            AddJsonValueEx(version, GetVersionStr(), jalloc);
            AddJsonValueEx(game, "th13", jalloc);
            AddJsonValueEx(mode, *mMode);
            AddJsonValueEx(stage, *mStage);
            AddJsonValueEx(warp, *mWarp);
            AddJsonValueEx(section, *mSection);
            AddJsonValueEx(phase, *mPhase);
            AddJsonValueEx(chapter, *mChapter);

            AddJsonValueEx(score, *mScore);
            AddJsonValueEx(life, *mLife);
            AddJsonValueEx(extend, *mExtend);
            AddJsonValueEx(life_fragment, *mLifeFragment);
            AddJsonValueEx(bomb, *mBomb);
            AddJsonValueEx(bomb_fragment, *mBombFragment);
            AddJsonValueEx(power, *mPower);
            AddJsonValueEx(value, *mValue);
            AddJsonValueEx(graze, *mGraze);
            AddJsonValueEx(trance_meter, *mTranceMeter);

            ReturnJson();
        }
    protected:
        virtual void OnLocaleChange() override
        {
            SetTitle(XSTR(TH_MENU));
            switch (Gui::LocaleGet()) {
            case Gui::LOCALE_ZH_CN:
                AutoSize(ImVec2(0.0f, 0.0f),
                    XSSS(3)[TH13_ST7_END_S1], nullptr, XSTR(TH_BOMB_FRAGMENT), nullptr,
                    15.5f, ImVec2(-1.0f, 450.0f), ImVec2(370.0f, -1.0f));
                AutoPos(0.9f, 0.65f);
                break;
            case Gui::LOCALE_EN_US:
                AutoSize(ImVec2(0.0f, 0.0f),
                    XSSS(3)[TH13_ST1_BOSS2], nullptr, XSTR(TH13_TRANCE_METER), nullptr,
                    15.5f, ImVec2(-1.0f, 450.0f), ImVec2(-1.0f, -1.0f));
                AutoPos(0.9f, 0.65f);
                break;
            case Gui::LOCALE_JA_JP:
                AutoSize(ImVec2(0.0f, 0.0f),
                    XSSS(3)[TH13_ST1_BOSS4], nullptr, XSTR(TH13_EXTEND), nullptr,
                    15.5f, ImVec2(-1.0f, 450.0f), ImVec2(370.0f, -1.0f));
                AutoPos(0.9f, 0.65f);
                break;
            default:
                break;
            }
            return;
        }
        virtual void OnContentUpdate() override
        {
            ImGui::Separator();

            PracticeMenu();
        }
        th_glossary_t* SpellPhase()
        {
            auto section = CalcSection();
            if (section == TH13_ST7_END_S10 || section == TH13_ST6_BOSS11) {
                return TH_SPELL_PHASE1;
            }
            return nullptr;
        }
        void PracticeMenu()
        {
            mMode();
            if (mStage())
                *mSection = *mChapter = 0;
            if (*mMode == 1) {
                if (mWarp())
                    *mSection = *mChapter = *mPhase = 0;
                if (*mWarp) {
                    SectionWidget();
                    mPhase(TH_PHASE, SpellPhase());
                }

                mLife();
                if (mExtend())
                    mLifeFragment.SetBound(0, ((int*)0x4bb994)[*mExtend] - 1);
                mLifeFragment();
                mBomb();
                mBombFragment();
                auto power_str = std::to_string((float)(*mPower) / 100.0f).substr(0, 4);
                mPower(power_str.c_str());
                auto trance_str = std::to_string((float)(*mTranceMeter) / 200.0f).substr(0, 4);
                mTranceMeter(trance_str.c_str());
                mValue();
                mValue.RoundDown(10);
                mGraze();
                mScore();
                mScore.RoundDown(10);
            } else if (*mMode == 2) {
                auto power_str = std::to_string((float)(*mPower) / 100.0f).substr(0, 4);
                mPower(power_str.c_str());
                mBRMidBoss();
                mBRExtra();
            }
            //WndDebugOutput();

            mNavFocus();
        }
        int CalcSection()
        {
            int chapterId = 0;
            switch (*mWarp) {
            case 1: // Chapter
                // Chapter Id = 10000 + Stage * 100 + Section
                chapterId += (*mStage + 1) * 100;
                chapterId += *mChapter;
                chapterId += 10000; // Base of chapter ID is 1000.
                return chapterId;
                break;
            case 2:
            case 3: // Mid boss & End boss
                return th_sections_cba[*mStage][*mWarp - 2][*mSection];
                break;
            case 4:
            case 5: // Non-spell & Spellcard
                return th_sections_cbt[*mStage][*mWarp - 4][*mSection];
                break;
            default:
                return 0;
                break;
            }
        }
        void SectionWidget()
        {
            static char chapterStr[256] {};
            auto& chapterCounts = mChapterSetup[*mStage];

            switch (*mWarp) {
            case 1: // Chapter
                mChapter.SetBound(1, chapterCounts[0] + chapterCounts[1]);

                if (chapterCounts[1] == 0 && chapterCounts[2] != 0) {
                    sprintf_s(chapterStr, XSTR(TH_STAGE_PORTION_N), *mChapter);
                } else if (*mChapter <= chapterCounts[0]) {
                    sprintf_s(chapterStr, XSTR(TH_STAGE_PORTION_1), *mChapter);
                } else {
                    sprintf_s(chapterStr, XSTR(TH_STAGE_PORTION_2), *mChapter - chapterCounts[0]);
                };

                mChapter(chapterStr);
                break;
            case 2:
            case 3: // Mid boss & End boss
                if (mSection(TH_WARP_SELECT[*mWarp],
                        th_sections_cba[*mStage][*mWarp - 2],
                        th_sections_str[::THPrac::Gui::LocaleGet()][mDiffculty]))
                    *mPhase = 0;
                break;
            case 4:
            case 5: // Non-spell & Spellcard
                if (mSection(TH_WARP_SELECT[*mWarp],
                        th_sections_cbt[*mStage][*mWarp - 4],
                        th_sections_str[::THPrac::Gui::LocaleGet()][mDiffculty]))
                    *mPhase = 0;
                break;
            default:
                break;
            }
        }

        Gui::GuiCombo mMode { TH_MODE, TH_MODE_SELECT };
        Gui::GuiCombo mStage { TH_STAGE, TH_STAGE_SELECT };
        Gui::GuiCombo mWarp { TH_WARP, TH_WARP_SELECT };
        Gui::GuiCombo mSection { TH_MODE };
        Gui::GuiCombo mPhase { TH_PHASE };

        Gui::GuiSlider<int, ImGuiDataType_S32> mChapter { TH_CHAPTER, 0, 0 };
        Gui::GuiDrag<int64_t, ImGuiDataType_S64> mScore { TH_SCORE, 0, 9999999990, 10, 100000000 };
        Gui::GuiSlider<int, ImGuiDataType_S32> mLife { TH_LIFE, 0, 9 };
        Gui::GuiSlider<int, ImGuiDataType_S32> mExtend { TH13_EXTEND, 0, 6 };
        Gui::GuiSlider<int, ImGuiDataType_S32> mLifeFragment { TH_LIFE_FRAGMENT, 0, 7 };
        Gui::GuiSlider<int, ImGuiDataType_S32> mBomb { TH_BOMB, 0, 9 };
        Gui::GuiSlider<int, ImGuiDataType_S32> mBombFragment { TH_BOMB_FRAGMENT, 0, 7 };
        Gui::GuiSlider<int, ImGuiDataType_S32> mPower { TH_POWER, 0, 400 };
        Gui::GuiSlider<int, ImGuiDataType_S32> mTranceMeter { TH13_TRANCE_METER, 0, 600, 2, 2 };
        Gui::GuiDrag<int, ImGuiDataType_S32> mValue { TH_VALUE, 0, 999990, 10, 100000 };
        Gui::GuiDrag<int, ImGuiDataType_S32> mGraze { TH_GRAZE, 0, 999999, 1, 100000 };

        Gui::GuiCheckBox mBRMidBoss { TH_BOSSRUSH_MIDBOSS };
        Gui::GuiCheckBox mBRExtra { TH_BOSSRUSH_EXTRA };

        Gui::GuiNavFocus mNavFocus { TH_STAGE, TH_MODE, TH_WARP,
            TH_MID_STAGE, TH_END_STAGE, TH_NONSPELL, TH_SPELL, TH_PHASE, TH_CHAPTER,
            TH_SCORE, TH_LIFE, TH13_EXTEND, TH_LIFE_FRAGMENT, TH_BOMB, TH_BOMB_FRAGMENT,
            TH_POWER, TH_VALUE, TH_GRAZE, TH13_TRANCE_METER,
            TH_BOSSRUSH_MIDBOSS, TH_BOSSRUSH_EXTRA };

        int mChapterSetup[7][2] {
            { 3, 2 },
            { 4, 2 },
            { 4, 4 },
            { 4, 4 },
            { 3, 4 },
            { 4, 0 },
            { 3, 3 },
        };

        int mDiffculty = 0;
    };
    class THGuiRep : public Gui::GameGuiWnd {
        THGuiRep() noexcept
        {
            wchar_t appdata[MAX_PATH];
            GetEnvironmentVariableW(L"APPDATA", appdata, MAX_PATH);
            mAppdataPath = appdata;
        }
        SINGLETON(THGuiRep);
    public:

        void CheckReplay()
        {
            uint32_t index = GetMemContent(0x4c22e0, 0x5aa0);
            char* repName = (char*)GetMemAddr(0x4c22e0, index * 4 + 0x5aa8, 0x220);
            std::wstring repDir(mAppdataPath);
            repDir.append(L"\\ShanghaiAlice\\th13\\replay\\");
            repDir.append(mb_to_utf16(repName));

            std::string param;
            if (ReplayLoadParam(repDir.c_str(), param) && mRepParam.ReadJson(param))
                mParamStatus = true;
            else
                mRepParam.Reset();
        }

        bool mRepStatus = false;
        void State(int state)
        {
            switch (state) {
            case 1:
                mRepStatus = false;
                mParamStatus = false;
                thPracParam.Reset();
                break;
            case 2:
                CheckReplay();
                break;
            case 3:
                mRepStatus = true;
                if (mParamStatus)
                    memcpy(&thPracParam, &mRepParam, sizeof(THPracParam));
                break;
            default:
                break;
            }
        }

    protected:
        std::wstring mAppdataPath;
        bool mParamStatus = false;
        THPracParam mRepParam;
    };
    class THOverlay : public Gui::GameGuiWnd {
        THOverlay() noexcept
        {
            SetTitle("Mod Menu");
            SetFade(0.5f, 0.5f);
            SetPos(10.0f, 10.0f);
            SetSize(0.0f, 0.0f);
            SetWndFlag(
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | 0);
            OnLocaleChange();
        }
        SINGLETON(THOverlay);
    public:

    protected:
        virtual void OnLocaleChange() override
        {
            float x_offset_1 = 0.0f;
            float x_offset_2 = 0.0f;
            switch (Gui::LocaleGet()) {
            case Gui::LOCALE_ZH_CN:
                x_offset_1 = 0.1f;
                x_offset_2 = 0.14f;
                break;
            case Gui::LOCALE_EN_US:
                x_offset_1 = 0.1f;
                x_offset_2 = 0.14f;
                break;
            case Gui::LOCALE_JA_JP:
                x_offset_1 = 0.1f;
                x_offset_2 = 0.14f;
                break;
            default:
                break;
            }

            mMenu.SetTextOffsetRel(x_offset_1, x_offset_2);
            mMuteki.SetTextOffsetRel(x_offset_1, x_offset_2);
            mInfLives.SetTextOffsetRel(x_offset_1, x_offset_2);
            mInfBombs.SetTextOffsetRel(x_offset_1, x_offset_2);
            mInfPower.SetTextOffsetRel(x_offset_1, x_offset_2);
            mTimeLock.SetTextOffsetRel(x_offset_1, x_offset_2);
            mAutoBomb.SetTextOffsetRel(x_offset_1, x_offset_2);
            mElBgm.SetTextOffsetRel(x_offset_1, x_offset_2);
        }
        virtual void OnContentUpdate() override
        {
            mMuteki();
            mInfLives();
            mInfBombs();
            mInfPower();
            mTimeLock();
            mAutoBomb();
            mElBgm();
        }
        virtual void OnPreUpdate() override
        {
            if (mMenu(false) && !ImGui::IsAnyItemActive()) {
                if (*mMenu) {
                    Open();
                } else {
                    Close();
                    //*((int32_t*)0x6c6eb0) = 2;
                }
            }
            //if (*((int32_t*)0x6c6ea4) == 2)
            //	SetPos(500.0f, 300.0f);
            //else
            //	SetPos(10.0f, 10.0f);
        }

        Gui::GuiHotKey mMenu { "ModMenuToggle", "BACKSPACE", VK_BACK };
        Gui::GuiHotKey mMuteki { TH_MUTEKI, "F1", VK_F1, {
            new HookCtxHook((void*)0x444D7F, [](PCONTEXT pCtx) {
                *(uint32_t*)(pCtx->Edi + 0x65C) = 1;
                DataRef<EVENT_MISS>()++;
            }) } };
        Gui::GuiHotKey mInfLives { TH_INFLIVES, "F2", VK_F2, {
            new HookCtxPatch((void*)0x444A52, "\xeb\x06", 2) } };
        Gui::GuiHotKey mInfBombs { TH_INFBOMBS, "F3", VK_F3, {
            new HookCtxPatch((void*)0x40A402, "\x66\x90", 2) } };
        Gui::GuiHotKey mInfPower { TH_INFPOWER, "F4", VK_F4, {
            new HookCtxPatch((void*)0x445A2D, "\xe8", 1) } };
        Gui::GuiHotKey mTimeLock { TH_TIMELOCK, "F5", VK_F5, {
            new HookCtxPatch((void*)0x412D36, "\xeb", 1),
            new HookCtxPatch((void*)0x41AABF, "\x0F\x1F\x44\x00\x00", 5) } };
        Gui::GuiHotKey mAutoBomb { TH_AUTOBOMB, "F6", VK_F6, {
            new HookCtxPatch((void*)0x443525, "\xc6", 1) } };

    public:
        Gui::GuiHotKey mElBgm { TH_EL_BGM, "F7", VK_F7 };
    };
    class THAuxDisp : public Gui::GameGuiWnd {
        THAuxDisp() noexcept
        {
            SetTitle("Aux Disp");
            SetFade(1.0f, 0.1f);
            SetPos(10.0f, 10.0f);
            SetSize(100.0f, 100.0f);
            SetWndFlag(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | 0);

            //SetWndFlag(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
            OnLocaleChange();
        }
        SINGLETON(THAuxDisp);

    public:
    protected:
        virtual void OnLocaleChange() override
        {
            switch (Gui::LocaleGet()) {
            case Gui::LOCALE_ZH_CN:
            case Gui::LOCALE_EN_US:
            case Gui::LOCALE_JA_JP:
                SetPos(424.0f, 40.0f);
                SetSize(210.0f, 430.0f);
                break;
            default:
                break;
            }

        }
        virtual void OnContentUpdate() override
        {
            mStageOffset(false);
            STAGE_OFFSET_INIT(DataBatchCache<0>(false), *mStageOffset);

            //WndDebugOutput();
            PRT_WITH_SO("High Score: %s", FormatScore(STAGE_OFFSET(DATA_SCORE)));
            PRT_WITH_SO("Score: %s", FormatScore(STAGE_OFFSET(DATA_SCORE)));
            ImGui::Separator();

            ImGui::Columns(2);
            ImGui::Text("Life: %d", DataRef<DATA_LIFE>());
            ImGui::NextColumn();
            ImGui::Text("Frag: %d/%d", DataRef<DATA_LIFE_FRAG>(), ((uint32_t*)(0x4bb994))[U32_REF(DATA_EXTEND)]);
            ImGui::NextColumn();
            ImGui::Text("Bomb: %d", DataRef<DATA_BOMB>());
            ImGui::NextColumn();
            ImGui::Text("Frag: %d/8", DataRef<DATA_BOMB_FRAG>());
            ImGui::NextColumn();
            ImGui::Text("Power: %1.2f", ((float)DataRef<DATA_POWER>() / 100.0f));
            ImGui::NextColumn();
            PRT_WITH_SO("Items: %d", STAGE_OFFSET(DATA_ITEM_POWER));
            ImGui::NextColumn();
            PRT_WITH_SO("Value: %d", STAGE_OFFSET(DATA_VALUE) / 100);
            ImGui::NextColumn();
            PRT_WITH_SO("Items: %d", STAGE_OFFSET(DATA_ITEM_POINT));
            ImGui::NextColumn();
            PRT_WITH_SO("Graze: %d", STAGE_OFFSET(DATA_GRAZE));
            ImGui::NextColumn();
            PRT_WITH_SO("Cancel: %d", STAGE_OFFSET(DATA_ITEM_CANCEL));
            ImGui::NextColumn();
            ImGui::Columns();
            ImGui::Separator();

            auto blue = STAGE_OFFSET(DATA_SPIRIT_BLUE);
            auto blueTrance = STAGE_OFFSET(DATA_SPIRIT_BLUE_TRANCE);
            auto blueTotal = blue + blueTrance;
            auto grey = STAGE_OFFSET(DATA_SPIRIT_GREY);
            auto greyTrance = STAGE_OFFSET(DATA_SPIRIT_GREY_TRANCE);
            auto greyTotal = grey + greyTrance;
            auto life = STAGE_OFFSET(DATA_SPIRIT_LIFE);
            auto lifeTrance = STAGE_OFFSET(DATA_SPIRIT_LIFE_TRANCE);
            auto lifeTotal = life + lifeTrance;
            auto bomb = STAGE_OFFSET(DATA_SPIRIT_BOMB);
            auto bombTrance = STAGE_OFFSET(DATA_SPIRIT_BOMB_TRANCE);
            auto bombTotal = bomb + bombTrance;
            PRT_WITH_SO("Blue: %d (%d, %.2f%%)", blueTotal, blueTrance, (blueTotal) ? (float)(blueTrance) / (float)(blueTotal)*100.0f : 0.0f);
            PRT_WITH_SO("Grey: %d (%d, %.2f%%)", greyTotal, greyTrance, (greyTotal) ? (float)(greyTrance) / (float)(greyTotal)*100.0f : 0.0f);
            PRT_WITH_SO("Purple: %d (%d, %.2f%%)", lifeTotal, lifeTrance, (lifeTotal) ? (float)(lifeTrance) / (float)(lifeTotal)*100.0f : 0.0f);
            PRT_WITH_SO("Green: %d (%d, %.2f%%)", bombTotal, bombTrance, (bombTotal) ? (float)(bombTrance) / (float)(bombTotal)*100.0f : 0.0f);
            PRT_WITH_SO("Active trance: %d", STAGE_OFFSET(EVENT_TRANCE_ACTIVE));
            PRT_WITH_SO("Passive trance: %d", STAGE_OFFSET(EVENT_TRANCE_PASSIVE));
            ImGui::Separator();

            ImGui::Columns(2);
            PRT_WITH_SO("Miss: %d", STAGE_OFFSET(EVENT_MISS));
            ImGui::NextColumn();
            ImGui::Text("%df", DataRef<DATA_FRAME>());
            ImGui::NextColumn();
            PRT_WITH_SO("Bomb: %d", STAGE_OFFSET(EVENT_BOMB) + STAGE_OFFSET(EVENT_DEATH_BOMB));
            ImGui::NextColumn();
            PRT_WITH_SO("D. Bomb: %d", STAGE_OFFSET(EVENT_DEATH_BOMB));
            ImGui::NextColumn();
            ImGui::Text("SC Enter: %d", DataRef<EVENT_SPELLCARD_ENTER>());
            ImGui::NextColumn();
            ImGui::Text("SC Get: %d", DataRef<EVENT_SPELLCARD_GET>());
            ImGui::Columns();
            ImGui::Separator();

            //ImGui::Text("Chapter NM Rate: %d/%d", 1, 1);
            //ImGui::Separator();

            InputDispWidget(0, 1, 3, 4, 5, 6, 7, 9);
        }
        virtual void OnPreUpdate() override
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
            if (!DataRef<SYSFLAG_INGAME>()) {
                Close();
                return;
            }
            if (mAuxDisp(false) && !ImGui::IsAnyItemActive()) {
                if (*mAuxDisp) {
                    Open();
                } else {
                    Close();
                }
            }
        }
        virtual void OnPostUpdate() override
        {
            ImGui::PopStyleColor();
        }

        Gui::GuiHotKey mAuxDisp { "AuxDisp", "", 'L' };
        Gui::GuiHotKey mStageOffset { "StageOffset", "", 'M' };
    };

    class THAdvOptWnd : public Gui::PPGuiWnd {
        EHOOK_ST(th13_all_clear_bonus_1, (void*)0x42ce28)
        {
            pCtx->Eip = 0x42ce33;
        }
        EHOOK_ST(th13_all_clear_bonus_2, (void*)0x42cf1b)
        {
            *(int32_t*)(GetMemAddr(0x4c2190, 0x144)) = *(int32_t*)(0x4be7c0);
            if (GetMemContent(0x4be830) & 0x10) {
                typedef void (*PScoreFunc)();
                PScoreFunc a = (PScoreFunc)0x43f720;
                a();
                pCtx->Eip = 0x42ce2d;
            }
        }
        EHOOK_ST(th13_all_clear_bonus_3, (void*)0x42d004)
        {
            *(int32_t*)(GetMemAddr(0x4c2190, 0x144)) = *(int32_t*)(0x4be7c0);
            if (GetMemContent(0x4be830) & 0x10) {
                typedef void (*PScoreFunc)();
                PScoreFunc a = (PScoreFunc)0x43f720;
                a();
                pCtx->Eip = 0x42ce2d;
            }
        }
    private:
        void VPResetFPS(int32_t FPS)
        {
            if (*(int32_t*)(mOptCtx.vpatch_base + 0x16a8c) != FPS) {
                LARGE_INTEGER PerformanceCount;
                auto flag = *(uint32_t*)(mOptCtx.vpatch_base + 0x16A88);
                if (flag > 0x101 || flag < 0x100 && flag - 1 > 2) {
                    *(uint32_t*)(mOptCtx.vpatch_base + 0x18f18) = 0;
                    *(uint32_t*)(mOptCtx.vpatch_base + 0x18f1c) = 0;
                    QueryPerformanceCounter(&PerformanceCount);
                    *(uint64_t*)(mOptCtx.vpatch_base + 0x18f10) = PerformanceCount.QuadPart;
                    *(uint64_t*)(mOptCtx.vpatch_base + 0x18f08) = PerformanceCount.QuadPart;
                }
            }
            *(int32_t*)(mOptCtx.vpatch_base + 0x1a034) = *(int32_t*)(mOptCtx.vpatch_base + 0x16a8c) = FPS;
        }
        void FpsInit()
        {
            mOptCtx.fps_debug_acc = 1;
            mOptCtx.fps_replay_fast = 600;

            if (mOptCtx.vpatch_base = (uintptr_t)GetModuleHandleW(L"openinputlagpatch.dll")) {
                OILPInit(mOptCtx);
            } else if (mOptCtx.vpatch_base = (uintptr_t)GetModuleHandleW(L"vpatch_th13.dll")) {
                uint64_t hash[2];
                CalcFileHash(L"vpatch_th13.dll", hash);
                if (hash[0] != 6450385832836080372ll || hash[1] != 579365625616419970ll)
                    mOptCtx.fps_status = -1;
                else if (*(int32_t*)(mOptCtx.vpatch_base + 0x1a024) == 0) {
                    mOptCtx.fps_status = 2;
                    mOptCtx.fps = *(int32_t*)(mOptCtx.vpatch_base + 0x1a034);
                    mOptCtx.fps_replay_slow = *(int32_t*)(mOptCtx.vpatch_base + 0x1a03c);
                }
            } else if (*(uint8_t*)0x4dc895 == 3) {
                mOptCtx.fps_status = 1;

                DWORD oldProtect;
                VirtualProtect((void*)0x45d36a, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
                *(double**)0x45d36a = &mOptCtx.fps_dbl;
                VirtualProtect((void*)0x45d36a, 4, oldProtect, &oldProtect);
            } else
                mOptCtx.fps_status = 0;
        }
        void FpsSet()
        {
            if (mOptCtx.fps_status == 3) {
                mOptCtx.oilp_set_game_fps(mOptCtx.fps);
                mOptCtx.oilp_set_replay_skip_fps(mOptCtx.fps_replay_fast);
                mOptCtx.oilp_set_replay_slow_fps(mOptCtx.fps_replay_slow);
            }
            else if (mOptCtx.fps_status == 1) {
                mOptCtx.fps_dbl = 1.0 / (double)mOptCtx.fps;
            } else if (mOptCtx.fps_status == 2) {
                VPResetFPS(mOptCtx.fps);
                *(int32_t*)(mOptCtx.vpatch_base + 0x1a03c) = mOptCtx.fps_replay_slow;
            }
        }
        void GameplayInit()
        {
            th13_all_clear_bonus_1.Setup();
            th13_all_clear_bonus_2.Setup();
            th13_all_clear_bonus_3.Setup();
        }
        void GameplaySet()
        {
            th13_all_clear_bonus_1.Toggle(mOptCtx.all_clear_bonus);
            th13_all_clear_bonus_2.Toggle(mOptCtx.all_clear_bonus);
            th13_all_clear_bonus_3.Toggle(mOptCtx.all_clear_bonus);
        }
        void DatRecInit()
        {
            mOptCtx.data_rec_func = [&](std::vector<RecordedValue>& values) {
                return DataRecFunc(values);
            };
            wchar_t appdata[MAX_PATH];
            GetEnvironmentVariableW(L"APPDATA", appdata, MAX_PATH);
            mOptCtx.data_rec_dir = appdata;
            mOptCtx.data_rec_dir += L"\\ShanghaiAlice\\th13\\replay\\";
        }
        void DataRecPreUpd()
        {
            DataRecOpt(mOptCtx, true, DataRef<SYSFLAG_INGAME>());
        }
        void DataRecFunc(std::vector<RecordedValue>& values)
        {
            values.clear();
            values.emplace_back("Score", (int64_t) * (int32_t*)(0x4be7c0) * 10ll);
            values.emplace_back("Graze", *(int32_t*)(0x4be7d0));
            values.emplace_back("Value", ((float)*(int32_t*)(0x4be7dc) / 100.0f), "%.2f");
        }
        void DataRecMenu()
        {
            if (DataRecOpt(mOptCtx)) {
                SetContentUpdFunc([&]() { ContentUpdate(); });
            }
        }

        THAdvOptWnd() noexcept
        {
            SetWndFlag(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
            SetFade(0.8f, 0.8f);
            SetStyle(ImGuiStyleVar_WindowRounding, 0.0f);
            SetStyle(ImGuiStyleVar_WindowBorderSize, 0.0f);

            InitUpdFunc([&]() { ContentUpdate(); },
                [&]() { LocaleUpdate(); },
                [&]() { PreUpdate(); },
                []() {});

            OnLocaleChange();
            FpsInit();
            GameplayInit();
            DatRecInit();
        }
        SINGLETON(THAdvOptWnd);

    public:
        void FpsUpd()
        {
            auto res = FPSHelper(mOptCtx, THGuiRep::singleton().mRepStatus, false, true,
                [](int32_t fps) { THAdvOptWnd::singleton().VPResetFPS(fps); });
            if (res) {

            }
        }
    public:
        __declspec(noinline) static bool StaticUpdate()
        {
            auto& advOptWnd = THAdvOptWnd::singleton();

            if (Gui::KeyboardInputUpdate(VK_F12) == 1) {
                if (advOptWnd.IsOpen())
                    advOptWnd.Close();
                else
                    advOptWnd.Open();
            }
            advOptWnd.Update();

            return advOptWnd.IsOpen();
        }

    protected:
        void LocaleUpdate()
        {
            SetTitle(XSTR(TH_SPELL_PRAC));
            switch (Gui::LocaleGet()) {
            case Gui::LOCALE_ZH_CN:
                SetSizeRel(1.0f, 1.0f);
                SetPosRel(0.0f, 0.0f);
                SetItemWidthRel(-0.0f);
                SetAutoSpacing(true);
                break;
            case Gui::LOCALE_EN_US:
                SetSizeRel(1.0f, 1.0f);
                SetPosRel(0.0f, 0.0f);
                SetItemWidthRel(-0.0f);
                SetAutoSpacing(true);
                break;
            case Gui::LOCALE_JA_JP:
                SetSizeRel(1.0f, 1.0f);
                SetPosRel(0.0f, 0.0f);
                SetItemWidthRel(-0.0f);
                SetAutoSpacing(true);
                break;
            default:
                break;
            }
        }
        void ContentUpdate()
        {
            ImGui::Text(XSTR(TH_ADV_OPT));
            ImGui::Separator();
            ImGui::BeginChild("Adv. Options", ImVec2(0.0f, 0.0f));

            if (BeginOptGroup<TH_GAME_SPEED>()) {
                if (GameFPSOpt(mOptCtx))
                    FpsSet();
                EndOptGroup();
            }
            if (BeginOptGroup<TH_GAMEPLAY>()) {
                if (GameplayOpt(mOptCtx))
                    GameplaySet();
                EndOptGroup();
            }
            if (BeginOptGroup<TH_DATANLY>()) {
                if (ImGui::Button(XSTR(TH_DATANLY_BUTTON))) {
                    SetContentUpdFunc([&]() { DataRecMenu(); });
                }
                EndOptGroup();
            }

            AboutOpt();
            ImGui::EndChild();
            ImGui::SetWindowFocus();
        }
        void PreUpdate()
        {
            DataRecPreUpd();
        }

        adv_opt_ctx mOptCtx;
    };

    void ECLJump(ECLHelper& ecl, unsigned int start, unsigned int dest, int at_frame, int ecl_time = 0)
    {
        if (start) {
            ecl.SetPos(start);
        } else {
            start = ecl.GetPos();
        }
        ecl << ecl_time << 0x0018000C << 0x02ff0000 << 0x00000000 << dest - start << at_frame;
    }
    __declspec(noinline) void THStageWarp(ECLHelper& ecl, int stage, int portion)
    {
        if (stage == 1) {
            switch (portion) {
            case 1:
                break;
            case 2:
                ECLJump(ecl, 0x8a70, 0x8c30, 60, 90); // 0x8c94
                ECLJump(ecl, 0x540c, 0x5440, 0, 0);
                break;
            case 3:
                ECLJump(ecl, 0x8a70, 0x8c30, 60, 90); // 0x8c94
                ECLJump(ecl, 0x540c, 0x54a8, 0, 0);
                break;
            case 4:
                ECLJump(ecl, 0x8a70, 0x8c94, 60, 90); 
                break;
            case 5:
                ECLJump(ecl, 0x8a70, 0x8c94, 60, 90);
                ECLJump(ecl, 0x5510, 0x5540, 0, 0);
                break;
            default:
                break;
            }
        } else if (stage == 2) {
            switch (portion) {
            case 1:
                break;
            case 2:
                ECLJump(ecl, 0x7718, 0x79ac, 60, 90); // 0x7a00
                ECLJump(ecl, 0x3d9c, 0x3dd0, 0, 0);
                break;
            case 3:
                ECLJump(ecl, 0x7718, 0x79ac, 60, 90); // 0x7a00
                ECLJump(ecl, 0x3d9c, 0x3e04, 0, 0);
                break;
            case 4:
                ECLJump(ecl, 0x7718, 0x79ac, 60, 90); // 0x7a00
                ECLJump(ecl, 0x3d9c, 0x3e5c, 0, 0);
                break;
            case 5:
                ECLJump(ecl, 0x7718, 0x79cc, 60, 90);
                ECLJump(ecl, 0x4078, 0x40d0, 0, 0);
                break;
            case 6:
                ECLJump(ecl, 0x7718, 0x7a00, 60, 90);
                break;
            case 7:
                ECLJump(ecl, 0x7718, 0x7a00, 60, 90);
                break;
            default:
                break;
            }
        } else if (stage == 3) {
            switch (portion) {
            case 1:
                break;
            case 2:
                ECLJump(ecl, 0x67b0, 0x6a44, 60, 90); // 0x6a64, 0x6a98
                ECLJump(ecl, 0x271c, 0x2750, 0, 0);
                break;
            case 3:
                ECLJump(ecl, 0x67b0, 0x6a44, 60, 90); // 0x6a64, 0x6a98
                ECLJump(ecl, 0x271c, 0x27b8, 0, 0);
                break;
            case 4:
                ECLJump(ecl, 0x67b0, 0x6a44, 60, 90); // 0x6a64, 0x6a98
                break;
            case 5:
                ECLJump(ecl, 0x67b0, 0x6a64, 60, 90); // 0x6a98
                ECLJump(ecl, 0x2a58, 0x2ad4, 10, 0);
                break;
            case 6:
                ECLJump(ecl, 0x67b0, 0x6a98, 60, 90);
                break;
            case 7:
                ECLJump(ecl, 0x67b0, 0x6a98, 60, 90);
                ECLJump(ecl, 0x28f0, 0x2934, 0, 0);
                break;
            case 8:
                ECLJump(ecl, 0x67b0, 0x6a98, 60, 90);
                ECLJump(ecl, 0x28f0, 0x2968, 0, 0);
                break;
            default:
                break;
            }
        } else if (stage == 4) {
            switch (portion) {
            case 1:
                break;
            case 2:
                ECLJump(ecl, 0x8554, 0x8828, 60, 90); // 0x8848, 0x8868
                ECLJump(ecl, 0x3f74, 0x3fc8, 0, 0);
                break;
            case 3:
                ECLJump(ecl, 0x8554, 0x8828, 60, 90); // 0x8848, 0x8868
                ECLJump(ecl, 0x3f74, 0x4030, 0, 0);
                break;
            case 4:
                ECLJump(ecl, 0x8554, 0x8828, 60, 90); // 0x8848, 0x8868
                ECLJump(ecl, 0x3f74, 0x40ac, 0, 0);
                break;
            case 5:
                ECLJump(ecl, 0x8554, 0x8848, 60, 90); // 0x8868
                ECLJump(ecl, 0x4228, 0x42d0, 0, 0);
                break;
            case 6:
                ECLJump(ecl, 0x8554, 0x8868, 60, 90);
                break;
            case 7:
                ECLJump(ecl, 0x8554, 0x8868, 60, 90);
                ECLJump(ecl, 0x4114, 0x418c, 0, 0);
                break;
            case 8:
                ECLJump(ecl, 0x8554, 0x8868, 60, 90);
                ECLJump(ecl, 0x4114, 0x41c0, 0, 0);
                break;
            default:
                break;
            }
        } else if (stage == 5) {
            switch (portion) {
            case 1:
                break;
            case 2:
                ECLJump(ecl, 0x60e4, 0x6418, 60, 90); // 0x6438, 0x6458
                ECLJump(ecl, 0x26f8, 0x2780, 0, 0);
                break;
            case 3:
                ECLJump(ecl, 0x60e4, 0x6418, 60, 90); // 0x6438, 0x6458
                ECLJump(ecl, 0x26f8, 0x27b4, 0, 0);
                break;
            case 4:
                ECLJump(ecl, 0x60e4, 0x6438, 60, 90); // 0x6458
                ECLJump(ecl, 0x28ec, 0x2994, 0, 0);
                break;
            case 5:
                ECLJump(ecl, 0x60e4, 0x6458, 60, 30);
                break;
            case 6:
                ECLJump(ecl, 0x60e4, 0x6458, 60, 90);
                ECLJump(ecl, 0x2850, 0x2898, 0, 0);
                break;
            case 7:
                ECLJump(ecl, 0x60e4, 0x6458, 60, 90);
                ECLJump(ecl, 0x2850, 0x2898, 0, 0);
                ECLJump(ecl, 0x5154, 0x54d0, 0, 0);
                break;
            default:
                break;
            }
        } else if (stage == 6) {
            switch (portion) {
            case 1:
                break;
            case 2:
                ECLJump(ecl, 0x4bdc, 0x4db0, 60, 90);
                ECLJump(ecl, 0x2adc, 0x2b24, 0, 0);
                break;
            case 3:
                ECLJump(ecl, 0x4bdc, 0x4db0, 60, 90);
                ECLJump(ecl, 0x2adc, 0x2b58, 0, 0);
                break;
            case 4:
                ECLJump(ecl, 0x4bdc, 0x4db0, 60, 90);
                ECLJump(ecl, 0x2adc, 0x2b78, 0, 0);
                break;
            default:
                break;
            }
        } else if (stage == 7) {
            switch (portion) {
            case 1:
                break;
            case 2:
                ECLJump(ecl, 0x4b98, 0x4e40, 60, 90); // 0x4e60, 0x4e80
                ECLJump(ecl, 0x3234, 0x327c, 0, 0);
                break;
            case 3:
                ECLJump(ecl, 0x4b98, 0x4e40, 60, 90); // 0x4e60, 0x4e80
                ECLJump(ecl, 0x3234, 0x32b0, 0, 0);
                break;
            case 4:
                ECLJump(ecl, 0x4b98, 0x4e80, 60, 90);
                ecl << pair(0x3328, 0);
                break;
            case 5:
                ECLJump(ecl, 0x4b98, 0x4e80, 60, 90);
                ECLJump(ecl, 0x3318, 0x3394, 0, 0);
                break;
            case 6:
                ECLJump(ecl, 0x4b98, 0x4e80, 60, 90);
                ECLJump(ecl, 0x3318, 0x33c8, 0, 0);
                break;
            default:
                break;
            }
        }
    }
    __declspec(noinline) void THPatch(ECLHelper& ecl, th_sections_t section)
    {
        auto st4_boss = [&]() {
            ECLJump(ecl, 0x8554, 0x8678, 60);
            ecl << pair(0x8694, 0);
            ecl << pair(0x3be0, 144.0f) << pair(0x3be4, -16.0f)
                << pair(0x3c24, -144.0f) << pair(0x3c28, -16.0f);
            ecl.SetFile(3);
            ecl << pair(0x498, 60) << pair(0x4e4, 61);
        };

        switch (section) {
        case THPrac::TH13::TH13_ST1_MID1:
            ECLJump(ecl, 0x8a70, 0x8c50, 60);
            break;
        case THPrac::TH13::TH13_ST1_BOSS1:
            ECLJump(ecl, 0x8a70, 0x8d04, 60);
            break;
        case THPrac::TH13::TH13_ST1_BOSS2:
            ECLJump(ecl, 0x8a70, 0x8d04, 60);
            ecl.SetFile(2);
            ECLJump(ecl, 0x35c, 0x444, 0); // Utilize Spell Practice Jump
            ecl << pair(0x454, 1500); // Set Health
            ecl << pair(0x474, (int8_t)0x31); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST1_BOSS3:
            ECLJump(ecl, 0x8a70, 0x8d04, 60);
            ecl.SetFile(2);
            ecl << pair(0x7f8, (int8_t)0x32); // Change Nonspell
            ECLJump(ecl, 0x1110, 0x11b0, 0); // Disable Item Drops
            ecl << pair(0x101c, (int16_t)0) << pair(0x11b4, (int16_t)0); // Disable SE
            ecl << pair(0x11fc, 0) << pair(0x10ac, 0); // Change Wait Time & Inv. Time
            break;
        case THPrac::TH13::TH13_ST1_BOSS4:
            ECLJump(ecl, 0x8a70, 0x8d04, 60);
            ecl.SetFile(2);
            ECLJump(ecl, 0x35c, 0x444, 0); // Utilize Spell Practice Jump
            ecl << pair(0x454, 1500); // Set Health
            ecl << pair(0x474, (int8_t)0x32); // Set Spell Ordinal
            ecl.SetPos(0x2dc);
            ecl << 0.0f << 128.0f << 280.0f << 192.0f; // Change Movement Restriction
            break;
        case THPrac::TH13::TH13_ST1_BOSS5:
            ECLJump(ecl, 0x8a70, 0x8d04, 60);
            ecl.SetFile(2);
            ECLJump(ecl, 0x35c, 0x444, 0); // Utilize Spell Practice Jump
            ecl << pair(0x454, 2500); // Set Health
            ecl << pair(0x474, (int8_t)0x33); // Set Spell Ordinal
            ecl.SetPos(0x2dc);
            ecl << 0.0f << 128.0f << 280.0f << 256.0f; // Change Movement Restriction
            break;
        case THPrac::TH13::TH13_ST1_BOSS6:
            ECLJump(ecl, 0x8a70, 0x8d04, 60);
            ecl.SetFile(2);
            ECLJump(ecl, 0x35c, 0x444, 0); // Utilize Spell Practice Jump
            ecl << pair(0x454, 2500); // Set Health
            ecl << pair(0x474, (int8_t)0x34); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST2_MID1:
            ECLJump(ecl, 0x7718, 0x79cc, 60);
            break;
        case THPrac::TH13::TH13_ST2_MID2:
            ECLJump(ecl, 0x7718, 0x79cc, 60);
            ecl.SetFile(2);
            ECLJump(ecl, 0x3b0, 0x498, 0); // Utilize Spell Practice Jump
            break;
        case THPrac::TH13::TH13_ST2_BOSS1:
            ECLJump(ecl, 0x7718, 0x7a5c, 60);
            break;
        case THPrac::TH13::TH13_ST2_BOSS2:
            ECLJump(ecl, 0x7718, 0x7a5c, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x374, 0x45c, 0); // Utilize Spell Practice Jump
            ecl << pair(0x46c, 2700); // Set Health
            ecl << pair(0x48c, (int8_t)0x31); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST2_BOSS3:
            ECLJump(ecl, 0x7718, 0x7a5c, 60);
            ecl.SetFile(3);
            ecl << pair(0x72c, (int8_t)0x32); // Change Nonspell
            ECLJump(ecl, 0x12d4, 0x1374, 0); // Disable Item Drops
            ecl << pair(0x11f4, (int16_t)0) << pair(0x1378, (int16_t)0); // Disable SE
            ecl << pair(0x13c0, 0) << pair(0x1270, 0); // Change Wait Time & Inv. Time
            break;
        case THPrac::TH13::TH13_ST2_BOSS4:
            ECLJump(ecl, 0x7718, 0x7a5c, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x374, 0x45c, 0); // Utilize Spell Practice Jump
            ecl << pair(0x46c, 2200); // Set Health
            ecl << pair(0x48c, (int8_t)0x32); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST2_BOSS5:
            ECLJump(ecl, 0x7718, 0x7a5c, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x374, 0x45c, 0); // Utilize Spell Practice Jump
            ecl << pair(0x46c, 2500); // Set Health
            ecl << pair(0x48c, (int8_t)0x33); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST3_MID1:
            ECLJump(ecl, 0x67b0, 0x6a64, 60);
            ECLJump(ecl, 0x2a9c, 0x2ac0, 10);
            break;
        case THPrac::TH13::TH13_ST3_MID2_EN:
            ECLJump(ecl, 0x67b0, 0x6a64, 60);
            ECLJump(ecl, 0x2a9c, 0x2ac0, 10);
            ecl.SetFile(2);
            ecl << pair(0x4bd, (int8_t)0x32); // Change Nonspell
            ECLJump(ecl, 0x11f8, 0x1280, 0); // Disable Item Drops
            ecl << pair(0x1134, (int16_t)0) << pair(0x1284, (int16_t)0); // Disable SE
            ecl << pair(0x12cc, 0) << pair(0x1194, 0); // Change Wait Time & Inv. Time
            break;
        case THPrac::TH13::TH13_ST3_MID2_HL:
            ECLJump(ecl, 0x67b0, 0x6a64, 60);
            ECLJump(ecl, 0x2a9c, 0x2ac0, 10);
            ecl.SetFile(2);
            ECLJump(ecl, 0x388, 0x470, 0); // Utilize Spell Practice Jump
            break;
        case THPrac::TH13::TH13_ST3_BOSS1:
            ECLJump(ecl, 0x67b0, 0x6b08, 60);
            break;
        case THPrac::TH13::TH13_ST3_BOSS2:
            ECLJump(ecl, 0x67b0, 0x6b08, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x3f0, 0x4d8, 0); // Utilize Spell Practice Jump
            ecl << pair(0x4e8, 2200); // Set Health
            ecl << pair(0x508, (int8_t)0x31); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST3_BOSS3:
            ECLJump(ecl, 0x67b0, 0x6b08, 60);
            ecl.SetFile(3);
            ecl << pair(0x7a8, (int8_t)0x32); // Change Nonspell
            ECLJump(ecl, 0x1460, 0x1500, 0); // Disable Item Drops
            ecl << pair(0x1360, (int16_t)0) << pair(0x1504, (int16_t)0); // Disable SE
            ecl << pair(0x154c, 20) << pair(0x13fc, 0); // Change Wait Time & Inv. Time
            ecl << pair(0x1374, (int16_t)0) << pair(0x350, (int16_t)0); // Void 401 & 504
            ecl << pair(0x2f8, 0.0f) << pair(0x2fc, 96.0f); // Change Movement
            break;
        case THPrac::TH13::TH13_ST3_BOSS4:
            ECLJump(ecl, 0x67b0, 0x6b08, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x3f0, 0x4d8, 0); // Utilize Spell Practice Jump
            ecl << pair(0x4e8, 2600); // Set Health
            ecl << pair(0x508, (int8_t)0x32); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST3_BOSS5:
            ECLJump(ecl, 0x67b0, 0x6b08, 60);
            ecl.SetFile(3);
            ecl << pair(0x7a8, (int8_t)0x33); // Change Nonspell
            ECLJump(ecl, 0x210c, 0x21ac, 0); // Disable Item Drops
            ecl << pair(0x202c, (int16_t)0) << pair(0x21b0, (int16_t)0); // Disable SE
            ecl << pair(0x21f8, 20) << pair(0x20a8, 0); // Change Wait Time & Inv. Time
            break;
        case THPrac::TH13::TH13_ST3_BOSS6:
            ECLJump(ecl, 0x67b0, 0x6b08, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x3f0, 0x4d8, 0); // Utilize Spell Practice Jump
            ecl << pair(0x4e8, 2800); // Set Health
            ecl << pair(0x508, (int8_t)0x33); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST4_MID1:
            ECLJump(ecl, 0x8554, 0x8848, 60);
            break;
        case THPrac::TH13::TH13_ST4_MID2:
            ECLJump(ecl, 0x8554, 0x8848, 60);
            ecl.SetFile(2);
            ECLJump(ecl, 0xbf0, 0xcd8, 0); // Utilize Spell Practice Jump
            break;
        case THPrac::TH13::TH13_ST4_BOSS1:
            st4_boss();
            break;
        case THPrac::TH13::TH13_ST4_BOSS2:
            st4_boss();
            ECLJump(ecl, 0x5ac, 0x694, 1); // Utilize Spell Practice Jump
            ecl << pair(0x6a4, 1700); // Set Health
            ecl << pair(0x6c4, (int8_t)0x31); // Set Spell Ordinal
            ECLJump(ecl, 0x4a04, 0x4a6c, 0); // Stall Miyako
            ecl.SetPos(0x518);
            ecl << 0.0f << 160.0f << 280.0f << 64.0f; // Change Movement Restriction
            break;
        case THPrac::TH13::TH13_ST4_BOSS3:
            st4_boss();

            // Seiga
            ecl << pair(0x964, (int8_t)0x32); // Change Nonspell
            ECLJump(ecl, 0x11f0, 0x1290, 0); // Disable Item Drops
            ecl << pair(0x10f0, (int16_t)0) << pair(0x1294, (int16_t)0); // Disable SE
            ecl << pair(0x12dc, 20) << pair(0x118c, 0); // Change Wait Time & Inv. Time
            ecl << pair(0x1104, (int16_t)0) << pair(0x50c, (int16_t)0); // Void 401 & 504
            ecl << pair(0x4a4, 0.0f) << pair(0x4a8, 96.0f); // Change Movement

            // Miyako
            ecl << pair(0x4924, 0.0f) << pair(0x4928, 260.0f) << pair(0x497c, (int16_t)0); // Change Pos & Void 504
            ECLJump(ecl, 0x4a04, 0x4a6c, 0); // Stall Miyako
            ecl << pair(0x51ec, 70); // Change Wait Time
            break;
        case THPrac::TH13::TH13_ST4_BOSS4:
            st4_boss();
            ECLJump(ecl, 0x5ac, 0x694, 1); // Utilize Spell Practice Jump
            ecl << pair(0x6a4, 1700); // Set Health
            ecl << pair(0x6c4, (int8_t)0x32); // Set Spell Ordinal
            ECLJump(ecl, 0x4a04, 0x4a6c, 0); // Stall Miyako
            break;
        case THPrac::TH13::TH13_ST4_BOSS5:
            st4_boss();

            // Seiga
            ecl << pair(0x964, (int8_t)0x33); // Change Nonspell
            ECLJump(ecl, 0x1a94, 0x1b34, 0); // Disable Item Drops
            ecl << pair(0x1994, (int16_t)0) << pair(0x1b38, (int16_t)0); // Disable SE
            ecl << pair(0x1b80, 20) << pair(0x1a30, 0); // Change Wait Time & Inv. Time
            ecl << pair(0x19a8, (int16_t)0) << pair(0x50c, (int16_t)0); // Void 401 & 504
            ecl << pair(0x4a4, 0.0f) << pair(0x4a8, 96.0f); // Change Movement

            // Miyako
            ecl << pair(0x4924, 0.0f) << pair(0x4928, 160.0f) << pair(0x497c, (int16_t)0); // Change Pos & Void 504
            ECLJump(ecl, 0x4a04, 0x4a6c, 0); // Stall Miyako
            ecl << pair(0x5a14, (int16_t)0); // Void 401;
            ecl << pair(0x5a40, 70); // Change Wait Time
            break;
        case THPrac::TH13::TH13_ST4_BOSS6:
            st4_boss();
            ECLJump(ecl, 0x5ac, 0x694, 1); // Utilize Spell Practice Jump
            ecl << pair(0x6a4, 2200); // Set Health
            ecl << pair(0x6c4, (int8_t)0x33); // Set Spell Ordinal
            ECLJump(ecl, 0x4a04, 0x4a6c, 0); // Stall Miyako
            break;
        case THPrac::TH13::TH13_ST5_MID1:
            ECLJump(ecl, 0x60e4, 0x6438, 60);
            break;
        case THPrac::TH13::TH13_ST5_MID2:
            ECLJump(ecl, 0x60e4, 0x6438, 60);
            ecl.SetFile(2);
            ECLJump(ecl, 0x3fc, 0x4e4, 0);
            break;
        case THPrac::TH13::TH13_ST5_BOSS1:
            ECLJump(ecl, 0x60e4, 0x64b4, 60);
            break;
        case THPrac::TH13::TH13_ST5_BOSS2:
            ECLJump(ecl, 0x60e4, 0x64b4, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x380, 0x468, 0); // Utilize Spell Practice Jump
            ecl << pair(0x478, 2100); // Set Health
            ecl << pair(0x498, (int8_t)0x31); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST5_BOSS3:
            ECLJump(ecl, 0x60e4, 0x64b4, 60);
            ecl.SetFile(3);
            ecl << pair(0x81c, (int8_t)0x32); // Change Nonspell
            ECLJump(ecl, 0x1278, 0x1318, 0); // Disable Item Drops
            ecl << pair(0x1178, (int16_t)0) << pair(0x131c, (int16_t)0); // Disable SE
            ecl << pair(0x1364, 0) << pair(0x1214, 0); // Change Wait Time & Inv. Time
            ecl << pair(0x118c, (int16_t)0) << pair(0x2e0, (int16_t)0); // Void 401 & 504
            ecl << pair(0x288, 0.0f) << pair(0x28c, 128.0f); // Change Movement
            break;
        case THPrac::TH13::TH13_ST5_BOSS4:
            ECLJump(ecl, 0x60e4, 0x64b4, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x380, 0x468, 0); // Utilize Spell Practice Jump
            ecl << pair(0x478, 2500); // Set Health
            ecl << pair(0x498, (int8_t)0x32); // Set Spell Ordinal
            ecl.SetPos(0x2ec);
            ecl << 0.0f << 128.0f << 280.0f << 32.0f; // Change Movement Restriction
            break;
        case THPrac::TH13::TH13_ST5_BOSS5:
            ECLJump(ecl, 0x60e4, 0x64b4, 60);
            ecl.SetFile(3);
            ecl << pair(0x81c, (int8_t)0x33); // Change Nonspell
            ECLJump(ecl, 0x1e14, 0x1ecc, 0); // Disable Item Drops
            ecl << pair(0x1d14, (int16_t)0) << pair(0x1ed0, (int16_t)0); // Disable SE
            ecl << pair(0x1f18, 0) << pair(0x1db0, 0); // Change Wait Time & Inv. Time
            ecl << pair(0x1d28, (int16_t)0) << pair(0x2e0, (int16_t)0); // Void 401 & 504
            ecl << pair(0x288, 0.0f) << pair(0x28c, 128.0f); // Change Movement
            break;
        case THPrac::TH13::TH13_ST5_BOSS6:
            ECLJump(ecl, 0x60e4, 0x64b4, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x380, 0x468, 0); // Utilize Spell Practice Jump
            ecl << pair(0x478, 2300); // Set Health
            ecl << pair(0x498, (int8_t)0x33); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST5_BOSS7:
            ECLJump(ecl, 0x60e4, 0x64b4, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x380, 0x468, 0); // Utilize Spell Practice Jump
            ecl << pair(0x478, 4500); // Set Health
            ecl << pair(0x498, (int8_t)0x34); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST6_BOSS1:
            ECLJump(ecl, 0x4bdc, 0x4e20, 60);
            break;
        case THPrac::TH13::TH13_ST6_BOSS2:
            ECLJump(ecl, 0x4bdc, 0x4e20, 60);
            ecl.SetFile(2);
            ECLJump(ecl, 0x4c0, 0x5a8, 0); // Utilize Spell Practice Jump
            ecl << pair(0x5b8, 2300); // Set Health
            ecl << pair(0x5d8, (int8_t)0x31); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST6_BOSS3:
            ECLJump(ecl, 0x4bdc, 0x4e20, 60);
            ecl.SetFile(2);
            ecl << pair(0xc50, (int8_t)0x32); // Change Nonspell
            ECLJump(ecl, 0x14bc, 0x155c, 0); // Disable Item Drops
            ecl << pair(0x13bc, (int16_t)0) << pair(0x1560, (int16_t)0); // Disable SE
            ecl << pair(0x15a8, 0) << pair(0x1458, 0); // Change Wait Time & Inv. Time
            ecl << pair(0x13d0, (int16_t)0) << pair(0x420, (int16_t)0); // Void 401 & 504
            ecl << pair(0x3c8, 0.0f) << pair(0x3cc, 128.0f); // Change Movement
            break;
        case THPrac::TH13::TH13_ST6_BOSS4:
            ECLJump(ecl, 0x4bdc, 0x4e20, 60);
            ecl.SetFile(2);
            ECLJump(ecl, 0x4c0, 0x5a8, 0); // Utilize Spell Practice Jump
            ecl << pair(0x5b8, 2500); // Set Health
            ecl << pair(0x5d8, (int8_t)0x32); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST6_BOSS5:
            ECLJump(ecl, 0x4bdc, 0x4e20, 60);
            ecl.SetFile(2);
            ecl << pair(0xc50, (int8_t)0x33); // Change Nonspell
            ECLJump(ecl, 0x1ec0, 0x1f74, 0); // Disable Item Drops
            ecl << pair(0x1dc0, (int16_t)0) << pair(0x1f78, (int16_t)0); // Disable SE
            ecl << pair(0x1fc0, 0) << pair(0x1e5c, 60); // Change Wait Time & Inv. Time
            ecl << pair(0x1dd4, (int16_t)0) << pair(0x420, (int16_t)0); // Void 401 & 504
            ecl << pair(0x3c8, 0.0f) << pair(0x3cc, 128.0f); // Change Movement
            break;
        case THPrac::TH13::TH13_ST6_BOSS6:
            ECLJump(ecl, 0x4bdc, 0x4e20, 60);
            ecl.SetFile(2);
            ECLJump(ecl, 0x4c0, 0x5a8, 0); // Utilize Spell Practice Jump
            ecl << pair(0x5b8, 3100); // Set Health
            ecl << pair(0x5d8, (int8_t)0x33); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST6_BOSS7:
            ECLJump(ecl, 0x4bdc, 0x4e20, 60);
            ecl.SetFile(2);
            ecl << pair(0xc50, (int8_t)0x34); // Change Nonspell
            ECLJump(ecl, 0x29d8, 0x2a78, 0); // Disable Item Drops
            ecl << pair(0x27f0, (int16_t)0) << pair(0x2a7c, (int16_t)0); // Disable SE
            ecl << pair(0x2ac4, 0) << pair(0x2974, 0); // Change Wait Time & Inv. Time
            ecl << pair(0x2814, (int16_t)0) << pair(0x420, (int16_t)0); // Void 401 & 504
            ecl << pair(0x3c8, 0.0f) << pair(0x3cc, 160.0f); // Change Movement
            break;
        case THPrac::TH13::TH13_ST6_BOSS8:
            ECLJump(ecl, 0x4bdc, 0x4e20, 60);
            ecl.SetFile(2);
            ECLJump(ecl, 0x4c0, 0x5a8, 0); // Utilize Spell Practice Jump
            ecl << pair(0x5b8, 2800); // Set Health
            ecl << pair(0x5d8, (int8_t)0x34); // Set Spell Ordinal
            break;
        case THPrac::TH13::TH13_ST6_BOSS9:
            ECLJump(ecl, 0x4bdc, 0x4e20, 60);
            ecl.SetFile(2);
            ECLJump(ecl, 0x4c0, 0x9ec, 0); // Utilize Spell Practice Jump
            ecl << pair(0x3c8, 0.0f) << pair(0x3cc, 128.0f); // Change Pos
            ecl.SetPos(0x42c);
            ecl << 0.0f << 128.0f << 280.0f << 64.0f; // Change Movement Restriction
            break;
        case THPrac::TH13::TH13_ST6_BOSS10:
            ECLJump(ecl, 0x4bdc, 0x4e20, 60);
            ecl.SetFile(2);
            ECLJump(ecl, 0x4c0, 0x5a8, 0); // Utilize Spell Practice Jump
            ecl << pair(0x5b8, 4000); // Set Health
            ecl << pair(0x5d8, (int8_t)0x36); // Set Spell Ordinal
            ecl << pair(0x3c8, 0.0f) << pair(0x3cc, 128.0f); // Change Pos
            ecl.SetPos(0x42c);
            ecl << 0.0f << 128.0f << 280.0f << 64.0f; // Change Movement Restriction
            break;
        case THPrac::TH13::TH13_ST6_BOSS11:
            ECLJump(ecl, 0x4bdc, 0x4e20, 60);
            ecl.SetFile(2);
            ECLJump(ecl, 0x4c0, 0x5a8, 0); // Utilize Spell Practice Jump
            ecl << pair(0x5b8, 6800); // Set Health
            ecl << pair(0x5d8, (int8_t)0x37); // Set Spell Ordinal
            ecl << pair(0x3c8, 0.0f) << pair(0x3cc, 200.0f); // Change Pos
            ecl << pair(0x420, (int16_t)0); // Void 504

            if (thPracParam.phase == 1) {
                ecl << pair(0x5b8, 1650);
                ECLJump(ecl, 0xc430, 0xc998, 0);
            }
            break;
        case THPrac::TH13::TH13_ST7_MID1:
            ECLJump(ecl, 0x4b98, 0x4e60, 60);
            ECLJump(ecl, 0x3460, 0x3484, 10);
            break;
        case THPrac::TH13::TH13_ST7_MID2:
            ECLJump(ecl, 0x4b98, 0x4e60, 60);
            ECLJump(ecl, 0x3460, 0x3484, 10);
            ecl.SetFile(2);
            ECLJump(ecl, 0x394, 0x4e8, 0);
            break;
        case THPrac::TH13::TH13_ST7_MID3:
            ECLJump(ecl, 0x4b98, 0x4e60, 60);
            ECLJump(ecl, 0x3460, 0x3484, 10);
            ecl.SetFile(2);
            ECLJump(ecl, 0x394, 0x59c, 0);
            break;
        case THPrac::TH13::TH13_ST7_END_NS1:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            break;
        case THPrac::TH13::TH13_ST7_END_S1:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x770, 0x810, 0); // Utilize Spell Practice Jump
            break;
        case THPrac::TH13::TH13_ST7_END_NS2:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ecl << pair(0xf60, (int8_t)0x32); // Change Nonspell
            ECLJump(ecl, 0x1c5c, 0x1cfc, 0); // Disable Item Drops
            ecl << pair(0x1b5c, (int16_t)0) << pair(0x1d00, (int16_t)0); // Disable SE
            ecl << pair(0x1d48, 0) << pair(0x1bf8, 0); // Change Wait Time & Inv. Time
            ecl << pair(0x1b70, (int16_t)0) << pair(0x6d0, (int16_t)0); // Void 401 & 504
            ecl << pair(0x678, 0.0f) << pair(0x67c, 128.0f); // Change Movement
            break;
        case THPrac::TH13::TH13_ST7_END_S2:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x770, 0x8c4, 0); // Utilize Spell Practice Jump
            break;
        case THPrac::TH13::TH13_ST7_END_NS3:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ecl << pair(0xf60, (int8_t)0x33); // Change Nonspell
            ECLJump(ecl, 0x24f8, 0x2598, 0); // Disable Item Drops
            ecl << pair(0x23f8, (int16_t)0) << pair(0x259c, (int16_t)0); // Disable SE
            ecl << pair(0x25e4, 0) << pair(0x2494, 0); // Change Wait Time & Inv. Time
            ecl << pair(0x23f8, (int16_t)0) << pair(0x6d0, (int16_t)0); // Void 401 & 504
            ecl << pair(0x678, 0.0f) << pair(0x67c, 128.0f); // Change Movement
            break;
        case THPrac::TH13::TH13_ST7_END_S3:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x770, 0x978, 0); // Utilize Spell Practice Jump
            break;
        case THPrac::TH13::TH13_ST7_END_NS4:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ecl << pair(0xf60, (int8_t)0x34); // Change Nonspell
            ECLJump(ecl, 0x2da8, 0x2e48, 0); // Disable Item Drops
            ecl << pair(0x2ca8, (int16_t)0) << pair(0x2e4c, (int16_t)0); // Disable SE
            ecl << pair(0x2e94, 0) << pair(0x2d44, 0); // Change Wait Time & Inv. Time
            ecl << pair(0x2cbc, (int16_t)0) << pair(0x6d0, (int16_t)0); // Void 401 & 504
            ecl << pair(0x678, 0.0f) << pair(0x67c, 128.0f); // Change Movement
            break;
        case THPrac::TH13::TH13_ST7_END_S4:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x770, 0xa2c, 0); // Utilize Spell Practice Jump
            break;
        case THPrac::TH13::TH13_ST7_END_NS5:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ecl << pair(0xf60, (int8_t)0x35); // Change Nonspell
            ECLJump(ecl, 0x3618, 0x36b8, 0); // Disable Item Drops
            ecl << pair(0x3518, (int16_t)0) << pair(0x36bc, (int16_t)0); // Disable SE
            ecl << pair(0x3704, 0) << pair(0x35b4, 0); // Change Wait Time & Inv. Time
            ecl << pair(0x352c, (int16_t)0) << pair(0x6d0, (int16_t)0); // Void 401 & 504
            ecl << pair(0x678, 0.0f) << pair(0x67c, 128.0f); // Change Movement
            break;
        case THPrac::TH13::TH13_ST7_END_S5:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x770, 0xae0, 0); // Utilize Spell Practice Jump
            break;
        case THPrac::TH13::TH13_ST7_END_NS6:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ecl << pair(0xf60, (int8_t)0x36); // Change Nonspell
            ECLJump(ecl, 0x3ec8, 0x3f7c, 0); // Disable Item Drops
            ecl << pair(0x3dc8, (int16_t)0) << pair(0x3f80, (int16_t)0); // Disable SE
            ecl << pair(0x3fc8, 0) << pair(0x3e64, 0); // Change Wait Time & Inv. Time
            ecl << pair(0x3ddc, (int16_t)0) << pair(0x6d0, (int16_t)0); // Void 401 & 504
            ecl << pair(0x678, 0.0f) << pair(0x67c, 128.0f); // Change Movement
            break;
        case THPrac::TH13::TH13_ST7_END_S6:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x770, 0xb94, 0); // Utilize Spell Practice Jump
            break;
        case THPrac::TH13::TH13_ST7_END_NS7:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ecl << pair(0xf60, (int8_t)0x37); // Change Nonspell
            ECLJump(ecl, 0x49d8, 0x4a78, 0); // Disable Item Drops
            ecl << pair(0x48d8, (int16_t)0) << pair(0x4a7c, (int16_t)0); // Disable SE
            ecl << pair(0x4ac4, 0) << pair(0x4974, 0); // Change Wait Time & Inv. Time
            ecl << pair(0x48ec, (int16_t)0) << pair(0x6d0, (int16_t)0); // Void 401 & 504
            ecl << pair(0x678, 0.0f) << pair(0x67c, 128.0f); // Change Movement
            break;
        case THPrac::TH13::TH13_ST7_END_S7:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x770, 0xcb0, 0); // Utilize Spell Practice Jump
            break;
        case THPrac::TH13::TH13_ST7_END_NS8:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ecl << pair(0xf60, (int8_t)0x38); // Change Nonspell
            ECLJump(ecl, 0x54d4, 0x5574, 0); // Disable Item Drops
            ecl << pair(0x53d4, (int16_t)0) << pair(0x5578, (int16_t)0); // Disable SE
            ecl << pair(0x55c0, 0) << pair(0x5470, 0); // Change Wait Time & Inv. Time
            ecl << pair(0x53e8, (int16_t)0) << pair(0x6d0, (int16_t)0); // Void 401 & 504
            ecl << pair(0x678, 0.0f) << pair(0x67c, 128.0f); // Change Movement
            break;
        case THPrac::TH13::TH13_ST7_END_S8:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x770, 0xd64, 0); // Utilize Spell Practice Jump
            break;
        case THPrac::TH13::TH13_ST7_END_S9:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x770, 0xe18, 0); // Utilize Spell Practice Jump
            break;
        case THPrac::TH13::TH13_ST7_END_S10:
            ECLJump(ecl, 0x4b98, 0x4ef0, 60);
            ecl.SetFile(3);
            ECLJump(ecl, 0x770, 0xefc, 0); // Utilize Spell Practice Jump

            if (thPracParam.phase == 1) {
                ECLJump(ecl, 0x6a3c, 0x6ab0, 120);
            }
            break;
        default:
            break;
        }
    }
    EHOOK_G1(th13_bossrush_msg, (void*)0x428ba4) 
    {
        while (true) {
            uint32_t msgPtr = *(uint32_t*)(pCtx->Edi + 0x14c);
            uint8_t msgId = *(uint8_t*)(msgPtr + 2);
            switch (msgId) {
            case 0:
            case 19:
            case 21:
            case 22:
                return;
            default:
                break;
            }
            *(uint32_t*)(pCtx->Edi + 0x14c) = pCtx->Eax = msgPtr + *(uint8_t*)(msgPtr + 3) + 4;
            *(uint32_t*)(pCtx->Edi + 0x1c) = (uint32_t) * (uint16_t*)(msgPtr);
        }

    }
    __declspec(noinline) void THBossRushInit() 
    {
        th13_bossrush_msg::GetHook().Enable();
        ECLHelper ecl;
        ecl.SetBaseAddr((void*)U32_PTR(DATA_ECL_BASE));
        switch (U8_REF(DATA_STAGE)) {
        case 1: {
            if (thPracParam.bossrush_midboss) {
                ECLJump(ecl, 0x8a70, 0x8c50, 60);
                ecl.SetPos(0x8c80);
                ecl << 0 << 0x00100208 << 0x00ff0000 << 0
                    << 0 << 0x00140017 << 0x01ff0000 << 0 << 180;
                ECLJump(ecl, 0, 0x8cc8, 60);
            } else {
                ECLJump(ecl, 0x8a70, 0x8cc8, 60);
            }
            break;
        }
        case 2: {

            break;
        }
        case 3: {

            break;
        }
        case 4: {

            break;
        }
        case 5: {

            break;
        }
        case 6: {

            break;
        }
        case 7: {

            break;
        }
        default:
            break;
        }
    }
    __declspec(noinline) void THSectionPatch()
    {
        ECLHelper ecl;
        ecl.SetBaseAddr((void*)U32_PTR(DATA_ECL_BASE));

        if (thPracParam.mode == 1) {
            auto section = thPracParam.section;
            if (section >= 10000 && section < 20000) {
                int stage = (section - 10000) / 100;
                int portionId = (section - 10000) % 100;
                THStageWarp(ecl, stage, portionId);
            } else {
                THPatch(ecl, (th_sections_t)section);
            }
        }
    }

    int THBGMTest()
    {
        if (!thPracParam.mode)
            return 0;
        else if (thPracParam.section >= 10000)
            return 0;
        else
            return th_sections_bgm[thPracParam.section];
    }
    void THSaveReplay(char* repName)
    {
        ReplaySaveParam(mb_to_utf16(repName).c_str(), thPracParam.GetJson());
    }
    void THDataInit()
    {
        AnlyDataInit();

        SwitchHackSet<uint8_t, uint8_t>(InsHookCallback, (void*)0x4772ea, 30, (void*)0x4772fb, 0x10c, (void*)0x4772f4, 0x5e);
        InsHookInit(100, (void*)0x4772e4, [](PCONTEXT pCtx) { return (pCtx->Eax + 4); });

        InsHookRegister(1, 0, 0x5410, 10101);
        InsHookRegister(1, 0, 0x5444, 10102);
        InsHookRegister(1, 0, 0x54ac, 10103);
        InsHookRegister(1, 0, 0x8c54, TH13_ST1_MID1);
        InsHookRegister(1, 0, 0x4c44, 10104);
        InsHookRegister(1, 0, 0x5544, 10105);
        InsHookRegister(1, 2, 0x868, TH13_ST1_BOSS1);
        InsHookRegister(1, 2, 0x1030, TH13_ST1_BOSS3);
        InsHookRegister(1, 2, 0x167c, TH13_ST1_BOSS5);
        InsHookRegister(1, 2, 0x1a34, TH13_ST1_BOSS6);
        InsHookRegister(1, 2, 0x1f4c, TH13_ST1_BOSS2);
        InsHookRegister(1, 2, 0x3124, TH13_ST1_BOSS4);
        InsHookRegister(1, 2, 0x4114, TH13_ST1_BOSS5);
        InsHookRegister(1, 2, 0x4a18, TH13_ST1_BOSS6);

        DataRef<DATA_SCENE_ID>(U32_ARG(0x4dcc24));
        DataRef<DATA_RND_SEED>(U16_ARG(0x4dc324));
        DataRef<DATA_DIFFCULTY>(U8_ARG(0x4be7c4));
        DataRef<DATA_SHOT_TYPE>(U8_ARG(0x4be7b8));
        DataRef<DATA_STAGE>(U8_ARG(0x4be81c));
        DataRef<DATA_STARTING_STAGE>(U8_ARG(0x4be820));
        DataRef<DATA_FRAME>(U32_ARG(0x4be828));
        DataRef<DATA_ECL_BASE>(U32_ARG(0x4c2188, 0xac, 0xC));
        DataRef<DATA_GAME_INPUT>(U16_ARG(0x4E4C08));

        DataRef<DATA_SCORE>(U32_ARG(0x4be7c0));
        DataRef<DATA_LIFE>(U32_ARG(0x4be7f4));
        DataRef<DATA_EXTEND>(U32_ARG(0x4be7fc));
        DataRef<DATA_LIFE_FRAG>(U32_ARG(0x4be7f8));
        DataRef<DATA_BOMB>(U32_ARG(0x4be800));
        DataRef<DATA_BOMB_FRAG>(U32_ARG(0x4be804));
        DataRef<DATA_POWER>(U32_ARG(0x4be7e8));
        DataRef<DATA_VALUE>(U32_ARG(0x4be7dc));
        DataRef<DATA_GRAZE>(U32_ARG(0x4be7d0));
        DataRef<DATA_TRANCE_METER>(U32_ARG(0x4be808));

        DataRef<DATA_SPIRIT_BLUE>(0x438ba6);
        DataRef<DATA_SPIRIT_GREY>(0x438c3d);
        DataRef<DATA_SPIRIT_LIFE>(0x438b5d);
        DataRef<DATA_SPIRIT_BOMB>(0x438c19);
        DataRef<DATA_SPIRIT_BLUE_TRANCE>(0x438d51);
        DataRef<DATA_SPIRIT_GREY_TRANCE>(0x438da5);
        DataRef<DATA_SPIRIT_LIFE_TRANCE>(0x438d0e);
        DataRef<DATA_SPIRIT_BOMB_TRANCE>(0x438d84);
        DataRef<DATA_ITEM_POWER>(0x42e94d);
        DataRef<DATA_ITEM_POINT>(0x42e9e5);
        DataRef<DATA_ITEM_CANCEL>(0x42eb46);

        DataBatch<DATA_SCORE, DATA_LIFE, DATA_EXTEND, DATA_LIFE_FRAG, DATA_BOMB, DATA_BOMB_FRAG,
            DATA_POWER, DATA_VALUE, DATA_GRAZE, DATA_TRANCE_METER, 
            DATA_SPIRIT_BLUE, DATA_SPIRIT_GREY, DATA_SPIRIT_LIFE, DATA_SPIRIT_BOMB,
            DATA_SPIRIT_BLUE_TRANCE, DATA_SPIRIT_GREY_TRANCE, DATA_SPIRIT_LIFE_TRANCE, DATA_SPIRIT_BOMB_TRANCE,
            DATA_ITEM_POWER, DATA_ITEM_POINT, DATA_ITEM_CANCEL,
            EVENT_MISS, EVENT_BOMB, EVENT_DEATH_BOMB, EVENT_TRANCE_ACTIVE, EVENT_TRANCE_PASSIVE, EVENT_TRANCE_EXIT, EVENT_CONTINUE>();

        ImHook<EVENT_SPELLCARD_ENTER>((void*)0x413270, [](PCONTEXT pCtx) {
            AnlyEventRec(EVENT_SPELLCARD_ENTER, *(uint32_t*)(pCtx->Esp + 4), (uint32_t)U8_REF(DATA_DIFFCULTY));
            DataRef<EVENT_SPELLCARD_ENTER>()++;
        });
        ImHook<EVENT_SPELLCARD_GET>((void*)0x413acf, [](PCONTEXT pCtx) {
            AnlyEventRec(EVENT_SPELLCARD_GET, GetMemContent(0x4c2178, 0x78), (uint32_t)U8_REF(DATA_DIFFCULTY));
            DataRef<EVENT_SPELLCARD_GET>()++;
        });
        ImHook<EVENT_MISS>((void*)0x405c3e, [](PCONTEXT pCtx) {
            AnlyEventRec(EVENT_MISS);
            DataRef<EVENT_MISS>()++;
        });
        ImHook<EVENT_BOMB>((void*)0x4433f7, [](PCONTEXT pCtx) {
            AnlyEventRec(EVENT_BOMB);
            DataRef<EVENT_BOMB>()++;
        });
        ImHook<EVENT_DEATH_BOMB>((void*)0x44354c, [](PCONTEXT pCtx) {
            AnlyEventRec(EVENT_DEATH_BOMB);
            DataRef<EVENT_DEATH_BOMB>()++;
        });
        ImHook<EVENT_TRANCE_ACTIVE>((void*)0x405883, [](PCONTEXT pCtx) {
            AnlyEventRec(EVENT_TRANCE_ACTIVE);
            DataRef<EVENT_TRANCE_ACTIVE>()++;
        });
        ImHook<EVENT_TRANCE_PASSIVE>((void*)0x444a43, [](PCONTEXT pCtx) {
            AnlyEventRec(EVENT_TRANCE_PASSIVE);
            DataRef<EVENT_TRANCE_PASSIVE>()++;
        });
        ImHook<EVENT_TRANCE_EXIT>((void*)0x405c44, [](PCONTEXT pCtx) {
            if (!(GetMemContent(0x4c2164, 0x14) & 2)) {
                AnlyEventRec(EVENT_TRANCE_EXIT);
                DataRef<EVENT_TRANCE_EXIT>()++;
            }
        });
        ImHook<EVENT_CONTINUE>((void*)0x440758, [](PCONTEXT pCtx) {
            AnlyEventRec(EVENT_CONTINUE);
            DataRef<EVENT_CONTINUE>()++;
        });

        DataBatchCache<0>(true);
    }
    void THDataStage()
    {
        bool isStartingStage = U8_REF(DATA_STAGE) == U8_REF(DATA_STARTING_STAGE);
        AnlyDataStageStart();
        DataBatchCache<0>(true);
        AnlyDataCollect();
        InsHookApply();
        DataRef<SYSFLAG_INGAME>() = 1;
    }

    bool th13ElBgmTranceFlag = false;
    bool th13ElBgmFlag = false;

    HOOKSET_DEFINE(THMainHook)
    EHOOK_ST(th13_dump_rep, (void*)0x448d8c)
    {
        auto filePtr = (void*)pCtx->Eax;
        auto fileSize = *(uint32_t*)(*(uint32_t*)(pCtx->Ebx + 0x18) + 0x20);
        auto fileName = (char*)(pCtx->Esp + 0xC);

        std::wstring fileNameDump = mb_to_utf16(fileName);
        fileNameDump += L".dump";

        DWORD bytesProcessed;
        auto hFile = CreateFileW(fileNameDump.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
        SetEndOfFile(hFile);
        WriteFile(hFile, filePtr, fileSize, &bytesProcessed, NULL);
        CloseHandle(hFile);
    }
    EHOOK_DY(th13_everlasting_bgm_2, (void*)0x42c444)
    {
        if (th13ElBgmFlag) {
            th13ElBgmFlag = false;
            pCtx->Eip = 0x42c44b;
        }
    }
    EHOOK_DY(th13_everlasting_bgm, (void*)0x461830)
    {
        int32_t retn_addr = ((int32_t*)pCtx->Esp)[0];
        int32_t bgm_cmd = ((int32_t*)pCtx->Esp)[1];
        int32_t bgm_id = ((int32_t*)pCtx->Esp)[2];
        int32_t call_addr = ((int32_t*)pCtx->Esp)[3];

        bool el_switch;
        bool is_practice;
        bool result;

        if (bgm_cmd == 9)
            th13ElBgmTranceFlag = !th13ElBgmTranceFlag;

        el_switch = *(THOverlay::singleton().mElBgm) && !THGuiRep::singleton().mRepStatus && thPracParam.mode == 1 && thPracParam.section && !th13ElBgmTranceFlag;
        is_practice = (*((int32_t*)0x4be830) & 0x1);

        if (th13ElBgmTranceFlag && bgm_cmd == 3) {
            th13ElBgmTranceFlag = false;
            el_switch = false;
        }

        result = ElBgmTest<0x43ae75, 0x42c444, 0x43e555, 0x43e738, 0xffffffff>(
            el_switch, is_practice, retn_addr, bgm_cmd, bgm_id, 0xffffffff);

        if (result && retn_addr == 0x42c444) {
            th13ElBgmFlag = true;
        }

        if (result) {
            pCtx->Eip = 0x4618c0;
        }
    }
    EHOOK_DY(th13_param_reset, (void*)0x44cd2f)
    {
        thPracParam.Reset();
        DataRef<SYSFLAG_INGAME>() = 0;
    }
    EHOOK_DY(th13_prac_menu_1, (void*)0x451d14)
    {
        THGuiPrac::singleton().State(1);
    }
    EHOOK_DY(th13_prac_menu_2, (void*)0x451d39)
    {
        THGuiPrac::singleton().State(2);
    }
    EHOOK_DY(th13_prac_menu_3, (void*)0x452047)
    {
        THGuiPrac::singleton().State(3);
    }
    EHOOK_DY(th13_prac_menu_4, (void*)0x4520f5)
    {
        THGuiPrac::singleton().State(4);
    }
    PATCH_DY(th13_prac_menu_enter_1, (void*)0x451df9, "\xeb", 1);
    EHOOK_DY(th13_prac_menu_enter_2, (void*)0x4520b0)
    {
        pCtx->Eax = thPracParam.stage;
    }
    EHOOK_DY(th13_disable_prac_menu_1, (void*)0x452280)
    {
        pCtx->Eip = 0x4522b9;
    }
    EHOOK_DY(th13_disable_prac_menu_2, (void*)0x451cf2)
    {
        pCtx->Esp += 0x4;
        pCtx->Eip = 0x451cf7;
    }
    PATCH_DY(th13_disable_prac_menu_3, (void*)0x451dc6, "\x00", 1);
    EHOOK_DY(th13_menu_rank_fix, (void*)0x43f483)
    {
        //*((int32_t*)0x4be7d4) = -1;
        *((int32_t*)0x4be7c4) = *((int32_t*)0x4bb4d0);
    }
    EHOOK_DY(th13_patch_main, (void*)0x42bf8e)
    {
        if (thPracParam.mode == 1) {
            *(int32_t*)(0x4be7c0) = (int32_t)(thPracParam.score / 10);
            *(int32_t*)(0x4be7f4) = thPracParam.life;
            *(int32_t*)(0x4be7fc) = thPracParam.extend;
            *(int32_t*)(0x4be7f8) = thPracParam.life_fragment;
            *(int32_t*)(0x4be800) = thPracParam.bomb;
            *(int32_t*)(0x4be804) = thPracParam.bomb_fragment;
            *(int32_t*)(0x4be7e8) = thPracParam.power;
            *(int32_t*)(0x4be7dc) = thPracParam.value * 100;
            *(int32_t*)(0x4be7d0) = thPracParam.graze;
            *(int32_t*)(0x4be808) = thPracParam.trance_meter;

            THSectionPatch();
        } else if (thPracParam.mode == 2) {
            *(int32_t*)(0x4be7e8) = thPracParam.power;
            THBossRushInit();
        }
        THDataStage();
    }
    EHOOK_DY(th13_bgm, (void*)0x42c864)
    {
        if (THBGMTest()) {
            PushHelper32(pCtx, 1);
            pCtx->Eip = 0x42c865;
        }
    }
    EHOOK_DY(th13_rep_save, (void*)0x448c05)
    {
        char* repName = (char*)(pCtx->Esp + 0x38);
        if (thPracParam.mode == 1)
            THSaveReplay(repName);
        //else if (thPracParam.mode == 2 && thPracParam.phase)
        //	THSaveReplay(repName);
    }
    EHOOK_DY(th13_rep_menu_1, (void*)0x452776)
    {
        THGuiRep::singleton().State(1);
    }
    EHOOK_DY(th13_rep_menu_2, (void*)0x4528a2)
    {
        THGuiRep::singleton().State(2);
    }
    EHOOK_DY(th13_rep_menu_3, (void*)0x452a94)
    {
        THGuiRep::singleton().State(3);
    }
    EHOOK_DY(th13_update, (void*)0x470c04)
    {
        GameGuiBegin(IMPL_WIN32_DX9, !THAdvOptWnd::singleton().IsOpen());

        // Gui components update
        AnlyAutoRecTest();
        THGuiPrac::singleton().Update();
        THGuiRep::singleton().Update();
        THOverlay::singleton().Update();
        bool drawCursor = THAdvOptWnd::StaticUpdate() || THGuiPrac::singleton().IsOpen();
        THAuxDisp::singleton().Update();

        THAdvOptWnd::singleton().FpsUpd();
        GameGuiEnd(drawCursor);
    }
    EHOOK_DY(th13_render, (void*)0x470d27)
    {
        GameGuiRender(IMPL_WIN32_DX9);
    }
    HOOKSET_ENDDEF()

    HOOKSET_DEFINE(THInitHook)
    static __declspec(noinline) void THGuiCreate()
    {
        // Init
        GameGuiInit(IMPL_WIN32_DX9, 0x4dc6a8, 0x4dd0a8, 0x45cb40,
            Gui::INGAGME_INPUT_GEN2, 0x4e49fc, 0x4e49f8, 0,
            -1);

        // Gui components creation
        THGuiPrac::singleton();
        THGuiRep::singleton();
        THOverlay::singleton();
        THAuxDisp::singleton();

        // Hooks
        THMainHook::singleton().EnableAllHooks();
        THDataInit();

        // Reset thPracParam
        thPracParam.Reset();
    }
    static __declspec(noinline) void THInitHookDisable()
    {
        auto& s = THInitHook::singleton();
        s.th13_gui_init_1.Disable();
        s.th13_gui_init_2.Disable();
    }
    PATCH_DY(th13_disable_demo, (void*)0x44c5a1, "\xff\xff\xff\x7f", 4);
    EHOOK_DY(th13_disable_mutex, (void*)0x45c1bc)
    {
        pCtx->Eip = 0x45c340;
    }
    PATCH_DY(th13_startup_1, (void*)0x44c107, "\xeb", 1);
    PATCH_DY(th13_startup_2, (void*)0x44cdb7, "\xeb", 1);
    EHOOK_DY(th13_gui_init_1, (void*)0x44d4fd)
    {
        THGuiCreate();
        THInitHookDisable();
    }
    EHOOK_DY(th13_gui_init_2, (void*)0x45e1d5)
    {
        THGuiCreate();
        THInitHookDisable();
    }
    HOOKSET_ENDDEF()
}

void TH13Init()
{
    TH13::THInitHook::singleton().EnableAllHooks();
    TryKeepUpRefreshRate((void*)0x45dd99);
    if (GetModuleHandleW(L"vpatch_th13.dll")) {
        TryKeepUpRefreshRate((void*)((DWORD)GetModuleHandleW(L"vpatch_th13.dll") + 0x5cc7));
    }
}
}
