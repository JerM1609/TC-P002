#include <iostream>
#include <string>
#include <chrono>
using namespace std;

using time_point_t = chrono::time_point<chrono::high_resolution_clock>;
const int FIRST_UPPER=65, UPPER_SIZE=90-65+1, FIRST_LOWER=97, LOWER_SIZE=122-97+1;

// AUTOMATA
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

// GIC
struct Rule;
struct Right;
struct RightArray;
struct GIC;
struct MapRight;
struct charSet;
struct charQueue;

struct charSet
{
    int sz=0, capacity=1;
    char* arr = new char[capacity];
    bool F_next = false;

    charSet() = default;
    void add(char el);
    void resize();
    void show();
};

struct charQueue
{
    char *array;      // for runtime resizing
    int capacity = 2;   // for table doubling
    int back, front;    // indirection markers
    int size_ = 0;

    charQueue()
    {
        this->array = new char[this->capacity];         // dynamic array in heap
        this->front = this->back = -1;          // initialize indirection markers
        size_ = 0;
    };
    ~charQueue(){ delete [] array; }

    void enqueue(char data);
    char dequeue();

    bool is_full() { return size_ == capacity; }
    bool empty(){ return (size_ == 0); }

    int size(){ return size_; }
    void resize(int new_capacity);

    int next(int index){ return (index + 1) % capacity; }
    int prev(int index){ return (index <= 0) ? capacity - 1 : index - 1; }
    void display();
};

struct Right
{
    /**
     * Define the right side of rule in GIC
     * Eg:
     * S -> BA
     * BA with respective pointers
     *
     * U -> iUCWvCG
     * iUCWvCG with respective pointers
    */
    int sz=0, capacity=1;
    char* right = new char[capacity];   // iUCWvCG
    Rule** rules = new Rule*[capacity];
    int count;

    Right() = default;
    void add(char el, Rule* r);
    void resize();
    void show();
    bool is_generator();
};

struct RightArray
{
    /**
     * Define all RightSide of Rule
     * U -> iUCWvCG | UUvVAU
    */
    int sz=0, capacity=1;
    Right** right = new Right*[capacity]{};

    RightArray() = default;
    void add(Right* rr);
    void resize();
    bool generator();
    void display();
};

struct Rule
{
    /**
     * Define a set of Rules
     * Eg:
     * S -> BA | b
     * U -> iUCWvCG | UUvVAU
     */
    char left;
    bool is_generator = false;

    RightArray rights;

    Rule(char l): left{l} {}
    void add(Right* r);

    void display();
    bool update_generator();
};

struct MapRight
{
    RightArray** mapa = new RightArray*[UPPER_SIZE]{};
    bool is_gen[UPPER_SIZE]{false};

    MapRight() = default;
    void add(char k, Right* v);
    void update(char front);

    void show();
};

struct GIC
{
    string terminales, variables;
    char var_inicial;
    Rule** rule_arr = new Rule*[UPPER_SIZE];
    MapRight* map = new MapRight();
    charQueue cq;

    GIC(string term, string var);
    void add_rule(char left, string right);
    void show();

    bool empty_test();
    bool empty_test_n();
};

