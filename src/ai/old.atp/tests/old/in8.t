argument {
<x>(Pass(x, history) && Win(x, lottery) --> Happy(x));
<x><y>(Studies(x) || Lucky(x) --> Pass(x, y));
~Study(john) && Lucky(john);
<x>(Lucky(x) --> Win(x, lottery));
|- Happy(john);
}
#
quit;

