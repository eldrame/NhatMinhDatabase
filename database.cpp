
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdlib>

class DBValue {
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
    };
    //Constructor for float
    DBValue(float value) : type_(FLOAT) {
        value_.floating = value;
    };
    //Constructor for text
    DBValue(const std::string& value) : type_(TEXT) {
        value_.text = new char[value.length() + 1];
        strcpy(value_.text, value.c_str());
    };
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
        };
        };
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
        };
        };
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

// class DatablockManager {
//     public:
//         static DatablockManager& getInstance() {
//             static DatablockManager instance;
//             return instance;
//         }
        
        
//         int *offset;
    
//         // method to allocate new datablock
//         void write(int block_num, const char* data) {
//             //Write Data to the block
//         }

//         //method to read datablock
//         char * read(int id, int* offset) {
//             //read
//             //return the buffer
//             return nullptr;
//         }

//     private:
//         DatablockManager() {
//             //Constructor Logic
//         }

//         ~DatablockManager() {
//         // Release any allocated resources
//         }


//         //Disable copy amd move constructor
//         DatablockManager(const DatablockManager&) = delete;
//         DatablockManager(DatablockManager&&) = delete;

//         //Disable copy and move assignment operator
//         DatablockManager& operator=(const DatablockManager&) = delete;
//         DatablockManager& operator=(DatablockManager&&) = delete;

// };


//structure for column Definition:


class Column {
public:
    enum Type {INT, FLOAT, TEXT};
    
    Column(const std::string& name, Type type)
        : name_(name), type_(type) {
            values_ = static_cast<std::string*>(std::malloc(0));
        }

    ~Column() {
        std::free(values_);
    }
    
    void addValue(const std::string& value) {
        const size_t size = sizeOfType(type_);
        void* newValue = realloc(values_, size * (numOfRows_ + 1));
        if (newValue = nullptr) {
            throw std::bad_alloc();
        }
        values_ = newValue;
        char* p = reinterpret_cast<char*>(values_);
        p += size * numOfRows_;
        switch(type_) {
            case INT:
                *reinterpret_cast<int*>(p) = std::stoi(value);
            case FLOAT:
                *reinterpret_cast<float*>(p) = std::stof(value);
            case TEXT:
                memcpy(p, value.c_str(), value.length());
                *(p + value.length()) = '\0' ;
                break;
        }
        numOfRows_++;   
    }

    const std::string& getName() const {
        return name_;
    }

    Type getType() const {
        return type_;
    }

    const void* values() const {
        return values_;
    }

    size_t numOfRows() const {
        return numOfRows_;
    }
private:
    std::string name_;
    Type type_;
    void* values_;
    size_t numOfRows_ = 0;

    static size_t sizeOfType(Type type) {
        switch(type) {
            case INT:
                return sizeof(int);
            case FLOAT:
                return sizeof(float);
            case TEXT:
                return 256;
            default:
                throw std::invalid_argument("Invalid column type");
        }
    }
};

class Row{
//a hasmap of column name and value
public:
    std::unordered_map<std::string, std::string> row;
    void addValue(const std::string& name, const std::string& value) {
        row[name] = value;
    }
    std::string getValue(const std::string& name) {
        return row[name];
    }
    //function to get Field's value by field name
    void GetFieldValue(const std::string& name) {
        std::cout << row[name] << std::endl;
    }
    //function to set Field's value by field name
    void SetFieldValue(const std::string& name, const std::string& value) {
        row[name] = value;
    }
};





class Database {
public:
    Database(const std::string& name) : name_(name) {
        // Table tableOfTables("sys.tables", {"table_name"}, {Column::TEXT});
        // tableOfTables.insertRow({"sys.tables"});
        // tables_.insert({"tables", tableOfTables});

        // Table tableOfColumns("sys.columns", {"column name", "column type", "table name"}, {Column::TEXT, Column::TEXT, Column::TEXT});
        

    }

    Table getTable(const std::string& name) {
        if (tables_.count(name) == 0) {
            throw std::invalid_argument("Table does not exist");
        }
        return tables_.at(name);
    }
    
    void createTable(std::string name, const std::vector<std::string>& columnNames, const std::vector<Column::Type>& columnTypes) {
        if (tables_.count(name) > 0) {
            throw std::invalid_argument("Table already exists");
        }
        Table& table(std::string name, const std::vector<std::string>& columnNames, const std::vector<Column::Type>& columnTypes);

        //Update system tables
        Table& tableOfTables = tables_.at("tables");
        tableOfTables.insertRow({name});

        Table& tableOfColumns = tables_.at("columns");
        for (size_t i = 0; i < columnNames.size(); i++) {
            tableOfColumns.insertRow({name, columnNames[i], columnTypes[i]});
        }
    }
    void dropTable(const std::string& name) {
        if (tables_.count(name) == 0) {
            throw std::invalid_argument("Table does not exist");
        }
        tables_.erase(name);
    }

private:
    std::string name_;
    std::map<std::string, Table> tables_;
};


class StorageEngine {
public:
    static StorageEngine& getInstance() {
        static StorageEngine instance;
        return instance;
    }

    void createDatabase(const std::string& name) {
        Database db(name);
        db.getTable("hocsinh").insertRow({"1", "2", "3"});
        
        db.getTable("hocsinh").getRow(0).SetFieldValue("name", "phan nhat minh");
        db.getTable("hocsinh").getRow(0).GetFieldValue("name");
        //Database db(name);
    }

private:
    StorageEngine() {
        //Constructor logic
    }
};
