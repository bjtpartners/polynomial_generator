
#ifndef _POLY_H_
#define _POLY_H_

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * The following functions allow to generate states tables used to calate a polynomial code.
 * These functions were created for calculate the polynomial code or to generate states tables used in channels coding to GSM Network (3GPP TS 05.03
 * https://en.wikipedia.org/wiki/Polynomial_code
 * https://en.wikipedia.org/wiki/Hamming_weight
 */

/* Generation states tables for : next out, next state, next term out, next term state */
int gen_stateTable_out(uint8_t ***stateTable, const uint32_t m_state, const uint8_t rate, const uint32_t *polys);
int gen_stateTable_term_out(uint8_t **stateTable, const uint32_t m_state, const uint8_t rate, const uint32_t *polys);
int gen_stateTable_state(uint8_t **stateTable, uint32_t poly, const uint32_t m_state);
int gen_stateTable_term_state(uint8_t *stateTable, uint32_t poly, const uint32_t m_state);

/*  */
int toString_stateTable_term_out(char *out, size_t m_out, const char *s_name, const uint8_t **stateTable, const uint32_t m_state, const uint8_t rate);
int toString_stateTable_term_state(char *out, size_t m_out, const char *s_name, const uint8_t *stateTable, const uint32_t m_state);
int toString_stateTable_state(char *out, size_t m_out, const char *s_name, const uint8_t **stateTable, const uint32_t m_state);
int toString_stateTable_out(char *out, size_t m_out, const char *s_name, const uint8_t ***stateTable, const uint32_t m_state, const uint8_t rate);

/*  */
int init_stateTable(uint8_t ****out, uint8_t ***state, uint8_t ***term_out, uint8_t **term_state, uint32_t m_state, uint8_t rate);
int free_stateTable(uint8_t ****out, uint8_t ***state, uint8_t ***term_out, uint8_t **term_state, uint32_t m_state, uint8_t rate);

/* */
int calc_polyCode(uint8_t *c, uint8_t *u, uint32_t *polys, uint8_t rate, bool termination);

#endif /* _POLY_H_*/
