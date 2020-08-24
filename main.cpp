#include <iostream>

#include "mapper.h"
#include "mail_mapper.h"
#include "mail_reducer.h"
#include "job.h"

int main(int argc, char* argv[]) {

    try {
        if (argc != 4) {
            std::cerr << "Usage: yamr <src> <mnum> <rnum>\n";
            return 1;
        }
        
        std::string src_filename = argv[1];
        std::size_t mnum = std::stoi(argv[2]);
        std::size_t rnum = std::stoi(argv[3]);
        
        MailMapper* mapper = new MailMapper(src_filename, 3);
        MailReducer* reducer = new MailReducer();
        
        struct BinaryOp {
            bool operator()(bool l, bool r) const {
                return l && r;
            }
        };
        
        mr::Job<MailString, bool, BinaryOp, MailContainer> job(mapper, reducer);
        job.set_map_workers_count(mnum);
        job.set_reduce_workers_count(rnum);
        job.wait_for_completion();
        
        std::cout << job.get_result() << std::endl;
        
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
    }

    return 0;
}
