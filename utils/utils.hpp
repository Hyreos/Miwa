#pragma once

#include<vector>
#include<algorithm>
#include<type_traits>

namespace utils {
    template <typename T>
    inline void quick_remove_at(std::vector<T> &v, std::size_t idx)
    {
        if (idx < v.size()) {
            v.at(idx) = std::move(v.back());
            v.pop_back();
        }
    }

    template <typename T>
    inline void quick_remove_at(std::vector<T> &v, typename std::vector<T>::iterator&& it)
    {
        if (it != std::end(v)) {
            *it = std::move(v.back());
            v.pop_back();
        }
    }
}
