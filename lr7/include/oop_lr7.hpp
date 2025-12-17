#ifndef OOP_LR7_HPP
#define OOP_LR7_HPP

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <random>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>
#include <ostream>

namespace oop {

constexpr std::size_t kMapWidth = 100;
constexpr std::size_t kMapHeight = 100;
constexpr double kMapMin = 0.0;
constexpr double kMapMaxX = static_cast<double>(kMapWidth - 1);
constexpr double kMapMaxY = static_cast<double>(kMapHeight - 1);
constexpr std::size_t kInitialNPCCount = 50;
constexpr std::chrono::seconds kSimulationTime{30};

struct Point {
    double x{0.0};
    double y{0.0};
};

double distance(const Point& a, const Point& b);

enum class NPCType { Bear, Bittern, Desman };

struct NPCStats {
    double move_distance{0.0};
    double kill_distance{0.0};
    char symbol{'?'};
};

std::string to_string(NPCType type);
NPCStats stats_for(NPCType type);
bool can_attack(NPCType attacker, NPCType defender);

class NPC {
public:
    NPC(NPCType type, std::string name, Point pos);

    NPCType type() const noexcept { return type_; }
    const std::string& name() const noexcept { return name_; }
    Point position() const noexcept { return pos_; }
    void set_position(Point pos);
    bool alive() const noexcept { return alive_; }
    void kill() noexcept { alive_ = false; }
    double move_distance() const noexcept { return stats_.move_distance; }
    double kill_distance() const noexcept { return stats_.kill_distance; }
    char symbol() const noexcept { return stats_.symbol; }

private:
    NPCType type_;
    std::string name_;
    Point pos_;
    bool alive_{true};
    NPCStats stats_;
};

class Simulator {
public:
    explicit Simulator(std::size_t npc_count = kInitialNPCCount);
    ~Simulator();

    void start();
    void stop();

    void print_map(std::ostream& os) const;
    void print_survivors(std::ostream& os) const;

private:
    void spawn_random_npcs(std::size_t count);
    void movement_worker();
    void fight_worker();
    void enqueue_fight(std::size_t first, std::size_t second);
    bool should_stop() const noexcept { return stop_requested_.load(); }

    mutable std::shared_mutex npcs_mutex_;
    std::vector<NPC> npcs_;

    std::mutex queue_mutex_;
    std::condition_variable queue_cv_;
    std::queue<std::pair<std::size_t, std::size_t>> fight_queue_;

    std::atomic<bool> stop_requested_{false};
    std::thread movement_thread_;
    std::thread fight_thread_;
};

std::mutex& console_mutex();

}

#endif
