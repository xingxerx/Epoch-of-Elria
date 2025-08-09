#pragma once
#include <string>
#include <optional>

namespace elria {

struct Command {
    std::string name;
    std::string arg;
};

class CLICommands {
public:
    static std::optional<Command> parse(const std::string& line);
};

} // namespace elria

