#include "GraphManager.h"

void GraphManager::AddNode(int nodeId, std::shared_ptr<IScomeshNode> node) {
    std::lock_guard<std::mutex> lock(graphMutex);
    nodes[nodeId] = node;
}

void GraphManager::RemoveNode(int nodeId) {
    std::lock_guard<std::mutex> lock(graphMutex);
    nodes.erase(nodeId);
}

void GraphManager::AddConnection(const Connection& connection) {
    std::lock_guard<std::mutex> lock(graphMutex);
    connections.push_back(connection);
}

void GraphManager::RemoveConnection(const Connection& connection) {
    std::lock_guard<std::mutex> lock(graphMutex);
    connections.erase(std::remove(connections.begin(), connections.end(), connection), connections.end());
}

bool GraphManager::CalculateFlow() {
    std::lock_guard<std::mutex> lock(graphMutex);
    
    return true;
}

bool GraphManager::ProcessAllNodes() {
    std::lock_guard<std::mutex> lock(graphMutex);
    for (const auto& [nodeId, node] : nodes) {
        node->Process();
    }
    return true;
}


