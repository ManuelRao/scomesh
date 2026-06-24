#pragma once
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <variant>

enum class PinRequirement {
    Optional,
    Required
};

enum class PinType {
    Float,
    Int,
    Bool,
    String,
    FloatArray 
};

using ScomeshPayload = std::variant<float, int, bool, std::string, std::vector<float>>; // Data payload that can be sent through pins

struct DataBuffer { // Represents a buffer for data that can be shared between nodes
    PinType type;
    ScomeshPayload data;
    bool isDataReady = false;
    std::mutex bufferMutex;
};


struct NodeParameter { // Represents a parameter of a node, which can be exposed as a pin or have a static value
    std::string name;
    PinType expectedType;
    ScomeshPayload staticValue;              
    PinRequirement requirement;
    bool isExposedAsPin;            
    std::shared_ptr<DataBuffer> dynamicInput; 

    // Example: A safe getter for Floats that handles implicit Int->Float conversion
    float GetFloat() {
        if (isExposedAsPin && dynamicInput) {
            // Check what is actually coming down the wire
            if (std::holds_alternative<float>(dynamicInput->data)) {
                return std::get<float>(dynamicInput->data); 
            } 
            else if (std::holds_alternative<int>(dynamicInput->data)) {
                return static_cast<float>(std::get<int>(dynamicInput->data)); 
            }
        }
        // Fallback to the UI slider value
        return std::get<float>(staticValue); 
    }

    int GetInt() {
        if (isExposedAsPin && dynamicInput) {
            if (std::holds_alternative<int>(dynamicInput->data)) {
                return std::get<int>(dynamicInput->data);
            } 
            else if (std::holds_alternative<float>(dynamicInput->data)) {
                return static_cast<int>(std::get<float>(dynamicInput->data));
            }
        }
        return std::get<int>(staticValue);
    }

    bool GetBool() {
        if (isExposedAsPin && dynamicInput) {
            if (std::holds_alternative<bool>(dynamicInput->data)) {
                return std::get<bool>(dynamicInput->data);
            }
        }
        return std::get<bool>(staticValue);
    }

    std::string GetString() {
        if (isExposedAsPin && dynamicInput) {
            if (std::holds_alternative<std::string>(dynamicInput->data)) {
                return std::get<std::string>(dynamicInput->data);
            }
        }
        return std::get<std::string>(staticValue);
    }
};

// 3. The Moldable Node Interface
class IScomeshNode {
public:
    virtual ~IScomeshNode() = default;

    virtual std::string GetName() const = 0;
    virtual int GetId() const = 0;

    // --- Dynamic Parameters ---
    virtual std::vector<NodeParameter*>& GetParameters() = 0;
    virtual std::vector<std::shared_ptr<DataBuffer>>& GetOutputs() = 0;

    // --- Execution ---
    virtual void Process() = 0; 
    virtual void DrawControls() = 0; 
};