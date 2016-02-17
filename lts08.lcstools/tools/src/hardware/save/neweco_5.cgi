#!/usr/bin/perl -w
use diagnostics;
use strict;
use DirHandle;
use Mysql;
use Storable;

require "RMcommon.cgi";

########
#GLOBALS
########
my $ecoDir = "/telica/toaster/home/release/ECO";
my @kitTypes = ("cpu","ds1","ds1_2","ds3","octds3","octds3_2","octds3_3","trids3","trids3_3","e1","voip","ena","ena2","pna","voa","cm","vs3");

my @allGeneralKits = ("cpu","ds1","ds1_2","ds3","octds3","ana","voa","pna","ena","voip","tdmoc","trids3","e1","cm","vs3");

#######
#BEGIN
#######
$| = 1;
parse_form_data();
my $SessionFile = $::FORM_DATA{'SessionFile'};
my $Session = retrieve($SessionFile);

printHeader();

my $DB_patches = Mysql->connect("build2","patches","builder","turk74") || die2("create_patch: ERROR: Can't connect to DB server");
my %VerifiedKits;


if($Session->{bts} ne "NONE"){
    print("<BR> Ecoing BTS=$Session->{bts}\n");

    print("<BR>btsBP=$Session->{btsBP}\n");
    print("<BR>btsBase=$Session->{btsBase}\n");
    print("<BR>btsBranch=$Session->{btsBranch}\n");

    my $btsRelDir = "/telica/toaster/home/release/$Session->{btsBranch}/swBTS/$Session->{bts}";
    $Session->{btsRelDir} = $btsRelDir;
    print("<BR>btsRelDir=$btsRelDir\n");

    my $btsEcoDir = "/telica/toaster/home/release/ECO/swBTS/$Session->{btsBase}";
    $Session->{btsEcoDir} = $btsEcoDir;
    print("<BR>btsEcoDir=$btsEcoDir\n");

    unless( -d $btsEcoDir ){
        print("<BR>Creatig $btsEcoDir\n");
	system("mkdir -p $btsEcoDir");
        system("chmod 777 $btsEcoDir");
    }

    print("<BR>cp -pR $btsRelDir $btsEcoDir");	
    system("cp -pR $btsRelDir $btsEcoDir");

    CreateBtsEcoMail();
}
else{
    my $btsEcoDir = "/telica/toaster/home/release/ECO/swBTS/$Session->{btsBase}";
    $Session->{btsEcoDir} = $btsEcoDir;

    my @btsVersions = `ls -I doc $btsEcoDir`;

    my @sortedBtsVersions = reverse sort sortVersions @btsVersions;

    $Session->{bts} = $sortedBtsVersions[0];

}

eco_ecoRelease();

printEnd();

exit();

#####################################
# FUNCTIONS
#####################################

my $ACTION = "";
my %NewReleases;


sub eco_ecoRelease(){
    my $EcoVersion = $Session->{ecoAs};
    my $prevEcoVersion = "";
    my $foundKits = $Session->{foundKits};
    	

    if( $Session->{alreadyEcodCpu} == 0 ){ # ALREADY ECOd
	#print("<BR>This tool is under construction for combining OCTDS3 and TRIDS3 eco mails!!\n");
	#print("<BR>Please wait till 10/12 afternoon... \n");
	#exit(1);
	print("$Session->{ecoAs} is a already Ecod release.<BR> \n");

	my @targetedGaVersions = getTargetedGaVersions($Session->{ecoAs});

	my @actions = ("VERIFY","MOVE");
        foreach my $targetDir ( @targetedGaVersions ){
            foreach my $action (@actions){
                $ACTION = $action;
 	        print("<BR><BR>############ action=$ACTION ############\n");
                foreach my $kit ( @allGeneralKits ){
	            if( defined($Session->{$kit})){
			savePreviouslyReleasedKits($kit,$targetDir) if($action eq "MOVE");
                        eco("NEW_RELEASE",$kit,$prevEcoVersion,$targetDir);
		        if($NewReleases{$kit} ne "yes"){
		             $NewReleases{$kit} = "yes";	
		        }
                    }
                }
	    }
  	}

        foreach my $kit ( @allGeneralKits ){
            if( $NewReleases{$kit} eq "yes" ){
	        next if( $kit eq "cpu" or $kit eq "trids3");
	        print("<BR> ######## Creating ECO Doc for $kit ########\n");
	        CreatePatchEcoDoc($Session->{ecoAs},"KDI",$kit);
	    }
        }	 

        exit(1);
    }
    else{ # NEW ECO
	print("$Session->{ecoAs} will be Ecod.\n");

	my $gaVersions = $Session->{gaVersions};
        $prevEcoVersion = @$gaVersions[0];
        if($prevEcoVersion eq ""){
	    print("<BR>PROBLEM: Can not determine the previously Ecod release!\n");
	    print("The reason may be the GA release for $Session->{base} might Not be Ecod yet!!\n");
            exit(1);
        }
	elsif( $prevEcoVersion eq $Session->{ecoAs} ){
	    print("<BR>PROBLEM: prevEcoVersion=$prevEcoVersion is equal to $Session->{ecoAs}\n");
	    exit(1);
	}
	else{
	    print("<BR>$prevEcoVersion is the previously Ecod version.<BR>\n");
        }

        checkIfExists($EcoVersion);

	my @actions = ("VERIFY","MOVE");
        foreach my $action (@actions){
            $ACTION = $action;
 	    print("<BR><BR>############ action=$ACTION ############\n");
            foreach my $kit ( @allGeneralKits ){
	        if( defined($Session->{$kit})){
                    eco("NEW_RELEASE",$kit,$prevEcoVersion,$Session->{ecoAs});
		    if($NewReleases{$kit} ne "yes"){
		         $NewReleases{$kit} = "yes";	
		    }
                }
	        else{
                    eco("PRV_RELEASE",$kit,$prevEcoVersion,$Session->{ecoAs});
	        }	
            }
        }

        if($NewReleases{cpu} eq "yes"){
	    print("<BR> ######## Creating ECO Doc for cpu ########\n");
            CreatePatchEcoDoc($Session->{ecoAs},"SP","cpu");
	    SaveEcodRelease("cpu");
        }
        foreach my $kit ( @allGeneralKits ){
            if( $NewReleases{$kit} eq "yes" ){
	        next if( $kit eq "cpu" );
	        print("<BR> ######## Creating ECO Doc for $kit ########\n");
	        CreatePatchEcoDoc($Session->{ecoAs},"KDI",$kit);
	        SaveEcodRelease($kit);
	    }
        }	 
    }

    #updateGaVersions();

    print("<BR> Done !!!!\n");

    exit(1);
}

sub savePreviouslyReleasedKits(){
	my $kit = shift;
	my $targetedDir = shift;
        #print("<BR>Verified Kits\n");
        #foreach my $kit ( keys %VerifiedKits ){
            #print("<BR>$kit\n");
        #}

	print("<BR>mkdir -p /telica/toaster/home/release/ECO/$targetedDir/oldKits\n");
	system("mkdir -p /telica/toaster/home/release/ECO/$targetedDir/oldKits");
        system("chmod 775 /telica/toaster/home/release/ECO/$targetedDir");
	print("<BR>mv /telica/toaster/home/release/ECO/$targetedDir/*_$kit* /telica/toaster/home/release/ECO/$targetedDir/oldKits\n");
	system("mv /telica/toaster/home/release/ECO/$targetedDir/*_$kit* /telica/toaster/home/release/ECO/$targetedDir/oldKits");
}

sub getTargetedGaVersions(){
    my $ecoAs = shift;
    my @targetedGaVersions;
    push(@targetedGaVersions,$ecoAs);

    my $gaVersions = $Session->{gaVersions};

    foreach my $kit ( @$gaVersions ){
	if( $kit eq $ecoAs ){
	    last;
	}
	else{
	    push(@targetedGaVersions,$kit);
	}
    }	
    print("<BR>targetedGaVersions=@targetedGaVersions\n");
    return @targetedGaVersions;
}

sub SaveEcodRelease(){
    my $kit = shift;

    print("<BR>SaveEcodRelease kit=$kit kitVersion=$Session->{$kit} \n");
    SaveKit($kit,$Session->{$kit});
    SaveKit("octds3_2",$Session->{$kit}) if($kit eq "octds3");
    SaveKit("octds3_3",$Session->{$kit}) if($kit eq "octds3");
    SaveKit("trids3_3",$Session->{$kit}) if($kit eq "trids3");
    SaveKit("voip6",$Session->{$kit}) if($kit eq "voip");
    SaveKit("ena2",$Session->{$kit}) if($kit eq "ena");
}

sub SaveKit(){
    my $kit = shift;
    my $kitVersion = shift;	
    if($kit eq "cpu"){
    	$kitVersion =~  s/\./-/g;	
    }
    else{
	$kitVersion =~  s/\-/./g;
    }	
    print("<BR>kit=$kit kitVersion=$kitVersion\n");
    my $swKit = "sw" . uc($kit);
    $swKit = "sw" . "VOIP" if($kit eq "voip6");

    my $src = "/home2/builder/newpatches/$Session->{branch}/$swKit/$kitVersion";
    my $targetDir = "/home2/builder/newpatches/ECOd/$Session->{branch}/$swKit";
    print("<BR>src=$src");
    if( $kitVersion ne "" and -d $src ){
 	if( ! -d $targetDir ){
	    print("<BR> $targetDir does not exist! Creating.\n");
	}
        print("<BR>mv $src $targetDir\n");
	system("mv $src $targetDir");
    }
}

sub eco($){
    my $relType = shift;
    my $kit = shift;
    my $prevEcoVersion = shift;
    my $targetDir = shift;

    ecoRelease($relType,$kit,$prevEcoVersion,$Session->{$kit},"UPDATE_DB",$targetDir);
    ecoRelease($relType,"octds3_2",$prevEcoVersion,$Session->{$kit},"NO_DB",$targetDir) if($kit eq "octds3");
    ecoRelease($relType,"octds3_3",$prevEcoVersion,$Session->{$kit},"NO_DB",$targetDir) if($kit eq "octds3");
    ecoRelease($relType,"trids3_3",$prevEcoVersion,$Session->{$kit},"NO_DB",$targetDir) if($kit eq "trids3");
    ecoRelease($relType,"voip6",$prevEcoVersion,$Session->{$kit},"NO_DB",$targetDir) if($kit eq "voip");
    ecoRelease($relType,"ena2",$prevEcoVersion,$Session->{$kit},"NO_DB",$targetDir) if($kit eq "ena");
}

sub ecoRelease(){
    my $relType = shift;
    my $kit = shift;
    my $prevEcoVersion = shift;
    my $kitVersion = shift;
    my $updateDb = shift;
    my $target = shift;

    my $targetDir = "/telica/toaster/home/release/ECO/$target";

    if($relType eq "NEW_RELEASE"){
        my $swKit = "sw" . uc($kit);
	$swKit = "sw" . "VOIP" if($kit eq "voip6");

        my $base = $Session->{base};
	my $baseWDot = $base;
	$baseWDot =~ s/-/\./g;

	my $kitV = $kitVersion;
	$kitV =~ s/-/\./g;

	my $dirName = $kitV;
	$dirName =~ s/\./-/g if($kit eq "cpu");

	my $branch = "BP-" . $base;
	my $tarFileName =  $kitV . "_" . lc($kit) . "\.tar\.gz";	
	my $srcDir = "/telica/toaster/home/builder/newpatches/$branch/$swKit/$dirName";


	if($ACTION eq "VERIFY"){
	    print("<BR>$kit $tarFileName $relType\n");
	    print("<BR>srcDir=$srcDir\n");
	    print("<BR>tarFileName=$tarFileName\n");
	    print("<BR>targetDir=$targetDir\n");
	    if(-f "$srcDir/$tarFileName"){
	        $VerifiedKits{$kit} = "yes";
	        print("<BR>$srcDir/$tarFileName exists\n");
	    } 
	}
	elsif($ACTION eq "MOVE"){
	    if( $VerifiedKits{$kit} eq "yes" ){
	        print("<BR>cp -p $srcDir/$tarFileName  $targetDir");
	        system("cp -p $srcDir/$tarFileName  $targetDir");

	        if($updateDb eq "UPDATE_DB"){
	            if($kit eq "cpu"){
	                updatePatchDbForCpu($baseWDot,$Session->{cpu});
	            }
	            else{
	                updatePatchDbForIom($kit,$baseWDot,$Session->{$kit});
 	            }
	        }
	    }
        }

        print("<BR>\n");	

    }
    else{
	my $srcDir = "/telica/toaster/home/release/ECO/$prevEcoVersion";
        my $tarFileStr =   "_" . lc($kit) . "\.tar\.gz";	
	#print("<BR>ls $srcDir | grep _$kit");
	my @lsOut= `ls $srcDir | grep _$kit`;
        #print("<BR>lsOut=$lsOut[0]\n");
	chomp($lsOut[0]);
	my $tarFileName = "";
	if($lsOut[0] ne ""){
	    $tarFileName = `basename $lsOut[0]`;
	}
	
	chomp($tarFileName);
	
	if($ACTION eq "VERIFY"){
            print("<BR>$kit $tarFileName $relType\n");
            #print("<BR>srcDir=$srcDir\n");
            #print("<BR>tarFileName=$tarFileName\n");
            #print("<BR>targetDir=$targetDir\n");
            if(-f "$srcDir/$tarFileName"){
	        $VerifiedKits{$kit} = "yes";
	        print("<BR>$srcDir/$tarFileName exists\n");
            } 
            print("<BR>\n");	
	}
	elsif($ACTION eq "MOVE"){
	    if( $VerifiedKits{$kit} eq "yes" ){
                print("<BR>cp -p $srcDir/$tarFileName  $targetDir");
                system("cp -p $srcDir/$tarFileName  $targetDir");
                print("<BR>\n");	
	    }
        }

    }
}

sub printEnd(){
    print "<BR>\n";
    print "<BR>\n";
    print "<BR>\n";

    print "</BODY>";
    print "</HTML>\n";
}

sub printHeader(){
    print "Content-type: text/html\n\n";

    print "<HTML><HEAD>\n<TITLE>ECO Process</TITLE>\n";
    print "<H1>Eco Copy for $Session->{ecoAs}</H1>\n";
}

sub checkIfExists($){
    my $EcoVersion = shift;	

    if( -d "$ecoDir/$EcoVersion"){
        print("$ecoDir/$EcoVersion exists..  Updating!\n");print("<BR>");
	#exit;
    }

    system("mkdir -p $ecoDir/$EcoVersion/doc");
    system("chmod 777 $ecoDir/$EcoVersion/doc");
}

sub updateGaVersions(){
    my $query = "update gaVersions  where cpu = '$::FORM_DATA{cpu}'";
}

sub updatePatchDbForCpu($$){
    my ($base,$cpuVersion) = @_;

    #print("<BR>base=$base cpuVersion=$cpuVersion\n");
    $cpuVersion =~ s/-/\./g;
    my $cpuV = getCpuV($cpuVersion);

    my $query = "update patch set status='RELEASED',restricted='$::FORM_DATA{restricted}',date_req='$::FORM_DATA{date_req}',date_avail='$::FORM_DATA{date_avail}',comments='$::FORM_DATA{comments}'  where base='$base' and type='SP' and pak=$cpuV";
    print("<BR>query=$query");
    $DB_patches->query($query);
}

sub updatePatchDbForIom($$$){
    my ($kit,$base,$kitVersion) = @_;

    $kitVersion =~ s/-/\./g;
    #print("<BR>kitVersion=$kitVersion base=$base kit=$kit\n");
	
    my $kitV = getKitV($kitVersion);
    $kit="ds12" if (  $kit eq "ds1_2" );
    my $query = "select patch_id from bugs where base='$base' and type='KDI' and $kit=$kitV";
    print("<BR>query=$query");

    my $query_out = $DB_patches->query($query);
    my $patch_id = $query_out->fetchrow();

    $query = "update patch set status='RELEASED',restricted='$::FORM_DATA{restricted}',date_req='$::FORM_DATA{date_req}',date_avail='$::FORM_DATA{date_avail}',comments='$::FORM_DATA{comments}'  where patch_id = $patch_id";

    print("<BR>query=$query");
    $DB_patches->query($query);
}

sub getKitV($){
    my ($kitVersion) = @_;

    my @words = split(/\.P/,$kitVersion);

    return $words[1];
}

sub getCpuV($){
    my ($cpuVersion) = @_;

    my @words = split(/\.SP\./,$cpuVersion);

    return $words[1];
}

sub sortVersions {
    my @awords;
    my @bwords;
    #print("a=$a b=$b<BR>\n");
    if( $a =~ /-SP-/ ){
        @awords = split(/-SP-/,$a);
        @bwords = split(/-SP-/,$b);
    }
    else{
        @awords = split(/-SP-/,$a);
        @bwords = split(/-SP-/,$b);
    }
    #else{
        #my @c  = split(/\./,$a);
        #$awords[1] = $c[$#c];
        #my @d  = split(/\-/,$b);
        #$bwords[1] = $d[$#d];
    #}
    $awords[1] <=> $bwords[1];
}

sub getLocation($){
    my $kit = shift;

    my @location; 

    my @tarFileName = `ls $ecoDir/$Session->{ecoAs} | grep _$kit\.tar`;
    print("<BR>kit=$kit tarFileName=$tarFileName[0]\n");
    if($tarFileName[0] ne ""){
	my $lstr = "$ecoDir/$Session->{ecoAs}/$tarFileName[0]\n";
        push(@location,$lstr);	
    }

    if($kit eq "octds3"){
        my @tarFileName = `ls $ecoDir/$Session->{ecoAs} | grep _octds3_2\.tar`;
        print("<BR>kit=$kit tarFileName=$tarFileName[0]\n");
        if($tarFileName[0] ne ""){
            my $lstr = "$ecoDir/$Session->{ecoAs}/$tarFileName[0]\n";
            push(@location,$lstr);
        }

        @tarFileName = `ls $ecoDir/$Session->{ecoAs} | grep _octds3_3\.tar`;
        print("<BR>kit=$kit tarFileName=$tarFileName[0]\n");
        if($tarFileName[0] ne ""){
            my $lstr = "$ecoDir/$Session->{ecoAs}/$tarFileName[0]\n";
            push(@location,$lstr);
        }

        @tarFileName = `ls $ecoDir/$Session->{ecoAs} | grep _trids3\.tar`;
        print("<BR>kit=$kit tarFileName=$tarFileName[0]\n");
        if($tarFileName[0] ne ""){
            my $lstr = "$ecoDir/$Session->{ecoAs}/$tarFileName[0]\n";
            push(@location,$lstr);
        }

        @tarFileName = `ls $ecoDir/$Session->{ecoAs} | grep _trids3_3\.tar`;
        print("<BR>kit=$kit tarFileName=$tarFileName[0]\n");
        if($tarFileName[0] ne ""){
            my $lstr = "$ecoDir/$Session->{ecoAs}/$tarFileName[0]\n";
            push(@location,$lstr);
        }
    }
    elsif($kit eq "voip"){
	my @tarFileName = `ls $ecoDir/$Session->{ecoAs} | grep _voip6\.tar`;
        print("<BR>kit=$kit tarFileName=$tarFileName[0]\n");
        if($tarFileName[0] ne ""){
            my $lstr = "$ecoDir/$Session->{ecoAs}/$tarFileName[0]\n";
            push(@location,$lstr);
        }
    }
    elsif($kit eq "ena"){
	my @tarFileName = `ls $ecoDir/$Session->{ecoAs} | grep _ena2\.tar`;
        print("<BR>kit=$kit tarFileName=$tarFileName[0]\n");
        if($tarFileName[0] ne ""){
            my $lstr = "$ecoDir/$Session->{ecoAs}/$tarFileName[0]\n";
            push(@location,$lstr);
        }
    }

    print("<BR>location=@location\n");
    return @location;
}

sub CreatePatchEcoDoc($$$){
        my ($ecoVersion,$type,$kit) = @_;
        my $productDescription;

        my %h;
	my $kitTarFile = 0;
	my $kitVer = 1;

        my $base = $Session->{base};
        my $baseWDot = $base;
        $baseWDot =~ s/-/\./g;

        my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time());
        $mon++;
        $year = $year + 1900;

        my $today = $mon ."/" . $mday . "/" . $year;

        my @bugs;
        my $query = "select distinct bugzilla from bugs where base='$baseWDot' and type='$type' order by bugzilla";
        print("<BR>query=$query\n");

	my $query_out = $DB_patches->query($query);

        while(my $row = $query_out->fetchrow()){
                push(@bugs, $row);
        }

        if($type eq "SP"){
		my @AllSpecificKits = ("cpu","ds1","ds1_2","ds3","octds3","octds3_2","octds3_3","ana","voa","pna","ena","ena2","voip","voip6","tdmoc","trids3", "trids3_3","e1","cm","vs3");

                $productDescription = "CPU Software";

		foreach my $kit ( @AllSpecificKits ){
			#print("<BR>ls $ecoDir/$Session->{ecoAs} | grep _$kit\.tar\n");
			my @tarFileName = `ls $ecoDir/$Session->{ecoAs} | grep _$kit\.tar`;
			#print("<BR>kit=$kit tarFileName=$tarFileName[0]\n");
			chomp($tarFileName[0]);
			$h{$kit}[$kitTarFile] = "$ecoDir/$Session->{ecoAs}/$tarFileName[0]";
			if($tarFileName[0] ne ""){
				my @words = split(/_/,$tarFileName[0]);
				$h{$kit}[$kitVer] = $words[0];
			}
		}	
        }
        else{
                $productDescription = "KDI Software";
        }


#To: <steverc\@lucent.com>
open(SENDMAIL, "| /usr/lib/sendmail -t ") or "Die can't fork sendmail: $!\n";

if($type eq "SP"){

print SENDMAIL <<"ECO_DOC";
Subject: ECO for $ecoVersion
To: <steverc\@lucent.com>,<jmdelaney\@lucent.com>,<dyildiz\@lucent.com>


|-------------------------------------------------------------------------------|

   Product part number: 99-5000-x
   New version number: $ecoVersion
|-------------------------------------------------------------------------------|
   Date Submitted: $today
   Submitted by: $::FORM_DATA{owner}
|-------------------------------------------------------------------------------|
   []Restricted: $::FORM_DATA{restricted}
   $::FORM_DATA{comments}
|-------------------------------------------------------------------------------|
   Location:
   $h{cpu}[$kitTarFile]
|-------------------------------------------------------------------------------|
   Description of changes: (See sheet 2 for list of firmware & kdi):


|_______________________________________________________________________________|
   Comments/Reason for Changes (Include bug fixes if applicable):

   Incremental release containing fixes for bugs:

   @bugs


|-------------------------------------------------------------------------------|

   Billing (BTS) Software Tested With This Build:
   File Location: $Session->{btsEcoDir}/$Session->{bts}

|--------------------------------------------------------------------------------|
   Traffic Collection (TCA) Software Tested With This Build:
   File Location:


|--------------------------------------------------------------------------------|


|--------------------------------------------------------------------------------|
   Note: Part Number CLEI code are for information only and does not represent
         the modules tested.

   Kit/Images           Version         Part No         CLEI Code
   ----------           -------         -------         ---------
   DS1 IOM KIT                          89-0360-A       BA9IAA0AAA
   KDI			$h{ds1}[$kitVer]
   File Path:		$h{ds1}[$kitTarFile]

   Kit/Images           Version         Part No         CLEI Code
   ----------           -------         -------         ---------
   DS3 IOM KIT                          89-0365-B       BA9IX70AAB
   KDI                  $h{ds3}[$kitVer]                89-0365-C       BA9IX72AAA
   File Path:		$h{ds3}[$kitTarFile]

   Kit/Images           Version         Part No         CLEI Code
   ----------           -------         -------         ---------
   DS1.2 IOM KIT                        89-0414-A       BA7ATP0FAA
   KDI                  $h{ds1_2}[$kitVer]                
   File Path:		$h{ds1_2}[$kitTarFile]

   Kit/Images           Version         Part No         CLEI Code
   ----------           -------         -------         ---------
   OCT IOM KIT                          89-0382-A       BAA9UV0GAA
   KDI                  $h{octds3}[$kitVer]                89-0382-B       BAA9UVZGAA
   File Path:		$h{octds3}[$kitTarFile]

   Kit/Images           Version         Part No         CLEI Code
   ----------           -------         -------         ---------
   OCT_2 IOM KIT                        89-0398-A       BAA9UVXGAA
   KDI                  $h{octds3_2}[$kitVer]                89-0411-A       BA4A701FAA
   File Path:		$h{octds3_2}[$kitTarFile]

   Kit/Images           Version         Part No         CLEI Code
   ----------           -------         -------         ---------
   OCT_3 IOM KIT                        89-0398-A       BAA9UVXGAA
   KDI                  $h{octds3_3}[$kitVer]                89-0411-A       BA4A701FAA
   File Path:		$h{octds3_3}[$kitTarFile]

   Kit/Images           Version         Part No         CLEI Code
   ----------           -------         -------         ---------
   TRIDS3 IOM KIT                       89-0397-A       BA9IX04AAA
   KDI                  $h{trids3}[$kitVer]                89-0410-A       BA4A60ZFAA
   File Path:		$h{trids3}[$kitTarFile]

   Kit/Images           Version         Part No         CLEI Code
   ----------           -------         -------         ---------
   TRIDS3_3 IOM KIT                     89-0397-A       BA9IX04AAA
   KDI                  $h{trids3_3}[$kitVer]                89-0410-A       BA4A60ZFAA
   File Path:		$h{trids3_3}[$kitTarFile]

   Kit/Images           Version         Part No         CLEI Code
   ----------           -------         -------         ---------
   VPS6 IOM KIT                         89-0384-A       BAA9Z20GAA
   KDI			$h{voip6}[$kitVer]
   File Path:		$h{voip6}[$kitTarFile]

   Kit/Images           Version         Part No         CLEI Code
   ----------           -------         -------         ---------
   VPS8 IOM KIT
   KDI                  $h{voip}[$kitVer]                89-0395-A       BA1AX60AAA
   File Path:		$h{voip}[$kitTarFile]

   Kit/Images           Version         Part No         CLEI Code
   ----------           -------         -------         ---------
   VS3 IOM KIT
   KDI                  $h{vs3}[$kitVer]                89-0413-B       BAUIAA1EAB
   File Path:		$h{vs3}[$kitTarFile]

   Kit/Images           Version         Part No         CLEI Code
   ----------           -------         -------         ---------
   ENA IOM KIT                          89-0390-A       BA2A30TGAA
   KDI 			$h{ena}[$kitVer]
   File Path:		$h{ena}[$kitTarFile]

   Kit/Images           Version         Part No         CLEI Code
   ----------           -------         -------         ---------
   ENA2 IOM KIT                         89-0432-A       BAUIADPEAA
   KDI 			$h{ena2}[$kitVer]
   File Path:		$h{ena2}[$kitTarFile]

   Kit/Images           Version         Part No         CLEI Code
   ----------           -------         -------         ---------
   CM IOM KIT                           89-0419-A       BA9ATS0FAA
   KDI 			$h{cm}[$kitVer]                89-0419-B       BA9ATS0FAB
   File Path:		$h{cm}[$kitTarFile]

|--------------------------------------------------------------------------------|


ECO_DOC

}
else{

my $firmWare;
my @loc; 

if( defined($Session->{$kit} )){
    @loc = getLocation($kit);		
}

foreach my $loc ( @loc ){
    chomp($loc);	
    my @firms = `tar tzf $loc`;

    $firmWare .= join('',@firms);
    $firmWare .= "\n"; 
}

my $kitName = $kit;
if($kit eq "octds3"){
    $kitName = "octds3 & trids3";
}

#To: <steverc\@lucent.com>
print SENDMAIL <<"ECO_KDI_DOC";
Subject: ECO for $Session->{$kit} $kitName
To: <steverc\@lucent.com>,<jmdelaney\@lucent.com>,<dyildiz\@lucent.com>


|-------------------------------------------------------------------------------|

   Product part number: 99-5000-x
   New version number: $Session->{$kit}
   Ecod Under: $Session->{ecoAs}
|-------------------------------------------------------------------------------|
   Date Submitted: $today
   Submitted by: $::FORM_DATA{owner}
|-------------------------------------------------------------------------------|
   []Restricted: $::FORM_DATA{restricted}
   $::FORM_DATA{comments}
|-------------------------------------------------------------------------------|
   Location:
@loc 
|_______________________________________________________________________________|
   Comments/Reason for Changes (Include bug fixws if applicable):

   Incremental release containing fixes for bugs:

   @bugs

|-------------------------------------------------------------------------------|
   Firmware Components

$firmWare

|-------------------------------------------------------------------------------|
ECO_KDI_DOC
}

close(SENDMAIL);

}

sub CreateBtsEcoMail(){

	print("<BR> Creating ECO mail for BTS\n");

        my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time());
        $mon++;
        $year = $year + 1900;

        my $today = $mon ."/" . $mday . "/" . $year;

        my @bugs;

        #my $query = "select distinct fixed_in.Bugzilla from fixed_in,file where file.Branch="BRANCH-3-8-3" and file.filepath like "%data_collect%" and file.fix_id=fixed_in.FixId order by fixed_in.Bugzilla";
        #print("<BR>query=$query\n");

        #my $query_out = $DB_patches->query($query);

        #while(my $row = $query_out->fetchrow()){
                #push(@bugs, $row);
        #}


#To: <steverc\@lucent.com>
open(SENDMAIL, "| /usr/lib/sendmail -t ") or "Die can't fork sendmail: $!\n";
print SENDMAIL <<"ECO_BTS_DOC";
Subject: ECO for $Session->{bts} BTS
To: <steverc\@lucent.com>,<jmdelaney\@lucent.com>,<dyildiz\@lucent.com>


|-------------------------------------------------------------------------------|

   Product part number: 99-5000-x
   BTS Billing Software Package	
   New version number: $Session->{bts}
|-------------------------------------------------------------------------------|
   Date Submitted: $today
   Submitted by: $::FORM_DATA{owner}
|-------------------------------------------------------------------------------|
   []Restricted: $::FORM_DATA{restricted}
   $::FORM_DATA{comments}
|-------------------------------------------------------------------------------|
   Location:
$Session->{btsEcoDir}/$Session->{bts}
|_______________________________________________________________________________|
   Comments/Reason for Changes (Include bug fixws if applicable):


|-------------------------------------------------------------------------------|
ECO_BTS_DOC

}

sub ecoLog($){
    my $log = shift;
    print("$log");
}
