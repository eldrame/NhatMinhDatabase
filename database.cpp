
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdlib>

class DatablockManager {
    public:
        static DatablockManager& getInstance() {
            static DatablockManager instance;
            return instance;
        }
        
        
        int *offset;
    
        // method to allocate new datablock
        void write(int block_num, const char* data) {
            //Write Data to the block
        }

        //method to read datablock
        char * read(int id, int* offset) {
            //read
            //return the buffer
            return nullptr;
        }

    private:
        DatablockManager() {
            //Constructor Logic
        }

        ~DatablockManager() {
        // Release any allocated resources
        }


        //Disable copy amd move constructor
        DatablockManager(const DatablockManager&) = delete;
        DatablockManager(DatablockManager&&) = delete;

        //Disable copy and move assignment operator
        DatablockManager& operator=(const DatablockManager&) = delete;
        DatablockManager& operator=(DatablockManager&&) = delete;

};


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


class Table {
public:

    Table(std::string name, const std::vector<std::string>& columnNames, const std::vector<Column::Type>& columnTypes) {
        if (columnTypes.size() != numOfColumns) {
            throw std::invalid_argument("Incorrect number of column types");
        }

        if (columnNames.size() != numOfColumns) {
            throw std::invalid_argument("Incorrect number of column names");
        }

        columns_ = static_cast<Column*>(malloc(numOfColumns * sizeof(Column)));
        if (columns_ == nullptr) {
            throw std::bad_alloc();
        }

        for (size_t i = 0; i < numOfColumns; i++) {
            new(&columns_[i]) Column(columnNames[i], columnTypes[i]);
        }
    }

    ~Table() {
        std::free(columns_);
    }

    void insertRow(const std::vector<std::string>& row) {
        if (row.size() != numOfColumns) {
            throw std::runtime_error("Invalid row: Number of values does not match number of columns");
        }
        for (size_t i = 0; i < numOfColumns; i++) {
            columns_[i].addValue(row[i]);
        }
    }
    
private:
    std::string name_;
    Column* columns_;
    size_t numOfColumns;
};


class Database {
public:
    Database(const std::string& name) : name_(name) {
        Table tableOfTables("tables", 2);
        tableOfTables.insertRow({"name", "num_columns"});
        tables_.insert({"tables", tableOfTables});

        Table tableOfColumns("tables", 3);
        tableofColumns.insertRow({"table name", "column name", "column type"});
        tables_.insert("columns", tableOfColumns);

    }
    
    void createTable(std::string name, const std::vector<std::string>& columnNames, const std::vector<Column::Type>& columnTypes) {
        if (tables_.count(name) > 0) {
            throw std::invalid_argument("Table already exists");
        }
        Table& table(std::string name, const std::vector<std::string>& columnNames, const std::vector<Column::Type>& columnTypes);

        //Update system tables
        Table& tableOfTables = tables_.at("tables");
        tableOfTables.insertRow({name, std::to_string(columnNames.size())});

        Table& tableOfColumns = tables_.at("columns");
        for (size_t i = 0; i < columnNames.size(); i++) {
            tableOfColumns.insertRow({name, columnNames[i], columnTypes[i]});
        }
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
    }

private:
    StorageEngine() {
        //Constructor logic
    }
};
