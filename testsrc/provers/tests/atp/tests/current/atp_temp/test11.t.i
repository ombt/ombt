argument {
	forall(x1)(
		forall(x2)(
			P(g(x1, x2), x1, x2)
		)
	);
	forall(x1)(
		forall(x2)(
			P(x1, h(x1, x2), x2)
		)
	);
	forall(x1)(
		forall(x2)(
			P(x1, x2, f(x1, x2))
		)
	);
	forall(x1)(
		forall(x2)(
			forall(x3)(
				forall(x4)(
					forall(x5)(
						forall(x6)(
							~P(x1, x2, x3) ||
							~P(x2, x4, x5) ||
							~P(x1, x5, x6) ||
							P(x3, x4, x6)
						)
					)
				)
			)
		)
	);
	forall(x1)(
		forall(x2)(
			forall(x3)(
				forall(x4)(
					forall(x5)(
						forall(x6)(
							~P(x1, x2, x3) ||
							~P(x2, x4, x5) ||
							~P(x3, x4, x6) ||
							P(x1, x5, x6)
						)
					)
				)
			)
		)
	);
	#
	SOS |- forall(x1)(P(j(x1), x1, j(x1)));
}
#
quit;
