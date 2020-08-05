#include <iostream>
#include <boost/asio.hpp>

#include "server.hpp"

int main(int argc, char* argv[]) {

    try {
        if (argc != 2) {
            std::cerr << "Usage: join_server <port>\n";
            return 1;
        }
        
        //boost::asio::thread_pool pool(4);

        boost::asio::io_context io_context;

        Server server(io_context, std::atoi(argv[1]));
        
        std::cout << "Join server started..." << std::endl;

        io_context.run();
        
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
    }

    return 0;
}
