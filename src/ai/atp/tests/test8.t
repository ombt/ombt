argument {
forall(x)forall(y)forall(u)forall(v)(T(x,y,u,v) --> P(x,y,u,v));
forall(x)forall(y)forall(u)forall(v)(P(x,y,u,v) --> E(x,y,v,u,v,y));
T(a,b,c,d);
SOS |- E(a,b,d,c,d,b);
}
#
quit;
