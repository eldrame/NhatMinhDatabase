#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cstring>
#include <stdio.h>

#include "columndefs.cpp"

TEST(Rowtest, stoi) {
    EXPECT_EQ(std::stoi("8"), 8);
}

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

TEST_F(TableTest, TestGetColumnDefs) {
    ColumnDefs columnDefs = table_->getColumnDefs();
    EXPECT_EQ(columnDefs.getColumnCount(), 3);
    EXPECT_EQ(columnDefs.getColumnDef(0).getName(), "id");
    EXPECT_EQ(columnDefs.getColumnDef(1).getName(), "name");
    EXPECT_EQ(columnDefs.getColumnDef(2).getName(), "age");
}

TEST_F(TableTest, TestGetRowSize) {
    ColumnDefs columnDefs = table_->getColumnDefs();
    EXPECT_EQ(columnDefs.getRowSize(), sizeof(int) + 30 + sizeof(int));
}


TEST_F(TableTest, TestGetColumns) {

    //std::vector<ColumnDef> columns = table_->getColumns();
    
    EXPECT_EQ(table_->getColumns().size(), 3);
    EXPECT_EQ(table_->getColumns()[0].getName(), "id");
    EXPECT_EQ(table_->getColumns()[1].getName(), "name");

    EXPECT_EQ(table_->getColumns()[2].getName(), "age");

}


class RowTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a ColumnDefs object, a Table object, a Row object to test the row buffer:
        
        columnDefs.addColumn(ColumnDef("id", "int", sizeof(int)));
        columnDefs.addColumn(ColumnDef("name", "string", 10));
        columnDefs.addColumn(ColumnDef("age", "int", sizeof(int)));
        row_ = new Row(columnDefs);
        row_->SetValue({"7", "Phan Nhat Minh", "18"});
    }

    void TearDown() override {
        //delete table_;
        delete row_;
    }
    
    ColumnDefs columnDefs;
    Table* table_;
    Row* row_;
};

TEST_F(RowTest, TestencodeRow) {
    char* buffer = row_->encodeRow();
    EXPECT_EQ(buffer[0], '\a');
    EXPECT_EQ(buffer[1], '\0');
    EXPECT_EQ(buffer[2], '\0');
    EXPECT_EQ(buffer[3], '\0');
    EXPECT_EQ(buffer[4], 'P');
    EXPECT_EQ(buffer[5], 'h');
    EXPECT_EQ(buffer[6], 'a');
    EXPECT_EQ(buffer[7], 'n');
    EXPECT_EQ(buffer[8], ' ');
    EXPECT_EQ(buffer[9], 'N');
    EXPECT_EQ(buffer[10], 'h');
    EXPECT_EQ(buffer[11], 'a');
    EXPECT_EQ(buffer[12], 't');
    EXPECT_EQ(buffer[13], ' ');
    EXPECT_EQ(buffer[14], 'M');
    EXPECT_EQ(buffer[15], 'i');
    EXPECT_EQ(buffer[16], 'n');
    EXPECT_EQ(buffer[17], 'h');
    EXPECT_EQ(buffer[18], '\0');
    EXPECT_EQ(buffer[19], '\0');
    EXPECT_EQ(buffer[20], '\0');
    EXPECT_EQ(buffer[33], '\0');
    EXPECT_EQ(buffer[34], '\x12');
    EXPECT_EQ(buffer[35], '\0');
    EXPECT_EQ(buffer[36], '\0');
    EXPECT_EQ(buffer[37], '\0');
}








