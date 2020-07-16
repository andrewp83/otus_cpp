#include <boost/asio.hpp>

#include "executor.h"

using boost::asio::ip::tcp;

class Session
  : public std::enable_shared_from_this<Session> {
      
public:
    Session(tcp::socket socket, std::size_t bulk_size)
      : socket_(std::move(socket)) {
          
          executor = std::make_shared<bulk::Executor>(bulk_size);
    }

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length), [this, self](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                //std::cout << "receive " << length << "=" << std::string{data_, length} << bulk_size << std::endl;
                //do_write(length);
                std::string str = std::string(data_, length);
                executor->parse_buffer(str);
                do_read();
            }
        });
    }

//    void do_write(std::size_t length) {
//        auto self(shared_from_this());
//        boost::asio::async_write(socket_, boost::asio::buffer(data_, length), [this, self](boost::system::error_code ec, std::size_t /*length*/) {
//            if (!ec) {
//                do_read();
//            }
//        });
//    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
      
    bulk::ExecutorPtr executor;
};
