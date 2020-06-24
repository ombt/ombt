BEGIN {
    oFS=FS
    title=0
    cflag=-1
    dsflag=0
    list=0
    toc=1
    abs=-1;
    mt["0"]=""
    mt["1"]="TECHNICAL MEMORANDUM"
    mt["2"]="INTERNAL MEMORANDUM"
    mt["3"]="ADMINISTRATIVE MEMORANDUM"
    pm["P"]="PROPRIETARY"
    pm["1"]="PROPRIETARY"
    pm["RS"]="PROPRIETARY (RESTRICTED)"
    pm["2"]="PROPRIETARY (RESTRICTED)"
    pm["RG"]="PROPRIETARY (REGISTERED)"
    pm["3"]="PROPRIETARY (REGISTERED)"
    pm["CP"]="SEE COVER PAGE message"
    pm["4"]="SEE COVER PAGE message"
    pm["CR"]="Copyright notice"
    pm["5"]="Copyright notice"
    pm["UW"]="Unpublished notice"
    pm["6"]="Unpublished notice"
    pm["BP"]="BELL LABORATORIES PROPRIETARY"
    PM=""
}
cflag==0 { print "<p>" }
cflag >= 0 { cflag--; }
$1==".PM" && NF > 1 { if($2 in pm) PM=pm[$2] }
$1==".br" { print "<br>"; next }
$1==".bp" { print "<hr>"; next }
$1==".sp" { print "<p>"; next }
$1==".tr" {
    l=length($2)
    for(i=1;i<=l;i++)
    {
	t=substr($2,i+1,1)
	if(t=="" || l==1) t=" "
	tr[substr($2,i++,1)]=t
    }
    next
}
$1==".PH" {
    clean()
    split($0,h,"'")
    print "<h2><P><TABLE WIDTH=100%><TR><TD ALIGN=LEFT>"h[2]"</TD><TD ALIGN=LEFT>"h[3]"</TD><TD ALIGN=RIGHT>"h[4]"</TD></TR></TABLE></h2><P>"
}
$1==".ND" {
	dline=parse($0)
        split(dline,a,";")
	date=a[2]
	next
}
$1==".TL" {
    print "<TITLE>"
    nt=0
    while ($1 != ".ND" && $1 != ".MT" && $1 != ".AF" && $1 != ".AU")
    {
	getline
	if(substr($0,1,1)!=".") 
	{
		clean()
		print
		Title[++nt]=$0
	}
    }
    print "</TITLE>"
    if (PM != "") print "<CENTER><B>"COMPANY" - "PM"</B><BR><small>Use Pursuant to Company Instructions.</small></CENTER>"
}
$1==".AU" {
    na=0
    while ($1 == ".AU" || $1 == ".AF")
    {
	aline=parse($0)
        split(aline,a,";")
	if (a[1]==".AF") { comp[na]=a[2]; continue; }
	++na
        #.AU "D. J. Noonan" DJN CB 131E10000 614-860-5412 3E398 
	name[na]=a[2]
	initial[na]=a[3]
	loc[na]=a[4]
	dept[na]=a[5]
	ext[na]=a[6]
	comp[na]=COMPANY
	if(length(a[6])<=5 && length(a[6])>0) ext[na]="x"ext[na]
	room[na]=a[7]
	getline
    }
}
$1==".MT" {
    if ($2 == 5)
    {
	print "<h2>"
        for(i=1;i<=nt;i++) print Title[i]
	print "</h2><p>"
	print "<P>"
	print "<P align=right>" date "</P>"
    }
    else if ($2 == 4)
    {
        print "<h2 align=center>"
        for(i=1;i<=nt;i++) print Title[i]
	print "</h2>"
	for (na in name)
	{
	    print "<h3 align=center><i>"name[na]"</i><br>"comp[na]"</h3>"
	}
    }
    else
    {
        print "<table width=100%><tr><td align=right><font size=+3><B><img src=\""LOGOFILE"\"  alt=\""COMPANY"\"></B></font size></td></tr></table><HR>\n"DIVISION"\n<P>"
        print "<table width=100%>\n<tr><td align=right valign=top><font size=-2>subject:</font size></td><td align=left><B>"
        for(i=1;i<=nt;i++) print Title[i]
        print "</B></td><td></td><td valign=top align=right><font size=-2>date:</font size></td><td valign=top><B>"date"</B></td></tr>"
        print "<tr><td></td><td></td><td></td><td align=right><font size=-2>author:</font size></td><td><B>"name[1]"</td></tr>"
        if(na>1)
        {
	    for(i=2;i<=na;i++)
    	        print "<tr><td></td><td></td><td></td><td></td><td><B>"name[i]"</td></tr>"
        }
        else
        {
    	    print "<tr><td></td><td></td><td></td><td></td><td><B>"loc[1]" "room[1]"</td></tr>"
    	    print "<tr><td></td><td></td><td></td><td></td><td><B>"dept[1]"</td></tr>"
    	    print "<tr><td></td><td></td><td></td><td></td><td><B>"ext[1]"</td></tr>"
        }
        print "</B></table>"
        if($2 in mt) MT=mt[$2]
	else if (NF==1) MT="TECHNICAL MEMORANDUM"
        else
        {
	    mtline=parse($0)
	    split(mtline,a,";")
	    $0=a[2]
	    clean()
	    MT=$0
        }
        print "<H2 align=center>"MT"</H2>"
    }
    next
}
$1==".AS" {
    ABS[++abs]="<HR><H2><CENTER>ABSTRACT</CENTER></H2>"
    while ($1 != ".AE")
    {
	getline
	if(substr($0,1,1)!=".") 
	{
		clean()
		ABS[++abs]=$0
	}
    }
    next
}
$1==".ce" { 
    print "<p ALIGN=CENTER>"
    cflag=1
    if(NF>1)cflag=$2
    next
    #getline
    #clean()
    #print
    #print "<p>"
    #next
}
$1==".NS" {
    print "<hr>\nCopy To:<p>"
    while (getline > 0 )
    {
	if($1==".NE") break
	print $0"<br>"
    }
    next
}
substr($0,1,3)=="'\\\"" { next }
$1==".EQ" { print "<p ALIGN=CENTER>" ; next }
$1==".EN" { print "<p>" ; next }
$1==".LI" { 
    gsub("^\.LI","")
    gsub(quote,"")
    clean()
    if (eol[list]=="</dl>")
    {
	print "<dt>" $0
	print "<dd>"
    }
    else print "<li>" $0
    next 
}
$1==".LE" { 
    print eol[list]
    list--
    next
}
$1==".VL" { print "<dl>";eol[++list]="</dl>"; next }
$1==".AL" { 
    if ($2=="") print "<ol>"
    else print "<ol type="$2">"
    eol[++list]="</ol>"
    next
}
$1==".BL" { print "<ul>";eol[++list]="</ul>"; next }
$1==".DL" { print "<ul>";eol[++list]="</ul>"; next }
$1==".TB" { 
    gsub(quote,"")
    print "<p><A NAME=\"Table " ++tbl "\"><B>Table " tbl": " substr($0,index($0,$2)) "</B><p>"
    next
}
$1==".FG" { 
    gsub(quote,"")
    print "<p><A NAME=\"Figure " ++fig "\"><B>Figure " fig": " substr($0,index($0,$2)) "</B><p>"
    next
}
$1=="delim" { eqns=$2; eqne=$3; next }
$1==".BP" {
    print "<p>\n<B><I>Missing Figure. See Original Compas Document for Figure</B></I>\n<p>"
}
$1==".PS" {
    while (getline > 0 )
    {
	if($1==".PE") break
    }
    print "<p>\n<B><I>Missing Figure. See Original Compas Document for Figure</B></I>\n<p>"
}
$1==".G1" {
    while (getline > 0 )
    {
	if($1==".G2") break
    }
    print "<p>\n<B><I>Missing Figure. See Original Compas Document for Figure</B></I>\n<p>"
}
$1==".RS" {
    gsub(quote,"")
    refname=$2
    REFN[++refn]=refname
    while (getline > 0 )
    {
	if($1==".RF") break
	clean()
	if(substr($0,1,1)==".") continue
        REFI[refn]=REFI[refn] $0 "\n"
    }
    print "<A HREF=\"#Ref_" refn "\"><font size=-2><I>[Reference "refn"]</I></font size></A>"
    next
}
$1==".FS" {
    gsub(quote,"")
    ++fn
    while (getline > 0 )
    {
	if($1==".FE") break
	clean()
        FNI[fn]=FNI[fn] $0 "\n"
    }
    print "<A HREF=\"#Fn_" fn "\"><font size=-2><I>[Footnote "fn"]</I></font size></A>"
    next
}
$1==".DS" {print "<pre>";dsflag=1;next}
$1==".DE" {print "</pre>";dsflag=0;next}
$1==".nf" {print "<pre>";next}
$1==".fi" {print "</pre>";next}
$1==".html" {print substr($0,index($0,$2))}
$1==".HU" {
    if (toc==1) system("cat "tocfile);
    toc=0
    for (i=0;i<=abs;i++) { print ABS[i] }
    abs=-1;
    gsub(quote,"")
    gsub(".HU ","")
    clean()
    print "<p>\n<A NAME=\""$0"\"><H3>"$0"</H3></A>\n<p>"
    sec[2]++
    next
}
$1==".H" {
    if (toc==1) system("cat "tocfile);
    toc=0
    for (i=0;i<=abs;i++) { print ABS[i] }
    abs=-1;
    gsub(quote,"")
    if ($2=="1") print "<HR>"
    clean()
    heading=substr($0,index($0,$3))
    sec[$2]++
    getsection($2)
    hsize=$2+1
    print "<p>\n<A NAME=\"" heading "\"><H" hsize ">" section "   " heading "</H" hsize "></A>\n<p>"
    next
}
$1==".TS" {
    if(dsflag==1) print "</pre>"
    getline
    i=index($0,"tab")
    fs="	"
    if(i>0)fs=substr($0,index($0,"tab")+4,1)
    border=""
    if(index($0,"box")>0) border="border"
    print "<p>\n<TABLE "border">"
    th=1
    FS=fs
    tsn=0  #Table specification  counter
    tdn=0  #Table data counter
    while (substr($0,1,3) !=".TE")
    {
	getline
	clean()
	if (th==1)
	{
	    # Save Table Specification
            if (length($0)==index($0,"."))
	    {
	        $0=substr($0,1,length($0)-1)
		th=0
	    }
	    ts[++tsn]=$0
	    continue
	}
	if (th==0 && substr($0,1,3) != ".TE" )
	{
	    if ($1==".TH"||$0=="="||$0=="_")
	    {
		#print "<TR></TR>"
		#if($0=="=") print "<TR></TR>"
	        continue
	    }
	    skip=0
	    for(j=1;j<=NF;j++) if($j=="_") skip=1
	    if (skip==1) continue
	    if(++tdn>tsn)tdn=tsn
	    nc=split(ts[tdn],sp," ")
	    ns=1
	    nf=0
	    for(j=nc;j>=1;j--)
	    {
		if(sp[j]=="|" || sp[j]=="||") continue
		nf++
		if(sp[j]=="s") ns++
		else
		{
		    #nf++
		    td[j]="colspan="ns
		    attrs[j]=""
		    attre[j]=""
		    ns=1
		    if(index(sp[j],"B")>0)
		    {
			attrs[j]=attrs[j]"<B>"
			attre[j]=attre[j]"</B>"
		    }
		    else if(index(sp[j],"I")>0)
		    {
			attrs[j]=attrs[j]"<I>"
			attre[j]=attre[j]"</I>"
		    }
		    if(index(sp[j],"c")>0) td[j]=td[j]" align=center "
		    if(index(sp[j],"r")>0) td[j]=td[j]" align=right "
		}
	    }
	    print "<TR>"
	    tflag=0
	    i=0
	    adder=0
	    for (j=1;j<=nf+adder;j++)
	    {
		if(sp[j]=="|" || sp[j]=="||") {++adder;continue}
		if(sp[j]=="s") continue
		i++
		span=""
		if(substr($i,1,2)=="T{")
		{
		    print "<TD "td[j]">"attrs[i]	
		    i=0
		    while (getline > 0)
		    {
		        if($1=="T}")
		        {
		            print attre[j]"</TD>"
		            tflag=0
			    i=1
			    break
		        }
		        else if(substr($0,1,1)!=".")
			{
		            clean()
			    print
			}
		    }
		}
		else
		{
	            print "<TD " td[j]">"attrs[j] $i attre[j]"</TD>"
		}
	    }
	    print "</TR>"
	    dt="TD"
	}
    }
    print "</TABLE><p>"
    if(dsflag==1) print "<pre>"
    FS=oFS
}
$1==".P" { print "<p>"; next }
substr($0,1,1) != "." { 
    clean()
    print
    if(cflag>0)
    {
	print "<br>"
    }
}
function clean()
{
    if($1==".B") { $0=pre"<B>" ; if(opre>0)pre="" }
    else if($1==".R") { $0=pre"</B></I>" ; if(opre>0)pre="" }
    else if($1==".I") { $0=pre"<I>" ; if(opre>0)pre="" }
    else
    {
        npre=gsub("\\\\f\\(CO","<pre>")
        if(npre>0)pre="</pre>"
        opre=gsub("\\\\fB",pre"<B>")
        opre+=gsub("\\\\f3",pre"<B>")
        opre+=gsub("\\\\fI",pre"<I>")
        opre+=gsub("\\\\f2",pre"<I>")
        opre+=gsub("\\\\f4",pre"<I><B>")
        opre+=gsub("\\\\fP",pre"</B></I>")
        opre+=gsub("\\\\fR",pre"</B></I>")
        gsub("\\\\s..","")
        if(opre>0) pre=""
        #gsub("\\\\\\(..","")
        #gsub("\\\\\\(rg","")
        #gsub("\\\\\\(dg","")
        #gsub("\\\\\\(rh","")
        #gsub("\\\\\\(lh","")
        gsub("\\\\\\(co","\\&#169;")
        gsub("\\\\\\(dg","\\&#134;")
        gsub("\\\\\\(co","\\&#1;")
        gsub("\\\\\\(dd","\\&#135;")
        gsub("\\\\\\(rg","\\&#174;")
        gsub("\\\\\\(tm","\\&#153;")
        for (cli=1;cli<=refn;cli++)
        {
	    search="\\\\\\*\\("REFN[cli]
	    repl="<A HREF=\"#Ref_"cli"\"><font size=-2><I>[Reference "cli"]</I></font size></A>"
            gsub(search,repl)
        }
        gsub("\\\\\\*\\(..","")
        gsub("\\\\\\.","")
	for(t in tr)
	{
	    gsub(t,tr[t])
	}
    }
}
# Parses lines that may have some tokens in quotes
# return parsed string with delimeters of ";"
# any original ";" will be changed to a ":"
function parse(line)
{
	gsub(";",":",line)
	dquote="\""
	nf=split(line,args,dquote)
	nline=""
	for(i=1;i<=nf;i++)
	{
	    if(i%2 == 1) gsub("  *",";",args[i])
	    nline=nline args[i]
	}
	return(nline)
}
function getsection(l)
{
    section=""
    for (i=1;i<=l;i++)
    {
	if(sec[i]=="") break
	if (section=="") section=sec[i]
	else section=section "." sec[i]
    }
    for (i=l+1;i<=6;i++) sec[i]=""
    if(l==1) section=section"."
}
END {
    print "</BODY>"
    if(fn!="") 
    {
	print "<hr>\n<H2>Footnotes:</H2>\n<dl>"
    	for (i=1;i<=fn;i++)
    	{
	    print "<A NAME=\"Fn_" i "\"><dt>Footnote "i":</A>"
	    print "<dd>"FNI[i]
    	}
    	print "</dl>"
    }
    if(refn!="") 
    {
	print "<hr>\n<H2>References:</H2>\n<dl>"
    	for (i=1;i<=refn;i++)
    	{
	    print "<A NAME=\"Ref_" i "\"><dt>Reference "i":</A>"
	    print "<dd>"REFI[i]
    	}
    	print "</dl>"
    }
}
