#include <gtest/gtest.h>

#include "finder.h"

TEST(Finder, Simple) {
    Finder f;
    f.set_directories({"data/test_1"});
    
    f.run();
    
    auto dups = f.get_duplicates();
    
    ASSERT_EQ(dups.size(), 2);
    
    ASSERT_EQ(dups[0].size(), 3);
    ASSERT_TRUE(dups[0].count("data/test_1/test_main.cpp"));
    ASSERT_TRUE(dups[0].count("data/test_1/test_main_copy.cpp"));
    ASSERT_TRUE(dups[0].count("data/test_1/test_main_copy2.cpp"));
   
    ASSERT_EQ(dups[1].size(), 2);
    ASSERT_TRUE(dups[1].count("data/test_1/create_proj.sh"));
    ASSERT_TRUE(dups[1].count("data/test_1/create_proj_copy.sh"));
}


TEST(Finder, MultiDirs) {
    Finder f;
    f.set_directories({ "data/test_1", "data/test_2" });
    
    f.run();
    
    auto dups = f.get_duplicates();
    
    ASSERT_EQ(dups.size(), 2);
    
    ASSERT_EQ(dups[0].size(), 3);
    ASSERT_TRUE(dups[0].count("data/test_1/test_main.cpp"));
    ASSERT_TRUE(dups[0].count("data/test_1/test_main_copy.cpp"));
    ASSERT_TRUE(dups[0].count("data/test_1/test_main_copy2.cpp"));

    ASSERT_EQ(dups[1].size(), 3);
    ASSERT_TRUE(dups[1].count("data/test_1/create_proj.sh"));
    ASSERT_TRUE(dups[1].count("data/test_1/create_proj_copy.sh"));
    ASSERT_TRUE(dups[1].count("data/test_2/create_proj_copy2.sh"));
}

TEST(Finder, RecursiveDirs) {
    Finder f;
    f.set_directories({ "data/test_2", "data/test_3" });
    f.set_level(1);
     
    f.run();
     
    auto dups = f.get_duplicates();
    
    ASSERT_EQ(dups.size(), 2);
    
    ASSERT_EQ(dups[0].size(), 2);
    ASSERT_TRUE(dups[0].count("data/test_3/timus_1545.cpp"));
    ASSERT_TRUE(dups[0].count("data/test_3/test_3_1/timus_1545_.cpp"));
    
    ASSERT_EQ(dups[1].size(), 2);
    ASSERT_TRUE(dups[1].count("data/test_3/timus_1196.cpp"));
    ASSERT_TRUE(dups[1].count("data/test_3/test_3_1/timus_1196_.cpp"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
