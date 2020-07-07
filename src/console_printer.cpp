//
//  ConsolePrinter.cpp
//  bulk
//
//  Created by a.polyakov on 02.06.2020.
//

#include <iostream>

#include "console_printer.h"

void ConsolePrinter::bulk_executed(const BulkResult& result) {
    std::cout << result.to_string() << std::endl;
}
