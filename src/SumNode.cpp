#include "SumNode.h"

void SumNode::Process() {
    // Lock the output buffer to ensure thread safety
    std::lock_guard<std::mutex> lock(outputs[0]->bufferMutex);

    // Retrieve input values
    float a = inputA.GetFloat();
    float b = inputB.GetFloat();

    // Perform the sum operation
    float result = a + b;

    // Store the result in the output buffer
    outputs[0]->data = result;
    outputs[0]->isDataReady = true;
}

void SumNode::DrawControls() {
    // Implementation for drawing controls
}

