
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdlib>
#include "columndefs.cpp"

using namespace std;

/*class DBValue {
public:
    enum Type {INT, FLOAT, TEXT};

    Type type_;
    union {
        int integer;
        float floating;
        char* text;
    } value_;

    //Constructor for int
    DBValue(int value) : type_(INT) {
        value_.integer = value;
    }
    //Constructor for float
    DBValue(float value) : type_(FLOAT) {
        value_.floating = value;
    }
    //Constructor for text
    DBValue(const std::string& value) : type_(TEXT) {
        value_.text = new char[value.length() + 1];
        strcpy(value_.text, value.c_str());
    }
    //encode the values to buffer
    void encode(char* buffer) const {
        switch(type_) {
            case INT:
                memcpy(buffer, &value_.integer, sizeof(int));
                break;
            case FLOAT:
                memcpy(buffer, &value_.floating, sizeof(float));
                break;
            case TEXT:
                memcpy(buffer, value_.text, strlen(value_.text) + 1);
                break;
        }
        }
    //decode the values from buffer
    void decode(const char* buffer) {
        switch(type_) {
            case INT:
                memcpy(&value_.integer, buffer, sizeof(int));
                break;
            case FLOAT:
                memcpy(&value_.floating, buffer, sizeof(float));
                break;
            case TEXT:
                value_.text = new char[strlen(buffer) + 1];
                memcpy(value_.text, buffer, strlen(buffer) + 1);
                break;
        }
        }
    };
class DBINTValue : public DBValue {
public:
    DBINTValue(int value) : DBValue(value) {}
    int getValue() const {
        return value_.integer;

    }
    };
class DBFLOATValue : public DBValue {
public:
    DBFLOATValue(float value) : DBValue(value) {}
    float getValue() const {
        return value_.floating;
    }
    };
class DBTEXTValue : public DBValue {
public:
    DBTEXTValue(const std::string& value) : DBValue(value) {}
    const char* getValue() const {
        return value_.text;
    }
    };
*/


class Database {
public:
    Database() {}
    
    map<string, Table> TableNames_;
    map<int, Table> Tables_;

    void addTable(Table table, string table_name) {
        int last_index = Tables_.rbegin()->first;
        Tables_.insert({last_index + 1, table});
    }

    Table getTable(int index) {
        if (index < 0 || index > Tables_.rbegin()->first) {
            throw std::out_of_range("the hell?");
        }
        else {
            return Tables_[index];
        }
    }

    void eraseTable(int index) {
        if (index < 0 || index > Tables_.rbegin()->first) {
            throw std::out_of_range("the hell?");
        }
        else {
            Tables_.erase(index);
        }
    }

    void clearTable() {
         Tables_.clear();
     }

     void joinTable(Table table1, Table table2) {
         // can primary key va forein key 
     }

    private:
        ColumnDefs columnDefsTables;
        ColumnDefs columnDefsColumns;
        columnDefsTables.addColumn(ColumnDef("TableId", "int", sizeof(int)));
        columnDefsTables.addColumn(ColumnDef("table name", "string", 30));
        columnDefsColumns.addColumn(ColumnDef("ColumnId", "int", sizeof(int)));
        columnDefsColumns.addColumn(ColumnDef("column name", "string", 30));
        columnDefsColumns.addColumn(ColumnDef("column type", "string", 30));
        Table sys_Tables;
        Table sys_Columns;
        Row* row_1 = sys_Tables.createRow({"0", "sys_Tables"});
        Row* row_2 sys_Tables.createRow({"1", "sys_Columns"});
        Row* row_3 sys_Columns.createRow({"0", "TableId", "int"});
        Row* row_4 sys_Columns.createRow({"1", "table name", "string"});
        Row* row_5 sys_Columns.createRow({"2", "ColumnId", "string"});
        Row* row_6 sys_Columns.createRow({"3", "column name", "string"});
        Row* row_7 sys_Columns.createRow({"4", "column type", "string"});
        sys_Tables.addRow(row_1);
        sys_Tables.addRow(row_2);
        sys_Columns.addRow(row_3);
        sys_Columns.addRow(row_4);
        sys_Columns.addRow(row_5);
        sys_Columns.addRow(row_6);
        sys_Columns.addRow(row_7);
};

int main() {
    Database db = new Database();
    return 0;
}

int main() {
    Database db = new Database();
    return 0;
}



class StorageEngine {}; 


