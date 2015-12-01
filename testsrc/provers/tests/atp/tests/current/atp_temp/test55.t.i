#
options {
	gconst1 = 1;
	uconst1 = 1;
	cconst1 = 0;
}
#
argument {
<x>Equal(x,x);
<x><y>(~Equal(x,y) || Equal(y,x));
<x><y><z>(~Equal(x,y) || ~Equal(y,z) || Equal(x,z));
 
<x><y><z>(~Equal(y,z) || ~P(x,y) || P(x,z));
<x><y><z>(~Equal(x,z) || ~P(x,y) || P(z,y));
 
<y>(Equal(y,a) || ~P(y,a) || Equal(y,k));
<y>(Equal(y,a) || P(y,a) || ~Equal(y,k));
<y>(Equal(y,n) || ~P(y,n) || ~Equal(g(y),n));
<y>(Equal(y,n) || ~P(y,n) || ~Equal(g(y),y));
 
 
<y>(Equal(y,n) || ~P(y,n) || P(y,g(y)));
<y>(Equal(y,n) || ~P(y,n) || P(g(y),y));
<y><u>(Equal(y,n) || P(y,n) || Equal(u,n) || Equal(u,y) || ~P(y,u) || ~P(u,y));
 
<y>(Equal(y,b) || P(y,b) || ~Equal(j(y),b));
<y>(Equal(y,b) || P(y,b) || ~Equal(j(y),y));
<y>(Equal(y,b) || P(y,b) || P(y,j(y)));
<y>(Equal(y,b) || P(y,b) || P(j(y),y));
<y>(Equal(y,b) || ~P(y,b) || Equal(v,b) || Equal(v,y) || ~P(y,v) || ~P(v,y));
 
<y>(Equal(y,k) || ~Equal(y,n) || P(y,k));
<y>(Equal(y,k) || ~Equal(y,b) || P(y,k));
<y>(Equal(y,k) || Equal(y,n) || Equal(y,b) || ~P(y,k));
 
~Equal(n,b);
Equal(n,k);
Equal(b,k);
~Equal(n,a);
~Equal(k,a);
 
	SOS |- Equal(b,a);
 
}
#
quit;
#
