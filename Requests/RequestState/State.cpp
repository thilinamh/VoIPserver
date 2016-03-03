#include "State.h"
//
// Created by tm on 05/01/16.
//


State::State() {

}

State::~State() {
    std::cout<<"Base State dtor"<<std::endl;
}

bool State::operator==(const State &b) {
    return this==&b;
}

bool State::operator!=(const State &b) {
    return this!=&b;
}
