#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cstring>
#include <stdio.h>

#include "columndefs.cpp"

class TableTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a ColumnDefs object to initialize the Table object
        ColumnDefs columnDefs;
        columnDefs.addColumn(ColumnDef("id", "int", sizeof(int)));
        columnDefs.addColumn(ColumnDef("name", "string", 10));
        columnDefs.addColumn(ColumnDef("age", "int", sizeof(int)));
        table_ = new Table(columnDefs);
    }

    void TearDown() override {
        delete table_;
    }

    Table* table_;
};

TEST_F(TableTest, TestGetName) {
    EXPECT_EQ(table_->getName(), "");
}

TEST_F(TableTest, TestGetColumns) {
    std::vector<ColumnDef> columns = table_->getColumns();
    EXPECT_EQ(columns.size(), 3);
    EXPECT_EQ(columns[0].getName(), "id");
    EXPECT_EQ(columns[1].getName(), "name");
    EXPECT_EQ(columns[2].getName(), "age");
}

TEST_F(TableTest, TestGetColumnDefs) {
    ColumnDefs columnDefs = table_->getColumnDefs();
    EXPECT_EQ(columnDefs.getColumnCount(), 3);
    EXPECT_EQ(columnDefs.getColumnDef(0).getName(), "id");
    EXPECT_EQ(columnDefs.getColumnDef(1).getName(), "name");
    EXPECT_EQ(columnDefs.getColumnDef(2).getName(), "age");
}

TEST_F(TableTest, TestGetRowSize) {
    EXPECT_EQ(table_->getRowSize(), sizeof(int) + 10 + sizeof(int));
}
