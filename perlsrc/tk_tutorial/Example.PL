#!/usr/bin/perl
use Tk;

#Global Variables
$age = 10;
$occupied = 1;
$gender = "Male";

# Main Window
$mw = new MainWindow;

#GUI Building Area
$frm_name = $mw -> Frame();
$lab = $frm_name -> Label(-text=>"Name:");
$ent = $frm_name -> Entry();
#Age
$scl = $mw -> Scale(-label=>"Age :",
	 -orient=>'v',	 	-digit=>1,
	 -from=>10,			-to=>50,
	 -variable=>\$age,	-tickinterval=>10);

#Jobs
$frm_job = $mw -> Frame();
$chk = $frm_job -> Checkbutton(-text=>"Occupied",
	-variable=>\$occupied);
$chk -> deselect();
$lst = $frm_job -> Listbox(-selectmode=>'single');

#Adding jobs
$lst -> insert('end',"Student","Teacher","Clerk","Business Man",
	"Militry Personal","Computer Expert","Others");

#Gender
$gender = $mw -> Frame();
$lbl_gender = $gender -> Label(-text=>"Sex ");
$rdb_m = $gender -> Radiobutton(-text=>"Male",  -value=>"Male",  -variable=>\$gender);
$rdb_f = $gender -> Radiobutton(-text=>"Female",-value=>"Female",-variable=>\$gender);
# $rdb_m -> select();

$but = $mw -> Button(-text=>"Push Me", -command =>\&push_button);

#Text Area
$textarea = $mw -> Frame();
$txt = $textarea -> Text(-width=>40, -height=>10);
$srl_y = $textarea -> Scrollbar(-orient=>'v',-command=>[yview => $txt]);
$srl_x = $textarea -> Scrollbar(-orient=>'h',-command=>[xview => $txt]);
$txt -> configure(-yscrollcommand=>['set', $srl_y], -xscrollcommand=>['set',$srl_x]);

#Geometry Management
$lab -> grid(-row=>1,-column=>1);
$ent -> grid(-row=>1,-column=>2);
$scl -> grid(-row=>2,-column=>1);
$frm_name -> grid(-row=>1,-column=>1,-columnspan=>2);

$chk -> grid(-row=>1,-column=>1,-sticky=>'w');
$lst -> grid(-row=>2,-column=>1);
$frm_job -> grid(-row=>2,-column=>2);

$lbl_gender -> grid(-row=>1,-column=>1);
$rdb_m -> grid(-row=>1,-column=>2);
$rdb_f -> grid(-row=>1,-column=>3);
$gender -> grid(-row=>3,-column=>1,-columnspan=>2);

$but -> grid(-row=>4,-column=>1,-columnspan=>2);

$txt -> grid(-row=>1,-column=>1);
$srl_y -> grid(-row=>1,-column=>2,-sticky=>"ns");
$srl_x -> grid(-row=>2,-column=>1,-sticky=>"ew");
$textarea -> grid(-row=>5,-column=>1,-columnspan=>2);

MainLoop;

## Functions
#This function will be exected when the button is pushed
sub push_button {
	my $name = $ent -> get();
	$txt -> insert(end,"$name\($gender\) is $age years old and is ");
	
	#See whether he is employed
	if ( $occupied == 1 ) {
		$job_id = $lst -> curselection(); #Get the no of selected jobs
		if ( $job_id eq "" ) { #If there is no job
			$job = "a Non worker.";
		}
		else {
			$job = $lst -> get($job_id) ;#Get the name of the job
			$txt -> insert(end,"a $job.");
		}
	}
	else {
		$txt -> insert(end,"unemployed.");
	}
}
