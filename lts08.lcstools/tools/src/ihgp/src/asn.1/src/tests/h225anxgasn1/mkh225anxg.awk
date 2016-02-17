#!/usr/bin/ksh
# SCCS id:	@(#)mkh225anxg.awk	7.1.2.2	11/17/99
# 
#              Copyright (c) 1996-1998 Lucent Technologies
#                      All Rights Reserved
#
#                      THIS IS UNPUBLISHED
#                      PROPRIETARY SOURCE
#                  CODE OF Lucent Technologies
#
#          The copyright notice above does not evidence any
#         actual or intended publication of such source code
#

if [ $# != 1 ]
then
	echo Usage: `$BASENAME $0` filename
	exit 1
fi

SYS=`uname -s`
if [ $SYS = "SunOS" ]
then
	AWK=nawk
else
	AWK=awk
fi

cp $1 $1-
$AWK 'BEGIN {
	# Defined in em_asn1_common.h
	RemoveStruct["Em_ObjectID"] = 1
	RemoveTypedef["EmObjectIdentifier"] = 1
	RemoveStruct["EmOctetString"] = 1
	RemoveStruct["EmBitString"] = 1
	RemoveStruct["EmIP"] = 1
	RemoveStruct["EmH221NonStandard"] = 1
	RemoveStruct["EmNonStandardIdentifier"] = 1
	RemoveStruct["EmNonStandardParameter"] = 1
	RemoveStruct["EmSeqOfIP"] = 1
	RemoveStruct["EmRouting"] = 1
	RemoveStruct["EmH225TransportAddress"] = 1
	RemoveStruct["EmPublicTypeOfNumber"] = 1
	RemoveTypedef["EmNumberDigits"] = 1
	RemoveStruct["EmPublicPartyNumber"] = 1
	RemoveStruct["EmPrivateTypeOfNumber"] = 1
	RemoveStruct["EmPrivatePartyNumber"] = 1
	RemoveStruct["EmPartyNumber"] = 1
	RemoveStruct["EmH323ID"] = 1
	RemoveStruct["EmAliasAddress"] = 1
	RemoveStruct["EmSeqOfAliasAddress"] = 1
	RemoveStruct["EmSeqOfNonStandardParameter"] = 1
	RemoveStruct["EmSeqOfH225TransportAddress"] = 1
	RemoveStruct["EmHASHED"] = 1
	RemoveTypedef["EmHashedEncodedGeneralToken"] = 1
	RemoveTypedef["EmHashedEncodedPwdCertToken"] = 1
	RemoveStruct["EmPublicPartyNumber"] = 1

	# Defined in em_h225.h
	RemoveStruct["EmChallengeString"] = 1
	RemoveTypedef["EmTimeStamp"] = 1
	RemoveTypedef["EmRandomVal"] = 1
	RemoveStruct["EmPassword"] = 1
	RemoveStruct["EmIdentifier"] = 1
	RemoveStruct["EmH235NonStandardParameter"] = 1
	RemoveStruct["EmBitString2K"] = 1
	RemoveStruct["EmDHset"] = 1
	RemoveStruct["EmTypedCertificate"] = 1
	RemoveStruct["EmAuthenticationMechanism"] = 1
	RemoveStruct["EmClearToken"] = 1
	RemoveStruct["EmIV8"] = 1
	RemoveStruct["EmParams"] = 1
	RemoveTypedef["EmEncodedGeneralToken"] = 1
	RemoveTypedef["EmPwdCertToken"] = 1
	RemoveTypedef["EmEncodedPwdCertToken"] = 1
	RemoveStruct["EmCryptoPwdEncr"] = 1
	RemoveStruct["EmEncryptedEncodedGeneralToken"] = 1
	RemoveStruct["EmSignedEncodedGeneralToken"] = 1
	RemoveStruct["EmHashedEncodedGeneralToken"] = 1
	RemoveStruct["EmCryptoToken"] = 1
	RemoveTypedef["EmProtocolIdentifier"] = 1
	RemoveStruct["EmVendorIdentifier"] = 1
	RemoveStruct["EmGatekeeperInfo"] = 1
	RemoveTypedef["EmBandWidth"] = 1
	RemoveStruct["EmDataRate"] = 1
	RemoveStruct["EmSeqOfDataRate"] = 1
	RemoveStruct["EmSupportedPrefix"] = 1
	RemoveStruct["EmSeqOfSupportedPrefix"] = 1
	RemoveStruct["EmH310Caps"] = 1
	RemoveStruct["EmH320Caps"] = 1
	RemoveStruct["EmH321Caps"] = 1
	RemoveStruct["EmH322Caps"] = 1
	RemoveStruct["EmH323Caps"] = 1
	RemoveStruct["EmH324Caps"] = 1
	RemoveStruct["EmVoiceCaps"] = 1
	RemoveStruct["EmT120OnlyCaps"] = 1
	RemoveStruct["EmNonStandardProtocol"] = 1
	RemoveStruct["EmSupportedProtocols"] = 1
	RemoveStruct["EmSeqOfSupportedProtocols"] = 1
	RemoveStruct["EmGatewayInfo"] = 1
	RemoveStruct["EmMcuInfo"] = 1
	RemoveStruct["EmTerminalInfo"] = 1
	RemoveStruct["EmEndpointType"] = 1
	RemoveTypedef["EmCallReferenceValue"] = 1
	RemoveStruct["EmSeqOfCallReferenceValue"] = 1
	RemoveStruct["EmGloballyUniqueID"] = 1
	RemoveTypedef["EmConferenceIdentifier"] = 1
	RemoveStruct["EmQ954Details"] = 1
	RemoveStruct["EmQseriesOptions"] = 1
	RemoveStruct["EmCallType"] = 1
	RemoveStruct["EmCallIdentifier"] = 1
	RemoveStruct["EmSecurityServiceMode"] = 1
	RemoveStruct["EmSecurityCapabilities"] = 1
	RemoveStruct["EmH245Security"] = 1
	RemoveStruct["EmSeqOfH245Security"] = 1
	RemoveStruct["EmSeqOfClearToken"] = 1
	RemoveStruct["EmHashedEncodedPwdCertToken"] = 1
	RemoveStruct["EmGatekeeperIdentifier"] = 1
	RemoveStruct["EmEncryptedEncodedPwdCertToken"] = 1
	RemoveStruct["EmSignedEncodedPwdCertToken"] = 1
	RemoveTypedef["EmFastStartToken"] = 1
	RemoveTypedef["EmEncodedFastStartToken"] = 1
	RemoveStruct["EmSignedEncodedFastStartToken"] = 1
	RemoveStruct["EmCryptoH323Token"] = 1
	RemoveStruct["EmSeqOfCryptoH323Token"] = 1
	RemoveStruct["EmSeqOfOctetString"] = 1
	RemoveStruct["EmConferenceGoal"] = 1
	RemoveStruct["EmSetup_UUIE"] = 1
	RemoveStruct["EmCallProceeding_UUIE"] = 1
	RemoveStruct["EmConnect_UUIE"] = 1
	RemoveStruct["EmAlerting_UUIE"] = 1
	RemoveStruct["EmInformation_UUIE"] = 1
	RemoveStruct["EmReleaseCompleteReason"] = 1
	RemoveStruct["EmReleaseComplete_UUIE"] = 1
	RemoveStruct["EmFacilityReason"] = 1
	RemoveStruct["EmConferenceList"] = 1
	RemoveStruct["EmSeqOfConferenceList"] = 1
	RemoveStruct["EmFacility_UUIE"] = 1
	RemoveStruct["EmProgress_UUIE"] = 1
	RemoveStruct["EmH323MessageBody"] = 1
	RemoveStruct["EmH323_UU_PDU"] = 1
	RemoveStruct["EmH323_UserInformation"] = 1
	RemoveStruct["EmAlternateGK"] = 1
	RemoveStruct["EmSeqOfAlternateGK"] = 1
	RemoveStruct["EmEndpoint"] = 1
	RemoveStruct["EmSeqOfEndpoint"] = 1
	RemoveStruct["EmSeqOfAuthenticationMechanism"] = 1
	RemoveStruct["EmSeqOfObjectIdentifier"] = 1
	RemoveStruct["EmEncryptIntAlg"] = 1
	RemoveStruct["EmNonIsoIntegrityMechanism"] = 1
	RemoveStruct["EmIntegrityMechanism"] = 1
	RemoveStruct["EmSeqOfIntegrityMechanism"] = 1
	RemoveStruct["EmTransportChannelInfo"] = 1
	RemoveStruct["EmRTPSession"] = 1
	RemoveStruct["EmSeqOfRTPSession"] = 1
	RemoveStruct["EmSeqOfTransportChannelInfo"] = 1
	RemoveStruct["EmSeqOfConferenceIdentifier"] = 1
	RemoveStruct["EmCallModel"] = 1
	RemoveStruct["EmPduInfo"] = 1
	RemoveStruct["EmSeqOfPDUInfo"] = 1
	RemoveStruct["EmPerCallInfo"] = 1
	RemoveStruct["EmSeqOfPerCallInfo"] = 1
	RemoveStruct["EmAltGKInfo"] = 1
	RemoveTypedef["EmRequestSeqNum"] = 1
	RemoveStruct["EmEndpointIdentifier"] = 1
	RemoveTypedef["EmTimeToLive"] = 1
	RemoveStruct["EmICV"] = 1
	RemoveStruct["EmGRQ"] = 1
	RemoveStruct["EmGCF"] = 1
	RemoveStruct["EmGRJReason"] = 1
	RemoveStruct["EmGRJ"] = 1
	RemoveStruct["EmRRQ"] = 1
	RemoveStruct["EmRCF"] = 1
	RemoveStruct["EmRRJReason"] = 1
	RemoveStruct["EmRRJ"] = 1
	RemoveStruct["EmURQReason"] = 1
	RemoveStruct["EmURQ"] = 1
	RemoveStruct["EmUCF"] = 1
	RemoveStruct["EmURJReason"] = 1
	RemoveStruct["EmURJ"] = 1
	RemoveStruct["EmTransportQOS"] = 1
	RemoveStruct["EmARQ"] = 1
	RemoveStruct["EmUUIEsRequested"] = 1
	RemoveStruct["EmACF"] = 1
	RemoveStruct["EmARJReason"] = 1
	RemoveStruct["EmARJ"] = 1
	RemoveStruct["EmBRQ"] = 1
	RemoveStruct["EmBCF"] = 1
	RemoveStruct["EmBRJReason"] = 1
	RemoveStruct["EmBRJ"] = 1
	RemoveStruct["EmDisengageReason"] = 1
	RemoveStruct["EmDRQ"] = 1
	RemoveStruct["EmDCF"] = 1
	RemoveStruct["EmDRJReason"] = 1
	RemoveStruct["EmDRJ"] = 1
	RemoveStruct["EmLRQ"] = 1
	RemoveStruct["EmLCF"] = 1
	RemoveStruct["EmLRJReason"] = 1
	RemoveStruct["EmLRJ"] = 1
	RemoveStruct["EmIRQ"] = 1
	RemoveStruct["EmIRR"] = 1
	RemoveStruct["EmNSM"] = 1
	RemoveStruct["EmXRS"] = 1
	RemoveStruct["EmRIP"] = 1
	RemoveStruct["EmRAI"] = 1
	RemoveStruct["EmRAC"] = 1
	RemoveStruct["EmIACK"] = 1
	RemoveStruct["EmINAKReason"] = 1
	RemoveStruct["EmINAK"] = 1
	RemoveStruct["EmRasMessage"] = 1
	printf("/* SCCS id:	%%%s%%	%%%s%%	*/\n\n","W","G")
}
{
	if(($1 == "") && (remove_empty == 1)) {
		remove_empty = 0
		next
	}
	if($0 ~ /#define.*Em/) {
		if(addedInclude == 0) {
			printf("#include \"asn1/em_asn1_common.h\"\n")
			printf("#include \"asn1/em_h225.h\"\n\n")
			addedInclude = 1
		}
		sub("Em","EM_",$0)
	}
	if($1 == "typedef") {
		if($2 == "struct") {
			if($3 in RemoveStruct) {	# match sprintf {
				if(RemoveStruct[$3] > 1) {
					RemoveStruct[$3]--
				} else {
					endstr = sprintf("%s;",RemoveStruct[$3])
					while((!($0 ~ /^}/)) && ($2 != endstr)) {
						getline
						sub(/\015/,"")
					}
					remove_empty = 1
					next
				}
			}
		} else {
			name = $NF
			gsub(/[^A-Za-z_]/,"",name)
			if(name in RemoveTypedef) {
				if(RemoveTypedef[name] > 1) {
					RemoveTypedef[name]--
				} else {
					remove_empty = 1
					next
				}
			}
		}
	}
	print $0
}' $1- > $1
rm $1-	
