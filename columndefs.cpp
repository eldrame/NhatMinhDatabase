#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include "storageEngine.cpp"



class ColumnDef {
public:
    ColumnDef(const std::string& name, const std::string& type, const int width)
        {
            name_ = name;
            type_ = type;
            width_ = width;
        }

    std::string& getName() {
        return name_;
    }

    std::string& getType() {
        return type_;
    }

    int getWidth() {
        if (getType() == "int") {
            return sizeof(int);
        }
        if (getType() == "double") {
            return sizeof(double);
        }
        if (getType() == "string") {
            return 30;
        }
    }

private:
    std::string name_;
    std::string type_;
    int width_;
};

class ColumnDefs {
public:
    std::vector<ColumnDef> columns_;

    void addColumn(ColumnDef column) {
        columns_.push_back(column);
    }

    std::vector<ColumnDef> getColumn()  {
        return columns_;
    }

    int getColumnCount()  {
        return columns_.size();
    }

    ColumnDef& getColumnDef(int i) {
        if (i < 0 || i >= columns_.size()) {
            throw std::out_of_range("Column index out of range");
        }
        return columns_[i];
    }

    size_t getRowSize() {
        size_t rowSize_ = 0;
            for (size_t i = 0; i < getColumnCount(); i++) {
                ColumnDef columnDef = getColumnDef(i);
                rowSize_ += columnDef.getWidth();
            }
        return rowSize_;
    }
};


class Row {
public:
    Row(ColumnDefs columnDefs)
        : values_(columnDefs.getColumnCount()) {
        columnDefs_ = columnDefs;
    }

    // template <typename T>
    // // T getColumnValue(int columnIndex) const {
    // //     if (columnIndex < 0 || columnIndex >= values_.size()) {
    // //         // handle index out of range error
    // //         return T();
    // //     }
    // //     return std::any_cast<T>(values_[columnIndex]);
    // // }

    // template <typename T>
    // void setColumnValue(int columnIndex, const T& value) {
    //     if (columnIndex < 0 || columnIndex >= values_.size()) {
    //         // handle index out of range error
    //         return;
    //     }
    //     values_[columnIndex] = value;
    // }

    ColumnDefs getColumnDefs() const {
        return columnDefs_;
    }

    void SetValue(std::vector<std::string> val_Vec) {
        if (val_Vec.size() == values_.size()) {
            for (int i = 0; i < values_.size(); i++) {
                values_[i] = val_Vec[i];
            }
        }
        else {
            std::cerr << "Error: Input vector has different size than value_ vector" << std::endl;
            return;
        }
    }

    //encode the Row into the buffer
    char* encodeRow() {
        ColumnDefs colDefs = getColumnDefs();
        int offset = 0;
        for (int i = 0; i < getColumnDefs().getRowSize(); i++) {
            buffer_[offset + i] = 0;
        }
        for (int i = 0; i < colDefs.getColumnCount(); i++) {
            if (colDefs.getColumnDef(i).getType() == "int") {
                int value = std::stoi(values_[i]);
                std::vector<uint8_t> encodedValue = encodeInt((int) value);
                for (int j = 0; j < sizeof(int); j++) {
                    buffer_[offset + j] = encodedValue[j];
                }
                offset += colDefs.getColumnDef(i).getWidth();
            }
            if (colDefs.getColumnDef(i).getType() == "double") {
                double value = std::stod(values_[i]);
                std::vector<uint8_t> encodedValue = encodeDouble((double) value);
                for (int j = 0; j < sizeof(double); j++) {
                    buffer_[offset + j] = encodedValue[j];
                }
                offset += colDefs.getColumnDef(i).getWidth();
            }
            if (colDefs.getColumnDef(i).getType() == "string") {
                std::vector<uint8_t> encodedValue = encodeString((std::string) values_[i]);
                for (int j = 0; j < encodedValue.size() + 1; j++) {
                    buffer_[offset + j] = encodedValue[j];
                }
                offset += colDefs.getColumnDef(i).getWidth();
            }   
        }
        return buffer_;
    }

    //encode from field value in Row to a byte array
    char* encode(std::string columnName, std::string fieldValue) {
        ColumnDefs colDefs = getColumnDefs();
        int offset = 0;
        for (int i = 0; i < colDefs.getColumnCount(); i++) {
            if (colDefs.getColumnDef(i).getName() == columnName) {
                if (colDefs.getColumnDef(i).getType() == "int") {
                    int value = std::stoi(fieldValue);
                    std::vector<uint8_t> encodedValue = encodeInt(value);
                    for (int i = 0; i < sizeof(int); i++) {
                        buffer_[offset + i] = encodedValue[i];
                    }
                    return buffer_;
                }
                if (colDefs.getColumnDef(i).getType() == "double") {
                    double value = std::stod(fieldValue);
                    std::vector<uint8_t> encodedValue = encodeDouble(value);
                    for (int i = 0; i < sizeof(double); i++) {
                        buffer_[offset + i] = encodedValue[i];
                    }
                    return buffer_;
                }
                if (colDefs.getColumnDef(i).getType() == "string") {
                    std::vector<uint8_t> encodedValue = encodeString(fieldValue);
                    for (int t = 0; t < 30; t++) {
                        buffer_[offset + t] = 0;
                    }
                    for (int j = 0; j < fieldValue.size() + 1; j++) {
                        buffer_[offset + j] = encodedValue[j];
                    }
                    return buffer_;
                }
                
            }
            else {
                offset += colDefs.getColumnDef(i).getWidth();
            }
        }
    }

    //decode from byte array to field value in Row
    template <typename T>
    T decode(ColumnDef& colDef) {
        int offset = 0;
        for (int i = 0; i < columnDefs_.getColumnCount(); i++) {
            if (columnDefs_.getColumnDef(i).getName() == colDef.getName()) {
                offset = i * 5 + 1;
                break;
            }
        }

        int value = 0;
        double doubleValue = 0;
        std::string strValue = "";
        if (colDef.getType() == "int") {
            value = (data[offset] << 24) | (data[offset+1] << 16) | (data[offset+2] << 8) | data[offset+3];
            return static_cast<T>(value);
        }
        else if (colDef.getType() == "double") {
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

        else if (colDef.getType() == "string") {
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
    void setFieldValue(std::string& fieldName, std::string fieldValue) {
        encode(fieldName, fieldValue);
    }

    // get a value of a field from a row
    template <typename T>
    T getFieldValue(std::string& fieldName) {
        for (size_t i = 0; i < columnDefs_.getColumnCount(); i++) {
            ColumnDef& columnDef = columnDefs_.getColumnDef(i);
            if (columnDef.getName() == fieldName) {
                int offset = static_cast<int>(columnDef.getWidth());
                char* bufferPtr = &buffer_[offset];
                return decode<T>(bufferPtr, columnDef.getType());
            }
        }
        throw std::runtime_error("Field" + fieldName + "not found in row");
    }

    //pass the buffer to Segment
    


private:
    ColumnDefs columnDefs_;
    std::vector<std::string> values_;
    char* data;
    int dataSize;
    char* buffer_ = new char[columnDefs_.getRowSize()];

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




class Table {
public:
    Table(ColumnDefs columnDefs) {
        columnDefs_ = columnDefs;
    }

    ColumnDefs& getColumnDefs() {
        return columnDefs_;
    }

    std::string& getName() {
        return name_;
    }

    std::vector<ColumnDef> getColumns() {
        return columnDefs_.getColumn();
    }

    Row createRow() {}

    void setRow() {}

    Row getRow(int index) {
        if (index < 0 || index >= rowList_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return rowList_[i];
    }

    Segment* createSegment() {
        if (segmentPtr != nullptr) {
            return nullptr;
        }
        else {
            segmentPtr = new Segment();
            return segmentPtr;
        }
    }

private:
    std::string name_;
    ColumnDefs columnDefs_;
    std::vector<ColumnDef> columns_ = columnDefs_.columns_;
    std::vector<size_t> columnOffset_;
    std::vector<Row> rowList_;
    Segment* segmentPtr;
};

