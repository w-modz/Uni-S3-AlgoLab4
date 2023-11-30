// ALGO2 IS1 221A LAB03
// Wiktor Modzelewski
// mw53766@zut.edu.pl
#include <iostream>
#include <string>
#include <ctime>
#include <random>
#include <list>
#include <algorithm>
#include <sstream>

template<typename T>
struct node
{
	T data;
	node* parent;
	node* left;
	node* right;

	node(const T& data, node* parent, node* left, node* right)
		: data(data), parent(parent), left(left), right(right)
	{}

	node(const T& data)
		: data(data), parent(nullptr), left(nullptr), right(nullptr)
	{}

	~node()
	{
		if (parent != nullptr)
		{
			parent = nullptr;
		}
		if (left != nullptr)
		{
			delete left;
			left = nullptr;
		}
		if (right != nullptr)
		{
			delete right;
			right = nullptr;
		}
	}
};

template<typename T>
class BST
{
private:
	uint32_t size;
	node<T>* root;
public:
	BST() noexcept
		: root(nullptr), size(0)
	{}

	~BST()
	{
		clear();
	}

	uint32_t get_size() const noexcept
	{
		return size;
	}

	node<T>* getRoot() const noexcept
	{
		return root;
	}

	void Insert(T new_data)
	{
		if (root == nullptr)
		{
			root = new node<T>(new_data);
		}
		else
		{
			node<T>* new_node = new node<T>(new_data);
			bool inserted = false;
			node<T>* previous_node = nullptr;
			node<T>* current_node = root;
			bool is_less;
			while (!inserted)
			{
				if (current_node == nullptr)
				{
					new_node->parent = previous_node;
					if (is_less)
					{
						previous_node->left = new_node;
					}
					else
					{
						previous_node->right = new_node;
					}
					inserted = true;
				}
				else if (new_node->data == current_node->data) return;
				else if (new_node->data < current_node->data)
				{
					previous_node = current_node;
					current_node = current_node->left;
					is_less = true;
				}
				else if (new_node->data > current_node->data)
				{
					previous_node = current_node;
					current_node = current_node->right;
					is_less = false;
				}
			}
		}
		size++;
	}
	// Function that visits the root, traverses the left subtree, then the right 
	void preorder(node<T>* root, std::list<T>* list)
	{
		if (root == nullptr)
		{
			return;
		}
		list->push_back(root->data);
		preorder(root->left,list);
		preorder(root->right,list);
	}
	// Function that traverses the left subtree, visits the root and traverses the right subtree
	void inorder(node<T>* root, std::list<T>* list)
	{
		if (root == nullptr)
		{
			return;
		}
		inorder(root->left, list);
		list->push_back(root->data);
		inorder(root->right, list);
	}

	void print_traversal(std::string str)
	{
		std::list<int>* preorder_list = new std::list<int>;
		if (str == "preorder")
		{
			preorder(root, preorder_list);
		}
		else if (str == "inorder")
		{
			inorder(root, preorder_list);
		}
		else
		{
			throw std::invalid_argument("invalid function argument");
		}

		for (std::list<int>::iterator i = preorder_list->begin(); i != preorder_list->end(); ++i)
		{
			std::cout << *i << " ";
		}
		std::cout << "\n";
	}
	// Function that return the height of the given node's subtree
	uint32_t findHeight(node<T>* root)
	{
		if (root == nullptr)
		{
			return 0;
		}
		uint32_t left_height = findHeight(root->left);
		uint32_t right_height = findHeight(root->right);
		return std::max(left_height, right_height) + 1;
	}

	std::string to_string(node<T>* root)
	{
		std::ostringstream output_stream;
		output_stream << "---BST Structure---\nNotation: (data,parent data)\n";
		int height = findHeight(root);
		if (height > 5)
		{
			output_stream << "(Only the top 5 levels of the tree have been shown)\n";
			height = 5;
		}
		for (int i = 1; i <= height; i++)
		{
			output_stream << "Level " << i << ": ";
			output_stream << getLevel(root,i);
			output_stream << "\n";
		}
		return output_stream.str();
	}
	// Returns the ith level of bst as string
	std::string getLevel(node<T>* root, int level)
	{
		std::ostringstream output_stream;
		if (root == nullptr)
		{
			return "( )";
		}
		if (level == 1)
		{
			if (root->parent != nullptr)
			{
				output_stream << "(" << root->data << "," << root->parent->data << ")";
			}
			else
			{
				output_stream << "(" << root->data << "," << "NULL" << ")";
			}
		}
		else if (level > 1) 
		{
			output_stream << getLevel(root->left, level - 1);
			output_stream << getLevel(root->right, level - 1);
		}
		return output_stream.str();
	}
	template <typename Comp>
	node<T>* getNode(node<T>* element, Comp equal, Comp greater, Comp less)
	{
		node<T>* current_node = root;
		while (true)
		{
			if (current_node == nullptr)
			{
				return nullptr;
			}
			else if (equal(current_node->data,element->data))
			{
				return current_node;
			}
			else if (greater(current_node->data,element->data))
			{
				current_node = current_node->left;
			}
			else if (less(current_node->data,element->data))
			{
				current_node = current_node->right;
			}
		}
	}

	void deleteNode(node<T>* element)
	{
		if (element == nullptr)
		{
			return;
		}
		if (element->left == nullptr && element->right == nullptr)
		{
			if (element == root)
			{
				root = nullptr;
			}
			else if (element->parent->left == element)
			{
				element->parent->left = nullptr;
			}
			else
			{
				element->parent->right = nullptr;
			}
			element->left = nullptr;
			element->right = nullptr;
			delete element;
			size--;
		}
		else if (element->left == nullptr || element->right == nullptr)
		{
			if (element->parent == nullptr)
			{
				if (element->left != nullptr)
				{
					root = element->left;
					root->parent = nullptr;
				}
				else
				{
					root = element->right;
					root->parent = nullptr;
				}
			}
			else if (element->left != nullptr)
			{
				if (element->parent->left == element)
				{
					element->parent->left = element->left;
					element->left->parent = element->parent;
				}
				else
				{
					element->parent->right = element->left;
					element->left->parent = element->parent;
				}
			}
			else
			{
				if (element->parent->left == element)
				{
					element->parent->left = element->right;
					element->right->parent = element->parent;
				}
				else
				{
					element->parent->right = element->right;
					element->right->parent = element->parent;
				}
			}
			element->left = nullptr;
			element->right = nullptr;
			delete element;
			size--;
		}
		else
		{
			node<T>* successor = inorderSuccessor(element);
			if (element->right == successor)
			{
				element->right = successor->right;
				if (successor->right)
				{
					successor->right->parent = element;
				}
			}
			else
			{
				successor->parent->left = successor->right;
				if (successor->right)
				{
					successor->right->parent = successor->parent;
				}
			}
			element->data = successor->data;
			successor->left = nullptr;
			successor->right = nullptr;
			delete successor;
			size--;
		}
	}
	// Function that looks for the smallest value in the right subtree of a given node
	node<T>* inorderSuccessor(node<T>* root)
	{
		if (root == nullptr || root->right == nullptr)
		{
			return nullptr;
		}
		node<T>* current_node = root->right;
		while (current_node->left != nullptr)
		{
			current_node = current_node->left;
		}
		return current_node;
	}

	T get_min_node_data()
	{
		if (root == nullptr)
		{
			return NULL;
		}
		node<T>* current_node = root;
		while (current_node->left != nullptr)
		{
			current_node = current_node->left;
		}
		return current_node->data;
	}

	void clear()
	{
		delete root;
		size = 0;
		root = nullptr;
	}
};
template<typename T>
bool compare(const T data1, const T data2)
{
	return data1 == data2;
}

template<typename T>
bool greater(const T data1, const T data2)
{
	return data1 > data2;
}

template<typename T>
bool less(const T data1, const T data2)
{
	return data1 < data2;
}

#include <map>
#include <vector>
#include <numeric>
#include <algorithm>
void test()
{
	std::vector<int> v(8);
	std::iota(v.begin(), v.end(), 0);
	std::map<int, int> heights, min_node_data;
	do
	{
		BST<int> t;
		for (int i : v)
			t.Insert(i);
		++heights[t.findHeight(t.getRoot())];
		for (int i : v)
		{
			++min_node_data[t.get_min_node_data()];
			node<int>* so = new node<int>(i);
			t.deleteNode(t.getNode(so,compare<int>,greater<int>,less<int>));
		}
	} while (std::next_permutation(v.begin(), v.end()));
	for (auto [height, cnt] : heights)
		std::cout << height << ": " << cnt << std::endl;
	std::cout << "***" << std::endl;
	for (auto [data, cnt] : min_node_data)
		std::cout << data << ": " << cnt << std::endl;
}

int main()
{
	/*srand(time(NULL));
	const int MAX_ORDER = 7;
	BST <int>* bst = new BST<int>();
		for (int o = 1; o <= MAX_ORDER; o++)
		{
			const int n = pow(10, o);
			clock_t t1 = clock();
			for (int i = 0; i < n; i++)
			{
				int so = rand() % (10*n + 1);
				bst->Insert(so);
			}
			clock_t t2 = clock();
			std::cout << bst->to_string(bst->getRoot()) << "\n";
			std::cout << "BST size: " << bst->get_size() << "\n";
			std::cout << "Total time of inserting = " << ((t2 - t1) / (double)CLOCKS_PER_SEC) * 1000 << "ms\n";
			std::cout << "Average time of inserting = " << ((t2 - t1) / (double)CLOCKS_PER_SEC) * 1000 / n << "ms\n";

			const int m = pow(10, 4);
			int hits = 0;
			t1 = clock();
			for (int i = 0; i < m; i++)
			{
				node<int>* so = new node(rand() % (10*m + 1));
				node<int>* result = bst->getNode(so, compare<int>,greater<int>,less<int>);
				if (result != NULL)
					hits++;
				delete so;
			}
			t2 = clock();
			std::cout << "Amout of hits: " << hits << "\n";
			std::cout << "Total time of searching = " << ((t2 - t1) / (double)CLOCKS_PER_SEC) * 1000 << "ms\n";
			std::cout << "Average time of searching = " << ((t2 - t1) / (double)CLOCKS_PER_SEC) * 1000 / m << "ms\n\n";
			bst->clear();
		}
	delete bst;
	return 0;*/


	test();
}
