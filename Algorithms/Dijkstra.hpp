#pragma once

#include "Algorithm.hpp"
#include "../DataStructures/PriorityQueue.hpp"

template <typename T>
class Dijkstra final : public Algorithm<T> {
public:
    const char* getName() const override { return "Dijkstra's algorithm"; }

    Node<T>* run(Grid<T>& grid) const override {
        grid.clearSearch();

        // Each waiting node has one entry containing its best distance so far.
        PriorityQueue<T> frontier;
        int discoveryCount = 0;

        grid.getStart()->discover(++discoveryCount);
        frontier.push(grid.getStart(), 0);

        while (!frontier.isEmpty()) {
            // The list is unordered; pop scans it for the smallest distance.
            const auto [current, cost] = frontier.pop();
            current->settled = true;

            if (current == grid.getGoal()) return linkPath(grid, current);

            for (Node<T>* neighbor : current->getNeighbors()) {
                if (neighbor == nullptr) continue;
                if (neighbor->settled) continue;
                const int nextCost = cost + 1; // Every grid move currently costs one.
                // Search for the neighbor's entry and lower its priority if needed.
                if (!frontier.push(neighbor, nextCost)) continue;

                neighbor->prev = current;
                if (!neighbor->discovered) neighbor->discover(++discoveryCount);
            }
        }
        return nullptr;
    }

private:
    static Node<T>* linkPath(Grid<T>& grid, Node<T>* goal) {
        for (Node<T>* current = goal; current->prev != nullptr; current = current->prev) {
            current->prev->next = current;
        }
        return grid.getStart();
    }
};
