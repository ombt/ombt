struct Barf {
	int i;
	int j;
};

struct Barf2 {
	int i :2;
	int j :4;
};

struct Barf3 {
	int i :2;
	int j :4;
	struct Barf2 k;
};

int EatMe1;
int EatMe2=3;
char EatMe3;
char EatMe4=5;
struct Barf barf;
struct Barf2 barf2;

int f()
{
	int f_1;
	struct Barf f_barf;
	struct Barf2 f_barf2;
	struct Barf3 f_barf3;
	return(0);
}
void g()
{
	int g_1;
	struct Barf g_barf;
	struct Barf2 g_barf2;
	struct Barf3 g_barf3;
	return;
}
main(int argc, char **argv)
{
	int babalou;
	int babalou2=1;
	struct Barf main_barf;
	struct Barf2 main_barf2;
	struct Barf3 main_barf3;
	f();
	g();
}
