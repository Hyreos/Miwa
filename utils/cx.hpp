#pragma once
#include<array>
#include<initializer_list>

namespace cx {
    template<typename Ty, unsigned int Size=5>
    class Array {
    public:
        Array() : length_(0) {}

        constexpr void push(Ty object) {
            data_[length_++] = object;
        }

        constexpr unsigned int size() {
            return length_;
        }

        constexpr unsigned int capacity() {
            return Size;
        }

        constexpr Ty& operator[](unsigned int index) {
            return data_[index];
        }
    private:
        std::array<Ty, Size> data_;
        int length_;

    };
}