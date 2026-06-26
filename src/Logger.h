#pragma once
#include <iostream>
#include <string>
#include <mutex>
#include "ScomeshInterface.h"

class ScomeshLogger : public ILogger {
private:
    std::mutex logMutex; // Protects the console from multi-threaded overlapping

public:
    void Log(LogSeverity severity, const std::string& msg) override {
        // Lock the console until this specific message is completely printed
        std::lock_guard<std::mutex> lock(logMutex);
        
        switch (severity) {
            case LogSeverity::Info:    
                std::cout << "[INFO]  " << msg << "\n"; 
                break;
            case LogSeverity::Warning: 
                std::cout << "[WARN]  " << msg << "\n"; 
                break;
            case LogSeverity::Error:   
                std::cerr << "[ERROR] " << msg << "\n"; 
                break;
            case LogSeverity::Fatal:   
                std::cerr << "[FATAL] " << msg << "\n"; 
                break;
        }
    }
};