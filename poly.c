
#include "poly.h"

#include <stdio.h>
#include <stdlib.h>

/* Hamming Weight calculator */
static inline uint8_t _hamming_weight(const uint32_t val, const uint32_t poly){
	int value = val & poly;
	unsigned count = 0;
	for(; value != 0; count++, value &= value-1);
        return (uint8_t) count & 0x01;
}

/* Generation states tables for : next out, next state, next term out, next term state */
int gen_stateTable_out(uint8_t ***stateTable, const uint32_t m_state, const uint8_t rate, const uint32_t *polys){
	if(stateTable == NULL || polys == NULL){
		return (int) -1;
	}

	for(unsigned i = 0; i < m_state; i++){
		for(unsigned g = 0; g < rate; g++){
			for(unsigned b = 0; b <= 1; b++){
				stateTable[g][i][b] = _hamming_weight((i << 1) | b, polys[g]);
			}
		}
	}

	return (int) 0;
}

int gen_stateTable_term_out(uint8_t **stateTable, const uint32_t m_state, const uint8_t rate, const uint32_t *polys){
	if(stateTable == NULL || polys == NULL){
		return (int) -1;
	}

	for(unsigned i = 0; i < m_state; i++){
		for(unsigned g = 0; g < rate; g++){
			stateTable[g][i] = _hamming_weight((i << 1), polys[g]);
		}
	}

	return (int) 0;
}

int gen_stateTable_state(uint8_t **stateTable, uint32_t poly, const uint32_t m_state){
	const unsigned max = m_state * 2;
	unsigned mask = (max - 1) >> 1;
	if(stateTable == NULL){
		return (int) -1;
	}

	for(unsigned s = 0, i = 0; ((i = s << 0x01)) < max; s++){
		stateTable[s][0] = (i | _hamming_weight(i       , poly)) & mask;
		stateTable[s][1] = (i | _hamming_weight(i | 0x01, poly)) & mask;
	}

	return (int) 0;
}

int gen_stateTable_term_state(uint8_t *stateTable, uint32_t poly, const uint32_t m_state){
	const unsigned max = m_state * 2;
	unsigned mask = (max - 1) >> 1;
	if(stateTable == NULL){
		return (int) -1;
	}

	for(unsigned s = 0, i = 0; ((i = s << 0x01)) < max; s++){
		stateTable[s] = (i | _hamming_weight(i, poly)) & mask;
	}

	return (int) 0;
}

/* to String fo states tables for : next out, next state, next term out, next term state */
int toString_stateTable_term_out(char *out, size_t m_out, const char *s_name, const uint8_t **stateTable, const uint32_t m_state, const uint8_t rate){
	int len = -1;
	if(out == NULL || s_name == NULL || stateTable == NULL){
		return (int) len;
	}

	len += snprintf(out + len, m_out - len, "/*Integer version\nuint8_t %s_term_out[%d] = {\n", s_name, m_state);
	for(unsigned i = 0; i < m_state; i++){
		int nb = 0;
		if(i == 0){
			len += snprintf(out + len, m_out - len, "\t");
		}else if((i % 8) == 0){
			len += snprintf(out + len, m_out - len, "\n\t");
		}
		for(unsigned g = 0; g < rate; g++){
			if(g > 0) nb <<= 1;
			nb |= stateTable[g][i];
		}
		len += snprintf(out + len, m_out - len, "%d, ", nb);
	}
	len += snprintf(out + len, m_out - len, "\n};\n */\n\n");
	
	len += snprintf(out + len, m_out - len, "/* Binary version */\nuint8_t %s_term_out[%d][%d] = {", s_name, rate, m_state);
	for(unsigned g = 0; g < rate; g++){
		len += snprintf(out + len, m_out - len, "\n  {");
		for(unsigned i = 0; i < m_state; i++){
			if(i == 0){
				len += snprintf(out + len, m_out - len, "\t");
			}else if((i % 8) == 0){
				len += snprintf(out + len, m_out - len, "\n\t");
			}
			len += snprintf(out + len, m_out - len, "%d, ", stateTable[g][i]);
		}
		len += snprintf(out + len, m_out - len, "\n  },");
	}
	len += snprintf(out + len, m_out - len, "\n};\n\n");

	return (int) len;
}

int toString_stateTable_term_state(char *out, size_t m_out, const char *s_name, const uint8_t *stateTable, const uint32_t m_state){
	int len = -1;
	if(out == NULL || s_name == NULL || stateTable == NULL){
		return (int) len;
	}
	
	len += snprintf(out + len, m_out - len, "uint8_t %s_term_state[%d] = {\n", s_name, m_state);
	for(unsigned i = 0; i < m_state; i++){
		if(i == 0){
			len += snprintf(out + len, m_out - len, "\t");
		}else if((i % 8) == 0){
			len += snprintf(out + len, m_out - len, "\n\t");
		}
		len += snprintf(out + len, m_out - len, "%2d, ", stateTable[i]);
	}
	len += snprintf(out + len, m_out - len, "\n};\n\n\n");

	return (int) len;
}

int toString_stateTable_state(char *out, size_t m_out, const char *s_name, const uint8_t **stateTable, const uint32_t m_state){
	int len = -1;
	if(out == NULL || s_name == NULL || stateTable == NULL){
		return (int) len;
	}
	
	len += snprintf(out + len, m_out - len, "uint8_t %s_state[%d][2] = {\n", s_name, m_state);
	for(unsigned i = 0; i < m_state; i++){
		if(i == 0){
			len += snprintf(out + len, m_out - len, "\t");
		}else if((i % 4) == 0){
			len += snprintf(out + len, m_out - len, "\n\t");
		}
		len += snprintf(out + len, m_out - len, "{ %2d, %2d }, ", stateTable[i][0], stateTable[i][1]);
	}
	len += snprintf(out + len, m_out - len, "\n};\n\n\n");

	return (int) len;
}

int toString_stateTable_out(char *out, size_t m_out, const char *s_name, const uint8_t ***stateTable, const uint32_t m_state, const uint8_t rate){
	int len = -1;
	if(out == NULL || s_name == NULL || stateTable == NULL){
		return (int) len;
	}

	//Coment - Integer version
	len = 0;
	len += snprintf(out + len, m_out - len, "/* Integer version\nuint32_t %s_out[%d][2] = {\n", s_name,  m_state);
	for(unsigned i = 0; i < m_state; i++){
		int nb[2] = { 0 };
		for(unsigned g = 0; g < rate; g++){
			if(g > 0) nb[0] <<= 1;
			if(g > 0) nb[1] <<= 1;
			nb[0] |= stateTable[g][i][0];
			nb[1] |= stateTable[g][i][1];
		}
		if(i == 0){
			len += snprintf(out + len, m_out - len, "\t");
		}else if((i % 4) == 0){
			len += snprintf(out + len, m_out - len, "\n\t");
		}
		len += snprintf(out + len, m_out - len, "{ %d, %d }, ", nb[0], nb[1]);
	}
	len += snprintf(out + len, m_out - len, "\n};\n */\n\n");

	//Binary version
	len += snprintf(out + len, m_out - len, "/* Binary version */\nuint8_t %s_out[%d][%d][2] = {\n", s_name, rate, m_state);
	for(unsigned g = 0; g < rate; g++){
		len += snprintf(out + len, m_out - len, "  { "); 
		for(unsigned i = 0; i < m_state; i++){
			if(i == 0){
				len += snprintf(out + len, m_out - len, "\t");
			}else if((i % 4) == 0){
				len += snprintf(out + len, m_out - len, "\n\t");
			}

			len += snprintf(out + len, m_out - len, "{ %d, %d }, ", stateTable[g][i][0], stateTable[g][i][1]);
		}
		len += snprintf(out + len, m_out - len, "\n  },\n"); 
	}
	len += snprintf(out + len, m_out - len, "};\n\n");
	
	return (int) len;
}

/* init & free of all states tables : out, state, term_out, term_state */

int free_stateTable(uint8_t ****out, uint8_t ***state, uint8_t ***term_out, uint8_t **term_state, uint32_t m_state, uint8_t rate){
	if(*out){
		for(unsigned g = 0; g < rate; g++){
			for(unsigned s = 0; s < m_state ; s++){
				free((*out)[g][s]);
			}
			free((*out)[g]);
		}
		free(*out);
		*out = NULL;
	}

	if(*state){
		for(unsigned s = 0; s < m_state; s++){
			free((*state)[s]);
		}
		free(*state);
		*state = NULL;
	}

	if(*term_out){
		for(unsigned g = 0; g < rate; g++){
			free((*term_out)[g]);
		}
		free(*term_out);
		*term_out = NULL;
	}

	if(*term_state){
		free(*term_state);
		*term_state = NULL;
	}

	return (int) 0;
}

int init_stateTable(uint8_t ****out, uint8_t ***state, uint8_t ***term_out, uint8_t **term_state, uint32_t m_state, uint8_t rate){
	if(out == NULL || state == NULL || term_out == NULL || term_state == NULL){
		return (int) -1;
	}
	(*out) = (uint8_t***)calloc(rate, sizeof(uint8_t**));
	if(*out == NULL){
		fprintf(stderr, "We've probably an out of memory !");
		return (int) free_stateTable(out,state,term_out,term_state,m_state,rate);
	}
	for(unsigned g = 0; g < rate; g++){
		(*out)[g] = (uint8_t**)calloc(m_state, sizeof(uint8_t*));
		if((*out)[g] == NULL){
			fprintf(stderr, "We've probably an out of memory !");
			return (int) free_stateTable(out,state,term_out,term_state,m_state,rate);
		}
		for(unsigned s = 0; s < m_state ; s++){
			(*out)[g][s] = (uint8_t*)calloc(2, sizeof(uint8_t));
			if((*out)[g][s] == NULL){
				fprintf(stderr, "We've probably an out of memory !");
				return (int) free_stateTable(out,state,term_out,term_state,m_state,rate);
			}
		}
	}

	(*state) = (uint8_t**)calloc(m_state, sizeof(uint8_t*));
	if(*state == NULL){
		fprintf(stderr, "We've probably an out of memory !");
		return (int) free_stateTable(out,state,term_out,term_state,m_state,rate);
	}
	for(unsigned s = 0; s < m_state; s++){
		(*state)[s] = (uint8_t*)calloc(2, sizeof(uint8_t));
		if((*state)[s] == NULL){
			fprintf(stderr, "We've probably an out of memory !");
			return (int) free_stateTable(out,state,term_out,term_state,m_state,rate);
		}
	}

	(*term_out) = (uint8_t**)calloc(rate, sizeof(uint8_t*));
	if(*term_out == NULL){
		fprintf(stderr, "We've probably an out of memory !");
		return (int) free_stateTable(out,state,term_out,term_state,m_state,rate);
	}
	for(unsigned g = 0; g < rate; g++){
		(*term_out)[g] = (uint8_t*)calloc(m_state, sizeof(uint8_t*));
		if((*term_out)[g] == NULL){
			fprintf(stderr, "We've probably an out of memory !");
			return (int) free_stateTable(out,state,term_out,term_state,m_state,rate);
		}
	}

	(*term_state) = (uint8_t*)calloc(m_state, sizeof(uint8_t*));
	if(*term_state == NULL){
		fprintf(stderr, "We've probably an out of memory !");
		return (int) free_stateTable(out,state,term_out,term_state,m_state,rate);
	}

	return (int) 0;
}

