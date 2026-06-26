#include "GraphManager.h"

void GraphManager::AddNode(int nodeId, std::shared_ptr<IScomeshNode> node) {
    std::lock_guard<std::mutex> lock(graphMutex);
    node->SetLogger(&logger); // Set the logger for the node
    nodes[nodeId] = node;
}

void GraphManager::RemoveNode(int nodeId) {
    std::lock_guard<std::mutex> lock(graphMutex);
    nodes.erase(nodeId);
}

bool GraphManager::AddConnection(const Connection& connection) {
    if (connection.sourceNodeId == connection.targetNodeId) {
        // Prevent self-connections
        logger.Log(LogSeverity::Error, 
            "Self-connection detected. (Node ID: " + std::to_string(connection.sourceNodeId) + 
            ") (" + __FILE__ + ":" + std::to_string(__LINE__) + ")");
        return false;
    }

    if (connection.sourceNodeId < 0 || connection.targetNodeId < 0) {
        // Prevent connections with negative node IDs
        logger.Log(LogSeverity::Error, 
            "Connection with negative node ID detected. (Source Node ID: " + std::to_string(connection.sourceNodeId) + 
            ", Target Node ID: " + std::to_string(connection.targetNodeId) + 
            ") (" + __FILE__ + ":" + std::to_string(__LINE__) + ")");
        return false;
    }

    if (nodes.find(connection.sourceNodeId) == nodes.end() || nodes.find(connection.targetNodeId) == nodes.end()) {
        // Prevent connections to non-existent nodes
        logger.Log(LogSeverity::Error, 
            "Connection to non-existent node detected. (Source Node ID: " + std::to_string(connection.sourceNodeId) + 
            ", Target Node ID: " + std::to_string(connection.targetNodeId) + 
            ") (" + __FILE__ + ":" + std::to_string(__LINE__) + ")");
        return false;
    }

    if (connection.sourcePinIndex < 0 || connection.targetPinIndex < 0) {
        // Prevent connections with negative pin indices
        logger.Log(LogSeverity::Error, 
            "Connection with negative pin index detected. (Source Node ID: " + std::to_string(connection.sourceNodeId) + 
            ", Source Pin Index: " + std::to_string(connection.sourcePinIndex) + 
            ", Target Node ID: " + std::to_string(connection.targetNodeId) + 
            ", Target Pin Index: " + std::to_string(connection.targetPinIndex) + 
            ") (" + __FILE__ + ":" + std::to_string(__LINE__) + ")");
        return false;
    }

    if (connection.sourcePinIndex >= nodes[connection.sourceNodeId]->GetOutputs().size() ||
        connection.targetPinIndex >= nodes[connection.targetNodeId]->GetParameters().size()) {
        // Prevent connections with out-of-bounds pin indices
        logger.Log(LogSeverity::Error, 
            "Connection pin index out of bounds. (Source Node ID: " + std::to_string(connection.sourceNodeId) +
                   ", Source Pin Index: " + std::to_string(connection.sourcePinIndex) +
                   ", Target Node ID: " + std::to_string(connection.targetNodeId) +
                   ", Target Pin Index: " + std::to_string(connection.targetPinIndex) + 
                   ") (" + __FILE__ + ":" + std::to_string(__LINE__) + ")");
        return false;
    }

    if (std::find(connections.begin(), connections.end(), connection) != connections.end()) {
        // Prevent duplicate connections
        logger.Log(LogSeverity::Error, 
            "Duplicate connection detected. (Source Node ID: " + std::to_string(connection.sourceNodeId) +
                   ", Source Pin Index: " + std::to_string(connection.sourcePinIndex) +
                   ", Target Node ID: " + std::to_string(connection.targetNodeId) +
                   ", Target Pin Index: " + std::to_string(connection.targetPinIndex) + 
                   ") (" + __FILE__ + ":" + std::to_string(__LINE__) + ")");
        return false;
    }

    std::lock_guard<std::mutex> lock(graphMutex);
    connections.push_back(connection);
    nodes[connection.sourceNodeId]->GetOutputs()[connection.sourcePinIndex]->isDataReady = false; // Reset the data ready flag for the source output
    nodes[connection.targetNodeId]->GetParameters()[connection.targetPinIndex]->dynamicInput = nodes[connection.sourceNodeId]->GetOutputs()[connection.sourcePinIndex]; 
    return true;
}

void GraphManager::RemoveConnection(const Connection& connection) {
    std::lock_guard<std::mutex> lock(graphMutex);
    connections.erase(std::remove(connections.begin(), connections.end(), connection), connections.end());
    nodes[connection.targetNodeId]->GetParameters()[connection.targetPinIndex]->dynamicInput = nullptr; // Disconnect the dynamic input
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


