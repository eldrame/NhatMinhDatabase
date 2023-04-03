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

    void Write(std::vector<uint8_t> newData) {
        byteBuffer_ = newData;
    }

    std::vector<uint8_t> Read() {
        return dataFromExtent_;
    }

    int computeOffset(int index, std::vector<uint8_t> buffer_) {
        int offset = index * buffer_.size();
    }

    std::vector<uint8_t> getByteBuffer() {
        return byteBuffer_;
    }

private:
    std::vector<uint8_t> byteBuffer_;
    std::vector<uint8_t> dataFromExtent_;
    int segmentSize;
};