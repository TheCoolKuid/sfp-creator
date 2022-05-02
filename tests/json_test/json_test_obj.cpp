#include "../../shared/json.h"
#include <iostream>
#include <sstream>

int main(int argc, char** argv) {
    JSON::JObject<char> obj(argv[1]);
    auto val = obj.Value();
    std::stringstream st;
    st << val.at("string")->GetSrc() << "," << val.at("number")->GetSrc() << "," << val.at("bool")->GetSrc();
    if (st.str() == "test,123,true") {
        std::cout << "Pass";
        return 0;
    } else {
        std::cout << "Fail";
        return -1;
    }
    return -1;
}