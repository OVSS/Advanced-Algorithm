#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#define MAX_NODE_NUM 1000
using namespace std;
typedef enum { ME, SE } GraphKind;
typedef struct NODE
{
	int id;
	struct LINK* link;
	struct NODE* prev;
	struct NODE* next;
}Node;

typedef struct LINK
{
	Node* startVertices;
	Node* endVertices;
	struct LINK* next;
}Link;

typedef struct
{
	Node* head;
	vector<Node*> vertices;
	vector<Link*> links;
	int verNum, acrNum;
	GraphKind kind;
}Graph;

void print(Graph graph) {
	cout << "---------" << endl;
	for_each(graph.vertices.begin(), graph.vertices.end(), [](Node* node) {cout << node->id << " "; for (Link* l = node->link; l; l = l->next) cout << l->endVertices->id << " "; cout << endl; });
	cout << "---------" << endl;
}

void InsertEdge(Graph& graph, Node*& startNode, Node*& endNode) {
	Link* incoming = new Link;
	incoming->startVertices = startNode;
	incoming->endVertices = endNode;
	incoming->next = startNode->link;
	startNode->link = incoming;
	graph.links.push_back(incoming);
	Link* outcoming = new Link;
	outcoming->startVertices = endNode;
	outcoming->endVertices = startNode;
	outcoming->next = endNode->link;
	endNode->link = outcoming;
	graph.links.push_back(outcoming);
}

void InsertEdge(Graph& graph, int startPoint, int endPoint) {
	InsertEdge(graph, graph.vertices[startPoint], graph.vertices[endPoint]);
}


void CreateGraph(Graph& graph) {
	graph.kind = ME;
	int start = 0, end = 0;
	cout << "please input number of vertices and arcs" << endl;
	cin >> graph.verNum >> graph.acrNum;
	graph.head = NULL;
	for (int i = 0; i<graph.verNum; i++) {
		Node* node = new Node;
		node->id = i;
		node->prev = NULL;
		node->link = NULL;
		node->next = graph.head;
		graph.head = node;
		if (node->next) {
			node->next->prev = node;
		}
		graph.vertices.push_back(node);
	}
	for (int i = 0; i<graph.acrNum; i++) {
		int startPoint, endPoint;
		cout << "please input start point and end point" << endl;
		cin >> startPoint >> endPoint;
		InsertEdge(graph, startPoint, endPoint);
	}
}

void freeNode(Graph& graph, Node* node) {
	for (Link* l = node->link; l;) {
		Link* p = l->next;
		graph.links.erase(remove(graph.links.begin(), graph.links.end(), l), graph.links.end());
		free(l);
		l = p;
	}
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	graph.vertices.erase(remove(graph.vertices.begin(), graph.vertices.end(), node), graph.vertices.end());
	free(node);
}

void DeleteEdge(Graph& graph, Node* startNode, Node* endNode) {
	Node* node1 = startNode;
	Link* l = node1->link;
	if (l->endVertices == endNode) {
		node1->link = l->next;
		graph.links.erase(remove(graph.links.begin(), graph.links.end(), l), graph.links.end());
		free(l);
		return;
	}
	while (l->next&&l->next->endVertices != endNode) {
		l = l->next;
	}
	Link* p = l->next;
	l->next = p->next;
	graph.links.erase(remove(graph.links.begin(), graph.links.end(), p), graph.links.end());
	free(p);
}

void contract(Graph& graph, Node* verticesNode1, Node* verticesNode2) {
	//Add node2 to node1
	for (Link* l = verticesNode2->link; l; l = l->next) {
		DeleteEdge(graph, l->endVertices, verticesNode2);
		//print(graph);
		if (l->endVertices != verticesNode1) {
			InsertEdge(graph, verticesNode1, l->endVertices);
		}
	}
	//delete node2
	freeNode(graph, verticesNode2);
}

int minicut(Graph graph) {
	while (graph.vertices.size()>2) {
		int pickId = rand() % graph.links.size();
		int node1 = graph.links[pickId]->startVertices->id;
		int node2 = graph.links[pickId]->endVertices->id;
		if (node1<node2)
			contract(graph, graph.links[pickId]->startVertices, graph.links[pickId]->endVertices);
		else
			contract(graph, graph.links[pickId]->endVertices, graph.links[pickId]->startVertices);
	}
	return graph.links.size() / 2;
}

Graph CopyGraph(Graph graph) {
	//cout<<"copy graph"<<endl;
	Graph copy;
	copy.kind = ME;
	copy.head = NULL;
	for (int i = 0; i<graph.verNum; i++) {
		Node* node = new Node;
		node->id = i;
		node->prev = NULL;
		node->link = NULL;
		node->next = copy.head;
		copy.head = node;
		if (node->next) {
			node->next->prev = node;
		}
		copy.vertices.push_back(node);
	}
	for_each(graph.links.begin(), graph.links.end(), [&copy](Link *link) {
		Link* copyLink = new Link;
		int startPoint = link->startVertices->id;
		int endPoint = link->endVertices->id;
		copyLink->startVertices = copy.vertices[startPoint];
		copyLink->endVertices = copy.vertices[endPoint];
		copyLink->next = copy.vertices[startPoint]->link;
		copy.vertices[startPoint]->link = copyLink;
		copy.links.push_back(copyLink);
	});
	return copy;
}

int main() {
	srand(time(0));
	Graph graph;
	CreateGraph(graph);
	print(graph);
	int minicutNum = graph.verNum;
	int iteration = 0;
	iteration = graph.acrNum*graph.acrNum;
	for (int i = 0; i<iteration; i++) {
		minicutNum = min(minicutNum, minicut(CopyGraph(graph)));
	}
	cout << "minicut:" << minicutNum << endl;
	return 0;
}