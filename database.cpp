
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdlib>
#include "columndefs.cpp"
#include <filesystem>
#include <dirent.h>

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
    Database() {
       //Set up Default tables:
        createSys_Tables();  
        createSys_Columns(); 
    }

    Database(const string name) {
        //Set a name for the database
        nameOfDatabase = name;

        //Open a directory for the database:
        database_dir = opendir(name.c_str());

        //Check if the directory is opened successfully
        if (!database_dir) {
            cerr << "Failed to open database's directory" << name << endl;
        }

        //Set up Default tables:
        createSys_Tables();  
        createSys_Columns();
    }
    
    map<string, Table> TableNames_;
    map<int, Table> Tables_;

    void createTableByName(string table_name) {}

    Table getTableByName(string table_name) {}

    void eraseTableByName(string table_name) {}

     void joinTable(Table table1, Table table2) {
         // can primary key va forein key 
     }

     Table* getSys_Tables() {
        return sys_Tables;
     }

     Table* getSys_Columns() {
        return sys_Columns;
     }

    private:
        string nameOfDatabase;
        DIR* database_dir;
        ColumnDefs columnDefsTables;
        ColumnDefs columnDefsColumns;
        Table* sys_Tables;
        Table* sys_Columns;

        void createSys_Tables() {
        //set up default sysTables:
            columnDefsTables.addColumn(ColumnDef("TableId", "int", sizeof(int)));
            columnDefsTables.addColumn(ColumnDef("table name", "string", 30));

            sys_Tables = new Table(columnDefsTables);

            sys_Tables -> createRow("sysTables", {"0", "sys_Tables"});
            sys_Tables -> createRow("sysColumns", {"1", "sys_Columns"});
        }

        void createSys_Columns() {
            columnDefsColumns.addColumn(ColumnDef("ColumnId", "int", sizeof(int)));
            columnDefsColumns.addColumn(ColumnDef("column name", "string", 30));
            columnDefsColumns.addColumn(ColumnDef("column type", "string", 30));

            sys_Columns = new Table(columnDefsColumns);

        
            sys_Columns -> createRow("TableId", {"0", "TableId", "int"});
            sys_Columns -> createRow("table name", {"1", "table name", "string"});
            sys_Columns -> createRow("ColumnId", {"2", "ColumnId", "string"});
            sys_Columns -> createRow("column name", {"3", "column name", "string"});
            sys_Columns -> createRow("column type", {"4", "column type", "string"});
    }

    };

class DatabaseEngine {
public:
    DatabaseEngine() {}

    ~DatabaseEngine() {
        // Cleanup the database engine...
    }
    static DatabaseEngine& getInstance() {
    static DatabaseEngine instance;
    return instance;
    }

    Database createDatabase(string name) {
        Database* db = new Database(name);
        return *db;
    }
    Database* getDatabase(string name) {}

private:
    // Disable copy and move constructors
    DatabaseEngine(const DatabaseEngine&) = delete;
    DatabaseEngine& operator=(const DatabaseEngine&) = delete;
    DatabaseEngine(DatabaseEngine&&) = delete;
    DatabaseEngine& operator=(DatabaseEngine&&) = delete;
};

int main() {
    DatabaseEngine db;
    db.getInstance().createDatabase("MySchool").createTableByName("hocsinh");
    db.getInstance().getDatabase("Myschool")->getTableByName("hocsinh").createRow("Phan Nhat Minh", {"Phan Nhat Minh", "01", "18"});
}

 


