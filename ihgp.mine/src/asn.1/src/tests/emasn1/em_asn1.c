/* $CCRev: ./src/h323ds/3rdparty/elemedia/ex3230s/asn1/emasn1/em_asn1.c@@/main/3 */
/************************************************************************
 *
 *  Copyright (c) 2000,2001 Lucent Technologies.
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
 *  2001-02-21 03:42pm  saras-3749  ragh      /main/3
 *  2000-10-31 02:16pm  saras-3326  ragh      /main/2
 *  2000-07-14 06:55pm  saras-2710  smaroulis  /main/1
 *
 ************************************************************************/
/* SCCS id:	@(#)em_asn1.c	7.1.2.2	10/28/99	*/

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
 * @Filename:		em_asn1.c
 * @Descrition:		This library provides an abstract interface to and a 
 *					consistent method of using the underlying ASN.1
 *					encoder/decoder libraries. As well, this library
 *					provides added logging support consistent with
 *					the h323 stack architecture.
 *
 * Revision History
 * Date			Revision description
 * 06/12/1998	File Created
 * 08/13/1999	Anxg Contents added
 **/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1code.h"

#include "em_errors.h"
#include "em_errmacros.h"
#include "em_logger.h"
#include "em_logmacros.h"
#include "em_stamp.h"
#include "em_usvcs_int.h"
#include "em_assert.h"

#include "asn1/em_h225.h"
#include "asn1/em_h225anxg.h"
#include "asn1/em_h245.h"
#include "asn1/em_h450.h"

#include "asn1/em_asn1.h"

/**
 *	@Name: 			S_Initialized
 *
 *	@Description:	Indicates if the library has the library been initialized.
 *
 *	@Private
 **/
EM_STATIC int S_Initialized = 0;

/**
 *	@Name:			S_EmASN1ContextSize
 *	
 *	@Description:	Holds the size of the EmASN1Context structure. Set 
 *					during library initialization.
 *	@Private
 **/
EM_STATIC int S_EmASN1ContextSize = 0;

/**
 *	@Name: 			EmASN1PDUTableEntry
 *
 *	@Description:	Each entry hold asn.1 protocol information.
 *
 *	@Private
 **/
typedef struct EmASN1PDUTableEntry
{
	/**
	 *	@Name:			pdu
	 *	@Description:	Holds the pdu number necessary for encoding and decoding
	 *					the protocol.
	 **/
	int			pdu;
	
	/**
	 *	@Name:			control_table
	 *	@Description:	Holds a pointer to the structure holding the protocol's
	 *					control structure. This structure holds things like the
	 *					protocol constraint table.
	 **/
	void		* control_table;
	
	/**
	 *	@Name:			name
	 *	@Description:	Name of the protocol for logging purposes.
	 **/
	const char 	* name;

	/**
	 *	@Name:			master_world
	 *	@Description:	Master copy of ossGlobal for this protocol.
	 **/
	OssGlobal master_world;

} EmASN1PDUTableEntry;

/**
 *	@Name:			asn1PDUInfo
 *	
 *	@Description:	The static table of protocols.
 *
 *	@Private
 **/
EM_STATIC EmASN1PDUTableEntry	asn1PDUInfo[EM_ASN1_PDUTYPE_MAX+1] =
{
	{	0,
		0,
		"EM_ASN1_PDUTYPE_INVALID"
	},
	{	EM_H323_UserInformation_PDU,
		0, /* em_h225 */
		"EM_ASN1_PDUTYPE_H225_Q931"
	},
	{	EM_RasMessage_PDU,
		0, /* em_h225 */
		"EM_ASN1_PDUTYPE_H225_RAS"
	},
	{	EM_ClearToken_PDU,
		0, /* em_h225 */
		"EM_ASN1_PDUTYPE_H225_CLEARTOKEN"
	},
	{	EM_MSCM_PDU,
		0, /* em_h245 */
		"EM_ASN1_PDUTYPE_H245"
	},
	{	EM_OpenLogicalChannel_PDU,
		0, /* em_h245 */
		"EM_ASN1_PDUTYPE_H245_OLC"
	},
	{	EM_H4501SupplementaryService_PDU,
		0,
		"EM_ASN1_PDUTYPE_H450"
	},
	{	EM_CallTransferExtension_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4502_CT_EXTENSION"
	},
	{	EM_CTInitiateArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4502_CT_INITIATE_ARG"
	},
	{	EM_CTSetupArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4502_CT_SETUP_ARG"
	},
	{	EM_CTIdentifyRes_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4502_CT_IDENTIFY_RES"
	},
	{	EM_CTUpdateArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4502_CT_UPDATE_ARG"
	},
	{	EM_SubaddressTransferArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4502_SUBADDRESS_TRANSFER_ARG"
	},
	{	EM_CTCompleteArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4502_CT_COMPLETE_ARG"
	},
	{	EM_CTActiveArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4502_CT_ACTIVE_ARG"
	},
	{	EM_CallTransferUnspecifiedParameter_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4502_CT_UNSPECIFIED_PARM"
	},
	{	EM_CallDiversionExtension_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4503_CD_EXTENSION"
	},
	{	EM_ActivateDiversionQArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4503_ACTIVATE_DIVERSION_Q_ARG"
	},
	{	EM_DeactivateDiversionQArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4503_DEACTIVATE_DIVERSION_Q_ARG"
	},
	{	EM_InterrogateDiversionQArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4503_INTERROGATE_DIVERSION_Q_ARG"
	},
	{	EM_CheckRestrictionArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4503_CHECK_RESTRICTION_ARG"
	},
	{	EM_CallReroutingArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4503_CALL_REROUTING_ARG"
	},
	{	EM_DivertingLegInformation1Arg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4503_DIVERTINGLEG_INFORMATION1_ARG"
	},
	{	EM_DivertingLegInformation2Arg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4503_DIVERTINGLEG_INFORMATION2_ARG"
	},
	{	EM_DivertingLegInformation3Arg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4503_DIVERTINGLEG_INFORMATION3_ARG"
	},
	{	EM_DivertingLegInformation4Arg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4503_DIVERTINGLEG_INFORMATION4_ARG"
	},
	{	EM_IntResultList_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4503_INT_RESULT_LIST"
	},
	{	EM_CallDiversionUnspecifiedParameter_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4503_CD_UNSPECIFIED_PARM"
	},
	{	EM_HoldNotificArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4504_REMOTE_HOLD_ARG"
	},
	{	EM_RetrieveNotificArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4504_REMOTE_HOLD_ARG"
	},
	{	EM_RemoteHoldArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4504_REMOTE_HOLD_ARG"
	},
	{	EM_RemoteHoldRes_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4504_REMOTE_HOLD_RES"
	},
	{	EM_RemoteRetrieveArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4504_REMOTE_RETRIEVE_ARG"
	},
	{	EM_RemoteRetrieveRes_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4504_REMOTE_RETRIEVE_RES"
	},
	{	EM_MixedExtensionSeq_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4504_MIXED_EXTENSION_SEQ"
	},
	{	EM_CpRequestArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_CP_REQUEST_ARG"
	},		
	{	EM_CpRequestRes_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_CP_REQUEST_RES"
	},
	{	EM_CpSetupArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_CP_SETUP_ARG"
	},		
	{	EM_CpSetupRes_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_CP_SETUP_RES"
	},		
	{	EM_GroupIndicationOnArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_GROUP_INDICATION_ON_ARG"
	},		
	{	EM_GroupIndicationOnRes_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_GROUP_INDICATION_ON_RES"
	},		
	{	EM_GroupIndicationOffArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_GROUP_INDICATION_OFF_ARG"
	},		
	{	EM_GroupIndicationOffRes_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_GROUP_INDICATION_OFF_RES"
	},		
	{	EM_PickrequArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_PICK_REQU_ARG"
	},		
	{	EM_PickrequRes_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_PICK_REQU_RES"
	},		
	{	EM_PickupArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_PICKUP_ARG"
	},		
	{	EM_PickupRes_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_PICKUP_RES"
	},		
	{	EM_PickExeArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_PICK_EXE_ARG"
	},		
	{	EM_PickExeRes_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_PICK_EXE_RES"
	},		
	{	EM_CpNotifyArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_CP_NOTIFY_ARG"
	},		
	{	EM_CpickupNotifyArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4505_CPICKUP_NOTIFY_ARG"
	},		
	{	EM_CallWaitingArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4506_CALL_WAITING_ARG"
	},
	{	EM_MWIActivateArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4507_MWI_ACTIVATE_ARG"
	},
	{	EM_MWIDeactivateArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4507_MWI_DEACTIVATE_ARG"
	},
	{	EM_MWIInterrogateArg_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4507_MWI_INTERROGATE_ARG"
	},
	{	EM_MWIInterrogateRes_PDU,
		0,
		"EM_ASN1_PDUTYPE_H4507_MWI_INTERROGATE_RES"
	},
	{	EM_AnxgMessage_PDU,
		0, /* em_h225anxg */
		"EM_ASN1_PDUTYPE_H225_ANXG"
	}

};

/**
 *	@Name: 			EmASN1InitLibrary
 *
 * 	@Description: 	Initializes the ASN.1 library.
 **/
void EmASN1InitLibrary(void)
{
	/* Set static variables. */
	S_Initialized = 1;
	S_EmASN1ContextSize = sizeof(EmASN1Context);
	
	/* Setup PDU table */
	asn1PDUInfo[EM_ASN1_PDUTYPE_H225_Q931].control_table 	= em_h225;
	asn1PDUInfo[EM_ASN1_PDUTYPE_H225_RAS].control_table 	= em_h225;
	asn1PDUInfo[EM_ASN1_PDUTYPE_H225_CLEARTOKEN].control_table 	= em_h225;
	asn1PDUInfo[EM_ASN1_PDUTYPE_H245].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H245_OLC].control_table 	= em_h245;
	asn1PDUInfo[EM_ASN1_PDUTYPE_H450].control_table 		
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4502_CT_EXTENSION].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4502_CT_INITIATE_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4502_CT_SETUP_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4502_CT_IDENTIFY_RES].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4502_CT_UPDATE_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4502_SUBADDRESS_TRANSFER_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4502_CT_COMPLETE_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4502_CT_ACTIVE_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4502_CT_UNSPECIFIED_PARM].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4503_CD_EXTENSION].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4503_ACTIVATE_DIVERSION_Q_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4503_DEACTIVATE_DIVERSION_Q_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4503_INTERROGATE_DIVERSION_Q_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4503_CHECK_RESTRICTION_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4503_CALL_REROUTING_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4503_DIVERTING_LEG_INFORMATION_1_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4503_DIVERTING_LEG_INFORMATION_2_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4503_DIVERTING_LEG_INFORMATION_3_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4503_DIVERTING_LEG_INFORMATION_4_ARG].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4503_INT_RESULT_LIST].control_table 
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4503_CD_UNSPECIFIED_PARM].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4504_HOLD_NOTIFIC_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4504_RETRIEVE_NOTIFIC_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4504_REMOTE_HOLD_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4504_REMOTE_HOLD_RES].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4504_REMOTE_RETRIEVE_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4504_REMOTE_RETRIEVE_RES].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4504_MIXED_EXTENSION_SEQ].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_CP_REQUEST_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_CP_REQUEST_RES].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_CP_SETUP_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_CP_SETUP_RES].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_GROUP_INDICATION_ON_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_GROUP_INDICATION_ON_RES].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_GROUP_INDICATION_OFF_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_GROUP_INDICATION_OFF_RES].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_PICK_REQU_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_PICK_REQU_RES].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_PICKUP_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_PICKUP_RES].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_PICK_EXE_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_PICK_EXE_RES].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_CP_NOTIFY_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4505_CPICKUP_NOTIFY_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4506_CALL_WAITING_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4507_MWI_ACTIVATE_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4507_MWI_DEACTIVATE_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4507_MWI_INTERROGATE_ARG].control_table
		= asn1PDUInfo[EM_ASN1_PDUTYPE_H4507_MWI_INTERROGATE_RES].control_table
		= em_h450;
	asn1PDUInfo[EM_ASN1_PDUTYPE_H225_ANXG].control_table 	= em_h225anxg;


	/*
	 * ossinit master copy of OssGlobals
	 */
	{
		int nPDUs = sizeof(asn1PDUInfo) / sizeof(EmASN1PDUTableEntry);
		int i;
		int rc;

		for (i=1;i<nPDUs;++i)
		{
#ifndef win32
    		rc = ossinit(&asn1PDUInfo[i].master_world,
					asn1PDUInfo[i].control_table);
#else
    		rc = ossWinit(&asn1PDUInfo[i].master_world,
					asn1PDUInfo[i].control_table,
					0,	/* if non-zero, ctl_tbl compiled as DLL */
					0);	/* hWnd: 0 -> tracing window !exist */
#endif
			if ( rc != 0 )
			{
				return;
			}

		}
	}
}

/**
 *	@Name: 			EmASN1InitContext
 *
 *	@Description: 	Initializes the EmASN1Context passed in by contextPtr.
 *
 * 	@Caveats: 		The encoding flags default to 
 *				  	NOCONSTRAIN | NOTRAPPING | FRONT_ALIGN.
 *				  	The decoding flags default to FRONT_ALIGN.
 **/
ProtReturnCode EmASN1InitContext(
	EmASN1Context			*	contextPtr, 
	EmAsn1PDUType				pdu_type)
{
	const char	fn[] = "EmASN1InitContext";
	int rc = 0;
	
	/* Assume failure.
	 */
	contextPtr->pduType = 0;
	
	/* Log function entry 
	 */
	LOG(ASN1_FNENT(contextPtr->logId),
		"%s(0x%x, %d)\n", 
		fn,	(unsigned)contextPtr, pdu_type);
	
	if ( pdu_type > EM_ASN1_PDUTYPE_MAX )
	{
		pdu_type = EM_ASN1_PDUTYPE_INVALID;
	}
	LOG(ASN1_INFO(contextPtr->logId), 
		"%s: PDU type:%s\n",fn, asn1PDUInfo[pdu_type].name);
	
	/* Check library intialization.
	 */
	if ( !S_Initialized )
	{
		return ( ASN1_ERROR(EM_LIBRARY_NOT_INTIALIZED) );
	}

	/* Check validity of function arguments.
	 */
	if ( contextPtr == 0 || pdu_type == EM_ASN1_PDUTYPE_INVALID )
	{
		return ( ASN1_ERROR(EM_INVALID_PARAM) );
	}

	/* Initialize the OssGlobal structure.
	 */
	STAMP_OBJECT(contextPtr, EM_ASN1_CONTEXT);

	LOG(ASN1_TRACE(contextPtr->logId),"%s: Calling ossinit\n", fn);

#ifdef EVAL_BUILD
	contextPtr->p_ossContext =
				emGlobalUserServicesPtr->FnAllocateMemory(sizeof(OssGlobal));
	if(!contextPtr->p_ossContext)
	{
		return ( ASN1_ERROR(EM_MEM_ALLOC_FAILED) );
	}
#else
	contextPtr->p_ossContext = &contextPtr->ossContext;
#endif

	rc = ossDupWorld(&asn1PDUInfo[pdu_type].master_world,
					contextPtr->p_ossContext);
	LOG(ASN1_TRACE(contextPtr->logId),
		"%s: Returned from ossDupWorld: %d\n", fn, rc);
	if ( rc != 0 )
	{
		return ( ASN1_ERROR(rc + EM_CODE_MAX) );
	}
	
	/* Set encoding and decoding flags.
	 */
	if (	(rc = EmASN1SetEncodingFlags(contextPtr, 
				NOCONSTRAIN | NOTRAPPING | FRONT_ALIGN)) 
			!= EM_SUCCESS
			
		||	(rc = EmASN1SetDecodingFlags(contextPtr, FRONT_ALIGN)) 
			!= EM_SUCCESS
	   )
	{
		return ( rc );
	}

	/* Set PDU type.
	 */
	contextPtr->pduType = pdu_type;

#ifdef EVAL_BUILD
	contextPtr->p_ossContext->asn1prnt = contextPtr->asn1prnt;
#endif

	return ( EM_SUCCESS );
}


/**
 *
 *	@Name:			EmASN1UnInitContext
 *
 *	@Description:	Uninitializes the EmASN1Context passed in by contextPtr.
 *
 * 	@Caveats:		Should only be called when all encoding/decoding has
 *				  	ceased. Frees resources needed to perform encoding/decoding.
 **/
void EmASN1UnInitContext(
	EmASN1Context			*	contextPtr)
{
	const char	fn[] = "EmASN1UninitContext";

	/* Log function entry 
	 */
	LOG(ASN1_FNENT(contextPtr->logId), 
		"%s(0x%x)\n", fn, (unsigned)contextPtr);
	
    /* Check library intialization.
     */
    if ( !S_Initialized )
    {
        return;
    }
 
	/* Check validity of function arguments.
	 */
	if ( contextPtr == 0 )
	{
		return;
	}

	CHECK_STAMP(contextPtr, EM_ASN1_CONTEXT);

	/* Invalidate the context.
	 */
	contextPtr->pduType = 0;

	/* Terminate the oss encoder/decoder.
	 */
	LOG(ASN1_TRACE(contextPtr->logId),"%s: Calling ossterm\n", fn);
#ifndef win32
    ossterm(contextPtr->p_ossContext);
#else
    ossWterm(contextPtr->p_ossContext);
#endif

#ifdef EVAL_BUILD
	emGlobalUserServicesPtr->FnFreeMemory((void *)contextPtr->p_ossContext);
#endif

	LOG(ASN1_TRACE(contextPtr->logId),"%s: Returned from ossterm\n", fn);
}


/**
 * 	@Name:			EmASN1SetDecodingFlags
 *
 *	@Description:	Allows the user to set the decoding flags dynamically.
 **/
ProtReturnCode EmASN1SetDecodingFlags(
	EmASN1Context			*	contextPtr, 
	int							decodeFlag)
{
	const char	fn[] = "EmASN1SetDecodingFlags";
	int rc = 0;

	/* 	Log function entry 
	 */
	LOG(ASN1_FNENT(contextPtr->logId),
		"%s(0x%x, 0x%x)\n", fn,
		(unsigned)contextPtr, (unsigned)decodeFlag);
	
    /* Check library intialization.
     */
    if ( !S_Initialized )
    {
        return ( ASN1_ERROR(EM_LIBRARY_NOT_INTIALIZED) );
    }
 
	/* 	Check validity of the function argument.
	 */
	if ( contextPtr == 0 )
	{
		return ( ASN1_ERROR(EM_INVALID_PARAM) );
	}

	CHECK_STAMP(contextPtr, EM_ASN1_CONTEXT);

	/*	Set the flag
	 */		
	LOG(ASN1_TRACE(contextPtr->logId),
		"%s: Calling ossSetDecodingFlags\n", fn);
	rc = ossSetDecodingFlags(contextPtr->p_ossContext, decodeFlag);
	LOG(ASN1_TRACE(contextPtr->logId),
		"Returned from ossSetDecodingFlags: %d\n", fn, rc);

	if ( rc != 0 )
	{
		return ( ASN1_ERROR(rc + EM_CODE_MAX) );
	}
	
	return ( EM_SUCCESS );	
}

/**
 * 	@Name:			EmASN1SetEncodingFlags
 *
 * 	@Description:	Allows the user to set the encoding flags dynamically.
 **/
ProtReturnCode EmASN1SetEncodingFlags(
	EmASN1Context			*	contextPtr, 
	int							encodeFlag)
{
	const char	fn[] = "EmASN1SetEncodingFlag";
	int rc = 0;

	/* 	Log function entry 
	 */
	LOG(ASN1_FNENT(contextPtr->logId),
		"%s(0x%x, 0x%x)\n", fn,
		(unsigned)contextPtr, (unsigned)encodeFlag);
	
    /* Check library intialization.
     */
    if ( !S_Initialized )
    {
        return ( ASN1_ERROR(EM_LIBRARY_NOT_INTIALIZED) );
    }
 
	/* 	Check validity of the function argument.
	 */
	if ( contextPtr == 0 )
	{
		return ( ASN1_ERROR(EM_INVALID_PARAM) );
	}

	CHECK_STAMP(contextPtr, EM_ASN1_CONTEXT);

	/*	Set the flag
	 */		
	LOG(ASN1_TRACE(contextPtr->logId),
		"%s: Calling ossSetEncodingFlags\n", fn);
	rc = ossSetEncodingFlags(contextPtr->p_ossContext, encodeFlag);
	LOG(ASN1_TRACE(contextPtr->logId),
		"%s: Returned from ossSetEncodingFlags: %d\n", fn, rc);

	if ( rc != 0 )
	{
		return ( ASN1_ERROR(rc + EM_CODE_MAX) );
	}
	
	return ( EM_SUCCESS );	
}

/**
 *	@Name:			EmASN1EncodePDU
 *
 *	@Descrition:	This function encodes "pduPtr", and puts the encoded data 
 * 				  	into *outputBytesPtr and the length of the encoded data
 *    			  	into *lengthPtr.
 *
 *	@Caveats:			If the outputBytesPtr field is 0 the encoder will 
 *					allocate the output buffer, otherwise it will use the 
 *					pre-allocated buffer identified by *outputBytesPtr and 
 *					use *lengthPtr as the length of the pre-allocated buffer. 
 *						If the encoder allocates the output buffer, 
 *					EmASN1FreeBuf should be called to free the memory.
 **/
ProtReturnCode EmASN1EncodePDU(
	EmASN1Context			*	contextPtr, 
	void					*	pduPtr, 
	unsigned char			**	outputBytesPtr, 
	long					*	lengthPtr)
{
	const char fn[] = "EmASN1EncodePDU";
	int 	rc = 0;
	int 	pduNum;
	OssBuf	asnMsg;

	/* 	Log function entry 
	 */
	LOG(ASN1_FNENT(contextPtr->logId),
		"%s(0x%x, 0x%x, 0x%x, 0x%x)\n", fn,
		(unsigned)contextPtr, (unsigned)pduPtr,
		(unsigned)outputBytesPtr,(unsigned)lengthPtr);
	
    /* Check library intialization.
     */
    if ( !S_Initialized )
    {
        return ( ASN1_ERROR(EM_LIBRARY_NOT_INTIALIZED) );
    }

	/* 	Check validity of the function argument.
	 */
	if (	contextPtr				== 0 
		||	contextPtr->pduType		== 0
		||	contextPtr->pduType		>  EM_ASN1_PDUTYPE_MAX
		||	pduPtr					== 0 
		||	outputBytesPtr			== 0
		||	lengthPtr				== 0 )
	{
		return ( ASN1_ERROR(EM_INVALID_PARAM) );
	}

	CHECK_STAMP(contextPtr, EM_ASN1_CONTEXT);

	/*	Set the pdunum from our table.
	 */	
	pduNum = asn1PDUInfo[contextPtr->pduType].pdu;
	
	LOG(ASN1_INFO(contextPtr->logId),
		"%s: pduType: %s\n", fn, asn1PDUInfo[contextPtr->pduType].name);
	
	/* 	Set up the buffer.
	 */
	if ( *outputBytesPtr )
	{
		/* 	We have a pre-allocated buffer.
		 */
	 	asnMsg.value 	= (void *)*outputBytesPtr;
		asnMsg.length 	= *lengthPtr;

		LOG(ASN1_INFO(contextPtr->logId),
			"%s: Pre-allocated buffer: value:0x%x length:%ld\n", fn, 
			(unsigned)asnMsg.value, asnMsg.length);
	}
	else
	{
		/*	Let the ASN.1 library allocate the buffer.
		 */
		asnMsg.value 	= 0;
		asnMsg.length 	= 0;

		LOG(ASN1_INFO(contextPtr->logId),
			"%s: Library allocated buffer\n", fn);
	}

	/* Encode
	 */
	LOG(ASN1_TRACE(contextPtr->logId),
		"%s: Calling ossEncode\n", fn);
	rc = ossEncode(contextPtr->p_ossContext, pduNum, pduPtr, &asnMsg);
	LOG(ASN1_TRACE(contextPtr->logId),
		"%s: Returning from ossEncode: %d\n", fn, rc);

	if ( rc != PDU_ENCODED ) 
	{
		*lengthPtr = 0;
		return ( ASN1_ERROR(rc + EM_CODE_MAX) );
	}
	
	*lengthPtr = asnMsg.length;

	/*
	 * If the the library allocated the buffer, pass it back.
	 */
	if ( *outputBytesPtr == 0)
	{
		*outputBytesPtr = asnMsg.value;
	}

	LOG(ASN1_INFO(contextPtr->logId),
		"%s: Encoded buffer: value:0x%x length:%ld\n", fn, 
		(unsigned)asnMsg.value, asnMsg.length);
	
	return ( EM_SUCCESS );
}


/**
 *	@Name:			EmASN1DecodePDU
 *
 *	@Description:	This function decodes "*inputBytesPtr" and puts the
 *					decoded data into *pduPtr.
 *
 *	@Caveats:			The input argument lengthPtr will be modified to reflect 
 *					the number of bytes left in the *inputBytesPtr buffer.
 *					*inputBytesPtr will be modified to point to the byte
 *					after the last that was decoded. If *lengthPtr is returned
 *					as non-zero multiple PDU have been contatenated in one 
 *					message.
 *						If the pduPtr field is 0 the decoder will allocate
 *					the pdu structure and the value in pduLength is ignored.
 *					the user should call EmASN1FreePDU to free the allocated
 *					memory.
 *						If the pduPtr field is non-0, it is assumed the pduPtr
 *					points to a pre-allocated pdu buffer and the pduLength
 *					holds the size of the pdu buffer.
 *						In the case of failure, *lengthPtr and *inputBytesPtr 
 *					will be set to 0 and an error will be returned.
 **/
ProtReturnCode EmASN1DecodePDU(
	EmASN1Context			*	contextPtr, 
	void					**	pduPtr, 
	long						pduLength, 
	unsigned char			**	inputBytesPtr, 
	long					*	lengthPtr)
{
	const char	fn[] = "EmASN1DecodePDU";
	int		rc = 0;
	int 	pduNum;
	OssBuf	asnMsg;

	/* 	Log function entry 
	 */
	LOG(ASN1_FNENT(contextPtr->logId),
		"%s(0x%x, 0x%x, %ld, 0x%x, 0x%x)\n", fn,
		(unsigned)contextPtr, (unsigned)pduPtr, pduLength,
		(unsigned)inputBytesPtr, (unsigned)lengthPtr);

    /* Check library intialization.
     */
    if ( !S_Initialized )
    {
        return ( ASN1_ERROR(EM_LIBRARY_NOT_INTIALIZED) );
    }
	
	/* 	Check validity of the function argument.
	 */
	if (	contextPtr				== 0 
		||	contextPtr->pduType		== 0
		||	contextPtr->pduType		>  EM_ASN1_PDUTYPE_MAX
		||	pduPtr					== 0
		||	inputBytesPtr			== 0 
		||	*inputBytesPtr			== 0 
		||	lengthPtr				== 0 )
	{
		return ( ASN1_ERROR(EM_INVALID_PARAM) );
	}

	CHECK_STAMP(contextPtr, EM_ASN1_CONTEXT);

	/*	Set the pdunum from our table.
	 */	
	pduNum = asn1PDUInfo[contextPtr->pduType].pdu;
	
	LOG(ASN1_INFO(contextPtr->logId),
		"%s: pduType: %s\n", fn, asn1PDUInfo[contextPtr->pduType].name);
	
	/* 	Set up the pdu buffer.
	 */
	if ( *pduPtr )
	{
		/* 	We have a pre-allocated buffer.
		 */
		LOG(ASN1_INFO(contextPtr->logId),
			"%s: Pre-allocated PDU buffer: value:0x%x length:%d\n", fn, 
			(unsigned)*pduPtr, pduLength);

		LOG(ASN1_TRACE(contextPtr->logId), 
			"%s: Calling ossSetDecodingLength:%d\n", fn, pduLength);
		rc = ossSetDecodingLength(contextPtr->p_ossContext, pduLength);
		LOG(ASN1_TRACE(contextPtr->logId), 
			"%s: Returning from ossSetDecodingLength: %d\n", fn, rc);

		if ( rc != 0 )
		{
			*lengthPtr = 0;		/* Defensive: In the case where the user */
			*inputBytesPtr = 0;	/*			  ignores the return code.   */

			return ( ASN1_ERROR(rc + EM_CODE_MAX) );
		}
	}

	/* Set up the byte buffer.
	 */
	asnMsg.value	= (void *)*inputBytesPtr;
	asnMsg.length	= *lengthPtr;

	/* Decode
	 *
	 * Note:	Decode changes the value of pduNum on the way out
	 *			when decoding BER types. Since this is used in
	 *			an h.323 stack, all the protocols use PER, which
	 *			means we need to provide the pdu number and can
	 *			ignore the returned pdu number.
	 */
	LOG(ASN1_TRACE(contextPtr->logId),
		"%s: Calling ossDecode\n", fn);
	rc = ossDecode(contextPtr->p_ossContext, &pduNum, &asnMsg, pduPtr);
	LOG(ASN1_TRACE(contextPtr->logId),
		"%s: Returning from ossDecode: %d\n", fn, rc);

	if ( rc != PDU_DECODED ) 
	{
		*lengthPtr = 0;		/* Defensive: In the case where the user */
		*inputBytesPtr = 0;	/*			  ignores the return code.   */

		return ( ASN1_ERROR(rc + EM_CODE_MAX) );
	}

	/* Update the contents lengthPtr.
	 */
	*lengthPtr = asnMsg.length;

	LOG(ASN1_INFO(contextPtr->logId),
		"%s: Decoded input buffer: value:0x%x length:%d\n", fn, 
		(unsigned)asnMsg.value, asnMsg.length);

	return ( EM_SUCCESS );
}


/**
 * 	@Name:			EmASN1FreePDU
 *
 *	@Description:	Frees the output memory pointed to by "pduPtr".  
 *    
 *	@Caveats:		You should not use this function to free a data 
 *					structure that you yourself allocated.
 *
 **/
ProtReturnCode EmASN1FreePDU(
	EmASN1Context			*	contextPtr, 
	void					*	pduPtr)
{
	const char	fn[] = "EmASN1FreePDU";
	int		rc = 0;
	int 	pduNum;

	/* 	Log function entry 
	 */
	LOG(ASN1_FNENT(contextPtr->logId),
		"%s(0x%x, 0x%x)\n", fn,
		(unsigned)contextPtr, (unsigned)pduPtr);

    /* Check library intialization.
     */
    if ( !S_Initialized )
    {
        return ( ASN1_ERROR(EM_LIBRARY_NOT_INTIALIZED) );
    }

	/* 	Check validity of the function argument.
	 */
	if (	contextPtr				== 0 
		||	contextPtr->pduType		== 0
		||	contextPtr->pduType		>  EM_ASN1_PDUTYPE_MAX 
		||	pduPtr					== 0 )
	{
		return ( ASN1_ERROR(EM_INVALID_PARAM) );
	}

	CHECK_STAMP(contextPtr, EM_ASN1_CONTEXT);

	/* Get pdu number.
	 */
	pduNum = asn1PDUInfo[contextPtr->pduType].pdu;

	/* Free the PDU.
	 */
	LOG(ASN1_TRACE(contextPtr->logId), 
		"%s: Calling ossFreePDU\n", fn);
	rc = ossFreePDU(contextPtr->p_ossContext, pduNum, pduPtr);
	LOG(ASN1_TRACE(contextPtr->logId), 
		"%s: Returning from ossFreePDU: %d\n", fn, rc);

	if ( rc != 0 )
	{
		return ( ASN1_ERROR(rc + EM_CODE_MAX) );
	}

	return ( EM_SUCCESS );
}


/**
 *	@Name:			EmASN1FreeBuf
 *
 *	@Description:	Frees the encoder output memory pointed to by "bufPtr".
 *
 * 	@Caveat:		If you pre-allocated the output memory passed to 
 *					EmASN1Encode do not use this function to free it.
 **/
ProtReturnCode EmASN1FreeBuf(
	EmASN1Context			*	contextPtr, 
	unsigned char			*	bufPtr)
{
	const char	fn[] = "EmASN1FreeBuf";
	int		rc = 0;

	/* 	Log function entry 
	 */
	LOG(ASN1_FNENT(contextPtr->logId),
		"%s(0x%x, 0x%x)\n", fn,
		(unsigned)contextPtr, (unsigned)bufPtr);

    /* Check library intialization.
     */
    if ( !S_Initialized )
    {
        return ( ASN1_ERROR(EM_LIBRARY_NOT_INTIALIZED) );
    }

	/* 	Check validity of the function argument.
	 */
	if (	contextPtr				== 0 
		||	bufPtr					== 0 )
	{
		return ( ASN1_ERROR(EM_INVALID_PARAM) );
	}

	CHECK_STAMP(contextPtr, EM_ASN1_CONTEXT);

	/* Free the buffer.
	 */
	LOG(ASN1_TRACE(contextPtr->logId), 
		"%s: Calling ossFreeBuf\n", fn);
	ossFreeBuf(contextPtr->p_ossContext, (char *)bufPtr);
	LOG(ASN1_TRACE(contextPtr->logId), 
		"%s: Returning from ossFreeBuf: %d\n", fn, rc);

	return ( EM_SUCCESS );
}

/**
 *	@Name:			EmASN1GetContextSize
 *
 *	@Description:	Gives the size of the EmASN1Context structure 
 *					at run-time, so a re-compile isn't necessary if the 
 *					structure size changes across different releases.
 *
 *	@Returns:		Returns 0 if the library has not been initialized, 
 *					otherwise a valid size is returned.
 **/
int	EmASN1GetContextSize()
{
	return ( S_EmASN1ContextSize );
}

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
