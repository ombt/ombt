
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <atomic/UseCntPtr.h>
#include <sockets/Socket.h>
#include <sockets/LocalAddress.h>
#include <sockets/EndPoint.h>

using namespace ombt;

int
main(int argc, char *argv[])
{
    std::string host("localhost");
    std::string filepath("localhost");
    int msgs_to_send = 0;

    if (argc == 3)
    {
        filepath = argv[1];
        msgs_to_send = atoi(argv[2]);
    }
    else
    {
        std::cout << "usage: " << argv[0] << " file_path number_of_msgs" << std::endl;
        return(2);
    }

    std::cout << "Creating Datagram Socket ..." << std::endl;
    DatagramSocket *datagram_socket = new DatagramSocket();
    assert(datagram_socket->isOk());

    std::cout << "Creating Local Server Address ..." << std::endl;
    LocalAddress server_address(filepath);
    assert(server_address.isOk());

    std::cout << "Creating Client Server Address ..." << std::endl;
    std::string client_filepath = filepath + "client";
    ::unlink(client_filepath.c_str());
    LocalAddress *client_address = new LocalAddress(client_filepath);
    assert(client_address->isOk());

    std::cout << "Creating Udp EndPoint ..." << std::endl;
    UseCntPtr<EndPoint> connection(new EndPoint(datagram_socket, client_address));
    assert(connection->isOk());

    std::cout << "Binding to address ..." << std::endl;
    assert(connection->bind() == 0);

    char inbuf[BUFSIZ+1];
    char outbuf[BUFSIZ+1];
    size_t count;
    int status;

    for ( ; msgs_to_send > 0; msgs_to_send--)
    {
        strcpy(outbuf, "hello world !!!");
        count = strlen(outbuf);
        count += 1;

        std::cout << "Send message ... " << outbuf << std::endl;
        assert((status = connection->write(outbuf, count, server_address)) >= 0);
        std::cout << "Write status ... " << status << std::endl;

        LocalAddress peer_address;
        std::cout << "Waiting for echo message ..." << std::endl;
        assert((status = connection->read(inbuf, count, peer_address)) >= 0);
        std::cout << "Read status ... " << status << std::endl;

        inbuf[BUFSIZ] = '\0';
        std::cout << "Read ..." << inbuf << std::endl;
    }

    std::cout << "Closing socket ..." << std::endl;
    assert(connection->close() == 0);

    return(0);
}

