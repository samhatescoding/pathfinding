#pragma once

#include <utility>

// A link owned by a Queue or Stack. Graph/grid nodes are a separate concept.
template <typename T>
struct LinkedNode {
    template <typename U>
    explicit LinkedNode(U&& initial, LinkedNode* nextNode = nullptr)
        : value(std::forward<U>(initial)), next(nextNode) {}

    T value;
    LinkedNode* next;
};
