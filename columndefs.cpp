#include <string>
#include<vector>
#include<map>
#include<iostream>
#include<csting>
#include<math>


class ColumnDef {
public:
    ColumnDef(const std::string& name, const std::string& type, int width)
        : name_(name), type_(type), width_(width) {}

    const std::string& getName() const {
        return name_;
    }

    const std::string& getType() const {
        return type_;
    }

    int getwidth() const {
        return width_;
    }

private:
    std::string name_;
    std::string type_;
    int width_;
};

class ColumnDefs {
public:
    std::vector<ColumnDef> columns_;

    void addColumn(const ColumnDef& column) {
        columns_.push_back(column);
    }

    const st::vector<ColumnDef> getColumn() const {
        return columns_;
    }

    int getColumnCount() const {
        return columnDefs_.size();
    }
};


class Table {
public:
    Table(const std::string& name, const std::vector<ColumnDef>& columns)
        : name_(name), columns_(columns) {}

    const std::string& getName() const {
        return name_;
    }

    const std::vector<ColumnDef>& getColumns() const {
        return columns_;
    }

    const std::vector<ColumnDef> getColumnDefs() const {
        ColumnDefs columndefs;
        columndefs.columns_ = columns_;
        columndefs.getColumn();
    }

private:
    std::string name_;
    std::vector<ColumnDef> columns_;
};

class Row {
public:
    Row(Table* table)
        : table_(table), values_(table->getColumnDefs().getColumnCount()) {}

    template <typename T>
    T getColumnValue(int columnIndex) const {
        if (columnIndex < 0 || columnIndex >= values_.size()) {
            // handle index out of range error
            return T();
        }
        return std::any_cast<T>(values_[columnIndex]);
    }

    template <typename T>
    void setColumnValue(int columnIndex, const T& value) {
        if (columnIndex < 0 || columnIndex >= values_.size()) {
            // handle index out of range error
            return;
        }
        values_[columnIndex] = value;
    }

    Table* getTable() const {
        return table_;
    }

    std::vector<uint8_t> encode(std::string columnName, std::string fieldValue) {
        ColumnDefs columnDefs = tables_.getColumnDefs();
        for (int i = 0; i < columnDefs.getColumnCount(); i++) {
            if (columnDefs.getColumnDefs(i).name_ == columnName) {
                if (columnDefs.getColumnDefs(i).type == "int") {
                    int value = std::stoi(fieldValue);
                    return encodeInt(value);
                }
                if (columnDefs.getColumnDefs(i).type == "double") {
                    double value = std::stod(fieldValue);
                    return encodeDouble(value);
                }
                if (columnDefs.getColumnDefs(i).type == "string") {
                    return encodeString(value);
                }
            }
        }
        return std::vector<uint8_t>();
    }

     template <typename T>
    T decode(const ColumnDef& colDef) const {
        int offset = 0;
        for (int i = 0; i < colDefs.size(); i++) {
            if (colDefs[i].name == colDef.name) {
                offset = i * 5 + 1;
                break;
            }
        }

        int value = 0;
        double doubleValue = 0;
        std::string strValue = "";
        if (colDef.type == "int") {
            value = (data[offset] << 24) | (data[offset+1] << 16) | (data[offset+2] << 8) | data[offset+3];
            return static_cast<T>(value);
        }
        else if (colDef.type == "double") {
            uint64_t encodedValue = ((uint64_t)data[offset] << 56) |
                                    ((uint64_t)data[offset+1] << 48) |
                                    ((uint64_t)data[offset+2] << 40) |
                                    ((uint64_t)data[offset+3] << 32) |
                                    ((uint64_t)data[offset+4] << 24) |
                                    ((uint64_t)data[offset+5] << 16) |
                                    ((uint64_t)data[offset+6] << 8) |
                                    (uint64_t)data[offset+7];
            memcpy(&doubleValue, &encodedValue, sizeof(double));
            return static_cast<T>(doubleValue);
        }

        else if (colDef.type == "string") {
            int len = (data[offset] << 8) | data[offset+1];
            strValue.resize(len);
            memcpy(&strValue[0], &data[offset+2], len);
            return static_cast<T>(strValue);
        }
        else {
            std::cout << "Invalid column type" << std::endl;
            return static_cast<T>(0);
        }
    }

private:
    Table* table_;
    std::vector<std::any> values_;
    char* data;
    int dataSize;

    // Encode an integer value to a byte array
std::vector<uint8_t> encodeInt(int value) {
    std::vector<uint8_t> buffer(sizeof(int));
    int offset = 0;
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&value);
    for (int i = 0; i < sizeof(int); i++) {
        buffer[offset + i] = *(ptr + i);
    }
    return buffer;
}

// Encode a double value to a byte array
std::vector<uint8_t> encodeDouble(double value) {
    std::vector<uint8_t> buffer(sizeof(double));
    int offset = 0;
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&value);
    for (int i = 0; i < sizeof(double); i++) {
        buffer[offset + i] = *(ptr + i);
    }
    return buffer;
}

// Encode a string value to a byte array
std::vector<uint8_t> encodeString(const std::string& value) {
    std::vector<uint8_t> buffer(value.size() + 1);
    int offset = 0;
    memcpy(buffer.data() + offset, value.c_str(), value.size() + 1);
    return buffer;
}
};

