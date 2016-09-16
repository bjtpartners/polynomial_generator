
#include "poly.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct params_t {
	char     *name;
	uint8_t   rate;
	uint32_t  m_state;
	bool      term;
	uint32_t  polyDiv;
	uint32_t  polys[];
} params_t;

/*
	========================================================

	All polynomials used in GSM 05.03

	Order 4 :

	G0 = 1 + D3 + D4                   (0001 1001) => 0x0019
	G1 = 1 + D  + D3 + D4              (0001 1011) => 0x001B
	G2 = 1 + D2 + D4                   (0001 1010) => 0x001A
	G3 = 1 + D  + D2 + D3 + D4         (0001 1111) => 0x001F

	Order 6 :

	G4 = 1 + D2 + D3 + D5 + D6         (0110 1101) => 0x006D
	G5 = 1 + D  + D4 + D6              (0101 0011) => 0x0053
	G6 = 1 + D  + D2 + D3 + D4 + D6    (0101 1111) => 0x005F

	========================================================

	The max state 'm_state' calcule:

	m_state = 2^(the greater order);

	========================================================
*/

/* Full-rate Speech */

params_t tch_fr = {
	.name    = "TCH_FR",
	.rate    = 2,
	.m_state = 16,
	.term    = false,
	/* G0 - G1 */
	.polys   = { 0x0019, 0x001B, },
	.polyDiv = 1,
};

params_t tch_afs_speech_12_2 = {
	.name    = "TCH_AFS_speech_12_2",
	.rate    = 2,
	.m_state = 16,
	.term    = true,
	/* G0/G0 - G1/G0 */
	.polys   = { 0x0019, 0x001B, },
	.polyDiv = 0x0019,
};

params_t tch_afs_speech_10_2 = {
	.name    = "TCH_AFS_speech_10_2",
	.rate    = 3,
	.m_state = 16,
	.term    = true,
	/* G1/G3 - G2/G3 - G3/G3 */
	.polys   = { 0x001B, 0x001A, 0x001F },
	.polyDiv = 0x001F,
};

params_t tch_afs_speech_7_95 = {
	.name    = "TCH_AFS_speech_7_95",
	.rate    = 3,
	.m_state = 64,
	.term    = true,
	/* G4/G4 - G5/G4 - G6/G4 */
	.polys   = { 0x006D, 0x0053, 0x005F },
	.polyDiv = 0x006D,
};

params_t tch_afs_speech_7_4 = {
	.name    = "TCH_AFS_speech_7_4",
	.rate    = 3,
	.m_state = 16,
	.term    = true,
	/* G1/G3 - G2/G3 - G3/G3 */
	.polys   = { 0x001B, 0x001A, 0x001F},
	.polyDiv = 0x001F,
};

params_t tch_afs_speech_6_7 = {
	.name    = "TCH_AFS_speech_6_7",
	.rate    = 4,
	.m_state = 16,
	.term    = true,
	/* G1/G3 - G2/G3 - G3/G3 - G3/G3 */
	.polys   = { 0x001B, 0x001A, 0x001F, 0x001F},
	.polyDiv = 0x001F,
};

params_t tch_afs_speech_5_9 = {
	.name    = "TCH_AFS_speech_5_9",
	.rate    = 4,
	.m_state = 64,
	.term    = true,
	/* G4/G6 - G5/G6 - G6/G6 - G6/G6 */
	.polys   = { 0x006D, 0x0053, 0x005F, 0x005F },
	.polyDiv = 0x005F,
};

params_t tch_afs_speech_5_15 = {
	.name    = "TCH_AFS_speech_5_15",
	.rate    = 5,
	.m_state = 16,
	.term    = true,
	/* G1/G3 - G1/G3 - G2/G3 - G3/G3 - G3/G3 */
	.polys   = { 0x001B, 0x001B, 0x001A, 0x001F, 0x001F },
	.polyDiv = 0x001F,
};

params_t tch_afs_speech_4_75 = {
	.name    = "TCH_AFS_speech_4_75",
	.rate    = 5,
	.m_state = 64,
	.term    = true,
	/* G4/G6 - G4/G6 - G5/G6 - G6/G6 - G6/G6 */
	.polys   = { 0x006D, 0x006D, 0x0053, 0x005F, 0x005F},
	.polyDiv = 0x005F,
};

/* Half-rate Speech */

params_t tch_hs = {
	.name    = "TCH_HS",
	.rate    = 3,
	.m_state = 0,
	.term    = true,
	/* G4 - G5 - G6 */
	.polys   = { 0x006D, 0x0053, 0x005F},
	.polyDiv = 1,
};

params_t tch_ahs_speech_7_95 = {
	.name    = "TCH_AHS_speech_7_95",
	.rate    = 2,
	.m_state = 16,
	.term    = true,
	/* G0/G0 - G1/G0 */
	.polys   = { 0x0019, 0x001B, },
	.polyDiv = 0x0019,
};

params_t tch_ahs_speech_7_4 = {
	.name    = "TCH_AHS_speech_7_4",
	.rate    = 2,
	.m_state = 16,
	.term    = true,
	/* G0/G0 - G1/G0 */
	.polys   = { 0x0019, 0x001B, },
	.polyDiv = 0x0019,
};

params_t tch_ahs_speech_6_7 = {
	.name    = "TCH_AHS_speech_6_7",
	.rate    = 2,
	.m_state = 16,
	.term    = true,
	/* G0/G0 - G1/G0 */
	.polys   = { 0x0019, 0x001B, },
	.polyDiv = 0x0019,
};

params_t tch_ahs_speech_5_9 = {
	.name    = "TCH_AHS_speech_5_9",
	.rate    = 2,
	.m_state = 16,
	.term    = true,
	/* G0/G0 - G1/G0 */
	.polys   = { 0x0019, 0x001B, },
	.polyDiv = 0x0019,
};

params_t tch_ahs_speech_5_15 = {
	.name    = "TCH_AHS_speech_5_15",
	.rate    = 3,
	.m_state = 16,
	.term    = true,
	/* G1/G3 - G2/G3 - G3/G3 */
	.polys   = { 0x0019, 0x001B, 0x001F },
	.polyDiv = 0x001F,
};

params_t tch_ahs_speech_4_75 = {
	.name    = "TCH_AHS_speech_4_75",
	.rate    = 3,
	.m_state = 64,
	.term    = true,
	/* G4/G4 - G5/G4 - G6/G4 */
	.polys   = { 0x006D, 0x0053, 0x005F},
	.polyDiv = 0x006D,
};

#define LEN_PARAMS_FR	9
params_t *params_fr[LEN_PARAMS_FR] = {
	&tch_fr,
	&tch_afs_speech_12_2,
	&tch_afs_speech_10_2,
	&tch_afs_speech_7_95,
	&tch_afs_speech_7_4,
	&tch_afs_speech_6_7,
	&tch_afs_speech_5_9,
	&tch_afs_speech_5_15,
	&tch_afs_speech_4_75,
};

#define LEN_PARAMS_HR	7
params_t *params_hr[LEN_PARAMS_HR] = {
	&tch_hs,
	&tch_ahs_speech_7_95,
	&tch_ahs_speech_7_4,
	&tch_ahs_speech_6_7,
	&tch_ahs_speech_5_9,
	&tch_ahs_speech_5_15,
	&tch_ahs_speech_4_75,
};

#define BUFFER_STR_LEN	16384
int main(int argc, char *argv[]){
	uint8_t ***out   = NULL;        // [Rate][m_state][1bit]
	uint8_t  **state = NULL;        // [m_state][1bit]
	uint8_t   *term_state = NULL;   // string of table state[m_state][1bit]
	uint8_t  **term_out   = NULL;   // string[Rate] of tables out[m_state][1bit]
	char       str[BUFFER_STR_LEN] = {0};
	char       str_term[BUFFER_STR_LEN] = {0};
	unsigned   i = 0;
	/* Which params FR ? */
	params_t **params = params_fr;
	size_t     m_params = LEN_PARAMS_FR;
	/* or HR 
	params_t **params = params_hr;
	size_t     m_params = LEN_PARAMS_HR;*/

	for(; i < m_params; i ++){
		unsigned y = 0;
		size_t   l_str = 0;
		size_t   l_str_term = 0;
		params_t *p = (params[i]);
		memset(str, 0, BUFFER_STR_LEN);
		memset(str_term, 0, BUFFER_STR_LEN);

		init_stateTable(&out, &state, &term_out, &term_state, p->m_state, p->rate);

		if(p->term){
			gen_stateTable_term_state((uint8_t*)term_state, 0, p->m_state);
			gen_stateTable_term_out((uint8_t**)term_out, p->m_state, p->rate, p->polys);
		}
		for(; y < p->rate; y++){
			p->polys[y] = (p->polys[y] ^ p->polyDiv) + 1;
		}
		gen_stateTable_out((uint8_t***)out, p->m_state, p->rate, (const uint32_t*)p->polys);
		gen_stateTable_state((uint8_t**)state, p->polyDiv, p->m_state);


		l_str += toString_stateTable_out(str + l_str, BUFFER_STR_LEN - l_str, p->name, (const uint8_t***) out, p->m_state, p->rate);
		l_str += toString_stateTable_state(str + l_str, BUFFER_STR_LEN - l_str, p->name, (const uint8_t **)state, p->m_state);
		printf("/********************************\n %s \n ********************************/\n\n", p->name);
		printf("%s\n\n", str);

		if(p->term){
			l_str_term += toString_stateTable_term_state(str_term + l_str_term, BUFFER_STR_LEN - l_str_term, p->name, (const uint8_t *)term_state, p->m_state);
			l_str_term += toString_stateTable_term_out(str_term + l_str_term, BUFFER_STR_LEN - l_str_term, p->name, (const uint8_t **)term_out, p->m_state, p->rate);
			printf("/***\n %s Termination\n ***/\n\n", p->name);
			printf("%s\n\n", str_term);
		}
		
		free_stateTable(&out, &state, &term_out, &term_state, p->m_state, p->rate);
	}
	return (int) 0;
}

