
#include <iostream>
#include <vector>
using namespace std;

template<typename Type>
class DLL{

private:
    struct Node{

        Type value;
        Node* previous;
        Node* next;

        Node(const Type& v, Node* p = nullptr, Node* n = nullptr) : value(v), previous(p), next(n){

            if(p) p->next = this;
            if(n) n->previous = this;
        }
    };

    Node* head;
    Node* tail;
    size_t size;

    Node* node_at_index(size_t index) const {

        if(index>=size) return nullptr;
        Node* jumper = nullptr;

        if(index < size/2){

            jumper = head;
            while(--index + 1) jumper = jumper->next;
        }
        else { // index >= size/2

            index = size - index;
            jumper = tail;
            while(--index) jumper = jumper->previous;
        }
        return jumper;
    }

    void sort_swap(Node* start){

        if (!start || !start->next) return;
        if(start->value > start->next->value) std::swap(start->value, start->next->value);
    }

    Node* node_between(Node* start, Node* end) const {

        while(true){

            if(start == end) return start;
            if(start->next == end) return start;

            start = start->next;
            end = end->previous;
        }
    }

    void merge(Node* start, Node* mid, Node* end){

        if(start == mid) return sort_swap(start);

        vector<Type> result;

        Node* i = start;
        Node* j = mid->next;

        while(i != mid->next || j!=end->next){

            if(i!=mid->next && j!=end->next){ // both are valid

                if(i->value < j->value){
                    result.push_back(i->value);
                    i = i->next;
                }
                else {
                    result.push_back(j->value);
                    j = j->next;
                }
            }

            else if(i!=mid->next && j==end->next){ // only i is valid

                result.push_back(i->value);
                i = i->next;
            }
            else{

                result.push_back(j->value);
                j = j->next;
            }
        }

        for(const Type& elt : result){
            start->value = elt;
            start = start->next;
        }
    }

    void merge_sort(Node* start, Node* end){

        Node* mid = node_between(start, end);
        if(start == mid) return sort_swap(start);

        merge_sort(start, mid);
        merge_sort(mid->next, end);
        merge(start, mid, end);
    }

    template<typename T>
    friend void swap(DLL<T>& l1, DLL<T>& l2);

public:
    DLL() : head(nullptr), tail(nullptr), size(0) {}

    DLL(const vector<Type>& v) : head(nullptr), tail(nullptr), size(0) {
        for(const Type& elt : v) push_back(elt);
    }

    DLL(const DLL& other) : head(nullptr), tail(nullptr), size(0) {
        for(Node* jumper = other.head; jumper; jumper = jumper->next) push_back(jumper->value);
    }

    DLL& operator=(DLL other){

        swap(*this, other);
        return *this;
    }

    ostream& to_ostream(ostream& output) const {

        output << "[ ";
        for(Node* jumper = head; jumper; jumper = jumper->next){
            output << jumper->value << " ";
        }
        output << "]";
        return output;
    }

    bool push_front(const Type& new_value){

        head = new Node(new_value, nullptr, head);
        if(++size == 1) tail = head;
        return true;
    }

    bool push_back(const Type& new_value){

        if(is_empty()) return push_front(new_value);

        tail = new Node(new_value, tail, nullptr);
        ++size;
        return true;
    }

    bool insert(const Type& new_value, const size_t& index){

        if(index>size) return false;
        if(index==0) return push_front(new_value);
        if(index==size) return push_back(new_value);

        // index is somewhere in the middle
        Node* jumper = node_at_index(index-1);
        jumper->next = new Node(new_value, jumper, jumper->next);
        ++size;
        return true;
    }

    bool pop_front(){

        if(is_empty()) return false;

        Node* trash = head;
        head = head->next; // could become nullptr
        delete trash;
        if(--size != 0) head->previous = nullptr; // if size became 0, head is nullptr; no need for fix
        return true;
    }

    bool pop_back(){

        if(size<2) return pop_front();

        tail = tail->previous; // definitely not nullptr, possibly == head
        delete tail->next;
        tail->next = nullptr;
        --size;
        return true;
    }

    bool remove(const size_t& index){

        if(index>=size) return false;
        if(index==0) return pop_front();
        if(index==size-1) return pop_back();

        // index is somewhere in the middle
        Node* trash = node_at_index(index);
        trash->previous->next = trash->next;
        trash->next->previous = trash->previous;
        delete trash;
        --size;
        return true;
    }

    bool replace(const size_t& index, const Type& new_value){

        Node* wanted = node_at_index(index);
        if(wanted) wanted->value = new_value;
        return wanted;
    }

    bool contains(const Type& elt) const {

        if(is_empty()) return false;

        for(Node *j1 = head, *j2 = tail; j1!=j2; j1 = j1->next, j2 = j2->previous){

            if(j1->value==elt || j2->value==elt) return true;
        }
        return false;
    }

    bool is_sorted() const {

        if(size<2) return true;

        Node* start = head;
        Node* end = tail;

        while(start!=end && start!=end->previous){

            if(start->value > start->next->value) return false;
            if(end->value < end->previous->value) return false;

            start = start->next;
            end = end->previous;
        }
        return true;
    }

    void sort(){

        if(size<2) return;
        merge_sort(head, tail);
    }

    Type& operator[](const size_t& index){

        Node* wanted = node_at_index(index);
        if(wanted) return wanted->value;
        else throw "Index out of bounds";
    }

    const Type operator[](const size_t& index) const {

        Node* wanted = node_at_index(index);
        if(wanted) return wanted->value;
        else throw "Index out of bounds";
    }

    bool is_empty() const {
        return !head;
    }

    size_t length() const {
        return size;
    }

    void view_debug_info() const {

        cout << "DLinked List:\n\t-Elements: [ ";
        for(Node* jumper = head; jumper; jumper = jumper->next) cout << jumper->value << " ";
        cout << "]\n\t-Size: " << size << endl;
        if(!is_empty()) cout << "\t-Head: " << head->value << "\n\t-Tail: " << tail->value << endl;
        if(size>1 && head->previous) cout << "There is a head prev!!" << endl;
        if(size>1 && tail->next) cout << "There is a tail next!!" << endl;
    }

    ~DLL(){
        while(pop_back());
    }
};

template<typename Type>
void swap(DLL<Type>& l1, DLL<Type>& l2){

    using std::swap;
    swap(l1.head, l2.head);
    swap(l1.tail, l2.tail);
    swap(l1.size, l2.size);
}

template<typename Type>
ostream& operator<<(ostream& output, const DLL<Type> l){
    return l.to_ostream(output);
}

int main(int argc, char* argv[]){

    DLL<int> my_list;

    return 0;
}