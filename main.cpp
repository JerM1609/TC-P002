#include "clases.h"

int main()
{
#ifndef TEST
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    // ios_base::sync_with_stdio(0); cin.tie(NULL);
    int t, q;
    string E;

    cin >> E;
    auto AFN = new Automata(E);

    scanf("%i", &t);
    auto T = new string[t];

    cin.ignore();
    for (int i=0; i<t; i++)
        getline(cin, T[i]);

    {
        timer_t timer;
        AFN->build_AFN(T, t);
    }

    printf("\tP1\n");
    // AFN->adjacency_list();

    scanf("%i", &q);
    cin.ignore();
    auto S = new string[q];
    for (int i=0; i<q; i++)
        getline(cin, S[i]);

    printf("\tP2\n");
    {
        timer_t timer;
        AFN->test(S, q);
    }

    printf("\tP3\n");
    Automata* AFD = AFN->transform_AFD();
    // AFD->adjacency_list();

    printf("\tP4\n");
    {
        timer_t timer;
        AFD->test(S, q);
    }
    return 0;
}