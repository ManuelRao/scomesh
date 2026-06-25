# pragma once
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include "ScoMeshInterface.h"

struct Connection {
    int sourceNodeId;
    int sourcePinIndex;
    int targetNodeId;
    int targetPinIndex;
};

class GraphManager {
private:
    std::unordered_map<int, std::shared_ptr<IScomeshNode>> nodes;
    std::vector<Connection> connections;
    std::unordered_map<int, std::vector<Connection>> nodeConnections; // Maps node IDs to their connections
    std::unordered_map<int, std::vector<int>> nodeDependencies; // Maps node IDs to the IDs of nodes they depend on
    std::unordered_map<int, std::vector<int>> nodeDependents; // Maps node IDs to the IDs of nodes that depend on them
    std::unordered_map<int, bool> nodeExecutionStatus; // Maps node IDs to their execution status (true if executed, false otherwise)
    std::vector<int> executionOrder; // Stores the order in which nodes should be executed
    mutable std::mutex graphMutex;

public:
    GraphManager() = default;
    ~GraphManager() = default;

    std::vector<std::shared_ptr<IScomeshNode>> GetAllNodes() const;

    void AddNode(int nodeId, std::shared_ptr<IScomeshNode> node);
    void RemoveNode(int nodeId);
    std::shared_ptr<IScomeshNode> GetNode(int nodeId);

    void AddConnection(const Connection& connection);
    void RemoveConnection(const Connection& connection);
    const std::vector<Connection>& GetConnections() const;

    bool CalculateFlow();
    bool ProcessAllNodes();
    bool ExecuteNode(int nodeId);
    bool ValidateGraph() const;

};