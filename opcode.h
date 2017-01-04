#pragma once

enum op_val {
	LOAD = 0,
	PUT,
	SIN,
	SOUT,
	SWAP,
	MAX_OP
};

static const wchar_t *const op_strings[MAX_OP] = {
	L"load",
	L"put",
	L"sin",
	L"sout",
	L"swap",
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
};

static const u32 opmodes[MAX_OP] = {
	RK | RA | A,			/* LOAD */
	RA | KA | A,			/* PUT */
	AK | RK,			/* SIN */
	AK | RK | KK,		/* SOUT */
	AA | RR | RA | AR,	/* SWAP */
};

//#define FP(ret, name, ...) ret(*name)(##__VA_ARGS__)
//#define AFP(name, ...) void(*name)(struct cpu_state *, ##__VA_ARGS__)
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

//struct op_funcs {
//	rk_func rkf;
//	ra_func raf;
//	k_func kf;
//	a_func af;
//	ak_func akf;
//	kk_func kkf;
//	rr_func rrf;
//	aa_func aaf;
//	ar_func arf;
//	ka_func kaf;
//	r_func rf;
//};
//
//extern struct op_funcs all_opfuncs[MAX_OP] = {
//	
//};
