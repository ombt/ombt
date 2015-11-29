// simple pseudo-random number generator

// 
static unsigned long m = 121500;
static unsigned long a = 2041;
static unsigned long c = 25673;
static unsigned long Ikey = 71551;

void
setKey(unsigned long newKey)
{
	Ikey = newKey;
	return;
}

unsigned long
myrandom()
{
	Ikey = (a*Ikey+c)%m;
	return(Ikey);
}

unsigned char
myrandomchar()
{
	Ikey = (a*Ikey+c)%m;
	return('A'+Ikey%26);
}
