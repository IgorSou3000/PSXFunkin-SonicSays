/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "sonic.h"

#include "psx/mem.h"
#include "psx/archive.h"
#include "psx/stage.h"
#include "psx/main.h"

//Sonic character structure
enum
{
	Sonic_ArcMain_Idle0,
	Sonic_ArcMain_Idle1,
	Sonic_ArcMain_Idle2,
	Sonic_ArcMain_Idle3,
	Sonic_ArcMain_Left0,
	Sonic_ArcMain_Left1,
	Sonic_ArcMain_Down0,
	Sonic_ArcMain_Down1,
	Sonic_ArcMain_Up0,
	Sonic_ArcMain_Up1,
	Sonic_ArcMain_Right,
	
	Sonic_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Sonic_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Sonic;

//Sonic character definitions
static const CharFrame char_sonic_frame[] = {
	{Sonic_ArcMain_Idle0, {  0,   0, 140, 186}, { 69, 186}}, //0 idle 1
	{Sonic_ArcMain_Idle1, {  0,   0, 134, 194}, { 75, 194}}, //1 idle 2
	{Sonic_ArcMain_Idle2, {  0,   0, 132, 196}, { 77, 196}}, //2 idle 3
	{Sonic_ArcMain_Idle3, {  0,   0, 132, 196}, { 77, 196}}, //3 idle 4
	
	{Sonic_ArcMain_Left0, {  0,   0, 164, 185}, {105, 185}}, //4 left 1
	{Sonic_ArcMain_Left1, {  0,   0, 171, 186}, {103, 186}}, //5 left 2
	
	{Sonic_ArcMain_Down0, {  0,   0, 162, 169}, { 96, 169}}, //6 down 1
	{Sonic_ArcMain_Down1, {  0,   0, 156, 182}, { 91, 182}}, //7 down 2
	
	{Sonic_ArcMain_Up0, {  0,   0, 171, 230}, { 91, 230}}, //8 up 1
	{Sonic_ArcMain_Up1, {  0,   0, 165, 214}, { 91, 214}}, //9 up 2
	
	{Sonic_ArcMain_Right, {  0,   0, 127, 187}, { 39, 187}}, //10 right 1
	{Sonic_ArcMain_Right, {127,   0, 128, 187}, { 54, 187}}, //11 right 2
};

static const Animation char_sonic_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 4,  5, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 6,  7, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 8,  9, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){10, 11, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt

	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special1
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special2
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special3
};

//Sonic character functions
void Char_Sonic_SetFrame(void *user, u8 frame)
{
	Char_Sonic *this = (Char_Sonic*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_sonic_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Sonic_Tick(Character *character)
{
	Char_Sonic *this = (Char_Sonic*)character;
	
	Character_CheckAnimationUpdate(character);
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Sonic_SetFrame);
	Character_Draw(character, &this->tex, &char_sonic_frame[this->frame]);
}

void Char_Sonic_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Sonic_Free(Character *character)
{
	Char_Sonic *this = (Char_Sonic*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Sonic_New(fixed_t x, fixed_t y)
{
	//Allocate sonic object
	Char_Sonic *this = Mem_Alloc(sizeof(Char_Sonic));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Sonic_New] Failed to allocate sonic object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Sonic_Tick;
	this->character.set_anim = Char_Sonic_SetAnim;
	this->character.free = Char_Sonic_Free;
	
	Animatable_Init(&this->character.animatable, char_sonic_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	//Health Icon
	this->character.health_i = 1;

	//Health Bar
	this->character.health_b = 0xFF3A91E8;

	//Character scale
	this->character.scale = FIXED_DEC(1,1);
	
	this->character.focus_x = FIXED_DEC(1,1);
	this->character.focus_y = FIXED_DEC(-125,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\SONIC.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //Sonic_ArcMain_Idle0
		"idle1.tim", //Sonic_ArcMain_Idle1
		"idle2.tim", //Sonic_ArcMain_Idle2
		"idle3.tim", //Sonic_ArcMain_Idle3
		"left0.tim",  //Sonic_ArcMain_Left0
		"left1.tim",  //Sonic_ArcMain_Left1
		"down0.tim",  //Sonic_ArcMain_Down0
		"down1.tim",  //Sonic_ArcMain_Down1
		"up0.tim",    //Sonic_ArcMain_Up0
		"up1.tim",    //Sonic_ArcMain_Up1
		"right.tim", //Sonic_ArcMain_Right
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
