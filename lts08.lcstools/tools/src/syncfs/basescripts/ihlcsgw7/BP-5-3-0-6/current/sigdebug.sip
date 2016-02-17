#Run these sigdebug commands on the active SP, if a fail over occurs run them again on the active SP.
#
#
#These are the selected commands for SIP 
#
#
sigdbg:::::dest-b-ccs-1/4/1 procLogMsgLevel 0x010070ff
sigdbg:::::dest-b-ccs-1/4/1 ccDbg ccDbgMask 0xFFFFFFFF
sigdbg:::::dest-b-ccs-14/4/1 procLogMsgLevel 0x010070ff
sigdbg:::::dest-b-ccs-14/4/1 ccDbg ccDbgMask 0xFFFFFFFF
sigdbg:::::dest-b-ccs-1/4/1 sipw0
sigdbg:::::dest-b-ccs-1/4/1 sipa dumpraw 1
sigdbg:::::dest-b-ccs-1/4/1 sipa dbglvl 0
sigdbg:::::dest-b-ccs-14/4/1 sipw0
sigdbg:::::dest-b-ccs-14/4/1 sipa dumpraw 1
sigdbg:::::dest-b-ccs-14/4/1 sipa dbglvl 0
