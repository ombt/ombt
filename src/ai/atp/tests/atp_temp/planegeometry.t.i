
argument {
	<x><y>S(x,y,y,x);
	<x><y><u><v>(S(u,v,x,y)-->S(x,y,u,v));
	<x><y><u><v>(S(u,v,x,y)-->S(v,u,x,y));
	<x><y><z>(T(x,y,z)-->T(y,z,x));
	<x><y><z>(T(x,y,z)-->T(y,x,z));
	<x><y><z><u><v><w>(C(u,v,w,x,y,z)-->(A(u,v,w,x,y,z)&&A(v,u,w,y,x,z)&&
		A(u,w,v,x,z,y)));
	<x><y><z><u><v><w>((T(u,v,w)&&T(x,y,z)&&S(u,v,x,y)&&S(v,w,y,z)&&
		S(u,w,x,z))-->C(u,v,w,x,y,z));
	SOS |- (T(a,b,c)&&S(a,c,b,c))-->A(c,a,b,c,b,a);
}
quit;
