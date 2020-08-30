/**
 * @file
 *
 * @author Smileynator
 *
 * @brief Class that handles the main code for the controlbar. The mayor UI ingame.
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "controlbar.h"
#ifdef GAME_DLL
#include "hooker.h"
#endif

ControlBar::ControlBar() : 
	m_CommandButtons(nullptr), 
	m_commandSets(nullptr), 
	m_controlBarSchemeManager(nullptr),
    m_PADDING5(),
    m_PADDING3(),
    m_currContext(CB_CONTEXT_NONE),
    m_controlBarParentPosX(0),
    m_controlBarParentPosY(0),
    m_playerIDSet(false),
    m_barButtonGeneralStarOFF(nullptr),
    m_barButtonGeneralStarON(nullptr),
    m_UIDirty(false),
	//TODO 2 variables need to be 0x64000000 here
    m_contextParent(nullptr),
    m_purchaseScience(nullptr),
    m_commandWindow(nullptr),
    m_productionQueueContextWindow(nullptr),
    m_beaconContextWindow(nullptr),
    m_underConstructionContextWindow(nullptr),
    m_observerPlayerInfoWindow(nullptr),
    m_observerPlayerListContextWindow(nullptr),
    m_OCLTimerContextWindow(nullptr),
    m_specialPowerShortcutWindow(nullptr),
    m_PADDING2(0),
    m_generalsExpPointsWindow(nullptr),
    m_rightHUDWindow(nullptr),
    m_rightHUDCameoWindow(nullptr),
    m_unitSelectedWindow(nullptr),
    m_popUpCommunicatorWindow(nullptr),
    m_currentSelectedDrawable(nullptr),
    m_rallyPointDrawableID(0),
    m_displayedConstructPercent(-1.0),
    m_alwaysZeroUint(0),
    m_unknownObjectContext1(0),
	// TODO set 0x2C4 to 0x3D0 to 0xFFFFFFFF
    m_unknownObjectContext2(0),
    m_videoManager(nullptr),
    m_AnimateWindowManager1(nullptr),
    m_AnimateWindowManager2(nullptr),
    m_AnimateWindowManager3(nullptr),
    m_upDownImageLow1(nullptr),
    m_upDownImageLow2(nullptr),
    m_upDownImageLow3(nullptr),
    m_upDownImageDefault1(nullptr),
    m_upDownImageDefault2(nullptr),
    m_upDownImageDefault3(nullptr),
    m_starImageA(nullptr),
    m_starImageB(nullptr),
    m_PADDING6(),
    m_radarAttackGlowActive(false),
    m_radarAttackGlowCounter(0),
    m_radarAttackGlowWindow(nullptr)
{
    int arr_size = sizeof(m_commonCommands) / sizeof(m_commonCommands[0]);
    for (int i = 0; i < arr_size; i++) {
        m_commonCommands[i] = nullptr;
    }
    arr_size = sizeof(m_commandWindows) / sizeof(m_commandWindows[0]);
    for (int i = 0; i < arr_size; i++) {
        m_commandWindows[i] = nullptr;
    }
    arr_size = sizeof(m_ScieneRank1Windows) / sizeof(m_ScieneRank1Windows[0]);
    for (int i = 0; i < arr_size; i++) {
        m_ScieneRank1Windows[i] = nullptr;
    }
    arr_size = sizeof(m_ScieneRank3Windows) / sizeof(m_ScieneRank3Windows[0]);
    for (int i = 0; i < arr_size; i++) {
        m_ScieneRank3Windows[i] = nullptr;
    }
    arr_size = sizeof(m_ScieneRank8Windows) / sizeof(m_ScieneRank8Windows[0]);
    for (int i = 0; i < arr_size; i++) {
        m_ScieneRank8Windows[i] = nullptr;
    }
    arr_size = sizeof(m_specialPowerShortcutWindows2) / sizeof(m_specialPowerShortcutWindows2[0]);
    for (int i = 0; i < arr_size; i++) {
        m_specialPowerShortcutWindows2[i] = nullptr;
    }
    Reset_Build_Queue_Data();
    // TODO, make local call instead of the pointer call
    //Reset_Contain_Data();
#ifdef GAME_DLL
    Call_Function<void>(PICK_ADDRESS(0x005a3750, 0x0)); // hah, suck it editor
#endif
}

ControlBar::~ControlBar() 
{
#ifdef GAME_DLL
    Call_Function<void>(PICK_ADDRESS(0x0045ce30, 0x0)); // hah, suck it editor
#endif
}

void ControlBar::Init() 
{
#ifdef GAME_DLL
    Call_Function<void>(PICK_ADDRESS(0x0045d010, 0x0)); // hah, suck it editor
#endif
}

void ControlBar::Reset() 
{
#ifdef GAME_DLL
    Call_Function<void>(PICK_ADDRESS(0x0045e2a0, 0x0)); // hah, suck it editor
#endif
}

void ControlBar::Update() 
{
#ifdef GAME_DLL
    Call_Function<void>(PICK_ADDRESS(0x0045e4c0, 0x0)); // hah, suck it editor
#endif
}

void ControlBar::Reset_Build_Queue_Data() 
{
    int arr_size = sizeof(m_transportData) / sizeof(m_transportData[0]);
    for (int i = 0; i < arr_size; i++) {
        m_transportData[i].control = 0;
        m_transportData[i].objectID = 0;
        m_transportData[i].field_8 = 0;
    }
}

void ControlBar::Get_Background_Marker_Position(int *x, int *y) 
{
    *x = m_backgroundMarkerPosX;
    *y = m_backgroundMarkerPosY;
}

void ControlBar::Get_Foreground_Marker_Position(int *x, int *y)
{
    *x = m_foregroundMarkerPosX;
    *y = m_foregroundMarkerPosY;
}

Image *ControlBar::Calculate_Veterancy_Overlay_For_Object(Object *object)
{
    if (object == nullptr) {
        return nullptr;
	}
    int lvl = 0;
    //object->getVeterancyLevel(object);
	//TODO after object is known
    switch (lvl) {
        case 1:
            return g_veteranIconLvl1;
        case 2:
            return g_veteranIconLvl2;
        case 3:
            return g_veteranIconLvl3;
        default:
            return nullptr;
	}
}
