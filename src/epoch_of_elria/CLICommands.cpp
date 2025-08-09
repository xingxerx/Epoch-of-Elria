#include "CLICommands.h"
#include <sstream>

namespace elria {

std::optional<Command> CLICommands::parse(const std::string& line) {
    std::istringstream is(line);
    Command cmd;
    if (!(is >> cmd.name)) return std::nullopt;
    std::getline(is, cmd.arg);
    if (!cmd.arg.empty() && cmd.arg[0] == ' ') cmd.arg.erase(0,1);
    return cmd;
}

} // namespace elria

