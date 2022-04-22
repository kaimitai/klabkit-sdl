#ifndef KLABKIT_CONSTANTS_H
#define KLABKIT_CONSTANTS_H

#include <array>

namespace kkit {

	namespace c {

		using byte = unsigned char;

		// filename constants
		constexpr char FILE_WALLS[]{ "WALLS" };
		constexpr char FILE_BOARDS[]{ "BOARDS" };
		constexpr char FILE_EXT_DAT[]{ "DAT" };
		constexpr char FILE_EXT_KZP[]{ "KZP" };
		constexpr char FILE_EXT_BMP[]{ "bmp" };

		// palette constants
		constexpr int TRANSP_PAL_INDEX{ 255 };
		// palette generator values
		constexpr std::array<std::array<byte, 3>, 16> PAL_GEN{ {
			{0,30,63},
			{28,34,60},
			{0,50,20},
			{41,52,28},
			{63,63,25},
			{63,63,63},
			{63,20,20},
			{63,0,63},
			{63,48,27},
			{63,40,25},
			{63,48,48},
			{45,63,45},
			{55,55,63},
			{63,40,63},
			{63,30,20},
			{55,25,30}
			} };

		// wall files
		constexpr int WALL_IMG_W{ 64 };
		constexpr int WALL_IMG_H{ 64 };
		constexpr int WALL_IMG_BYTES{ WALL_IMG_W * WALL_IMG_H };

		constexpr int WALL_DATA_OFFSET{ 1024 };

		// level files
		constexpr int MAP_W{ 64 };
		constexpr int MAP_H{ 64 };
		constexpr int MAP_BYTES_PER_TILE{ 2 };
		constexpr int MAP_BYTES{ MAP_W * MAP_H * MAP_BYTES_PER_TILE };

		// tiles grouped by type
		const std::vector<int> TILES
			// doors
		{ -1, 79, 80, 87,92,93,98,99,100,106,107,114,
			// monsters
		214, 221, 222, 225, 228, 233, 238, 244, 250, 261, 267, 272,
		277, 279, 284, 285, 289, 292, 302, 340, 355, 369, 376, 406,
		310, -2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,
			// walls
			0,1,2,3,4,5,6,7,8,9,10,11,
			14,15,18,19,20,21,22,23,24,25,26,27,
			28,29,30,31,32,33,34,35,36,37,38,39,
			40,41,42,43,44,45,46,47,48,49,50,51,
			52,53,54,55,56,57,58,59,60,61,62,63, // 60,61?
			64,65,66,67,68,69,70,71,72,73,75,76,
			77,78,81,82,83,84,85,86,-2,-2,-2,-2, // 84,85, 86?
			// decorative
			133,135,137,139,140,141,142,143,144,146,147,148,
			136, 419, 82, 129
		}
		;

	}
}

#endif

/*

 -- walls
[1]		1879
[2]		46
[3]		871
[4]		44
[5]		39
[6]		20
[7]		4278
[8]		6
[9]		63
[10]	6
[11]	2035
[12,13,14]	40

[15]	778
[16,17,18]	19

[19]	5422
[20]	54
[21]	21
[22]	12
[23]	2274
[24]	12
[25]	303
[26]	11
[27]	13
[28]	4803
[29]	63
[30]	882
[31]	373
[32]	1770
[33]	30
[34]	5417
[35]	54
[36]	1157
[37]	12
[38]	629
[39]	26
[40]	2952
[41]	232
[42]	487
[43]	26
[44]	387
[45]	20
[46]	1063
[47]	20
[48]	2398
[49]	67
[50]	600
[51]	126
[52]	1913
[53]	3483
[54]	2744
[55]	575
[56]	3207
[57]	173
[58]	516
[59]	2636
[60]	3330
  ? 61, 62
[63]	433
[64]	24
[65]	365
[66]	585
[67]	57
[68]	137
[69]	2
[70]	40
[71]	37
[72]	14
[73]	48
[74,75]	38

[76]	61
[77]	4
[78]	69
[79]	1588
[80]	59 -- DOOR: gold
[81]	37 -- DOOR: silver
[82]	48
[83]	31
[84]	14
? 85,86,87

 -- doors
[88]	724 -closed
[93]	3   -open

S

[123]	35
[124]	75
[125]	81
[126]	83
[127]	2
[128]	18
[129]	43
[130]	2s
[131]	1
[132]	174
[133]	146

 -- decorative

[134]	14 -- slot machine

[136]	4 -- slot out of order

[138]	23 -- soda

[140]	14 -- soda ooo

[141]	38 -- stairs

[142]	505 -- welcome

[143]	20 --exit

[145]	20 --colorful

[147]	51
[148]	167
[149]	165
[150]	1826
[151]	3
[152]	113 -- pole

[156]	140
[157]	222
[158]	645
[159]	19
[160]	45
[161]	592
[162]	57
[163]	215 -- table

[165]	17
[166]	39
[167]	42
[168]	255
[169]	2

-- powerups & items
[170]	63 -- warp
[171]	207
[172]	193
[173]	73
[174]	434
[175]	69
[176]	298
[177]	1
[178]	111
[179]	1
[180]	112
[181]	26
[182]	93
[183]	25
[184]	125
[185]	66
[186]	49
[187]	21
[188]	74
[189]	2
[190]	27 -- gold key
[191]	22
[192]	57
[193]	7 -- meat
[197]	7 -- star
[200]	7 -- rocket
[209]	1
[210]	7
[211]	8
[212]	1 --?

-- monsters/actors
[215]	32 --hive
[222]	10 -- destroyed hive
[223]	371 -- bat
[225]	1 -- bat last frame

[226]	82 -- skeleton
[227]	1

[229]	497 -- pirate
[233]	1 -- pirate last frame

[234]	131 -- witch

[239]	56 -- bee

[245]	300 -- spider

[251]	360 -- troll

[262]	175 -- green devil
[263]	1

[268]	67 -- blue monster

[273]	414 -- pirate #2
[277]	1

[278]	146 -- ufo
[279]	1

[280]	71 -- ball
[281]	1

[285]	70 -- hole
[286]	103 - walking hole
[288]	1

[290]	25 -- spectre
[291]	1

[293]	53 -- robot

[303]	119 -- small robot

[311]	10 -- sparky

[341]	2 -- lvl 1 boss
[342]	1

[356]	1 -- ken
[357]	1

[370]	2 --- andy
[371]	1

[376]	82 -- small andy

[405]	2 -- dog house - DECORATION?

[406]	210 -- WALL?

[407]	83 -- mummy

[419]	123 -- ken WALL?
[420]	14 -- WALL?
[421]	2 -- decor
[422]	1 -- decor
[423]	8 -- decor

[424]	1 -- globe (DECOR)

[425]	1 -- end walls (decor/wall)
[426]	1
[427]	1

[428]	21 -- don't cheat
[429]	2 -- game over
[439]	247 -- WALL?
[442]	1 -- ep1
[443]	1 -- ep2
[444]	1 -- ep3

*/
