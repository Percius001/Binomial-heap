#include<iostream>
#include<vector>
#include<iterator>
#include<climits>

using namespace std;


struct Node
{
	Node(int data)
	{
		this->data = data;
		this->degree = 0;
		this->parent = this->child = this->sibling = NULL;
	}
	int data;
	int degree;
	Node* parent;
	Node* child;
	Node* sibling;
};


Node* MergeBinomialTree(Node* b1 , Node* b2)
{
	if(b1->data > b2->data)
	{
		swap(b1,b2);
	}

	b2->parent = b1;
	b2->sibling = b1->child;
	b1->child = b2;
	b1->degree++;

	return b1;
}

vector<Node*> UnionBinomialHeap(vector<Node*> h1 , vector<Node*> h2)
{
	vector<Node*> _new;
	vector<Node*>::iterator it1 = h1.begin();
	vector<Node*>::iterator it2 = h2.begin();

	while(it1 != h1.end() && it2 != h2.end())
	{
		if((*it1)->degree <= (*it2)->degree)
		{
			_new.push_back(*it1);
			it1++;
		}
		else
		{
			_new.push_back(*it2);
			it2++;
		}
	}

	while(it1 != h1.end())
	{
		_new.push_back(*it1);
		it1++;
	}

	while(it2 != h2.end())
	{
		_new.push_back(*it2);
		it2++;
	}

	return _new;
}

vector<Node*> adjust(vector<Node*> _heap)
{
	if(_heap.size() <= 1)
		return _heap;
	vector<Node*>::iterator it1,it2,it3;
	it1 = it2 = it3 = _heap.begin();

	if(_heap.size() == 2)
	{
		it2 = it1;
		it2++;
		it3 = _heap.end();
	}
	else
	{
		it2++;
		it3 = it2;
		it3++;
	}

	while(it1 != _heap.end())
	{
		if(it2 == _heap.end())
			it1++;
		else if((*it1)->degree < (*it2)->degree)
		{
			it1++;
			it2++;
			if(it3 != _heap.end())
				it3++;
		}
		else if(it3 != _heap.end() && (*it1)->degree == (*it2)->degree && (*it1)->degree == (*it3)->degree)
		{
			it1++;
			it2++;
			it3++;
		}
		else if((*it1)->degree == (*it2)->degree)
		{
			*it1 = MergeBinomialTree(*it1 ,*it2);
			it2 = _heap.erase(it2);
			if(it3 != _heap.end())
				it3++;
		}
	}

	return _heap;
}

vector<Node*> insertATreeInHeap(vector<Node*> _heap,Node* tree)
{
	vector<Node*> temp;

	temp.push_back(tree);

	temp = UnionBinomialHeap(_heap,temp);

	return adjust(temp);
}

vector<Node*> removeMinFromTree(Node* tree)
{
	vector<Node*> heap;
	Node* temp = tree->child;
	Node* lol;
	while(temp)
	{
		lol = temp;
		temp = temp->sibling;
		lol->sibling = NULL;
		heap.insert(heap.begin(),lol);
	}

	return heap;
}

vector<Node*> insert(vector<Node*> _heap,int data)
{
	Node* newNode = new Node(data);
	return insertATreeInHeap(_heap , newNode);
}

Node* getMin(vector<Node*> _heap)
{
	vector<Node*>::iterator it = _heap.begin();
	Node* temp = *it;

	while(it != _heap.end())
	{
		if((*it)->data < temp->data)
		{
			temp = (*it);
		}
		it++;
	}

	return temp;
}

vector<Node*> extractMin(vector<Node*> _heap)
{
	vector<Node*> lol,new_heap;
	Node* temp;
	vector<Node*>::iterator it = _heap.begin();

	temp = getMin(_heap);

	while(it != _heap.end())
	{
		if((*it) != temp)
			new_heap.push_back((*it));
		it++;
	}

	lol = removeMinFromTree(temp);
	new_heap = UnionBinomialHeap(new_heap,lol);
	new_heap = adjust(new_heap);

	return new_heap;
}

Node* findNode(Node* root,int val)
{
	if(!root)
		return NULL;
	if(root->data == val)
		return root;
	Node* res = findNode(root->child,val);
	if(res)
		return res;
	return findNode(root->sibling,val); 
}

Node* searchRoot(vector<Node*> heap,int val)
{
	vector<Node*>::iterator root = heap.begin();
	while(root != heap.end())
	{
		Node* res = findNode(*root,val);
		if(res)
			return res;
		root++;
	}

	return NULL;
}

void decreaseKey(vector<Node*> heap,int old_val,int new_val)
{
	Node* node = searchRoot(heap,old_val);

	if(!node)
		return;
	node->data = new_val;
	Node* parent = node->parent;

	while(parent!=NULL && node->data<parent->data)
	{
		swap(node->data,parent->data);
		node = parent;
		parent = parent->parent;
	}
}

vector<Node*> deleteKey(vector<Node*> heap,int val)
{

	decreaseKey(heap,val,INT_MIN);
	return extractMin(heap);
}

void printTree(Node* h)
{
	while(h){
	cout<<h->data<<"\t";
	printTree(h->child);
	h = h->sibling;	
	}
	
}

void printHeap(vector<Node*> _heap)
{
	vector<Node*>::iterator it = _heap.begin();
	while(it != _heap.end())
	{
		printTree((*it));
		it++;
	}
}

int main()
{
	vector<Node*> heap;

	heap = insert(heap,10);
	heap = insert(heap,20);
	heap = insert(heap,30);
	heap = insert(heap,40);
	heap = insert(heap,50);

	cout<<"The binomial heap:";
	printHeap(heap);
	cout<<endl;
	heap = deleteKey(heap,10);
	cout<<"The binomial heap after:";
	printHeap(heap);

}