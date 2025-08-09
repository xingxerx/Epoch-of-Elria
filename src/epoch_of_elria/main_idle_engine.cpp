// main_idle_engine.cpp - Minimal C++ idle engine entrypoint (ported concept from Rust)
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "MetaCognitionLayer.h"
#include "ArchReactorAIlogic.h"
#include "CLICommands.h"

int main() {
    using clock = std::chrono::high_resolution_clock;

    std::cout << "[Epoch of Elria] Idle Engine (C++)\n";
    std::cout << "Type commands or press Ctrl+C to quit.\n";

    elria::UI ui;
    elria::GameStateContext game_state;
    elria::ArchReactorAIlogic ai;
    ai.reset();

    auto last = clock::now();

    // Simple loop at ~20 Hz to keep console output readable
    while (true) {
        auto now = clock::now();
        std::chrono::duration<double> dt = now - last;
        last = now;

        // Tick AI and update UI status
        ai.tick(dt.count());
        ui.setStatus(ai.status());

        // Update higher-level game state
        game_state.update(ui, dt.count());

        // Render/UI (console placeholder)
        ui.draw();

        // Non-blocking read of a line not trivial in pure standard C++;
        // keep loop simple and low frequency. Future improvement: use Win32 or select/poll.

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}

