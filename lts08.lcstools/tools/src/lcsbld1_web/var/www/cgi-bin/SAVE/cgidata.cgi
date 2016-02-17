# cgi-bin data
#
%filedata = (
	auditsps => {
		pattern => "/lcsl100/tmp/auditsps*",
		ignorelabid => "no",
		hug => "no",
	},
	bbbscripts => {
		pattern => "/lcsl100/tmp/lcsbbbscripts*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
	lcsbu => {
		pattern => "/lcsl100/tmp/bupp*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
	bupp => {
		pattern => "/lcsl100/tmp/bupp*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
	webreload => {
		pattern => "/lcsl100/tmp/bupp*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
	prehug => {
		pattern => "/lcsl100/tmp/*prehug*<labid>*",
		ignorelabid => "no",
		hug => "yes",
	},
	cpuhug => {
		pattern => "/lcsl100/tmp/*cpuhug*<labid>*",
		ignorelabid => "no",
		hug => "yes",
	},
	cpu_cmhug => {
		pattern => "/lcsl100/tmp/*cpu_cmhug*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
	cmhug => {
		pattern => "/lcsl100/tmp/*cmhug*<labid>*",
		ignorelabid => "no",
		hug => "yes",
	},
	iomhug => {
		pattern => "/lcsl100/tmp/*iomhug*<labid>*",
		ignorelabid => "no",
		hug => "yes",
	},
	posthug => {
		pattern => "/lcsl100/tmp/*posthug*<labid>*",
		ignorelabid => "yes",
		hug => "yes",
	},
	lcsreload => {
		pattern => "/lcsl100/logfiles/reload/reload*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
	lcssaveload => {
		pattern => "/lcsl100/tmp/saveload*",
		ignorelabid => "no",
		hug => "no",
	},
	recoversps => {
		pattern => "/lcsl100/tmp/recoversps*",
		ignorelabid => "no",
		hug => "no",
	},
	cpymems => {
		pattern => "/lcsl100/tmp/cpymems*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
	rmvallioms => {
		pattern => "/lcsl100/tmp/rmvioms*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
	rstallioms => {
		pattern => "/lcsl100/tmp/rstioms*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
	updateioms => {
		pattern => "/lcsl100/tmp/updateioms*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
	lcshug => {
		pattern => "/lcsl100/tmp/lcshug*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
	"/lcsl100/tmp log files" => {
		pattern => "/lcsl100/tmp/*",
		ignorelabid => "yes",
		hug => "no",
	},
	"/lcsl100/logfiles/reload log files" => {
		pattern => "/lcsl100/logfiles/reload/*",
		ignorelabid => "yes",
		hug => "no",
	},
	"lcstools log files" => {
		pattern => "/home/lcstools/tools/logfiles/*",
		ignorelabid => "yes",
		hug => "no",
	},
	"Apache log files" => {
		pattern => "/var/apache/logs/*log",
		ignorelabid => "yes",
		hug => "no",
	},
	lcsremoveload => {
		pattern => "/home/lcstools/tools/data/BACKUP/removeload.logfile",
		ignorelabid => "yes",
		hug => "no",
	},
	lcsgench2ioms => {
		pattern => "/lcsl100/tmp/gench2ioms*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
	lcssyncfs => {
		pattern => "/lcsl100/tmp/*syncfs*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
	ftpload => {
		pattern => "/lcsl100/tmp/*ftpload*",
		ignorelabid => "yes",
		hug => "no",
	},
	getsploads => {
		pattern => "/lcsl100/tmp/*getsploads*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
	cleanupsps => {
		pattern => "/lcsl100/tmp/*cleanupsps*<labid>*",
		ignorelabid => "no",
		hug => "no",
	},
);
#
%maintmenu = (
	"LCS Reload and Provision" => {
		handler => "bupp",
		active => "yes",
		selected => "no",
		operation => "bupp",
	},
	"LCS Reboot" => {
		handler => "lcsreboot",
		active => "yes",
		selected => "no",
		operation => "lcsreboot",
	},
	"LCS Status" => {
		handler => "nlcsstat",
		active => "yes",
		selected => "no",
		operation => "nlcsstat",
	},
	"LCS SP Audit" => {
		handler => "lcsspaudit",
		active => "yes",
		selected => "no",
		operation => "lcsspaudit",
	},
	"LCS Picks" => {
		handler => "lcspicks",
		active => "yes",
		selected => "no",
		operation => "lcspicks",
	},
	"Show Locks" => {
		handler => "showlocks",
		active => "yes",
		selected => "no",
		operation => "",
	},
	"Retrieve Equipment Data" => {
		handler => "rtrveqptdata",
		active => "yes",
		selected => "no",
		operation => "rtrveqptdata",
	},
	"Tail Log Files" => {
		handler => "taillogfiles",
		active => "yes",
		selected => "yes",
		operation => "taillogfiles",
	},
	"Remove All IOMs" => {
		handler => "rmvallioms",
		active => "yes",
		selected => "no",
		operation => "rmvioms",
	},
	"Restore All IOMs" => {
		handler => "rstallioms",
		active => "yes",
		operation => "rstioms",
	},
	"CpyMem All IOMS" => {
		handler => "cpymems",
		active => "yes",
		selected => "no",
		operation => "cpymems",
	},
	"Update IOM Flash Partitions" => {
		handler => "updpartioms",
		active => "yes",
		selected => "no",
		operation => "updateioms",
	},
	"Recover SPs" => {
		handler => "recoversps",
		active => "no",
		selected => "no",
		operation => "recoversps",
	},
	"Audit IPs" => {
		handler => "auditsps",
		active => "no",
		selected => "no",
		operation => "auditips",
	},
	"Generate CHASSIS2IOMS" => {
		handler => "gench2ioms",
		active => "yes",
		selected => "no",
		operation => "gench2ioms",
	},
	"Synchronize MA Basescripts" => {
		handler => "syncfs",
		active => "yes",
		selected => "no",
		operation => "syncfs",
	},
	"FTP Load" => {
		handler => "ftpload",
		active => "yes",
		selected => "no",
		operation => "ftpload",
	},
	"Get SP Loads" => {
		handler => "getsploads",
		active => "yes",
		selected => "no",
		operation => "getsploads",
	},
	"Make BBB TL1 Scripts" => {
		handler => "bbbscripts",
		active => "yes",
		selected => "no",
		operation => "bbbscripts",
	},
	"Clean Up SPs" => {
		handler => "cleanupsps",
		active => "yes",
		selected => "no",
		operation => "cleanupsps",
	},
	"Server Process Status" => {
		handler => "procstatus",
		active => "yes",
		selected => "no",
		operation => "procstatus",
	},
);
#
1;
