#include <algorithm>
#include <cassert>

#include <iostream>
#include <set>



namespace util{

enum class NoneType {
    None
};
const NoneType None = None;

template <typename Object, size_t N>
class small_vector {
public:
    
    explicit small_vector(size_t initSize = 0)
    : theSize(initSize), theCapacity(N) {
        assert(initSize <= N);
    }
    
    small_vector(const small_vector & rhs) : objects(NULL) {
        operator=(rhs);
    }
    
    ~small_vector() {
        
    }
    
    bool empty() const {
        return size() == 0;
    }
    
    int size() const {
        return theSize;
    }
    
    int capacity() const {
        return theCapacity;
    }
    
    Object & operator[](int index) {
#ifndef NO_CHECK
        assert(index < theSize);
#endif
        return objects[ index ];
    }
    
    const Object & operator[](int index) const {
#ifndef NO_CHECK
        assert(index < theSize);
#endif
        return objects[ index ];
    }
    
    const small_vector & operator=(const small_vector & rhs) {
        if (this != &rhs) {
            std::memset(&objects, 0, N);
            theSize = rhs.size();
            theCapacity = rhs.theCapacity;
            
            for (int k = 0; k < size(); k++)
                objects[ k ] = rhs.objects[ k ];
        }
        return *this;
    }
    
    
    
    // Stacky stuff
    
    void push_back(const Object & x) {
        assert(theSize < (N));
        objects[ theSize++ ] = x;
    }
    
    void pop_back() {
        assert(theSize > 0);
        theSize--;
    }
    
    const Object & back() const {
        //        if (empty())
        //            throw UnderflowException();
        return objects[ theSize - 1 ];
    }
    
    // Iterator stuff: not bounds checked
    typedef Object * iterator;
    typedef const Object * const_iterator;
    
    iterator begin() {
        return &objects[ 0 ];
    }
    
    const_iterator begin() const {
        return &objects[ 0 ];
    }
    
    iterator end() {
        return &objects[ size() ];
    }
    
    const_iterator end() const {
        return &objects[ size() ];
    }
    
    void clear() {
        std::memset(&objects, 0, N);
        this->theSize = 0;
    }
    
private:
    int theSize;
    int theCapacity;
    Object objects[N];
};



template <typename T, unsigned N = 5, typename C = std::less<T> >
class small_set {
    
    small_vector<T, N> data;
    
public:
    
    typedef typename small_vector<T, N>::const_iterator const_iterator;
    typedef typename small_vector<T, N>::iterator iterator;
    
    typedef size_t size_type;
    
    small_set() {
    }
    
    bool empty() const {
        return data.empty();
    }
    
    const_iterator begin() const {
        return data.begin();
    }
    
    iterator begin() {
        return data.begin();
    }
    
    const_iterator end() const {
        return data.end();
    }
    
    iterator end() {
        return data.end();
    }
    
    size_type size() const {
        
        return data.size();
    }
    
    size_type contains(const T &V) const {
        
        return vfind(V) == data.end() ? 0 : 1;
    }
    
    std::pair<NoneType, bool> insert(const T &V) {
        
        const_iterator I = vfind(V);
        if (I != data.end()) // Don't reinsert if it already exists.
            return std::make_pair(None, false);
        if (data.size() < N) {
            data.push_back(V);
            return std::make_pair(None, true);
        }
        
        
        
        return std::make_pair(None, true);
    }
    
    template <typename IterT>
    void insert(IterT I, IterT E) {
        
        for (; I != E; ++I)
            insert(*I);
    }
    
    bool erase(const T &V) {
        
        for (iterator I = data.begin(), E = data.end(); I != E; ++I)
            if (*I == V) {
                data.erase(I);
                
                return true;
            }
        return false;
    }
    
    void clear() {
        
        data.clear();
    }
private:
    
    const_iterator vfind(const T &V) const {
        for (const_iterator I = data.begin(), E = data.end(); I != E; ++I)
            if (*I == V)
                return I;
        
        return data.end();
    }
};
}