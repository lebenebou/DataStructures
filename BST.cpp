
#include <iostream>
#include <vector>
#include <functional>
#include <queue>
#include <stack>
using namespace std;

template<typename Type>
class BST{

private:

    struct Node{

        Type value;
        Node* left;
        Node* right;

        Node(const Type& v) : value(v), left(nullptr), right(nullptr){}
    };

    template<typename T>
    friend void swap(BST<T>& t1, BST<T>& t2);

    typedef function<void(Node*&)> node_processor;
    typedef function<bool(const Type&, const Type&)> comp_type;

    Node* root;
    size_t size;

    comp_type comparator;
    comp_type equalizer;
    
    bool insert(Node*& start, const Type& new_value){

        if(!start){
            root = new Node(new_value);
            return size=1;
        }

        if(equalizer(new_value, start->value)) return false;

        if(comparator(new_value, start->value)){

            if(!start->left){
                start->left = new Node(new_value);
                return ++size;
            }

            return insert(start->left, new_value);
        }

        else { // !comparator(new_value, start->value)

            if(!start->right){
                start->right = new Node(new_value);
                return ++size;
            }

            return insert(start->right, new_value);
        }
    }

    bool contains(Node* start, const Type& elt) const {

        if(!start) return false;
        if(equalizer(elt, start->value)) return true;

        if(comparator(elt, start->value)) return contains(start->left, elt);
        return contains(start->right, elt);
    }

    size_t height(Node* start) const {

        if(!start) return 0;
        return 1 + std::max(height(start->left), height(start->right));
    }

    void in_order(Node* current, const node_processor& process) const {

        stack<Node*> stack;
        
        while(true){

            if(current){
                stack.push(current);
                current = current->left;
            }
            else if(!stack.empty()){ // node is null but stack not empty
                
                current = stack.top();
                stack.pop();
                process(current);
                current = current->right;
            }
            else return; // stack is empty and node is null
        }
    }

    void pre_order(Node* start, const node_processor& process) const {

        if(!start) return;

        process(start);
        pre_order(start->left, process);
        pre_order(start->right, process);
    }

    void post_order(Node* start, const node_processor& process) const {

        if(!start) return;

        post_order(start->left, process);
        post_order(start->right, process);
        process(start);
    }

    void bfs(Node* start, const node_processor& process) const {

        queue<Node*> q;
        q.push(start);

        while(!q.empty()){

            Node* current = q.front();
            process(current);
            q.pop();

            if(current->left) q.push(current->left);
            if(current->right) q.push(current->right);
        }
    }

public:
    BST(comp_type comp = [](const Type& a, const Type& b)->bool { return a < b; }, comp_type equ = [](const Type& a, const Type& b)->bool { return a == b; }){
        
        root = nullptr;
        size = 0;
        comparator = comp;
        equalizer = equ;
    }

    BST(const vector<Type>& v, comp_type comp = [](const Type& a, const Type& b)->bool { return a < b; }, comp_type equ = [](const Type& a, const Type& b)->bool { return a == b; }){
        
        root = nullptr;
        size = 0;
        comparator = comp;
        equalizer = equ;

        for(const Type& elt : v) this->insert(elt);
    }

    BST(const BST<Type>& other) : root(nullptr), size(0), comparator(other.comparator), equalizer(other.equalizer){

        other.bfs(other.root, [this](Node*& n)->void { this->insert(n->value); });
    }

    BST& operator=(BST<Type> other){

        swap(*this, other);
        return this;
    }

    ostream& to_ostream(ostream& output) const {

        output << "[ ";
        bfs(root, [&output](Node*& n)->void { output << n->value << " "; });
        return output << "]";
    }

    bool insert(const Type& new_value){

        return insert(root, new_value);
    }

    bool contains(const Type& elt) const {
        return contains(root, elt);
    }

    size_t height() const {
        return height(root);
    }

    Type accumulate(const function<Type(const Type& a, const Type& b)>& op, const Type& zero) const {

        Type answer = zero;
        bfs(root, [&answer, op](Node*& n)->void { answer = op(answer, n->value); });
        return answer;
    }

    bool is_empty() const {
        return !root;
    }

    void display(const short& traversal_type = 1) const {

        const node_processor print_val = [](Node*& n)->void { cout << n->value << " "; };

        cout << "[ ";

        switch (traversal_type){
            
            case 1:
                in_order(root, print_val);
                break;

            case 2:
                pre_order(root, print_val);
                break;

            case 3:
                post_order(root, print_val);
                break;

            default:
                bfs(root, print_val);
                break;
        }

        cout << "]" << endl;
    }

    ~BST(){
        post_order(root, [](Node*& n)->void { delete n; });
    }
};

template<typename Type>
void swap(BST<Type>& t1, BST<Type>& t2){

    using std::swap;
    swap(t1.root, t2.root);
    swap(t1.size, t2.size);
    swap(t1.comparator, t2.comparator);
    swap(t1.equalizer, t2.equalizer);
}

template<typename Type>
ostream& operator<<(ostream& output, const BST<Type>& tree){
    return tree.to_ostream(output);
}

int main(){

    BST<int> tree({8, 3, 10, 1, 6, 14, 4, 7, 13});
    tree.display(1);

    return 0;
}