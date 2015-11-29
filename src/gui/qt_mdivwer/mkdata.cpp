// make the data
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>

int
main(int argc, char *argv[])
{
    if (argc != 5 && argc != 6)
    {
        std::cerr << "usage: " << argv[0] 
                  << " keys levels sublevels fields [groups]"
                  << std::endl;
        return(2);
    }
   
    int arg = 1;
    int nkeys = atoi(argv[arg]);
    int nlevels = atoi(argv[++arg]);
    int nsublevels = atoi(argv[++arg]);
    int nfields = atoi(argv[++arg]);

    int ngroups = 1;
    if (argc == 6) ngroups = atoi(argv[++arg]);

    std::ostringstream ofilename;
    ofilename << nkeys << "_";
    ofilename << nlevels << "_";
    ofilename << nsublevels << "_";
    ofilename << nfields << "_";
    ofilename << ngroups << ".dat";

    std::ofstream ofd(ofilename.str().c_str());
    assert(ofd);

    ofd << "key";
    for (int field = 1; field <= nfields; ++field)
    {
        ofd << "\tdata" << field;
    }
    ofd << std::endl;

    for (int group = 1; group <= ngroups; ++group)
    {
        for (int key = 1; key <= nkeys; ++key)
        {
            for (int level = 1; level <= nlevels; ++level)
            {
                for (int sublevel = 1; sublevel <= nsublevels; ++sublevel)
                {
                    for (int sp = 1; sp < level; ++sp)
                    {
                        ofd << "\t";
                    }
                    ofd << "key" << "_" << key;
                    ofd << "_" << level;
                    ofd << "_" << sublevel;
                    for (int field = 1; field <= nfields; ++field)
                    {
                        ofd << "\tdata" << key;
                        ofd << "_" << level;
                        ofd << "_" << sublevel;
                        ofd << "_" << field;
                    }
                    ofd << std::endl;
                }
            }
            for (int level = nlevels-1; level >= 2; --level)
            {
                for (int sublevel = 1; sublevel <= nsublevels; ++sublevel)
                {
                    for (int sp = 1; sp < level; ++sp)
                    {
                        ofd << "\t";
                    }
                    ofd << "key" << "_" << key;
                    ofd << "_" << level;
                    ofd << "_" << sublevel;
                    for (int field = 1; field <= nfields; ++field)
                    {
                        ofd << "\tdata" << key;
                        ofd << "_" << level;
                        ofd << "_" << sublevel;
                        ofd << "_" << field;
                    }
                    ofd << std::endl;
                }
            }
        }
    }

    ofd.close();
    
    return(0);
}
