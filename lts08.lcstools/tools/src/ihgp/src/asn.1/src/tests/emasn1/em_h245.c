/* $CCRev: ./src/h323ds/3rdparty/elemedia/ex3230s/asn1/emasn1/em_h245.c@@/main/1 */
/************************************************************************
 *
 *  Copyright (c) 2000 Lucent Technologies.
 *  All rights reserved.
 *
 *  ALL RIGHTS ARE RESERVED BY LUCENT TECHNOLOGIES.  ACCESS TO THIS
 *  SOURCE CODE IS STRICTLY RESTRICTED UNDER CONTRACT. THIS CODE IS TO
 *  BE KEPT STRICTLY CONFIDENTIAL.
 *
 *  UNAUTHORIZED MODIFICATIONS OF THIS FILE WILL VOID YOUR SUPPORT
 *  CONTRACT WITH LUCENT TECHNOLOGIES.  IF SUCH MODIFICATIONS ARE FOR
 *  THE PURPOSE OF CIRCUMVENTING LICENSING LIMITATIONS, LEGAL ACTION
 *  MAY RESULT.
 *
 ************************************************************************
 *
 *  2000-07-14 06:56pm  saras-2710  smaroulis  /main/1
 *
 ************************************************************************/
/* SCCS id:	@(#)em_h245.c	7.1.2.2	03/14/00	*/

/* 
 *              Copyright (c) 1996-1998 Lucent Technologies
 *                      All Rights Reserved
 *
 *                      THIS IS UNPUBLISHED
 *                      PROPRIETARY SOURCE
 *                  CODE OF Lucent Technologies
 *
 *          The copyright notice above does not evidence any
 *         actual or intended publication of such source code
 *
 *
 * Filename		:	em_h245.c
 *
 * Revision History
 * Date			Revision description
 * 06/08/1998	File Created
 * 06/17/1998   Changed h245.c to em_h245.c.
 * 03/29/1999	Included PSOS simulator support
 */


#ifdef sun5
#include "../../os/asn1/sun5/em_h245.c"
#endif

#ifdef uw
#include "../../os/asn1/uw/em_h245.c"
#endif

#ifdef hpux
#include "../../os/asn1/hpux/em_h245.c"
#endif

#ifdef win32
#include "../../os/asn1/win32/em_h245.c"
#endif

#ifdef inferno
#if (CPU == 386)
#include "../../os/asn1/inferno/386/em_h245.c"
#elif (CPU == R4000)
#include "../../os/asn1/inferno/R4000/em_h245.c"
#elif (CPU == R4700)
#include "../../os/asn1/inferno/R4000/em_h245.c"
#else
#error Unknown inferno CPU type
#endif /* CPU == ?? */
#endif /* inferno */

#ifdef comos
#if (CPU == X86)
#include "../../os/asn1/comos/X86/em_h245.c"
#else
#error Unknown comos CPU type
#endif /* CPU == ?? */
#endif /* comos */

#ifdef vxworks
#if (CPU == SIMSPARCSOLARIS)
#include "../../os/asn1/vxworks/SIMSPARCSOLARIS/em_h245.c"
#elif (CPU == SH4)
#include "../../os/asn1/vxworks/SH4/em_h245.c"
#elif (CPU == R4000)
#if (CPU_VAR == R4700)
#ifdef MIPSEB
#include "../../os/asn1/vxworks/R4700EB/em_h245.c"
#else
#include "../../os/asn1/vxworks/R4700EL/em_h245.c"
#endif
#else
#error Unknown MIPS CPU Variant
#endif	/* CPU_VAR == ?? */
#else
#error Unknown CPU type
#endif	/* CPU == ?? */
#endif	/* vxworks */

/*
 * Note that PSOS simulator uses the SUN5 version of the
 * OSS libraries and headers. There is no seperate port for the
 * psos simulator platform.
 */
#ifdef psos
#if defined(PSOSIM)
#include "../os/asn1/sun5/em_h245.c"
#else
#error unsupported PSOS target
#endif
#endif /*psos*/
