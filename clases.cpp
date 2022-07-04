#include "clases.h"


//              AUTOMATA

// AUXILIARY FUNCTIONS
void find(State* curr_state, string& path, intArray id_)
{
    // curr_state->id.show();
    if (curr_state->id == id_)
        return;
    if (curr_state->F)
    {
        path=""; 
        return;
    }
    for (int i = 0; i < curr_state->sz_delta; i++)
    {
        Transition* curr_transition = curr_state->delta[i];
        intArray next_id = curr_transition->next->id;
        if (next_id != curr_state->id)
        {
            path += curr_transition->symbol;
            find(curr_transition->next, path, id_);
        }
        // cout << path << " ";
        if (!path.empty())
            break;
    }
}

void traverse(State* curr_state, intArray& id_, string resto)
{
    if (resto == "")
        id_.merge(curr_state->id);
    if (curr_state->F)
        return;
    for (int i = 0; i < curr_state->sz_delta; i++)
    {
        Transition* curr_transition = curr_state->delta[i];
        if (curr_transition->symbol == resto[0])
            traverse(curr_transition->next, id_, resto.substr(1, resto.size()-1));
    }
}

// TRANSITION
Transition::Transition(char _symbol, State* _next): symbol{_symbol}, next{_next}{}

void Transition::show()
{
    // printf("%c ", this->symbol);
    printf("(%c ", this->symbol);
    this->next->id.show();
    printf(") ");
}

// STATE
State::State(int i, bool F_ = false) : F{F_}
{
    this->id.add(i);
}

State::State(int i, int c, bool F_ = false) : cap_delta{c}
{
    this->id.add(i);
    delta = new Transition*[cap_delta]{};
}

void State::add_id(int new_id)
{
    this->id.add(new_id);
}

void State::add_transition(Transition *t)
{
    if (this->sz_delta == this->cap_delta)
        this->resize_transition();
    this->delta[sz_delta++] = t;
}

void State::resize_transition()
{
    this->cap_delta *= 2;
    auto aux_delta = new Transition*[this->cap_delta]{};
    for (int i = 0; i < this->sz_delta; ++i)
        aux_delta[i] = this->delta[i];
    delete [] this->delta;
    this->delta = aux_delta;
}

intArray State::get_set_id(char symbol)
{
    intArray arr;
    bool FF = false;
    for (int i = 0; i < this->sz_delta; ++i) {
        if (this->delta[i] == nullptr)
            continue;
        if (this->delta[i]->symbol == symbol)
        {
            FF |= this->delta[i]->next->F;
            //if (FF)
            //    printf("%c", symbol);
            // FF = false;
            for (int j = 0; j < this->delta[i]->next->id.sz; ++j)
                arr.add(this->delta[i]->next->id.arr[j]);
        }
    }
    arr.F_next = FF;
    return arr;
}

bool State::compare_ids(intArray arr)
{
    if (this->id.sz != arr.sz)
        return false;
    for (int i = 0; i < this->id.sz; ++i)
        if (this->id.arr[i] != arr.arr[i])
            return false;
    return true;
}

void State::display()
{
    this->id.show();
    printf("%c -> ", (this->F ? '\'' : ' '));
    for (int i = 0; i < this->sz_delta; ++i)
        this->delta[i]->show();
    printf("\n");
}

// AUTOMATA
void Automata::build_AFN(string *&T, int t)
{
    // loop
    for (int i = 0; i < this->E.size(); ++i)
        this->Q[0]->add_transition(new Transition(E[i], Q[0]));

    // branches for each word
    for (int i = 0; i < t; i++)
    {   // for each word in T[]
        string c_word = T[i];
        State* c_state = Q[0];

        if (c_word.empty())
            c_state->F = true;

        for (int j = 0; j < c_word.size(); ++j)
        {   // for each character in current word add new transition
            auto n_state = new State(this->id_count++, (j == c_word.size() - 1));
            c_state->add_transition(new Transition(c_word[j], n_state));
            this->add_state(n_state);
            c_state = n_state;
        }
    }
}

Automata *Automata::transform_AFD()
{
    auto AFD = new Automata(this->E);
    int sz_E = this->E.size();

    for (int i = 0; i < sz_E; ++i)
    {   // por cada elemento del abecedario
        auto current_id = this->Q[0]->get_set_id(this->E[i]);
        State* state = AFD->get_state(current_id);      // O(n)

        if (state == nullptr)
        {
            state = new State(current_id);      // , global_E
            AFD->Q[0]->add_transition(new Transition(this->E[i], state)); // O(n)
            AFD->add_state(state);  // O(n)
        }
        else
            AFD->Q[0]->add_transition(new Transition(this->E[i], state)); // O(n)
    }

    for (int i = 1; i < AFD->sz_Q; ++i)
    {   // por cada estado nuevo en el AFD generado a partir del estado inicial
        intArray state_id =  AFD->Q[i]->id;
        // state_id.show();
        // printf("\n");
        for (int j = 0; j < sz_E; ++j)
        {   // por cada letra del abecedario
            intArray id_to_search;
            for (int k = 0; k < state_id.sz; ++k)
            {   // por cada estado en el id de los estados compuestos del AFD
                // ver a donde van con la letra E[j]
                intArray curr_state(state_id.arr[k]);
                State* from = this->get_state(curr_state);

                intArray to = from->get_set_id(E[j]);
                for (int l = 0; l < to.sz; ++l)
                    id_to_search.add(to.arr[l]);
                id_to_search.F_next |= to.F_next;
            }
            State* find_state = AFD->get_state(id_to_search);
            if (find_state == nullptr)
            {   // crear nuevo estado y agregar transicion del actual al nuevo estado
                auto n_state = new State(id_to_search, id_to_search.F_next);
                AFD->add_state(n_state);
                AFD->Q[i]->add_transition(new Transition(this->E[j], n_state));
            }
            else
                AFD->Q[i]->add_transition(new Transition(this->E[j], find_state));
        }
    }

    return AFD;
}

Automata* Automata::transform_AFD_2()
{
    // generate new states
    auto AFD = new Automata(this->E);
    for (int i=0; i<this->sz_Q; ++i)
    {
        if ( AFD->Q[0]->id != this->Q[i]->id)
        {   // if ids are not equal
            intArray copy_state_id;
            copy_state_id.merge(this->Q[i]->id);
            copy_state_id.F_next = this->Q[i]->F;
            // auto  = this->Q[i]->id.copy();
            
            // añadirle los otros mediante el sufijo
            string path = this->DFS(copy_state_id);
            string suffix;

            // buscar a donde se llega con cierto sufijo y mergear
            for (int i = 0; i < path.size(); i++)
            {
                string suffix = path.substr(i, path.size()-i+1);
                intArray suffix_id = this->get_by_suffix(suffix);
                copy_state_id.merge(suffix_id);
            }
            
            // copy_state_id.show(); 
            // cout << boolalpha << " " << copy_state_id.F_next << endl;

            auto new_state = new State(copy_state_id, copy_state_id.F_next);
            AFD->add_state(new_state);
        }
    }
    
    // add transitions
    
    for (size_t i = 0; i < AFD->sz_Q; i++)
    {   // por cada estado del nuevo AFD
        State* curr_state = AFD->Q[i];

        // se parsean los estados individualmente
        StateArray states_of_AFN;
        int sz_arr = curr_state->id.sz;
        for (size_t i = 0; i < sz_arr; i++)
        {
            intArray id_to_search;
            id_to_search.add(curr_state->id.arr[i]);

            State* state_searched = this->get_state(id_to_search);
            states_of_AFN.add(state_searched);
        }
        // curr_state->id.show(); cout << endl;
        int sz_st_arr = states_of_AFN.sz;
        for (size_t i = 0; i < this->E.size(); i++)
        {
            char curr_letter = this->E[i];
            // cout << "curr_letter: " << curr_letter << ": ";
            intArray id_to_add_transition;
            for (int i = 0; i < sz_st_arr; i++)
            {
                intArray id_next = states_of_AFN.arr[i]->get_set_id(curr_letter);
                id_to_add_transition.merge(id_next);
            }
            // id_to_add_transition.show(); cout << endl;
            State* next_state = AFD->get_state(id_to_add_transition);
            Transition* new_transition = new Transition(curr_letter, next_state);
            curr_state->add_transition(new_transition);
        }
        
    }
    
    return AFD;
}

void Automata::BFS(string s)
{
    Queue q;
    q.enqueue(ParBFS(-1, this->Q[0]));
    while (!q.empty())
    {
        ParBFS p = q.dequeue();
        if (p.state->F)
        {
            printf("YES\n");
            return;
        }
        int idx = p.index + 1;
        if (idx == s.size())
            continue;

        for (int i = 0; i < p.state->sz_delta; ++i)
        {
            if (p.state->delta[i]->symbol == s[idx])
                q.enqueue(ParBFS(idx, p.state->delta[i]->next));
        }
    }
    printf("NO\n");
}

void Automata::test(string *&S, int q)
{
    for (int i = 0; i < q; ++i)
        this->BFS(S[i]);
}

void Automata::resize_Q()
{
    this->cap_Q *=2;
    auto aux_Q = new State*[this->cap_Q]{};
    for (int i = 0; i < this->sz_Q; ++i)
        aux_Q[i] = this->Q[i];
    delete [] this->Q;
    this->Q = aux_Q;
}

void Automata::adjacency_list()
{
    for (int i = 0; i < this->sz_Q; ++i)
    {
        if (this->Q[i] == nullptr)
            break;
        this->Q[i]->display();
    }
}

void Automata::add_state(State *s)
{
    for (size_t i = 0; i < sz_Q; i++)
        if (this->Q[i]->id == s->id)
            return;
    
    if (sz_Q == cap_Q)
        this->resize_Q();
    this->Q[sz_Q++] = s;
}

State *Automata::get_state(intArray id)
{
    for (int i=0; i<this->sz_Q; ++i)
    {
        if (this->Q[i]->id == id)
            return this->Q[i];
    }
    return nullptr;
}

string Automata::DFS(intArray id)
{
    string res="";
    State* first_state = this->Q[0];
    find(first_state, res, id);
    return res;   
}

intArray Automata::get_by_suffix(string suffix)
{
    intArray base;
    traverse(this->Q[0], base, suffix);
    return base;
}

// QUEUE
void Queue::enqueue(ParBFS data)
{
    if (this->empty() && capacity == 0 && array == nullptr)
    {
        capacity = 1;
        this->resize(capacity);
    }
    if (this->is_full())       // if array is completely full
        this->resize(capacity * 2);

    back = next(back);         // go to the next index
    array[back] = data;        // fill index with new element
    size_++;                   // adjust size_ attribute

    if (size_ == 1 && front < 0)
        front = back;          // special case
}

ParBFS Queue::dequeue()
{
    if (array == nullptr || this->empty())
        throw std::runtime_error("dequeue() method doesn't work because structure is empty\n");

    ParBFS rt_value = array[front];  // obtain value of front
    front = next(front);        // go to the next index
    size_--;                    // adjust size_ member attribute

    // estrategia para liberar espacios inutilizados
    if (size_ < (capacity/2))
        this->resize(capacity/2);
    return rt_value;
}

void Queue::resize(int new_capacity)
{
    ParBFS* new_array = new ParBFS[new_capacity];
    int traverse, i, aux_sz;

    // initialize "pointers" and accountant
    traverse = front, i = traverse % new_capacity, aux_sz = 0;
    this->front = i;    // reassign front indirection marker

    // copy numbers in new array
    while (aux_sz < size_)
    {
        new_array[i] = array[traverse];
        i = (i + 1) % new_capacity;
        traverse = next(traverse);
        aux_sz++;
    }
    delete[] array;     // release memory

    // reassing member attributes
    this->capacity = new_capacity;
    this->back = prev(i);
    this->array = new_array;
}

// INT ARRAY
intArray::intArray(int v){
    
    this->arr[sz++] = v;
}

void intArray::add(int el)
{
    // previous resizing
    if (this->sz == this->capacity)
        this->resize();
    // inserting (only unique values)
    for (int i = 0; i < this->sz; ++i)
        if (this->arr[i] == el)
            return;
    this->arr[sz++] = el;

    // sorting
    for (int i = 0; i < this->sz; ++i)
    {
        int idx = i;
        for (int j = i-1; j >= 0 ; --j)
        {
            if (this->arr[idx] < this->arr[j])
            {   // swapping
                auto s_aux = this->arr[idx];
                this->arr[idx] = this->arr[j];
                this->arr[j] = s_aux;
                idx--;
            }
            else break;
        }
    }
}

void intArray::resize()
{
    // table doubling
    this->capacity *= 2;
    auto aux_arr = new int[this->capacity]{};

    // copying into new array
    for (int i = 0; i < this->sz; ++i)
        aux_arr[i] = this->arr[i];
    
    // release memory and reassigning
    delete [] this->arr;
    this->arr = aux_arr;
}

void intArray::show()
{
    if (this->sz == 0)
        printf("[]");
    // else if (this->sz == 1)
    //     printf("%i", this->arr[0]);
    else
    {
        printf("[");
        for (int i = 0; i < this->sz-1; ++i)
            printf("%i ", this->arr[i]);
        printf("%i]", this->arr[this->sz-1]);
    }
}

intArray intArray::copy()
{
    intArray cp;
    cp.sz = this->sz;
    cp.capacity = this->capacity;
    cp.F_next = this->F_next;

    cp.arr = new int[cp.capacity];
    for (int i = 0; i < cp.sz; i++)
        cp.arr[i] = this->arr[i];
    
    return cp;
}

void intArray::merge(intArray other)
{
    for (int i = 0; i < other.sz; i++)
        this->add(other.arr[i]);
    this->F_next |= other.F_next;
}

bool intArray::contain(int other)
{
    for (int i = 0; i < this->sz; i++)
        if (this->arr[i] == other)
            return true;
    return false;
}

bool intArray::operator==(const intArray other)
{
    if (this->sz == other.sz)
    {
        for (int i = 0; i < other.sz; i++)
            if (this->arr[i] != other.arr[i])
                return false;
        return true;
    }
    return false;
}

bool intArray::operator!=(const intArray other)
{
    if (this->sz == other.sz)
    {
        for (int i = 0; i < other.sz; i++)
            if (this->arr[i] != other.arr[i])
                return true;
        return false;
    }
    return true;
}

// STATE ARRAY

void StateArray::add(State* st)
{
    if (this->sz == this->capacity)
        this->resize();
    this->arr[sz++] = st;
}

void StateArray::resize()
{
    // table doubling
    this->capacity *= 2;
    auto aux_arr = new State*[this->capacity]{};

    // copying into new array
    for (int i = 0; i < this->sz; ++i)
        aux_arr[i] = this->arr[i];
    
    // release memory and reassigning
    delete [] this->arr;
    this->arr = aux_arr;
}

//          GIC
// CHARSET
void charSet::add(char el)
{
    // previous resizing
    if (this->sz == this->capacity)
        this->resize();
    // inserting (only unique values)
    for (int i = 0; i < this->sz; ++i)
        if (this->arr[i] == el)
            return;
    this->arr[sz++] = el;

    // sorting
    for (int i = 0; i < this->sz; ++i)
    {
        int idx = i;
        for (int j = i-1; j >= 0 ; --j)
        {
            if (this->arr[idx] < this->arr[j])
            {   // swapping
                auto s_aux = this->arr[idx];
                this->arr[idx] = this->arr[j];
                this->arr[j] = s_aux;
                idx--;
            }
            else break;
        }
    }
}

void charSet::resize()
{
    // table doubling
    this->capacity *= 2;
    auto aux_arr = new char[this->capacity]{};

    // copying into new array
    for (int i = 0; i < this->sz; ++i)
        aux_arr[i] = this->arr[i];

    // release memory and reassigning
    delete [] this->arr;
    this->arr = aux_arr;
}

void charSet::show()
{
    if (this->sz == 0)
        printf("[]");
    else
    {
        printf("[");
        for (int i = 0; i < this->sz-1; ++i)
            printf("%i ", this->arr[i]);
        printf("%i]", this->arr[this->sz-1]);
    }
}

// CHAR QUEUE
void charQueue::enqueue(char data)
{
    if (this->empty() && capacity == 0 && array == nullptr)
    {
        capacity = 1;
        this->resize(capacity);
    }
    if (this->is_full())       // if array is completely full
        this->resize(capacity * 2);

    back = next(back);         // go to the next index
    array[back] = data;        // fill index with new element
    size_++;                   // adjust size_ attribute

    if (size_ == 1 && front < 0)
        front = back;          // special case
}

char charQueue::dequeue()
{
    if (array == nullptr || this->empty())
        throw std::runtime_error("dequeue() method doesn't work because structure is empty\n");

    char rt_value = array[front];  // obtain value of front
    front = next(front);        // go to the next index
    size_--;                    // adjust size_ member attribute

    // estrategia para liberar espacios inutilizados
    if (size_ < (capacity/2))
        this->resize(capacity/2);
    return rt_value;
}

void charQueue::resize(int new_capacity)
{
    char* new_array = new char[new_capacity];
    int traverse, i, aux_sz;

    // initialize "pointers" and accountant
    traverse = front, i = traverse % new_capacity, aux_sz = 0;
    this->front = i;    // reassign front indirection marker

    // copy numbers in new array
    while (aux_sz < size_)
    {
        new_array[i] = array[traverse];
        i = (i + 1) % new_capacity;
        traverse = next(traverse);
        aux_sz++;
    }
    delete[] array;     // release memory

    // reassing member attributes
    this->capacity = new_capacity;
    this->back = prev(i);
    this->array = new_array;
}

void charQueue::display()
{
    int traverse = this->front;
    while (traverse != this->back)
    {
        cout << array[traverse] << " -> ";
        traverse = this->next(traverse);
    }   cout << array[this->back] << "\n";
}
// RIGHT
void Right::add(char el, Rule *r)
{
    if (this->sz == this->capacity)
        this->resize();
    this->right[sz] = el;
    this->rules[sz] = r;
    sz++;
}

void Right::resize()
{
    // table doubling
    this->capacity *= 2;
    auto aux_right = new char[this->capacity]{};
    auto aux_rule = new Rule*[this->capacity]{};

    for (int i = 0; i < this->sz; ++i)
        aux_right[i] = this->right[i], aux_rule[i] = this->rules[i];

    // release memory and reassigning
    delete [] this->right;
    delete [] this->rules;
    this->right = aux_right;
    this->rules = aux_rule;
}

void Right::show()
{
    if (this->sz == 0)
    {
        printf("[]");
        return;
    }

    printf("[");
    for (size_t i = 0; i < this->sz-1; i++)
        printf("%c", this->right[i]);
    printf("%c]", this->right[this->sz-1]);
    printf("{%i}", this->count);
}

bool Right::is_generator()
{
    int count_generators = 0;
    for (size_t i = 0; i < this->sz; i++)
    {
        if (this->rules[i] != nullptr)
        {
            if (this->rules[i]->is_generator)
                count_generators++;
        }
        else
            count_generators++;
    }
    return count_generators == this->sz;
}

// RIGHT ARRAY
void RightArray::add(Right* rr)
{
    if (this->sz == this->capacity)
        this->resize();
    this->right[this->sz++] = rr;
}

void RightArray::resize()
{
    this->capacity *= 2;
    auto aux_arr = new Right*[this->capacity]{};
    for (int i = 0; i < this->sz; ++i)
        aux_arr[i] = this->right[i];

    delete [] this->right;
    this->right = aux_arr;
}

bool RightArray::generator()
{
    bool generator = false;
    for (int i = 0; i < this->sz; ++i)
        generator |= this->right[i]->is_generator();
    return generator;
}

void RightArray::display()
{
    for (int i = 0; i < this->sz; ++i)
        this->right[i]->show(), printf(" ");
}

// RULE
void Rule::add(Right* rr)
{
    rights.add(rr);
}

void Rule::display()
{
    printf("%c%c -> ", this->left,
           (this->is_generator ? '\'' : ' '));
    this->rights.display();
    printf("\n");
}

void Rule::update_generator()
{
    this->is_generator = rights.generator();
}

// GIC
GIC::GIC(string term, string var): terminales{term}, variables{var}
{
    for (char v : variables)
        this->rule_arr[int(v)-FIRST_UPPER] = new Rule(v);
}

void GIC::add_rule(char left, string right)
{
    Right* rr = new Right();
    for (char c: right)
    {
        Rule* next = nullptr;
        if (isupper(c))
            next = this->rule_arr[int(c)-FIRST_UPPER];
        rr->add(c, next);
        // cout << "(" << c << " " << next << ") ";
    }
    charSet chSet;
    for (char c: right)
    {
        if (isupper(c))
            chSet.add(c);
    }
    int cs_sz = chSet.sz;
    for (int i = 0; i < cs_sz; ++i)
        map->add(chSet.arr[i], rr);
    rr->count = chSet.sz;
    if (rr->count == 0)
    {
        map->is_gen[int(left) - FIRST_UPPER] = true;
        cq.enqueue(left);
    }
    this->rule_arr[int(left)-FIRST_UPPER]->add(rr);
}

void GIC::show()
{
    for (char v : variables)
        this->rule_arr[int(v)-FIRST_UPPER]->display();
}

bool GIC::empty_test()
{
    for (char vv : variables)
    {
        for (char v : variables)
            rule_arr[int(v) - FIRST_UPPER]->update_generator();
    }
    // this->show();
    return rule_arr[int(this->var_inicial) - FIRST_UPPER]->is_generator;
}

bool GIC::empty_test_n()
{
    // cout << "--------------------------------------------\n";
    while (!cq.empty())
    {
        char front = cq.dequeue();
        this->map->update(front);
        // this->map->show();
        // cout << "--------------------------------------------\n";
    }
    // this->show();
    return rule_arr[int(this->var_inicial) - FIRST_UPPER]->is_generator;
}

// MAP RIGHT
void MapRight::add(char k, Right* v)
{
    // cout << " ("<< k << " ";
    int idx = int(k)-FIRST_UPPER;
    // cout << idx << ") ";
    if (this->mapa[idx] == nullptr)
        this->mapa[idx] = new RightArray();
    this->mapa[idx]->add(v);
}

void MapRight::update(char front)
{
    int idx = int(front) - FIRST_UPPER;
    if (this->mapa[idx] == nullptr)
        return;
    int sz = this->mapa[idx]->sz;
    for (int i = 0; i < sz; ++i)
        this->mapa[idx]->right[i]->count--;
}

void MapRight::show()
{
    for (int i = 0; i < UPPER_SIZE; ++i)
    {
        if (this->mapa[i] != nullptr)
        {
            cout << char(i+FIRST_UPPER)<< (is_gen[i] ? "\'" : " ") << " -> ";
            this->mapa[i]->display();
            cout << endl;
        }
    }
}

