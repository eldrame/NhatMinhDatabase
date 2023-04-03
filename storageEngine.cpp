#include <vector>
#include <iostream>
#include <stdio.h>
#include <cstdint>


class Segment {
public:
    void Write(std::vector<uint8_t> newData) {
        data = newData;
    }

    std::vector<uint8_t> Read() {
        return data;
    }

private:
    std::vector<uint8_t> data;
    int segmentSize;
};

class Extent {

};

class DataBlock {

};