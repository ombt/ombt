#ident "@(#)/usr/src/cmd/spell/hashlook.c	5.1.1.1 6/1/90 00:27:08 - (c) Copyright 1988, 1989, 1990 Amdahl Corporation"
/* Copyright (c) 1984 AT&T */
/*	@(#)hashlook.c	1.4	*/
#include <stdio.h>
#include "hash.h"
#include "huff.h"

unsigned *table;
int index[NI];

#define B (BYTE*sizeof(unsigned))
#define L (BYTE*sizeof(long)-1)
#define MASK (~(1L<<L))

#ifdef pdp11	/*sizeof(unsigned)==sizeof(long)/2 */
#define fetch(wp,bp)\
	(((((long)wp[0]<<B)|wp[1])<<(B-bp))|(wp[2]>>bp))
#else 		/*sizeof(unsigned)==sizeof(long)*/
#define fetch(wp,bp) ((wp[0]<<(B-bp))|(wp[1]>>bp))
#endif

hashlook(s)
char *s;
{
	unsigned long h;
	long t;
	register bp;
	register unsigned *wp;
      /*int i;*/
	long sum;
	unsigned *tp;

	h = hash(s);
	t = h>>(HASHWIDTH-INDEXWIDTH);
	wp = &table[index[t]];
	tp = &table[index[t+1]];
	bp = B;
	sum = (long)t<<(HASHWIDTH-INDEXWIDTH);
	for(;;) {
		{/*	this block is equivalent to
			 bp -= decode((fetch(wp,bp)>>1)&MASK, &t);*/
			long y;
			long v;
#if u3b || u3b15	|| u3b2 /* shift 32 on 3b leaves destination unchanged */
			if (bp == 0)
				y = 0;
			else
				y = wp[0] << (B - bp);
			if (bp < 32)
				y |= (wp[1] >> bp);
			y = (y >> 1) & MASK;
#else
			y = (fetch(wp,bp)>>1) & MASK;
#endif
			if(y < cs) {
				t = y >> (L+1-w);
				bp -= w-1;
			}
			else {
				for(bp-=w,v=v0; y>=qcs; y=(y<<1)&MASK,v+=n)
					bp -= 1;
				t = v + (y>>(L-w));
			}
		}
		while(bp<=0) {
			bp += B;
			wp++;
		}
		if(wp>=tp&&(wp>tp||bp<B))
			return(0);
		sum += t;
		if(sum<h)
			continue;
		return(sum==h);
	}
}


prime(argc,argv)
char **argv;
{
	register FILE *f;
#ifdef pdp11
	register fd;
#endif
	extern char *malloc();
	if(argc <= 1)
		return(0);
#ifdef pdp11	/* because of insufficient address space for buffers*/
	fd = dup(0);
	close(0);
	if(open(argv[1], 0) != 0)
		return(0);
	f = stdin;
	if(rhuff(f)==0
	|| read(fileno(f), (char *)index, NI*sizeof(*index)) != NI*sizeof(*index)
	|| (table = (unsigned*)malloc(index[NI-1]*sizeof(*table))) == 0
	|| read(fileno(f), (char*)table, sizeof(*table)*index[NI-1])
	   != index[NI-1]*sizeof(*table))
		return(0);
	close(0);
	if(dup(fd) != 0)
		return(0);
	close(fd);
#else
	if((f = fopen(argv[1], "r")) == NULL)
		return(0);
	if(rhuff(f)==0
	|| fread((char*)index, sizeof(*index),  NI, f) != NI
	|| (table = (unsigned*)malloc(index[NI-1]*sizeof(*table))) == 0
	|| fread((char*)table, sizeof(*table), index[NI-1], f)
	   != index[NI-1])
		return(0);
	fclose(f);
#endif
	hashinit();
	return(1);
}
