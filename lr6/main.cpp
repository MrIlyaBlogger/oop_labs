#include "include/oop_lr6.hpp"

#include <iostream>
#include <memory>
#include <string>

using namespace oop;

int main() {
    Dungeon dungeon;
    dungeon.add_observer(std::make_shared<ConsoleObserver>());
    dungeon.add_observer(std::make_shared<FileObserver>());

    std::cout << "Balagur Fate 3 Dungeon Editor\n";
    std::cout << "Commands:\n";
    std::cout << "  add <type> <name> <x> <y>   (type: bear/bittern/desman)\n";
    std::cout << "  list\n";
    std::cout << "  save <file>\n";
    std::cout << "  load <file>\n";
    std::cout << "  battle <range>\n";
    std::cout << "  exit\n";

    std::string cmd;
    while (std::cout << "> " && std::cin >> cmd) {
        if (cmd == "add") {
            std::string type_str, name;
            Point p{};
            std::cin >> type_str >> name >> p.x >> p.y;
            auto type = npc_type_from_string(type_str);
            if (!type) {
                std::cout << "Unknown type.\n";
                continue;
            }
            if (!dungeon.add_npc(*type, name, p)) {
                std::cout << "Cannot add NPC (duplicate name or bad coords).\n";
            } else {
                std::cout << "NPC added.\n";
            }
        } else if (cmd == "list") {
            dungeon.list(std::cout);
        } else if (cmd == "save") {
            std::string file;
            std::cin >> file;
            if (dungeon.save(file)) {
                std::cout << "Saved to " << file << "\n";
            } else {
                std::cout << "Save failed.\n";
            }
        } else if (cmd == "load") {
            std::string file;
            std::cin >> file;
            if (dungeon.load(file)) {
                std::cout << "Loaded " << dungeon.size() << " NPCs.\n";
            } else {
                std::cout << "Load failed.\n";
            }
        } else if (cmd == "battle") {
            double range = 0.0;
            std::cin >> range;
            dungeon.battle(range);
            std::cout << "Battle ended. Survivors: " << dungeon.size() << "\n";
        } else if (cmd == "exit") {
            break;
        } else {
            std::cout << "Unknown command.\n";
        }
    }

    return 0;
}
