//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <string>
#include <vector>
#include <iostream>
#include "system/Debug.h"
#include "config/ConfigData.h"
#include "stringutils/StdStringUtils.h"

using namespace ombt;

using namespace std;

class Derived
{
public:
    void separator() {
        cout << "================ " << endl;
    }

    short  _short_value;
    long   _long_value;
    string _string_value;
};

template < >
const ConfigData<Derived>::ParserTableEntry 
      ConfigData<Derived>::_parser_table[] =
{
    PARSERSHORTENTRY(Derived, short_value, "123", true),
    PARSERLONGENTRY(Derived, long_value, "123456789", true),
    PARSERSTRINGENTRY(Derived, string_value, "I'm a string", true),
    PARSERLASTENTRY(),
};

#define BIGDUMP(VAR) cout << #VAR << " = " << VAR << endl

main(int argc, char *argv[])
{
    cout << endl << "ConfigData<Derived> d1 ... " << endl;
    ConfigData<Derived> d1;
    d1.write(cout);
    d1.separator();

    cout << endl << "d1 reset values ... " << endl;
    d1._short_value = -100;
    d1._long_value = 200;
    d1._string_value = "ok to be a horse";
    d1.write(cout);
    d1.separator();

    cout << endl << "d1 putenv ... " << endl;
    d1.put_env();

    cout << endl << "d1 write out test1_cfg_file1.out ... " << endl;
    d1.write("test1_cfg_file1.out");
    d1.separator();

    cout << endl << "d1 write out to stdout ... " << endl;
    d1.separator();
    d1.write(cout);

    cout << endl << "d1 read in test1_cfg_file1.in ... " << endl;
    d1.read("test1_cfg_file1.in");
    d1.write(cout);
    d1.separator();

    cout << endl << "ConfigData<Derived> d2 ... " << endl;
    ConfigData<Derived> d2;
    d1.write(cout);
    d1.separator();

    cout << endl << "d2 = d1, dump d2 ... " << endl;
    d2 = d1;
    d2.write(cout);
    d2.separator();

    cout << endl << "d2 putenv ... " << endl;
    d2.put_env();
    d2.separator();

    cout << endl << "d2 write out test1_cfg_file2.out ... " << endl;
    d2.write("test1_cfg_file2.out");
    d2.separator();

    return(0);
}
