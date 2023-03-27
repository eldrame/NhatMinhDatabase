#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cstring>
#include <stdio.h>



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

    int getWidth() const {
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

    const std::vector<ColumnDef> getColumn() const {
        return columns_;
    }

    int getColumnCount() const {
        return columns_.size();
    }

    const ColumnDef& getColumnDef(int i) {
        if (i < 0 || i >= columns_.size()) {
            throw std::out_of_range("Column index out of range");
        }
        return columns_[i];
    }
};


class Table {
public:
    Table(ColumnDefs& columnDefs) : columnDefs_(columnDefs) {}

    const ColumnDefs& getColumnDefs() const {
        return columnDefs_;
    }

    const std::string& getName() const {
        return name_;
    }

    const std::vector<ColumnDef>& getColumns() const {
        return columns_;
    }

    

    size_t getRowSize() const {
        size_t rowSize_ = 0;
            for (size_t i = 0; i < columns_.size(); i++) {
                const ColumnDef& columnDef = columns_[i];
                rowSize_ += columnDef.getWidth();
            }
        return rowSize_;
    }

private:
    std::string name_;
    ColumnDefs& columnDefs_;
    std::vector<ColumnDef> columns_ = columnDefs_.columns_;
    std::vector<size_t> columnOffset_;
};

class Row {
public:
    Row(Table* table)
        : table_(table), buffer_(table->getRowSize()) {}

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

    //encode the Row into the buffer
    std::vector<uint8_t> encodeRow() {
        ColumnDefs colDefs = getTable()-> getColumnDefs();
        int offset = 0;
        for (int i = 0; i < colDefs.getColumnCount(); i++) {
            if (colDefs.getColumnDef(i).getType() == "int") {
                int value = std::stoi(values_[i]);
                std::vector<uint8_t> encodedValue = encodeInt(int value);
                for (int j = 0; j < sizeof(int); j++) {
                    buffer_[offset + j] = encodedValue[j];
                }
                offset += colDefs.getColumnDef(i).getWidth();
            }
            if (colDefs.getColumnDef(i).getType() == "double") {
                double value = std::stod(values_[i]);
                std::vector<uint8_t> encodedValue = encodeDouble(double value);
                for (int j = 0; j < sizeof(double); j++) {
                    buffer_[offset + j] = encodedValue[j];
                }
                offset += colDefs.getColumnDef(i).getWidth();
            }
            if (colDefs.getColumnDef(i).getType() == "string") {
                std::vector<uint8_t> encodedValue = encodeString(std::string value);
                for (int j = 0; j < encodedValue.size() + 1; j++) {
                    buffer_[offset + j] = encodedValue[j];
                }
                offset += colDefs.getColumnDef(i).getWidth();
            }
            //case "string" definitely will wrong.
            return buffer_;

            return std::vector<uint8_t>();
        }
    }

    //encode from field value in Row to a byte array
    std::vector<uint8_t> encode(std::string columnName, std::string fieldValue) {
        ColumnDefs columnDefs = table_->getColumnDefs();
        int offset = 0;
        for (int i = 0; i < columnDefs.getColumnCount(); i++) {
            if (columnDefs.getColumnDef(i).getName() == columnName) {
                if (columnDefs.getColumnDef(i).getType() == "int") {
                    int value = std::stoi(fieldValue);
                    std::vector<uint8_t> encodedValue = encodeInt(value);
                    for (int i = 0; i < sizeof(int); i++) {
                        buffer_[offset + i] = encodedValue[i];
                    }
                    return buffer_;
                }
                if (columnDefs.getColumnDef(i).getType() == "double") {
                    double value = std::stod(fieldValue);
                    std::vector<uint8_t> encodedValue = encodeDouble(value);
                    for (int i = 0; i < sizeof(double); i++) {
                        buffer_[offset + i] = encodedValue[i];
                    }
                    return buffer_;
                }
                if (columnDefs.getColumnDef(i).getType() == "string") {
                    std::vector<uint8_t> encodedValue = encodeString(value);
                    for (int j = 0; j < value.size() + 1; j++) {
                        buffer_[offset + j] = encodedValue[j];
                    }
                    return buffer_;
                }
                
            }
            else {
                offset += columnDefs.getColumnDef(i).getWidth();
            }
        }
    }

    //decode from byte array to field value in Row
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

    // set a value to a field inside a row
    template <typename T>
    void setFieldValue(const std::string& fieldName, std::string fieldValue) {
        encode(fieldName, fieldValue);
    }

    // get aa value of a field from a row
    template <typename T>
    T getFieldValue(const std::string& fieldName) {
        for (size_t i = 0; i < columnDefs_.size(); i++) {
            const ColumnDef& columnDef = columnDefs_.at(i);
            if (columnDef.getName() == fieldName) {
                const uint8_t* bufferPtr = buffer_[columnDef.getOffset()];
                return decode<T>(bufferPtr, columnDef.getType());
            }
        }
        throw std::runtime_error("Field" + fieldName + "not found in row");
    }

private:
    Table* table_;
    std::vector<std::string> values_;
    char* data;
    int dataSize;
    std::vector<uint8_t> buffer_;

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

