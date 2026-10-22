# Grid pathfinding

A small C++17 grid search example. `Grid<T>` owns `GridNode<T>` objects. Each
node holds a `T value` plus `up`, `right`, `down`, and `left` pointers; edges
and walls leave the corresponding pointers null. `Grid<T>` stores pointers to
its start and goal nodes and exposes them through `getStart()` and `getGoal()`.
Breadth-first search records
the first predecessor in `prev`, then links the found path through `next`.
Each node also has a `discovered` flag to avoid revisiting it and a 1-based
`discoveredTime` that records when it was first discovered. Zero means it was
not discovered. Each search run owns its discovery counter and asks the node to
record its time. The grid resets both fields, `prev`, and `next` before each
search. The animation plays back the discovery times.
Iterative DFS marks a node when it is pushed onto the stack, so each node is
stacked once. Its discovery times may differ from recursive DFS, which marks a
node when the recursion enters it.
`Algorithm<T>`, `BreadthFirstSearch<T>`, and `DepthFirstSearch<T>` work with
either `int` or `std::string` values. `LinkedNode<T>` is a separate singly
linked node used by `Queue<T>` and `Stack<T>` for the iterative frontiers;
BFS also uses `Queue<T>` recursively.

The C++ backend writes `web/run.json` through `Output/JsonWriter.hpp`. The
static frontend (`web/index.html`, `web/style.css`, and `web/app.js`) draws the
grid and animates the search. The JSON writer follows `GridNode::next` to export
the path; the algorithms do not store a second path. Node values stay in C++
and are not shown in the animation.

For example, both grids can run the same search algorithm:

```cpp
Grid<int> numbers({"S.G"}, 0);
numbers.getNode(numbers.getStartIndex()).value = 7;
BreadthFirstSearch<int> numberSearch;
numberSearch.run(numbers);

Grid<std::string> labels({"S.G"}, "empty");
labels.getNode(labels.getGoalIndex()).value = "destination";
BreadthFirstSearch<std::string> labelSearch;
labelSearch.run(labels);
```

In `main.cpp`, change `using CellValue = int;` to `using CellValue = std::string;`
to switch the application's node value type.

## Use in VS Code on Windows

1. Have a C++17 compiler (`g++`, `clang++`, or MSVC `cl`) available on `PATH`.
2. Install the recommended **Live Preview** extension by Microsoft. Install
   **Run on Save** as well if you want every C++ save to rebuild automatically.
3. Press **Ctrl+Shift+B** once. This compiles and runs the app, producing
   `web/run.json`.
4. Open `web/index.html` with **Live Preview: Show Preview** and move the
   preview editor beside `main.cpp`. Keep it open while editing.
5. Change the map near the top of `main.cpp`, or switch between
   `BreadthFirstSearch` and `DepthFirstSearch` there. Press **Ctrl+Shift+B**, or
   just save if you installed Run on Save. The open page detects the new JSON
   and restarts the animation automatically.

`S` is the start, `G` is the goal, `#` is a wall, and `.` is open space. Rows
must be equally wide. Movement is in four directions. Breadth-first search
finds a shortest path when each move has the same cost; depth-first search finds
a path but does not guarantee the shortest one. The HTML has pause, restart,
and scrub controls. The iterative implementations avoid recursion depth limits;
the recursive implementations can exhaust the call stack on very large grids.

Both search classes also expose `runIterative(grid)` and `runRecursive(grid)`
for direct calls. `run(grid)` uses the iterative search through the common
`Algorithm` interface and returns the first node of the linked path, or
`nullptr` when no path exists. Both iterative methods and BFS's recursive method
return the goal node without linking the path. DFS's recursive method returns
the linked path's first node. The grid owns these nodes and clears their search
links before another run.

From PowerShell, the equivalent command is `./build-and-run.ps1`.
