#include <vector>
#include <iostream>
#include <stdio.h>
#include <cstdint>

class DataBlock {

};

class Extent {

};

class Segment {
public:

    void Write(char* newData) {
        byteBuffer_ = newData;
    }

    std::vector<uint8_t> Read() {
        return dataFromExtent_;
    }

    int computeOffset(int index, char* buffer_) {
        int offset = index * sizeof(buffer_);
        return offset;
    }

    char* getByteBuffer() {
        return byteBuffer_;
    }

private:
    char* byteBuffer_;
    std::vector<uint8_t> dataFromExtent_;
    int segmentSize;
};

class StorageEngine {};