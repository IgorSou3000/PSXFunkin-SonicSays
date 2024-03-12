/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "sonic_old.h"

#include "psx/mem.h"
#include "psx/archive.h"
#include "psx/stage.h"
#include "psx/main.h"

//Sonic_Old character structure
enum
{
	Sonic_Old_ArcMain_Idle0,
	Sonic_Old_ArcMain_Idle1,
	Sonic_Old_ArcMain_Idle2,
	Sonic_Old_ArcMain_Idle3,
	Sonic_Old_ArcMain_Idle4,
	Sonic_Old_ArcMain_Left0,
	Sonic_Old_ArcMain_Left1,
	Sonic_Old_ArcMain_Left2,
	Sonic_Old_ArcMain_Left3,
	Sonic_Old_ArcMain_Down0,
	Sonic_Old_ArcMain_Down1,
	Sonic_Old_ArcMain_Down2,
	Sonic_Old_ArcMain_Down3,
	Sonic_Old_ArcMain_Up0,
	Sonic_Old_ArcMain_Up1,
	Sonic_Old_ArcMain_Up2,
	Sonic_Old_ArcMain_Up3,
	Sonic_Old_ArcMain_Right0,
	Sonic_Old_ArcMain_Right1,
	Sonic_Old_ArcMain_Right2,
	Sonic_Old_ArcMain_Right3,
	
	Sonic_Old_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Sonic_Old_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Sonic_Old;

//Sonic_Old character definitions
static const CharFrame char_sonic_old_frame[] = {
	{Sonic_Old_ArcMain_Idle0, {  0,   0, 124, 163}, { 64, 163}}, //0 idle 1
	{Sonic_Old_ArcMain_Idle0, {125,   0, 124, 163}, { 64, 163}}, //1 idle 2
	{Sonic_Old_ArcMain_Idle1, {  0,   0, 122, 163}, { 64, 163}}, //2 idle 3
	{Sonic_Old_ArcMain_Idle1, {123,   0, 121, 164}, { 66, 164}}, //3 idle 4
	{Sonic_Old_ArcMain_Idle2, {  0,   0, 121, 164}, { 71, 164}}, //4 idle 5
	{Sonic_Old_ArcMain_Idle2, {122,   0, 126, 165}, { 77, 165}}, //5 idle 6
	{Sonic_Old_ArcMain_Idle3, {  0,   0, 131, 165}, { 82, 165}}, //6 idle 7
	{Sonic_Old_ArcMain_Idle4, {  0,   0, 134, 166}, { 85, 166}}, //7 idle 8
	
	{Sonic_Old_ArcMain_Left0, {  0,   0, 169, 180}, {107, 180}}, //8 left 1
	{Sonic_Old_ArcMain_Left1, {  0,   0, 174, 183}, {101, 183}}, //9 left 2
	{Sonic_Old_ArcMain_Left2, {  0,   0, 177, 184}, {100, 184}}, //10 left 3
	{Sonic_Old_ArcMain_Left3, {  0,   0, 179, 185}, { 99, 185}}, //11 left 4
	
	{Sonic_Old_ArcMain_Down0, {  0,   0, 165, 157}, {105, 157}}, //12 down 1
	{Sonic_Old_ArcMain_Down1, {  0,   0, 164, 162}, {103, 162}}, //13 down 2
	{Sonic_Old_ArcMain_Down2, {  0,   0, 163, 163}, {102, 163}}, //14 down 3
	{Sonic_Old_ArcMain_Down3, {  0,   0, 162, 165}, {101, 165}}, //15 down 4
	
	{Sonic_Old_ArcMain_Up0, {  0,   0, 183, 221}, {102, 221}}, //16 up 1
	{Sonic_Old_ArcMain_Up1, {  0,   0, 176, 212}, {100, 212}}, //17 up 2
	{Sonic_Old_ArcMain_Up2, {  0,   0, 173, 211}, { 99, 211}}, //18 up 3
	{Sonic_Old_ArcMain_Up3, {  0,   0, 171, 208}, { 98, 208}}, //19 up 4
	
	{Sonic_Old_ArcMain_Right0, {  0,   0, 149, 192}, { 56, 192}}, //20 right 1
	{Sonic_Old_ArcMain_Right1, {  0,   0, 150, 191}, { 64, 191}}, //21 right 2
	{Sonic_Old_ArcMain_Right2, {  0,   0, 150, 191}, { 66, 191}}, //22 right 3
	{Sonic_Old_ArcMain_Right3, {  0,   0, 150, 191}, { 69, 191}}, //23 right 4
};

static const Animation char_sonic_old_anim[CharAnim_Max] = {
	{1, (const u8[]){ 0,  1,  2,  3,  4,  5,  6,  7, ASCR_BACK, 1}}, //CharAnim_Idle
	{1, (const u8[]){ 8,  9, 10, 11, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{1, (const u8[]){12, 13, 14, 15, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{1, (const u8[]){16, 17, 18, 19, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{1, (const u8[]){20, 21, 22, 23, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt

	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special1
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special2
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special3
};

//Sonic_Old character functions
void Char_Sonic_Old_SetFrame(void *user, u8 frame)
{
	Char_Sonic_Old *this = (Char_Sonic_Old*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_sonic_old_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Sonic_Old_Tick(Character *character)
{
	Char_Sonic_Old *this = (Char_Sonic_Old*)character;
	
	Character_CheckAnimationUpdate(character);
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Sonic_Old_SetFrame);
	Character_Draw(character, &this->tex, &char_sonic_old_frame[this->frame]);
}

void Char_Sonic_Old_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Sonic_Old_Free(Character *character)
{
	Char_Sonic_Old *this = (Char_Sonic_Old*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Sonic_Old_New(fixed_t x, fixed_t y)
{
	//Allocate sonic_old object
	Char_Sonic_Old *this = Mem_Alloc(sizeof(Char_Sonic_Old));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Sonic_Old_New] Failed to allocate sonic_old object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Sonic_Old_Tick;
	this->character.set_anim = Char_Sonic_Old_SetAnim;
	this->character.free = Char_Sonic_Old_Free;
	
	Animatable_Init(&this->character.animatable, char_sonic_old_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	//Health Icon
	this->character.health_i = 2;

	//Health Bar
	this->character.health_b = 0xFF0066CC;

	//Character scale
	this->character.scale = FIXED_DEC(1,1);
	
	this->character.focus_x = FIXED_DEC(1,1);
	this->character.focus_y = FIXED_DEC(-125,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\SONICOLD.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //Sonic_Old_ArcMain_Idle0
		"idle1.tim", //Sonic_Old_ArcMain_Idle1
		"idle2.tim", //Sonic_Old_ArcMain_Idle2
		"idle3.tim", //Sonic_Old_ArcMain_Idle3
		"idle4.tim", //Sonic_Old_ArcMain_Idle4
		"left0.tim",  //Sonic_Old_ArcMain_Left0
		"left1.tim",  //Sonic_Old_ArcMain_Left1
		"left2.tim",  //Sonic_Old_ArcMain_Left2
		"left3.tim",  //Sonic_Old_ArcMain_Left3
		"down0.tim",  //Sonic_Old_ArcMain_Down0
		"down1.tim",  //Sonic_Old_ArcMain_Down1
		"down2.tim",  //Sonic_Old_ArcMain_Down2
		"down3.tim",  //Sonic_Old_ArcMain_Down3
		"up0.tim",    //Sonic_Old_ArcMain_Up0
		"up1.tim",    //Sonic_Old_ArcMain_Up1
		"up2.tim",    //Sonic_Old_ArcMain_Up2
		"up3.tim",    //Sonic_Old_ArcMain_Up3
		"right0.tim", //Sonic_Old_ArcMain_Right0
		"right1.tim", //Sonic_Old_ArcMain_Right1
		"right2.tim", //Sonic_Old_ArcMain_Right2
		"right3.tim", //Sonic_Old_ArcMain_Right3
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
