#include <iostream>
#include "ScoMeshInterface.h"
#include "SumNode.h"

SumNode sumNode(1);

int main(int argc, char** argv){
    std::cout << "Hello, from ScoMesh!\n";
    auto params = sumNode.GetParameters();
    for (const auto& param : params) {
        std::cout << "Parameter Name: " << param->name << ", Type: ";
        switch (param->expectedType) {
            case PinType::Float:
                std::cout << "Float";
                break;
            case PinType::Int:
                std::cout << "Int";
                break;
            case PinType::Bool:
                std::cout << "Bool";
                break;
            case PinType::String:
                std::cout << "String";
                break;
            case PinType::FloatArray:
                std::cout << "FloatArray";
                break;
        }
        std::cout << ", Requirement: ";
        switch (param->requirement) {
            case PinRequirement::Optional:
                std::cout << "Optional";
                break;
            case PinRequirement::Required:
                std::cout << "Required";
                break;
        }
        if (param->isExposedAsPin) {
            std::cout << ", Exposed as Pin";
            std::cout << ", Dynamic Input:";
            if (param->dynamicInput) {
                std::cout << " Yes";
            } else {
                std::cout << " No";
            }
            
        } else {
            std::cout << ", Not Exposed as Pin";
        }

        std::cout << "\n";
    }
    sumNode.Process();
    return 0;
}
