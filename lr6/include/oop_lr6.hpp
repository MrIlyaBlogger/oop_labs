#ifndef OOP_LR6_HPP
#define OOP_LR6_HPP

#include <cmath>
#include <fstream>
#include <memory>
#include <optional>
#include <iostream>
#include <string>
#include <vector>

namespace oop {

constexpr double kMinCoord = 0.0;
constexpr double kMaxCoord = 500.0;

struct Point {
    double x{0.0};
    double y{0.0};
};

double distance(const Point& a, const Point& b);

enum class NPCType { Bear, Bittern, Desman };

std::string to_string(NPCType type);
std::optional<NPCType> npc_type_from_string(const std::string& value);

class Bear;
class Bittern;
class Desman;

class NPCVisitor {
public:
    virtual ~NPCVisitor() = default;
    virtual void visit(Bear&) = 0;
    virtual void visit(Bittern&) = 0;
    virtual void visit(Desman&) = 0;
};

class NPC {
public:
    NPC(std::string name, Point pos);
    virtual ~NPC() = default;

    const std::string& name() const noexcept { return name_; }
    const Point& position() const noexcept { return pos_; }
    void set_position(Point pos);
    double distance_to(const NPC& other) const noexcept;

    virtual NPCType type() const noexcept = 0;
    virtual void accept(NPCVisitor& v) = 0;
    virtual void print(std::ostream& os) const;

protected:
    std::string name_;
    Point pos_;
};

class Bear : public NPC {
public:
    Bear(std::string name, Point pos);
    NPCType type() const noexcept override { return NPCType::Bear; }
    void accept(NPCVisitor& v) override { v.visit(*this); }
};

class Bittern : public NPC {
public:
    Bittern(std::string name, Point pos);
    NPCType type() const noexcept override { return NPCType::Bittern; }
    void accept(NPCVisitor& v) override { v.visit(*this); }
};

class Desman : public NPC {
public:
    Desman(std::string name, Point pos);
    NPCType type() const noexcept override { return NPCType::Desman; }
    void accept(NPCVisitor& v) override { v.visit(*this); }
};

struct FightResult {
    bool first_dies{false};
    bool second_dies{false};
};

FightResult resolve_combat(NPC& first, NPC& second);

class Observer {
public:
    virtual ~Observer() = default;
    virtual void on_event(const std::string& message) = 0;
};

class ConsoleObserver : public Observer {
public:
    void on_event(const std::string& message) override;
};

class FileObserver : public Observer {
public:
    explicit FileObserver(const std::string& filename = "log.txt");
    void on_event(const std::string& message) override;
private:
    std::string filename_;
};

class NPCFactory {
public:
    static std::unique_ptr<NPC> create(NPCType type, std::string name, Point pos);
    static std::unique_ptr<NPC> from_stream(std::istream& is);
};

class Dungeon {
public:
    void add_observer(std::shared_ptr<Observer> obs);
    bool add_npc(NPCType type, const std::string& name, Point pos);
    void list(std::ostream& os) const;
    bool save(const std::string& filename) const;
    bool load(const std::string& filename);
    void battle(double range);
    std::size_t size() const noexcept { return npcs_.size(); }
    std::vector<NPCType> snapshot_types() const;

private:
    bool name_exists(const std::string& name) const;
    void notify(const std::string& msg) const;

    std::vector<std::unique_ptr<NPC>> npcs_;
    std::vector<std::shared_ptr<Observer>> observers_;
};

}

#endif
