#pragma once

#include <array>
#include <utility>

template <typename T>
class GridNode {
public:
    GridNode(int x, int y, bool blocked, T initialValue = T{})
        : value(std::move(initialValue)), x_(x), y_(y), blocked_(blocked) {}

    int getX() const { return x_; }
    int getY() const { return y_; }
    bool getBlocked() const { return blocked_; }

    // User data associated with this cell; search does not change it.
    T value;

    // A null pointer means an edge of the grid or a wall.
    GridNode<T>* up = nullptr;
    GridNode<T>* right = nullptr;
    GridNode<T>* down = nullptr;
    GridNode<T>* left = nullptr;

    // Set when first discovered: the node that led to this one.
    GridNode<T>* prev = nullptr;

    // Search state, reset by Grid::clearSearch before each run.
    bool discovered = false;
    // Zero until discovered; otherwise the 1-based discovery order.
    int discoveredTime = 0;

    void discover(int time) {
        discovered = true;
        discoveredTime = time;
    }

    // Filled in after a search: follow next from S to G.
    GridNode<T>* next = nullptr;

    std::array<GridNode<T>*, 4> getNeighbors() const {
        return {right, down, left, up};
    }

private:
    int x_;
    int y_;
    bool blocked_;
};
