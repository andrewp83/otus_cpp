#include <boost/asio.hpp>

#include "session.hpp"
#include "console_printer.h"
#include "command_publisher.hpp"
#include "executor.h"
#include "file_writer.h"

using boost::asio::ip::tcp;

class Server {
public:
    Server(boost::asio::io_context& io_context, short port, std::size_t bulk_size)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), bulk_size(bulk_size) {
        
        printer = std::make_shared<ConsolePrinter>();
        Publisher<CommandObserver>::add(printer);
        
        writer = std::make_shared<FileWriter>();
        Publisher<CommandObserver>::add(writer);

        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket), bulk_size)->start();
            }

            do_accept();
        });
    }

    tcp::acceptor acceptor_;
    std::size_t bulk_size {0};
    bulk::ExecutorPtr executor;
    std::shared_ptr<ConsolePrinter> printer;
    std::shared_ptr<FileWriter> writer;
};
