#
argument {
SOS |- (<x>P(x) && <x>Q(x)) <--> ~(~<x>P(x) || ~<x>Q(x));
}
#
argument {
SOS |- (<x>P(x) && <x>Q(x)) <--> ~(~<x>P(x) && ~<x>Q(x));
}
#
argument {
SOS |- (<x>P(x) && <x>Q(x)) <--> ~(<x>P(x) --> ~<x>Q(x));
}
#
quit;
