# Delivery Pathfinding Project

Link to [Research paper](https://docs.google.com/document/d/1rywBJfyH-XEH8YfsDc_0LaLk_LyaK9Fyd81A-2P0dho/edit?usp=sharing)

## Introduction

This project was developed as part of the CSC 212: Data Structures and Algorithms course. The goal was to apply various data structures and algorithms in a real-world scenario by simulating a delivery service. The project integrates three key components: Dijkstra's Algorithm for finding the shortest path, Quad Trees for efficient spatial representation, and Bucket Sort for sorting potential delivery paths. The program simulates a 2D grid where random orders are placed, and the shortest path between delivery points is calculated.

## Planning and Concept

Our project began with the idea of using a 2D coordinate plane to represent a town and find the shortest delivery routes between houses. The key data structures and algorithms used include:
- **Quad Trees**: To efficiently represent the 2D space.
- **Dijkstra’s Algorithm**: For calculating the shortest path between two points.
- **Bucket Sort**: For sorting potential paths.

We initially considered a more complex food delivery service but decided to focus on a simplified delivery service to better showcase the key aspects of our code.

## Features

### Bucket Sort
- **Purpose**: Used for general sorting within the program, especially for sorting potential paths in descending or ascending order.
- **Functionality**: Divides the unsorted list into buckets, sorts each bucket individually, and then merges them to form the final sorted list.
- **Limitations**: Best suited for uniformly distributed data and primarily used for integer sorting in this program.

### Quad Trees
- **Purpose**: Efficiently stores and processes 2D spatial data, particularly useful for representing roads and houses in the grid.
- **Functionality**: Subdivides the space into quadrants to store and access data points efficiently.
- **Limitations**: Requires square grids and lacks search functionality, which was deemed unnecessary for our implementation.

### Dijkstra’s Algorithm
- **Purpose**: Finds the shortest path between two nodes in a weighted graph, represented by the 2D grid.
- **Functionality**: Utilizes a greedy approach to explore the shortest path from a starting point to all reachable nodes.
- **Limitations**: Limited to calculating the shortest path between two points without additional road weights like speed limits.

### City Generator Class
- **Purpose**: Generates a procedurally created city represented as a 2D grid, including roads, houses, and a delivery hub.
- **Functionality**: Uses various methods to generate different types of roads and neighborhoods, creating a unique city layout each time.
- **Limitations**: The program can handle maps up to a certain size before memory constraints become an issue.

## Integration & Main Class

The program integrates the Quad Tree, Dijkstra's Algorithm, and Bucket Sort to create a functional  delivery pathfinding tool. The main class handles generating the city, mapping the shortest paths between multiple delivery points, and outputting the results to a text file.

## Testing & Evaluation

- **Testing**: Individual components were tested with specific scenarios to ensure functionality. Integration testing ensured that the components worked together correctly.
- **Evaluation**: The program was run multiple times to fine-tune the city generation and pathfinding processes. Adjustments were made to correct minor oversights, such as overly long neighborhood roads.

## Pros & Cons

**Pros**:
- Efficient use of data structures and algorithms.
- Easy to understand and translate into visual representation.
- Successfully demonstrates core concepts in a practical scenario.

**Cons**:
- Limited to square grids and specific map sizes.
- Space-intensive, causing issues with larger maps or more complex configurations.

## Findings & Future

The project has potential beyond the  delivery microcosm. Future iterations could include more complex maps, user-inputted real-world maps, and more sophisticated visualizations. Additionally, further optimizations could improve efficiency and functionality.

## Conclusion

This project provided valuable hands-on experience in applying data structures and algorithms. It required teamwork, research, and practical application of classroom knowledge. The project successfully met its goals, demonstrating the utility of Dijkstra’s Algorithm, Quad Trees, and Bucket Sort in a real-world-like scenario.

## References

- “Find Shortest Paths from Source to All Vertices Using Dijkstra’s Algorithm.” GeeksforGeeks, 11 Jan. 2024.
- “Quad Tree.” GeeksforGeeks, 6 Feb. 2024.
- “Bucket Sort - Data Structures and Algorithms Tutorials.” GeeksforGeeks, 27 Mar. 2024.
- Warnimont, Joe. “How to Use CHATGPT to Build a Website.” Themeisle Blog, 5 Dec. 2023.
- “Coding Challenge #98.1: Quadtree - Part 1.” YouTube, 26 Mar. 2018.


## Program Usage Instructions

cityRepresentation.html
	Gives a visual representation for the city and the plotted path when inputted
	when generating a city with SIZE 2 use 256 for the map size
	when generating a city of SIZE 1 use 64 for the map size
	open the cityRepresentation.html file it should open a window in your browser
	in the box where it says "Grid size:" enter either 256 or 64 depending on your choice
	
	AFTER RUNNING THE PROGRAM (map and path are generated)
	to put map in go to the map.txt file generated by the program and open it
	while in the map.txt file press "ctrl + a" to highlight the enitre file contents, press "ctrl + c" to copy them
	after copying them return to the cityRepresentation.html, in the spot where it says "Enter grid here" delete the text
	after deleting the text past in the map with "ctrl + v" 
	With the map pasted in ensure you have the size set as described above for size 1 or 2
	You can now visualize the map to see it in its glory
	
	To plot the path for the delivery driver to take
	open the pathOutput.txt file generated by the program and open it
	while in the pathOutput.txt file press "ctrl + a" to highlight the entire file contents, press "ctrl + c" to copy them
	after copying them return to the cityRepresentation.html tab, in the box where it says "Enter coordinates (one pair per line)" above
	click on the box and press "ctrl + v" to past the path in. 
	You can now visualize the path in red on the map

main usage instructions
	Download and extract all associated files
	If attempting to run from within an IDE (assuming you are familiar with your IDE)
		Make a new project inside your IDE, drop in all files
		locate the main and run the program from there

	If attempting to run from the command prompt and have the required compilation dependencies (g++, minGW)
		Navigate to the location of the installation open the file containing the main 
		Use the following command to compile the program "g++ -o main main.cpp [SIZE]" without the brackets and replacing SIZE with 1 or 2
		With the program compiled enter this command to run the program "main.exe"
		
	The program accepts one single argument. 
	This is the size of the map either 1 or 2
	a size of 1 will generate a 64x64 map
	a size of 2 will generate a 256x256 map
