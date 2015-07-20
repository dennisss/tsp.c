The fastest TSP Solver I could make in a week
=============================================

- Written by Dennis Shtatnov <densht@gmail.com>
- Originally for CS 680: Autonomous Agents, Drexel University


- The solver is divided into two phases:

1. First, the problem is converted to an undirected graph
2. Second, the graph is solved using generic graph search algorithms



- Data available at <http://www.math.uwaterloo.ca/tsp/world/countries.html>



Getting Started
---------------




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
