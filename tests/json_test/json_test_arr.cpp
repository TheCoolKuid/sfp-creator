#include "../../shared/json.h"
#include <iostream>
#include <sstream>

int main(int argc, char** argv) {
    JSON::JArray<char> obj(argv[1]);
    auto val = obj.Value();
    std::stringstream st;
    
    for(const auto& elem : val) {
        st << elem.second->GetSrc() << ",";
    }

    if(st.str() == "abc,12,true,")
    {
        std::cout << "Pass";
        return 0;
    }else{
        std::cout << "Fail " << " Return " << st.str();
        return -1;
    }

    return -1;
}