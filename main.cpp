#include "clases.h"

int main()
{
#ifndef TEST
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
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

    printf("\tP2\n");
    {
        timer_t timer;
        AFN->test(S, q);
    }
    
    printf("\tP3\n");
    Automata* AFD = nullptr;
    {
        timer_t timer;
        AFD = AFN->transform_AFD();
    }
    // AFD->adjacency_list();
    {
        timer_t timer;
        AFD->test(S, q);
    }

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

    printf("\tP5\n");
    printf("\tP6\n");
    return 0;
}
