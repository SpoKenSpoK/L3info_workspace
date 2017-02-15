
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"

/**********************************************************
** definition de la memoire simulee
***********************************************************/

WORD mem[128];     /* memoire                       */


/**********************************************************
** Placer une instruction en memoire
***********************************************************/

void make_inst(int adr, unsigned code, unsigned i, unsigned j, short arg) {
	union { WORD word; INST fields; } inst;
	inst.fields.OP  = code;
	inst.fields.i   = i;
	inst.fields.j   = j;
	inst.fields.ARG = arg;
	mem[adr] = inst.word;
}


INST decode_instruction(WORD value) {
	union { WORD integer; INST instruction; } inst;
	inst.integer = value;
	return inst.instruction;
}


/**********************************************************
** instruction d'addition
***********************************************************/

PSW cpu_ADD(PSW m) {
	m.AC = m.DR[m.RI.i] += (m.DR[m.RI.j] + m.RI.ARG);
	m.PC += 1;
	return m;
}


/**********************************************************
** instruction de soustraction
***********************************************************/

PSW cpu_SUB(PSW m) {
	m.AC = m.DR[m.RI.i] -= (m.DR[m.RI.j] + m.RI.ARG);
	m.PC += 1;
	return m;
}


/**********************************************************
** instruction de comparaison
***********************************************************/

PSW cpu_CMP(PSW m) {
	m.AC = (m.DR[m.RI.i] - (m.DR[m.RI.j] + m.RI.ARG));
	m.PC += 1;
	return m;
}

/* instruction de IFGT */
PSW cpu_IFGT(PSW m) {
	if( m.AC > 0 ) m.PC = m.RI.ARG;
	return m;
}

/* instruction NOP */
PSW cpu_NOP(PSW m) {
	m.PC += 1;
	printf(" -- NOP -- \n");
	return m;
}

/* instruction JUMP */
PSW cpu_JUMP(PSW m) {
	m.PC = m.RI.ARG;
	return m;
}

/* instruction HALT */
PSW cpu_HALT(PSW m) {
	printf("-- HALT --\n");
	exit(0);
}


/**********************************************************
** Simulation de la CPU (mode utilisateur)
***********************************************************/

PSW cpu(PSW m) {

	for(int i=0; i < 3; ++i){

		/*** lecture et decodage de l'instruction ***/
		if (m.PC < 0 || m.PC >= m.SS) {
			m.IN = INT_SEGV;
			printf("N° interruption - arret : %d\n", m.IN);
			exit(-1);
			//return (m);
		}
		m.RI = decode_instruction(mem[m.PC + m.SB]);

		printf("%d\n", m.RI.OP);

		/*** execution de l'instruction ***/
		switch (m.RI.OP) {
			case INST_ADD: m = cpu_ADD(m); break;
			case INST_SUB: m = cpu_SUB(m); break;
			case INST_CMP: m = cpu_CMP(m); break;
			case INST_IFGT: m = cpu_IFGT(m); break;
			case INST_JUMP: m = cpu_JUMP(m); break;
			case INST_NOP: m = cpu_NOP(m); break;
			case INST_HALT: m = cpu_HALT(m); break;

			default:
				/*** interruption instruction inconnue ***/
				m.IN = INT_INST;
				printf("N° interruption - arret : %d\n", m.IN);
				exit(-1);
				//return (m);
		}

		/*** interruption apres chaque instruction ***/
		//m.IN = INT_TRACE;
	}

	m.IN = INT_CLOCK;
	return m;

}
