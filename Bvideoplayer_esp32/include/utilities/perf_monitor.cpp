// utilities/perf_monitor.cpp
void PerfMonitor::update() {
    static uint32_t last = 0;
    if(millis() - last > 1000) {
        Serial.printf("Heap: %d/%d | CPU: %.1f%%\n", 
            ESP.getFreeHeap(), 
            ESP.getHeapSize(),
            getCpuUsage()
        );
        last = millis();
    }
}

float getCpuUsage() {
    static uint32_t lastIdle = 0;
    uint32_t idle = xTaskGetIdleRunTimeCounter();
    float usage = 1.0 - (idle - lastIdle) / (configTICK_RATE_HZ * 1000.0);
    lastIdle = idle;
    return usage * 100;
}