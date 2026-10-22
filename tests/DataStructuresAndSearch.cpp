#include "../Algorithms/BreadthFirstSearch.hpp"
#include "../Algorithms/DepthFirstSearch.hpp"
#include "../DataStructures/Queue.hpp"
#include "../DataStructures/Stack.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {
template <typename T>
std::vector<int> path(const Grid<T>& grid) {
    std::vector<int> result;
    if (grid.getPathLength() < 0) return result;
    const GridNode<T>* current = grid.getStart();
    while (current != nullptr) {
        result.push_back(grid.getIndex(current->getX(), current->getY()));
        current = current->next;
    }
    return result;
}

template <typename T>
std::vector<int> discoveryTimes(const Grid<T>& grid) {
    std::vector<int> result;
    for (int i = 0; i < grid.getNodeCount(); ++i) {
        result.push_back(grid.getNode(i).discoveredTime);
    }
    return result;
}

template <typename T>
void assertDiscoveryTimes(const Grid<T>& grid) {
    std::vector<bool> seen(static_cast<std::size_t>(grid.getNodeCount()) + 1, false);
    int count = 0;
    for (int i = 0; i < grid.getNodeCount(); ++i) {
        const auto& node = grid.getNode(i);
        const int time = node.discoveredTime;
        assert(node.discovered == (time > 0));
        if (time == 0) continue;
        assert(time <= grid.getNodeCount());
        assert(!seen[time]);
        seen[time] = true;
        ++count;
    }
    for (int time = 1; time <= count; ++time) assert(seen[time]);
}

void testContainers() {
    Queue<std::string> queue;
    assert(queue.isEmpty());
    try { queue.getFront(); assert(false); } catch (const std::out_of_range&) {}
    try { queue.pop(); assert(false); } catch (const std::out_of_range&) {}
    queue.push("first");
    queue.push("second");
    assert(queue.getSize() == 2 && queue.getFront() == "first");
    Queue<std::string> movedQueue(std::move(queue));
    assert(queue.isEmpty() && movedQueue.getSize() == 2);
    assert(movedQueue.pop() == "first");
    assert(movedQueue.pop() == "second");
    assert(movedQueue.isEmpty());
    movedQueue.push("again");
    assert(movedQueue.getFront() == "again");

    Stack<int> stack;
    assert(stack.isEmpty());
    try { stack.getTop(); assert(false); } catch (const std::out_of_range&) {}
    try { stack.pop(); assert(false); } catch (const std::out_of_range&) {}
    stack.push(1);
    stack.push(2);
    assert(stack.getSize() == 2 && stack.getTop() == 2);
    Stack<int> movedStack;
    movedStack = std::move(stack);
    assert(stack.isEmpty() && movedStack.getTop() == 2);
    assert(movedStack.pop() == 2);
    assert(movedStack.getTop() == 1);
    movedStack.clear();
    assert(movedStack.isEmpty());
}

template <template <typename> class Search, typename T>
void testSearch() {
    Grid<T> grid({"S..", "...", "G.."});
    assert(grid.getStart() == &grid.getNode(grid.getStartIndex()));
    assert(grid.getGoal() == &grid.getNode(grid.getGoalIndex()));
    Search<T> search;
    assert(search.run(grid) == &grid.getNode(grid.getStartIndex()));
    const auto iterativePath = path(grid);
    const auto iterativeTimes = discoveryTimes(grid);
    assert(grid.getStart()->discoveredTime == 1);
    assertDiscoveryTimes(grid);
    GridNode<T>* recursiveResult = search.runRecursive(grid);
    assertDiscoveryTimes(grid);
    if constexpr (std::is_same_v<Search<T>, BreadthFirstSearch<T>>) {
        assert(recursiveResult == grid.getGoal());
        assert(grid.getStart()->next == nullptr);
        assert(search.run(grid) == grid.getStart());
        assert(path(grid) == iterativePath);
        assert(discoveryTimes(grid) == iterativeTimes);
    } else {
        assert(recursiveResult == grid.getStart());
        assert(path(grid).front() == grid.getStartIndex());
        assert(path(grid).back() == grid.getGoalIndex());
    }
    assert(grid.getNode(grid.getGoalIndex()).discovered);
    grid.clearSearch();
    for (int i = 0; i < grid.getNodeCount(); ++i) {
        assert(!grid.getNode(i).discovered);
        assert(grid.getNode(i).discoveredTime == 0);
    }

    Grid<T> blocked({"S#G"});
    assert(search.run(blocked) == nullptr);
    assert(blocked.getPathLength() == -1);
    assert(search.runRecursive(blocked) == nullptr);
    assert(blocked.getPathLength() == -1);
    assert(blocked.getStart()->discoveredTime == 1);
    assert(!blocked.getNode(blocked.getGoalIndex()).discovered);
    assert(blocked.getGoal()->discoveredTime == 0);
}

void testBfsPredecessors() {
    Grid<int> grid({"S..", "...", "G.."});
    BreadthFirstSearch<int> bfs;
    assert(bfs.runIterative(grid) == grid.getGoal());
    assert(grid.getStart()->next == nullptr);
    assert(bfs.run(grid) == grid.getStart());
    assert(path(grid) == std::vector<int>({0, 3, 6}));
    assert(grid.getNode(0).discoveredTime == 1);
    assert(grid.getNode(1).discoveredTime == 2);
    assert(grid.getNode(3).discoveredTime == 3);
    assert(grid.getNode(6).prev == &grid.getNode(3));
    assert(grid.getNode(3).prev == &grid.getNode(0));
    assert(grid.getNode(0).next == &grid.getNode(3));
    assert(grid.getNode(3).next == &grid.getNode(6));
    assert(grid.getNode(6).next == nullptr);

    grid.clearSearch();
    assert(grid.getNode(6).prev == nullptr);
    assert(grid.getNode(0).next == nullptr);
    assert(!grid.getNode(0).discovered && !grid.getNode(6).discovered);
    assert(grid.getNode(0).discoveredTime == 0 && grid.getNode(6).discoveredTime == 0);
}

void testDfsIterative() {
    Grid<int> grid({"S..", "...", "G.."});
    DepthFirstSearch<int> dfs;
    assert(dfs.runIterative(grid) == grid.getGoal());
    assert(grid.getNode(3).discoveredTime == 2);
    assert(grid.getNode(1).discoveredTime == 3);
    assert(grid.getStart()->next == nullptr);
    assert(grid.getPathLength() == -1);
    const GridNode<int>* current = grid.getGoal();
    for (int steps = 0; current != grid.getStart() && steps < grid.getNodeCount(); ++steps) {
        current = current->prev;
        assert(current != nullptr);
    }
    assert(current == grid.getStart());
    assert(dfs.run(grid) == grid.getStart());
    assert(grid.getPathLength() >= 0);
}

void testNodeValues() {
    Grid<int> numbers({"SG"}, 5);
    assert(numbers.getNode(numbers.getStartIndex()).value == 5);
    numbers.getNode(numbers.getGoalIndex()).value = 42;
    BreadthFirstSearch<int> bfs;
    bfs.run(numbers);
    assert(numbers.getPathLength() == 1);
    assert(numbers.getNode(numbers.getGoalIndex()).value == 42);

    Grid<std::string> labels({"SG"}, "empty");
    assert(labels.getNode(labels.getGoalIndex()).value == "empty");
    labels.getNode(labels.getStartIndex()).value = "start";
    assert(labels.getNode(labels.getGoalIndex()).value == "empty");
    DepthFirstSearch<std::string> dfs;
    dfs.run(labels);
    assert(labels.getPathLength() == 1);
    assert(labels.getNode(labels.getStartIndex()).value == "start");
}
}

int main() {
    testContainers();
    testNodeValues();
    testBfsPredecessors();
    testDfsIterative();
    testSearch<BreadthFirstSearch, int>();
    testSearch<DepthFirstSearch, int>();
    testSearch<BreadthFirstSearch, std::string>();
    testSearch<DepthFirstSearch, std::string>();
}
