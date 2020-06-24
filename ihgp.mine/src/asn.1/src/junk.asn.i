


MULTIMEDIA-SYSTEM-CONTROL DEFINITIONS AUTOMATIC TAGS ::=
BEGIN







MultimediaSystemControlMessage	::= CHOICE
{
	request	RequestMessage ,
	response	ResponseMessage ,
	command	CommandMessage ,
	indication	IndicationMessage ,
	...
}



RequestMessage	::= CHOICE
{
	nonStandard	NonStandardMessage,

	masterSlaveDetermination	MasterSlaveDetermination,

	terminalCapabilitySet	TerminalCapabilitySet,

	openLogicalChannel	OpenLogicalChannel,
	closeLogicalChannel	CloseLogicalChannel,

	requestChannelClose	RequestChannelClose,

	multiplexEntrySend	MultiplexEntrySend ,

	requestMultiplexEntry	RequestMultiplexEntry ,

	requestMode	RequestMode,

	roundTripDelayRequest	RoundTripDelayRequest,

	maintenanceLoopRequest	MaintenanceLoopRequest,

	...,
	communicationModeRequest	CommunicationModeRequest,
	conferenceRequest	ConferenceRequest ,
						
	h223AnnexAReconfiguration	H223AnnexAReconfiguration 

}



ResponseMessage	::= CHOICE
{
	nonStandard	NonStandardMessage,

	masterSlaveDeterminationAck	MasterSlaveDeterminationAck,
	masterSlaveDeterminationReject	MasterSlaveDeterminationReject,

	terminalCapabilitySetAck	TerminalCapabilitySetAck,
	terminalCapabilitySetReject	TerminalCapabilitySetReject,

	openLogicalChannelAck	OpenLogicalChannelAck,
	openLogicalChannelReject	OpenLogicalChannelReject,
	closeLogicalChannelAck	CloseLogicalChannelAck,

	requestChannelCloseAck	RequestChannelCloseAck ,
	requestChannelCloseReject	RequestChannelCloseReject ,

	multiplexEntrySendAck	MultiplexEntrySendAck ,
	multiplexEntrySendReject	MultiplexEntrySendReject ,

	requestMultiplexEntryAck	RequestMultiplexEntryAck,
	requestMultiplexEntryReject	RequestMultiplexEntryReject,

	requestModeAck	RequestModeAck,
	requestModeReject	RequestModeReject,

	roundTripDelayResponse	RoundTripDelayResponse,

	maintenanceLoopAck	MaintenanceLoopAck,
	maintenanceLoopReject	MaintenanceLoopReject,

	...,
	communicationModeResponse	CommunicationModeResponse,

	conferenceResponse	ConferenceResponse ,
							
	h223AnnexAReconfigurationAck	H223AnnexAReconfigurationAck ,
	h223AnnexAReconfigurationReject	H223AnnexAReconfigurationReject	


}



CommandMessage	::= CHOICE
{
	nonStandard	NonStandardMessage,

	maintenanceLoopOffCommand	MaintenanceLoopOffCommand,

	sendTerminalCapabilitySet	SendTerminalCapabilitySet,

	encryptionCommand	EncryptionCommand,

	flowControlCommand	FlowControlCommand,

	endSessionCommand	EndSessionCommand,

	miscellaneousCommand	MiscellaneousCommand,

	...,
	communicationModeCommand	CommunicationModeCommand,

	conferenceCommand	ConferenceCommand 

}



IndicationMessage	::= CHOICE
{
	nonStandard	NonStandardMessage,

	functionNotUnderstood	FunctionNotUnderstood, 
	
	masterSlaveDeterminationRelease	MasterSlaveDeterminationRelease,

	terminalCapabilitySetRelease	TerminalCapabilitySetRelease,

	openLogicalChannelConfirm	OpenLogicalChannelConfirm,

	requestChannelCloseRelease	RequestChannelCloseRelease,

	multiplexEntrySendRelease	MultiplexEntrySendRelease ,

	requestMultiplexEntryRelease	RequestMultiplexEntryRelease ,

	requestModeRelease	RequestModeRelease,

	miscellaneousIndication	MiscellaneousIndication ,

	jitterIndication	JitterIndication,

	h223SkewIndication	H223SkewIndication ,

	newATMVCIndication	NewATMVCIndication ,

	userInput	UserInputIndication,
	...,
	h2250MaximumSkewIndication	H2250MaximumSkewIndication,
 
	mcLocationIndication	MCLocationIndication,

	conferenceIndication	ConferenceIndication ,

	vendorIdentification	VendorIdentification ,
	
	functionNotSupported	FunctionNotSupported

}


SequenceNumber	::=INTEGER (0..255)





NonStandardMessage	::= SEQUENCE
{
	nonStandardData	NonStandardParameter ,
	...
}

NonStandardParameter	::= SEQUENCE
{
	nonStandardIdentifier	NonStandardIdentifier,
	data		OCTET STRING
}

NonStandardIdentifier	::= CHOICE
{
	object		OBJECT IDENTIFIER ,
	h221NonStandard	SEQUENCE
	{
		t35CountryCode	INTEGER (0..255),	
		t35Extension	INTEGER (0..255),	
		manufacturerCode	INTEGER (0..65535)	
	}
}





MasterSlaveDetermination	::= SEQUENCE
{
	terminalType	INTEGER (0..255),
	statusDeterminationNumber	INTEGER (0..16777215),
	...
}

MasterSlaveDeterminationAck	::= SEQUENCE
{
	decision	CHOICE
	{
		master	NULL,
		slave	NULL
	},
	...
}

MasterSlaveDeterminationReject	::= SEQUENCE
{
	cause		CHOICE
	{
		identicalNumbers	NULL,
		...
	},
	...
}

MasterSlaveDeterminationRelease	::= SEQUENCE
{
	...
}







TerminalCapabilitySet	::= SEQUENCE
{
	sequenceNumber	SequenceNumber,

	protocolIdentifier	OBJECT IDENTIFIER ,
				
				
				

	multiplexCapability	MultiplexCapability  OPTIONAL,

	capabilityTable	SET  SIZE (1..256) OF CapabilityTableEntry  OPTIONAL,

	capabilityDescriptors	SET  SIZE (1..256) OF CapabilityDescriptor  OPTIONAL,

	...
}

V75Capability	::= SEQUENCE
{
	audioHeader	BOOLEAN,
	...
}

CapabilityTableEntry	::= SEQUENCE
{
	capabilityTableEntryNumber	CapabilityTableEntryNumber,
	capability	Capability  OPTIONAL
}

CapabilityDescriptor	::= SEQUENCE
{
	capabilityDescriptorNumber	CapabilityDescriptorNumber,
	simultaneousCapabilities	SET  SIZE (1..256) OF AlternativeCapabilitySet  OPTIONAL
}

AlternativeCapabilitySet	::= SEQUENCE  SIZE (1..256) OF CapabilityTableEntryNumber 

CapabilityTableEntryNumber	::=INTEGER (1..65535)

CapabilityDescriptorNumber	::=INTEGER (0..255)

TerminalCapabilitySetAck	::= SEQUENCE
{
	sequenceNumber	SequenceNumber,
	...
}

TerminalCapabilitySetReject	::= SEQUENCE
{
	sequenceNumber	SequenceNumber,
	cause		CHOICE
	{
		unspecified	NULL,
		undefinedTableEntryUsed	NULL,
		descriptorCapacityExceeded	NULL,
		tableEntryCapacityExceeded	CHOICE
		{
			highestEntryNumberProcessed	CapabilityTableEntryNumber,
			noneProcessed	NULL
		},
		...
	},
	...
}

TerminalCapabilitySetRelease	::= SEQUENCE
{
	...
}





Capability		::= CHOICE
{
	nonStandard	NonStandardParameter ,

	receiveVideoCapability	VideoCapability ,
	transmitVideoCapability	VideoCapability ,
	receiveAndTransmitVideoCapability	VideoCapability ,

	receiveAudioCapability	AudioCapability ,
	transmitAudioCapability	AudioCapability ,
	receiveAndTransmitAudioCapability	AudioCapability ,

	receiveDataApplicationCapability	DataApplicationCapability ,
	transmitDataApplicationCapability	DataApplicationCapability ,
	receiveAndTransmitDataApplicationCapability	DataApplicationCapability ,

	h233EncryptionTransmitCapability	BOOLEAN,
	h233EncryptionReceiveCapability	SEQUENCE
	{
		h233IVResponseTime	INTEGER (0..255),	
		...
	},
	...,
	conferenceCapability	ConferenceCapability

}





MultiplexCapability	::= CHOICE
{
	nonStandard	NonStandardParameter ,
	h222Capability	H222Capability ,
	h223Capability	H223Capability ,
	v76Capability	V76Capability ,
	...,
	h2250Capability	H2250Capability ,
					
	h223AnnexACapability	H223AnnexACapability 	
}

H222Capability	::= SEQUENCE
{
	numberOfVCs	INTEGER (1..256),
	vcCapability	SET  OF VCCapability ,
	...
}

VCCapability	::= SEQUENCE
{
	aal1		SEQUENCE
	{
		nullClockRecovery	BOOLEAN,
		srtsClockRecovery	BOOLEAN,
		adaptiveClockRecovery	BOOLEAN,
		nullErrorCorrection	BOOLEAN,
		longInterleaver	BOOLEAN,
		shortInterleaver	BOOLEAN,
		errorCorrectionOnly	BOOLEAN,
		structuredDataTransfer	BOOLEAN,
		partiallyFilledCells	BOOLEAN,
		...
	}  OPTIONAL,
	aal5		SEQUENCE
	{
		forwardMaximumSDUSize	INTEGER (0..65535),	
		backwardMaximumSDUSize	INTEGER (0..65535),	
		...
	}  OPTIONAL,
	transportStream	BOOLEAN,
	programStream	BOOLEAN,
	availableBitRates	SEQUENCE
	{
		type	CHOICE
		{
			singleBitRate	INTEGER (1..65535),	
			rangeOfBitRates	SEQUENCE
			{
				lowerBitRate	INTEGER (1..65535),	
				higherBitRate	INTEGER (1..65535)	
			}
		},
		...
	},
	...
}

H223Capability	::= SEQUENCE
{
	transportWithI-frames	BOOLEAN,	

	videoWithAL1	BOOLEAN,
	videoWithAL2	BOOLEAN,
	videoWithAL3	BOOLEAN,
	audioWithAL1	BOOLEAN,
	audioWithAL2	BOOLEAN,
	audioWithAL3	BOOLEAN,
	dataWithAL1	BOOLEAN,
	dataWithAL2	BOOLEAN,
	dataWithAL3	BOOLEAN,

	maximumAl2SDUSize	INTEGER (0..65535),	
	maximumAl3SDUSize	INTEGER (0..65535),	

	maximumDelayJitter	INTEGER (0..1023),	

	h223MultiplexTableCapability	CHOICE
	{
		basic	NULL,
		enhanced	SEQUENCE
		{
			maximumNestingDepth	INTEGER (1..15),
			maximumElementListSize	INTEGER (2..255),
			maximumSubElementListSize	INTEGER (2..255),
			...
		}
	},
	...
}

V76Capability	::= SEQUENCE
{
	suspendResumeCapabilitywAddress	BOOLEAN,
	suspendResumeCapabilitywoAddress	BOOLEAN,
	rejCapability	BOOLEAN,
	sREJCapability	BOOLEAN,
	mREJCapability	BOOLEAN,
	crc8bitCapability	BOOLEAN,
	crc16bitCapability	BOOLEAN, 
	crc32bitCapability	BOOLEAN,
	uihCapability	BOOLEAN,
	numOfDLCS	INTEGER (2..8191),
	twoOctetAddressFieldCapability	BOOLEAN,
	loopBackTestCapability	BOOLEAN,
	n401Capability	INTEGER (1..4095),
	maxWindowSizeCapability	INTEGER (1..127),
	v75Capability	V75Capability,
	...
}

H2250Capability	::= SEQUENCE
{
	maximumAudioDelayJitter	INTEGER(0..1023),	
	receiveMultipointCapability	MultipointCapability,
	transmitMultipointCapability	MultipointCapability,
	receiveAndTransmitMultipointCapability	MultipointCapability,
	mcCapability	SEQUENCE
	{
		centralizedConferenceMC	BOOLEAN,
		decentralizedConferenceMC	BOOLEAN,
		...
	},
	rtcpVideoControlCapability	BOOLEAN,	
	mediaPacketizationCapability	MediaPacketizationCapability,
	...
}

MediaPacketizationCapability	::= SEQUENCE
{
	h261aVideoPacketization	BOOLEAN,
	...
}

MultipointCapability	::= SEQUENCE
{
	multicastCapability	BOOLEAN,	
	multiUniCastConference	BOOLEAN,	
	mediaDistributionCapability	SEQUENCE  OF MediaDistributionCapability ,
	...
}

MediaDistributionCapability	::= SEQUENCE
{
	centralizedControl	BOOLEAN,
	distributedControl	BOOLEAN,	
	centralizedAudio	BOOLEAN,
	distributedAudio	BOOLEAN,
	centralizedVideo	BOOLEAN,
	distributedVideo	BOOLEAN,
	centralizedData	SEQUENCE OF DataApplicationCapability OPTIONAL,
	distributedData	SEQUENCE OF DataApplicationCapability OPTIONAL,	
						
	...
}

H223AnnexACapability	::= SEQUENCE
{
	transferWithI-frames	BOOLEAN,		

	videoWithAL1M	BOOLEAN,
	videoWithAL2M	BOOLEAN,
	videoWithAL3M	BOOLEAN,
	audioWithAL1M	BOOLEAN,
	audioWithAL2M	BOOLEAN,
	audioWithAL3M	BOOLEAN,
	dataWithAL1M	BOOLEAN,
	dataWithAL2M	BOOLEAN,
	dataWithAL3M	BOOLEAN,
 
	maximumAL2MSDUSize	INTEGER (0..65535),	
	maximumAL3MSDUSize	INTEGER (0..65535),	
 
	maximumDelayJitter 	INTEGER (0..1023),	
 
	reconfigurationCapability	BOOLEAN,

	h223AnnexAMultiplexTableCapability	CHOICE		
	{
		basic	NULL,
		enhanced	SEQUENCE
		{
			maximumNestingDepth	INTEGER (1..15),
			maximumElementListSize	INTEGER (2..255),
			maximumSubElementListSize	INTEGER (2..255),
			...
		},
		...
	},
	...
}






VideoCapability	::= CHOICE
{
	nonStandard	NonStandardParameter ,
	h261VideoCapability	H261VideoCapability,
	h262VideoCapability	H262VideoCapability,
	h263VideoCapability	H263VideoCapability,
	is11172VideoCapability	IS11172VideoCapability,
	...
}

H261VideoCapability	::= SEQUENCE
{
	qcifMPI	INTEGER (1..4)  OPTIONAL,	
	cifMPI		INTEGER (1..4)  OPTIONAL,	
	temporalSpatialTradeOffCapability	BOOLEAN,
	maxBitRate	INTEGER (1..19200),	
	stillImageTransmission	BOOLEAN,	
	...
}

H262VideoCapability	::= SEQUENCE
{
	profileAndLevel-SPatML	BOOLEAN,
	profileAndLevel-MPatLL	BOOLEAN,
	profileAndLevel-MPatML	BOOLEAN,
	profileAndLevel-MPatH-14	BOOLEAN,
	profileAndLevel-MPatHL	BOOLEAN,
	profileAndLevel-SNRatLL	BOOLEAN,
	profileAndLevel-SNRatML	BOOLEAN,
	profileAndLevel-SpatialatH-14	BOOLEAN,
	profileAndLevel-HPatML	BOOLEAN,
	profileAndLevel-HPatH-14	BOOLEAN,
	profileAndLevel-HPatHL	BOOLEAN,
	videoBitRate	INTEGER (0.. 1073741823)  OPTIONAL,	
	vbvBufferSize	INTEGER (0.. 262143)  OPTIONAL,	
	samplesPerLine	INTEGER (0..16383)  OPTIONAL,	
	linesPerFrame	INTEGER (0..16383)  OPTIONAL,	
	framesPerSecond	INTEGER (0..15)  OPTIONAL,	
	luminanceSampleRate	INTEGER (0..4294967295)  OPTIONAL,	
	...
}

H263VideoCapability	::= SEQUENCE
{
	sqcifMPI	INTEGER (1..32)  OPTIONAL,	
	qcifMPI	INTEGER (1..32)  OPTIONAL,	
	cifMPI		INTEGER (1..32)  OPTIONAL,	
	cif4MPI	INTEGER (1..32)  OPTIONAL,	
	cif16MPI	INTEGER (1..32)  OPTIONAL,	
	maxBitRate	INTEGER (1..192400),	
	unrestrictedVector	BOOLEAN,
	arithmeticCoding	BOOLEAN,
	advancedPrediction	BOOLEAN,
	pbFrames	BOOLEAN,
	temporalSpatialTradeOffCapability	BOOLEAN,
	hrd-B		INTEGER (0..524287)  OPTIONAL,	
	bppMaxKb	INTEGER (0..65535)  OPTIONAL,	
	...,
					
	slowSqcifMPI	INTEGER (1..3600)  OPTIONAL,	
	slowQcifMPI	INTEGER (1..3600)  OPTIONAL, 
	slowCifMPI	INTEGER (1..3600)  OPTIONAL, 
	slowCif4MPI	INTEGER (1..3600)  OPTIONAL, 
	slowCif16MPI	INTEGER (1..3600)  OPTIONAL, 
	errorCompensation	BOOLEAN
}

IS11172VideoCapability	::= SEQUENCE
{
	constrainedBitstream	BOOLEAN,
	videoBitRate	INTEGER (0.. 1073741823)  OPTIONAL,	
	vbvBufferSize	INTEGER (0.. 262143)  OPTIONAL,	
	samplesPerLine	INTEGER (0..16383)  OPTIONAL,	
	linesPerFrame	INTEGER (0..16383)  OPTIONAL,	
	pictureRate	INTEGER (0..15)  OPTIONAL,	
	luminanceSampleRate	INTEGER (0..4294967295)  OPTIONAL,	
	...
}









AudioCapability	::= CHOICE
{
	nonStandard	NonStandardParameter ,
	g711Alaw64k	INTEGER (1..256),
	g711Alaw56k	INTEGER (1..256),
	g711Ulaw64k	INTEGER (1..256),
	g711Ulaw56k	INTEGER (1..256),

	g722-64k	INTEGER (1..256),
	g722-56k	INTEGER (1..256),
	g722-48k	INTEGER (1..256),

	g7231		SEQUENCE
	{
		maxAl-sduAudioFrames	INTEGER (1..256),
		silenceSuppression	BOOLEAN
	},
				

	g728		INTEGER (1..256),
	g729		INTEGER (1..256),
	g729AnnexA	INTEGER (1..256),
	is11172AudioCapability	IS11172AudioCapability,
	is13818AudioCapability	IS13818AudioCapability,
	 ...,
	g729AnnexAwSilenceSuppression	INTEGER(1..256)
}

IS11172AudioCapability	::= SEQUENCE
{
	audioLayer1	BOOLEAN,
	audioLayer2	BOOLEAN,
	audioLayer3	BOOLEAN,

	audioSampling32k	BOOLEAN,
	audioSampling44k1	BOOLEAN,
	audioSampling48k	BOOLEAN,

	singleChannel	BOOLEAN,
	twoChannels	BOOLEAN,

	bitRate	INTEGER (1..448),	
	...
}

IS13818AudioCapability	::= SEQUENCE
{
	audioLayer1	BOOLEAN,
	audioLayer2	BOOLEAN,
	audioLayer3	BOOLEAN,

	audioSampling16k	BOOLEAN,
	audioSampling22k05	BOOLEAN,
	audioSampling24k	BOOLEAN,
	audioSampling32k	BOOLEAN,
	audioSampling44k1	BOOLEAN,
	audioSampling48k	BOOLEAN,

	singleChannel	BOOLEAN,
	twoChannels	BOOLEAN,
	threeChannels2-1	BOOLEAN,
	threeChannels3-0	BOOLEAN,
	fourChannels2-0-2-0	BOOLEAN,
	fourChannels2-2	BOOLEAN,
	fourChannels3-1	BOOLEAN,
	fiveChannels3-0-2-0	BOOLEAN,
	fiveChannels3-2	BOOLEAN,

	lowFrequencyEnhancement	BOOLEAN,

	multilingual	BOOLEAN,

	bitRate	INTEGER (1..1130),	
	...
}





DataApplicationCapability	::= SEQUENCE
{
	application	CHOICE
	{
		nonStandard	NonStandardParameter ,
		t120	DataProtocolCapability,
		dsm-cc	DataProtocolCapability,
		userData	DataProtocolCapability,
		t84	SEQUENCE
		{
			t84Protocol	DataProtocolCapability,
			t84Profile	T84Profile
		},
		t434	DataProtocolCapability,
		h224	DataProtocolCapability,
		nlpid	SEQUENCE
		{
			nlpidProtocol	DataProtocolCapability,
			nlpidData	OCTET STRING
		},
		dsvdControl	NULL,
		h222DataPartitioning	DataProtocolCapability,
		...
	},
	maxBitRate	INTEGER (0..4294967295),	
	...
}

DataProtocolCapability	::= CHOICE
{
	nonStandard	NonStandardParameter ,
	v14buffered	NULL,
	v42lapm	NULL,	
	hdlcFrameTunnelling	NULL,
	h310SeparateVCStack	NULL,
	h310SingleVCStack	NULL,
	transparent	NULL,
	...,
	segmentationAndReassembly	NULL,
	hdlcFrameTunnelingwSAR	NULL,
	v120		NULL,	
	separateLANStack	NULL

}

T84Profile		::= CHOICE
{
	t84Unrestricted	NULL,
	t84Restricted	SEQUENCE
	{
		qcif	BOOLEAN,
		cif		BOOLEAN,
		ccir601Seq	BOOLEAN,
		ccir601Prog	BOOLEAN,
		hdtvSeq	BOOLEAN,
		hdtvProg	BOOLEAN,

		g3FacsMH200x100	BOOLEAN,
		g3FacsMH200x200	BOOLEAN,
		g4FacsMMR200x100	BOOLEAN,
		g4FacsMMR200x200	BOOLEAN,
		jbig200x200Seq	BOOLEAN,
		jbig200x200Prog	BOOLEAN,
		jbig300x300Seq	BOOLEAN,
		jbig300x300Prog	BOOLEAN,

		digPhotoLow	BOOLEAN,
		digPhotoMedSeq	BOOLEAN,
		digPhotoMedProg	BOOLEAN,
		digPhotoHighSeq	BOOLEAN,
		digPhotoHighProg	BOOLEAN,

		...
	}
}




ConferenceCapability	::= SEQUENCE
{
	nonStandardData	SEQUENCE OF NonStandardParameter OPTIONAL,
	chairControlCapability	BOOLEAN,
	...
}











OpenLogicalChannel	::= SEQUENCE
{
	forwardLogicalChannelNumber	LogicalChannelNumber,

	forwardLogicalChannelParameters	SEQUENCE
	{
		portNumber	INTEGER (0..65535)  OPTIONAL,
		dataType	DataType ,
		multiplexParameters	CHOICE
		{
			h222LogicalChannelParameters	H222LogicalChannelParameters ,
			h223LogicalChannelParameters	H223LogicalChannelParameters ,
			v76LogicalChannelParameters V76LogicalChannelParameters ,
			...,
			h2250LogicalChannelParameters H2250LogicalChannelParameters ,
						
			h223AnnexALogicalChannelParameters H223AnnexALogicalChannelParameters 
		},
		...
	},

	

	reverseLogicalChannelParameters	SEQUENCE
	{
		dataType	DataType ,
		multiplexParameters	CHOICE
		{
			
			h223LogicalChannelParameters	H223LogicalChannelParameters ,
			v76LogicalChannelParameters	V76LogicalChannelParameters ,
			...,
			h2250LogicalChannelParameters H2250LogicalChannelParameters ,
						
			h223AnnexALogicalChannelParameters H223AnnexALogicalChannelParameters 

		}  OPTIONAL,	
		...
	}  OPTIONAL,	
	...,
	separateStack	NetworkAccessParameters  OPTIONAL 
				

}

LogicalChannelNumber	::=INTEGER (1..65535)

NetworkAccessParameters	::= SEQUENCE
{
	distribution	CHOICE
	{
		unicast	NULL,
		multicast	NULL,	
		...
	}  OPTIONAL,

	networkAddress	CHOICE
	{
		q2931Address	Q2931Address,
 		e164Address	IA5String(SIZE(1..128)) (FROM ("0123456789#*,")),
		localAreaAddress	TransportAddress ,
		...
	},
	associateConference	BOOLEAN,
	externalReference	OCTET STRING(SIZE(1..255))  OPTIONAL,
	...
}

Q2931Address	::= SEQUENCE
{
	address	CHOICE
	{
		internationalNumber	NumericString(SIZE(1..16)),
		nsapAddress	OCTET STRING (SIZE(1..20)),
		...
	},
	subaddress	OCTET STRING (SIZE(1..20)) OPTIONAL,
	...
}

V75Parameters	::= SEQUENCE
{
	audioHeaderPresent	BOOLEAN,
	 ...
}	


DataType		::= CHOICE
{
	nonStandard	NonStandardParameter ,
	nullData	NULL,
	videoData	VideoCapability ,	
	audioData	AudioCapability ,
	data		DataApplicationCapability ,
	encryptionData	EncryptionMode,
	...
}


H222LogicalChannelParameters	::= SEQUENCE
{
	resourceID	INTEGER (0..65535),
	subChannelID	INTEGER (0..8191),
	pcr-pid	INTEGER (0..8191)  OPTIONAL,
	programDescriptors	OCTET STRING  OPTIONAL,
	streamDescriptors	OCTET STRING  OPTIONAL,
	...
}

H223LogicalChannelParameters	::= SEQUENCE
{
	adaptationLayerType	CHOICE
	{
		nonStandard	NonStandardParameter ,
		al1Framed	NULL,
		al1NotFramed	NULL,
		al2WithoutSequenceNumbers	NULL,
		al2WithSequenceNumbers	NULL,
		al3		SEQUENCE
		{
			controlFieldOctets	INTEGER (0..2),
			sendBufferSize	INTEGER (0..16777215)	
		},

		...
	},

	segmentableFlag	BOOLEAN,
	...
}	

V76LogicalChannelParameters	::= SEQUENCE
{
	hdlcParameters	V76HDLCParameters,
 	suspendResume	CHOICE
	{ 
		wAddress	NULL,
		woAddress	NULL,
		...
	},
	uIH			BOOLEAN,
	mode		CHOICE
	{
		eRM	SEQUENCE
		{
			windowSize	INTEGER (1..127) ,
			recovery	CHOICE
			{
				rej	NULL,
				sREJ	NULL,
				mSREJ	NULL,
				...
			},
			...
		},
	 	uNERM	NULL,
		...
	},
	v75Parameters	V75Parameters,
	...
}



V76HDLCParameters	::= SEQUENCE
{
	crcLength	CRCLength,
	n401		INTEGER (1..127),
	loopbackTestProcedure	BOOLEAN,
		...
}


CRCLength	::= CHOICE
{
	crc8bit		NULL,
	crc16bit	NULL,
	crc32bit	NULL,
	...
}

H2250LogicalChannelParameters	::= SEQUENCE
{
	nonStandard	SEQUENCE OF NonStandardParameter OPTIONAL,
	sessionID	INTEGER(0..255),
	associatedSessionID	INTEGER(1..255)  OPTIONAL,
	mediaChannel	TransportAddress  OPTIONAL,
	mediaGuaranteedDelivery	BOOLEAN  OPTIONAL,
	mediaControlChannel	TransportAddress  OPTIONAL,  
	mediaControlGuaranteedDelivery	BOOLEAN  OPTIONAL,
	silenceSuppression	BOOLEAN  OPTIONAL,
	destination	TerminalLabel  OPTIONAL,
	
	dynamicRTPPayloadType	INTEGER(96..127)  OPTIONAL,
	mediaPacketization	CHOICE
	{
		h261aVideoPacketization	NULL,
		...
	}  OPTIONAL,
	...
}


TransportAddress	::= CHOICE
{
	unicastAddress	UnicastAddress,
	multicastAddress	MulticastAddress,
	...
}

UnicastAddress	::= CHOICE
{
	iPAddress	SEQUENCE
	{
		network	OCTET STRING (SIZE(4)),
		tsapIdentifier	INTEGER(0..65535),
		...
	},
	iPXAddress	SEQUENCE
	{
		node	OCTET STRING (SIZE(6)),
		netnum	OCTET STRING (SIZE(4)),
		tsapIdentifier	OCTET STRING (SIZE(2)),
		...
	},
	iP6Address	SEQUENCE
	{
		network	OCTET STRING (SIZE(16)),
		tsapIdentifier	INTEGER(0..65535), 
		...
	},
	netBios	OCTET STRING (SIZE(16)),
	iPSourceRouteAddress	SEQUENCE
	{
		routing	CHOICE
		{
			strict	NULL,
			loose	NULL
		},
		network	OCTET STRING (SIZE(4)),
		tsapIdentifier	INTEGER(0..65535),
		route	SEQUENCE OF OCTET STRING (SIZE(4)),
		...
	},
	...,
	nsap			OCTET STRING (SIZE(1..20)),
	nonStandardAddress NonStandardParameter 
}

MulticastAddress	::= CHOICE
{
	iPAddress	SEQUENCE
	{
		network	OCTET STRING (SIZE(4)),
		tsapIdentifier	INTEGER(0..65535),
		...
	},
	iP6Address	SEQUENCE
	{
		network	OCTET STRING (SIZE(16)),
		tsapIdentifier	INTEGER(0..65535), 
		...
	},
	...,
	nsap			OCTET STRING (SIZE(1..20)),
	nonStandardAddress NonStandardParameter 
}

H223AnnexALogicalChannelParameters	::= SEQUENCE
{
	adaptationLayertype	CHOICE
	{
		nonStandard	NonStandardParameter ,
		al1M	AL1MParameters,
		al2M	NULL,	
		al3M	AL3MParameters,
		...
	},
	segmentableFlag	BOOLEAN,
	...
}

OpenLogicalChannelAck	::= SEQUENCE
{
	forwardLogicalChannelNumber	LogicalChannelNumber,

	reverseLogicalChannelParameters	SEQUENCE
	{
		reverseLogicalChannelNumber	LogicalChannelNumber,
		portNumber	INTEGER (0..65535)  OPTIONAL,
		multiplexParameters	CHOICE
		{
			h222LogicalChannelParameters	H222LogicalChannelParameters ,
			
			...,
			h2250LogicalChannelParameters H2250LogicalChannelParameters 

		}  OPTIONAL,	
		...

	}  OPTIONAL,	
	...,
	separateStack	NetworkAccessParameters  OPTIONAL, 
				
	forwardMultiplexAckParameters	CHOICE
	{
		
		
		
		h2250LogicalChannelAckParameters H2250LogicalChannelAckParameters ,
		...
	}  OPTIONAL

}

OpenLogicalChannelReject	::= SEQUENCE
{
	forwardLogicalChannelNumber	LogicalChannelNumber,
	cause		CHOICE
	{
		unspecified	NULL,
		unsuitableReverseParameters	NULL,
		dataTypeNotSupported	NULL,
		dataTypeNotAvailable	NULL,
		unknownDataType	NULL,
		dataTypeALCombinationNotSupported	NULL,
		...,
		multicastChannelNotAllowed	NULL,
		insufficientBandwidth	NULL,
		separateStackEstablishmentFailed	NULL,
		invalidSessionID	 NULL,
		masterSlaveConflict	NULL
	},
	...
}

OpenLogicalChannelConfirm	::= SEQUENCE
{
	forwardLogicalChannelNumber	LogicalChannelNumber,
	...
}

H2250LogicalChannelAckParameters	::= SEQUENCE
{
	nonStandard	SEQUENCE OF NonStandardParameter  OPTIONAL,
	sessionID	INTEGER(1..255)  OPTIONAL,
	mediaChannel	TransportAddress  OPTIONAL,
	mediaControlChannel	TransportAddress  OPTIONAL, 
	dynamicRTPPayloadType	INTEGER(96..127)  OPTIONAL, 
						
	...
}



CloseLogicalChannel	::= SEQUENCE
{
	forwardLogicalChannelNumber	LogicalChannelNumber,
	source		CHOICE
	{
		user	NULL,
		lcse	NULL
	},
	...
}

CloseLogicalChannelAck	::= SEQUENCE
{
	forwardLogicalChannelNumber	LogicalChannelNumber,
	...
}

RequestChannelClose	::= SEQUENCE
{
	forwardLogicalChannelNumber	LogicalChannelNumber,
	...
}

RequestChannelCloseAck	::= SEQUENCE
{
	forwardLogicalChannelNumber	LogicalChannelNumber,
	...
}

RequestChannelCloseReject	::= SEQUENCE
{
	forwardLogicalChannelNumber	LogicalChannelNumber,
	cause		CHOICE
	{
		unspecified	NULL,
		...
	},
	...
}

RequestChannelCloseRelease	::= SEQUENCE
{
	forwardLogicalChannelNumber	LogicalChannelNumber,
	...
}







MultiplexEntrySend	::= SEQUENCE
{
	sequenceNumber	SequenceNumber,
	multiplexEntryDescriptors	SET SIZE (1..15) OF MultiplexEntryDescriptor,
	...
}

MultiplexEntryDescriptor	::= SEQUENCE
{
	multiplexTableEntryNumber	MultiplexTableEntryNumber,
	elementList	SEQUENCE SIZE (1..256) OF MultiplexElement OPTIONAL
}

MultiplexElement	::= SEQUENCE
{
	type		CHOICE
	{
		logicalChannelNumber	INTEGER(0..65535),
		subElementList	SEQUENCE SIZE (2..255) OF MultiplexElement
	},
	repeatCount	CHOICE
	{
		finite	INTEGER (1..65535),	
		untilClosingFlag	NULL	
	}
}

MultiplexTableEntryNumber	::=INTEGER (1..15)

MultiplexEntrySendAck	::= SEQUENCE
{
	sequenceNumber	SequenceNumber,
	multiplexTableEntryNumber	SET SIZE (1..15) OF MultiplexTableEntryNumber,
	...
}

MultiplexEntrySendReject	::= SEQUENCE
{
	sequenceNumber	SequenceNumber,
	rejectionDescriptions	SET SIZE (1..15) OF MultiplexEntryRejectionDescriptions,
	...
}

MultiplexEntryRejectionDescriptions	::= SEQUENCE
{
	multiplexTableEntryNumber	MultiplexTableEntryNumber,
	cause		CHOICE
	{
		unspecifiedCause	NULL,
		descriptorTooComplex	NULL,
		...
	},
	...
}

MultiplexEntrySendRelease	::= SEQUENCE
{
	multiplexTableEntryNumber	SET SIZE (1..15) OF MultiplexTableEntryNumber,
	...
}

RequestMultiplexEntry	::= SEQUENCE
{
	entryNumbers	SET SIZE (1..15) OF MultiplexTableEntryNumber,
	...
}

RequestMultiplexEntryAck	::= SEQUENCE
{
	entryNumbers	SET SIZE (1..15) OF MultiplexTableEntryNumber,
	...
}

RequestMultiplexEntryReject	::= SEQUENCE
{
	entryNumbers	SET SIZE (1..15) OF MultiplexTableEntryNumber,
	rejectionDescriptions	SET SIZE (1..15) OF RequestMultiplexEntryRejectionDescriptions,
	...
}

RequestMultiplexEntryRejectionDescriptions	::= SEQUENCE
{
	multiplexTableEntryNumber	MultiplexTableEntryNumber,
	cause		CHOICE
	{
		unspecifiedCause	NULL,
		...
	},
	...
}

RequestMultiplexEntryRelease	::= SEQUENCE
{
	entryNumbers	SET SIZE (1..15) OF MultiplexTableEntryNumber,

	...
}










RequestMode	::= SEQUENCE
{
	sequenceNumber	SequenceNumber,
	requestedModes	SEQUENCE SIZE (1..256) OF ModeDescription,
	...
}

RequestModeAck	::= SEQUENCE
{
	sequenceNumber	SequenceNumber,
	response	CHOICE
	{
		willTransmitMostPreferredMode	NULL,
		willTransmitLessPreferredMode	NULL,
		...
	},
	...
}

RequestModeReject	::= SEQUENCE
{
	sequenceNumber	SequenceNumber,
	cause		CHOICE
	{
		modeUnavailable	NULL,
		multipointConstraint	NULL,
		requestDenied	NULL,
		...
	},
	...
}

RequestModeRelease	::= SEQUENCE
{
	...
}





ModeDescription	::= SET SIZE (1..256) OF ModeElement

ModeElement	::= SEQUENCE
{
	type		CHOICE
	{
		nonStandard	NonStandardParameter ,
		videoMode	VideoMode ,
		audioMode	AudioMode ,
		dataMode	DataMode ,
		encryptionMode	EncryptionMode,
		...
	},

	h223ModeParameters	H223ModeParameters  OPTIONAL,
	...,
	v76ModeParameters	V76ModeParameters  OPTIONAL,
							 
	h223AnnexAModeParameters	H223AnnexAModeParameters  OPTIONAL 
							
							
	
}

H223ModeParameters	::= SEQUENCE
{
	adaptationLayerType	CHOICE
	{
		nonStandard	NonStandardParameter ,
		al1Framed	NULL,
		al1NotFramed	NULL,
		al2WithoutSequenceNumbers	NULL,
		al2WithSequenceNumbers	NULL,
		al3		SEQUENCE
		{
			controlFieldOctets	INTEGER(0..2),
			sendBufferSize	INTEGER(0..16777215)	
		},

		...
	},

	segmentableFlag	BOOLEAN,
	...
}

 H223AnnexAModeParameters	::= SEQUENCE
{
	adaptationLayertype	CHOICE
	{
		nonStandard	NonStandardParameter,

		al1M	AL1MParameters,
		al2M	NULL,	
		al3M	AL3MParameters,
		...
        },
 
        segmentableFlag	BOOLEAN,
        ...
}

AL1MParameters	::= SEQUENCE
{
	crcLength	CHOICE
	{
		nil		NULL,
		eightbits	NULL,
		sixteenbits	NULL,
		thirtytwobits	NULL,
		...
	},
	targetCodeRate	INTEGER (0..24),	
	arqType	CHOICE
	{
		noArq	NULL,
		typeIArq	NULL,
		typeIIArq	NULL,
		...
	},
	numberOfRetransmissions	CHOICE
	{
		finite	INTEGER (0..16),
		infinite	NULL,
		...
	},
	sendBufferSize	INTEGER (0..16777215),
	...
}

AL3MParameters	::= SEQUENCE
{
	crcLength	CHOICE
	{
		nil	NULL,
		eightbits	NULL,
		sixteenbits	NULL,
		thirtytwobits	NULL,
		...
	},
	targetCodeRate	INTEGER (0..24),	
	arqType	CHOICE
	{
		noArq	NULL,
		typeIArq	NULL,
		typeIIArq	NULL,
		...
	},
	numberOfRetransmissions	INTEGER (0..16),
	sendBufferSize	INTEGER (0..16777215),
	...
}

V76ModeParameters	::= CHOICE
{
	suspendResumewAddress	NULL,
	suspendResumewoAddress	NULL,
	...
}






VideoMode		::= CHOICE
{
	nonStandard	NonStandardParameter ,
	h261VideoMode	H261VideoMode,
	h262VideoMode	H262VideoMode,
	h263VideoMode	H263VideoMode,
	is11172VideoMode	IS11172VideoMode,
	...
}

H261VideoMode	::= SEQUENCE
{
	resolution	CHOICE
	{
		qcif	NULL,
		cif		NULL
	},
	bitRate	INTEGER (1..19200),	
	stillImageTransmission	BOOLEAN,
	...
}

H262VideoMode	::= SEQUENCE
{
	profileAndLevel	CHOICE
	{
		profileAndLevel-SPatML	NULL,
		profileAndLevel-MPatLL	NULL,
		profileAndLevel-MPatML	NULL,
		profileAndLevel-MPatH-14	NULL,
		profileAndLevel-MPatHL	NULL,
		profileAndLevel-SNRatLL	NULL,
		profileAndLevel-SNRatML	NULL,
		profileAndLevel-SpatialatH-14	NULL,
		profileAndLevel-HPatML	NULL,
		profileAndLevel-HPatH-14	NULL,
		profileAndLevel-HPatHL	NULL,
		...
	},
	videoBitRate	INTEGER(0..1073741823)  OPTIONAL,	
	vbvBufferSize	INTEGER(0..262143)  OPTIONAL,	
	samplesPerLine	INTEGER(0..16383)  OPTIONAL,	
	linesPerFrame	INTEGER(0..16383)  OPTIONAL,	
	framesPerSecond	INTEGER(0..15)  OPTIONAL,	
	luminanceSampleRate	INTEGER(0..4294967295)  OPTIONAL,	
	...
}

H263VideoMode	::= SEQUENCE
{
	resolution	CHOICE
	{
		sqcif	NULL,	
		qcif	NULL,
		cif		NULL,
		cif4	NULL,
		cif16	NULL,
		...
	},
	bitRate	INTEGER (1..19200),	
	unrestrictedVector	BOOLEAN,
	arithmeticCoding	BOOLEAN,
	advancedPrediction	BOOLEAN,
	pbFrames	BOOLEAN,
	...,
								
	errorCompensation	BOOLEAN

}

IS11172VideoMode	::= SEQUENCE
{
	constrainedBitstream	BOOLEAN,
	videoBitRate	INTEGER(0..1073741823)  OPTIONAL,	
	vbvBufferSize	INTEGER(0..262143)  OPTIONAL,	
	samplesPerLine	INTEGER(0..16383)  OPTIONAL,	
	linesPerFrame	INTEGER(0..16383)  OPTIONAL,	
	pictureRate	INTEGER(0..15)  OPTIONAL,
	luminanceSampleRate	INTEGER(0..4294967295)  OPTIONAL,	
	...
}





AudioMode	::= CHOICE
{
	nonStandard	NonStandardParameter ,
	g711Alaw64k	NULL,
	g711Alaw56k	NULL,
	g711Ulaw64k	NULL,
	g711Ulaw56k	NULL,

	g722-64k	NULL,
	g722-56k	NULL,
	g722-48k	NULL,

	g728		NULL,
	g729		NULL,
	g729AnnexA	NULL,

	g7231		CHOICE
	{
		noSilenceSuppressionLowRate	NULL,
		noSilenceSuppressionHighRate	NULL,
		silenceSuppressionLowRate	NULL,
		silenceSuppressionHighRate	NULL
	},

	is11172AudioMode	IS11172AudioMode,
	is13818AudioMode	IS13818AudioMode,

	...,
	g729AnnexAwSilenceSuppression	NULL

}

IS11172AudioMode	::= SEQUENCE
{
	audioLayer	CHOICE
	{
		audioLayer1	NULL,
		audioLayer2	NULL,
		audioLayer3	NULL
	},

	audioSampling	CHOICE
	{
		audioSampling32k	NULL,
		audioSampling44k1	NULL,
		audioSampling48k	NULL
	},

	multichannelType	CHOICE
	{
		singleChannel	NULL,
		twoChannelStereo	NULL,
		twoChannelDual	NULL
	},

	bitRate	INTEGER (1..448),	
	...
}

IS13818AudioMode	::= SEQUENCE
{
	audioLayer	CHOICE
	{
		audioLayer1	NULL,
		audioLayer2	NULL,
		audioLayer3	NULL
	},

	audioSampling	CHOICE
	{
		audioSampling16k	NULL,
		audioSampling22k05	NULL,
		audioSampling24k	NULL,
		audioSampling32k	NULL,
		audioSampling44k1	NULL,
		audioSampling48k	NULL
	},

	multichannelType	CHOICE
	{
		singleChannel	NULL,
		twoChannelStereo	NULL,
		twoChannelDual	NULL,
		threeChannels2-1	NULL,
		threeChannels3-0	NULL,
		fourChannels2-0-2-0	NULL,
		fourChannels2-2	NULL,
		fourChannels3-1	NULL,
		fiveChannels3-0-2-0	NULL,
		fiveChannels3-2	NULL
	},

	lowFrequencyEnhancement	BOOLEAN,

	multilingual	BOOLEAN,

	bitRate	INTEGER (1..1130),	
	...
}





DataMode		::= SEQUENCE
{
	application	CHOICE
	{
		nonStandard	NonStandardParameter ,
		t120	DataProtocolCapability,
		dsm-cc	DataProtocolCapability,
		userData	DataProtocolCapability,
		t84	DataProtocolCapability,
		t434	DataProtocolCapability,
		h224	DataProtocolCapability,
		nlpid	SEQUENCE
		{
			nlpidProtocol	DataProtocolCapability,
			nlpidData	OCTET STRING
		},
		dsvdControl	NULL,
		h222DataPartitioning	DataProtocolCapability,
		...
	},
	bitRate	INTEGER (0..4294967295),	
	...
}





EncryptionMode	::= CHOICE
{
	nonStandard	NonStandardParameter ,
	h233Encryption	NULL,
	...
}







RoundTripDelayRequest	::= SEQUENCE
{
	sequenceNumber	SequenceNumber,
	...
}

RoundTripDelayResponse	::= SEQUENCE
{
	sequenceNumber	SequenceNumber,
	...
}





MaintenanceLoopRequest	::= SEQUENCE
{
	type		CHOICE
	{
		systemLoop	NULL,
		mediaLoop	LogicalChannelNumber,
		logicalChannelLoop	LogicalChannelNumber,
		...
	},
	...
}

MaintenanceLoopAck	::= SEQUENCE
{
	type		CHOICE
	{
		systemLoop	NULL,
		mediaLoop	LogicalChannelNumber,
		logicalChannelLoop	LogicalChannelNumber,
		...
	},
	...
}

MaintenanceLoopReject	::= SEQUENCE
{
	type		CHOICE
	{
		systemLoop	NULL,
		mediaLoop	LogicalChannelNumber,
		logicalChannelLoop	LogicalChannelNumber,
		...
	},
	cause		CHOICE
	{
		canNotPerformLoop	NULL,
		...
	},
	...
}

MaintenanceLoopOffCommand	::= SEQUENCE
{
	...
}




CommunicationModeCommand	::= SEQUENCE
{
	communicationModeTable	SET SIZE(1..256) OF CommunicationModeTableEntry,
	...
}

CommunicationModeRequest	::= SEQUENCE
{
	...
}

CommunicationModeResponse	::= CHOICE
{
	communicationModeTable	SET SIZE(1..256) OF CommunicationModeTableEntry,
	...
}

CommunicationModeTableEntry	::= SEQUENCE
{
	nonStandard	SEQUENCE OF NonStandardParameter OPTIONAL,
	sessionID	INTEGER(1..255),
	associatedSessionID	INTEGER(1..255)  OPTIONAL,

	terminalLabel	TerminalLabel   OPTIONAL, 
					
					
	sessionDescription	BMPString (SIZE(1..128)) , 
	dataType	CHOICE
	{
		videoData	VideoCapability ,
		audioData	AudioCapability ,
		data	DataApplicationCapability ,
		...
	},
	mediaChannel	TransportAddress  OPTIONAL,
	mediaGuaranteedDelivery	BOOLEAN  OPTIONAL,
	mediaControlChannel	TransportAddress  OPTIONAL,  
	mediaControlGuaranteedDelivery	BOOLEAN  OPTIONAL,
	...
}





ConferenceRequest	::= CHOICE
{
	
	terminalListRequest	NULL,                    

	makeMeChair	NULL,                    
	cancelMakeMeChair	NULL,                    

	dropTerminal	TerminalLabel ,       

	requestTerminalID	TerminalLabel ,       

	enterH243Password	NULL,                    
	enterH243TerminalID	NULL,                    
						
	enterH243ConferenceID	NULL,                    
	...
}

TerminalLabel	::= SEQUENCE
{
		mcuNumber	McuNumber,
		terminalNumber	TerminalNumber,
		...
}

McuNumber	::=INTEGER(0..192)
TerminalNumber	::=INTEGER(0..192)




ConferenceResponse	::= CHOICE
{
	mCTerminalIDResponse	SEQUENCE             
	{				
		terminalLabel	TerminalLabel ,
		terminalID	TerminalID,
		...
	},

	terminalIDResponse	SEQUENCE	
	{				
		terminalLabel	TerminalLabel ,	
		terminalID	TerminalID,
		...
	},

	conferenceIDResponse	SEQUENCE	
	{				
		terminalLabel	TerminalLabel ,	
		conferenceID	ConferenceID,
		...
	},

	passwordResponse	SEQUENCE	
	{				
		terminalLabel	TerminalLabel ,	
		password	Password,
		...
	},

	terminalListResponse	SET SIZE (1..256) OF TerminalLabel,

	videoCommandReject	NULL,	
	terminalDropReject	NULL,	

	makeMeChairResponse	CHOICE	
	{
		grantedChairToken	NULL,	
		deniedChairToken	NULL,	
		...
	},
	...
}

TerminalID 	::=OCTET STRING  (SIZE(1..128))	
ConferenceID	::=OCTET STRING  (SIZE(1..32))
Password		::=OCTET STRING  (SIZE(1..32))




H223AnnexAReconfiguration	::= SEQUENCE
{
	synchFlagLength	CHOICE
	{
		length15	NULL,
		length31	NULL,
		...
	},
	informationFieldSize	INTEGER (0..65535),	
	headerInterleaving 	CHOICE
	{
		bitInterleaving 	NULL,
		byteInterleaving 	NULL,
		noInterleaving 	NULL,
		...
	},
	headerFEC	CHOICE
	{
		rate5by15	NULL,
		rate6by31	NULL,
		rate7by63	NULL,
		rate7by15	NULL,
		rate10by63	NULL,
		rate11by31	NULL,
		rate11by15	NULL,
		rate16by63	NULL,
		rate16by31	NULL,
		rate18by63	NULL,
		...
	},
		
	headerCRC	CHOICE
	{
		nil	NULL,
		threebits	NULL,
		eightbits	NULL,
		...
	},
	headerCounterForward	INTEGER (0..14),
	...
}





H223AnnexAReconfigurationAck	::= SEQUENCE
{
	...
}
 
H223AnnexAReconfigurationReject	::= SEQUENCE
{
	...
}











SendTerminalCapabilitySet	::= CHOICE
{
	specificRequest	SEQUENCE
	{
		multiplexCapability	BOOLEAN,

		capabilityTableEntryNumbers	SET SIZE (1..65535) OF CapabilityTableEntryNumber OPTIONAL,

		capabilityDescriptorNumbers	SET SIZE (1..256) OF CapabilityDescriptorNumber OPTIONAL,
		...
	},
	genericRequest	NULL,
	...
}





EncryptionCommand	::= CHOICE
{
	encryptionSE	OCTET STRING,	
	encryptionIVRequest	NULL,	
	encryptionAlgorithmID	SEQUENCE
	{
		h233AlgorithmIdentifier	SequenceNumber,
		associatedAlgorithm	NonStandardParameter 
	},
	...
}





FlowControlCommand	::= SEQUENCE
{
	scope		CHOICE
	{
		logicalChannelNumber	LogicalChannelNumber,
		resourceID	INTEGER (0..65535),
		wholeMultiplex	NULL
	},
	restriction	CHOICE
	{
		maximumBitRate	INTEGER (0..16777215),	
		noRestriction	NULL
	},
	...
}





EndSessionCommand	::= CHOICE
{
	nonStandard	NonStandardParameter ,

	disconnect	NULL,

	gstnOptions	CHOICE
	{
		telephonyMode	NULL,
		v8bis	NULL,
		v34DSVD	NULL,
		v34DuplexFAX	NULL,
		v34H324	NULL,
		...
	},

	...
}





ConferenceCommand	::= CHOICE
{
	broadcastMyLogicalChannel	LogicalChannelNumber,	
	cancelBroadcastMyLogicalChannel	LogicalChannelNumber, 

	makeTerminalBroadcaster	TerminalLabel ,	
	cancelMakeTerminalBroadcaster	NULL,	
		
	sendThisSource	TerminalLabel ,	
	cancelSendThisSource	NULL,	

	dropConference	NULL,	
	...
}






MiscellaneousCommand	::= SEQUENCE
{
	logicalChannelNumber	LogicalChannelNumber,
	type		CHOICE
	{
		equaliseDelay	NULL,	
		zeroDelay	NULL,	
		multipointModeCommand	NULL,
		cancelMultipointModeCommand	NULL,
		videoFreezePicture	NULL,
		videoFastUpdatePicture	NULL,

		videoFastUpdateGOB	SEQUENCE
		{
			firstGOB	INTEGER (0..17),
			numberOfGOBs	INTEGER (1..18)
		},

		videoTemporalSpatialTradeOff	INTEGER (0..31),	

		videoSendSyncEveryGOB	NULL,
		videoSendSyncEveryGOBCancel	NULL,

		...,
		videoFastUpdateMB	SEQUENCE
		{
			firstGOB	INTEGER (0..255)  OPTIONAL,	
			firstMB	INTEGER (1..8192)  OPTIONAL,
			numberOfMBs	INTEGER (1..8192),
			...
		}

	},

	...
}













FunctionNotUnderstood	::= CHOICE
{
	request	RequestMessage ,
	response	ResponseMessage ,
	command	CommandMessage 
}







FunctionNotSupported	::= SEQUENCE
{
	cause		CHOICE
	{
		syntaxError	NULL,
		semanticError	NULL,
		unknownFunction	NULL,
		...
	},
	returnedFunction	OCTET STRING  OPTIONAL,
	...
}





ConferenceIndication	::= CHOICE
{
	sbeNumber		INTEGER (0..9),	
		
	terminalNumberAssign		TerminalLabel ,	

	terminalJoinedConference		TerminalLabel ,	

	terminalLeftConference		TerminalLabel ,	

	seenByAtLeastOneOther		NULL,	
	cancelSeenByAtLeastOneOther		NULL,	

	seenByAll		NULL,	
	cancelSeenByAll		NULL,	

	terminalYouAreSeeing		TerminalLabel ,	

	requestForFloor	NULL,	

	...
}






MiscellaneousIndication	::= SEQUENCE
{
	logicalChannelNumber	LogicalChannelNumber,
	type		CHOICE
	{
		logicalChannelActive	NULL,	
		logicalChannelInactive	NULL,	

		multipointConference	NULL,	
		cancelMultipointConference	NULL,	

		multipointZeroComm	NULL,	
		cancelMultipointZeroComm	NULL,	

		multipointSecondaryStatus	NULL,	
		cancelMultipointSecondaryStatus	NULL,	

		videoIndicateReadyToActivate	NULL,	

		videoTemporalSpatialTradeOff	INTEGER (0..31),	

		...,
						
		videoNotDecodedMBs	SEQUENCE
	      {
	          firstMB	INTEGER (1..6336),
	          numberOfMBs	INTEGER (1..6336),
	          temporalReference	INTEGER (0..255),
			...
	      }

	},
	...
}





JitterIndication	::= SEQUENCE
{
	scope		CHOICE
	{
		logicalChannelNumber	LogicalChannelNumber,
		resourceID	INTEGER (0..65535),
		wholeMultiplex	NULL
	},
	estimatedReceivedJitterMantissa	INTEGER (0..3),
	estimatedReceivedJitterExponent	INTEGER (0..7),
	skippedFrameCount	INTEGER (0..15)  OPTIONAL,
	additionalDecoderBuffer	INTEGER (0..262143)  OPTIONAL,	
	...
}





H223SkewIndication	::= SEQUENCE
{
	logicalChannelNumber1	LogicalChannelNumber,
	logicalChannelNumber2	LogicalChannelNumber,
	skew		INTEGER (0..4095),	
	...
}





H2250MaximumSkewIndication	::= SEQUENCE
{
	logicalChannelNumber1	LogicalChannelNumber,
	logicalChannelNumber2	LogicalChannelNumber,
	maximumSkew	INTEGER (0..4095),	
	...
}





MCLocationIndication	::= SEQUENCE
{
	signalAddress	TransportAddress , 
					
					
	...
}





VendorIdentification		::= SEQUENCE
{
	vendor				NonStandardIdentifier,
	productNumber		OCTET STRING (SIZE(1..256))  OPTIONAL,  
	versionNumber		OCTET STRING (SIZE(1..256))  OPTIONAL,  
	...
}






NewATMVCIndication	::= SEQUENCE
{
	resourceID	INTEGER(0..65535),
	bitRate	INTEGER(1..65535),	
	bitRateLockedToPCRClock	BOOLEAN,
	bitRateLockedToNetworkClock	BOOLEAN,
	aal			CHOICE
	{
		aal1	SEQUENCE
		{
			clockRecovery	CHOICE
			{
				nullClockRecovery	NULL,
				srtsClockRecovery	NULL,
				adaptiveClockRecovery	NULL,
				...
			},
			errorCorrection	CHOICE
			{
				nullErrorCorrection	NULL,
				longInterleaver	NULL,
				shortInterleaver	NULL,
				errorCorrectionOnly	NULL,
				...
			},
			structuredDataTransfer	BOOLEAN,
			partiallyFilledCells	BOOLEAN,
			...
		},
		aal5	SEQUENCE
		{
			forwardMaximumSDUSize	INTEGER (0..65535),	
			backwardMaximumSDUSize	INTEGER (0..65535),	
			...
		},
		...
	},
	multiplex	CHOICE
	{
		noMultiplex	NULL,
		transportStream	NULL,
		programStream	NULL,
		...
	},
	...
}





UserInputIndication	::= CHOICE
{
	nonStandard	NonStandardParameter ,
	alphanumeric	GeneralString,
	...
}


END
