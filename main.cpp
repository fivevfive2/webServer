#include "Buffer.hpp"

int main(){
    bre::Buffer* p = new bre::Buffer;
    p->Append("2323");
    std::cout << p->RetrieveAll() << std::endl;
}