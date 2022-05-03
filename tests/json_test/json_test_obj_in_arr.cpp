#include "../../shared/json.h"
#include <iostream>
#include <sstream>

int main(int argc, char** argv) {
    JSON::JArray<char> obj(argv[1]);
    auto val = obj.Value();
    std::stringstream st;
    
    for(const auto& elem : val) {
        st << elem.second->Value().at("field")->GetSrc() << ",";
    }

    if(st.str() == "filed1,filed2,filed3,filed4,")
    {
        std::cout << "Pass";
        return 0;
    }else{
        std::cout << "Fail " << " Return " << st.str();
        return -1;
    }

    return -1;
}