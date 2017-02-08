#pragma once

#include<wchar.h>
#include"karat.h"

#include"bits.h"
#include"system.h"
#include"cpu.h"
#include"log.h"
#include"types.h"

#define IMMEDIATE 0
#define REG_REG 1
#define DIRECT 2
#define INDIRECT_REG 3
#define INDIRECT_INDEXED 4

#define ARG_MODE(f, s) (((0x0f & (f)) << 4) | (0x0f & (s)))
#define GET_ARG_MODES(f, s, m) \
	(f) = (0xf0 & (m)) >> 4; (s) = (0x0f & (m));
#define GET_OP(o) (((o) & 0xff00) >> 8)

/* opcode indices for table */
enum {
	LOAD = 0,
	PUT,
	SIN,
	SOUT,
	SWAP, 
	MAXIMUM_OP,
};

struct op_def {
	u8 code;
	const wchar_t *mnemonic;
};

#define LOAD_B 0x30
#define PUT_B 0x31
#define SOUT_B 0x11
#define SIN_B 0x12
#define SWAP_B 0x13

static const struct op_def op_defs[MAXIMUM_OP] = {
	[LOAD] = {LOAD_B, L"load"},
	[PUT] = {PUT_B, L"put"},
	[SIN] = {SIN_B, L"sin"},
	[SOUT] = {SOUT_B, L"sout"},
	[SWAP] = {SWAP_B, L"swap"},
};

static inline void exec_op(struct cpu *cpu, struct system *sys)
{
	addr_t pc = cpu->pc;
	u8 op = sys->mem[pc];
	switch(op){
	case LOAD_B:
		break;
	default:
		break;
	};
}
