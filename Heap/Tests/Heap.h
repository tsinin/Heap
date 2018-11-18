//
// Created by Tsinin on 27.10.2018.
//
#include "Vector.h"

template <typename Key>
class Heap {
public:
    class Pointer {
    private:
        friend class Heap<Key>;
        Pointer(int new_index): index(new_index) {}
        ~Pointer() = default;
        int index;
    };
    bool is_empty() const {
        return (heap.size() == 0);
    }
    Pointer* insert(Key x) {
        heap.push_back(x);
        Pointer* ptr_to_Pointer = new Pointer(heap.size() - 1);
        back_ptr.push_back(ptr_to_Pointer);
        sift_up(heap.size() - 1);
        return ptr_to_Pointer;
    }
    Key get_min() {
        try {
            if(this->is_empty())
                throw std::logic_error("Heap is empty.");
        }
        catch(std::logic_error& error) {
            std::cerr << error.what() << std::endl;
            exit(1);
        }

        return heap[0];
    }
    Key extract_min() {
        try {
            if(this->is_empty())
                throw std::logic_error("Heap is empty.");
        }
        catch(std::logic_error& error) {
            std::cerr << error.what() << std::endl;
            exit(1);
        }

        Key min_el = heap[0];
        swap_elements(0, heap.size() - 1);
        delete back_ptr[heap.size() - 1];
        heap.pop_back();
        back_ptr.pop_back();
        sift_down(0);
        return min_el;
    }
    void erase(Pointer* pointer) {
        try {
            if(pointer == nullptr)
                throw std::invalid_argument("Element has been already deleted.");
        }
        catch(std::invalid_argument& error) {
            std::cerr << error.what() << std::endl;
            exit(3);
        }

        int index = pointer->index;
        swap_elements(pointer->index, heap.size() - 1);
        delete back_ptr[heap.size() - 1];
        heap.pop_back();
        back_ptr.pop_back();
        sift_down(index);
        sift_up(index);
    }
    void change(Pointer* pointer, Key newKey) {
        try {
            if(pointer == nullptr)
                throw std::invalid_argument("Element does not exist.");
        }
        catch(std::invalid_argument& error) {
            std::cerr << error.what() << std::endl;
            exit(3);
        }

        heap[pointer->index] = newKey;
        if(pointer->index > 0 && newKey < heap[(pointer->index - 1) / dimension])
            sift_up(pointer->index);
        else
            sift_down(pointer->index);
    }

    void optimize(int insertCount, int extractCount) {
        try {
            if(!(this->is_empty()))
                throw std::logic_error("Heap is not empty.");
        }
        catch(std::logic_error error) {
            std::cerr << error.what() << '\n';
            exit(2);
        }
        if(extractCount)
            dimension = insertCount / extractCount;
        else
            dimension = insertCount;
        if(dimension < 2)
            dimension = 2;
    }
    Heap() = default;

    template <class Iterator>
    Heap(Iterator begin, Iterator end) {
        Iterator current = begin;
        while(current != end) {
            heap.push_back(*current);
            back_ptr.push_back(nullptr);
            back_ptr[back_ptr.size() - 1] = new Pointer(back_ptr.size() - 1);
            ++current;
        }
        for(int i = heap.size() / dimension; i >= 0; --i)
            sift_down(i);
    }
    ~Heap() {
        for(int i = 0; i < back_ptr.size(); ++i)
            delete back_ptr[i];
    }
private:
    int dimension = 2;
    Vector<Key> heap;
    Vector<Pointer*> back_ptr;
    void swap_elements(int a, int b) {
        Key temporaryForSwapKeys = heap[a];
        heap[a] = heap[b];
        heap[b] = temporaryForSwapKeys;

        Pointer* temporaryForSwapPtr = back_ptr[a];
        back_ptr[a] = back_ptr[b];
        back_ptr[b] = temporaryForSwapPtr;

        back_ptr[a]->index = a;
        back_ptr[b]->index = b;
    }
    void sift_up(int pt) {
        while (pt > 0 && heap[pt] < heap[(pt - 1) / dimension]) {
            swap_elements((pt - 1) / dimension, pt);
            pt = (pt - 1) / dimension;
        }
    }
    void sift_down(int pt) {
        while((long long)dimension * pt + 1 < heap.size()) {
            int minPtr = pt;
            long long maxChild = (long long) dimension * (pt + 1);
            for(long long child = dimension * pt + 1; child <= maxChild; ++child) {
                if(child >= heap.size())
                    break;
                if(heap[child] < heap[minPtr])
                    minPtr = child;
            }
            if(minPtr == pt)
                break;
            swap_elements(pt, minPtr);
            pt = minPtr;
        }
    }
};