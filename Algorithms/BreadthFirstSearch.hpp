#pragma once

#include "Algorithm.hpp"
#include "../DataStructures/Queue.hpp"


template <typename T>
class BreadthFirstSearch final : public Algorithm<T> {
public:
    const char* getName() const override { return "Breadth-first search"; }

    GridNode<T>* run(Grid<T>& grid) const override {
        GridNode<T>* goal = runIterative(grid);
        return goal == nullptr ? nullptr : linkPath(grid, goal);
    }

    GridNode<T>* runIterative(Grid<T>& grid) const {
        grid.clearSearch();
        int discoveryCount = 0;
        Queue<GridNode<T>*> frontier;
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
        Queue<GridNode<T>*> frontier;
        GridNode<T>* start = grid.getStart();
        start->discover(++discoveryCount);
        frontier.push(start);

        return exploreRecursive(grid, frontier, discoveryCount);
    }

private:
    static void enqueueNeighbors(GridNode<T>* current,
                                 Queue<GridNode<T>*>& frontier, int& discoveryCount) {
        for (GridNode<T>* neighbor : current->getNeighbors()) {
            if (neighbor == nullptr) continue;
            if (neighbor->discovered) continue;
            // The first discovery gives a shortest route to this neighbor.
            neighbor->discover(++discoveryCount);
            neighbor->prev = current;
            frontier.push(neighbor);
        }
    }

    static GridNode<T>* exploreRecursive(Grid<T>& grid, Queue<GridNode<T>*>& frontier,
                                         int& discoveryCount) {
        if (frontier.isEmpty()) return nullptr;

        GridNode<T>* current = frontier.pop();
        if (current == grid.getGoal()) return current;

        enqueueNeighbors(current, frontier, discoveryCount);
        return exploreRecursive(grid, frontier, discoveryCount);
    }

    static GridNode<T>* linkPath(Grid<T>& grid, GridNode<T>* goal) {
        // prev points backward through the search tree; next marks only the path.
        for (GridNode<T>* current = goal; current->prev != nullptr; current = current->prev) {
            current->prev->next = current;
        }
        return grid.getStart();
    }

};
