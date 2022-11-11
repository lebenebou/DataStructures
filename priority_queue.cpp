
#include <iostream>
#include <vector>
#include <functional>
#include <cassert>
using namespace std;

template<typename Type>
class PriorityQueue{

private:
    vector<Type> heap;
    size_t size;
    function<bool(const Type& e1, const Type& e2)> comparator;

    int left(const int& elt) const {

        int answer = (elt+1)*2 - 1;
        if(answer>=size || elt>=size) return -1;
        return answer;
    }

    int right(const int& elt) const {
        
        int answer = (elt+1)*2;
        if(answer>=size || elt>=size) return -1;
        return answer;
    }

    int parent(const int& elt) const {

        if(elt<=0 || elt>=size) return -1;
        return (elt-1)/2;
    }

    int last_elt() const {
        return size-1;
    }

    int better_elt(const int& e1, const int& e2) const {
        
        if(e1>=size && e2>=size) return -1;
        if(e1 >= size) return e2;
        if(e2 >= size) return e1;

        if(comparator(heap[e1], heap[e2])) return e1;
        return e2;
    }

    void bubble_up(int i){

        if(parent(i) == -1) return;

        while(comparator(heap[i], heap[parent(i)])){

            swap(heap[i], heap[parent(i)]);
            i = parent(i);

            if(i==-1) return;
        }
    }

    void bubble_down(int i){

        int m = better_elt(left(i), right(i));
        if(m == -1) return; // i has no children

        while(comparator(heap[m], heap[i])){
            
            swap(heap[m], heap[i]);
            i = m;

            m = better_elt(left(i), right(i));
            if(m == -1) return; // i has no children
        }
    }

public:
    PriorityQueue(function<bool(const Type& e1, const Type& e2)> comp = [](const Type& e1, const Type& e2){ return e1 > e2; }) : heap({}), size(0), comparator(comp) {}

    void push(const Type& elt){

        heap.push_back(elt);
        assert(++size == heap.size() && "heap size not equal to queue size");
        bubble_up(last_elt()); // size-1 which was just inserted
    }

    void pop(){

        assert(size>0 && "cannot pop from empty queue");

        swap(heap[0], heap[last_elt()]);
        heap.pop_back();
        assert(--size == heap.size() && "heap size is not equal to queue size");
        bubble_down(0);
    }
    
    Type top() const {
        return heap[0];
    }

    void display() const {

        cout << "[ ";
        for(const Type& elt : heap) cout << elt << " ";
        cout << "]" << endl;
    }

    size_t count() const {
        return size;
    }

    bool is_empty() const {
        return size==0;
    }
};

int main(){

    PriorityQueue<int> q;

    q.push(1);
    q.push(2);
    q.push(10);
    q.push(11);
    q.display();

    while(!q.is_empty()){

        cout << q.top() << endl;
        q.pop();
    }


    return 0;
}