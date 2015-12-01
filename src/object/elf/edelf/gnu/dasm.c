// disassembly functions

// headers
#include "edelf.h"
#include "trans.h"

// general instructions format - sparc version 8
//
// format	bits	description
// -----------------------------------------------
// call		31,30	op
// 		29-0	displacements relative to pc.
// sethi and
// branches	31,30	op
//		29	annullment bit
//		28-25	condition codes
//		24-22	op2
//		21-0	displacement for branches.
//
//		31-30	op
//		29-25	source/destination register
//		24-22	op2
//		21-0	constant for destination register
// remaining
// instructions	31,30	op
//		29-25	source/destination register
//		24-19	op3
//		18-14	source/destination register
//		13	selects ALU
//		12-5	address space identifier
//		4-0	source/destination register
//
// 		31,30	op
//		29-25	source/destination register
//		24-19	op3
//		18-14	source/destination register
//		13	selects ALU
//		12-0	sign-extended value
//
// 		31,30	op
//		29-25	source/destination register
//		24-19	op3
//		18-14	source/destination register
//		13-5	co-processor or floating-point instuction
//		4-0	source/destination register
//
// name	value	description
// --------------------------------------------
// op	1	call
// 	0	Bicc, FBfcc, CBccc, SETHI
//	3	memory instructions
//	2	arithmetic, logical, shift, etc.
// op2	0	UNIMP
//	1	unimplemented
//	2	Bicc
//	3	unimplemented
//	4	SETHI
//	5	unimplemented
//	6	FBfcc
//	7	CBccc
//
// synthetic instructions - these instructions are provided
// in the assembly language for convenience, the actual opcode
// is different. for example, "cmp" is really "subcc".
// 
// synthetic	actual		comment
// opcode	opcode
// -------------------------------------------------------
// cmp		subcc		compare
// jmp		jmpl
// call		jmpl
// tst		orcc		test
// ret		jmpl		return from subroutine
// retl		jmpl		return from leaf subroutine
// restore	restore		trivial restore
// save		save		trivial save
// set		sethi
// not		xnor		1's complement
// neg		sub		2's complement
// inc		add		increment by a const
// inccc	addcc		increment by a const and set icc
// dec		sub		decrement by a const
// deccc	subcc		decrement by a const and set icc
// btst		andcc		bit test
// bset		or		bit set
// bclr		andn		bit clear
// btog		xor		bit toggle
// clr		or		clear register
// clrb		stb		clear byte
// clrh		sth		clear halfword
// clr		st		clear word
// mov		or
// mov		rd
// mov		wr
//
////////////////////////////////////////////////////////////////////////////////
//

// local flags
static unsigned long maxlinestoprint = 0;

// local sparc-related constants
static const unsigned long Op_0 = 0;
static const unsigned long Op_1 = 1;
static const unsigned long Op_2 = 2;
static const unsigned long Op_3 = 3;

static const unsigned long Op2_unimp = 0;
static const unsigned long Op2_unimplemented_1 = 1;
static const unsigned long Op2_bicc = 2;
static const unsigned long Op2_unimplemented_3 = 3;
static const unsigned long Op2_sethi = 4;
static const unsigned long Op2_unimplemented_5 = 5;
static const unsigned long Op2_fbfcc = 6;
static const unsigned long Op2_cbccc = 7;

static const unsigned long Op3_ldsb = 9;
static const unsigned long Op3_ldsh = 10;
static const unsigned long Op3_ldub = 1;
static const unsigned long Op3_lduh = 2;
static const unsigned long Op3_ld = 0;
static const unsigned long Op3_ldd = 3;

static const unsigned long Op3_ldsba = 25;
static const unsigned long Op3_ldsha = 26;
static const unsigned long Op3_lduba = 17;
static const unsigned long Op3_lduha = 18;
static const unsigned long Op3_lda = 16;
static const unsigned long Op3_ldda = 19;

static const unsigned long Op3_ldf = 32;
static const unsigned long Op3_lddf = 35;
static const unsigned long Op3_ldfsr = 33;

static const unsigned long Op3_ldc = 48;
static const unsigned long Op3_lddc = 51;
static const unsigned long Op3_ldcsr = 49;

static const unsigned long Op3_stb = 5;
static const unsigned long Op3_sth = 6;
static const unsigned long Op3_st = 4;
static const unsigned long Op3_std = 7;

static const unsigned long Op3_stba = 21;
static const unsigned long Op3_stha = 22;
static const unsigned long Op3_sta = 20;
static const unsigned long Op3_stda = 23;

static const unsigned long Op3_stf = 36;
static const unsigned long Op3_stdf = 39;
static const unsigned long Op3_stfsr = 37;
static const unsigned long Op3_stdfq = 38;

static const unsigned long Op3_stc = 52;
static const unsigned long Op3_stdc = 55;
static const unsigned long Op3_stcsr = 53;
static const unsigned long Op3_stdcq = 54;

static const unsigned long Op3_ldstub = 13;
static const unsigned long Op3_ldstuba = 29;

static const unsigned long Op3_swap = 15;
static const unsigned long Op3_swapa = 31;

static const unsigned long Op3_and = 1;
static const unsigned long Op3_andcc = 17;
static const unsigned long Op3_andn = 5;
static const unsigned long Op3_andncc = 21;
static const unsigned long Op3_or = 2;
static const unsigned long Op3_orcc = 18;
static const unsigned long Op3_orn = 6;
static const unsigned long Op3_orncc = 22;
static const unsigned long Op3_xor = 3;
static const unsigned long Op3_xorcc = 19;
static const unsigned long Op3_xnor = 7;
static const unsigned long Op3_xnorcc = 23;

static const unsigned long Op3_sll = 37;
static const unsigned long Op3_srl = 38;
static const unsigned long Op3_sra = 39;

static const unsigned long Op3_add = 0;
static const unsigned long Op3_addcc = 16;
static const unsigned long Op3_addx = 8;
static const unsigned long Op3_addxcc = 24;
static const unsigned long Op3_taddcc = 32;
static const unsigned long Op3_taddcctv = 34;

static const unsigned long Op3_sub = 4;
static const unsigned long Op3_subcc = 20;
static const unsigned long Op3_subx = 12;
static const unsigned long Op3_subxcc = 28;
static const unsigned long Op3_tsubcc = 33;
static const unsigned long Op3_tsubcctv = 35;

static const unsigned long Op3_mulscc = 36;
static const unsigned long Op3_umul = 10;
static const unsigned long Op3_smul = 11;
static const unsigned long Op3_umulcc = 26;
static const unsigned long Op3_smulcc = 27;

static const unsigned long Op3_udiv = 14;
static const unsigned long Op3_sdiv = 15;
static const unsigned long Op3_udivcc = 30;
static const unsigned long Op3_sdivcc = 31;

static const unsigned long Op3_save = 60;
static const unsigned long Op3_restore = 61;

static const unsigned long Op3_jmpl = 56;
static const unsigned long Op3_rett = 58;

static const unsigned long Op3_itrap = 57;

static const unsigned long Op3_rdy = 40;
static const unsigned long Op3_rdpsr = 41;
static const unsigned long Op3_rdwim = 42;
static const unsigned long Op3_rdtbr = 43;

static const unsigned long Op3_wry = 48;
static const unsigned long Op3_wrpsr = 49;
static const unsigned long Op3_wrwim = 50;
static const unsigned long Op3_wrtbr = 51;

static const unsigned long Op3_stbar = 40;

static const unsigned long Op3_flush = 59;

static const unsigned long Op3_fpop1 = 52;
static const unsigned long Op3_fpop2 = 53;

static const unsigned long Op3_cpop1 = 54;
static const unsigned long Op3_cpop2 = 55;

static const unsigned long Cond_ba = 8;
static const unsigned long Cond_bn = 0;
static const unsigned long Cond_bne = 9;
static const unsigned long Cond_be = 1;
static const unsigned long Cond_bg = 10;
static const unsigned long Cond_ble = 2;
static const unsigned long Cond_bge = 11;
static const unsigned long Cond_bl = 3;
static const unsigned long Cond_bgu = 12;
static const unsigned long Cond_bleu = 4;
static const unsigned long Cond_bcc = 13;
static const unsigned long Cond_bcs = 5;
static const unsigned long Cond_bpos = 14;
static const unsigned long Cond_bneg = 6;
static const unsigned long Cond_bvc = 15;
static const unsigned long Cond_bvs = 7;

static const unsigned long Cond_fba = 8;
static const unsigned long Cond_fbn = 0;
static const unsigned long Cond_fbu = 7;
static const unsigned long Cond_fbg = 6;
static const unsigned long Cond_fbug = 5;
static const unsigned long Cond_fbl = 4;
static const unsigned long Cond_fbul = 3;
static const unsigned long Cond_fblg = 2;
static const unsigned long Cond_fbne = 1;
static const unsigned long Cond_fbe = 9;
static const unsigned long Cond_fbue = 10;
static const unsigned long Cond_fbge = 11;
static const unsigned long Cond_fbuge = 12;
static const unsigned long Cond_fble = 13;
static const unsigned long Cond_fbule = 14;
static const unsigned long Cond_fbo = 15;

static const unsigned long Cond_cba = 8;
static const unsigned long Cond_cbn = 0;
static const unsigned long Cond_cb3 = 7;
static const unsigned long Cond_cb2 = 6;
static const unsigned long Cond_cb23 = 5;
static const unsigned long Cond_cb1 = 4;
static const unsigned long Cond_cb13 = 3;
static const unsigned long Cond_cb12 = 2;
static const unsigned long Cond_cb123 = 1;
static const unsigned long Cond_cb0 = 9;
static const unsigned long Cond_cb03 = 10;
static const unsigned long Cond_cb02 = 11;
static const unsigned long Cond_cb023 = 12;
static const unsigned long Cond_cb01 = 13;
static const unsigned long Cond_cb013 = 14;
static const unsigned long Cond_cb012 = 15;

static const unsigned long Cond_ta = 8;
static const unsigned long Cond_tn = 0;
static const unsigned long Cond_tne = 9;
static const unsigned long Cond_te = 1;
static const unsigned long Cond_tg = 10;
static const unsigned long Cond_tle = 2;
static const unsigned long Cond_tge = 11;
static const unsigned long Cond_tl = 3;
static const unsigned long Cond_tgu = 12;
static const unsigned long Cond_tleu = 4;
static const unsigned long Cond_tcc = 13;
static const unsigned long Cond_tcs = 5;
static const unsigned long Cond_tpos = 14;
static const unsigned long Cond_tneg = 6;
static const unsigned long Cond_tvc = 15;
static const unsigned long Cond_tvs = 7;

static const unsigned long Opf_fitos = 196;
static const unsigned long Opf_fitod = 200;
static const unsigned long Opf_fitoq = 204;

static const unsigned long Opf_fstoi = 209;
static const unsigned long Opf_fdtoi = 210;
static const unsigned long Opf_fqtoi = 211;

static const unsigned long Opf_fstod = 201;
static const unsigned long Opf_fstoq = 205;
static const unsigned long Opf_fdtos = 198;
static const unsigned long Opf_fdtoq = 206;
static const unsigned long Opf_fqtos = 199;
static const unsigned long Opf_fqtod = 203;

static const unsigned long Opf_fmovs = 1;
static const unsigned long Opf_fnegs = 5;
static const unsigned long Opf_fabss = 9;

static const unsigned long Opf_fsqrts = 41;
static const unsigned long Opf_fsqrtd = 42;
static const unsigned long Opf_fsqrtq = 43;

static const unsigned long Opf_fadds = 65;
static const unsigned long Opf_faddd = 66;
static const unsigned long Opf_faddq = 67;

static const unsigned long Opf_fsubs = 69;
static const unsigned long Opf_fsubd = 70;
static const unsigned long Opf_fsubq = 71;

static const unsigned long Opf_fmuls = 73;
static const unsigned long Opf_fmuld = 74;
static const unsigned long Opf_fmulq = 75;

static const unsigned long Opf_fsmuld = 105;
static const unsigned long Opf_fdmulq = 110;

static const unsigned long Opf_fdivs = 77;
static const unsigned long Opf_fdivd = 78;
static const unsigned long Opf_fdivq = 79;

static const unsigned long Opf_fcmps = 81;
static const unsigned long Opf_fcmpd = 82;
static const unsigned long Opf_fcmpq = 83;

static const unsigned long Opf_fcmpes = 85;
static const unsigned long Opf_fcmped = 86;
static const unsigned long Opf_fcmpeq = 87;

// sparc translation tables
IntegerToString tcond_trans[] = {
	Cond_ta, "ta",
	Cond_tn, "tn",
	Cond_tne, "tne",
	Cond_te, "te",
	Cond_tg, "tg",
	Cond_tle, "tle",
	Cond_tge, "tge",
	Cond_tl, "tl",
	Cond_tgu, "tgu",
	Cond_tleu, "tleu",
	Cond_tcc, "tcc",
	Cond_tcs, "tcs",
	Cond_tpos, "tpos",
	Cond_tneg, "tneg",
	Cond_tvc, "tvc",
	Cond_tvs, "tvs",
	-1, NULL,
};

IntegerToString icond_trans[] = {
	Cond_ba, "ba",
	Cond_bn, "bn",
	Cond_bne, "bne",
	Cond_be, "be",
	Cond_bg, "bg",
	Cond_ble, "ble",
	Cond_bge, "bge",
	Cond_bl, "bl",
	Cond_bgu, "bgu",
	Cond_bleu, "bleu",
	Cond_bcc, "bcc",
	Cond_bcs, "bcs",
	Cond_bpos, "bpos",
	Cond_bneg, "bneg",
	Cond_bvc, "bvc",
	Cond_bvs, "bvs",
	-1, NULL,
};

IntegerToString fcond_trans[] = {
	Cond_fba, "fba",
	Cond_fbn, "fbn",
	Cond_fbu, "fbu",
	Cond_fbg, "fbg",
	Cond_fbug, "fbug",
	Cond_fbl, "fbl",
	Cond_fbul, "fbul",
	Cond_fblg, "fblg",
	Cond_fbne, "fbne",
	Cond_fbe, "fbe",
	Cond_fbue, "fbue",
	Cond_fbge, "fbge",
	Cond_fbuge, "fbuge",
	Cond_fble, "fble",
	Cond_fbule, "fbule",
	Cond_fbo, "fbo",
	-1, NULL,
};

IntegerToString ccond_trans[] = {
	Cond_cba, "cba",
	Cond_cbn, "cbn",
	Cond_cb3, "cb3",
	Cond_cb2, "cb2",
	Cond_cb23, "cb23",
	Cond_cb1, "cb1",
	Cond_cb13, "cb13",
	Cond_cb12, "cb12",
	Cond_cb123, "cb123",
	Cond_cb0, "cb0",
	Cond_cb03, "cb03",
	Cond_cb02, "cb02",
	Cond_cb023, "cb023",
	Cond_cb01, "cb01",
	Cond_cb013, "cb013",
	Cond_cb012, "cb012",
	-1, NULL,
};

IntegerToString op_11_op3_trans[] = {
	Op3_ldsb, "ldsb",
	Op3_ldsh, "ldsh",
	Op3_ldub, "ldub",
	Op3_lduh, "lduh",
	Op3_ld, "ld",
	Op3_ldd, "ldd",

	Op3_ldsba, "ldsba",
	Op3_ldsha, "ldsha",
	Op3_lduba, "lduba",
	Op3_lduha, "lduha",
	Op3_lda, "lda",
	Op3_ldda, "ldda",

	Op3_ldf, "ldf",
	Op3_lddf, "lddf",
	Op3_ldfsr, "ldfsr",

	Op3_ldc, "ldc",
	Op3_lddc, "lddc",
	Op3_ldcsr, "ldcsr",

	Op3_stb, "stb",
	Op3_sth, "sth",
	Op3_st, "st",
	Op3_std, "std",

	Op3_stba, "stba",
	Op3_stha, "stha",
	Op3_sta, "sta",
	Op3_stda, "stda",

	Op3_stf, "stf",
	Op3_stdf, "stdf",
	Op3_stfsr, "stfsr",
	Op3_stdfq, "stdfq",

	Op3_stc, "stc",
	Op3_stdc, "stdc",
	Op3_stcsr, "stcsr",
	Op3_stdcq, "stdcq",

	Op3_ldstub, "ldstub",
	Op3_ldstuba, "ldstuba",

	Op3_swap, "swap",
	Op3_swapa, "swapa",

	-1, NULL,
};

IntegerToString op_10_op3_trans[] = {
	Op3_and, "and",
	Op3_andcc, "andcc",
	Op3_andn, "andn",
	Op3_andncc, "andncc",
	Op3_or, "or",
	Op3_orcc, "orcc",
	Op3_orn, "orn",
	Op3_orncc, "orncc",
	Op3_xor, "xor",
	Op3_xorcc, "xorcc",
	Op3_xnor, "xnor",
	Op3_xnorcc, "xnorcc",

	Op3_sll, "sll",
	Op3_srl, "srl",
	Op3_sra, "sra",

	Op3_add, "add",
	Op3_addcc, "addcc",
	Op3_addx, "addx",
	Op3_addxcc, "addxcc",
	Op3_taddcc, "taddcc",
	Op3_taddcctv, "taddcctv",

	Op3_sub, "sub",
	Op3_subcc, "subcc",
	Op3_subx, "subx",
	Op3_subxcc, "subxcc",
	Op3_tsubcc, "tsubcc",
	Op3_tsubcctv, "tsubcctv",

	Op3_mulscc, "mulscc",
	Op3_umul, "umul",
	Op3_smul, "smul",
	Op3_umulcc, "umulcc",
	Op3_smulcc, "smulcc",

	Op3_udiv, "udiv",
	Op3_sdiv, "sdiv",
	Op3_udivcc, "udivcc",
	Op3_sdivcc, "sdivcc",

	Op3_save, "save",
	Op3_restore, "restore",

	Op3_jmpl, "jmpl",
	Op3_rett, "rett",

	Op3_rdy, "rdy",
	Op3_rdpsr, "rdpsr",
	Op3_rdwim, "rdwin",
	Op3_rdtbr, "rdtbr",

	Op3_wry, "wry",
	Op3_wrpsr, "wrpsr",
	Op3_wrwim, "wrwin",
	Op3_wrtbr, "wrtbr",

	Op3_flush, "flush",

	Op3_cpop1, "cpop1",
	Op3_cpop2, "cpop2",

	-1, NULL,
};

IntegerToString fpop1_opf_trans[] = {
	Opf_fitos, "fitos",
	Opf_fitod, "fitod",
	Opf_fitoq, "fitoq",

	Opf_fstod, "fstod",
	Opf_fstoq, "fstoq",
	Opf_fdtos, "fdtos",
	Opf_fdtoq, "fdtoq",
	Opf_fqtos, "fqtos",
	Opf_fqtod, "fqtod",

	Opf_fmovs, "fmovs",
	Opf_fnegs, "fnegs",
	Opf_fabss, "fabss",

	Opf_fsqrts, "fsqrts",
	Opf_fsqrtd, "fsqrtd",
	Opf_fsqrtq, "fsqrtq",

	Opf_fadds, "fadds",
	Opf_faddd, "faddd",
	Opf_faddq, "faddq",

	Opf_fsubs, "fsubs",
	Opf_fsubd, "fsubd",
	Opf_fsubq, "fsubq",

	Opf_fmuls, "fmuls",
	Opf_fmuld, "fmuld",
	Opf_fmulq, "fmulq",

	Opf_fsmuld, "fsmuld",
	Opf_fdmulq, "fdmulq",

	Opf_fdivs, "fdivs",
	Opf_fdivd, "fdivd",
	Opf_fdivq, "fdivq",

	-1, NULL,
};

IntegerToString fpop2_opf_trans[] = {
	Opf_fcmps, "fcmps",
	Opf_fcmpd, "fcmpd",
	Opf_fcmpq, "fcmpq",

	Opf_fcmpes, "fcmpes",
	Opf_fcmped, "fcmped",
	Opf_fcmpeq, "fcmpeq",

	-1, NULL,
};

// local ppc-related constants
static const unsigned long PPC_OP_00 = 0;
static const unsigned long PPC_OP_01 = 1;
static const unsigned long PPC_OP_02 = 2;
static const unsigned long PPC_OP_03 = 3;
static const unsigned long PPC_OP_04 = 4;
static const unsigned long PPC_OP_05 = 5;
static const unsigned long PPC_OP_06 = 6;
static const unsigned long PPC_OP_07 = 7;
static const unsigned long PPC_OP_08 = 8;
static const unsigned long PPC_OP_09 = 9;
static const unsigned long PPC_OP_10 = 10;
static const unsigned long PPC_OP_11 = 11;
static const unsigned long PPC_OP_12 = 12;
static const unsigned long PPC_OP_13 = 13;
static const unsigned long PPC_OP_14 = 14;
static const unsigned long PPC_OP_15 = 15;
static const unsigned long PPC_OP_16 = 16;
static const unsigned long PPC_OP_17 = 17;
static const unsigned long PPC_OP_18 = 18;
static const unsigned long PPC_OP_19 = 19;
static const unsigned long PPC_OP_20 = 20;
static const unsigned long PPC_OP_21 = 21;
static const unsigned long PPC_OP_22 = 22;
static const unsigned long PPC_OP_23 = 23;
static const unsigned long PPC_OP_24 = 24;
static const unsigned long PPC_OP_25 = 25;
static const unsigned long PPC_OP_26 = 26;
static const unsigned long PPC_OP_27 = 27;
static const unsigned long PPC_OP_28 = 28;
static const unsigned long PPC_OP_29 = 29;
static const unsigned long PPC_OP_30 = 30;
static const unsigned long PPC_OP_31 = 31;
static const unsigned long PPC_OP_32 = 32;
static const unsigned long PPC_OP_33 = 33;
static const unsigned long PPC_OP_34 = 34;
static const unsigned long PPC_OP_35 = 35;
static const unsigned long PPC_OP_36 = 36;
static const unsigned long PPC_OP_37 = 37;
static const unsigned long PPC_OP_38 = 38;
static const unsigned long PPC_OP_39 = 39;
static const unsigned long PPC_OP_40 = 40;
static const unsigned long PPC_OP_41 = 41;
static const unsigned long PPC_OP_42 = 42;
static const unsigned long PPC_OP_43 = 43;
static const unsigned long PPC_OP_44 = 44;
static const unsigned long PPC_OP_45 = 45;
static const unsigned long PPC_OP_46 = 46;
static const unsigned long PPC_OP_47 = 47;
static const unsigned long PPC_OP_48 = 48;
static const unsigned long PPC_OP_49 = 49;
static const unsigned long PPC_OP_50 = 50;
static const unsigned long PPC_OP_51 = 51;
static const unsigned long PPC_OP_52 = 52;
static const unsigned long PPC_OP_53 = 53;
static const unsigned long PPC_OP_54 = 54;
static const unsigned long PPC_OP_55 = 55;
static const unsigned long PPC_OP_56 = 56;
static const unsigned long PPC_OP_57 = 57;
static const unsigned long PPC_OP_58 = 58;
static const unsigned long PPC_OP_59 = 59;
static const unsigned long PPC_OP_60 = 60;
static const unsigned long PPC_OP_61 = 61;
static const unsigned long PPC_OP_62 = 62;
static const unsigned long PPC_OP_63 = 63;

static const unsigned long PPC_SUBOP_00 = 0;
static const unsigned long PPC_SUBOP_04 = 4;
static const unsigned long PPC_SUBOP_08 = 8;
static const unsigned long PPC_SUBOP_10 = 10;
static const unsigned long PPC_SUBOP_12 = 12;
static const unsigned long PPC_SUBOP_14 = 14;
static const unsigned long PPC_SUBOP_15 = 15;
static const unsigned long PPC_SUBOP_16 = 16;
static const unsigned long PPC_SUBOP_18 = 18;
static const unsigned long PPC_SUBOP_19 = 19;
static const unsigned long PPC_SUBOP_20 = 20;
static const unsigned long PPC_SUBOP_21 = 21;
static const unsigned long PPC_SUBOP_22 = 22;
static const unsigned long PPC_SUBOP_23 = 23;
static const unsigned long PPC_SUBOP_24 = 24;
static const unsigned long PPC_SUBOP_25 = 25;
static const unsigned long PPC_SUBOP_26 = 26;
static const unsigned long PPC_SUBOP_28 = 28;
static const unsigned long PPC_SUBOP_29 = 29;
static const unsigned long PPC_SUBOP_30 = 30;
static const unsigned long PPC_SUBOP_31 = 31;
static const unsigned long PPC_SUBOP_32 = 32;
static const unsigned long PPC_SUBOP_33 = 33;
static const unsigned long PPC_SUBOP_38 = 38;
static const unsigned long PPC_SUBOP_40 = 40;
static const unsigned long PPC_SUBOP_50 = 50;
static const unsigned long PPC_SUBOP_54 = 54;
static const unsigned long PPC_SUBOP_55 = 55;
static const unsigned long PPC_SUBOP_60 = 60;
static const unsigned long PPC_SUBOP_64 = 64;
static const unsigned long PPC_SUBOP_70 = 70;
static const unsigned long PPC_SUBOP_72 = 72;
static const unsigned long PPC_SUBOP_83 = 83;
static const unsigned long PPC_SUBOP_86 = 86;
static const unsigned long PPC_SUBOP_87 = 87;
static const unsigned long PPC_SUBOP_104 = 104;
static const unsigned long PPC_SUBOP_119 = 119;
static const unsigned long PPC_SUBOP_124 = 124;
static const unsigned long PPC_SUBOP_129 = 129;
static const unsigned long PPC_SUBOP_134 = 134;
static const unsigned long PPC_SUBOP_136 = 136;
static const unsigned long PPC_SUBOP_138 = 138;
static const unsigned long PPC_SUBOP_144 = 144;
static const unsigned long PPC_SUBOP_146 = 146;
static const unsigned long PPC_SUBOP_150 = 150;
static const unsigned long PPC_SUBOP_151 = 151;
static const unsigned long PPC_SUBOP_183 = 183;
static const unsigned long PPC_SUBOP_193 = 193;
static const unsigned long PPC_SUBOP_200 = 200;
static const unsigned long PPC_SUBOP_202 = 202;
static const unsigned long PPC_SUBOP_210 = 210;
static const unsigned long PPC_SUBOP_215 = 215;
static const unsigned long PPC_SUBOP_225 = 225;
static const unsigned long PPC_SUBOP_232 = 232;
static const unsigned long PPC_SUBOP_234 = 234;
static const unsigned long PPC_SUBOP_235 = 235;
static const unsigned long PPC_SUBOP_242 = 242;
static const unsigned long PPC_SUBOP_246 = 246;
static const unsigned long PPC_SUBOP_247 = 247;
static const unsigned long PPC_SUBOP_257 = 257;
static const unsigned long PPC_SUBOP_264 = 264;
static const unsigned long PPC_SUBOP_266 = 266;
static const unsigned long PPC_SUBOP_278 = 278;
static const unsigned long PPC_SUBOP_279 = 279;
static const unsigned long PPC_SUBOP_289 = 289;
static const unsigned long PPC_SUBOP_284 = 284;
static const unsigned long PPC_SUBOP_306 = 306;
static const unsigned long PPC_SUBOP_310 = 310;
static const unsigned long PPC_SUBOP_311 = 311;
static const unsigned long PPC_SUBOP_316 = 316;
static const unsigned long PPC_SUBOP_339 = 339;
static const unsigned long PPC_SUBOP_343 = 343;
static const unsigned long PPC_SUBOP_370 = 370;
static const unsigned long PPC_SUBOP_371 = 371;
static const unsigned long PPC_SUBOP_375 = 375;
static const unsigned long PPC_SUBOP_407 = 407;
static const unsigned long PPC_SUBOP_412 = 412;
static const unsigned long PPC_SUBOP_417 = 417;
static const unsigned long PPC_SUBOP_438 = 438;
static const unsigned long PPC_SUBOP_439 = 439;
static const unsigned long PPC_SUBOP_444 = 444;
static const unsigned long PPC_SUBOP_449 = 449;
static const unsigned long PPC_SUBOP_459 = 459;
static const unsigned long PPC_SUBOP_467 = 467;
static const unsigned long PPC_SUBOP_470 = 470;
static const unsigned long PPC_SUBOP_476 = 476;
static const unsigned long PPC_SUBOP_491 = 491;
static const unsigned long PPC_SUBOP_512 = 512;
static const unsigned long PPC_SUBOP_528 = 528;
static const unsigned long PPC_SUBOP_533 = 533;
static const unsigned long PPC_SUBOP_534 = 534;
static const unsigned long PPC_SUBOP_535 = 535;
static const unsigned long PPC_SUBOP_536 = 536;
static const unsigned long PPC_SUBOP_566 = 566;
static const unsigned long PPC_SUBOP_567 = 567;
static const unsigned long PPC_SUBOP_583 = 583;
static const unsigned long PPC_SUBOP_595 = 595;
static const unsigned long PPC_SUBOP_597 = 597;
static const unsigned long PPC_SUBOP_598 = 598;
static const unsigned long PPC_SUBOP_599 = 599;
static const unsigned long PPC_SUBOP_631 = 631;
static const unsigned long PPC_SUBOP_659 = 659;
static const unsigned long PPC_SUBOP_661 = 661;
static const unsigned long PPC_SUBOP_662 = 662;
static const unsigned long PPC_SUBOP_663 = 663;
static const unsigned long PPC_SUBOP_695 = 695;
static const unsigned long PPC_SUBOP_711 = 711;
static const unsigned long PPC_SUBOP_725 = 725;
static const unsigned long PPC_SUBOP_727 = 727;
static const unsigned long PPC_SUBOP_758 = 758;
static const unsigned long PPC_SUBOP_759 = 759;
static const unsigned long PPC_SUBOP_790 = 790;
static const unsigned long PPC_SUBOP_792 = 792;
static const unsigned long PPC_SUBOP_824 = 824;
static const unsigned long PPC_SUBOP_854 = 854;
static const unsigned long PPC_SUBOP_918 = 918;
static const unsigned long PPC_SUBOP_922 = 922;
static const unsigned long PPC_SUBOP_954 = 954;
static const unsigned long PPC_SUBOP_982 = 982;
static const unsigned long PPC_SUBOP_983 = 983;
static const unsigned long PPC_SUBOP_1014 = 1014;

// read in data
void
readdasm(ElfObject &eo, char *s)
{
	eo.readehdr();
	eo.readshdrs();
	eo.readphdrs();
	eo.readstrings();
	eo.readsymbols();
	return;
}

// return the bit field, bits range from 0 to 31
static unsigned long
bitval(unsigned long v, int lowbit, int highbit)
{
	MustBeTrue(lowbit <= highbit);
	MustBeTrue(0 <= lowbit && highbit <= 31);
	unsigned long v2 = ((v >> lowbit) << lowbit);
	unsigned long v3 = ((v2 << (31-highbit)) >> (31-highbit));
	unsigned long v4 = (v3 >> lowbit);
	return(v4);
}

// return the bit field, bits range from 31 to 0
static unsigned long
rbitval(unsigned long v, int lowbit, int highbit)
{
	MustBeTrue(lowbit <= highbit);
	MustBeTrue(0 <= lowbit && highbit <= 31);
	unsigned long v2 = ((v >> (31-highbit)) << (31-highbit));
	unsigned long v3 = ((v2 << lowbit) >> lowbit);
	unsigned long v4 = (v3 >> (31-highbit));
	return(v4);
}

// disassemble a block for sparc architecture
static void
sparc_dasm(long base, long size, unsigned char *buf)
{
	unsigned long opc, opf, unused, a, op, op2, op3, disp30, imm22, cond;
	unsigned long rd, rs1, i, rs2, simm13, asi, shcnt, disp22, const22;

	// step thru buffer and attempt to recognize
	unsigned long linesprinted = 0;
	for (unsigned char *pb=buf; pb < (buf+size); )
	{
		// bits 31,30 give the format. since we are trying
		// to synchronize with the text, we are stepping 
		// through the buffer byte-by-byte. then bits 31,30
		// in a LONG are bits 7,6 in the highest BYTE. that's
		// why bits 6,7 are pulled out in the following line,
		// and not 31,30. so leave it the way it is.
		//
		unsigned long op = bitval((unsigned long)*pb, 6, 7);
		unsigned long *pul = (unsigned long *)pb;

		// check alignment
		if (((unsigned long)pb)%sizeof(long) != 0)
		{
			// not aligned correctly, skip it
			pb += 1;
			continue;
		}

		// check lines printed
		if (maxlinestoprint > 0 && linesprinted >= maxlinestoprint)
		{
			char s[BUFSIZ];
			printf("continue? [n/y/cr=y] ");
			rmvnlgets(s);
			if (!(*s == 'y' || *s == '\0'))
				return;
			linesprinted = 0;
		}
		linesprinted += 1;

		// determine opcode and print
		switch (op)
		{
		case Op_0:
		{
			// format 2 (Bicc,FBfcc,CBccc,SETHI)
			op2 = bitval(*pul, 22, 24);
			switch (op2)
			{
			case Op2_unimp:
				const22 = bitval(*pul, 0, 21);
				printf("%08lx: %08lx\tunimp\tconst22=0x%lx\n", 
					base+(pb-buf), *pul, const22);
				break;

			case Op2_unimplemented_1:
				printf("%08lx: %08lx\top=0x%lx op2=0x%lx\n", 
					base+(pb-buf), *pul, op, op2);
				break;

			case Op2_bicc:
				a = bitval(*pul, 29, 29);
				cond = bitval(*pul, 25, 28);
				disp22 = bitval(*pul, 0, 21);
				printf("%08lx: %08lx\t%s\ta=%ld disp22=%ld\n",
					base+(pb-buf), *pul, 
					i2s(icond_trans, cond), a, disp22);
				break;

			case Op2_unimplemented_3:
				printf("%08lx: %08lx\top=0x%lx op2=0x%lx\n", 
					base+(pb-buf), *pul, op, op2);
				break;

			case Op2_sethi:
				imm22 = bitval(*pul, 0, 21);
				rd = bitval(*pul, 25, 29);
				if (rd == 0 && imm22 == 0)
				{
					printf("%08lx: %08lx\tnop\n",
						base+(pb-buf), *pul);
				}
				else
				{
					printf("%08lx: %08lx\tsethi\trd=%ld imm22=%ld\n",
						base+(pb-buf), *pul, rd, imm22);
				}
				break;

			case Op2_unimplemented_5:
				printf("%08lx: %08lx\top=0x%lx op2=0x%lx\n", 
					base+(pb-buf), *pul, op, op2);
				break;

			case Op2_fbfcc:
				a = bitval(*pul, 29, 29);
				cond = bitval(*pul, 25, 28);
				disp22 = bitval(*pul, 0, 21);
				printf("%08lx: %08lx\t%s\ta=%ld disp22=%ld\n",
					base+(pb-buf), *pul, 
					i2s(fcond_trans, cond), a, disp22);
				break;

			case Op2_cbccc:
				a = bitval(*pul, 29, 29);
				cond = bitval(*pul, 25, 28);
				disp22 = bitval(*pul, 0, 21);
				printf("%08lx: %08lx\t%s\ta=%ld disp22=%ld\n",
					base+(pb-buf), *pul, 
					i2s(ccond_trans, cond), a, disp22);
				break;
			}
			pb+=sizeof(long);
			break;
		}
		case Op_1:
		{
			// format 1 (call)
			disp30 = bitval(*pul, 0, 29);
			printf("%08lx: %08lx\tcall\t0x%lx\n", 
				base+(pb-buf), *pul, disp30);
			pb+=sizeof(long);
			break;
		}
		case Op_2:
		{
			// format 3 (arithmetic,logical,shift,etc)
			op3 = bitval(*pul, 19, 24);
			switch (op3)
			{
			case Op3_itrap:
				cond = bitval(*pul, 25, 28);
				rs1 = bitval(*pul, 14, 18);
				i = bitval(*pul, 13, 13);
				if (i == 0)
				{
					rs2 = bitval(*pul, 0, 4);
					printf("%08lx: %08lx\t%s\trs1=%ld rs2=%ld\n",
						base+(pb-buf), *pul, 
						i2s(tcond_trans, cond), 
						rs1, rs2);
				}
				else if (i == 1)
				{
					simm13 = bitval(*pul, 0, 12);
					printf("%08lx: %08lx\t%s\trs1=%ld simm13=%ld\n",
						base+(pb-buf), *pul, 
						i2s(tcond_trans, cond), 
						rs1, simm13);
				}
				else
				{
					pb += 1;
					continue;
				}
				break;

			case Op3_and:
			case Op3_andcc:
			case Op3_andn:
			case Op3_andncc:
			case Op3_or:
			case Op3_orcc:
			case Op3_orn:
			case Op3_orncc:
			case Op3_xor:
			case Op3_xorcc:
			case Op3_xnor:
			case Op3_xnorcc:

			case Op3_add:
			case Op3_addcc:
			case Op3_addx:
			case Op3_addxcc:
			case Op3_taddcc:
			case Op3_taddcctv:

			case Op3_sub:
			case Op3_subcc:
			case Op3_subx:
			case Op3_subxcc:
			case Op3_tsubcc:
			case Op3_tsubcctv:

			case Op3_mulscc:
			case Op3_umul:
			case Op3_smul:
			case Op3_umulcc:
			case Op3_smulcc:

			case Op3_udiv:
			case Op3_sdiv:
			case Op3_udivcc:
			case Op3_sdivcc:

			case Op3_save:
			case Op3_restore:

			case Op3_jmpl:
			case Op3_rett:

			case Op3_wry:
			case Op3_wrpsr:
			case Op3_wrwim:
			case Op3_wrtbr:
				rd = bitval(*pul, 25, 29);
				rs1 = bitval(*pul, 14, 18);
				i = bitval(*pul, 13, 13);
				if (i == 0)
				{
					rs2 = bitval(*pul, 0, 4);
					printf("%08lx: %08lx\t%s\trd=%ld rs1=%ld rs2=%ld\n",
					base+(pb-buf), *pul, 
					i2s(op_10_op3_trans, op3), 
					rd, rs1, rs2);
				}
				else if (i == 1)
				{
					simm13 = bitval(*pul, 0, 12);
					printf("%08lx: %08lx\t%s\trd=%ld rs1=%ld simm13=%ld\n",
						base+(pb-buf), *pul, 
						i2s(op_10_op3_trans, op3), 
						rd, rs1, simm13);
				}
				else
				{
					pb += 1;
					continue;
				}
				break;

			case Op3_flush:
				unused = bitval(*pul, 25, 29);
				MustBeTrue(unused == 0);
				rs1 = bitval(*pul, 14, 18);
				i = bitval(*pul, 13, 13);
				if (i == 0)
				{
					rs2 = bitval(*pul, 0, 4);
					printf("%08lx: %08lx\t%s\trs1=%ld rs2=%ld\n",
					base+(pb-buf), *pul, 
					i2s(op_10_op3_trans, op3), 
					rs1, rs2);
				}
				else if (i == 1)
				{
					simm13 = bitval(*pul, 0, 12);
					printf("%08lx: %08lx\t%s\trs1=%ld simm13=%ld\n",
						base+(pb-buf), *pul, 
						i2s(op_10_op3_trans, op3), 
						rs1, simm13);
				}
				else
				{
					pb += 1;
					continue;
				}
				break;


			case Op3_rdy:
				// rdy and stbar are the same,
				// except rd=0 and rs1=15
				//
				MustBeTrue(Op3_rdy == Op3_stbar);
				rd = bitval(*pul, 25, 29);
				rs1 = bitval(*pul, 14, 18);
				if (rd==0 && rs1==15)
				{
					unused = bitval(*pul, 14, 18);
					MustBeTrue(unused == 0xf);
					printf("%08lx: %08lx\tstbar\n",
						base+(pb-buf), *pul);
				}
				else
				{
					unused = bitval(*pul,0,13);
					MustBeTrue(unused == 0);
					printf("%08lx: %08lx\t%s\trd=%ld rs1=%ld\n",
						base+(pb-buf), *pul, 
						i2s(op_10_op3_trans, op3), 
						rd, rs1);
				}
				break;

			case Op3_rdpsr:
			case Op3_rdwim:
			case Op3_rdtbr:
				rd = bitval(*pul, 25, 29);
				rs1 = bitval(*pul, 14, 18);
				unused = bitval(*pul,0,13);
				MustBeTrue(unused == 0);
				printf("%08lx: %08lx\t%s\trd=%ld rs1=%ld\n",
					base+(pb-buf), *pul, 
					i2s(op_10_op3_trans, op3), 
					rd, rs1);
				break;

			case Op3_sll:
			case Op3_srl:
			case Op3_sra:
				rd = bitval(*pul, 25, 29);
				rs1 = bitval(*pul, 14, 18);
				i = bitval(*pul, 13, 13);
				if (i == 0)
				{
					rs2 = bitval(*pul, 0, 4);
					printf("%08lx: %08lx\t%s\trd=%ld rs1=%ld rs2=%ld\n",
						base+(pb-buf), *pul, 
						i2s(op_10_op3_trans, op3), 
						rd, rs1, rs2);
				}
				else if (i == 1)
				{
					shcnt = bitval(*pul, 0, 4);
					printf("%08lx: %08lx\t%s\trd=%ld rs1=%ld shcnt=%ld\n",
						base+(pb-buf), *pul, 
						i2s(op_10_op3_trans, op3), 
						rd, rs1, shcnt);
				}
				else
				{
					pb += 1;
					continue;
				}
				break;


			case Op3_fpop1:
				opf = bitval(*pul, 5, 13);
				rd = bitval(*pul, 25, 29);
				rs1 = bitval(*pul, 14, 18);
				rs2 = bitval(*pul, 0, 4);
				switch (opf)
				{
				case Opf_fitos:
				case Opf_fitod:
				case Opf_fitoq:
				case Opf_fstod:
				case Opf_fstoq:
				case Opf_fdtos:
				case Opf_fdtoq:
				case Opf_fqtos:
				case Opf_fqtod:
				case Opf_fmovs:
				case Opf_fnegs:
				case Opf_fabss:
				case Opf_fsqrts:
				case Opf_fsqrtd:
				case Opf_fsqrtq:
					printf("%08lx: %08lx\t%s\trd=%ld rs2=%ld\n",
						base+(pb-buf), *pul, 
						i2s(fpop1_opf_trans, opf), 
						rd, rs2);
					break;

				case Opf_fadds:
				case Opf_faddd:
				case Opf_faddq:
				case Opf_fsubs:
				case Opf_fsubd:
				case Opf_fsubq:
				case Opf_fmuls:
				case Opf_fmuld:
				case Opf_fmulq:
				case Opf_fsmuld:
				case Opf_fdmulq:
				case Opf_fdivs:
				case Opf_fdivd:
				case Opf_fdivq:
					printf("%08lx: %08lx\t%s\trd=%ld rs1=%ld rs2=%ld\n",
						base+(pb-buf), *pul, 
						i2s(fpop1_opf_trans, opf), 
						rd, rs1, rs2);
					break;

				case Opf_fcmps:
				case Opf_fcmpd:
				case Opf_fcmpq:
				case Opf_fcmpes:
				case Opf_fcmped:
				case Opf_fcmpeq:
					printf("%08lx: %08lx\t%s\trs1=%ld rs2=%ld\n",
						base+(pb-buf), *pul, 
						i2s(fpop1_opf_trans, opf), 
						rs1, rs2);
					break;

				default:
					printf("%08lx: %08lx\top=0x%lx op3=0x%lx\n", 
						base+(pb-buf), *pul, op, op3);
					break;
				}
				break;

			case Op3_fpop2:
				opf = bitval(*pul, 5, 13);
				rs1 = bitval(*pul, 14, 18);
				rs2 = bitval(*pul, 0, 4);
				switch (opf)
				{
				case Opf_fcmps:
				case Opf_fcmpd:
				case Opf_fcmpq:
				case Opf_fcmpes:
				case Opf_fcmped:
				case Opf_fcmpeq:
					printf("%08lx: %08lx\t%s\trs1=%ld rs2=%ld\n",
						base+(pb-buf), *pul, 
						i2s(fpop1_opf_trans, opf), 
						rs1, rs2);
					break;

				default:
					printf("%08lx: %08lx\top=0x%lx op3=0x%lx\n", 
						base+(pb-buf), *pul, op, op3);
					break;
				}
				break;

			case Op3_cpop1:
			case Op3_cpop2:
				opc = bitval(*pul, 5, 13);
				rd = bitval(*pul, 25, 29);
				rs1 = bitval(*pul, 14, 18);
				rs2 = bitval(*pul, 0, 4);
				printf("%08lx: %08lx\t%s\trd=%ld rs1=%ld opc=%ld rs2=%ld\n",
					base+(pb-buf), *pul, 
					i2s(op_10_op3_trans, op3), 
					rd, rs1, opc, rs2);
				break;

			default:
				printf("%08lx: %08lx\top=0x%lx op3=0x%lx\n", 
					base+(pb-buf), *pul, op, op3);
				break;
			}
			pb+=sizeof(long);
			break;
		}
		case Op_3:
		{
			// format 3 (memory instructions)
			op3 = bitval(*pul, 19, 24);
			switch (op3)
			{
			case Op3_ldsb:
			case Op3_ldsh:
			case Op3_ldub:
			case Op3_lduh:
			case Op3_ld:
			case Op3_ldd:
			case Op3_ldsba:
			case Op3_ldsha:
			case Op3_lduba:
			case Op3_lduha:
			case Op3_lda:
			case Op3_ldda:

			case Op3_stb:
			case Op3_sth:
			case Op3_st:
			case Op3_std:
			case Op3_stba:
			case Op3_stha:
			case Op3_sta:
			case Op3_stda:

			case Op3_ldstub:
			case Op3_ldstuba:

			case Op3_swap:
			case Op3_swapa:
				rd = bitval(*pul, 25, 29);
				rs1 = bitval(*pul, 14, 18);
				i = bitval(*pul, 13, 13);
				if (i == 0)
				{
					asi = bitval(*pul, 5, 12);
					rs2 = bitval(*pul, 0, 4);
					printf("%08lx: %08lx\t%s\trd=%ld rs1=%ld asi=%ld rs2=%ld\n",
						base+(pb-buf), *pul, 
						i2s(op_11_op3_trans, op3), 
						rd, rs1, asi, rs2);
				}
				else if (i == 1)
				{
					simm13 = bitval(*pul, 0, 12);
					printf("%08lx: %08lx\t%s\trd=%ld rs1=%ld simm13=%ld\n",
						base+(pb-buf), *pul, 
						i2s(op_11_op3_trans, op3), 
						rd, rs1, simm13);
				}
				else
				{
					pb += 1;
					continue;
				}
				break;

			case Op3_ldf:
			case Op3_lddf:
			case Op3_ldfsr:

			case Op3_ldc:
			case Op3_lddc:
			case Op3_ldcsr:

			case Op3_stf:
			case Op3_stdf:
			case Op3_stfsr:
			case Op3_stdfq:

			case Op3_stc:
			case Op3_stdc:
			case Op3_stcsr:
			case Op3_stdcq:
				rd = bitval(*pul, 25, 29);
				rs1 = bitval(*pul, 14, 18);
				i = bitval(*pul, 13, 13);
				if (i == 0)
				{
					rs2 = bitval(*pul, 0, 4);
					printf("%08lx: %08lx\t%s\trd=%ld rs1=%ld rs2=%ld\n",
						base+(pb-buf), *pul, 
						i2s(op_11_op3_trans, op3), 
						rd, rs1, rs2);
				}
				else if (i == 1)
				{
					simm13 = bitval(*pul, 0, 12);
					printf("%08lx: %08lx\t%s\trd=%ld rs1=%ld simm13=%ld\n",
						base+(pb-buf), *pul, 
						i2s(op_11_op3_trans, op3), 
						rd, rs1, simm13);
				}
				else
				{
					pb += 1;
					continue;
				}
				break;

			default:
				printf("%08lx: %08lx\top=0x%lx op3=0x%lx\n", 
					base+(pb-buf), *pul, op, op3);
				break;
			}
			pb+=sizeof(long);
			break;
		}
		}
	}
}

// utility functions for printing ppc assembler.
void
D_form_D_A_d(long addr, unsigned long machcode, const char *opcode)
{
	printf("%08lx: %08lx\t%s\tD=%ld A=%ld d=%ld\n",
		addr, machcode, opcode,
		rbitval(machcode, 6, 10),
		rbitval(machcode, 11, 15),
		rbitval(machcode, 16, 31));
}

void
D_form_S_A_d(long addr, unsigned long machcode, const char *opcode)
{
	printf("%08lx: %08lx\t%s\tS=%ld A=%ld d=%ld\n",
		addr, machcode, opcode,
		rbitval(machcode, 6, 10),
		rbitval(machcode, 11, 15),
		rbitval(machcode, 16, 31));
}

void
D_form_D_A_SIMM(long addr, unsigned long machcode, const char *opcode)
{
	printf("%08lx: %08lx\t%s\tD=%ld A=%ld SIMM=%ld\n",
		addr, machcode, opcode,
		rbitval(machcode, 6, 10),
		rbitval(machcode, 11, 15),
		rbitval(machcode, 16, 31));
}

void
D_form_S_A_UIMM(long addr, unsigned long machcode, const char *opcode)
{
	printf("%08lx: %08lx\t%s\tS=%ld A=%ld UIMM=%ld\n",
		addr, machcode, opcode,
		rbitval(machcode, 6, 10),
		rbitval(machcode, 11, 15),
		rbitval(machcode, 16, 31));
}

void
D_form_crfD_L_A_XIMM(long addr, unsigned long machcode, 
		const char *copcode, const char *ximm,
		unsigned long opcode, unsigned long operands)
{
	if (rbitval(machcode, 9, 9) != 0)
	{
		printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
			addr, machcode, opcode, operands);
	}
	else
	{
		printf("%08lx: %08lx\t%s\tcrfD=%ld L=%ld A=%ld %s=%ld\n",
			addr, machcode, copcode, 
			rbitval(machcode, 6, 8),
			rbitval(machcode, 10, 10),
			rbitval(machcode, 11, 15),
			ximm, rbitval(machcode, 16, 31));
	}
}

void
X_form_D_A_B_XO_0(long addr, unsigned long machcode, const char *copcode,
			unsigned long opcode, unsigned long operands)
{
	if (rbitval(machcode, 31, 31) != 0)
	{
		printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
			addr, machcode, opcode, operands);
	}
	else
	{
		printf("%08lx: %08lx\t%s\tD=%ld A=%ld B=%ld\n",
			addr, machcode, copcode,
			rbitval(machcode, 6, 10),
			rbitval(machcode, 11, 15),
			rbitval(machcode, 16, 20));
	}
}

void
X_form_S_A_B_XO_0(long addr, unsigned long machcode, const char *copcode,
			unsigned long opcode, unsigned long operands)
{
	if (rbitval(machcode, 31, 31) != 0)
	{
		printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
			addr, machcode, opcode, operands);
	}
	else
	{
		printf("%08lx: %08lx\t%s\tS=%ld A=%ld B=%ld\n",
			addr, machcode, copcode,
			rbitval(machcode, 6, 10),
			rbitval(machcode, 11, 15),
			rbitval(machcode, 16, 20));
	}
}

void
X_form_0_A_B_0(long addr, unsigned long machcode, const char *copcode,
			unsigned long opcode, unsigned long operands)
{
	if (rbitval(machcode, 6, 10) != 0 ||
	    rbitval(machcode, 31, 31) != 0)
	{
		printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
			addr, machcode, opcode, operands);
	}
	else
	{
		printf("%08lx: %08lx\t%s\tA=%ld B=%ld\n",
			addr, machcode, copcode,
			rbitval(machcode, 11, 15),
			rbitval(machcode, 16, 20));
	}
}

void
X_form_D_0_B_Rc(long addr, unsigned long machcode, const char *copcode,
		unsigned long opcode, unsigned long operands)
{
	if (rbitval(machcode, 11, 15) != 0)
	{
		printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
			addr, machcode, opcode, operands);
	}
	else
	{
		printf("%08lx: %08lx\t%s\tD=%ld B=%ld Rc=%ld\n",
			addr, machcode, copcode, 
			rbitval(machcode, 6, 8),
			rbitval(machcode, 16, 20),
			rbitval(machcode, 31, 31));
	}
}

void
X_form_0_0(long addr, unsigned long machcode, const char *copcode,
		unsigned long opcode, unsigned long operands)
{
	if (rbitval(machcode, 6, 20) != 0 ||
	    rbitval(machcode, 31, 31) != 0)
	{
		printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
			addr, machcode, opcode, operands);
	}
	else
	{
		printf("%08lx: %08lx\t%s\n",
			addr, machcode, copcode);
	}
}

void
X_form_S_A_B_XO_Rc(long addr, unsigned long machcode, const char *copcode)
{
	printf("%08lx: %08lx\t%s\tS=%ld A=%ld B=%ld Rc=%ld\n",
		addr, machcode, copcode,
		rbitval(machcode, 6, 10),
		rbitval(machcode, 11, 15),
		rbitval(machcode, 16, 20),
		rbitval(machcode, 31, 31));
}

void
A_form_D_A_B_C_Rc(long addr, unsigned long machcode, const char *copcode)
{
	printf("%08lx: %08lx\t%s\tD=%ld A=%ld B=%ld C=%ld Rc=%ld\n",
		addr, machcode, copcode,
		rbitval(machcode, 6, 10),
		rbitval(machcode, 11, 15),
		rbitval(machcode, 16, 20),
		rbitval(machcode, 21, 25),
		rbitval(machcode, 31, 31));
}

void
XL_form_crbD_crbA_crbB_0(long addr, unsigned long machcode, const char *copcode,
			unsigned long opcode, unsigned long operands)
{
	if (rbitval(machcode, 31, 31) != 0)
	{
		printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
			addr, machcode, opcode, operands);
	}
	else
	{
		printf("%08lx: %08lx\t%s\tcrbD=%ld crbA=%ld crbB=%ld\n",
			addr, machcode, copcode, 
			rbitval(machcode, 6, 10),
			rbitval(machcode, 11, 15),
			rbitval(machcode, 16, 20));
	}
}

void
XO_form_D_A_B_OE_Rc(long addr, unsigned long machcode, const char *copcode)
{
	printf("%08lx: %08lx\t%s\tD=%ld A=%ld B=%ld OE=%ld Rc=%ld\n",
		addr, machcode, copcode,
		rbitval(machcode, 6, 10),
		rbitval(machcode, 11, 15),
		rbitval(machcode, 16, 20),
		rbitval(machcode, 21, 21),
		rbitval(machcode, 31, 31));
}

void
XO_form_D_A_0_OE_Rc(long addr, unsigned long machcode, const char *copcode,
		unsigned long opcode, unsigned long operands)
{

	if (rbitval(machcode, 16, 20) != 0)
	{
		printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
			addr, machcode, opcode, operands);
	}
	else
	{
		printf("%08lx: %08lx\t%s\tD=%ld A=%ld OE=%ld Rc=%ld\n",
			addr, machcode, copcode,
			rbitval(machcode, 6, 10),
			rbitval(machcode, 11, 15),
			rbitval(machcode, 21, 21),
			rbitval(machcode, 31, 31));
	}
}

// disassemble a block for ppc architecture
static void
ppc_dasm(long base, long size, unsigned char *buf)
{
	// step thru buffer and attempt to recognize
	unsigned long linesprinted = 0;
	for (unsigned char *pb=buf; pb < (buf+size); )
	{
		// check alignment, since we are trying
		// to synchronize with the text, we are stepping 
		// through the buffer byte-by-byte. 
		//
		if (((unsigned long)pb)%sizeof(long) != 0)
		{
			// not aligned correctly, skip it
			pb += 1;
			continue;
		}

		// check lines printed
		if (maxlinestoprint > 0 && linesprinted >= maxlinestoprint)
		{
			char s[BUFSIZ];
			printf("continue? [n/y/cr=y] ");
			rmvnlgets(s);
			if (!(*s == 'y' || *s == '\0'))
				return;
			linesprinted = 0;
		}
		linesprinted += 1;

		// bits 0-5 gives the opcode. 
		unsigned long *pul = (unsigned long *)pb;
		unsigned long opcode = rbitval(*pul, 0, 5);

		// get remaining operands
		unsigned long operands = rbitval(*pul, 6, 31);

		// determine opcode and print
		switch (opcode)
		{
		case PPC_OP_00:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_01:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_02:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_03:
		{
			printf("%08lx: %08lx\ttwi\tTO=%ld A=%ld SIMM=%ld\n",
				base+(pb-buf), *pul, 
				rbitval(*pul, 6, 10),
				rbitval(*pul, 11, 15),
				rbitval(*pul, 16, 31));
			break;
		}
		case PPC_OP_04:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_05:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_06:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_07:
		{
			D_form_D_A_SIMM(base+(pb-buf), *pul, "mulli");
			break;
		}
		case PPC_OP_08:
		{
			D_form_D_A_SIMM(base+(pb-buf), *pul, "subfic");
			break;
		}
		case PPC_OP_09:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_10:
		{
			D_form_crfD_L_A_XIMM(base+(pb-buf), *pul, 
				"cmpli", "UIMM", opcode, operands);
			break;
		}
		case PPC_OP_11:
		{
			D_form_crfD_L_A_XIMM(base+(pb-buf), *pul, 
				"cmpi", "SIMM", opcode, operands);
			break;
		}
		case PPC_OP_12:
		{
			D_form_D_A_SIMM(base+(pb-buf), *pul, "addic");
			break;
		}
		case PPC_OP_13:
		{
			D_form_D_A_SIMM(base+(pb-buf), *pul, "addic.");
			break;
		}
		case PPC_OP_14:
		{
			D_form_D_A_SIMM(base+(pb-buf), *pul, "addi.");
			break;
		}
		case PPC_OP_15:
		{
			D_form_D_A_SIMM(base+(pb-buf), *pul, "addis");
			break;
		}
		case PPC_OP_16:
		{
			printf("%08lx: %08lx\tbcx\tBO=%ld Bl=%ld BD=%ld AA=%ld LK=%ld\n",
				base+(pb-buf), *pul, 
				rbitval(*pul, 6, 10),
				rbitval(*pul, 11, 15),
				rbitval(*pul, 16, 29),
				rbitval(*pul, 30, 30),
				rbitval(*pul, 31, 31));
			break;
		}
		case PPC_OP_17:
		{
			if (rbitval(*pul, 6, 29) != 0 ||
			    rbitval(*pul, 30, 30) != 1 ||
			    rbitval(*pul, 31, 31) != 0)
			{
				printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
					base+(pb-buf), *pul, opcode, operands);
			}
			else
			{
				printf("%08lx: %08lx\tsc\n",
					base+(pb-buf), *pul);
			}
			break;
		}
		case PPC_OP_18:
		{
			// bx
			printf("%08lx: %08lx\tbx\tLI=%ld AA=%ld LK=%ld\n",
				base+(pb-buf), *pul, 
				rbitval(*pul, 6, 29),
				rbitval(*pul, 30, 30),
				rbitval(*pul, 31, 31));
			break;
		}
		case PPC_OP_19:
		{
			unsigned long subopcode = rbitval(*pul, 21, 30);
			switch (subopcode)
			{
			case PPC_SUBOP_00:
			{
				if (rbitval(*pul, 9, 10) != 0 ||
				    rbitval(*pul, 14, 20) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmcrf\tcrfD=%ld crfS=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 8),
						rbitval(*pul, 11, 13));
				}
				break;
			}
			case PPC_SUBOP_16:
			{
				if (rbitval(*pul, 16, 20) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tbctrx\tBO=%ld Bl=%ld LK=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			case PPC_SUBOP_18:
			{
				if (rbitval(*pul, 6, 20) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\trfid\n",
						base+(pb-buf), *pul);
				}
				break;
			}
			case PPC_SUBOP_33:
			{
				XL_form_crbD_crbA_crbB_0(base+(pb-buf), 
					*pul, "crnor", opcode, operands);
				break;
			}
			case PPC_SUBOP_50:
			{
				if (rbitval(*pul, 6, 20) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\trfi\n",
						base+(pb-buf), *pul);
				}
				break;
			}
			case PPC_SUBOP_129:
			{
				XL_form_crbD_crbA_crbB_0(base+(pb-buf), 
					*pul, "crandc", opcode, operands);
				break;
			}
			case PPC_SUBOP_150:
			{
				if (rbitval(*pul, 6, 20) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tlsync\n",
						base+(pb-buf), *pul);
				}
				break;
			}
			case PPC_SUBOP_193:
			{
				XL_form_crbD_crbA_crbB_0(base+(pb-buf), 
					*pul, "crxor", opcode, operands);
				break;
			}
			case PPC_SUBOP_225:
			{
				XL_form_crbD_crbA_crbB_0(base+(pb-buf), 
					*pul, "crnand", opcode, operands);
				break;
			}
			case PPC_SUBOP_257:
			{
				XL_form_crbD_crbA_crbB_0(base+(pb-buf), 
					*pul, "crand", opcode, operands);
				break;
			}
			case PPC_SUBOP_289:
			{
				XL_form_crbD_crbA_crbB_0(base+(pb-buf), 
					*pul, "creqv", opcode, operands);
				break;
			}
			case PPC_SUBOP_417:
			{
				XL_form_crbD_crbA_crbB_0(base+(pb-buf), 
					*pul, "crorc", opcode, operands);
				break;
			}
			case PPC_SUBOP_449:
			{
				XL_form_crbD_crbA_crbB_0(base+(pb-buf), 
					*pul, "cror", opcode, operands);
				break;
			}
			case PPC_SUBOP_528:
			{
				if (rbitval(*pul, 16, 20) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tbcctrx\tBO=%ld Bl=%ld LK=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			default:
			{
				printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
					base+(pb-buf), *pul, opcode, operands);
				break;
			}
			}
			break;
		}
		case PPC_OP_20:
		{
			printf("%08lx: %08lx\trlwimix\tS=%ld A=%ld SH=%ld MB=%ld ME=%ld Rc=%ld\n",
				base+(pb-buf), *pul, 
				rbitval(*pul, 6, 10),
				rbitval(*pul, 11, 15),
				rbitval(*pul, 16, 20),
				rbitval(*pul, 21, 25),
				rbitval(*pul, 26, 30),
				rbitval(*pul, 31, 31));
			break;
		}
		case PPC_OP_21:
		{
			printf("%08lx: %08lx\trlwinmx\tS=%ld A=%ld SH=%ld MB=%ld ME=%ld Rc=%ld\n",
				base+(pb-buf), *pul, 
				rbitval(*pul, 6, 10),
				rbitval(*pul, 11, 15),
				rbitval(*pul, 16, 20),
				rbitval(*pul, 21, 25),
				rbitval(*pul, 26, 30),
				rbitval(*pul, 31, 31));
			break;
		}
		case PPC_OP_22:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_23:
		{
			printf("%08lx: %08lx\trlwnmx\tS=%ld A=%ld B=%ld MB=%ld ME=%ld Rc=%ld\n",
				base+(pb-buf), *pul, 
				rbitval(*pul, 6, 10),
				rbitval(*pul, 11, 15),
				rbitval(*pul, 16, 20),
				rbitval(*pul, 21, 25),
				rbitval(*pul, 26, 30),
				rbitval(*pul, 31, 31));
			break;
		}
		case PPC_OP_24:
		{
			D_form_S_A_UIMM(base+(pb-buf), *pul, "ori");
			break;
		}
		case PPC_OP_25:
		{
			D_form_S_A_UIMM(base+(pb-buf), *pul, "oris");
			break;
		}
		case PPC_OP_26:
		{
			printf("%08lx: %08lx\txori\tS%ld A=%ld UIMM=%ld\n",
				base+(pb-buf), *pul, 
				rbitval(*pul, 6, 10),
				rbitval(*pul, 11, 15),
				rbitval(*pul, 16, 31));
			break;
		}
		case PPC_OP_27:
		{
			printf("%08lx: %08lx\txoris\tS%ld A=%ld UIMM=%ld\n",
				base+(pb-buf), *pul, 
				rbitval(*pul, 6, 10),
				rbitval(*pul, 11, 15),
				rbitval(*pul, 16, 31));
			break;
		}
		case PPC_OP_28:
		{
			D_form_S_A_UIMM(base+(pb-buf), *pul, "andi.");
			break;
		}
		case PPC_OP_29:
		{
			D_form_S_A_UIMM(base+(pb-buf), *pul, "andis.");
			break;
		}
		case PPC_OP_30:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_31:
		{
			unsigned long subopcode = rbitval(*pul, 21, 30);
			switch (subopcode)
			{
			case PPC_SUBOP_00:
			{
				if (rbitval(*pul, 9, 9) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tcmp\tcrfD=%ld L=%ld A=%ld B=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 8),
						rbitval(*pul, 10, 10),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 16, 20));
				}
				break;
			}
			case PPC_SUBOP_04:
			{
				if (rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\ttw\tTO=%ld A=%ld B=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 16, 20));
				}
				break;
			}
			case PPC_SUBOP_19:
			{
				if (rbitval(*pul, 11, 20) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmfcr\tD=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10));
				}
				break;
			}
			case PPC_SUBOP_20:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lwarx", opcode, operands);
				break;
			}
			case PPC_SUBOP_23:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lwzx", opcode, operands);
				break;
			}
			case PPC_SUBOP_24:
			{
				X_form_S_A_B_XO_Rc(
					base+(pb-buf), *pul, "slws");
				break;
			}
			case PPC_SUBOP_26:
			{
				if (rbitval(*pul, 16, 20) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tcntlzwx\tS=%ld A=%ld Rc=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			case PPC_SUBOP_28:
			{
				X_form_S_A_B_XO_Rc(
					base+(pb-buf), *pul, "andx");
				break;
			}
			case PPC_SUBOP_32:
			{
				if (rbitval(*pul, 9, 9) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tcmpl\tcrfD=%ld L=%ld A=%ld B=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 8),
						rbitval(*pul, 10, 10),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 16, 20));
				}
				break;
			}
			case PPC_SUBOP_54:
			{
				X_form_0_A_B_0(base+(pb-buf), 
					*pul, "dcbst", opcode, operands);
				break;
			}
			case PPC_SUBOP_55:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lwzux", opcode, operands);
				break;
			}
			case PPC_SUBOP_60:
			{
				X_form_S_A_B_XO_Rc(
					base+(pb-buf), *pul, "andcx");
				break;
			}
			case PPC_SUBOP_83:
			{
				if (rbitval(*pul, 11, 20) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmfmsr\tD=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10));
				}
				break;
			}
			case PPC_SUBOP_86:
			{
				X_form_0_A_B_0(base+(pb-buf), 
					*pul, "dcbf", opcode, operands);
				break;
			}
			case PPC_SUBOP_87:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lbzx", opcode, operands);
				break;
			}
			case PPC_SUBOP_119:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lbzux", opcode, operands);
				break;
			}
			case PPC_SUBOP_124:
			{
				X_form_S_A_B_XO_Rc(
					base+(pb-buf), *pul, "norx");
				break;
			}
			case PPC_SUBOP_144:
			{
				if (rbitval(*pul, 11, 11) != 0 ||
				    rbitval(*pul, 20, 20) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmtcrf\tS=%ld CRM=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 12, 19));
				}
				break;
			}
			case PPC_SUBOP_146:
			{
				if (rbitval(*pul, 11, 20) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmtmsr\tS=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10));
				}
				break;
			}
			case PPC_SUBOP_150:
			{
				if (rbitval(*pul, 31, 31) != 1)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tstwcx.\tS=%ld A=%ld B=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 16, 20));
				}
				break;
			}
			case PPC_SUBOP_151:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "stwx", opcode, operands);
				break;
			}
			case PPC_SUBOP_183:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "stwux", opcode, operands);
				break;
			}
			case PPC_SUBOP_210:
			{
				if (rbitval(*pul, 11, 11) != 0 ||
				    rbitval(*pul, 16, 20) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmtsr\tS=%ld SR=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 12, 15));
				}
				break;
			}
			case PPC_SUBOP_215:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "stbx", opcode, operands);
				break;
			}
			case PPC_SUBOP_242:
			{
				if (rbitval(*pul, 11, 15) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmtsrin\tS=%ld B=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 16, 20));
				}
				break;
			}
			case PPC_SUBOP_246:
			{
				X_form_0_A_B_0(base+(pb-buf), 
					*pul, "dcbtst", opcode, operands);
				break;
			}
			case PPC_SUBOP_247:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "stbux", opcode, operands);
				break;
			}
			case PPC_SUBOP_266:
			{
				XO_form_D_A_B_OE_Rc(
					base+(pb-buf), *pul, "addx");
				break;
			}
			case PPC_SUBOP_278:
			{
				X_form_0_A_B_0(base+(pb-buf), 
					*pul, "dcbt", opcode, operands);
				break;
			}
			case PPC_SUBOP_279:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lhzx", opcode, operands);
				break;
			}
			case PPC_SUBOP_284:
			{
				X_form_S_A_B_XO_Rc(
					base+(pb-buf), *pul, "eqvx");
				break;
			}
			case PPC_SUBOP_306:
			{
				if (rbitval(*pul, 6, 15) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\ttlbie\tB=%ld\n",
						base+(pb-buf), *pul,
						rbitval(*pul, 16, 20));
				}
				break;
			}
			case PPC_SUBOP_310:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "eciwx", opcode, operands);
				break;
			}
			case PPC_SUBOP_311:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lhzux", opcode, operands);
				break;
			}
			case PPC_SUBOP_316:
			{
				X_form_S_A_B_XO_Rc(
					base+(pb-buf), *pul, "xorx");
				break;
			}
			case PPC_SUBOP_339:
			{
				if (rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmfspr\tD=%ld spr=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 20));
				}
				break;
			}
			case PPC_SUBOP_343:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lhax", opcode, operands);
				break;
			}
			case PPC_SUBOP_370:
			{
				X_form_0_0(base+(pb-buf), 
					*pul, "tlbia", opcode, operands);
				break;
			}
			case PPC_SUBOP_371:
			{
				if (rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmftb\tD=%ld tbr=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 20));
				}
				break;
			}
			case PPC_SUBOP_375:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lhaux", opcode, operands);
				break;
			}
			case PPC_SUBOP_407:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "sthx", opcode, operands);
				break;
			}
			case PPC_SUBOP_412:
			{
				X_form_S_A_B_XO_Rc(
					base+(pb-buf), *pul, "orcx");
				break;
			}
			case PPC_SUBOP_438:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "ecowx", opcode, operands);
				break;
			}
			case PPC_SUBOP_439:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "sthux", opcode, operands);
				break;
			}
			case PPC_SUBOP_444:
			{
				X_form_S_A_B_XO_Rc(
					base+(pb-buf), *pul, "orx");
				break;
			}
			case PPC_SUBOP_467:
			{
				if (rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmtspr\tD=%ld spr=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 20));
				}
				break;
			}
			case PPC_SUBOP_470:
			{
				X_form_0_A_B_0(base+(pb-buf), 
					*pul, "dcbi", opcode, operands);
				break;
			}
			case PPC_SUBOP_476:
			{
				X_form_S_A_B_XO_Rc(
					base+(pb-buf), *pul, "nandx");
				break;
			}
			case PPC_SUBOP_512:
			{
				if (rbitval(*pul, 9, 20) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmcrxr\tcrfD=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 8));
				}
				break;
			}
			case PPC_SUBOP_533:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lswx", opcode, operands);
				break;
			}
			case PPC_SUBOP_534:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lwbrx", opcode, operands);
				break;
			}
			case PPC_SUBOP_535:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lfsx", opcode, operands);
				break;
			}
			case PPC_SUBOP_536:
			{
				X_form_S_A_B_XO_Rc(
					base+(pb-buf), *pul, "srwx");
				break;
			}
			case PPC_SUBOP_566:
			{
				X_form_0_0(base+(pb-buf), 
					*pul, "tlbsync", opcode, operands);
				break;
			}
			case PPC_SUBOP_567:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lfsux", opcode, operands);
				break;
			}
			case PPC_SUBOP_597:
			{
				if (rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tlswi\tD=%ld A=%ld NB=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 16, 20));
				}
				break;
			}
			case PPC_SUBOP_598:
			{
				X_form_0_0(base+(pb-buf), 
					*pul, "sync", opcode, operands);
				break;
			}
			case PPC_SUBOP_595:
			{
				if (rbitval(*pul, 11, 11) != 0 ||
				    rbitval(*pul, 16, 20) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmfsr\tD=%ld SR=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 12, 15));
				}
				break;
			}
			case PPC_SUBOP_599:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lfdx", opcode, operands);
				break;
			}
			case PPC_SUBOP_631:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lfdux", opcode, operands);
				break;
			}
			case PPC_SUBOP_659:
			{
				if (rbitval(*pul, 11, 15) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmfsrin\tD=%ld B=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 16, 20));
				}
				break;
			}
			case PPC_SUBOP_661:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "stswx", opcode, operands);
				break;
			}
			case PPC_SUBOP_662:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "stwbrx", opcode, operands);
				break;
			}
			case PPC_SUBOP_663:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "stfsx", opcode, operands);
				break;
			}
			case PPC_SUBOP_695:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "stfsux", opcode, operands);
				break;
			}
			case PPC_SUBOP_711:
			{
				if (rbitval(*pul, 6, 6) != 0 ||
				    rbitval(*pul, 15, 15) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmtfsfx\tFM=%ld B=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 7, 14),
						rbitval(*pul, 16, 20));
				}
				break;
			}
			case PPC_SUBOP_725:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "stswi", opcode, operands);
				break;
			}
			case PPC_SUBOP_727:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "stfdx", opcode, operands);
				break;
			}
			case PPC_SUBOP_758:
			{
				X_form_0_A_B_0(base+(pb-buf), 
					*pul, "dcba", opcode, operands);
				break;
			}
			case PPC_SUBOP_759:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "stfdux", opcode, operands);
				break;
			}
			case PPC_SUBOP_790:
			{
				X_form_D_A_B_XO_0(base+(pb-buf), 
					*pul, "lhbrx", opcode, operands);
				break;
			}
			case PPC_SUBOP_792:
			{
				X_form_S_A_B_XO_Rc(
					base+(pb-buf), *pul, "srawx");
				break;
			}
			case PPC_SUBOP_824:
			{
				printf("%08lx: %08lx\tsrawix\tS=%ld A=%ld SH=%ld Rc=%ld\n",
					base+(pb-buf), *pul, 
					rbitval(*pul, 6, 10),
					rbitval(*pul, 11, 15),
					rbitval(*pul, 16, 20),
					rbitval(*pul, 31, 31));
				break;
			}
			case PPC_SUBOP_854:
			{
				X_form_0_0(base+(pb-buf), 
					*pul, "eieio", opcode, operands);
				break;
			}
			case PPC_SUBOP_918:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "sthbrx", opcode, operands);
				break;
			}
			case PPC_SUBOP_922:
			{
				if (rbitval(*pul, 16, 20) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\textshx\tS=%ld A=%ld Rc=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			case PPC_SUBOP_954:
			{
				if (rbitval(*pul, 16, 20) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\textsbx\tS=%ld A=%ld Rc=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			case PPC_SUBOP_982:
			{
				if (rbitval(*pul, 6, 10) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\ticbi\tA=%ld B=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 11, 15),
						rbitval(*pul, 16, 20));
				}
				break;
			}
			case PPC_SUBOP_983:
			{
				X_form_S_A_B_XO_0(base+(pb-buf), 
					*pul, "stfiwx", opcode, operands);
				break;
			}
			case PPC_SUBOP_1014:
			{
				X_form_0_A_B_0(base+(pb-buf), 
					*pul, "dcbz", opcode, operands);
				break;
			}
			default:
			{
				subopcode = rbitval(*pul, 22, 30);
				switch (subopcode)
				{
				case PPC_SUBOP_104:
				{
					XO_form_D_A_0_OE_Rc(base+(pb-buf), 
						*pul, "negx", 
						opcode, operands);
					break;
				}
				case PPC_SUBOP_200:
				{
					XO_form_D_A_0_OE_Rc(base+(pb-buf), 
						*pul, "subfzex", 
						opcode, operands);
					break;
				}
				case PPC_SUBOP_202:
				{
					XO_form_D_A_0_OE_Rc(base+(pb-buf), 
						*pul, "addzex", 
						opcode, operands);
					break;
				}
				case PPC_SUBOP_232:
				{
					XO_form_D_A_0_OE_Rc(base+(pb-buf), 
						*pul, "subfmex", 
						opcode, operands);
					break;
				}
				case PPC_SUBOP_234:
				{
					XO_form_D_A_0_OE_Rc(base+(pb-buf), 
						*pul, "addmex", 
						opcode, operands);
					break;
				}
				case PPC_SUBOP_08:
				{
					XO_form_D_A_B_OE_Rc(
						base+(pb-buf), *pul, "subfcx");
					break;
				}
				case PPC_SUBOP_10:
				{
					XO_form_D_A_B_OE_Rc(
						base+(pb-buf), *pul, "addcx");
					break;
				}
				case PPC_SUBOP_40:
				{
					XO_form_D_A_B_OE_Rc(
						base+(pb-buf), *pul, "subfx");
					break;
				}
				case PPC_SUBOP_136:
				{
					XO_form_D_A_B_OE_Rc(
						base+(pb-buf), *pul, "subfex");
					break;
				}
				case PPC_SUBOP_138:
				{
					XO_form_D_A_B_OE_Rc(
						base+(pb-buf), *pul, "addex");
					break;
				}
				case PPC_SUBOP_235:
				{
					XO_form_D_A_B_OE_Rc(
						base+(pb-buf), *pul, "mullwx");
					break;
				}
				case PPC_SUBOP_266:
				{
					XO_form_D_A_B_OE_Rc(
						base+(pb-buf), *pul, "addx");
					break;
				}
				case PPC_SUBOP_459:
				{
					XO_form_D_A_B_OE_Rc(
						base+(pb-buf), *pul, "divwux");
					break;
				}
				case PPC_SUBOP_491:
				{
					XO_form_D_A_B_OE_Rc(
						base+(pb-buf), *pul, "divwx");
					break;
				}
				default:
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				}
				break;
			}
			}
			break;
		}
		case PPC_OP_32:
		{
			D_form_D_A_d(base+(pb-buf), *pul, "lwz");
			break;
		}
		case PPC_OP_33:
		{
			D_form_D_A_d(base+(pb-buf), *pul, "lwzu");
			break;
		}
		case PPC_OP_34:
		{
			D_form_D_A_d(base+(pb-buf), *pul, "lbz");
			break;
		}
		case PPC_OP_35:
		{
			D_form_D_A_d(base+(pb-buf), *pul, "lbzu");
			break;
		}
		case PPC_OP_36:
		{
			D_form_S_A_d(base+(pb-buf), *pul, "stw");
			break;
		}
		case PPC_OP_37:
		{
			D_form_S_A_d(base+(pb-buf), *pul, "stwu");
			break;
		}
		case PPC_OP_38:
		{
			D_form_S_A_d(base+(pb-buf), *pul, "stb");
			break;
		}
		case PPC_OP_39:
		{
			D_form_S_A_d(base+(pb-buf), *pul, "stbu");
			break;
		}
		case PPC_OP_40:
		{
			D_form_D_A_d(base+(pb-buf), *pul, "lhz");
			break;
		}
		case PPC_OP_41:
		{
			D_form_D_A_d(base+(pb-buf), *pul, "lhzu");
			break;
		}
		case PPC_OP_42:
		{
			D_form_D_A_d(base+(pb-buf), *pul, "lha");
			break;
		}
		case PPC_OP_43:
		{
			D_form_D_A_d(base+(pb-buf), *pul, "lhau");
			break;
		}
		case PPC_OP_44:
		{
			D_form_S_A_d(base+(pb-buf), *pul, "sth");
			break;
		}
		case PPC_OP_45:
		{
			D_form_S_A_d(base+(pb-buf), *pul, "sthu");
			break;
		}
		case PPC_OP_46:
		{
			D_form_D_A_d(base+(pb-buf), *pul, "lmw");
			break;
		}
		case PPC_OP_47:
		{
			D_form_S_A_d(base+(pb-buf), *pul, "stmw");
			break;
		}
		case PPC_OP_48:
		{
			D_form_D_A_d(base+(pb-buf), *pul, "lfs");
			break;
		}
		case PPC_OP_49:
		{
			D_form_D_A_d(base+(pb-buf), *pul, "lfsu");
			break;
		}
		case PPC_OP_50:
		{
			D_form_D_A_d(base+(pb-buf), *pul, "lfd");
			break;
		}
		case PPC_OP_51:
		{
			D_form_D_A_d(base+(pb-buf), *pul, "lfdu");
			break;
		}
		case PPC_OP_52:
		{
			D_form_S_A_d(base+(pb-buf), *pul, "stfs");
			break;
		}
		case PPC_OP_53:
		{
			D_form_S_A_d(base+(pb-buf), *pul, "stfsu");
			break;
		}
		case PPC_OP_54:
		{
			D_form_S_A_d(base+(pb-buf), *pul, "stfd");
			break;
		}
		case PPC_OP_55:
		{
			D_form_S_A_d(base+(pb-buf), *pul, "stfdu");
			break;
		}
		case PPC_OP_56:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_57:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_58:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_59:
		{
			unsigned long subopcode = rbitval(*pul, 26, 30);
			switch (subopcode)
			{
			case PPC_SUBOP_18:
			{
				if (rbitval(*pul, 21, 25) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tfdivsx\tD=%ld A=%ld B=%ld Rc=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 16, 20),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			case PPC_SUBOP_20:
			{
				if (rbitval(*pul, 21, 25) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tfsubsx\tD=%ld A=%ld B=%ld Rc=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 16, 20),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			case PPC_SUBOP_21:
			{
				if (rbitval(*pul, 21, 25) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tfaddsx\tD=%ld A=%ld B=%ld Rc=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 16, 20),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			case PPC_SUBOP_22:
			{
				if (rbitval(*pul, 11, 15) != 0 ||
				    rbitval(*pul, 21, 25) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tfsqrtsx\tD=%ld B=%ld Rc=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 16, 20),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			case PPC_SUBOP_23:
			{
				A_form_D_A_B_C_Rc(
					base+(pb-buf), *pul, "fselx");
				break;
			}
			case PPC_SUBOP_24:
			{
				if (rbitval(*pul, 11, 15) != 0 ||
				    rbitval(*pul, 21, 25) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tfresx\tD=%ld B=%ld Rc=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 16, 20),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			case PPC_SUBOP_25:
			{
				if (rbitval(*pul, 16, 20) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tfmulsx\tD=%ld A=%ld C=%ld Rc=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 21, 25),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			case PPC_SUBOP_26:
			{
				if (rbitval(*pul, 11, 15) != 0 ||
				    rbitval(*pul, 21, 25) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tfrsqrtex\tD=%ld C=%ld Rc=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 16, 20),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			case PPC_SUBOP_28:
			{
				A_form_D_A_B_C_Rc(
					base+(pb-buf), *pul, "fmsubsx");
				break;
			}
			case PPC_SUBOP_29:
			{
				A_form_D_A_B_C_Rc(
					base+(pb-buf), *pul, "fmaddsx");
				break;
			}
			case PPC_SUBOP_30:
			{
				A_form_D_A_B_C_Rc(
					base+(pb-buf), *pul, "fnmsubsx");
				break;
			}
			case PPC_SUBOP_31:
			{
				A_form_D_A_B_C_Rc(
					base+(pb-buf), *pul, "fnmaddsx");
				break;
			}
			default:
			{
				printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
					base+(pb-buf), *pul, opcode, operands);
				break;
			}
			}
			break;
		}
		case PPC_OP_60:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_61:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_62:
		{
			printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
				base+(pb-buf), *pul, opcode, operands);
			break;
		}
		case PPC_OP_63:
		{
			unsigned long subopcode = rbitval(*pul, 21, 30);
			switch (subopcode)
			{
			case PPC_SUBOP_00:
			{
				if (rbitval(*pul, 9, 10) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tfcmpu\tcrfD=%ld A=%ld B=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 8),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 16, 20));
				}
				break;
			}
			case PPC_SUBOP_12:
			{
				X_form_D_0_B_Rc(base+(pb-buf), 
					*pul, "frspx", opcode, operands);
				break;
			}
			case PPC_SUBOP_14:
			{
				X_form_D_0_B_Rc(base+(pb-buf), 
					*pul, "fctiwx", opcode, operands);
				break;
			}
			case PPC_SUBOP_15:
			{
				X_form_D_0_B_Rc(base+(pb-buf), 
					*pul, "fctiwzx", opcode, operands);
				break;
			}
			case PPC_SUBOP_32:
			{
				if (rbitval(*pul, 9, 10) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tfcmpo\tcrfD=%ld A=%ld B=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 8),
						rbitval(*pul, 11, 15),
						rbitval(*pul, 16, 20));
				}
				break;
			}
			case PPC_SUBOP_38:
			{
				if (rbitval(*pul, 11, 20) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmtfsb1x\tcrfD=%ld Rc=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			case PPC_SUBOP_40:
			{
				X_form_D_0_B_Rc(base+(pb-buf), 
					*pul, "fnegx", opcode, operands);
				break;
			}
			case PPC_SUBOP_64:
			{
				if (rbitval(*pul, 9, 10) != 0 ||
				    rbitval(*pul, 14, 20) != 0 ||
				    rbitval(*pul, 31, 31) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmcrfs\tcrfD=%ld crfS=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 8),
						rbitval(*pul, 11, 13));
				}
				break;
			}
			case PPC_SUBOP_70:
			{
				if (rbitval(*pul, 11, 20) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmtfsb0x\tcrbD=%ld Rc=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			case PPC_SUBOP_72:
			{
				X_form_D_0_B_Rc(base+(pb-buf), 
					*pul, "fmrx", opcode, operands);
				break;
			}
			case PPC_SUBOP_134:
			{
				if (rbitval(*pul, 9, 15) != 0 ||
				    rbitval(*pul, 20, 20))
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmtfsix\tcrbD=%ld IMM=%ld Rc=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 8),
						rbitval(*pul, 16, 19),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			case PPC_SUBOP_136:
			{
				X_form_D_0_B_Rc(base+(pb-buf), 
					*pul, "fnabsx", opcode, operands);
				break;
			}
			case PPC_SUBOP_264:
			{
				X_form_D_0_B_Rc(base+(pb-buf), 
					*pul, "fabsx", opcode, operands);
				break;
			}
			case PPC_SUBOP_583:
			{
				if (rbitval(*pul, 11, 20) != 0)
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
				}
				else
				{
					printf("%08lx: %08lx\tmffsx\tD=%ld Rc=%ld\n",
						base+(pb-buf), *pul, 
						rbitval(*pul, 6, 10),
						rbitval(*pul, 31, 31));
				}
				break;
			}
			default:
			{
				subopcode = rbitval(*pul, 26, 30);
				switch (subopcode)
				{
				case PPC_SUBOP_18:
				{
					if (rbitval(*pul, 21, 25) != 0)
					{
						printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
							base+(pb-buf), *pul, opcode, operands);
					}
					else
					{
						printf("%08lx: %08lx\tfdivx\tD=%ld A=%ld B=%ld Rc=%ld\n",
							base+(pb-buf), *pul, 
							rbitval(*pul, 6, 10),
							rbitval(*pul, 11, 15),
							rbitval(*pul, 16, 20),
							rbitval(*pul, 31, 31));
					}
					break;
				}
				case PPC_SUBOP_20:
				{
					if (rbitval(*pul, 21, 25) != 0)
					{
						printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
							base+(pb-buf), *pul, opcode, operands);
					}
					else
					{
						printf("%08lx: %08lx\tfsubx\tD=%ld A=%ld B=%ld Rc=%ld\n",
							base+(pb-buf), *pul, 
							rbitval(*pul, 6, 10),
							rbitval(*pul, 11, 15),
							rbitval(*pul, 16, 20),
							rbitval(*pul, 31, 31));
					}
					break;
				}
				case PPC_SUBOP_21:
				{
					if (rbitval(*pul, 21, 25) != 0)
					{
						printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
							base+(pb-buf), *pul, opcode, operands);
					}
					else
					{
						printf("%08lx: %08lx\tfaddx\tD=%ld A=%ld B=%ld Rc=%ld\n",
							base+(pb-buf), *pul, 
							rbitval(*pul, 6, 10),
							rbitval(*pul, 11, 15),
							rbitval(*pul, 16, 20),
							rbitval(*pul, 31, 31));
					}
					break;
				}
				case PPC_SUBOP_22:
				{
					if (rbitval(*pul, 11, 15) != 0 ||
					    rbitval(*pul, 21, 25) != 0)
					{
						printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
							base+(pb-buf), *pul, opcode, operands);
					}
					else
					{
						printf("%08lx: %08lx\tfsqrtx\tD=%ld B=%ld Rc=%ld\n",
							base+(pb-buf), *pul, 
							rbitval(*pul, 6, 10),
							rbitval(*pul, 16, 20),
							rbitval(*pul, 31, 31));
					}
					break;
				}
				case PPC_SUBOP_23:
				{
					A_form_D_A_B_C_Rc(
						base+(pb-buf), *pul, "fselx");
					break;
				}
				case PPC_SUBOP_25:
				{
					if (rbitval(*pul, 16, 20) != 0)
					{
						printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
							base+(pb-buf), *pul, opcode, operands);
					}
					else
					{
						printf("%08lx: %08lx\tfmulx\tD=%ld A=%ld C=%ld Rc=%ld\n",
							base+(pb-buf), *pul, 
							rbitval(*pul, 6, 10),
							rbitval(*pul, 11, 15),
							rbitval(*pul, 21, 25),
							rbitval(*pul, 31, 31));
					}
					break;
				}
				case PPC_SUBOP_26:
				{
					if (rbitval(*pul, 11, 15) != 0 ||
					    rbitval(*pul, 21, 25) != 0)
					{
						printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
							base+(pb-buf), *pul, opcode, operands);
					}
					else
					{
						printf("%08lx: %08lx\tfrsqrtex\tD=%ld C=%ld Rc=%ld\n",
							base+(pb-buf), *pul, 
							rbitval(*pul, 6, 10),
							rbitval(*pul, 16, 20),
							rbitval(*pul, 31, 31));
					}
					break;
				}
				case PPC_SUBOP_28:
				{
					A_form_D_A_B_C_Rc(
						base+(pb-buf), *pul, "fmsubx");
					break;
				}
				case PPC_SUBOP_29:
				{
					A_form_D_A_B_C_Rc(
						base+(pb-buf), *pul, "fmaddx");
					break;
				}
				case PPC_SUBOP_30:
				{
					A_form_D_A_B_C_Rc(
						base+(pb-buf), *pul, "fnmsubx");
					break;
				}
				case PPC_SUBOP_31:
				{
					A_form_D_A_B_C_Rc(
						base+(pb-buf), *pul, "fnmaddx");
					break;
				}
				default:
				{
					printf("%08lx: %08lx\t%02lx\t0x%lx\n", 
						base+(pb-buf), *pul, opcode, operands);
					break;
				}
				}
			}
			}
			break;
		}
		}
		pb+=sizeof(long);
	}
}

// disassembler 
static void
pdasm(ElfObject &eo, long addr, long size)
{
	int addrseg = -1;

	// find segment that contains the address
	for (int seg=0; seg < eo.pehdr()->e_phnum; seg++)
	{
		if (eo.pphdr(seg).p_vaddr <= addr &&
		    addr < (eo.pphdr(seg).p_vaddr+eo.pphdr(seg).p_memsz))
		{
			addrseg = seg;
			break;
		}
	}
	if (addrseg == -1)
	{
		printf("address NOT found.\n");
		return;
	}

	// verify the section is SPARC or PPC and executable text
	if (eo.pehdr()->e_machine != EM_SPARC && eo.pehdr()->e_machine != EM_PPC)
	{
		printf("Not a SPARC or PPC file; disassembler is only for SPARC and PPC.\n");
		return;
	}
	if ((eo.pphdr(addrseg).p_flags & PF_X) == 0)
	{
		printf("segment is not executable text.\n");
		return;
	}


	// read in segment for disassembly
	long numbytes = eo.pphdr(addrseg).p_filesz;
	unsigned char *p = new unsigned char [numbytes];
	MustBeTrue(p != NULL);
	MustBeTrue(lseek(eo.efd(), eo.pphdr(addrseg).p_offset, 
			SEEK_SET) != NOTOK)
	MustBeTrue(read(eo.efd(), p, numbytes) == numbytes);

	// disassemble
	long start = addr-eo.pphdr(addrseg).p_vaddr;
	if (size > eo.pphdr(addrseg).p_filesz-start)
	{
		size = eo.pphdr(addrseg).p_filesz-start;
	}
	switch (eo.pehdr()->e_machine)
	{
	case EM_SPARC:
		sparc_dasm(addr, size, p+start);
		break;
	case EM_PPC:
		ppc_dasm(addr, size, p+start);
		break;
	default:
		printf("Not a SPARC or PPC file; disassembler is only for SPARC and PPC.\n");
		return;
	}

	// release text buffer
	delete [] p;
	return;
}

static void
sdasm(ElfObject &eo, long offset, long size)
{
	int offsetsec = -1;

	// find section containing the offset
	for (int sec=0; sec < eo.pehdr()->e_shnum; sec++)
	{
		if (eo.pshdr(sec).sh_offset <= offset && 
		    offset < (eo.pshdr(sec).sh_offset+eo.pshdr(sec).sh_size))
		{
			offsetsec = sec;
			break;
		}
	}
	if (offsetsec == -1)
	{
		printf("offset not found.\n");
		return;
	}

	// verify the section is SPARC or PPC and executable text
	if (eo.pehdr()->e_machine != EM_SPARC && eo.pehdr()->e_machine != EM_PPC)
	{
		printf("Not a SPARC or PPC file; disassembler is only for SPARC and PPC.\n");
		return;
	}
	if (eo.pshdr(offsetsec).sh_type != SHT_PROGBITS)
	{
		printf("section is not executable text.\n");
		return;
	}

	// read in section for disassembly
	long numbytes = eo.pshdr(offsetsec).sh_size;
	unsigned char *p = new unsigned char [numbytes];
	MustBeTrue(p != NULL);
	MustBeTrue(lseek(eo.efd(), eo.pshdr(offsetsec).sh_offset, 
			SEEK_SET) != NOTOK)
	MustBeTrue(read(eo.efd(), p, numbytes) == numbytes);

	// disassemble
	long start = offset - eo.pshdr(offsetsec).sh_offset;
	if (size > eo.pshdr(offsetsec).sh_size-start)
	{
		size = eo.pshdr(offsetsec).sh_size-start;
	}
	printf("section %d: %s (%ld, %ld)\n", offsetsec, 
		eo.shdrnm(offsetsec), eo.pshdr(offsetsec).sh_offset, 
		eo.pshdr(offsetsec).sh_size);
	switch (eo.pehdr()->e_machine)
	{
	case EM_SPARC:
		sparc_dasm(offset, size, p+start);
		break;
	case EM_PPC:
		ppc_dasm(offset, size, p+start);
		break;
	default:
		printf("Not a SPARC or PPC file; disassembler is only for SPARC and PPC.\n");
		return;
	}

	// release text buffer
	delete [] p;
	return;
}

static void
printmenu()
{
	printf("\ndisassembler menu:\n\n");
	printf("? or h - show menu\n");
	printf("s - show section header data\n");
	printf("p - show program header data\n");
	printf("lf - list all functions in this file\n");
	printf("lf * - list all functions in this file\n");
	printf("lf <symbol> - list <symbol> functions in this file\n");
	printf("d <symbol> - disassembler\n");
	printf("d <symbol> <reloffset> <size> - disassembler\n");
	printf("sd <offset> <size> - disassembler (section headers)\n");
	printf("sd <section> <reloffset> <size> - disassembler (section headers)\n");
	printf("sd <section#> <reloffset> <size> - disassembler (section headers)\n");
	printf("pd <address> <size> - disassembler (program headers)\n");
	printf("D - toggle C++ demangle mode\n");
	printf("P <lines) - max lines per page, default of 0 means NO paging.\n");
	printf("q - quit\n\n");
	return;
}

void
editdasm(ElfObject &eo, char *)
{
	char s[BUFSIZ];

	// start of disassembler
	printf("disassembler:\n");

	// start interactive loop
	for (int done=0; !done; )
	{
		// get cmd from user
		printf("dasm cmd: ");
		rmvnlgets(s);
		tokenize(s, " \t");
		char *pt = gettoken(1);

		// what is the command
		if (pt == NULL)
		{
			printf("unknown cmd.\n");
		}
		else if (*pt == '?' || *pt == 'h')
		{
			printmenu();
		}
		else if (strcmp(pt, "D") == 0)
		{
			dflag = !dflag;
			if (dflag)
				printf("demangle mode ON.\n");
			else
				printf("demangle mode OFF.\n");
		}
		else if (strcmp(pt, "lf") == 0)
		{
			// get function name, if any
			char *pfcn = gettoken(2);

			// scan for functions
			Elf32_Sym *psym = NULL;
			for (int csec=0; csec<eo.pehdr()->e_shnum;csec++)
			{
                                // skip sections that are not symbol tables
				if (eo.pshdr(csec).sh_type != SHT_SYMTAB &&
				    eo.pshdr(csec).sh_type != SHT_DYNSYM)
					continue;
 
				// search the section for the symbol
				char *p0 = eo.pshdrdata(csec);
				char *pe = eo.pshdrdata(csec)+
						eo.pshdr(csec).sh_size;
				char *pstr = 
					eo.pshdrdata(eo.pshdr(csec).sh_link);
				for (int csymno=0;
				    (p0+csymno*eo.pshdr(csec).sh_entsize)<pe;
				     csymno++)
				{
					Elf32_Sym *psym = (Elf32_Sym *)(p0 +
						csymno*eo.pshdr(csec).sh_entsize);
					char *psymnm = pstr + psym->st_name;
					if (psym->st_shndx == SHN_UNDEF ||
					    ELF32_ST_TYPE(psym->st_info) != STT_FUNC)
					{
						// skip entries that are
						// not functions and
						// not in this file.
						//
						continue;
					}
					if (pfcn != NULL && *pfcn != '\0' &&
					    *pfcn != '*' &&
					    !REequal(pfcn, mydemangle(psymnm)))
						continue;
					showsymbolname(eo, csymno, csec, p0, pe);
				}
			}
		}
		else if (strcmp(pt, "s") == 0)
		{
			// show section data
			printf("section data:\n");
			for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
			{
				printf("section %d: %s (%ld, %ld)\n", sec, 
					eo.shdrnm(sec), 
					eo.pshdr(sec).sh_offset, 
					eo.pshdr(sec).sh_size);
			}
		}
		else if (strcmp(pt, "p") == 0)
		{
			// show section data
			printf("program data:\n");
			for (int seg=0; seg<eo.pehdr()->e_phnum; seg++)
			{
				printf("segment %d: (0x%lx, 0x%lx, %ld)\n", 
					seg, eo.pphdr(seg).p_vaddr, 
					eo.pphdr(seg).p_paddr,
					eo.pphdr(seg).p_memsz);
			}
		}
		else if (strcmp(pt, "P") == 0)
		{
			// get number of lines
			char *pt2 = gettoken(2);
			if (pt2 == NULL || *pt2 == '\0')
			{
				printf("no lines given; setting maxlines=0.\n");
				maxlinestoprint = 0;
			}
			else
			{
				maxlinestoprint = MYatoi(pt2);
			}
			printf("maxlines = %ld\n", maxlinestoprint);
		}
		else if (strcmp(pt, "pd") == 0)
		{
			// get address
			char *pt2 = gettoken(2);
			char *pt3 = gettoken(3);
			MustBeTrue(pt2 != NULL && *pt2 != '\0');
			MustBeTrue(pt3 != NULL && *pt3 != '\0');
			long addr = MYatoi(pt2);
			long size = MYatoi(pt3);
			pdasm(eo, addr, size);
		}
		else if (strcmp(pt, "sd") == 0)
		{
			// get tokens 
			char *pt2 = gettoken(2);
			char *pt3 = gettoken(3);
			char *pt4 = gettoken(4);

			// check if a section was given 
			if (pt2 != NULL && pt3 != NULL && pt4 != NULL)
			{
				long offset = 0;
				long size = 0;

				// get section
				if (strspn(pt2, "0123456789") == strlen(pt2))
				{
					// section number was given
					int csec = MYatoi(pt2);
					if (csec < 0 || csec >= eo.pehdr()->e_shnum)
					{
						printf("out-of-range section number.\n");
						printf("minimum section number = 0\n");
						printf("maximum section number = %d\n",
							eo.pehdr()->e_shnum);
						return;
					}
					offset = eo.pshdr(csec).sh_offset;
				}
				else
				{
					// section name was given
					int csec = nametosec(eo, pt2);
					if (csec == NOTOK)
					{
						printf("section not found.\n");
						return;
					}
					offset = eo.pshdr(csec).sh_offset;
				}

				// add relative offset to section offset
				// and get bytes to dasm
				MustBeTrue(pt3 != NULL && *pt3 != '\0');
				offset += MYatoi(pt3);
				MustBeTrue(pt4 != NULL && *pt4 != '\0');
				size = MYatoi(pt4);

				// dasm it
				sdasm(eo, offset, size);
			}
			else if (pt2 != NULL && pt3 != NULL && pt4 == NULL)
			{
				// no section was given, get offset and size
				MustBeTrue(pt2 != NULL && *pt2 != '\0');
				long offset = MYatoi(pt2);
				MustBeTrue(pt3 != NULL && *pt3 != '\0');
				long size = MYatoi(pt3);

				// dasm it
				sdasm(eo, offset, size);
			}
			else
				MustBeTrue(pt2 != NULL || pt3 != NULL);
			
		}
		else if (strcmp(pt, "d") == 0)
		{
			// get tokens 
			char *ptsym = gettoken(2);
			char *preloff = gettoken(3);
			char *psize = gettoken(4);

			// offsets, etc.
			long reloffset = -1;
			long size = -1;
			long offset = -1;

			// sanity check
			if (ptsym == NULL)
			{
				printf("symbol is missing.\n");
				continue;
			}
			else if (preloff == NULL && psize == NULL)
			{
				reloffset = 0;
				offset = 0;
			}
			else if (preloff != NULL && psize != NULL)
			{
				reloffset = MYatoi(preloff);
				size = MYatoi(psize);
				offset = 0;
			}
			else
			{
				printf("reloffset or size is missing.\n");
				continue;
			}

			// get symbol data
			int csec;
			int found = 0;
			Elf32_Sym *psym = NULL;
			for (csec=0; csec<eo.pehdr()->e_shnum;csec++)
			{
                                // skip sections that are not symbol tables
				if (eo.pshdr(csec).sh_type != SHT_SYMTAB &&
				    eo.pshdr(csec).sh_type != SHT_DYNSYM)
					continue;
 
				// search the section for the symbol
				char *p0 = eo.pshdrdata(csec);
				char *pe = eo.pshdrdata(csec)+
						eo.pshdr(csec).sh_size;
				char *pstr = 
					eo.pshdrdata(eo.pshdr(csec).sh_link);
				for (int csymno=0;
				    (p0+csymno*eo.pshdr(csec).sh_entsize)<pe;
				     csymno++)
				{
					psym = (Elf32_Sym *)(p0 +
						csymno*eo.pshdr(csec).sh_entsize);
					char *psymnm = pstr + psym->st_name;
					if (!REequal(ptsym, mydemangle(psymnm)))
						continue;
					if (psym->st_shndx == SHN_UNDEF ||
					    ELF32_ST_TYPE(psym->st_info) != STT_FUNC)
					{
						// skip entries that are
						// not functions and
						// not in this file.
						//
						continue;
					}
					showsymbol(eo, csymno, csec, p0, pe);
					char s[BUFSIZ];
					printf("use this symbol? [n/y/cr=n] ");
					rmvnlgets(s);
					if (*s == 'y')
					{
						found = 1;
						goto all_done;
					}
				}
			}
			all_done: ;

			// was the corrent symbol found?
			if (!found)
			{
				printf("symbol not found.\n");
				continue;
			}

			// do program headers exit?
			if (eo.pehdr()->e_phoff != 0)
			{
				// program headers exist, offsets
				// are virtual addresses
				long addr = psym->st_value + reloffset;
				if (size < 0) 
					size = psym->st_size;
				pdasm(eo, addr, size);
			}
			else
			{
				// no program headers, offsets are
				// file offsets.
				offset = eo.pshdr(psym->st_shndx).sh_offset;
				offset += psym->st_value + reloffset;
				if (size < 0) 
					size = psym->st_size;
				sdasm(eo, offset, size);
			}

		}
		else if (*pt == 'q')
		{
			done = 1;
		}
		else
		{
			printf("unknown cmd.\n");
		}
	}
	return;
}

