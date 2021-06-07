# Astar-Algorithm
Implementation of the A* algorithm in Artificial Intelligence using C++ and GP142 graphics

A* Algorithm is a Heuristic Search Algorithm which forms the basis of some real time algorithms such as Real Time A* and Learning Real Time A*. In this algorithm, the best path is chosen from the starting node to the goal node, based on the heuristic f*(n) = g*(n) + h*(n). Here g*(n) is the shortest distance from the starting node to the current node and h*(n) is the minimum distance from the current node to the goal node. The node with the minimum value of f* is chosen every time. This algorithm is complete, optimal as well as admissible. 

In this project, a simple implementation of the A* algorithm is given. 

## How to run 
Open Visual Studio and create a new empty Win32 Project.  \
Add the header and source files provided in this repo.  \
Build and run the main.cpp file. 

Once the project is running, click on a square to mark it as the initial node. Then click on another square to mark it as the destination node. \
After choosing the start and end, you can choose multiple squares to act as hurdles by clicking on them. \
Once you have marked all the hurdles, press enter to find the optimal path from the start to end. 

The running application is as follows: 

![Astar](https://user-images.githubusercontent.com/68266703/121016290-61474200-c7b5-11eb-9698-1c5c80e3bc55.png)


The Black node is the starting node, the Green node is the destination, the red blocks are the hurdles and the purple nodes mark the optimal path from start to end. 
