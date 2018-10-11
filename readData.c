/*
Functions we need to have:
	getCollection()
		returns List_of_Urls
		Create a set (list) of urls to process by reading data from file "collection.txt"
	getGraph(List_of_Urls)
		Returns Graph g
		Create empty graph (use graph ADT in say graph.h and graph.c)
		For each url in the above list
			- Read <url>.txt file, and update graph by adding a node and outgoing links
	GetInvertedList(List_of_Urls)
		Create empty inverted list (use say List of lists, BST where values are lists, etc)
		For each url in List_of_Urls
			- Read <url<.txt file, and update inverted index	



*/

#define SEEN 0
#define UNSEEN 1

set getCollection()
{
	FILE *file = fopen("collection.txt", "r");
	char *collection;
	int fscanf(file, collection);
	for (char *token = strtok(collection, " "); token != NULL; token = strtok(NULL, " ")) {
		// put the token in the set
	}
	
	free(collection);
}

graph getGraph(set listOfUrls)
{
	graph g = newGraph();
	// go through listOfUrls and update the graph by adding a node and outgoing links
	return g;
}

list getInvertedList(set listOfUrls)
{
}
