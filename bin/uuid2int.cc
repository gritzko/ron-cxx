#include "ron/uuid.hpp"
#include <iostream>

int main (int argc, char** args ){
    if (argc!=2) return -1;
    ron::Uuid id{args[1]};
    std::cout<<id.str()<<'\t'<<id.value()._64<<'\n';
}
