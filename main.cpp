#include "clases.h"

void automatas()
{
    int t, q;
    string E;

    cin >> E;
    auto AFN = new Automata(E);

    scanf("%i", &t);
    auto T = new string[t];

    cin.ignore();
    for (int i=0; i<t; i++)
        getline(cin, T[i]);

    scanf("%i", &q);
    cin.ignore();
    auto S = new string[q];
    for (int i=0; i<q; i++)
        getline(cin, S[i]);


    printf("\tP1\n");
    {
        timer_t timer;
        AFN->build_AFN(T, t);
    }
    // AFN->adjacency_list();

    // printf("\tP2\n");
    // {
    //     timer_t timer;
    //     AFN->test(S, q);
    // }
    
    // printf("\tP3\n");
    // Automata* AFD = nullptr;
    // {
    //     timer_t timer;
    //     AFD = AFN->transform_AFD();
    // }
    // AFD->adjacency_list();
    // {
    //     timer_t timer;
    //     AFD->test(S, q);
    // }

    printf("\tP4\n");
    Automata* AFD_AFN = nullptr;
    {
        timer_t timer;
        AFD_AFN = AFN->transform_AFD_2();
    }
    // AFD_AFN->adjacency_list();
    {
        timer_t timer;
        AFD_AFN->test(S, q);
    }
}

void gramaticas()
{
    string terminales, variables;
    char var; string rule;
    int r;

    cin >> terminales >> variables >> r;
    GIC* gic = new GIC(terminales, variables);
    for (int i = 0; i < r; i++)
    {
        cin >> var >> rule;
        if (i == 0)
            gic->var_inicial = var;
        gic->add_rule(var, rule);

        /*
        cout << "--------------------------------\n";
        cout << "\tinserting {" << var << ", " << rule <<"}\n";
        gic->show();
        cout << "--------------------------------\n";
        */
    }   cout << endl;

    // gic->show();

    printf("\tP5\n");
    {
        timer_t t;
        cout << ((!gic->empty_test()) ? "YES\n" : "NO\n");
    }

    // gic->show();

    printf("\tP6\n");
    {
        timer_t t;
        gic->map->show();
        gic->cq.display();
        cout << ((!gic->empty_test_n()) ? "YES\n" : "NO\n");
    }
    
}

int main()
{
#ifndef TEST
    freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
#endif
    // automatas();
    gramaticas();
    return 0;
}
