#ifndef BIMAP_H
#define BIMAP_H

//using namespace std;

struct bimap
{
    // Вы можете определить эти тайпдефы по вашему усмотрению.
    typedef std::string left_t;
    typedef std::string right_t;

    struct left_iterator;
    struct right_iterator;

    // Создает bimap не содержащий ни одной пары.
    bimap();

    // Деструктор. Вызывается при удалении объектов bimap.
    // Инвалидирует все итераторы ссылающиеся на элементы этого bimap
    // (включая итераторы ссылающиеся на элементы следующие за последними).
    ~bimap();

    // Вставка пары (left, right), возвращает итератор на left.
    // Если такой left или такой right уже присутствуют в bimap, вставка не
    // производится и возвращается end_left().
    left_iterator insert(left_t const& left, right_t const& right);

    // Удаляет элемент и соответствующий ему парный.
    // erase невалидного итератора неопределен.
    // erase(end_left()) и erase(end_right()) неопределены.
    // Пусть it ссылается на некоторый элемент e.
    // erase инвалидирует все итераторы ссылающиеся на e и на элемент парный к e.
    void erase(left_iterator it);
    void erase(right_iterator it);

    // Возвращает итератор по элементу. В случае если элемент не найден, возвращает
    // end_left()/end_right() соответственно.
    left_iterator  find_left (left_t  const& left)  const;
    right_iterator find_right(right_t const& right) const;

    // Возващает итератор на минимальный по величине left
    left_iterator begin_left() const;
    // Возващает итератор на следующий за последним по величине left
    left_iterator end_left() const;

    // Возващает итератор на минимальный по величине right
    right_iterator begin_right() const;
    // Возващает итератор на следующий за последним по величине right
    right_iterator end_right() const;
    
    private:
		struct node 
		{
			left_t left_data;
			node* left_left;
			node* left_right;
			node* left_parent;
			uint left_balance;
			
			right_t right_data;
			node* right_left;
			node* right_right;
			node* right_parent;
			uint right_balance;
			
			node();
			node(left_t l_data, right_t r_data);
			~node();
		};
		
		void erase(node* splitter);
		void delete_all_nodes(node*);
		
		node* root_left;
		node* root_right;
		node* merge(node* l, node* r, bool part);
		std::pair <node*, node*> split(node* root, node* splitter, bool part);
};

struct bimap::left_iterator
{
    // Элемент на который сейчас ссылается итератор.
    // Разыменование итератора end_left() неопределено.
    // Разыменование невалидного итератора неопределено.
    left_t const& operator*() const;

    // Переход к следующему по величине left'у.
    // Инкремент итератора end_left() неопределен.
    // Инкремент невалидного итератора неопределен.
    left_iterator& operator++();
    left_iterator operator++(int);

    // Переход к предыдущему по величине left'у.
    // Декремент итератора begin_left() неопределен.
    // Декремент невалидного итератора неопределен.
    left_iterator& operator--();
    left_iterator operator--(int);
    
    void operator=(left_iterator it);
    
    friend bool operator ==(left_iterator, left_iterator);
    friend bool operator !=(left_iterator, left_iterator);
    
    // left_iterator ссылается на левый элемент некоторой пары.
    // Эта функция возвращает итератор на правый элемент той же пары.
    // end_left().flip() возращает end_right().
    // end_right().flip() возвращает end_left().
    // flip() невалидного итератора неопределен.
    right_iterator flip() const;
    
    private:
		friend bimap;
    
		node* pointer;
		const bimap* whose;
		
		left_iterator(node* point, const bimap* that);
		//left_iterator(left_iterator const& another);
};

struct bimap::right_iterator
{
    // Здесь всё аналогично left_iterator.
    right_t const& operator*() const;

    right_iterator& operator++();
    right_iterator operator++(int);

    right_iterator& operator--();
    right_iterator operator--(int);
    
    void operator=(right_iterator it);
    
    left_iterator flip() const;
    
    friend bool operator ==(right_iterator, right_iterator);
    friend bool operator !=(right_iterator, right_iterator);
    
    private:
		friend bimap;
    
		node* pointer;
		const bimap* whose;
		
		right_iterator(node* point, const bimap* that);
		//right_iterator(right_iterator const& another);
		
};

#endif
