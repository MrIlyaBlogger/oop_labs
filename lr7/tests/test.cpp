#include "../include/oop_lr7.hpp"

#include <gtest/gtest.h>

using namespace oop;

TEST(Stats, MoveAndKillDistances) {
    EXPECT_DOUBLE_EQ(stats_for(NPCType::Bear).move_distance, 5.0);
    EXPECT_DOUBLE_EQ(stats_for(NPCType::Bear).kill_distance, 10.0);

    EXPECT_DOUBLE_EQ(stats_for(NPCType::Bittern).move_distance, 50.0);
    EXPECT_DOUBLE_EQ(stats_for(NPCType::Bittern).kill_distance, 10.0);

    EXPECT_DOUBLE_EQ(stats_for(NPCType::Desman).move_distance, 5.0);
    EXPECT_DOUBLE_EQ(stats_for(NPCType::Desman).kill_distance, 20.0);
}

TEST(CombatRules, CanAttackMatrix) {
    EXPECT_TRUE(can_attack(NPCType::Bear, NPCType::Bittern));
    EXPECT_TRUE(can_attack(NPCType::Bear, NPCType::Desman));
    EXPECT_FALSE(can_attack(NPCType::Bear, NPCType::Bear));

    EXPECT_FALSE(can_attack(NPCType::Bittern, NPCType::Bear));
    EXPECT_FALSE(can_attack(NPCType::Bittern, NPCType::Desman));
    EXPECT_FALSE(can_attack(NPCType::Bittern, NPCType::Bittern));

    EXPECT_TRUE(can_attack(NPCType::Desman, NPCType::Bear));
    EXPECT_FALSE(can_attack(NPCType::Desman, NPCType::Bittern));
    EXPECT_FALSE(can_attack(NPCType::Desman, NPCType::Desman));
}

TEST(NPC, ClampsPositionWithinMap) {
    NPC npc(NPCType::Bear, "B", {200.0, -50.0});
    EXPECT_LE(npc.position().x, kMapMaxX);
    EXPECT_GE(npc.position().y, kMapMin);

    npc.set_position({-10.0, 300.0});
    EXPECT_DOUBLE_EQ(npc.position().x, kMapMin);
    EXPECT_DOUBLE_EQ(npc.position().y, kMapMaxY);
}
