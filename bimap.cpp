#include <iostream>
#include <cstdio>
#include "bimap.h"

//Created by slelaron

//using namespace std;

bimap::bimap():
	root_left(nullptr),
	root_right(nullptr)
{
}

bimap::~bimap()
{
	delete_all_nodes(root_left);
}

void bimap::delete_all_nodes(node* pointer)
{
	if (pointer == nullptr) return;
	delete_all_nodes(pointer->left_left);
	delete_all_nodes(pointer->left_right);
	delete pointer;
}

bimap::node* bimap::merge(node* l, node* r, bool part)
{
	if (l == nullptr) return r;
	if (r == nullptr) return l;
	if (part)
	{
		if (l->left_balance > r->left_balance)
		{
			l->left_right = merge(l->left_right, r, part);
			if (l->left_right != nullptr)
				l->left_right->left_parent = l;
			l->left_parent = nullptr;
			return l;
		}
		else
		{
			r->left_left = merge(l, r->left_left, part);
			if (r->left_left != nullptr)
				r->left_left->left_parent = r;
			r->left_parent = nullptr;
			return r;
		}
	}
	else
	{
		if (l->right_balance > r->right_balance)
		{
			l->right_right = merge(l->right_right, r, part);
			if (l->right_right != nullptr)
				l->right_right->right_parent = l;
			l->right_parent = nullptr;
			return l;
		}
		else
		{
			r->right_left = merge(l, r->right_left, part);
			if (r->right_left != nullptr)
				r->right_left->right_parent = r;
			r->right_parent = nullptr;
			return r;
		}
	}
}

std::pair<bimap::node*, bimap::node*> bimap::split(node* root, node* splitter, bool part)
{
	if (root == nullptr)
		return std::make_pair(nullptr, nullptr);
	if (part)
	{
		if (root->left_data < splitter->left_data)
		{
			std::pair <node*, node*> tmp = split(root->left_right, splitter, part);
			root->left_right = tmp.first;
			if (tmp.first != nullptr)
				tmp.first->left_parent = root;
			root->left_parent = nullptr;
			return std::make_pair(root, tmp.second);
		}
		else
		{
			std::pair <node*, node*> tmp = split(root->left_left, splitter, part);
			root->left_left = tmp.second;
			if (tmp.second != nullptr)
				tmp.second->left_parent = root;
			root->left_parent = nullptr;
			return std::make_pair(tmp.first, root);
		}
	}
	else
	{
		if (root->right_data < splitter->right_data)
		{
			std::pair <node*, node*> tmp = split(root->right_right, splitter, part);
			root->right_right = tmp.first;
			if (tmp.first != nullptr)
				tmp.first->right_parent = root;
			root->right_parent = nullptr;
			return std::make_pair(root, tmp.second);
		}
		else
		{
			std::pair <node*, node*> tmp = split(root->right_left, splitter, part);
			root->right_left = tmp.second;
			if (tmp.second != nullptr)
				tmp.second->right_parent = root;
			root->right_parent = nullptr;
			return std::make_pair(tmp.first, root);
		}
	}
}

bimap::left_iterator::left_iterator(node* point, const bimap* that):
	pointer(point),
	whose(that)
{}

bimap::right_iterator::right_iterator(node* point, const bimap* that):
	pointer(point),
	whose(that)
{}

bimap::left_iterator& bimap::left_iterator::operator++()
{
	if (pointer->left_right != nullptr)
	{
		pointer = pointer->left_right;
		while (pointer->left_left != nullptr)
			pointer = pointer->left_left;
	}
	else
	{
		while (pointer->left_parent != nullptr && pointer->left_parent->left_right == pointer)
			pointer = pointer->left_parent;
		pointer = pointer->left_parent;
	}
	return *this;
}

bimap::left_iterator bimap::left_iterator::operator++(int)
{
	left_iterator it(*this);
	++(*this);
	return it;
}

bimap::left_iterator& bimap::left_iterator::operator--()
{
	if (pointer == nullptr)
	{
		pointer = whose->root_left;
		while (pointer->left_right != nullptr)
			pointer = pointer->left_right;
	}
	else if (pointer->left_left != nullptr)
	{
		pointer = pointer->left_left;
		while (pointer->left_right != nullptr)
			pointer = pointer->left_right;
	}
	else
	{
		while (pointer->left_parent != nullptr && pointer->left_parent->left_left == pointer)
			pointer = pointer->left_parent;
		pointer = pointer->left_parent;
	}
	return *this;
}

bimap::left_iterator bimap::left_iterator::operator--(int)
{
	left_iterator it(*this);
	--(*this);
	return it;
}

bimap::right_iterator& bimap::right_iterator::operator++()
{
	if (pointer->right_right != nullptr)
	{
		pointer = pointer->right_right;
		while (pointer->right_left != nullptr)
			pointer = pointer->right_left;
	}
	else
	{
		while (pointer->right_parent != nullptr && pointer->right_parent->right_right == pointer)
			pointer = pointer->right_parent;
		pointer = pointer->right_parent;
	}
	return *this;
}

bimap::right_iterator bimap::right_iterator::operator++(int)
{
	right_iterator it(*this);
	++(*this);
	return it;
}

bimap::right_iterator& bimap::right_iterator::operator--()
{
	if (pointer == nullptr)
	{
		pointer = whose->root_right;
		while (pointer->right_right != nullptr)
			pointer = pointer->right_right;
	}
	else if (pointer->right_left != nullptr)
	{
		pointer = pointer->right_left;
		while (pointer->right_right != nullptr)
			pointer = pointer->right_right;
	}
	else
	{
		while (pointer->right_parent != nullptr && pointer->right_parent->right_left == pointer)
			pointer = pointer->right_parent;
		pointer = pointer->right_parent;
	}
	return *this;
}

bimap::right_iterator bimap::right_iterator::operator--(int)
{
	right_iterator it(*this);
	--(*this);
	return it;
}

bimap::left_iterator bimap::begin_left() const
{
	if (root_left == nullptr)
		return left_iterator(nullptr, this);
	node* now = root_left;
	while (now->left_left != nullptr)
		now = now->left_left;
	return left_iterator(now, this);
}

bimap::left_iterator bimap::end_left() const
{
	return left_iterator(nullptr, this);
}

bimap::right_iterator bimap::begin_right() const
{
	if (root_right == nullptr)
		return right_iterator(nullptr, this);
	node* now = root_right;
	while (now->right_left != nullptr)
		now = now->right_left;
	return right_iterator(now, this);
}

bimap::right_iterator bimap::end_right() const
{
	return right_iterator(nullptr, this);
}

bimap::node::node(left_t l_data, right_t r_data):
	left_data(l_data), 
	left_left(nullptr),
	left_right(nullptr),
	left_parent(nullptr),
	left_balance(random() * RAND_MAX + random()),
	
	right_data(r_data),
	right_left(nullptr),
	right_right(nullptr),
	right_parent(nullptr),
	right_balance(random() * RAND_MAX + random()) 
{}

bimap::node::node():
	left_left(nullptr),
	left_right(nullptr),
	left_parent(nullptr),
	left_balance(random() * RAND_MAX + random()),
	
	right_left(nullptr),
	right_right(nullptr),
	right_parent(nullptr),
	right_balance(random() * RAND_MAX + random()) 
{}

bimap::node::~node()
{
}


bimap::left_iterator bimap::insert(left_t const& left, right_t const& right)
{
	left_iterator l = find_left(left);
	right_iterator r = find_right(right);
	if (l.pointer != nullptr || r.pointer != nullptr)
		return left_iterator(nullptr, this);
	node* splitter = new node(left, right);
	std::pair <node*, node*> tmp = split(root_left, splitter, true);
	root_left = merge(merge(tmp.first, splitter, true), tmp.second, true);
	tmp = split(root_right, splitter, false);
	root_right = merge(merge(tmp.first, splitter, false), tmp.second, false);
	return left_iterator(splitter, this);
}

void bimap::erase(node* splitter)
{
	std::pair <node*, node*> tmp = split(root_left, splitter, true);
	node* now = tmp.second;
	while (now->left_left != nullptr)
		now = now->left_left;
	if (now->left_parent != nullptr)
	{
		now->left_parent->left_left = now->left_right;
		if (now->left_right != nullptr)
			now->left_right->left_parent = now->left_parent;
	}
	else 
	{
		tmp.second = now->left_right;
		if (now->left_right != nullptr)
			now->left_right->left_parent = nullptr;
	}
	root_left = merge(tmp.first, tmp.second, true);
	tmp = split(root_right, splitter, false);
	now = tmp.second;
	while (now->right_left != nullptr)
		now = now->right_left;
	if (now->right_parent != nullptr)
	{
		now->right_parent->right_left = now->right_right;
		if (now->right_right != nullptr)
			now->right_right->right_parent = now->right_parent;
	}
	else 
	{
		tmp.second = now->right_right;
		if (now->right_right != nullptr)
			now->right_right->right_parent = nullptr;
	}
	root_right = merge(tmp.first, tmp.second, false);
	delete now;
}

void bimap::erase(left_iterator it)
{
	erase(it.pointer);
}

void bimap::erase(right_iterator it)
{
	erase(it.pointer);
}

bimap::left_iterator bimap::find_left(left_t const& left) const
{
	node* now = root_left;
	while (now != nullptr && now->left_data != left)
	{
		if (now->left_data < left)
			now = now->left_right;
		else
			now = now->left_left;
	}
	return left_iterator(now, this);
}

bimap::right_iterator bimap::find_right(right_t const& right) const
{
	node* now = root_right;
	while (now != nullptr && now->right_data != right)
	{
		if (now->right_data < right)
			now = now->right_right;
		else
			now = now->right_left;
	}
	return right_iterator(now, this);
}

bimap::left_t const& bimap::left_iterator::operator*() const
{
	return pointer->left_data;
}

bimap::right_t const& bimap::right_iterator::operator*() const
{
	return pointer->right_data;
}

bool operator==(bimap::left_iterator l, bimap::left_iterator r)
{
	return (l.whose == r.whose && l.pointer == r.pointer);
}

bool operator!=(bimap::left_iterator l, bimap::left_iterator r)
{
	return (l.whose != r.whose || l.pointer != r.pointer);
}

bool operator==(bimap::right_iterator l, bimap::right_iterator r)
{
	return (l.whose == r.whose && l.pointer == r.pointer);
}

bool operator!=(bimap::right_iterator l, bimap::right_iterator r)
{
	return (l.whose != r.whose || l.pointer != r.pointer);
}

bimap::right_iterator bimap::left_iterator::flip() const
{
	return right_iterator(pointer, whose);
}

bimap::left_iterator bimap::right_iterator::flip() const
{
	return left_iterator(pointer, whose);
}

void bimap::left_iterator::operator=(left_iterator it)
{
	pointer = it.pointer;
	whose = it.whose;
}

void bimap::right_iterator::operator=(right_iterator it)
{
	pointer = it.pointer;
	whose = it.whose;
}

//int main()
//{
	////FILE* input = freopen("input", "r", stdin);
	////FILE* output = freopen("output", "w", stdout);
	
	//bimap a;
	///*bimap::left_iterator it = */a.insert("Nikita", 10);
	///*bimap::left_iterator it1 = */a.insert("Ivan", 9);
	///*bimap::left_iterator it2 = */a.insert("Mechrubon", 8);
	//bimap::left_iterator it = a.insert("Mechrubon", 11);
	//cout << it.pointer << endl;
	//bimap::right_iterator i = a.begin_right();
	//do
	//{
		////cout << i.pointer << endl;
		//cout << *i << " " << *(i.flip())<< endl;
		//i++;
	//} while (i != a.end_right());
	//a.erase(a.begin_left());
	//i = a.begin_right();
	//do
	//{
		////cout << i.pointer << endl;
		//cout << *i << " " << *(i.flip())<< endl;
		//i++;
	//} while (i != a.end_right());
	//a.erase(a.begin_right());
	//i = a.begin_right();
	//do
	//{
		////cout << i.pointer << endl;
		//cout << *i << " " << *(i.flip())<< endl;
		//i++;
	//} while (i != a.end_right());
	////cout << it.pointer << ' ' << it.pointer->left_left << ' ' << it.pointer->left_right << ' ' << it.pointer->left_parent << endl;
	////cout << it1.pointer << ' ' << it1.pointer->left_left << ' ' << it1.pointer->left_right << ' ' << it1.pointer->left_parent << endl;
	////cout << it2.pointer << ' ' << it2.pointer->left_left << ' ' << it2.pointer->left_right << ' ' << it2.pointer->left_parent << endl;
	//bimap::right_iterator p = a.find_right(10);
	//cout << *p << endl;
	//cout << *(p.flip()) << endl;
	////fclose(input);
	////fclose(output);
	
	//return 0;
//}
