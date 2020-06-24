/* $CCRev: ./src/h323ds/3rdparty/elemedia/ex3230s/asn1/emasn1/em_h225anxg.c@@/main/1 */
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
 *  2000-07-14 06:40pm  saras-2710  smaroulis  /main/1
 *
 ************************************************************************/
/* SCCS id: @(#)em_h225anxg.c	7.1.2.2	03/14/00 */
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
 * CVS Information:
 * $Revision: $
 * $Date: $
 *
 * Filename		:	em_h225anxg.c
 *
 * Revision History
 * Date			Revision description
 * 07/30/1999	File Created
 * 09/16/1999	Added WIN32 include
 * 10/26/199	Added Vxworks/uw/Psos
 */

#ifdef sun5
#include "../../os/asn1/sun5/em_h225anxg.c"
#endif

#ifdef win32
#include "../../os/asn1/win32/em_h225anxg.c"
#endif

#ifdef uw
#include "../../os/asn1/uw/em_h225anxg.c"
#endif

#if 0

#ifdef hpux
#include "../../os/asn1/hpux/em_h225anxg.c"
#endif


#ifdef inferno
#if (CPU == 386)
#include "../../os/asn1/inferno/386/em_h225anxg.c"
#elif (CPU == R4000)
#include "../../os/asn1/inferno/R4000/em_h225anxg.c"
#elif (CPU == R4700)
#include "../../os/asn1/inferno/R4000/em_h225anxg.c"
#else
#error Unknown inferno CPU type
#endif /* CPU == ?? */
#endif /* inferno */

#ifdef comos
#if (CPU == X86)
#include "../../os/asn1/comos/X86/em_h225anxg.c"
#else
#error Unknown comos CPU type
#endif /* CPU == ?? */
#endif /* comos */

#endif /*if 0 */

#ifdef vxworks
#if (CPU == SIMSPARCSOLARIS)
#include "../../os/asn1/vxworks/SIMSPARCSOLARIS/em_h225anxg.c"
#elif (CPU == SH4)
#include "../../os/asn1/vxworks/SH4/em_h225anxg.c"
#elif (CPU == R4000)
#if (CPU_VAR == R4700)
#ifdef MIPSEB
#include "../../os/asn1/vxworks/R4700EB/em_h225anxg.c"
#else
#include "../../os/asn1/vxworks/R4700EL/em_h225anxg.c"
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
#include "../os/asn1/sun5/em_h225anxg.c"
#else
#error unsupported PSOS target
#endif
#endif /*psos*/

