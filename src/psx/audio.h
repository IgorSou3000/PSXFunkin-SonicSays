/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "psx.h"
#include "io.h"

typedef u32 SFX;

//XA enumerations
typedef enum
{
  XA_Menu,   //MENU.XA
  XA_Week1A, //WEEK1A.XA
  XA_Week1B, //WEEK1B.XA
  
  XA_Max,
} XA_File;

typedef enum
{
  //MENU.XA
  XA_GettinFreaky,
  XA_GameOver,
  //WEEK1A.XA
  XA_No_Good,
  XA_No_Good_Old,
  
  XA_TrackMax,
} XA_Track;

//Audio functions
void Audio_Init(void);
void Audio_Quit(void);
void Audio_PlayXA_Track(XA_Track track, u8 volume, u8 channel, boolean loop);
void Audio_SeekXA_Track(XA_Track track);
void Audio_PauseXA(void);
void Audio_ResumeXA(void);
void Audio_StopXA(void);
void Audio_ChannelXA(u8 channel);
s32 Audio_TellXA_Sector(void);
s32 Audio_TellXA_Milli(void);
boolean Audio_PlayingXA(void);
void Audio_WaitPlayXA(void);
void Audio_ProcessXA(void);
u16 Audio_GetLength(XA_Track track);

void Audio_ClearAlloc(void);
u32 Audio_LoadSFX(IO_Data sfx_data);
void Audio_PlaySFX(SFX addr, u16 volume);
