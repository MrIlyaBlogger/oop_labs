#include "../include/oop_lr6.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <sstream>

namespace oop {

namespace {

bool valid_point(const Point& p) {
    return p.x >= kMinCoord && p.x <= kMaxCoord && p.y >= kMinCoord && p.y <= kMaxCoord;
}

}

double distance(const Point& a, const Point& b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

std::string to_string(NPCType type) {
    switch (type) {
    case NPCType::Bear: return "Bear";
    case NPCType::Bittern: return "Bittern";
    case NPCType::Desman: return "Desman";
    }
    return "Unknown";
}

std::optional<NPCType> npc_type_from_string(const std::string& value) {
    std::string lowered;
    lowered.reserve(value.size());
    for (char c : value) {
        lowered.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }

    if (lowered == "bear" || lowered == "1") {
        return NPCType::Bear;
    }
    if (lowered == "bittern" || lowered == "2") {
        return NPCType::Bittern;
    }
    if (lowered == "desman" || lowered == "3") {
        return NPCType::Desman;
    }
    return std::nullopt;
}

NPC::NPC(std::string name, Point pos) : name_(std::move(name)), pos_(pos) {
    if (!valid_point(pos_)) {
        throw std::invalid_argument("Coordinates out of bounds");
    }
}

void NPC::set_position(Point pos) {
    if (!valid_point(pos)) {
        throw std::invalid_argument("Coordinates out of bounds");
    }
    pos_ = pos;
}

double NPC::distance_to(const NPC& other) const noexcept {
    return distance(pos_, other.pos_);
}

void NPC::print(std::ostream& os) const {
    os << to_string(type()) << " \"" << name_ << "\" at (" << pos_.x << ", " << pos_.y << ")";
}

Bear::Bear(std::string name, Point pos) : NPC(std::move(name), pos) {
}

Bittern::Bittern(std::string name, Point pos) : NPC(std::move(name), pos) {
}

Desman::Desman(std::string name, Point pos) : NPC(std::move(name), pos) {
}

FightResult resolve_combat(NPC& first, NPC& second) {
    FightResult result{};

    class FirstVisitor : public NPCVisitor {
    public:
        FirstVisitor(NPC& second, FightResult& res) : second_(second), res_(res) {
        }

        void visit(Bear&) override {
            class SecondVisitor : public NPCVisitor {
            public:
                explicit SecondVisitor(FightResult& res) : res_(res) {
                }

                void visit(Bear&) override {
                }

                void visit(Bittern&) override {
                    res_.second_dies = true;
                }

                void visit(Desman&) override {
                    res_.first_dies = true;
                    res_.second_dies = true;
                }
            private:
                FightResult& res_;
            };

            SecondVisitor sv(res_);
            second_.accept(sv);
        }

        void visit(Bittern&) override {
            class SecondVisitor : public NPCVisitor {
            public:
                explicit SecondVisitor(FightResult& res) : res_(res) {
                }

                void visit(Bear&) override {
                    res_.first_dies = true;
                }

                void visit(Bittern&) override {
                }

                void visit(Desman&) override {
                }
            private:
                FightResult& res_;
            };

            SecondVisitor sv(res_);
            second_.accept(sv);
        }

        void visit(Desman&) override {
            class SecondVisitor : public NPCVisitor {
            public:
                explicit SecondVisitor(FightResult& res) : res_(res) {
                }

                void visit(Bear&) override {
                    res_.first_dies = true;
                    res_.second_dies = true;
                }

                void visit(Bittern&) override {
                }

                void visit(Desman&) override {
                }
            private:
                FightResult& res_;
            };

            SecondVisitor sv(res_);
            second_.accept(sv);
        }

    private:
        NPC& second_;
        FightResult& res_;
    };

    FirstVisitor fv(second, result);
    first.accept(fv);
    return result;
}

void ConsoleObserver::on_event(const std::string& message) {
    std::cout << message << std::endl;
}

FileObserver::FileObserver(const std::string& filename) : filename_(filename) {
}

void FileObserver::on_event(const std::string& message) {
    std::ofstream out(filename_, std::ios::app);
    if (out) {
        out << message << '\n';
    }
}

std::unique_ptr<NPC> NPCFactory::create(NPCType type, std::string name, Point pos) {
    switch (type) {
    case NPCType::Bear: return std::make_unique<Bear>(std::move(name), pos);
    case NPCType::Bittern: return std::make_unique<Bittern>(std::move(name), pos);
    case NPCType::Desman: return std::make_unique<Desman>(std::move(name), pos);
    }
    return nullptr;
}

std::unique_ptr<NPC> NPCFactory::from_stream(std::istream& is) {
    std::string type_str;
    if (!(is >> type_str)) {
        return nullptr;
    }
    std::string name;
    Point pos{};
    if (!(is >> name >> pos.x >> pos.y)) {
        return nullptr;
    }

    auto type = npc_type_from_string(type_str);
    if (!type) {
        return nullptr;
    }

    return create(*type, name, pos);
}

void Dungeon::add_observer(std::shared_ptr<Observer> obs) {
    if (obs) {
        observers_.push_back(std::move(obs));
    }
}

bool Dungeon::name_exists(const std::string& name) const {
    return std::any_of(npcs_.begin(), npcs_.end(), [&](const auto& npc) { return npc->name() == name; });
}

bool Dungeon::add_npc(NPCType type, const std::string& name, Point pos) {
    if (name_exists(name)) {
        return false;
    }
    try {
        auto npc = NPCFactory::create(type, name, pos);
        npcs_.push_back(std::move(npc));
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

void Dungeon::list(std::ostream& os) const {
    if (npcs_.empty()) {
        os << "No NPCs placed.\n";
        return;
    }
    for (const auto& npc : npcs_) {
        npc->print(os);
        os << '\n';
    }
}

bool Dungeon::save(const std::string& filename) const {
    std::filesystem::path p(filename);
    auto dir = p.parent_path();
    if (!dir.empty()) {
        std::error_code ec;
        std::filesystem::create_directories(dir, ec);
        if (ec) {
            return false;
        }
    }
    std::ofstream out(filename);
    if (!out) {
        return false;
    }

    for (const auto& npc : npcs_) {
        const Point& p = npc->position();
        out << to_string(npc->type()) << ' ' << npc->name() << ' ' << p.x << ' ' << p.y << '\n';
    }
    return true;
}

bool Dungeon::load(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        return false;
    }

    std::vector<std::unique_ptr<NPC>> loaded;
    while (true) {
        auto npc = NPCFactory::from_stream(in);
        if (!npc) {
            if (in.eof()) {
                break;
            }
            return false;
        }
        if (std::any_of(loaded.begin(), loaded.end(),
                        [&](const auto& existing) { return existing->name() == npc->name(); })) {
            continue;
        }
        loaded.push_back(std::move(npc));
    }

    npcs_ = std::move(loaded);
    return true;
}

void Dungeon::notify(const std::string& msg) const {
    for (const auto& obs : observers_) {
        obs->on_event(msg);
    }
}

void Dungeon::battle(double range) {
    if (range < 0) {
        range = 0;
    }
    std::vector<bool> dead(npcs_.size(), false);

    for (std::size_t i = 0; i < npcs_.size(); ++i) {
        if (dead[i]) {
            continue;
        }
        for (std::size_t j = i + 1; j < npcs_.size(); ++j) {
            if (dead[j]) {
                continue;
            }
            if (npcs_[i]->distance_to(*npcs_[j]) > range) {
                continue;
            }

            auto res = resolve_combat(*npcs_[i], *npcs_[j]);
            if (res.first_dies && res.second_dies) {
                std::ostringstream ss;
                ss << to_string(npcs_[i]->type()) << " \"" << npcs_[i]->name()
                   << "\" and " << to_string(npcs_[j]->type()) << " \"" << npcs_[j]->name()
                   << "\" killed each other";
                notify(ss.str());
            } else if (res.second_dies) {
                std::ostringstream ss;
                ss << to_string(npcs_[i]->type()) << " \"" << npcs_[i]->name()
                   << "\" killed " << to_string(npcs_[j]->type()) << " \"" << npcs_[j]->name() << "\"";
                notify(ss.str());
            } else if (res.first_dies) {
                std::ostringstream ss;
                ss << to_string(npcs_[j]->type()) << " \"" << npcs_[j]->name()
                   << "\" killed " << to_string(npcs_[i]->type()) << " \"" << npcs_[i]->name() << "\"";
                notify(ss.str());
            }

            if (res.first_dies) {
                dead[i] = true;
            }
            if (res.second_dies) {
                dead[j] = true;
            }
            if (dead[i]) {
                break;
            }
        }
    }

    for (std::size_t idx = dead.size(); idx-- > 0;) {
        if (dead[idx]) {
            npcs_.erase(npcs_.begin() + static_cast<std::ptrdiff_t>(idx));
        }
    }
}

std::vector<NPCType> Dungeon::snapshot_types() const {
    std::vector<NPCType> out;
    out.reserve(npcs_.size());
    for (const auto& npc : npcs_) {
        out.push_back(npc->type());
    }
    return out;
}

}
