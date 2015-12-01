// perform editing operations

// headers
#include "edelf.h"
#include "trans.h"

// editing version entries
void
readversions(ElfObject &eo, char *s)
{
	eo.readehdr();
	eo.readshdrs();
	eo.readstrings();
	eo.readsymbols();
	eo.readversions();
	return;
}

static void
printmenu()
{
	printf("versions menu:\n");
	printf("? or h - show menu\n");
	printf("n - show all version section names\n");
	printf("r - show current version entry\n");
	printf("rd - review first version definition\n");
	printf("rd * - review all version definitions\n");
	printf("rd <version> - review <version> in version definitions\n");
	printf("rs - review first version symbol\n");
	printf("rs * - review all version symbols\n");
	printf("rs <symbol> - review <symbol> in version symbols\n");
	printf("rn - review first version dependency\n");
	printf("rn * - review all version dependencies\n");
	printf("rn <file> - review <file> in version dependencies\n");
	printf("+ - next entry in current section\n");
	printf("u - update current entry in current section\n");
	printf("D - toggle demangle mode\n");
	printf("q - quit\n");
	return;
}

static void
showverdef(ElfObject &eo, int cverno, Elf32_Verdef *pvd)
{
	printf("%d: vd_version: 0x%x (%s)\n", 
		cverno, pvd->vd_version, i2s(vd_version, pvd->vd_version));
	printf("%d: vd_flags  : 0x%x (%s)\n", 
		cverno, pvd->vd_flags, i2s(vd_flags, pvd->vd_flags));
	printf("%d: vd_ndx    : 0x%x\n", cverno, pvd->vd_ndx);
	printf("%d: vd_cnt    : 0x%x\n", cverno, pvd->vd_cnt);
	printf("%d: vd_hash   : 0x%lx\n", cverno, pvd->vd_hash);
	printf("%d: vd_aux    : 0x%lx\n", cverno, pvd->vd_aux);
	printf("%d: vd_next   : 0x%lx\n", cverno, pvd->vd_next);
	return;
}

static void
showverdaux(ElfObject &eo, int cverno, int csec, Elf32_Verdef *pvd, Elf32_Verdaux *pva)
{
	for (int va=0; va<=pvd->vd_cnt; va++)
	{
		printf("%d, %d: vda_name: %s (0x%lx)\n", 
			cverno, va, 
			eo.pshdrdata(eo.pshdr(csec).sh_link)+pva->vda_name, 
			pva->vda_name);
		printf("%d, %d: vd_next : 0x%lx\n", 
			cverno, va, pva->vda_next);
		if (pva->vda_next == 0) break;
		pva = (Elf32_Verdaux *)((char*)pva+pva->vda_next);
	}
	return;
}

static void
showverneed(ElfObject &eo, int cverno, int csec, Elf32_Verneed *pvn)
{
	printf("%d: vn_version: 0x%x (%s)\n", 
		cverno, pvn->vn_version, i2s(vn_version, pvn->vn_version));
	printf("%d: vn_cnt    : 0x%x\n", cverno, pvn->vn_cnt);
	printf("%d: vn_file   : %s (0x%lx)\n", 
		cverno, eo.pshdrdata(eo.pshdr(csec).sh_link)+pvn->vn_file,
		pvn->vn_file);
	printf("%d: vn_aux    : 0x%lx\n", cverno, pvn->vn_aux);
	printf("%d: vn_next   : 0x%lx\n", cverno, pvn->vn_next);
	return;
}

static void
showvernaux(ElfObject &eo, int cverno, int csec, Elf32_Verneed *pvn, Elf32_Vernaux *pva)
{
	for (int va=0; va<=pvn->vn_cnt; va++)
	{
		printf("%d, %d: vna_hash: 0x%lx\n", 
			cverno, va, pva->vna_hash);
		printf("%d, %d: vna_flags: 0x%x (%s)\n", 
			cverno, va, pva->vna_flags,
			i2s(vn_flags, pva->vna_flags));
		printf("%d, %d: vna_other: 0x%x\n", 
			cverno, va, pva->vna_other);
		printf("%d, %d: vna_name: %s (0x%lx)\n", 
			cverno, va, 
			eo.pshdrdata(eo.pshdr(csec).sh_link)+pva->vna_name, 
			pva->vna_name);
		printf("%d, %d: vna_next : 0x%lx\n", 
			cverno, va, pva->vna_next);
		if (pva->vna_next == 0) break;
		pva = (Elf32_Vernaux *)((char*)pva+pva->vna_next);
	}
	return;
}

static void
showversym(ElfObject &eo, int cverno, int csec, Elf32_Versym *pvs)
{
	switch (cverno)
	{
	case VER_NDX_LOCAL:
		printf("%d: versym: 0x%x (%s)\n", cverno, *pvs, 
			(eo.verdefnms() != NULL && 
		         eo.verdefnms(*pvs) != NULL) ? 
				eo.verdefnms(*pvs) : "unknown");
		break;
	case VER_NDX_GLOBAL:
		printf("%d: versym: 0x%x (%s)\n", cverno, *pvs, 
			(eo.verdefnms() != NULL && 
			 eo.verdefnms(*pvs) != NULL) ? 
				eo.verdefnms(*pvs) : "unknown");
		break;
	default:
	{
		int symsec = eo.pshdr(csec).sh_link;
		char *p0 = eo.pshdrdata(symsec);
		char *pe = eo.pshdrdata(symsec)+eo.pshdr(symsec).sh_size;
		printf("%d: versym: 0x%x (%s)\n", cverno, *pvs, 
			(eo.verdefnms() != NULL && 
			 eo.verdefnms(*pvs) != NULL) ? 
			 eo.verdefnms(*pvs) : "unknown");
		Elf32_Sym *psym = 
			(Elf32_Sym *)(p0+cverno*eo.pshdr(symsec).sh_entsize);
		char *psymnm = 
			eo.pshdrdata(eo.pshdr(symsec).sh_link)+psym->st_name;
		printf("%d: st_name : %s (%lx)\n", cverno,
			mydemangle(psymnm), psym->st_name);
		break;
	}	
	}
	return;
}

static int
eqverdef(ElfObject &eo, int , int csec, Elf32_Verdef *, Elf32_Verdaux *pva, char *ps)
{
	MustBeTrue(ps != NULL);
	return(REequal(ps, 
		mydemangle(eo.pshdrdata(eo.pshdr(csec).sh_link)+pva->vda_name)));
}

static int
eqverneed(ElfObject &eo, int , int csec, Elf32_Verneed *pvn, char *ps)
{
	MustBeTrue(ps != NULL);
	return(REequal(ps, 
		mydemangle(eo.pshdrdata(eo.pshdr(csec).sh_link)+pvn->vn_file)));
}

static int
eqversym(ElfObject &eo, int cverno, int csec, Elf32_Versym *pvs, char *ps)
{
	MustBeTrue(ps != NULL);
	switch (cverno)
	{
	case VER_NDX_LOCAL:
	case VER_NDX_GLOBAL:
		return(0);
	default:
	{
		int symsec = eo.pshdr(csec).sh_link;
		char *p0 = eo.pshdrdata(symsec);
		char *pe = eo.pshdrdata(symsec)+eo.pshdr(symsec).sh_size;
		Elf32_Sym *psym = 
			(Elf32_Sym *)(p0+cverno*eo.pshdr(symsec).sh_entsize);
		char *psymnm = eo.pshdrdata(eo.pshdr(symsec).sh_link)+
				psym->st_name;
		return(REequal(ps, mydemangle(psymnm)));
	}	
	}
}

static void
review(ElfObject &eo, int &cverno, int &cveroff, int &csec, char *&p0, char *&pe, 
		int &ctype, int showcurrent)
{
	// get tokens
	char *pt2 = gettoken(2);

	// review sections
	if (pt2 == NULL)
	{
		// show current entry
		switch (ctype)
		{
		case SHT_SUNW_verdef:
		{
			if (!showcurrent || eo.pshdr(csec).sh_type != ctype)
			{
				// resync to this type 
				for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
				{
					if (eo.pshdr(csec).sh_type == ctype)
						break;
				}
				if  (csec >= eo.pehdr()->e_shnum)
				{
					printf("verdef section not found.\n");	
					return;
				}
				p0 = eo.pshdrdata(csec);
				pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
				cveroff = 0;
				cverno = 0;
			}
			Elf32_Verdef *pvd = 
				(Elf32_Verdef *)(p0+cveroff);
			Elf32_Verdaux *pva = 
				(Elf32_Verdaux *)(p0+cveroff+pvd->vd_aux);
			showverdef(eo, cverno, pvd);
			showverdaux(eo, cverno, csec, pvd, pva);
			break;
		}
		case SHT_SUNW_verneed:
		{
			if (!showcurrent || eo.pshdr(csec).sh_type != ctype)
			{
				// resync to this type 
				for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
				{
					if (eo.pshdr(csec).sh_type == ctype)
						break;
				}
				if  (csec >= eo.pehdr()->e_shnum)
				{
					printf("verneed section not found.\n");	
					return;
				}
				p0 = eo.pshdrdata(csec);
				pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
				cveroff = 0;
				cverno = 0;
			}
			Elf32_Verneed *pvn = 
				(Elf32_Verneed *)(p0+cveroff);
			Elf32_Vernaux *pva = 
				(Elf32_Vernaux *)(p0+cveroff+pvn->vn_aux);
			showverneed(eo, cverno, csec, pvn);
			showvernaux(eo, cverno, csec, pvn, pva);
			break;
		}
		case SHT_SUNW_versym:
		{
			if (!showcurrent || eo.pshdr(csec).sh_type != ctype)
			{
				// resync to this type 
				for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
				{
					if (eo.pshdr(csec).sh_type == ctype)
						break;;
				}
				if  (csec >= eo.pehdr()->e_shnum)
				{
					printf("versym section not found.\n");	
					return;
				}
				p0 = eo.pshdrdata(csec);
				pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
				cveroff = 0;
				cverno = 0;
			}
			Elf32_Versym *pvs = (Elf32_Versym *)(p0+cveroff);
			showversym(eo, cverno, csec, pvs);
			break;
		}
		default:
			printf("no version type given.\n");	
			break;
		}
	}
	else if (*pt2 == '*')
	{
		for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
		{
			if (eo.pshdr(csec).sh_type != ctype)
				continue;
			p0 = eo.pshdrdata(csec);
			pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
			cveroff = eo.pshdr(csec).sh_size;
			switch (ctype)
			{
			case SHT_SUNW_verdef:
			{
				int maxverno = eo.pshdr(csec).sh_info;
				Elf32_Verdef *pvd = (Elf32_Verdef *)p0;
				Elf32_Verdaux *pva = 
					(Elf32_Verdaux *)(p0+pvd->vd_aux);
				for (cverno=0; cverno < maxverno; cverno++)
				{
					showverdef(eo, cverno, pvd);
					showverdaux(eo, cverno, csec, pvd, pva);
					char *p = (char *)pvd;
					pva = (Elf32_Verdaux *)(p+pvd->vd_aux);
					pvd = (Elf32_Verdef *)(p+pvd->vd_next);
				}
				break;
			}
			case SHT_SUNW_verneed:
			{
				int maxverno = eo.pshdr(csec).sh_info;
				Elf32_Verneed *pvn = (Elf32_Verneed *)p0;
				Elf32_Vernaux *pva = 
					(Elf32_Vernaux *)(p0+pvn->vn_aux);
				for (cverno=0; cverno < maxverno; cverno++)
				{
					showverneed(eo, cverno, csec, pvn);
					showvernaux(eo, cverno, csec, pvn, pva);
					char *p = (char *)pvn;
					pva = (Elf32_Vernaux *)(p+pvn->vn_aux);
					pvn = (Elf32_Verneed *)(p+pvn->vn_next);
				}
				break;
			}
			case SHT_SUNW_versym:
			{
				int symndx = eo.pshdr(csec).sh_link;
				int entsz = eo.pshdr(csec).sh_entsize;;
				int maxverno = eo.pshdr(symndx).sh_size/
						eo.pshdr(symndx).sh_entsize;
				Elf32_Versym *pvs = (Elf32_Versym *)p0;
				for (cverno=0; cverno<maxverno; cverno++)
				{
					showversym(eo, cverno, csec, pvs);
					char *p = (char *)pvs;
					pvs = (Elf32_Versym *)(p+entsz);
				}
				break;
			}
			default:
				printf("no version type given.\n");	
				break;
			}
		}
	}
	else
	{
		for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
		{
			if (eo.pshdr(csec).sh_type != ctype)
				continue;
			p0 = eo.pshdrdata(csec);
			pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
			cveroff = 0;
			switch (ctype)
			{
			case SHT_SUNW_verdef:
			{
				int maxverno = eo.pshdr(csec).sh_info;
				Elf32_Verdef *pvd = (Elf32_Verdef *)p0;
				Elf32_Verdaux *pva = 
					(Elf32_Verdaux *)(p0+pvd->vd_aux);
				cveroff = 0;
				for (cverno=0; cverno < maxverno; cverno++)
				{
					if (!eqverdef(eo, 
						cverno, csec, pvd, pva, pt2))
					{
						cveroff = ((char *)pvd)-p0;
						char *p = (char *)pvd;
						pva = (Elf32_Verdaux *)
							(p+pvd->vd_aux);
						pvd = (Elf32_Verdef *)
							(p+pvd->vd_next);
						continue;
					}
					cveroff = ((char *)pvd)-p0;
					showverdef(eo, cverno, pvd);
					showverdaux(eo, cverno, csec, pvd, pva);
                                        char s[BUFSIZ];
                                        printf("next entry? [n/y/cr=y] ");
                                        rmvnlgets(s);
                                        if (*s != 'y' && *s != '\0')
						goto all_done;
					char *p = (char *)pvd;
					pva = (Elf32_Verdaux *)(p+pvd->vd_aux);
					pvd = (Elf32_Verdef *)(p+pvd->vd_next);
				}
				break;
			}
			case SHT_SUNW_verneed:
			{
				int maxverno = eo.pshdr(csec).sh_info;
				Elf32_Verneed *pvn = (Elf32_Verneed *)p0;
				Elf32_Vernaux *pva = 
					(Elf32_Vernaux *)(p0+pvn->vn_aux);
				for (cverno=0; cverno < maxverno; cverno++)
				{
					if (!eqverneed(eo, cverno, csec, pvn, pt2))
					{
						cveroff = ((char *)pvn)-p0;
						char *p = (char *)pvn;
						pva = (Elf32_Vernaux *)
							(p+pvn->vn_aux);
						pvn = (Elf32_Verneed *)
							(p+pvn->vn_next);
						continue;
					}
					cveroff = ((char *)pvn)-p0;
					showverneed(eo, cverno, csec, pvn);
					showvernaux(eo, cverno, csec, pvn, pva);
                                        char s[BUFSIZ];
                                        printf("next entry? [n/y/cr=y] ");
                                        rmvnlgets(s);
                                        if (*s != 'y' && *s != '\0')
						goto all_done;
					char *p = (char *)pvn;
					pva = (Elf32_Vernaux *)(p+pvn->vn_aux);
					pvn = (Elf32_Verneed *)(p+pvn->vn_next);
				}
				break;
			}
			case SHT_SUNW_versym:
			{
				int symndx = eo.pshdr(csec).sh_link;
				int entsz = eo.pshdr(csec).sh_entsize;;
				int maxverno = eo.pshdr(symndx).sh_size/
						eo.pshdr(symndx).sh_entsize;
				Elf32_Versym *pvs = (Elf32_Versym *)p0;
				for (cverno=0; cverno<maxverno; cverno++)
				{
					if (!eqversym(
						eo, cverno, csec, pvs, pt2))
					{
						cveroff = ((char *)pvs)-p0;
						char *p = (char *)pvs;
						pvs = (Elf32_Versym *)
							(p+entsz);
						continue;
					}
					cveroff = ((char *)pvs)-p0;
					showversym(eo, cverno, csec, pvs);
                                        char s[BUFSIZ];
                                        printf("next entry? [n/y/cr=y] ");
                                        rmvnlgets(s);
                                        if (*s != 'y' && *s != '\0')
						goto all_done;
					char *p = (char *)pvs;
					pvs = (Elf32_Versym *)(p+entsz);
				}
				break;
			}
			default:
				printf("no version type given.\n");	
				break;
			}
		}
		all_done: ;
	}
	return;
}

static void
updverdef(ElfObject &eo, int cverno, int csec, Elf32_Verdef *pvd)
{
	int upd = 0;
	char s[BUFSIZ];

	printf("%d: vd_version [cr=0x%x]: ", cverno, pvd->vd_version);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pvd->vd_version = MYatoi(s);
		printf("%d: vd_version:  0x%x\n", cverno, pvd->vd_version);
	}
	printf("%d: vd_flags [cr=0x%x]: ", cverno, pvd->vd_flags);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pvd->vd_flags = MYatoi(s);
		printf("%d: vd_flags:  0x%x\n", cverno, pvd->vd_flags);
	}
	printf("%d: vd_ndx [cr=0x%x]: ", cverno, pvd->vd_ndx);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pvd->vd_ndx = MYatoi(s);
		printf("%d: vd_ndx:  0x%x\n", cverno, pvd->vd_ndx);
	}
	printf("%d: vd_cnt [cr=0x%x]: ", cverno, pvd->vd_cnt);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pvd->vd_cnt = MYatoi(s);
		printf("%d: vd_cnt:  0x%x\n", cverno, pvd->vd_cnt);
	}
	printf("%d: vd_hash [cr=0x%lx]: ", cverno, pvd->vd_hash);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pvd->vd_hash = MYatoi(s);
		printf("%d: vd_hash:  0x%lx\n", cverno, pvd->vd_hash);
	}
	printf("%d: vd_aux [cr=0x%lx]: ", cverno, pvd->vd_aux);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pvd->vd_aux = MYatoi(s);
		printf("%d: vd_aux:  0x%lx\n", cverno, pvd->vd_aux);
	}
	printf("%d: vd_next [cr=0x%lx]: ", cverno, pvd->vd_next);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pvd->vd_next = MYatoi(s);
		printf("%d: vd_next:  0x%lx\n", cverno, pvd->vd_next);
	}

	// write to file
	if (upd > 0)
	{
		printf("write to file [cr=n/n/y] ? ");
		rmvnlgets(s);
		if (*s == 'y')
		{
			eo.writeverdef(csec);
		}

		// reread data
		char dummy[1];
		readversions(eo, dummy);
	}
	return;
}

static void
updverdaux(ElfObject &eo, int cverno, int csec, Elf32_Verdef *pvd, Elf32_Verdaux *pva)
{
	// update auxiliary entries
	int upd = 0;
	char s[BUFSIZ];
	for (int va=0; va<=pvd->vd_cnt; va++)
	{
		char *psymnm = 
			eo.pshdrdata(eo.pshdr(csec).sh_link)+pva->vda_name;
		printf("%d, %d: vda_name [cr=%s]: ", cverno, va, psymnm);
		rmvnlgets(s);
		if (*s != '\0')
		{
			upd++;
			int sz = strlen(psymnm);
			strncpy(psymnm, s, sz);
			psymnm[sz] = '\0';
			printf("vda_name: %s\n", psymnm);
		}
		printf("%d, %d: vd_next [cr=0x%lx]: ", 
			cverno, va, pva->vda_next);
		rmvnlgets(s);
		if (*s != '\0')
		{
			upd++;
			pva->vda_next = MYatoi(s);
			printf("vd_next: 0x%lx\n", pva->vda_next);
		}
		if (pva->vda_next == 0) break;
		pva = (Elf32_Verdaux *)((char*)pva+pva->vda_next);
	}

	// write to file
	if (upd > 0)
	{
		printf("write to file [cr=n/n/y] ? ");
		rmvnlgets(s);
		if (*s == 'y')
		{
			eo.writeverdaux(csec);
		}

		// reread data
		char dummy[1];
		readversions(eo, dummy);
	}
	return;
}

static void
updverneed(ElfObject &eo, int cverno, int csec, Elf32_Verneed *pvn)
{
	int upd = 0;
	char s[BUFSIZ];

	// update fields
	printf("%d: vn_version [cr=0x%x]: ", cverno, pvn->vn_version);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pvn->vn_version = MYatoi(s);
		printf("%d: vn_version: 0x%x\n", cverno, pvn->vn_version);
	}
	printf("%d: vn_cnt [cr=0x%x]: ", cverno, pvn->vn_cnt);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pvn->vn_cnt = MYatoi(s);
		printf("%d: vn_cnt: 0x%x\n", cverno, pvn->vn_cnt);
	}
	char *psymnm = eo.pshdrdata(eo.pshdr(csec).sh_link)+pvn->vn_file;
	printf("%d: vn_file [cr=%s]:  ", cverno, psymnm);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		int sz = strlen(psymnm);
		strncpy(psymnm, s, sz);
		psymnm[sz] = '\0';
		printf("vn_file: %s\n", psymnm);
	}
	printf("%d: vn_aux [cr=0x%lx]: ", cverno, pvn->vn_aux);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pvn->vn_aux = MYatoi(s);
		printf("%d: vn_aux: 0x%lx\n", cverno, pvn->vn_aux);
	}
	printf("%d: vn_next [cr=0x%lx]: ", cverno, pvn->vn_next);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pvn->vn_next = MYatoi(s);
		printf("%d: vn_next: 0x%lx\n", cverno, pvn->vn_next);
	}

	// write to file
	if (upd > 0)
	{
		printf("write to file [cr=n/n/y] ? ");
		rmvnlgets(s);
		if (*s == 'y')
		{
			eo.writeverneed(csec);
		}

		// reread data
		char dummy[1];
		readversions(eo, dummy);
	}
	return;
}

static void
updvernaux(ElfObject &eo, int cverno, int csec, Elf32_Verneed *pvn, Elf32_Vernaux *pva)
{
	int upd = 0;
	char s[BUFSIZ];

	// update fields
	for (int va=0; va<=pvn->vn_cnt; va++)
	{
		printf("%d, %d: vna_hash [cr=0x%lx]: ", 
			cverno, va, pva->vna_hash);
		rmvnlgets(s);
		if (*s != '\0')
		{
			upd++;
			pva->vna_hash = MYatoi(s);
			printf("%d: vna_hash: 0x%lx\n", cverno, pva->vna_hash);
		}
		printf("%d, %d: vna_flags [cr=0x%x]: ", 
			cverno, va, pva->vna_flags);
		rmvnlgets(s);
		if (*s != '\0')
		{
			upd++;
			pva->vna_flags = MYatoi(s);
			printf("%d: vna_flags: 0x%x\n", cverno, pva->vna_flags);
		}
		printf("%d, %d: vna_other [cr=0x%x]: ", 
			cverno, va, pva->vna_other);
		rmvnlgets(s);
		if (*s != '\0')
		{
			upd++;
			pva->vna_other = MYatoi(s);
			printf("%d: vna_other: 0x%x\n", cverno, pva->vna_other);
		}
		char *psymnm = 
			eo.pshdrdata(eo.pshdr(csec).sh_link)+pva->vna_name;
		printf("%d, %d: vna_name [cr=%s]: ", 
			cverno, va, psymnm);
		rmvnlgets(s);
		if (*s != '\0')
		{
			upd++;
			int sz = strlen(psymnm);
			strncpy(psymnm, s, sz);
			psymnm[sz] = '\0';
			printf("vna_name: %s\n", psymnm);
		}
		printf("%d, %d: vna_next [cr=0x%lx]: ", 
			cverno, va, pva->vna_next);
		rmvnlgets(s);
		if (*s != '\0')
		{
			upd++;
			pva->vna_next = MYatoi(s);
			printf("%d: vna_next: 0x%lx\n", cverno, pva->vna_next);
		}
		if (pva->vna_next == 0) break;
		pva = (Elf32_Vernaux *)((char*)pva+pva->vna_next);
	}

	// write to file
	if (upd > 0)
	{
		printf("write to file [cr=n/n/y] ? ");
		rmvnlgets(s);
		if (*s == 'y')
		{
			eo.writevernaux(csec);
		}

		// reread data
		char dummy[1];
		readversions(eo, dummy);
	}
	return;
}

static void
updversym(ElfObject &eo, int cverno, int csec, Elf32_Versym *pvs)
{
	int upd = 0;
	char s[BUFSIZ];

	// update fields
	printf("%d: versym [cr=0x%x]: ", cverno, *pvs);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		*pvs = MYatoi(s);
		printf("%d: versym: 0x%x\n", cverno, *pvs);
	}

	// write to file
	if (upd > 0)
	{
		printf("write to file [cr=n/n/y] ? ");
		rmvnlgets(s);
		if (*s == 'y')
		{
			eo.writeversym(csec);
		}

		// reread data
		char dummy[1];
		readversions(eo, dummy);
	}
	return;
}

static void
update(ElfObject &eo, int &cverno, int &cveroff, int &csec, char *&p0, 
		char *&pe, int &ctype)
{
	// check section and string
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}

	// sanity check
	MustBeTrue(p0 == eo.pshdrdata(csec));
	MustBeTrue(pe == (eo.pshdrdata(csec)+eo.pshdr(csec).sh_size));

	// switch on type 
	switch (ctype)
	{
	case SHT_SUNW_verdef:
	{
		int maxverno = eo.pshdr(csec).sh_info;
		MustBeTrue(cverno < maxverno);
		MustBeTrue((p0+cveroff) < pe);
		Elf32_Verdef *pvd = (Elf32_Verdef *)(p0+cveroff);
		Elf32_Verdaux *pva = (Elf32_Verdaux *)(p0+cveroff+pvd->vd_aux);
		updverdef(eo, cverno, csec, pvd);
		updverdaux(eo, cverno, csec, pvd, pva);
		break;
	}
	case SHT_SUNW_verneed:
	{
		int maxverno = eo.pshdr(csec).sh_info;
		MustBeTrue(cverno < maxverno);
		MustBeTrue((p0+cveroff) < pe);
		Elf32_Verneed *pvn = (Elf32_Verneed *)(p0+cveroff);
		Elf32_Vernaux *pva = (Elf32_Vernaux *)(p0+cveroff+pvn->vn_aux);
		updverneed(eo, cverno, csec, pvn);
		updvernaux(eo, cverno, csec, pvn, pva);
		break;
	}
	case SHT_SUNW_versym:
	{
		int symndx = eo.pshdr(csec).sh_link;
		int entsz = eo.pshdr(csec).sh_entsize;;
		int maxverno = eo.pshdr(symndx).sh_size/entsz;
		MustBeTrue(cverno < maxverno);
		Elf32_Versym *pvs = (Elf32_Versym *)(p0+cveroff);
		updversym(eo, cverno, csec, pvs);
		break;
	}
	default:
		printf("invalid version type.\n");
		break;
	}
	return;
}

static void
increment(ElfObject &eo, int &cverno, int &cveroff, int &csec, char *&p0, 
		char *&pe, int &ctype)
{
	// check section and string
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}

	// sanity check
	MustBeTrue(p0 == eo.pshdrdata(csec));
	MustBeTrue(pe == (eo.pshdrdata(csec)+eo.pshdr(csec).sh_size));

	// switch on type 
	switch (ctype)
	{
	case SHT_SUNW_verdef:
	{
		int maxverno = eo.pshdr(csec).sh_info;
		if ((cverno+1) >= maxverno)
		{
			printf("cannot increment beyond top end.\n");
			return;
		}
		if ((p0+cveroff) >= pe)
		{
			printf("cannot increment beyond top end.\n");
			return;
		}
		Elf32_Verdef *pv = (Elf32_Verdef *)(p0+cveroff);
		if (pv->vd_next == 0)
		{
			printf("cannot increment beyond top end.\n");
			return;
		}
		cveroff += pv->vd_next;
		if ((p0+cveroff) >= pe)
		{
			printf("cannot increment beyond top end.\n");
			return;
		}
		cverno++;
		Elf32_Verdef *pvd = (Elf32_Verdef *)(p0+cveroff);
		Elf32_Verdaux *pva = (Elf32_Verdaux *)(p0+cveroff+pvd->vd_aux);
		showverdef(eo, cverno, pvd);
		showverdaux(eo, cverno, csec, pvd, pva);
		break;
	}
	case SHT_SUNW_verneed:
	{
		int maxverno = eo.pshdr(csec).sh_info;
		if ((cverno+1) >= maxverno)
		{
			printf("cannot increment beyond top end.\n");
			return;
		}
		if ((p0+cveroff) >= pe)
		{
			printf("cannot increment beyond top end.\n");
			return;
		}
		Elf32_Verneed *pv = (Elf32_Verneed *)(p0+cveroff);
		if (pv->vn_next == 0)
		{
			printf("cannot increment beyond top end.\n");
			return;
		}
		cveroff += pv->vn_next;
		if ((p0+cveroff) >= pe)
		{
			printf("cannot increment beyond top end.\n");
			return;
		}
		cverno++;
		Elf32_Verneed *pvn = (Elf32_Verneed *)(p0+cveroff);
		Elf32_Vernaux *pva = (Elf32_Vernaux *)(p0+cveroff+pvn->vn_aux);
		showverneed(eo, cverno, csec, pvn);
		showvernaux(eo, cverno, csec, pvn, pva);
		break;
	}
	case SHT_SUNW_versym:
	{
		int symndx = eo.pshdr(csec).sh_link;
		int entsz = eo.pshdr(csec).sh_entsize;;
		int maxverno = eo.pshdr(symndx).sh_size/entsz;
		if ((cverno+1) >= maxverno)
		{
			printf("cannot increment beyond top end.\n");
			return;
		}
		if ((p0+cveroff) >= pe)
		{
			printf("cannot increment beyond top end.\n");
			return;
		}
		cveroff += entsz;
		if ((p0+cveroff) >= pe)
		{
			printf("cannot increment beyond top end.\n");
			return;
		}
		cverno++;
		Elf32_Versym *pvs = (Elf32_Versym *)(p0+cveroff);
		showversym(eo, cverno, csec, pvs);
		break;
	}
	default:
		printf("invalid version type.\n");
		break;
	}
	return;
}

void
editversions(ElfObject &eo, char *)
{
	char s[BUFSIZ];

	// start of symbol editing
	printf("editing versions:\n");

	// initialize version pointers
	int csec = eo.pehdr()->e_shnum;
	int cverno = 0;
	int cveroff = 0;
	int ctype = -1;

	// find the first version table
	for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
	{
		if (eo.pshdr(sec).sh_type == SHT_SUNW_verdef ||
		    eo.pshdr(sec).sh_type == SHT_SUNW_verneed ||
		    eo.pshdr(sec).sh_type == SHT_SUNW_versym)
		{
			csec = sec;
			ctype = eo.pshdr(sec).sh_type;
			break;
		}
	}
	if (csec >= eo.pehdr()->e_shnum)
	{
		printf("no version tables found.\n");
		return;
	}

	// pointers to version tables
	char *p0 = eo.pshdrdata(csec);
	char *pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

	// start interactive loop
	for (int done=0; !done; )
	{
		// get cmd from user
		printf("versions cmd: ");
		rmvnlgets(s);
		tokenize(s, " \t");
		char *pt = gettoken(1);

		// what is the command
		if (pt == NULL || *pt == '\0')
		{
			increment(eo, cverno, cveroff, csec, p0, pe, ctype);
		}
		else if (*pt == 'n')
		{
			// show all section names
			printf("version table names:\n");
			for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
			{
				if (eo.pshdr(sec).sh_type != SHT_SUNW_verdef &&
				    eo.pshdr(sec).sh_type != SHT_SUNW_verneed &&
				    eo.pshdr(sec).sh_type != SHT_SUNW_versym)
					continue;
				printf("section %d: %s (%d, %s)\n", sec,
					eo.shdrnm(sec), eo.pshdr(sec).sh_name,
					ir2s(sh_type, rsh_type,
					eo.pshdr(sec).sh_type));
			}
		}
		else if (*pt == '?' || *pt == 'h')
		{
			printmenu();
		}
		else if (*pt == 'D')
		{
			dflag = !dflag;
			if (dflag)
				printf("demangle mode ON.\n");
			else
				printf("demangle mode OFF.\n");
		}
		else if (*pt == 'r')
		{
			switch (pt[1])
			{
			case 'd':
				ctype = SHT_SUNW_verdef;
				review(eo, cverno, cveroff, csec, p0, pe, ctype, 0);
				break;
			case 's':
				ctype = SHT_SUNW_versym;
				review(eo, cverno, cveroff, csec, p0, pe, ctype, 0);
				break;
			case 'n':
				ctype = SHT_SUNW_verneed;
				review(eo, cverno, cveroff, csec, p0, pe, ctype, 0);
				break;
			default:
				if (pt[1] == '\0')
					review(eo, cverno, cveroff, 
						csec, p0, pe, ctype, 1);
				else
					printf("unknown version type.\n");
				break;
			}
		}
		else if (*pt == 'u')
		{
			update(eo, cverno, cveroff, csec, p0, pe, ctype);
		}
		else if (*pt == '+')
		{
			increment(eo, cverno, cveroff, csec, p0, pe, ctype);
		}
		else if (*pt == 'q')
		{
			done = 1;
		}
		else
		{
			printf("unknown cmd.\n");
		}

	}
	return;
}

