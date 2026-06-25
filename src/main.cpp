#include <iostream>
#include "ScoMeshInterface.h"
#include "SumNode.h"

SumNode sumNode(1);

int main(int argc, char** argv){
    std::cout << "Hello, from ScoMesh!\n";
    auto params = sumNode.GetParameters();
    
    params[0]->dynamicInput->data = 3.5f; // Set static value for Input A
    params[1]->dynamicInput->data = 2.5f; // Set static value for

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

        std::cout << ", Static Value: ";
        if (param->expectedType == PinType::Float) {
            std::cout << param->GetFloat();
        } else {
            std::cout << "N/A";
        }

        std::cout << "\n";
    }
    sumNode.Process();

    std::cout << "Output after processing:\n";
    auto outputs = sumNode.GetOutputs();
    for (const auto& output : outputs) {
        std::cout << "Output Type: ";
        switch (output->type) {
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
        std::cout << ", Data Ready: " << (output->isDataReady ? "Yes" : "No") << "\n";
        if (output->isDataReady) {
            std::cout << "Output Value: ";
            if (output->type == PinType::Float) {
                std::cout << std::get<float>(output->data);
            } else {
                std::cout << "N/A";
            }
            std::cout << "\n";
        }
    }
    return 0;
}
