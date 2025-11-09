#ifndef MY_RBTREE_H
#define MY_RBTREE_H

#include "my_allocator.h"
#include "my_utility.h"
#include "my_iterator.h"

namespace mystl{

// ===================
// rbtree node struct
// ===================
enum class RBTreeColor{Red, Black};

template <typename T>
struct RBTreeNode{
    using self = RBTreeNode<T>;

    T value;
    self* parent;
    self* left;
    self* right;
    RBTreeColor color;

    RBTreeNode(const T& v)
        : value(v), parent(nullptr), left(nullptr), right(nullptr), color(RBTreeColor::Red){}
};

// ====================
// rb tree iterator
// ====================
template <typename T>
class RBTreeIterator{
public:
    using self = RBTreeIterator<T>;
    using node = RBTreeNode<T>;
    using node_ptr = node*;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    node_ptr cur;

    RBTreeIterator() : cur(nullptr){}

    explicit RBTreeIterator(node_ptr n) : cur(n){}

    reference operator*() const {return cur -> value;}

    pointer operator->() const {return &(operator*());}

    bool operator==(const self& other) const noexcept {
        return cur == other.cur;
    }

    bool operator!=(const self& other) const noexcept {
        return cur != other.cur;
    }

    self& operator++(){
        if(cur -> right){
            //case1: right tree, find the left most of right subtree
            cur = cur -> right;
            while(cur -> left){
                cur = cur -> left;
            }
        }else{
            // case 2: no right subtree, search for parent
            node_ptr p = cur -> parent;
            while(cur == p -> right){
                cur = p;
                p = p->parent;
            }
            if(cur->right != p){
                cur = p;
            }
        }
        return *this;
    }

    self operator++(int){ self tmp = *this; ++(*this); return tmp;}

    self& operator--(){
        if(cur ->color == RBTreeColor::Red && cur -> parent->parent == cur){
            //current is header_ (end) back to max node
            cur = cur -> right;
        }else if(cur -> left){
            //case 1 : left sub tree, find the right most in left
            cur = cur -> left;
            while(cur -> right){
                cur = cur -> right;
            }
        } else{
            // case 2: no left sub tree, back to parent?
            node_ptr p = cur -> parent;
            while(cur == p->left){
                cur = p;
                p = p -> parent;
            }
            cur = p;
        }
        return *this;
    }

    self operator--(int){self tmp = *this; --(*this); return tmp;}



};


// =====================
// rb tree 
// =====================
template <typename Key, typename Value, typename KeyOfValue, typename Compare, 
          typename Alloc = mystl::MyAllocator<RBTreeNode<Value>>>
class MyRBTree
{
public:
    using key_type = Key;
    using value_type = Value;
    using size_type = size_t;
    using node = RBTreeNode<Value>;
    using node_ptr = node*;
    using allocator_type = Alloc;

    using iterator = RBTreeIterator<Value>;
    using const_iterator = RBTreeIterator<const Value>;
    using reference = Value&;
    using const_reference = const Value&;


private:
    node_ptr header_; // sentinel node (header_ -> parent is root)
    size_type node_count_;
    Compare comp_;
    allocator_type alloc_;

    node_ptr& root() const noexcept {return header_ -> parent;}
    node_ptr& leftmost() const noexcept {return header_ -> left;}
    node_ptr& rightmost() const noexcept {return header_ -> right;}

public:

    MyRBTree() : node_count_(0), comp_(Compare()), alloc_() {
        header_ = create_node(Value{});
        header_->color = RBTreeColor::Black;
        header_ ->parent = nullptr;
        header_ -> left = header_;
        header_ -> right = header_;

    }
    
    ~MyRBTree () {
        //std::cout << "[DEBUG] Destroying RBTree, nodes=" << node_count_ << "\n";
        clear();
        destroy_node(header_); 
        header_ = nullptr;
        //std::cout << "[DEBUG] Header destroyed\n";
    }

    bool empty() const noexcept { return node_count_== 0;}

    size_type size() const noexcept {return node_count_;}

    iterator begin() noexcept {return iterator(leftmost());}

    iterator end() noexcept {return iterator(header_);}

    // ========= node destroy and create ========
    node_ptr create_node(const value_type& value){
        node_ptr n = alloc_.allocate(1);
        alloc_.construct(n, value);

        n->parent = n->left = n -> right = nullptr;
        n-> color = RBTreeColor::Red;
        return n;
    }

    void destroy_node(node_ptr n){
        alloc_.destroy(n);
        alloc_.deallocate(n, 1);
    }

    void clear(){
        if(root()){
            clear_subtree(root());
            root() = nullptr;
        }
        leftmost() = header_;
        rightmost() = header_;
        header_->parent = nullptr;
        node_count_ = 0;
    }

    iterator insert_equal(const value_type& v){
        node_ptr y = header_;  // 
        node_ptr x = root();

        while(x != nullptr){
            y = x;
            if( comp_(KeyOfValue()(v), KeyOfValue()(x->value))){
                x = x->left;
            }else{
                x = x-> right;
            }
        }

        node_ptr z = create_node(v);
        z->parent = y;

        if(y == header_){
            root() = z;
            leftmost() = z;
            rightmost() = z;
        }else if (comp_(KeyOfValue()(v), KeyOfValue()(y->value))){
            y->left = z;
            if(y == leftmost()){
                leftmost() = z;
            }
        }else{
            y->right = z;
            if(y == rightmost()){
                rightmost() = z;
            }
        }

        insert_reblance(z, root());
        ++node_count_;
        return iterator(z);
    }



    // ================= rotate left ================
    //     p                         x
    //     \                       /   \
    //     x          =>          p   xr
    //    / \                     \
    //  xl  xr                    xl

    void rotate_left(node_ptr x, node_ptr& root){
        node_ptr y = x -> right;
        x-> right = y->left;
        if(y ->left){
            y->left->parent = x;
        }

        y-> parent = x -> parent;
        if(x == root){
            root = y;
        }else if(x == x -> parent -> left){
            x -> parent ->left = y;
        }else{
            x -> parent -> right = y;
        }
        
        // Add missing connections
        y->left = x;
        x->parent = y;
    }

    void rotate_right(node_ptr x, node_ptr& root){
        node_ptr y = x -> left;
        x -> left = y -> right;
        if( y -> right){
            y -> right-> parent = x;
        }

        y-> parent = x -> parent;

        if(x == root){
            root = y;
        }else if ( x == x-> parent-> right){
            x->parent->right = y;   // Fix empty branch
        }else{
            x->parent->left = y;
        }

        // Add missing connections
        y->right = x;
        x->parent = y;
    }

    void insert_reblance(node_ptr z, node_ptr& root){
        z->color = RBTreeColor::Red;

        while(z != root && z-> parent->color == RBTreeColor::Red){
            if(z -> parent == z->parent -> parent -> left){
                node_ptr y = z-> parent -> parent -> right; // uncle
                if(y && y -> color == RBTreeColor::Red){
                    // Case 1: parent red, uncle red
                    //      g(black)
                    //      /    \
                    //  p(red) u(red)
                    //     /
                    // z(red)
                    z->parent->color = RBTreeColor::Black;
                    y->color = RBTreeColor::Black;
                    z->parent->parent->color = RBTreeColor::Red;
                    z = z -> parent -> parent;

                } else {
                    if( z == z->parent -> right){
                        /*
                        // Case 2: fold line
                        //         g(black)
                        //          /
                        //       p(red)
                        //          \
                        //          z(red)
                        */

                        z = z-> parent;
                        rotate_left(z, root);
                    }
                    //case 3: strate line
                    //       g(black)
                    //         /
                    //     p(red)
                    //     /
                    //  z(red)
                    // z->parent->color = RBTreeColor::Black;
                    z->parent->parent->color = RBTreeColor::Red;
                    rotate_right(z->parent->parent, root);
                }
            } else{
                //mirrorr: left -right reverse
                node_ptr y = z->parent->parent->left;
                if(y &&  y->color == RBTreeColor::Red){
                    //Case 1 mirror
                    z->parent->color = RBTreeColor::Black;
                    y->color = RBTreeColor::Black;
                    z->parent->parent->color = RBTreeColor::Red;
                    z = z->parent->parent;
                }else{
                    if(z == z-> parent -> left){
                        // Case 2 mirror
                        z = z->parent;
                        rotate_right(z, root);
                    }

                    // Case 3 mirror
                    z->parent->color = RBTreeColor::Black;
                    z->parent->parent->color = RBTreeColor::Red;
                    rotate_left(z->parent->parent, root);
                }
            }
        }

        root->color = RBTreeColor::Black;
    }


    node_ptr minimum(node_ptr x){
        if(!x) return nullptr;
        while( x-> left) x = x->left;
        return x;
    }

    void erase(iterator pos){
        node_ptr z = pos.cur;
        erase_node(z);
    }

    void erase_node(node_ptr z){
        //std::cout << "[DEBUG] Erase node: " << z << " value: " << z->value << "\n";
        node_ptr y = z;                     // actual node to delete
        node_ptr x = nullptr;               // y's child node
        node_ptr x_parent = nullptr;        // x's parent node
        RBTreeColor y_original_color = y->color;

        //std::cout << "[DEBUG] Case: ";
        // 1.  y determin which node to delete (if z has 2 children, use the intermediate to replace)
        if(z->left == nullptr){
            //std::cout << "z has no left child\n";
            x = z->right;
            transplant(z, z->right);
            x_parent = z->parent;
        }else if(z->right == nullptr){
            //std::cout << "z has no right child\n";
            x = z->left;
            transplant(z, z->left);
            x_parent = z->parent;
        }else{
            //std::cout << "z has two children\n";
            y = minimum(z->right);   // find the intermediate
            y_original_color = y->color;
            x = y->right;
            if(y->parent == z){
                //std::cout << "y is direct child of z\n";
                x_parent = y;
            }else{
                //std::cout << "y is not direct child of z\n";
                transplant(y, y->right);
                y->right = z->right;
                if (y->right) {
                    y->right->parent = y;
                }
                x_parent = y->parent;
            }

            transplant(z, y);
            y->left = z->left;
            if (y->left) {
                y->left->parent = y;
            }
            y->color = z->color;
        }

        // 2 if the deleted is black, break the balance -> fix
        if(y_original_color == RBTreeColor::Black){
            //std::cout << "[DEBUG] Rebalancing after erase\n";
            erase_rebalance(x, x_parent);
        }



        // Update leftmost and rightmost if needed
        if(z == leftmost()) {
            if (root() == nullptr) {
                leftmost() = header_;
            } else {
                leftmost() = minimum(root());
            }
        }
        if(z == rightmost()) {
            if (root() == nullptr) {
                rightmost() = header_;
            } else {
                rightmost() = maximum(root());
            }
        }

        // clear root() if it points to deleted node
        if(root() == z)
            root() = nullptr;
        
        header_->parent = root();

        if(root() == nullptr){
            leftmost() = header_;
            rightmost() = header_;
        }else{
            leftmost() == minimum(root());
            rightmost() == maximum(root());
        }
                

        destroy_node(z);
        --node_count_;

        //std::cout << "[DEBUG] Erase successful. New size: " << node_count_ << "\n";
    }

    void erase_rebalance(node_ptr x, node_ptr& x_parent){
        if(x_parent == nullptr) return;
        while ((x_parent && x != root()) &&
            (x == nullptr || x->color == RBTreeColor::Black)) {

            if (x == x_parent->left) {
                node_ptr w = x_parent->right;
                if (!w) break;

                if (w->color == RBTreeColor::Red) {                  // Case 1
                    w->color = RBTreeColor::Black;
                    x_parent->color = RBTreeColor::Red;              // FIXED
                    rotate_left(x_parent, root());
                    w = x_parent->right;
                    if (!w) break;
                }

                if ((w->left  == nullptr || w->left->color  == RBTreeColor::Black) &&
                    (w->right == nullptr || w->right->color == RBTreeColor::Black)) {  // Case 2
                    w->color = RBTreeColor::Red;
                    x = x_parent;
                    x_parent = x_parent->parent;                     // FIXED
                } else {
                    if (w->right == nullptr || w->right->color == RBTreeColor::Black) { // Case 3
                        if (w->left) w->left->color = RBTreeColor::Black;
                        w->color = RBTreeColor::Red;
                        rotate_right(w, root());
                        w = x_parent->right;
                    }
                    // Case 4
                    w->color = x_parent->color;
                    x_parent->color = RBTreeColor::Black;
                    if (w->right) w->right->color = RBTreeColor::Black;
                    rotate_left(x_parent, root());
                    x = root();
                }
            } else { // mirror
                node_ptr w = x_parent->left;
                if (!w) break;

                if (w->color == RBTreeColor::Red) {                  // Case 1 (mirror)
                    w->color = RBTreeColor::Black;
                    x_parent->color = RBTreeColor::Red;
                    rotate_right(x_parent, root());
                    w = x_parent->left;
                    if (!w) break;
                }

                if ((w->right == nullptr || w->right->color == RBTreeColor::Black) &&
                    (w->left  == nullptr || w->left->color  == RBTreeColor::Black)) {  // Case 2 (mirror)
                    w->color = RBTreeColor::Red;
                    x = x_parent;
                    x_parent = x_parent->parent;                     // FIXED
                } else {
                    if (w->left == nullptr || w->left->color == RBTreeColor::Black) {  // Case 3 (mirror)
                        if (w->right) w->right->color = RBTreeColor::Black;
                        w->color = RBTreeColor::Red;
                        rotate_left(w, root());
                        w = x_parent->left;
                    }
                    // Case 4 (mirror)
                    w->color = x_parent->color;
                    x_parent->color = RBTreeColor::Black;
                    if (w->left) w->left->color = RBTreeColor::Black;
                    rotate_right(x_parent, root());
                    x = root();
                }
            }
        }

        if (x) x->color = RBTreeColor::Black;
    }

    iterator find(const key_type& key) noexcept{
        node_ptr cur = root();
        while (cur != nullptr)
        {
            if(comp_(key, KeyOfValue()(cur->value)))
                cur = cur->left;
            else if (comp_(KeyOfValue()(cur->value), key))
                cur = cur->right;
            else
                return iterator(cur);
        }
        return end();        

    }



private:
    void clear_subtree(node_ptr x){
        if( x == nullptr || x == header_) return;

        clear_subtree(x -> left);
        clear_subtree(x -> right);
        destroy_node(x);
        
    }

    // use v to replace u in rbtree
    void transplant(node_ptr u, node_ptr v){
        if(u->parent == nullptr){   // u is root
            root() = v;
        }else if(u == u->parent->left){
            u->parent->left = v;
        }else{
            u->parent->right = v;
        }

        if(v != nullptr){
            v->parent = u->parent;
        }
    }

    node_ptr maximum(node_ptr x){
        if(!x) return nullptr;
        while(x->right) x = x->right;
        return x;
    }

};

} // namesapce mystl
#endif // MY_RBTREE_H