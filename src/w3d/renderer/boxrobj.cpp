/**
 * @file
 *
 * @author Jonathan Wilson
 *
 * @brief Box render objects
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "boxrobj.h"
#ifdef GAME_DLL
#include "hooker.h"
#endif

void BoxRenderObjClass::Init() 
{
#ifdef GAME_DLL
    Call_Function<void>(PICK_ADDRESS(0x00848A90, 0x0054C9C0));
#endif
}

void BoxRenderObjClass::Shutdown()
{
#ifdef GAME_DLL
    Call_Function<void>(PICK_ADDRESS(0x00848B90, 0x0054CB50));
#endif
}
