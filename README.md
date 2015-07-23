The fastest TSP Solver I could make in a week
=============================================

- Written by Dennis Shtatnov <densht@gmail.com>
- Originally for CS 680: Autonomous Agents, Drexel University


- The solver is divided into two phases:

1. First, the problem is converted to an undirected graph
2. Second, the graph is solved using generic graph search algorithms

- Data available at <http://www.math.uwaterloo.ca/tsp/world/countries.html>



Building
--------

- Running `make` to build the standard program
- Running `make clean` will clean all build objects. Note: this is required if you change of the below environment variables

Some environment variables can be passed to `make` to customize the build. This can be combined in any way

- `DEBUG=1 make` will enable additional logging
- `SPARSE=1 make` will use the sparse graph implementation instead of the dense one
- `VIEWER=1 make` will compile the OpenGL based viewer
	- Compiling the viewer requires GLFW, GLEW and OpenGL to be installed on the system


Running
-------

Run as `./tsp data.tsp 10` to find a tour for data.tsp using at most 10 seconds.

The resultant tour will be saved to `output.tour`

- Setting the time to 0 will make it run until interrupted (Ctrl-C). Also interruption it will still save the tour to a file.
- Adding `-view` to the command will start the viewer. This requires that the program was compiled with the viewer enabled.




Edge Generation
---------------

- TODO: I need to have both sparse and dense matrix representations

- Bruteforce: populate all possible edges between cities
- Delaunay Triangulation






Path Generation
---------------

- DFS: try all possible paths keeping the shortest one
- TODO: traveling the same path in the opposite direction is the same path


- Ant Colony Optimization (ACO):

- TODO: Have some method of stopping ants when they travel farther than the current estimate of the optimal path
