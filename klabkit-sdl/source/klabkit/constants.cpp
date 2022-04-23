#include <algorithm>
#include "constants.h"

std::vector<int> kkit::c::TILES 	// non-tiles; no tile, start position
{ -1, -2, -3, -3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,
// doors
79, 80, 87,92,93,98,99,100,106,107,114,115,122,-3,
// monsters
214, 221, 222, 225, 228, 233, 238, 244, 250, 261, 267, 272,	277, 279,
284, 285, 289, 292, 302, 340, 355, 369, 376, 406,310,-3,-3,-3,
// walls
0,1,2,3,4,5,6,7,8,9,10,11,14,15,
18,19,20,21,22,23,24,25,26,27,28,29,30,31,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,
46,47,48,49,50,51,52,53,54,55,56,57,58,59,
62,63,64,65,66,67,68,69,70,71,72,73,75,76,
 77, 78, 81, 82, 83,122,123,124,125,126,127,128,129,130,
131,132,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,
// decorative
133,135,137,139,140,141,142,144,146,147,148,149,150,151,
155,156,157,158,159,160,161,162,164,165,166,167,168,-3,
// powerups & items
169,170,171,172,173,174,175,176,177,178,179,180,181,182,
183,184,185,186,187,188,189,190,191,192,196,199,208,209,
210,211,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,
// rarely used sprites
404,405,406,418,419,420,421,422,423,424,425,426,427,428,
438,441,442,443,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3
};

void kkit::c::populate_tilemap(int p_wall_count) {
	for (int i{ -2 }; i < p_wall_count; ++i) {
		if (std::find(begin(c::TILES), end(c::TILES), i) == end(c::TILES))
			c::TILES.push_back(i);
	}
}