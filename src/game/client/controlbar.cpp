#include "controlbar.h"
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