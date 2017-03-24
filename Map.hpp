#ifndef MAP_HPP
#define MAP_HPP
#include <iostream>
#include <time.h>
#include <tuple>

namespace cs540
{
	
	#define MAXLEVEL 16
	
	template < typename Key_T, typename Mapped_T > 
	class SkipList;
	template < typename Key_T, typename Mapped_T > 
	class Map;
	
	template <typename Key_T, typename Mapped_T>
	class Node
	{
		public :
			std::pair<const Key_T, Mapped_T> m_pair;
			Node* next = nullptr;
			Node* prev = nullptr;
			Node* down = nullptr;
		public :
			//Node() = delete;
			Node(const Node& obj) : m_pair(obj.m_pair){};
			Node(std::pair<const Key_T, Mapped_T> pair) : m_pair(pair.first, pair.second), 
				next(nullptr), prev(nullptr), down(nullptr) {}
			Node(const Key_T& key, Mapped_T& val) : m_pair(std::make_pair(key, val)), next(nullptr), prev(nullptr), down(nullptr) {}
	};
	
/////////////////////////////////////////////// Skip List /////////////////////////////////////////////////////////////////////////////////////	
	template < class Key_T, class Mapped_T > 
	class SkipList
	{	
		friend Map<Key_T, Mapped_T>;
		
	private:
		Node < Key_T, Mapped_T > *head;
		Node < Key_T, Mapped_T > *tail;
		int m_iCurHight;
	private://member functions
		double uniformRandom() { return rand() / double(RAND_MAX); }
		int gen_random_level();
	public:
		SkipList (): m_iCurHight(1)
		{
			//head = new Node<Key_T, Mapped_T>(std::make_pair(key, value));
			//tail = new Node<Key_T, Mapped_T>(std::make_pair(key, value));
			head = (Node<Key_T, Mapped_T>*)malloc(sizeof(Node<Key_T, Mapped_T>));
			tail = (Node<Key_T, Mapped_T>*)malloc(sizeof(Node<Key_T, Mapped_T>));
			head->next = tail;
			tail->prev = head;
			head->down = nullptr;
			tail->down = nullptr;
			
		}
		SkipList(const SkipList& obj) 
		{
			Node<Key_T, Mapped_T>* tempHead = obj.head;
			while(tempHead->down != nullptr)
				tempHead = tempHead->down;
			Node<Key_T, Mapped_T>* tempNode = tempHead->next;
			while(tempNode != obj.tail)
			{
				this->insert(tempNode->m_pair.first, tempNode->m_pair.second);
				tempNode = tempNode->next;
			}
		}
		SkipList& operator=(const SkipList& obj)
		{
			Node<Key_T, Mapped_T>* tempHead = obj.head;
			while(tempHead->down != nullptr)
				tempHead = tempHead->down;
			Node<Key_T, Mapped_T>* tempNode = tempHead->next;
			while(tempNode != obj.tail)
			{
				this->insert(tempNode->m_pair.first, tempNode->m_pair.second);
				tempNode = tempNode->next;
			}
			return *this;
		}
		~SkipList()
		{
			delete_all_el();
		}
		Node<Key_T, Mapped_T>* get_head(void) const
		{
			Node<Key_T, Mapped_T>* tempHead = head;
		  	while(tempHead->down != nullptr)
		  		tempHead = tempHead->down;
		  	return tempHead;
		}
		Node<Key_T, Mapped_T>* first(void) const
		{
			if(head != nullptr)
			{
				Node<Key_T, Mapped_T>* tempHead = head;
			
				while(tempHead->down != nullptr)
			  	{
			  		tempHead = tempHead->down;
			  	}
			  	
			  	return tempHead->next;
		  	}
		  	return nullptr;
		}
		Node<Key_T, Mapped_T>* last(void) const
		{
			return tail->prev;
		}
		Node<Key_T, Mapped_T>* get_tail(void) const { return tail; }
		void set_head(Node<Key_T, Mapped_T>* head){ this->head = head; }
		void set_tail(Node<Key_T, Mapped_T>* tail){ this->tail = tail; }
		Node<Key_T, Mapped_T>* get (Key_T searchKey) const;
		Node<Key_T, Mapped_T>* insert(Key_T, Mapped_T);
		void delete_all_el(void);
		void delete_single_el(Key_T);
		void display(void);
	};//end class skiplist
	template<typename Key_T, typename Mapped_T>
	int SkipList<Key_T, Mapped_T>::gen_random_level(void)
	{
		int level = 1;
		double p = 0.5;
	
		if(m_iCurHight < MAXLEVEL)
		{
			while ( uniformRandom() < p && level < MAXLEVEL ) 
			{
			    level++;
			}
		}
		else
		{	
			while ( uniformRandom() < p && level < m_iCurHight ) 
			{

			}			    level++;
		}
		return level;
	}
	template<typename Key_T, typename Mapped_T>
	Node<Key_T, Mapped_T>* SkipList<Key_T, Mapped_T>::insert (const Key_T key, Mapped_T value)
	{
		Node < Key_T, Mapped_T >* newNode;
		/*if(head == nullptr && tail == nullptr)
		{
			//head = new Node<Key_T, Mapped_T>(std::make_pair(key, value));
			//tail = new Node<Key_T, Mapped_T>(std::make_pair(key, value));
			//head->next = tail;
			//tail->prev = head;
			//head->down = nullptr;
			//tail->down = nullptr;
		}*/
		if (head->next == tail)
	      	{
	      		//////std::cout << key << " " << value << std::endl;
			newNode = new Node < Key_T, Mapped_T > (std::make_pair(key, value));
			newNode->next = tail;
			head->next = newNode;
			tail->prev = newNode;
			newNode->prev = head;
		}
	    	else
	      	{
	      		Node< Key_T, Mapped_T>  *tempNode = head;
			int randLevel = gen_random_level();
		       	while((randLevel - m_iCurHight) > 1)
				randLevel = gen_random_level();
			if(randLevel > m_iCurHight)
			{
				Node < Key_T, Mapped_T >* newHead = new Node < Key_T, Mapped_T >(std::make_pair(key, value));
				newHead->down = head;
				newHead->next = tail;
				head = newHead;
				m_iCurHight++;
			}
			Node < Key_T, Mapped_T >* tempHead = head;
			Node < Key_T, Mapped_T >* tempNode1 = nullptr;
			for(int i = m_iCurHight; i > randLevel; --i)
				tempHead = tempHead->down;
			while(tempHead != nullptr)
			{
				tempNode = tempHead;
				while(tempNode->next != tail)
				{
					if(tempNode->next->m_pair.first < key && tempNode->next != tail)
						tempNode = tempNode->next;
					else
						break;
				}
				newNode = new Node < Key_T, Mapped_T >(std::make_pair(key, value));
				newNode->next = tempNode->next;
				tempNode->next = newNode;
				if(newNode->next != tail)
					newNode->next->prev = newNode;
				newNode->prev = tempNode;
				if(tempNode1 == nullptr)
					tempNode1 = newNode;
				else
				{
					tempNode1->down = newNode;
					tempNode1 = newNode;
				}
				tempHead = tempHead->down;
			}
			if(newNode->down == nullptr && newNode->next == tail) 
				tail->prev = newNode;
			tempNode1 = nullptr;
		}
		return newNode;
  	}//end insert
	template<typename Key_T, typename Mapped_T>
	void SkipList<Key_T, Mapped_T>::delete_single_el(const Key_T key)
	{
		Node < Key_T, Mapped_T >* tempHead = head;
	  	while(tempHead != nullptr)
	  	{
	  		Node < Key_T, Mapped_T >* tempNode = tempHead->next;
	  		while(tempNode != tail && !(tempNode->m_pair.first == key))
	  		{
	  			tempNode = tempNode->next;
	  			////std::cout << "1"<<std::endl;
	  		}
	  		////std::cout << "here " << std::endl;
	  		if(tempNode != tail)
	  		{
	  			
		  		if(tempNode->m_pair.first == key)
		  		{
		  			tempNode->prev->next = tempNode->next;
			  		tempNode->next->prev = tempNode->prev;
			  		/*if(head->next == tail)
			  		{
			  			head = head->down;
			  			if(tail->prev == head)
			  				delete tail;
			  			delete head;
			  		}*/
			  		if(tempNode->down != nullptr)
			  			tempHead = tempNode->down->prev;
			  		else 
			  			tempHead = nullptr;
			  		delete tempNode;
			  		continue;
		  		}
		  	}
		  	tempHead = tempHead->down;
	  	}
	  	
  	}
  	template<typename Key_T, typename Mapped_T>
	void SkipList<Key_T, Mapped_T>::delete_all_el()
  	{
	  	while(head != nullptr)
	  	{
	  		Node < Key_T, Mapped_T >* tempHead = head;
	  		head = head->down;
	  		while(tempHead != tail)
	  		{
	  			Node < Key_T, Mapped_T >* tempNode = tempHead->next;
	  			if(tempHead->down == nullptr)
	  				free (tempHead);
	  			else
	  				delete tempHead;
	  			tempHead = nullptr;
	  			tempHead = tempNode;
	  		}
	  	}
	  	free(tail);
  	}
	template<typename Key_T, typename Mapped_T>
	Node<Key_T, Mapped_T>* SkipList<Key_T, Mapped_T>::get (const Key_T searchKey) const
	{
		Node<Key_T, Mapped_T>* tempHead = head;
		while(tempHead != nullptr)
		{
			Node<Key_T, Mapped_T>* tempNode = tempHead->next;
			while(tempNode != tail)
			{
				if(tempNode->m_pair.first == searchKey)
				{
					while(tempNode->down != nullptr)
					{	
						//to get the key & value pair go down to main list
						tempNode = tempNode->down;
					}
					return tempNode;
				}
				tempNode = tempNode->next;
			}
			tempHead = tempHead->down;
		}
	    	return nullptr;
	}
	template<typename Key_T, typename Mapped_T>
	void SkipList<Key_T, Mapped_T>::display()
  	{
  		//cout << "display " << endl;
  		Node < Key_T, Mapped_T >* tempHead = head;
	  	while(tempHead != nullptr)
	  	{
	  		Node < Key_T, Mapped_T >* tempNode = tempHead->next;
	  		while(tempNode != tail)
	  		{
	  			//std::cout << tempNode->m_pair.first <<" ";
	  			tempNode = tempNode->next; 
	  		}
	  		//std::cout<<std::endl;
	  		tempHead = tempHead->down;
	  	}
  	}
///////////////////////////////////////////////////// End SkipList ////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////  Start Map //////////////////////////////////////////////////////////////////////////////////

	template <typename Key_T, typename Mapped_T>
	class Map
	{
		typedef std::pair<const Key_T, Mapped_T> ValueType;
		public:
		SkipList<Key_T, Mapped_T> list;
		size_t map_size;
		public:
			Map() : map_size(0) {}
			Map(const Map& obj) : map_size(obj.size())  {list = obj.list;}
			Map& operator=(const Map<Key_T, Mapped_T>& obj) 
			{
				this->list = obj.list;
				return *this;
			}
			Map(std::initializer_list<std::pair<const Key_T, Mapped_T> > list) : map_size(0)
			{
				for(auto pair : list)
				{
					//auto p(pair);
					map_size++;
					insert(pair);
				}
			}
			~Map(){}
			size_t size(void) const {return this->map_size;}
			bool empty(void) const {return this->map_size == 0;}
			void clear(void) {this->list.delete_all_el();}
			void display(void){list.display();}
			std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool> insert(const ValueType &pair);
			typename Map<Key_T, Mapped_T>::Iterator begin(void) ;
			typename Map<Key_T, Mapped_T>::Iterator end(void) ;
			typename Map<Key_T, Mapped_T>::ConstIterator begin(void) const;
			typename Map<Key_T, Mapped_T>::ConstIterator end(void) const ;
			typename Map<Key_T, Mapped_T>::ReverseIterator rbegin(void);
			typename Map<Key_T, Mapped_T>::ReverseIterator rend(void);
			void erase(const Key_T key);
		
			void erase(typename Map<Key_T, Mapped_T>::Iterator it);
			typename Map<Key_T, Mapped_T>::Iterator find(const Key_T& key);
			typename Map<Key_T, Mapped_T>::ConstIterator find(const Key_T& key) const;
			Mapped_T& at(const Key_T& key);
			const Mapped_T& at(const Key_T& key) const;
			Mapped_T& operator[](const Key_T& key);
		
		/////////////////////////////////// friends ///////////////////////////////////////////////////////////////////////////////////////
		
			friend bool operator==(const Map<Key_T, Mapped_T>& obj1, const Map<Key_T, Mapped_T>& obj2)
			{
				auto it1 = obj1.begin();
				auto it2 = obj2.begin();
				if(obj1.size() == obj2.size())
				{
					while(it1.curNode != obj1.list.get_tail())
					{
						if(it1.curNode->m_pair.first == it2.curNode->m_pair.first
						 && it1.curNode->m_pair.second == it2.curNode->m_pair.second)
						{
							it1++;
							it2++;
						}
						else
						{
							return false;
						}
					}
					return true;
				}
				return false;
			}

			friend bool operator!=(const Map<Key_T, Mapped_T>& obj1, const Map<Key_T, Mapped_T>& obj2)
			{
				if(!(obj1 == obj2))
					return true;
				return false;
			}
			friend bool operator<(const Map<Key_T, Mapped_T>& obj1, const Map<Key_T, Mapped_T>& obj2)
			{
				if(obj1.size() < obj2.size())
					return true;
				else
				{
					typename Map<Key_T, Mapped_T>::Iterator it1 = obj1.begin();
					typename Map<Key_T, Mapped_T>::Iterator it2 = obj2.begin();
					while(it1.curNode != obj1.list.get_tail())
					{
						if(it1.curNode->m_pair < it2.curNode->m_pair)
						{
							return true;
						}
						else
						{
							it1++;
							it2++;
						}
					}
					return false;
				}
				return false;
			}
	//////////////////////////////////// Iterator /////////////////////////////////////////////////////////////////////////////////////
			class Iterator
			{

			typedef std::pair<const Key_T, Mapped_T> ValueType;

			public:
				Node<Key_T, Mapped_T>* curNode;
			public:
				//Iterator(const Iterator&);
				virtual ~Iterator(){}
				Iterator& operator=(const Iterator& obj){this->curNode = obj.curNode;}
				Iterator& operator++(void){this->curNode = this->curNode->next; return *this;}
				Iterator operator++(int)
				{
					
					Iterator it; 
					if(this->curNode->next == nullptr)
						it.curNode = nullptr;
					else
					{
						it.curNode = this->curNode; 
						this->curNode = this->curNode->next; 
					}
					return it;
				}
				Iterator& operator--(void)
				{
					this->curNode = this->curNode->prev; 
					return *this;
				}
				Iterator operator--(int)
				{
					Iterator it; 
					it.curNode = this->curNode;
					this->curNode = this->curNode->prev; 
					return it;
				}
				ValueType& operator*() const{return this->curNode->m_pair;}
				ValueType* operator->() const{return &(this->curNode->m_pair);}
	
				//friend
				friend Map<Key_T, Mapped_T>;
	
			};//end of iterator
			//reverse iterator
			class ReverseIterator
			{

			typedef std::pair<const Key_T, Mapped_T> ValueType;

			public:
				Node<Key_T, Mapped_T>* curNode;
			public:
				//Iterator(const Iterator&);
				~ReverseIterator(){}
				ReverseIterator& operator=(const ReverseIterator& obj){this->curNode = obj.curNode;}
				ReverseIterator& operator++(void){this->curNode = this->curNode->prev; return *this;}
				ReverseIterator operator++(int)
				{
					ReverseIterator it; 
					it.curNode = this->curNode; 
					this->curNode = this->curNode->prev; 
					return it;
				}
				ReverseIterator& operator--(void)
				{
					this->curNode = this->curNode->next; 
					return *this;
				}
				ReverseIterator operator--(int)
				{
					ReverseIterator it; 
					it.curNode = this->curNode; 
					this->curNode = this->curNode->next; 
					return it;
				}
				ValueType& operator*() const{return this->curNode->m_pair;}
				ValueType* operator->() const{return &(this->curNode->m_pair);}
			
			
				//friend
				friend Map<Key_T, Mapped_T>;
		};//end of reverse iterator
	
		//const iterator
		class ConstIterator : public Iterator
		{

		typedef std::pair<const Key_T, Mapped_T> ValueType;

		public:
			//Iterator(const Iterator&);
			~ConstIterator(){}
			//ConstIterator& operator=(const ConstIterator& obj){this->curNode = obj.curNode;}
			//ConstIterator& operator++(void){this.curNode = this->curNode->next; return *this;}
			//ConstIterator operator++(int)
			//{
			//	ConstIterator it; 
			//	it.curNode = this->curNode; 
			//	this->curNode = this->curNode->next; 
			//	return it;
			//}
			//ConstIterator& operator--(void)
			//{
			//	this.curNode = this.curNode->prev; 
			//	return *this;
			//}
			//ConstIterator operator--(int){ConstIterator it; it.curNode = this->curNode; this->curNode = this->curNode->prev; return it;}
			const ValueType& operator*() const{return this->curNode->m_pair;}
			const ValueType* operator->() const{return &(this->curNode->m_pair);}
			
			//friend
			friend Map<Key_T, Mapped_T>;
		};//end of const iterator
		
		friend bool operator==(const Iterator& it1, const Iterator& it2){return it1.curNode == it2.curNode;}
		friend bool operator==(const ConstIterator& it1, const ConstIterator& it2){return it1.curNode == it2.curNode;}
		friend bool operator==(const Iterator& it1, const ConstIterator& it2){return it1.curNode == it2.curNode;}
		friend bool operator==(const ConstIterator& it1, const Iterator& it2){return it1.curNode == it2.curNode;}
		friend bool operator!=(const Iterator& it1, const Iterator& it2){return it1.curNode != it2.curNode;}
		friend bool operator!=(const ConstIterator& it1, const ConstIterator& it2){return it1.curNode != it2.curNode;}
		friend bool operator!=(const Iterator& it1, const ConstIterator& it2){return it1.curNode != it2.curNode;}
		friend bool operator!=(const ConstIterator& it1, const Iterator& it2){return it1.curNode != it2.curNode;}
		friend bool operator==(const ReverseIterator& rit1, const ReverseIterator& rit2){return rit1.curNode == rit2.curNode;}
		friend bool operator!=(const ReverseIterator& rit1, const ReverseIterator& rit2){return rit1.curNode != rit2.curNode;}
	};
	
	template<class Key_T, class Mapped_T>
	std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool> Map<Key_T, Mapped_T>::insert(const ValueType &pair)
	{
		Node<Key_T, Mapped_T>* node = list.get(pair.first);
		std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool> retVal;
		Iterator it;
		if(node != nullptr)
		{
			it.curNode = node;
			retVal.first = it;
			retVal.second = false;
			return retVal;
		}
		else
		{
			node = list.insert(pair.first, pair.second);
			this->map_size++;
			//cout << "size " << map_size <<endl;
			it.curNode = node;
			retVal.first = it;
			retVal.second = true;
			return retVal;
		}
	}
	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::begin(void) 
	{
		Map<Key_T, Mapped_T>::Iterator it;
		it.curNode = this->list.first(); 
		return it;
	}
	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::end(void) 
	{
		Map<Key_T, Mapped_T>::Iterator it;
		it.curNode = this->list.get_tail(); 
		return it;
	}
	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::begin(void) const
	{
		Map<Key_T, Mapped_T>::ConstIterator cit;
		cit.curNode = this->list.first(); 
		return cit;
	}
	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::end(void) const 
	{
		Map<Key_T, Mapped_T>::ConstIterator cit;
		cit.curNode = this->list.get_tail();
		return cit;
	}
	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rbegin(void)
	{
		Map<Key_T, Mapped_T>::ReverseIterator rit;
		rit.curNode = this->list.last();
		return rit;
	}
	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rend()
	{
		Map<Key_T, Mapped_T>::ReverseIterator rit;
		rit.curNode = this->list.get_head()->prev; 
		return rit;
	}
	template<class Key_T, class Mapped_T>
	void Map<Key_T, Mapped_T>::erase(const Key_T key)
	{
		this->list.delete_single_el(key); 
		this->map_size--;
	}
	template<class Key_T, class Mapped_T >
	void Map<Key_T, Mapped_T>::erase(Map<Key_T, Mapped_T>::Iterator it)
	{
		this->list.delete_single_el(it.curNode->m_pair.first); 
		this->map_size--;
	}
	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::find(const Key_T& key)
	{
		Map<Key_T, Mapped_T>::Iterator it;
		it.curNode = this->list.get(key);
		if(it.curNode != nullptr)
		{
			return it;
		}
		else
			return this->end();
	}
	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::find(const Key_T& key) const
	{
		Map<Key_T, Mapped_T>::ConstIterator cit;
		cit.curNode = this->list.get(key);
		if(cit.curNode != nullptr)
			return cit;
		else
			return this->end();
	}
	template<class Key_T, class Mapped_T>
	Mapped_T& Map<Key_T, Mapped_T>::at(const Key_T& key)
	{
		
		Node<Key_T, Mapped_T>* node = list.get(key);
		if(node == nullptr)
			throw std::out_of_range("Key not Found");
		else
		{
			return node->m_pair.second;
		}
	}
	template<class Key_T, class Mapped_T>
	const Mapped_T& Map<Key_T, Mapped_T>::at(const Key_T& key) const
	{
		Node<Key_T, Mapped_T>* node = list.get(key);
		if(node == nullptr)
			throw std::out_of_range("Key not Found");
		else
		{
			return node->m_pair.second;
		}
	}
	template<class Key_T, class Mapped_T>
	Mapped_T& Map<Key_T, Mapped_T>::operator[](const Key_T& key)
	{
		Node<Key_T, Mapped_T>* node = list.get(key);
		
		auto it = this->begin();
		//cout << "aaaaa"<<endl;
		std::pair<Iterator, bool> retVal;
		
		if(node == nullptr)
		{
			auto p = std::make_pair(key, NULL);
			retVal = this->insert(p);
			auto it1 = retVal.first;
			return (*it1).second;
		}
		else
		{
			it.curNode = node;
			return (*it).second;
		}
	}
}


#endif
