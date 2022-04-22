HW10: TSP Project, part 2 (Genetic Algorithm search)
Jackson Darling and Nico Anderson

This project uses genetic algorithm search to attempt to solve the Traveling Salesman Problem. The Chromosome class uses mutation and genetic recombination (in this case cities within the ordering work like alleles within a chromosome) to create new orderings of cities in an attempt to find the shortest total distance between the cities, stopping in each city only once. The Deme class finds the Chromosomes with the shortest total distance and choses those to exchange information. Using a genetic algorithm allows the program to improve upon the best solutions rather than attempting every permutation at random.

Files:
Five.tsv, linear.tsv, shortest.tsv, and challenge.tsv are all lists of coordinates to use as example cities.
The classes involved are Demes, Chromosomes, and Cities. A Deme is composed of Chromosomes, which are composed of Cities. Cities represents an order of cities(coordinates).
Tsp.cc is the file that includes the main function and incorporates all of the classes in conjunction.