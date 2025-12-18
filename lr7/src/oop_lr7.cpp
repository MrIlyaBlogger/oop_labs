#include "../include/oop_lr7.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>

namespace oop {

std::mutex& console_mutex() {
    static std::mutex mtx;
    return mtx;
}

namespace {
std::mutex& file_mutex() {
    static std::mutex mtx;
    return mtx;
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

NPCStats stats_for(NPCType type) {
    switch (type) {
    case NPCType::Bear: return {5.0, 10.0, 'B'};
    case NPCType::Bittern: return {50.0, 10.0, 'b'};
    case NPCType::Desman: return {5.0, 20.0, 'd'};
    }
    return {};
}

bool can_attack(NPCType attacker, NPCType defender) {
    switch (attacker) {
    case NPCType::Bear:
        return defender == NPCType::Bittern || defender == NPCType::Desman;
    case NPCType::Bittern:
        return false;
    case NPCType::Desman:
        return defender == NPCType::Bear;
    }
    return false;
}

void ConsoleObserver::on_event(const std::string& message) {
    std::lock_guard<std::mutex> out_lock(console_mutex());
    std::cout << message << std::endl;
}

FileObserver::FileObserver(const std::string& filename) : filename_(filename) {
}

void FileObserver::on_event(const std::string& message) {
    std::lock_guard<std::mutex> lock(file_mutex());
    std::ofstream out(filename_, std::ios::app);
    if (out) {
        out << message << '\n';
    }
}

NPC::NPC(NPCType type, std::string name, Point pos)
    : type_(type), name_(std::move(name)), pos_(pos), stats_(stats_for(type)) {
    pos_.x = std::clamp(pos_.x, kMapMin, kMapMaxX);
    pos_.y = std::clamp(pos_.y, kMapMin, kMapMaxY);
}

void NPC::set_position(Point pos) {
    pos_.x = std::clamp(pos.x, kMapMin, kMapMaxX);
    pos_.y = std::clamp(pos.y, kMapMin, kMapMaxY);
}

Simulator::Simulator(std::size_t npc_count) {
    spawn_random_npcs(npc_count);
}

Simulator::~Simulator() {
    stop();
}

void Simulator::start() {
    stop_requested_.store(false);
    movement_thread_ = std::thread(&Simulator::movement_worker, this);
    fight_thread_ = std::thread(&Simulator::fight_worker, this);
}

void Simulator::stop() {
    if (!stop_requested_.exchange(true)) {
        queue_cv_.notify_all();
    } else {
        queue_cv_.notify_all();
    }
    if (movement_thread_.joinable()) {
        movement_thread_.join();
    }
    if (fight_thread_.joinable()) {
        fight_thread_.join();
    }
}

void Simulator::spawn_random_npcs(std::size_t count) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> pos_x(kMapMin, kMapMaxX);
    std::uniform_real_distribution<double> pos_y(kMapMin, kMapMaxY);
    std::uniform_int_distribution<int> type_dist(0, 2);

    std::lock_guard<std::shared_mutex> lock(npcs_mutex_);
    npcs_.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        auto type = static_cast<NPCType>(type_dist(gen));
        std::string name = to_string(type) + "_" + std::to_string(i + 1);
        npcs_.emplace_back(type, std::move(name), Point{pos_x(gen), pos_y(gen)});
    }
}

void Simulator::add_observer(std::shared_ptr<Observer> obs) {
    if (!obs) {
        return;
    }
    std::lock_guard<std::shared_mutex> lock(observers_mutex_);
    observers_.push_back(std::move(obs));
}

void Simulator::notify(const std::string& msg) const {
    std::shared_lock<std::shared_mutex> lock(observers_mutex_);
    for (const auto& obs : observers_) {
        if (obs) {
            obs->on_event(msg);
        }
    }
}

void Simulator::enqueue_fight(std::size_t first, std::size_t second) {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    fight_queue_.push({first, second});
    queue_cv_.notify_one();
}

void Simulator::movement_worker() {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> step_factor(0.0, 1.0);
    std::uniform_int_distribution<int> axis_dist(0, 1);
    std::uniform_int_distribution<int> dir_dist(0, 1);

    while (!should_stop()) {
        {
            std::lock_guard<std::shared_mutex> lock(npcs_mutex_);
            for (auto& npc : npcs_) {
                if (!npc.alive()) {
                    continue;
                }
                const double step = npc.move_distance() * step_factor(gen);
                Point new_pos = npc.position();
                const double delta = dir_dist(gen) == 0 ? -step : step;
                if (axis_dist(gen) == 0) {
                    new_pos.x = std::clamp(new_pos.x + delta, kMapMin, kMapMaxX);
                } else {
                    new_pos.y = std::clamp(new_pos.y + delta, kMapMin, kMapMaxY);
                }
                npc.set_position(new_pos);
            }

            const std::size_t size = npcs_.size();
            for (std::size_t i = 0; i < size; ++i) {
                if (!npcs_[i].alive()) {
                    continue;
                }
                for (std::size_t j = i + 1; j < size; ++j) {
                    if (!npcs_[j].alive()) {
                        continue;
                    }
                    const double dist = distance(npcs_[i].position(), npcs_[j].position());
                    const double kill_range = std::max(npcs_[i].kill_distance(), npcs_[j].kill_distance());
                    if (dist <= kill_range) {
                        enqueue_fight(i, j);
                    }
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void Simulator::fight_worker() {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> die(1, 6);

    while (true) {
        std::unique_lock<std::mutex> qlock(queue_mutex_);
        queue_cv_.wait(qlock, [&] { return stop_requested_.load() || !fight_queue_.empty(); });
        if (stop_requested_.load() && fight_queue_.empty()) {
            break;
        }
        if (fight_queue_.empty()) {
            continue;
        }
        auto [first_idx, second_idx] = fight_queue_.front();
        fight_queue_.pop();
        qlock.unlock();

        std::lock_guard<std::shared_mutex> data_lock(npcs_mutex_);
        if (first_idx >= npcs_.size() || second_idx >= npcs_.size()) {
            continue;
        }
        NPC& first = npcs_[first_idx];
        NPC& second = npcs_[second_idx];
        if (!first.alive() || !second.alive()) {
            continue;
        }

        const bool first_attack = can_attack(first.type(), second.type());
        const bool second_attack = can_attack(second.type(), first.type());
        if (!first_attack && !second_attack) {
            continue;
        }

        bool first_dies = false;
        bool second_dies = false;

        if (first_attack) {
            const int attack = die(gen);
            const int defense = die(gen);
            if (attack > defense) {
                second_dies = true;
            }
        }

        if (second_attack && !first_dies && second.alive()) {
            const int attack = die(gen);
            const int defense = die(gen);
            if (attack > defense) {
                first_dies = true;
            }
        }

        if (first_dies) {
            first.kill();
        }
        if (second_dies) {
            second.kill();
        }

        if (first_dies || second_dies) {
            std::ostringstream ss;
            if (first_dies && second_dies) {
                ss << to_string(first.type()) << " \"" << first.name() << "\" and "
                   << to_string(second.type()) << " \"" << second.name() << "\" died in battle";
            } else if (first_dies) {
                ss << to_string(second.type()) << " \"" << second.name() << "\" killed "
                   << to_string(first.type()) << " \"" << first.name() << "\"";
            } else if (second_dies) {
                ss << to_string(first.type()) << " \"" << first.name() << "\" killed "
                   << to_string(second.type()) << " \"" << second.name() << "\"";
            }
            notify(ss.str());
        }
    }
}

void Simulator::print_map(std::ostream& os) const {
    std::vector<std::string> grid(kMapHeight, std::string(kMapWidth, '.'));
    {
        std::shared_lock<std::shared_mutex> lock(npcs_mutex_);
        for (const auto& npc : npcs_) {
            if (!npc.alive()) {
                continue;
            }
            std::size_t x = static_cast<std::size_t>(std::lround(npc.position().x));
            std::size_t y = static_cast<std::size_t>(std::lround(npc.position().y));
            if (x >= kMapWidth) x = kMapWidth - 1;
            if (y >= kMapHeight) y = kMapHeight - 1;
            char& cell = grid[y][x];
            cell = (cell == '.') ? npc.symbol() : '*';
        }
    }

    std::lock_guard<std::mutex> out_lock(console_mutex());
    os << "=== Map snapshot ===\n";
    for (const auto& row : grid) {
        os << row << '\n';
    }
    os.flush();
}

void Simulator::print_survivors(std::ostream& os) const {
    std::vector<std::string> survivors;
    {
        std::shared_lock<std::shared_mutex> lock(npcs_mutex_);
        for (const auto& npc : npcs_) {
            if (!npc.alive()) {
                continue;
            }
            std::ostringstream line;
            line << to_string(npc.type()) << " \"" << npc.name() << "\" at ("
                 << npc.position().x << ", " << npc.position().y << ")";
            survivors.push_back(line.str());
        }
    }

    std::lock_guard<std::mutex> out_lock(console_mutex());
    os << "Survivors: " << survivors.size() << '\n';
    if (survivors.empty()) {
        os << "  Nobody survived.\n";
    } else {
        for (const auto& line : survivors) {
            os << "  " << line << '\n';
        }
    }
    os.flush();
}

}
