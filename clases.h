#include <iostream>
#include <string>
#include <chrono>
using namespace std;

using time_point_t = chrono::time_point<chrono::high_resolution_clock>;

struct Transition;
struct State;
struct Automata;
struct ParBFS;
struct Queue;
struct intArray;
struct StateArray;

void find(State* curr_state, string& path, intArray id_);
void traverse(State* curr_state, intArray& id_, string resto);

struct intArray
{
    int sz=0, capacity=1;
    int* arr = new int[capacity];
    bool F_next = false;

    intArray() = default;
    intArray(int v);
    void add(int el);
    void resize();
    void show();
    
    intArray copy();
    void merge(intArray other);
    bool contain(int other);
    
    bool operator==(const intArray other);
    bool operator!=(const intArray other);
};

struct StateArray
{
    int sz=0, capacity=1;
    State** arr = new State*[capacity];

    StateArray() = default;
    void add(State* st);
    void resize();
};

class timer_t
{
    time_point_t start;
    time_point_t stop;
public:
    timer_t(){ start = chrono::high_resolution_clock::now(); }
    ~timer_t()
    {   // https://en.cppreference.com/w/cpp/chrono/duration
        stop = chrono::high_resolution_clock::now();
        cout << "\t\tduration: "
             << (chrono::duration_cast<chrono::nanoseconds>(stop-start).count())
             << " ns" << endl;
    }
};

struct Transition
{
    char symbol=' ';
    State* next = nullptr;

    Transition() = default;
    Transition(char _symbol, State* _next);
    void show();
};

struct State
{
    int sz_delta = 0, cap_delta = 1;
    intArray id;
    Transition** delta = new Transition*[cap_delta]{};
    bool F = false;

    State() = default;
    State(int i, bool F_);
    State(int i, int c, bool F_);
    State(intArray array, bool F_ = false): id{array}, F{F_}{}

    
    void add_id(int new_id);
    
    void add_transition(Transition* t);
    void resize_transition();

    intArray get_set_id(char symbol);
    bool compare_ids(intArray arr);

    void display();
};

struct ParBFS
{
    int index;
    State* state;
    ParBFS() = default;
    ParBFS(int _index, State* _state): index{_index}, state{_state}{}
};

struct Queue
{
    ParBFS *array;      // for runtime resizing
    int capacity = 2;   // for table doubling
    int back, front;    // indirection markers
    int size_ = 0;

    Queue()
    {
        this->array = new ParBFS[this->capacity];         // dynamic array in heap
        this->front = this->back = -1;          // initialize indirection markers
        size_ = 0;
    };
    ~Queue(){ delete [] array; }

    void enqueue(ParBFS data);
    ParBFS dequeue();

    bool is_full() { return size_ == capacity; }
    bool empty(){ return (size_ == 0); }

    int size(){ return size_; }
    void resize(int new_capacity);

    int next(int index){ return (index + 1) % capacity; }
    int prev(int index){ return (index <= 0) ? capacity - 1 : index - 1; }
};

struct Automata
{
    int sz_Q = 1, cap_Q = 2, id_count=1;
    State** Q = new State*[cap_Q]{};
    string E;

    Automata(string& E_): E{E_}
    {
        this->Q[0] = new State(this->id_count++);
    }

    void build_AFN(string*& T, int t);
    Automata* transform_AFD();
    Automata* transform_AFD_2();

    void BFS(string s);
    void test(string*& S, int q);

    void resize_Q();
    void adjacency_list();
    
    void add_state(State* s);
    State* get_state(intArray id);
    string DFS(intArray id);
    intArray get_by_suffix(string suffix);
};