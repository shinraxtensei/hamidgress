#include <iostream>
#include "hamidgress/common/types.h"
#include "hamidgress/common/config.h"

int main() {
    std::cout << "HamidGress - PostgreSQL Clone v0.1.0" << std::endl;
    std::cout << "Page size: " << hamidgress::PAGE_SIZE << " bytes" << std::endl;
    return 0;
}
