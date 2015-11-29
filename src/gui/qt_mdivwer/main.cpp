
#include <assert.h>
#include "TestDataStream.h"

int
main(int argc, char *argv[])
{
    for (int arg=1; arg<argc; ++arg)
    {
        TestDataStream datastream(argv[arg]);
        assert(datastream.isOk());

        assert(datastream.open() == 0);

        TestDataStream::KeyTypeListConstIterator dsit = datastream.begin();
        TestDataStream::KeyTypeListConstIterator dsend = datastream.end();

        for ( ; dsit != dsend; ++dsit)
        {
            std::cout << "Key: " << *dsit << std::endl;

            TestDataStream::DataType data;
            if (datastream.read(*dsit, data) && !data.empty())
            {
                std::cout << "\tData: ";

                TestDataStream::DataTypeConstIterator cit = data.begin();
                TestDataStream::DataTypeConstIterator citend = data.end();
                for ( ; cit != citend; ++cit)
                {
                    std::cout << " " << *cit;
                }
                std::cout << std::endl;
            }
        }
    }

    return(0);
}
