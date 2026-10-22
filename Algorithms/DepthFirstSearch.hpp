#pragma once

#include "Algorithm.hpp"
#include "../DataStructures/Stack.hpp"

#include <vector>

template <typename T>
class DepthFirstSearch final : public Algorithm<T> {
public:
    const char* getName() const override { return "Depth-first search"; }

    GridNode<T>* run(Grid<T>& grid) const override {
        GridNode<T>* goal = runIterative(grid);
        return goal == nullptr ? nullptr : linkPath(grid, goal);
    }

    GridNode<T>* runIterative(Grid<T>& grid) const {
        grid.clearSearch();
        int discoveryCount = 0;
        Stack<GridNode<T>*> frontier;
        GridNode<T>* start = grid.getStart();
        start->discover(++discoveryCount);
        frontier.push(start);

        while (!frontier.isEmpty()) {
            GridNode<T>* current = frontier.pop();
            if (current == grid.getGoal()) return current;
            enqueueNeighbors(current, frontier, discoveryCount);
        }
        return nullptr;
    }

    GridNode<T>* runRecursive(Grid<T>& grid) const {
        grid.clearSearch();
        int discoveryCount = 0;
        std::vector<int> parent(grid.getNodeCount(), -1);

        const bool found = exploreRecursive(grid, grid.getStartIndex(), parent, discoveryCount);
        return Algorithm<T>::finish(grid, parent, found);
    }

private:
    static void enqueueNeighbors(GridNode<T>* current,
                                 Stack<GridNode<T>*>& frontier, int& discoveryCount) {
        // Push in reverse so the stack explores right, down, left, up.
        const auto neighbors = current->getNeighbors();
        for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
            GridNode<T>* neighbor = *it;
            if (neighbor != nullptr && !neighbor->discovered) {
                neighbor->discover(++discoveryCount);
                neighbor->prev = current;
                frontier.push(neighbor);
            }
        }
    }

    static GridNode<T>* linkPath(Grid<T>& grid, GridNode<T>* goal) {
        for (GridNode<T>* current = goal; current->prev != nullptr; current = current->prev) {
            current->prev->next = current;
        }
        return grid.getStart();
    }

    static bool exploreRecursive(Grid<T>& grid, int current,
                                 std::vector<int>& parent, int& discoveryCount) {
        // Enter this cell and record its discovery order for the animation.
        grid.getNode(current).discover(++discoveryCount);
        if (&grid.getNode(current) == grid.getGoal()) return true;

        for (GridNode<T>* neighbor : grid.getNode(current).getNeighbors()) {
            if (neighbor == nullptr) continue;
            const int next = grid.getIndex(neighbor->getX(), neighbor->getY());
            if (neighbor->discovered) continue;
            // Visit this neighbor fully before trying the next direction.
            parent[next] = current;
            if (exploreRecursive(grid, next, parent, discoveryCount)) return true;
        }

        // No route from here; return to the caller to try another neighbor.
        return false;
    }

};
