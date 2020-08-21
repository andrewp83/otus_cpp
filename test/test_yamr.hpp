#include <gtest/gtest.h>

// // #include "database.h"


// // class DatabaseTest : public ::testing::Test {
// // protected:
// // 	virtual ~DatabaseTest() {
// // 	}

// // 	virtual void SetUp() {
// //         db.get_table("A")->truncate();
// //         db.get_table("B")->truncate();
// // 	}

// // 	virtual void TearDown() {
// // 		// ???
// // 	}
    
// //     my_db::Database db;

// // };

// TEST_F(DatabaseTest, Insert) {
    
// 	// Adjust ...
// 	//
    
//     my_db::QueryResult result = db.query("INSERT A 6 lean");
    
//     ASSERT_TRUE(result.is_ok());
    
//     db.query("INSERT A 2 lean");
//     db.query("INSERT A 10 lean");
//     db.query("INSERT A 11 lean");
//     db.query("INSERT A 7 lean");
//     db.query("INSERT A 5 lean");
//     db.query("INSERT A 8 lean");
    
//     const auto& records = db.get_table("A")->get_records();
    
//     ASSERT_EQ(records[0].get_id(), 2UL);
//     ASSERT_EQ(records[1].get_id(), 5UL);
//     ASSERT_EQ(records[2].get_id(), 6UL);
//     ASSERT_EQ(records[3].get_id(), 7UL);
//     ASSERT_EQ(records[4].get_id(), 8UL);
//     ASSERT_EQ(records[5].get_id(), 10UL);
//     ASSERT_EQ(records[6].get_id(), 11UL);
// }

// // INTERSECTION

// // ОБЕ ТАБЛИЦЫ ПУСТЫЕ
// TEST_F(DatabaseTest, IntersectionEmptyAll) {
//     my_db::QueryResult result = db.query("INTERSECTION");
//     ASSERT_TRUE(result.is_ok());
//     ASSERT_TRUE(result.get_rows().empty());
// }

// // ОДНА ИЗ ТАБЛИЦ ПУСТАЯ

// TEST_F(DatabaseTest, IntersectionEmptyA) {
//     db.get_table("B")->insert(my_db::Record(3, "proposal"));
//     db.get_table("B")->insert(my_db::Record(4, "example"));
//     db.get_table("B")->insert(my_db::Record(5, "lake"));
    
//     my_db::QueryResult result = db.query("INTERSECTION");
    
//     const auto& rows = result.get_rows();
//     ASSERT_EQ(rows.size(), 0UL);
// }

// TEST_F(DatabaseTest, IntersectionEmptyB) {
//     db.get_table("A")->insert(my_db::Record(3, "proposal"));
//     db.get_table("A")->insert(my_db::Record(4, "example"));
//     db.get_table("A")->insert(my_db::Record(5, "lake"));
    
//     my_db::QueryResult result = db.query("INTERSECTION");
    
//     const auto& rows = result.get_rows();
//     ASSERT_EQ(rows.size(), 0UL);
// }

// // ТАБЛИЦЫ ОДИНАКОВЫЕ (по ID)

// TEST_F(DatabaseTest, IntersectionEqual) {
    
//     db.get_table("A")->insert(my_db::Record(3, "lean"));
//     db.get_table("A")->insert(my_db::Record(4, "sweater"));
//     db.get_table("A")->insert(my_db::Record(5, "frank"));
    
//     db.get_table("B")->insert(my_db::Record(3, "proposal"));
//     db.get_table("B")->insert(my_db::Record(4, "example"));
//     db.get_table("B")->insert(my_db::Record(5, "lake"));
    
//     my_db::QueryResult result = db.query("INTERSECTION");
    
//     const auto& rows = result.get_rows();
    
//     ASSERT_EQ(rows.size(), 3UL);
//     ASSERT_EQ(rows[0], "3,lean,proposal");
//     ASSERT_EQ(rows[1], "4,sweater,example");
//     ASSERT_EQ(rows[2], "5,frank,lake");
// }

// // ТАБЛИЦЫ ПОЛНОСТЬЮ РАЗНЫЕ

// TEST_F(DatabaseTest, IntersectionDifferent) {
    
//     db.get_table("A")->insert(my_db::Record(3, "lean"));
//     db.get_table("A")->insert(my_db::Record(4, "sweater"));
//     db.get_table("A")->insert(my_db::Record(5, "frank"));
    
//     db.get_table("B")->insert(my_db::Record(6, "proposal"));
//     db.get_table("B")->insert(my_db::Record(7, "example"));
//     db.get_table("B")->insert(my_db::Record(8, "lake"));
    
//     my_db::QueryResult result = db.query("INTERSECTION");
    
//     const auto& rows = result.get_rows();
    
//     ASSERT_EQ(rows.size(), 0UL);
// }

// // ОБЫЧНЫЙ СЛУЧАЙ (ЧАСТИЧНОЕ ПЕРЕСЕЧЕНИЕ)

// TEST_F(DatabaseTest, Intersection) {
    
//     db.get_table("A")->insert(my_db::Record(3, "lean"));
//     db.get_table("A")->insert(my_db::Record(4, "sweater"));
//     db.get_table("A")->insert(my_db::Record(5, "frank"));
    
//     db.get_table("B")->insert(my_db::Record(4, "proposal"));
//     db.get_table("B")->insert(my_db::Record(5, "example"));
//     db.get_table("B")->insert(my_db::Record(6, "lake"));
    
//     my_db::QueryResult result = db.query("INTERSECTION");
    
//     const auto& rows = result.get_rows();
    
//     ASSERT_EQ(rows.size(), 2UL);
//     ASSERT_EQ(rows[0], "4,sweater,proposal");
//     ASSERT_EQ(rows[1], "5,frank,example");
// }


// // SYMMETRIC DIFFERENCE

// // ОБЕ ТАБЛИЦЫ ПУСТЫЕ
// TEST_F(DatabaseTest, SymDiffEmptyAll) {
//     my_db::QueryResult result = db.query("SYMMETRIC_DIFFERENCE");
//     ASSERT_TRUE(result.is_ok());
//     ASSERT_TRUE(result.get_rows().empty());
// }

// // ОДНА ИЗ ТАБЛИЦ ПУСТАЯ

// TEST_F(DatabaseTest, SymDiffEmptyA) {
//     db.get_table("B")->insert(my_db::Record(3, "proposal"));
//     db.get_table("B")->insert(my_db::Record(4, "example"));
//     db.get_table("B")->insert(my_db::Record(5, "lake"));
    
//     my_db::QueryResult result = db.query("SYMMETRIC_DIFFERENCE");
    
//     const auto& rows = result.get_rows();
//     ASSERT_EQ(rows.size(), 3UL);
//     ASSERT_EQ(rows[0], "3,,proposal");
//     ASSERT_EQ(rows[1], "4,,example");
//     ASSERT_EQ(rows[2], "5,,lake");
// }

// TEST_F(DatabaseTest, SymDiffEmptyB) {
//     db.get_table("A")->insert(my_db::Record(3, "proposal"));
//     db.get_table("A")->insert(my_db::Record(4, "example"));
//     db.get_table("A")->insert(my_db::Record(5, "lake"));
    
//     my_db::QueryResult result = db.query("SYMMETRIC_DIFFERENCE");
    
//     const auto& rows = result.get_rows();
//     ASSERT_EQ(rows.size(), 3UL);
    
//     ASSERT_EQ(rows[0], "3,proposal,");
//     ASSERT_EQ(rows[1], "4,example,");
//     ASSERT_EQ(rows[2], "5,lake,");
// }

// // ТАБЛИЦЫ ОДИНАКОВЫЕ (по ID)

// TEST_F(DatabaseTest, SymDiffEqual) {
    
//     db.get_table("A")->insert(my_db::Record(3, "lean"));
//     db.get_table("A")->insert(my_db::Record(4, "sweater"));
//     db.get_table("A")->insert(my_db::Record(5, "frank"));
    
//     db.get_table("B")->insert(my_db::Record(3, "proposal"));
//     db.get_table("B")->insert(my_db::Record(4, "example"));
//     db.get_table("B")->insert(my_db::Record(5, "lake"));
    
//     my_db::QueryResult result = db.query("SYMMETRIC_DIFFERENCE");
    
//     const auto& rows = result.get_rows();
    
//     ASSERT_EQ(rows.size(), 0UL);
// }

// // ТАБЛИЦЫ ПОЛНОСТЬЮ РАЗНЫЕ

// TEST_F(DatabaseTest, SymDiffDifferent) {
    
//     db.get_table("A")->insert(my_db::Record(3, "lean"));
//     db.get_table("A")->insert(my_db::Record(4, "sweater"));
//     db.get_table("A")->insert(my_db::Record(5, "frank"));
    
//     db.get_table("B")->insert(my_db::Record(6, "proposal"));
//     db.get_table("B")->insert(my_db::Record(7, "example"));
//     db.get_table("B")->insert(my_db::Record(8, "lake"));
    
//     my_db::QueryResult result = db.query("SYMMETRIC_DIFFERENCE");
    
//     const auto& rows = result.get_rows();
    
//     ASSERT_EQ(rows.size(), 6UL);
//     ASSERT_EQ(rows[0], "3,lean,");
//     ASSERT_EQ(rows[1], "4,sweater,");
//     ASSERT_EQ(rows[2], "5,frank,");
//     ASSERT_EQ(rows[3], "6,,proposal");
//     ASSERT_EQ(rows[4], "7,,example");
//     ASSERT_EQ(rows[5], "8,,lake");
// }

// // ОБЫЧНЫЙ СЛУЧАЙ (ЧАСТИЧНОЕ ПЕРЕСЕЧЕНИЕ)

// TEST_F(DatabaseTest, SymDiff) {
    
//     db.get_table("A")->insert(my_db::Record(3, "lean"));
//     db.get_table("A")->insert(my_db::Record(4, "sweater"));
//     db.get_table("A")->insert(my_db::Record(5, "frank"));
    
//     db.get_table("B")->insert(my_db::Record(4, "proposal"));
//     db.get_table("B")->insert(my_db::Record(5, "example"));
//     db.get_table("B")->insert(my_db::Record(6, "lake"));
    
//     my_db::QueryResult result = db.query("SYMMETRIC_DIFFERENCE");
    
//     const auto& rows = result.get_rows();
    
//     ASSERT_EQ(rows.size(), 2UL);
//     ASSERT_EQ(rows[0], "3,lean,");
//     ASSERT_EQ(rows[1], "6,,lake");
// }
