#pragma once

#include<wchar.h>

#include"bits.h"
#include"log.h"
#include"types.h"

enum op_val {
	LOAD = 0,
	PUT,
	SIN,
	SOUT,
	SWAP,
	ADR,
	MAX_OP
};

static const wchar_t *const op_strings[MAX_OP] = {
	L"load",
	L"put",
	L"sin",
	L"sout",
	L"swap",
	L"adr",
};

enum op_mode {
	RK 	= 	(1 << 1),
	RA 	= 	(1 << 2),
	K 	= 	(1 << 3),
	A 	= 	(1 << 4),
	AK 	= 	(1 << 5),
	KK 	= 	(1 << 6),
	RR 	= 	(1 << 7),
	AA 	= 	(1 << 8),
	AR 	= 	(1 << 9),
	KA 	= 	(1 << 10),
	R 	= 	(1 << 11),
	N 	= 	(1 << 12),
	KR 	= 	(1 << 13),
};

static const u16 valid_opmodes[MAX_OP] = {
	RK | RA | A | RR,				/* LOAD */
	RA | KA | A,					/* PUT */
	AK | RK,						/* SIN */
	AK | RK | KK,					/* SOUT */
	AA | RR | RA | AR,				/* SWAP */
	RR | RA | RK | AK | AA | AR,	/* ADD */
};

static inline int mode_to_index(enum op_mode m)
{
	size_t bit = 0;
	while(m){
		++bit;
		m >>= 1;
	}
	return bit;
}

#define make_op(op, mode) lbits((((op) << 16) | (mode)), 32)
#define get_opmode(n) lbits((n), 16)
#define get_opval(n) lbits(((n) >> 16), 16)

static inline enum op_mode combine_modes(enum op_mode first, enum op_mode sec)
{
	switch(first){
	case A:
		switch(sec){
		case A:
			return AA;
		case R:
			return AR;
		case K:
			return AK;
		default:
			err("enumeration not supported");
			return N;
		};
	case R:
		switch(sec){
		case A:
			return RA;
		case R:
			return RR;
		case K:
			return RK;
		default:
			err("enumeration not supported");
			return N;
		};
	case K:
		switch(sec){
		case A:
			return KA;
		case R:
			return KR;
		case K:
			return KK;
		default:
			err("enumeration not supported");
			return N;
		};
	default:
		err("enumeration not supported");
	};
	err("enumeration not supported");
	return N;
}
