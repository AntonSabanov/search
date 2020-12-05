#include "pch.h"
#include <iostream>
#include <cstring>
#include <cassert>

template <typename K, typename V>
class Dictionary final
{
private:
	int size;

class Node
{
public:
	K key;
	V val;
	bool color;

	Node(K key, V val, bool color)
	{
		this->key = key;
		this->val = val;
		this->color = color;
	}

	Node() {} // ����������� ��� ������������� �������� �� ���������

	Node* left = nullptr;
	Node* right = nullptr;
	Node* parent = nullptr; // ���� ��� ��������
};

public:
	bool red = true;
	bool black = false;
	Node* root = nullptr;

	class Iterator
	{
	private:
		Dictionary<K, V>* curDict;
		Node* curNode = nullptr;
		Node* lastNode = nullptr;
	public:
		Iterator(Dictionary<K, V>* iterated)
		{
			this->curDict = iterated;
			this->curNode = iterated->GetLeftmostNode(iterated->root); // ������ � ����� ����� ����
		}

		const K& Key() const
		{
			return this->curNode->key;
		}

		const V& Get() const
		{
			return this->curNode->val;
		}

		void Set(const V& value)
		{
			curNode->val = value;
		}

		void Next()
		{
			if (HasNext())
			{
				if (curNode->right != nullptr)
				{
					if (curNode->parent != nullptr && curDict->Compare(curNode->parent->key, curNode->key) > 0)
						lastNode = curNode; // ��������� ��������� ���� �� ����� ������
					auto tmp = curNode;
					curNode = curNode->right;
					curNode->parent = tmp;
				}
				else if (curNode->parent != nullptr && curDict->Compare(curNode->parent->key, curNode->key) > 0)
				{
					curNode = curNode->parent;
				}
				else if (curDict->Compare(curNode->parent->key, curNode->key) < 0 && lastNode->parent != nullptr)
				{
					curNode = lastNode->parent;
				}				
				if (curNode->left != nullptr)
				{
					if (curNode->left != lastNode && curNode != lastNode)
					{
						if (curDict->Compare(curNode->key, curNode->left->key) > 0 
							&& curNode->parent != nullptr 
							&& curDict->Compare(curNode->parent->key, curNode->left->key) < 0 )
						{
							lastNode = curNode; //��������� ��������� ���� �� ����� �����
							curNode = curDict->GetLeftmostNode(curNode);
						}
					}
				}
			}
		}

		void Prev()
		{
			if (HasPrev())
			{
				if (curNode->left != nullptr)
				{
					if (curNode->parent != nullptr && curDict->Compare(curNode->parent->key, curNode->key) < 0)
						lastNode = curNode; // ��������� ��������� ���� �� ����� ������
					curNode = curNode->left;
				}
				else if (curNode->parent != nullptr && curDict->Compare(curNode->parent->key, curNode->key) < 0)
				{
					curNode = curNode->parent;
				}
				else if (curDict->Compare(curNode->parent->key, curNode->key) > 0 && lastNode->parent != nullptr)
				{
					curNode = lastNode->parent;
				}
				if (curNode->right != nullptr)
				{
					if (curNode->right != lastNode && curNode != lastNode)
					{
						if (curDict->Compare(curNode->key, curNode->right->key) < 0 
							&& curNode->parent != nullptr
							&& curDict->Compare(curNode->parent->key, curNode->right->key) > 0)
						{
							lastNode = curNode; //��������� ��������� ���� �� ����� �����
							curNode = curDict->GetRightmostNode(curNode);
						}
					}
				}
			}
		}

		bool HasNext() const
		{
			if (curNode->right != nullptr)//� ����� ������ ���� ���������
			{
				return true;
			}
			if (curNode->parent != nullptr && curDict->Compare(curNode->parent->key, curNode->key) > 0)
			{
				return true;
			}
			else if (curNode->parent != nullptr && curDict->Compare(curNode->parent->key, curNode->key) < 0 
					&& lastNode != nullptr 
					&& lastNode->parent != nullptr && curDict->Compare(lastNode->parent->key, lastNode->key) > 0)
			{
				return true;
			}

			return false;
		}

		bool HasPrev() const
		{
			if (curNode->left != nullptr)//� ����� ������ ���� ���������
			{
				return true;
			}
			if (curNode->parent != nullptr && curDict->Compare(curNode->parent->key, curNode->key) < 0)
			{
				return true;
			}
			else if (curNode->parent != nullptr && curDict->Compare(curNode->parent->key, curNode->key) > 0
				&& lastNode != nullptr
				&& lastNode->parent != nullptr && curDict->Compare(lastNode->parent->key, lastNode->key) < 0)
			{
				return true;
			}

			return false;
		}		
	};

	Dictionary()
	{
		root = new Node();
		size = 0;
	}

	~Dictionary()
	{
		DestroyNodes(root);
	}

	Dictionary(const Dictionary&) = delete; //������ ����������� �������� ������
	void operator=(const Dictionary&) = delete;

	void DestroyNodes(Node* curNode) //�������� ��� ����������
	{
		if (curNode == nullptr)
			return;
		DestroyNodes(curNode->left);
		DestroyNodes(curNode->right);
		delete curNode;
	}

	Node* RotateRight(Node* h)
	{
		auto x = h->left;
		h->left = x->right;
		x->right = h;
		x->color = x->right->color;
		x->right->color = red;
		return x;
	}

	Node* RotateLeft(Node* h) // ����� �� ������?
	{
		auto x = h->right;
		h->right = x->left;
		x->left = h;
		x->color = x->left->color;
		x->left->color = red;
		return x;
	}

	void ColorFlip(Node* h)
	{
		h->color = !h->color;
		h->left->color = !h->left->color;
		h->right->color = !h->right->color;
	}

	bool IsRed(Node* x)
	{
		if (x == nullptr)
			return false;
		return (x->color == red);
	}

	int Compare(const K& key1, const K& key2)
	{
		if (key1 == key2) return 0;
		else if (key1 > key2) return 1;
		else if (key1 < key2) return -1;
	}

	Node* Insert(Node* h, K key, V val) //����������� �������� �������
	{
		if (h == nullptr)
		{
			size++;
			return new Node(key, val, red);
		}		

		int cmp;		
		cmp = Compare(key, h->key);					//��������� ����� ������� ���� � �����������

		if (cmp == 0)	                            // ���� ��� ���������, �� �������� ������ ����������
			h->val = val;
		else if (cmp < 0)                           // ���� ������, �� ���� � ����� ���������
			h->left = Insert(h->left, key, val);
		else                                        // ���� ������, �� ���� � ������ ���������
			h->right = Insert(h->right, key, val);

		if (IsRed(h->right))
			h = RotateLeft(h);	

		if (IsRed(h->left) && IsRed(h->left->left))
			h = RotateRight(h);

		if (IsRed(h->left) && IsRed(h->right))
			ColorFlip(h);
		
		return h;
	}

	Node* Delete(Node* h, K key)
	{
		int cmp = Compare(key, h->key);
		if (cmp < 0)
		{
			if (!IsRed(h->left) && !IsRed(h->left->left))
				h = MoveRedLeft(h);//
			h->left = Delete(h->left, key);
		}
		else
		{
			if (IsRed(h->left))
				h = RotateRight(h);

			if (cmp == 0 && (h->right == nullptr))
			{
				delete h;
				return nullptr;
			}				

			if (!IsRed(h->right) && !IsRed(h->right->left))
				h = MoveRedRight(h); //

			if (cmp == 0)
			{
				h->key = Min(h->right);//
				h->val = Find(h->key, h->right)->val;//
				h->right = DeleteMin(h->right);//
				delete h;
			}
			else h->right = Delete(h->right, key);
		}
		return FixUp(h);//
	}

	void DeleteMin()
	{
		root = DeleteMin(root);
		root->color = black;
	}

	Node* DeleteMin(Node* h)
	{
		if (h->left == nullptr)
			return nullptr;
		if (!IsRed(h->left) && !IsRed(h->left->left))
			h = MoveRedLeft(h);
		h->left = DeleteMin(h->left);
		return FixUp(h);
	}

	Node* MoveRedLeft(Node* h)
	{
		ColorFlip(h);
		if (IsRed(h->right->left))
		{
			h->right = RotateRight(h->right);
			h = RotateLeft(h);
			ColorFlip(h);
		}
		return h;
	}

	Node* MoveRedRight(Node* h)
	{
		ColorFlip(h);
		if (IsRed(h->left->left))
		{
			h = RotateRight(h);
			ColorFlip(h);
		}
		return h;
	}

	Node* FixUp(Node* h)
	{
		if (IsRed(h->right))
			h = RotateLeft(h);
		if (IsRed(h->left) && IsRed(h->left->left))
			h = RotateRight(h);
		if (IsRed(h->left) && IsRed(h->right))
			ColorFlip(h);
		return h;
	}

	K& Min(Node* x)
	{
		while (x->left != nullptr)
			x = x->left;
		return x->key;		
	}

	Node* GetLeftmostNode(Node* curNode) // ����� ����� ���� � ���������
	{
		while (curNode->left != nullptr)
		{
			auto tmp = curNode;
			curNode = curNode->left;
			curNode->parent = tmp;
		}
		return curNode;
	}

	Node* GetRightmostNode(Node* curNode) // ����� ������ ���� � ���������
	{
		while (curNode->right != nullptr)
		{
			curNode = curNode->right;
		}
		return curNode;
	}

	Node* Find(const K& key, Node* curNode)
	{
		while (curNode != nullptr)
		{
			int cmpResult;									
			cmpResult = Compare(key, curNode->key);

			if (cmpResult == 0)
				return curNode;
			else if (cmpResult < 0)
				curNode = curNode->left;
			else if (cmpResult > 0)
				curNode = curNode->right;
		}
		return nullptr;
	}

	void Put(const K& key, const V& value);
	void Remove(const K& key);
	bool Contains(const K& key);
	const V& operator[](const K& key) const;
	V& operator[](const K& key);
	int Size() const;
	Iterator GetIterator()
	{
		Iterator iterator(this);
		return iterator;
	}
};

template <typename K, typename V>
void Dictionary<K, V>::Put(const K& key, const V& val) //��������� ���������� ���� ����-�������� � ������������� ������. ���� ����� ���� ��� ����������, ��������� � ��� �������� ������ ���� �������� �� ����������
{
	if (size == 0)
	{
		root->key = key;
		root->val = val;
		root->color = red;
		size++;
		return;
	}
	else
	{
		root = Insert(root, key, val);
	}
}

template <typename K, typename V>
void Dictionary<K, V>::Remove(const K& key) //������� ������� � ��������� ������ �� �������������� �������
{
	if (size == 0)
	{
		return;
	}
	else
	{
		root = Delete(root, key);
	}
}

template <typename K, typename V>
bool Dictionary<K, V>::Contains(const K& key) //���������� true , ���� � ������������� ������� ���������� ������� � ��������� ������
{
	Node* curNode = nullptr;
	if (root != nullptr)
		curNode = root;
	else
		return false;
	
	if (Find(key, curNode) != nullptr)
		return true;
	else
		return false;
}

template <typename K, typename V>
int Dictionary<K, V>::Size() const //���������� ������� ������ (���������� ������� ������������ � ������������� ������� ���������).
{
	return size;
}

template <typename K, typename V>
const V& Dictionary<K, V>::operator[](const K& key) const 
{
	assert(root != nullptr);
	Node* curNode = root;	
	auto searchedNode = Find(key, curNode);
	assert(searchedNode != nullptr);	
	return searchedNode->val;
}

template <typename K, typename V>
V& Dictionary<K, V>::operator[](const K& key)
{
	Node* curNode = nullptr;
	if (root != nullptr)
		curNode = root;
	else
	{
		root = new Node();
		return root->val;
	}
		
	auto searchedNode = Find(key, curNode);
	if (searchedNode != nullptr)			// ���� ������� �������� ���� �� ��������� ���
	{
		return searchedNode->val;
	}	
	else									// ����	����, �� ������� ���� � ������� �� �������� �� ���������
	{
		searchedNode = new Node(); 
		return searchedNode->val;
	}
}


