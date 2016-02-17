/* $CCRev: ./src/h323ds/3rdparty/elemedia/ex3230s/asn1/emasn1/em_oids.c@@/main/1 */
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
 *  2000-07-14 06:57pm  saras-2710  smaroulis  /main/1
 *
 ************************************************************************/
/* SCCS id:	@(#)em_oids.c	7.1.2.2	10/28/99	*/

/** 
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
 * 	@Filename:		em_oids.c
 *	@Description:	Provides the global object identifiers for h.225 version 1,
 *					h.225 version 2, h.245 version 2, and h.245 version 3.
 *
 * Revision History
 * Date			Revision description
 * 06/25/1998	File Created
 * 08/13/1999   Anxg OID added
 **/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1/em_asn1_common.h"

/**
 *	@Name:			H225_ANXG_V1_OIDPtr
 *
 * 	@Description:	H.225 ANXG version 1 object id 
 * 		{itu-t (0) recommendation (0) h (8) 2250 annex (1) g (7) version (0) 1}
 **/
EM_STATIC unsigned long _h225_anxg_v1_oid[] 
							= { 0L, 0L, 8L, 2250L, 1L, 7L, 0L, 1L };
EM_STATIC EmObjectIdentifier _H225_ANXG_V1_OID =
{
	8,
	_h225_anxg_v1_oid
};
EM_EXPORT EmObjectIdentifier* H225_ANXG_V1_OIDPtr = &_H225_ANXG_V1_OID;

/**
 *	@Name:			H225_V2_OIDPtr
 *
 * 	@Description:	H.225 version 2 object id 
 * 		{itu-t (0) recommendation (0) h (8) 2250 version (0) 2}
 **/
EM_STATIC unsigned long _h225_v2_oid[] = { 0L, 0L, 8L, 2250L, 0L, 2L };
EM_STATIC EmObjectIdentifier _H225_V2_OID =
{
	6,
	_h225_v2_oid
};
EM_EXPORT EmObjectIdentifier* H225_V2_OIDPtr = &_H225_V2_OID;

/**
 *	@Name:			H245_V3_OIDPtr
 *
 *	@Description:	H.245 version 3 object id
 * 		{itu-t (0) recommendation (0) h (8) 245 version (0) 3}
 **/
EM_STATIC unsigned long _h245_v3_oid[] = { 0L, 0L, 8L, 245L, 0L, 3L };

EM_STATIC EmObjectIdentifier _H245_V3_OID =
{
	6,
	_h245_v3_oid
};
EM_EXPORT EmObjectIdentifier* H245_V3_OIDPtr = &_H245_V3_OID;

/**
 *	@Name:			H225_V1_OIDPtr
 *
 *	@Description:	H.225 version 1 object id 
 * 		{itu-t (0) recommendation (0) h (8) 2250 version (0) 1}
 **/
EM_STATIC unsigned long _h225_v1_oid[] = { 0L, 0L, 8L, 2250L, 0L, 1L };

EM_STATIC EmObjectIdentifier _H225_V1_OID =
{
	6,
	_h225_v1_oid
};
EM_EXPORT EmObjectIdentifier* H225_V1_OIDPtr = &_H225_V1_OID;

/**
 *	@Name:			H245_V2_OID
 *
 *	@Description:	H.245 version 2 object id
 * 		{itu-t (0) recommendation (0) h (8) 245 version (0) 2}
 **/
EM_STATIC unsigned long _h245_v2_oid[] = { 0L, 0L, 8L, 245L, 0L, 2L };
EM_STATIC EmObjectIdentifier _H245_V2_OID =
{
	6,
	_h245_v2_oid
};
EM_EXPORT EmObjectIdentifier* H245_V2_OIDPtr = &_H245_V2_OID;
 
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
