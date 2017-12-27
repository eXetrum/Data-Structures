#ifndef __ARRAY__LIST__H__
#define __ARRAY__LIST__H__


template <class Type>
class ArrayList {
private:
    typedef int (*Comparator)(const Type&, const Type&);
    static int defaultComparator(const Type& lhs, const Type& rhs) {
        if (lhs == rhs) return 0;
        return lhs < rhs ? -1 : 1;
    }

    template <class T>
    void swap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }
    void swap(ArrayList& rhs) {
        swap(this->_size, rhs._size);
        swap(this->_capacity, rhs._capacity);
        swap(this->data, rhs.data);
    }
    
    int partition(Type* A, int lo, int hi, Comparator cmp) {
        Type pivot = A[hi];
        int i = lo - 1;
        for (int j = lo; j <= hi - 1; ++j) {
            if (cmp(A[j], pivot) <= 0) {
                ++i;
                if (i != j)
                    swap(A[i], A[j]);
            }
        }
        if(i + 1 != hi)
            swap(A[i + 1], A[hi]);
        return i + 1;
    }

    void quicksort(Type* A, int lo, int hi, Comparator cmp) {
        if (lo < hi) {
            int p = partition(A, lo, hi, cmp);
            quicksort(A, lo, p - 1, cmp);
            quicksort(A, p, hi, cmp);
        }
    }
public:
    ArrayList() : _size(0), _capacity(1), data(new Type[_capacity]) { }
    ArrayList(unsigned int n, Type zerofill=Type()) : _size(n), _capacity(n), data(new Type[_capacity]) {
        for (unsigned int i = 0; i < n; ++i) data[i] = zerofill;
    }

    // Big three
    ~ArrayList() { if (data != 0) delete[]data; }

    ArrayList(const ArrayList& arr) : _size(arr._size), _capacity(arr._capacity), data(new Type[_capacity]) {
        for (unsigned int i = 0; i < _size; ++i) data[i] = arr.data[i]; // 'Type' should support assignment operator
    }

    ArrayList& operator=(const ArrayList& rhs) {
        if (this != &rhs) {
            ArrayList temp(rhs);
            this->swap(temp);
        }
        return *this;
    }

    unsigned int size() const { return _size; }
    unsigned int capacity() const { return _capacity; }
    bool empty() const { return _size == 0; }
    const Type& operator[](unsigned index) const { return data[index]; }
    Type& operator[](unsigned index) { return data[index]; }

    // inserts element into array and returns True on success (or False on failure)
    bool insert(const Type& element, int index) {
        
        // perform the required safety checks before insertion
        if (index < 0 || index > _size) return false;   // invalid indices

        if (_size == _capacity) { // if array is full
            _capacity *= 2;
            Type* temp = new Type[_capacity];
            // copy all elements of old array to new array
            for (unsigned int i = 0; i < _size; ++i) temp[i] = data[i];
            // replace old array with new array
            delete[] data;
            data = temp;
        }


        
        // make space for the new element (if insertion not at the end)
        for (int i = _size - 1; i >= index; --i)  data[i + 1] = data[i];

        // perform insertion
        data[index] = element;
        // increment number of elements
        ++_size;

        return true;
    }

    bool add(const Type& element) {
        return insert(element, _size);
    }

    void sort(Comparator cmp=defaultComparator) {
        quicksort(data, 0, _size - 1, cmp);
    }

    // removes element at position "index" in the array
    bool remove(int index) {
        // check for valid index
        if (index < 0 || index >= _size)  // invalid indices
            return false;

        // perform the removal algorithm
        Type* temp = &data[index];
        temp->~Type();

        // if we didn't remove from the very end of the list
        if (index < _size - 1) {
            // shift all elements left one slot
            for (int i = index; i < _size - 2; ++i) {
                data[i] = data[i + 1];
            }
            // not technically necessary since it'll be overwritten next insert
            temp = &data[_size - 1];
            temp->~Type();
            data[_size - 1] = Type();
        }

        // decrement number of elements
        --_size;

        return true;
    }

    bool contains(const Type& element) {
        for (unsigned int i = 0; i < _size; ++i) if (data[i] == element) return true;
        return false;
    }
protected:
    unsigned int _size;
    unsigned int _capacity;
    Type* data;
};
#endif
