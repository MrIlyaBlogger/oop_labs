#include "../include/oop_lr6.hpp"

#include <cstdio>
#include <gtest/gtest.h>
#include <sstream>

using namespace oop;

TEST(NPCFactory, FromStreamCreatesNPC) {
    std::istringstream in("Bear Bob 10 10");
    auto npc = NPCFactory::from_stream(in);
    ASSERT_TRUE(npc);
    EXPECT_EQ(npc->type(), NPCType::Bear);
    EXPECT_EQ(npc->name(), "Bob");
}

TEST(Dungeon, PreventsDuplicateNames) {
    Dungeon d;
    EXPECT_TRUE(d.add_npc(NPCType::Bear, "A", {10, 10}));
    EXPECT_FALSE(d.add_npc(NPCType::Desman, "A", {20, 20}));
    EXPECT_EQ(d.size(), 1);
}

TEST(Dungeon, SaveLoadRoundTrip) {
    Dungeon d;
    d.add_npc(NPCType::Bear, "A", {10, 10});
    d.add_npc(NPCType::Bittern, "B", {30, 30});

    const std::string filename = "tests/tmp_map_lr6.txt";
    ASSERT_TRUE(d.save(filename));

    Dungeon loaded;
    ASSERT_TRUE(loaded.load(filename));
    EXPECT_EQ(loaded.size(), 2);

    std::remove(filename.c_str());
}

TEST(Battle, BearBeatsBittern) {
    Dungeon d;
    d.add_npc(NPCType::Bear, "Boris", {0, 0});
    d.add_npc(NPCType::Bittern, "Vasya", {1, 1});

    d.battle(5.0);
    auto types = d.snapshot_types();
    ASSERT_EQ(types.size(), 1);
    EXPECT_EQ(types[0], NPCType::Bear);
}

TEST(Battle, BearAndDesmanDieTogether) {
    Dungeon d;
    d.add_npc(NPCType::Bear, "B1", {0, 0});
    d.add_npc(NPCType::Desman, "D1", {0, 1});

    d.battle(5.0);
    EXPECT_EQ(d.size(), 0);
}
