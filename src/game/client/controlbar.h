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
#pragma once

#include "always.h"
#include "SubsystemInterface.h"

class WindowVideoManager;
class AnimateWindowManager;
class GameWindow;
class Drawable;
class ControlBarSchemeManager;
class CommandButton;
class CommandSet;
class Image;
class Object;
class ThingTemplate;
class Player;
class Coord3D;
class INI;
class Utf8String;

enum ControlBarStages
{
    DEFAULT,
    SQUISHED,
    LOW,
    HIDDEN,
};

enum ControlBarContext
{
    CB_CONTEXT_NONE,
    CB_CONTEXT_SIDE_SELECT,
    CB_CONTEXT_STRUCTURE_INVENTORY,
    CB_CONTEXT_BEACON,
    CB_CONTEXT_UNDER_CONSTRUCTION,
    CB_CONTEXT_MULTI_SELECT,
    CB_CONTEXT_6, // Unknown
    CB_CONTEXT_OBSERVER_LIST,
    CB_CONTEXT_OCL_TIMER,
    CB_CONTEXT_OVERFLOW, // Unsure, could not exist, or be default?
};

class ControlBar : public SubsystemInterface
{
public:
    struct TransportData
    { // these are up to debate, looks like field_8 has a double meaning as well.
        int control;
        int objectID;
        int field_8;
    };

public:
    ControlBar();
    virtual ~ControlBar();

    void Init();
    void Reset();
    void Update();
	
    // event listeners?
    void Do_Transport_Inventory_UI(Object *object, CommandSet *commandSet);

    // Methods
    void Add_Common_Commands(Drawable *drawable, char flag);
    void Animate_Special_Power_Shortcut(char flag);
    void Evaluate_Context_UI();
    CommandButton Find_Command_Button(Utf8String *name);
    CommandSet Find_Command_Set(Utf8String *name);
    CommandButton Find_Non_Const_Command_Button(Utf8String *name);
    void Get_Background_Marker_Position(int *x, int *y);
    void Get_Foreground_Marker_Position(int *x, int *y);
    int Get_CommandAvailability(
        CommandButton *button, Object *object, GameWindow *window, GameWindow *window2, char unknown);
    void Get_Star_Image(); // No return value?
    void Hide_Purchase_Science();
    void Hide_Special_Power_Shortcut();
    void Mark_UI_Dirty();
    void Init_Special_Power_Shortcut_Bar(Player *player);
    CommandButton New_Command_Button(Utf8String *name);
    CommandButton New_Command_Button_Override(CommandButton *button);
    void Populate_Beacon(Object *object);
    void Populate_Build_Queue(Object *object);
    void Populate_Build_Tooltip_Layout(CommandButton *button, GameWindow *window);
    void Populate_Command(Object *object);
    void Populate_Multi_Select();
    void Populate_Observer_Info_Window();
    void Populate_OCL_Timer(Object *object);
    void Populate_Purchase_Science(Player *player);
    void Populate_Special_Power_Shortcut(Player *player);
    void Populate_Structure_Inventory(Object *object);
    void Populate_Under_Construction(Object *object);
    void Reset_Build_Queue_Data();
    void Set_Control_Command(GameWindow *window, CommandButton *button);
    void Set_Portrait_By_Object(Object *object);
    void Set_Up_Down_Images();
    void Show_Rally_Point_Marker(Coord3D *location);
    void Show_Special_Power_Shotcut();
    void Switch_To_Context(ControlBarContext context, Drawable *drawable);
    void Trigger_Radar_Attack_Glow();
    void Update_Construction_Text_Display(Object *object);
    void Update_Context_Command();
    void Update_Context_Under_Construction();
    void Update_OCL_Timer_Text_Displayer(uint32_t, float);
    void Update_Slot_Exit_Image(Image* image);
    void Update_Special_Power_Shortcut();
	//TODO not all methods are present, because i couldn't trace anything towards calling sites to ID their location
	
    static Image Calculate_Veterancy_Overlay_For_Object(Object *object);
    static Image Calculate_Veterancy_Overlay_For_Object(ThingTemplate *thing);
    static void Init_Observer_Controls();
    static void Parse_Command_Button_Definition(INI *ini);
    static void Parse_Command_Set_Definition(INI *ini);
    static void Populate_Button_Proc(Object *object, int uknownType); // type issue on 2nd param
    static void Populate_Inv_Data_Callback(Object *object, int value); // 2nd param issue?
    static void Populate_Observer_List();
    static void Process_Command_UI(GameWindow *window, int gadgetGameMessage); // not a clue what this 2nd param type is
    static void Reset_Contain_Data();

private: 
	//TODO resolve all padded and unknown variables
    WindowVideoManager *m_videoManager;
    AnimateWindowManager *m_AnimateWindowManager1;
    AnimateWindowManager *m_AnimateWindowManager2;
    AnimateWindowManager *m_AnimateWindowManager3;
    int m_controlBarParentPosX;
    int m_controlBarParentPosY;
    ControlBarStages m_currentControlBarStage;
    bool m_UIDirty;
    CommandButton *m_CommandButtons;
    CommandSet *m_commandSets;
    ControlBarSchemeManager *m_controlBarSchemeManager;
    GameWindow *m_contextParent;
    GameWindow *m_purchaseScience; // unsure
    GameWindow *m_commandWindow;
    GameWindow *m_productionQueueContextWindow;
    GameWindow *m_beaconContextWindow;
    GameWindow *m_underConstructionContextWindow;
    GameWindow *m_observerPlayerInfoWindow;
    GameWindow *m_observerPlayerListContextWindow;
    GameWindow *m_OCLTimerContextWindow;
    Drawable *m_currentSelectedDrawable;
    ControlBarContext m_currContext;
    int m_rallyPointDrawableID; // type unknown, needs drawable implementation
    float m_displayedConstructPercent;
    uint32_t m_alwaysZeroUint; // this is always 0, and used to check some zero it's weird
    int m_unknownObjectContext1; // needs Object to be defined types
    int m_unknownObjectContext2; // needs Object to be defined types
    GameWindow *m_rightHUDWindow;
    GameWindow *m_rightHUDCameoWindow;
    GameWindow *m_unitUpgradeWindows[5];
    GameWindow *m_unitySelectedWindow;
    GameWindow *m_popUpCommunicatorWindow;
    GameWindow *m_generalsExpPointsWindow;
    GameWindow *m_ScieneRank1Windows[4];
    GameWindow *m_ScieneRank3Windows[15];
    GameWindow *m_ScieneRank8Windows[4];
    GameWindow *m_specialPowerShortcutWindows[10]; // unsure what exactly they are.
    GameWindow *m_specialPowerShortcutWindows2[10]; // unsure what exactly they are.
    char m_PADDING[52]; // unknown variables
    int m_unlockedSpecialPowerShortcutCount; // Unsure if this is exact
    int m_PADDING2; // unknown variable
    GameWindow *m_specialPowerShortcutWindow;
    GameWindow *m_commandWindows[18];
    CommandButton *m_commonCommands[18];
    TransportData m_transportData[10];
    char m_PADDING3[32]; // unknown variables
    GameWindow *m_queueButton; // unsure what this does
    char m_PADDING5[36]; // unknown variables
    Image *m_barButtonGeneralStarON;
    Image *m_barButtonGeneralStarOFF;
    Image *m_upDownImageLow1; // unknown exact name
    Image *m_upDownImageLow2; // unknown exact name
    Image *m_upDownImageLow3; // unknown exact name
    Image *m_upDownImageDefault1; // unknown exact name
    Image *m_upDownImageDefault2; // unknown exact name
    Image *m_upDownImageDefault3; // unknown exact name
    char m_PADDING6[8]; // unknown variables
    Image *m_starImageA; // unknown exact name
    Image *m_starImageB; // unknown exact name
    bool m_playerIDSet; // Uncertain meaning
    int m_playerID; // uncertain
    int m_foregroundMarkerPosX;
    int m_foregroundMarkerPosY;
    int m_backgroundMarkerPosX;
    int m_backgroundMarkerPosY;
    bool m_radarAttackGlowActive;
    int m_radarAttackGlowCounter;
    GameWindow *m_radarAttackGlowWindow;
};