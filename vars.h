#pragma once
unsigned int dirtCubeAtlas;
unsigned int grassCubeAtlas;
unsigned int cobblestoneCubeAtlas;

const char* cobblestone = "textures/cobblestone.png";
const char* dirt = "textures/dirt.png";
const char* grass_side = "textures/grass_side.png";
const char* grass = "textures/grass.png";
const char* glass = "textures/glass.png";
const char* crafting_table_top = "textures/crafting_table_top.png";
const char* crafting_table_side = "textures/crafting_table_side.png";
const char* crafting_table_front = "textures/crafting_table_front.png";
const char* planks_oak = "textures/planks_oak.png";
const char* log_oak = "textures/log_oak.png";
const char* log_oak_top = "textures/log_oak_top.png";
const char* flower_blue_orchid = "textures/flower_blue_orchid.png";
const char* leaves_oak = "textures/leaves_oak.png";


const char* const cubesTextures[7][6] = {
	{ // dirt
		// top        bottom       left    
		dirt, dirt, dirt,
		// right        back        front
		dirt, dirt, dirt
	},

	{ // cobblestone
		cobblestone, cobblestone, cobblestone, cobblestone, cobblestone, cobblestone
	},

	{ // grass
		grass, dirt, grass_side,
		grass_side, grass_side, grass_side
	},
	{
		glass, glass, glass, glass, glass, glass
	},
	{
		crafting_table_top, planks_oak, crafting_table_front,
		crafting_table_side, crafting_table_side, crafting_table_side
	},
	{
		log_oak_top, log_oak_top, log_oak,
		log_oak, log_oak, log_oak
	},
	{
		leaves_oak, leaves_oak, leaves_oak,
		leaves_oak, leaves_oak, leaves_oak
	}
};

const char* const floraTextures[1][2] = {
	{
		flower_blue_orchid, flower_blue_orchid
	}
};