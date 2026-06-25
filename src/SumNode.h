# pragma once
#include "ScoMeshInterface.h"
#include <iostream>

class SumNode : public IScomeshNode {
    private:
        int id;

        NodeParameter inputA;
        NodeParameter inputB;

        std::vector<NodeParameter*> parameters;
        std::vector<std::shared_ptr<DataBuffer>> outputs;
    
    public:
        SumNode(int nodeId) : id(nodeId) {
            id = nodeId;
            inputA.name = "Input A";
            inputA.expectedType = PinType::Float;
            inputA.staticValue = 0.0f;
            inputA.requirement = PinRequirement::Required;
            inputA.isExposedAsPin = true;
            inputA.dynamicInput = std::make_shared<DataBuffer>();
            inputA.dynamicInput->type = PinType::Float;

            inputB.name = "Input B";
            inputB.expectedType = PinType::Float;
            inputB.staticValue = 0.0f;
            inputB.requirement = PinRequirement::Required;
            inputB.isExposedAsPin = true;
            inputB.dynamicInput = std::make_shared<DataBuffer>();
            inputB.dynamicInput->type = PinType::Float;

            parameters.push_back(&inputA);
            parameters.push_back(&inputB);

            // Initialize output buffer
            auto outputBuffer = std::make_shared<DataBuffer>();
            outputBuffer->type = PinType::Float;
            outputs.push_back(outputBuffer);
        }

        std::string GetName() const override {return "Sum Node";}

        int GetId() const override {return id;}

        std::vector<NodeParameter*>& GetParameters() override {return parameters;}

        std::vector<std::shared_ptr<DataBuffer>>& GetOutputs() override {return outputs;}

        void Process() override;
        void DrawControls() override;
};
