//
// Created by rajitha on 2/11/19.
//

#include <agent_math.h>

agent_math::agent_math() {}

agent_math::agent_math(std::string string) : length(string) {}

int agent_math::max(int num1, int num2) {
    // local variable declaration
    int result;

    if (num1 > num2)
        result = num1;
    else
        result = num2;

    return result;
}

std::string agent_math::getNum() {
    return length;
}
