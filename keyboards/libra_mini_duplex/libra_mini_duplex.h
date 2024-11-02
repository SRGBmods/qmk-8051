#ifndef LIBRA_MINI_DUPLEX_H 
#define LIBRA_MINI_DUPLEX_H

//#include "quantum.h"

#define LAYOUT_default( \
	K000, K001, K002, K003, K004, K005,      K400, K401, K402, K403, K404, K405, K705, \
	K100, K101, K102, K103, K104, K105,       K500, K501, K502, K503, K504, K505,      \
	K200, K201, K202, K203, K204, K205,    K600, K601, K602, K603, K604, K605, K704,   \
	K300, K301,       K303,       K304,    K700, K701,       K800,       K702, K703,   \
	                                                   K803, K801, K804  \
) { \
	{ K000,  K001,  K002,  K003,  K004,  K005 }, \
	{ K100,  K101,  K102,  K103,  K104,  K105 }, \
	{ K200,  K201,  K202,  K203,  K204,  K205 }, \
	{ K300,  K301,  KC_NO, K303,  K304,  KC_NO}, \
    { K400,  K401,  K402,  K403,  K404,  K405 }, \
    { K500,  K501,  K502,  K503,  K504,  K505 }, \
    { K600,  K601,  K602,  K603,  K604,  K605 }, \
    { K700,  K701,  K702,  K703,  K704,  K705 }, \
    { K800,  K801,  KC_NO, K803,  K804, KC_NO }  \
}

#endif
