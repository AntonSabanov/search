#include "pch.h"
#include <iostream>
#include <cstring>
#include <cassert>

//public class BST<Key extends Comparable<Key>, Value>
//{
//	private static final boolean RED = true;
//	private static final boolean BLACK = false;
//	private Node root;
//	private class Node
//	{
//		Key key;
//		Value val;
//		Node left, right;
//		boolean color;
//		Node(Key key, Value val, boolean color)
//		{
//			this.key = key;
//			this.val = val;
//			this.color = color;
//		}
//	}
//	public Value get(Key key)
//		// Search method.
//		public void put(Key key, Value val)
//		// Insert method.
//}

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

	Node() {} // конструктор для инициализации значений по умолчанию

/*	~Node()
	{
		this->key = key;
		this->val = val;
		this->color = color;
	}*/

	Node* left = nullptr;
	Node* right = nullptr;
	Node* parent = nullptr; // поле для итерации
};

public:
	bool red = true;
	bool black = false;
	Node* root = nullptr;
	//Node* lastNode = nullptr;
	//int size;

	class Iterator
	{
	private:
		//int curNodeIndex; // текущее положение итератора
		Dictionary<K, V>* curDict;
		Node* curNode = nullptr;
		Node* lastNode = nullptr;
	public:
		Iterator(Dictionary<K, V>* iterated)
		{
			this->curDict = iterated;
			this->curNode = iterated->GetLeftmostNode(iterated->root);//iterated->root; // отсчет с самой левой ноды
			//curChunkIndex = 0;
		}

		//int Compare(const K& key1, const K& key2)
		//{
		//	if (key1 == key2) return 0;
		//	else if (key1 > key2) return 1;
		//	else if (key1 < key2) return -1;
		//}

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
						lastNode = curNode; // запоминаю последнюю ноду до ухода вправо
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
							lastNode = curNode; //запоминаю последнюю ноду до ухода влево
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
						lastNode = curNode; // запоминаю последнюю ноду до ухода вправо
					//auto tmp = curNode;
					curNode = curNode->left;
					//curNode->parent = tmp;
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
							lastNode = curNode; //запоминаю последнюю ноду до ухода влево
							curNode = curDict->GetRightmostNode(curNode);
						}
					}
				}
			}
		}

		bool HasNext() const
		{
			if (curNode->right != nullptr)//в любом случае есть следующий
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
			if (curNode->left != nullptr)//в любом случае есть следующий
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
		//root = (Node*)malloc(sizeof(Node));
		//root;
		//root = nullptr;
		root = new Node();
		size = 0;
	}

	~Dictionary()
	{
		DestroyNodes(root);
	}

	Dictionary(const Dictionary&) = delete; //запрет копирования объектов класса
	void operator=(const Dictionary&) = delete;

	void DestroyNodes(Node* curNode) //удаление нод рекурсивно
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

	Node* RotateLeft(Node* h) // нужен ли ретурн?
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

	Node* Insert(Node* h, K key, V val) //рекурсивный алгоритм вставки
	{
		if (h == nullptr)
		{
			size++;
			return new Node(key, val, red);
		}		

		int cmp;		
		cmp = Compare(key, h->key);					//сравнение ключа текущей ноды и переданного
		/*if (key == h->key) cmp = 0; 
		else if (key > h->key) cmp = 1; 
		else if (key < h->key) cmp = -1;*/

		if (cmp == 0)	                            // если они совпадают, то значение просто заменяется
			h->val = val;
		else if (cmp < 0)                           // если меньше, то идем в левое поддерево
			h->left = Insert(h->left, key, val);
		else                                        // если больше, то идем в правое поддерево
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
				//h->val = Get(h->right, h->key);//
				h->val = Find(h->key, h->right)->val;//
				h->right = DeleteMin(h->right);//
				delete h;
			}
			else h->right = Delete(h->right, key);
			//delete h;
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

	Node* GetLeftmostNode(Node* curNode) // самая левая нода в поддереве
	{
		while (curNode->left != nullptr)
		{
			auto tmp = curNode;
			curNode = curNode->left;
			curNode->parent = tmp;
		}
		return curNode;
	}

	Node* GetRightmostNode(Node* curNode) // самая правая нода в поддереве
	{
		while (curNode->right != nullptr)
		{
			//auto tmp = curNode;
			curNode = curNode->right;
			//curNode->parent = tmp;
		}
		return curNode;
	}

	//V& Get(Node* x, K key)
	//{
	//	while (x != nullptr)
	//	{
	//		int cmp = (key, x->key);
	//		if (cmp == 0) 
	//			return x->val;
	//		else if (cmp < 0) 
	//			x = x->left;
	//		else if (cmp > 0) 
	//			x = x->right;
	//	}
	//}

	Node* Find(const K& key, Node* curNode)
	{
		while (curNode != nullptr)
		{
			int cmpResult;									//добавить сравнение с ключом
			cmpResult = Compare(key, curNode->key);
			//if (key == curNode->key) cmpResult = 0;
			//else if (key > curNode->key) cmpResult = 1;
			//else if (key < curNode->key) cmpResult = -1;
			//int cmpResult = strcmp(key, curNode->key);

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
void Dictionary<K, V>::Put(const K& key, const V& val) //Добавляет переданную пару ключ-значение в ассоциативный массив. Если такой ключ уже существует, связанное с ним значение должно быть заменено на переданное
{
	if (size == 0)
	{
		//root = new Node(key, val, red);
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
		
	//Node* curNode = root;
	//
	//
	//int cmp; //= key.compareTo(curNode->key);//сравнение ключа текущей ноды и переданного
	//if (key == curNode->key) cmp = 0; else if (key > curNode->key) cmp = 1; else if (key < curNode->key) cmp = -1;
	//
	//if (cmp == 0)	// если они совпадают, то значение просто заменяется
	//{
	//	curNode->val = val;
	//	return;
	//}
	//else if (cmp < 0) // если меньше, то идем в левое поддерево
	//{
	//	if (curNode->left == nullptr)
	//	{
	//		curNode->left = new Node(key, val, red);
	//		//curNode->left->parent = curNode;
	//		break;
	//	}
	//	else
	//	{
	//		curNode = curNode->left;
	//	}
	//}
	//else // если больше, то идем в правое поддерево
	//{
	//	if (curNode->right == nullptr)
	//	{
	//		curNode->right = new Node(key, val, red);
	//		//curNode->right->parent = curNode;
	//		break;
	//	}
	//	else
	//	{
	//		curNode = curNode->right;
	//	}
	//}
	////curNode = new Node(key, val, red); // красный или черный?
	//
	//if (curNode->right != nullptr && IsRed(curNode->right))
	//{
	//	curNode = RotateLeft(curNode);
	//	//auto temp = curNode->parent;
	//	//curNode->parent = curNode->left->parent;
	//	//curNode->left->parent = curNode;		
	//}
	//	
	//
	////if (curNode->left != nullptr && curNode->left->left != nullptr && IsRed(curNode->left) && IsRed(curNode->left->left))//
	////	curNode = RotateRight(curNode);
	////if (curNode->left != nullptr && curNode->parent != nullptr && IsRed(curNode->parent) && IsRed(curNode->left))//
	////	curNode = RotateRight(curNode);
	//if (curNode->left != nullptr && lastParent != nullptr && IsRed(curNode->left) && IsRed(lastParent))//
	//	curNode = RotateRight(lastParent);
	//
	//if (curNode->right != nullptr && curNode->left != nullptr && IsRed(curNode->left) && IsRed(curNode->right))
	//	ColorFlip(curNode);//
	//
	//lastParent = curNode;
	
}

template <typename K, typename V>
void Dictionary<K, V>::Remove(const K& key) //Удаляет элемент с указанным ключом из ассоциативного массива
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
bool Dictionary<K, V>::Contains(const K& key) //Возвращает true , если в ассоциативном массиве существует элемент с указанным ключом
{
	Node* curNode = nullptr;
	if (root != nullptr)
		curNode = root;
	else
		return false;

	//while (curNode != nullptr)
	//{
	//	int cmpResult;					//добавить сравнение с ключом
	//	if (key == curNode->key) cmpResult = 0;
	//	else if (key > curNode->key) cmpResult = 1;
	//	else if (key < curNode->key) cmpResult = -1;
	//	//int cmpResult = strcmp(key, curNode->key);
	//
	//	if (cmpResult == 0)
	//		return true;
	//	else if (cmpResult < 0)
	//		curNode = curNode->left;
	//	else if (cmpResult > 0)
	//		curNode = curNode->right;
	//}
	//return false;
	
	if (Find(key, curNode) != nullptr)
		return true;
	else
		return false;

	//public Value get(Key key)
	//{
	//	Node x = root;
	//	while (x != null)
	//	{
	//		int cmp = key.compareTo(x.key);
	//		if (cmp == 0) return x.val;
	//		else if (cmp < 0) x = x.left;
	//		else if (cmp > 0) x = x.right;
	//	}
	//	return null;
	//}
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
	if (searchedNode != nullptr)			// если искомое значение есть то возвращет его
	{
		return searchedNode->val;
	}	
	else									// если	нету, то создать ноду и вернуть ее значение по умолчанию
	{
		searchedNode = new Node(); 
		return searchedNode->val;
	}
}

template <typename K, typename V>
int Dictionary<K, V>::Size() const //Возвращает текущий размер (количество реально существующих в ассоциативном массиве элементов).
{
	return size;
}

//template <typename K, typename V>
//Dictionary<K, V>::Iterator Dictionary<K, V>::GetIterator()
//{
//	Iterator iterator(this);
//	return iterator;
//}