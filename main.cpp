#include <iostream>
#include <boost/asio.hpp>

#include "server.hpp"

int main(int argc, char* argv[]) {

    try {
        if (argc != 3) {
            std::cerr << "Usage: bulk_server <port> <bulk_size>\n";
            return 1;
        }
        
        //boost::asio::thread_pool pool(4);

        boost::asio::io_service io_context;

        Server server(io_context, std::atoi(argv[1]), std::atoi(argv[2]));

        io_context.run();
        
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
    }

    return 0;
}
