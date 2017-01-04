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
	RK = (1 << 1),
	RA = (1 << 2),
	K = (1 << 3),
	A = (1 << 4),
	AK = (1 << 5),
	KK = (1 << 6),
	RR = (1 << 7),
	AA = (1 << 8),
	AR = (1 << 9),
	KA = (1 << 10),
	R = (1 << 11),
	N = (1 << 12),
	KR = (1 << 13),
};

static const u16 valid_opmodes[MAX_OP] = {
	RK | RA | A,					/* LOAD */
	RA | KA | A,					/* PUT */
	AK | RK,						/* SIN */
	AK | RK | KK,					/* SOUT */
	AA | RR | RA | AR,				/* SWAP */
	RR | RA | RK | AK | AA | AR,	/* ADD */
};

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

/*
//#define FP(ret, name, ...) \
//	ret(*name)(##__VA_ARGS__)
//
//#define AFP(name, ...) \
//	void(*name)(struct cpu_state *, __VA_ARGS__)
//
//typedef AFP(rk_func	, u8, 		u32);
//typedef AFP(ra_func	, u8, 		addr_t);
//typedef AFP(k_func	, u32);
//typedef AFP(a_func	, addr_t);
//typedef AFP(ak_func	, addr_t, 	u32);
//typedef AFP(kk_func	, u32, 		u32);
//typedef AFP(rr_func	, u8, 		u8);
//typedef AFP(aa_func	, addr_t, 	addr_t);
//typedef AFP(ar_func	, addr_t, 	u8);
//typedef AFP(ka_func	, u32,		addr_t);
//typedef AFP(r_func	, u8);
//
//
//struct op_funcs {
//	rk_func rk;
//	ra_func ra;
//	k_func 	k;
//	a_func 	a;
//	ak_func ak;
//	kk_func kk;
//	rr_func rr;
//	aa_func aa;
//	ar_func ar;
//	ka_func ka;
//	r_func 	r;
//};
//
//static struct op_funcs op_funcs[MAX_OP];
//
//static inline void add_op_func(enum op_val which, struct op_funcs funcs)
//{
//	op_funcs[which] = funcs;
//}

#undef AFP
#undef FP
*/
