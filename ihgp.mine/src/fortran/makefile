F77 = /opt/SUNWspro/bin/f77

F77FLAGS = -g -DDEBUG

all:	f1 f2 f3 f4 f6 f7 f8 f9 f10  \
	f11 f12 f13 f14 f15 f16 f17 f18 f19 f20 \
	f21 f22 f23 f24 f25 f26 f27 quad f28 f29 proj5
	echo "all done" >all

f1:	f1.f
	$(F77) $(F77FLAGS) -o $@ $?

f2:	f2.f
	$(F77) $(F77FLAGS) -o $@ $?

f3:	f3.f
	$(F77) $(F77FLAGS) -o $@ $?

f4:	f4.f
	$(F77) $(F77FLAGS) -o $@ $?

# select case not supported by SUN F77 compiler, so don't build f5.f.
#f5:	f5.f
	# $(F77) $(F77FLAGS) -o $@ $?

f6:	f6.f
	$(F77) $(F77FLAGS) -o $@ $?

f7:	f7.f
	$(F77) $(F77FLAGS) -o $@ $?

f8:	f8.f
	$(F77) $(F77FLAGS) -o $@ $?

f9:	f9.f
	$(F77) $(F77FLAGS) -o $@ $?

f10:	f10.f
	$(F77) $(F77FLAGS) -o $@ $?

f11:	f11.f
	$(F77) $(F77FLAGS) -o $@ $?

f12:	f12.f
	$(F77) $(F77FLAGS) -o $@ $?

f13:	f13.f
	$(F77) $(F77FLAGS) -o $@ $?

f14:	f14.f
	$(F77) $(F77FLAGS) -o $@ $?

f15:	f15.f
	$(F77) $(F77FLAGS) -o $@ $?

f16:	f16.f
	$(F77) $(F77FLAGS) -o $@ $?

f17:	f17.f gauss.f
	$(F77) $(F77FLAGS) -o $@ f17.f gauss.f

f18:	f18.f linearfit.f
	$(F77) $(F77FLAGS) -o $@ f18.f linearfit.f

f19:	f19.f trig.f
	$(F77) $(F77FLAGS) -o $@ f19.f trig.f

f20:	f20.f bisect.f
	$(F77) $(F77FLAGS) -o $@ f20.f bisect.f

f21:	f21.f exp.f
	$(F77) $(F77FLAGS) -o $@ f21.f exp.f

f22:	f22.f newton.f
	$(F77) $(F77FLAGS) -o $@ f22.f newton.f

f23:	f23.f secant.f
	$(F77) $(F77FLAGS) -o $@ f23.f secant.f

f24:	f24.f graph.f
	$(F77) $(F77FLAGS) -o $@ f24.f graph.f

f25:	f25.f
	$(F77) $(F77FLAGS) -o $@ f25.f

f26:	f26.f
	$(F77) $(F77FLAGS) -o $@ f26.f

f27:	f27.f
	$(F77) $(F77FLAGS) -o $@ f27.f

quad:	quad.for
	$(F77) $(F77FLAGS) -o $@ quad.for

f28:	f28.f linearfit.f
	$(F77) $(F77FLAGS) -o $@ f28.f linearfit.f

f29:	f29.f ngauss.f
	$(F77) $(F77FLAGS) -o $@ f29.f ngauss.f

proj5:	proj5.for
	$(F77) $(F77FLAGS) -o $@ proj5.for

