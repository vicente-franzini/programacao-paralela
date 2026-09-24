#include <cmath>

void shellsort(int *vec, int min, int max) {
    // Comece com uma distância entre trocas grande. Itere
    // múltiplas vezes até a distância entre elementos ordenados
    // ser igual a 0 (o vetor está ordenado).
    for(int dist = log2(max - min) - 1; dist > 0; dist /= 2) {
        // Para cada item da lista, faça uma ordenação de Insertion
        // Sort, só que cada par de itens analisados tem distância
        // "dist" ao invés de 1.
        for(int i = dist + min; i <= max; i++) {
            int atual = vec[i];
            int j = i;

            for(;j >= dist + min && vec[j - dist] > atual; j -= dist)
                vec[j] = vec[j - dist];
            
            vec[j] = atual;
        }
    }
}