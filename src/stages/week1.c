/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "week1.h"

#include "psx/archive.h"
#include "psx/mem.h"

//Stage background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_back0; //Sky
} Back_Week1;

//Stage functions
void Back_Week1_Tick(StageBack *back)
{
	Back_Week1 *this = (Back_Week1*)back;

	if (((stage.song_beat >= 32 && stage.song_beat <= 95) || 
    (stage.song_beat >= 112 && stage.song_beat <= 143) || 
    (stage.song_beat >= 160 && stage.song_beat <= 223) || 
    (stage.song_beat >= 320 && stage.song_beat <= 383)) && stage.song_beat % 2 == 0 && stage.save.canbump && stage.flag & STAGE_FLAG_JUST_BEAT)
	{
		stage.bump += FIXED_DEC(3,100); //0.03
		stage.character_bump += FIXED_DEC(15,1000); //0.015
	}
	
	if (stage.song_beat >= 112 && stage.song_beat <= 143 && (stage.song_beat % 2) == 1 && stage.flag & STAGE_FLAG_JUST_BEAT)
	{
		stage.bump += FIXED_DEC(2,100); //0.02
		stage.character_bump += FIXED_DEC(36,1000); //0.036
	}
}

void Back_Week1_DrawBG(StageBack *back)
{
	Back_Week1 *this = (Back_Week1*)back;
	
	fixed_t fx, fy;
	
	fx = stage.camera.x / 2;
	fy = stage.camera.y / 2;
	
	//Draw Sky
	RECT sky_src = {0, 0, 255, 255};
	RECT_FIXED sky_dst = {
		FIXED_DEC(-185,1) - fx,
		FIXED_DEC(-150,1) - fy,
		FIXED_DEC(380,1),
		FIXED_DEC(284,1)
	};

	Debug_MoveTexture(&sky_dst, 1, "sky", fx, fy);
	
	Stage_DrawTex(&this->tex_back0, &sky_src, &sky_dst, stage.camera.bzoom);
}

void Back_Week1_Free(StageBack *back)
{
	Back_Week1 *this = (Back_Week1*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Week1_New(void)
{
	//Allocate background structure
	Back_Week1 *this = (Back_Week1*)Mem_Alloc(sizeof(Back_Week1));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.tick = Back_Week1_Tick;
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Week1_DrawBG;
	this->back.free = Back_Week1_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK1\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Mem_Free(arc_back);
	
	return (StageBack*)this;
}
