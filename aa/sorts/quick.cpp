#include "insertion.cpp"

void quicksort(int *vec, int min, int max) {
    // Caso o subvetor analisado tenha tamanho 1 ou inválido, retorne.
    if(min >= max) return;

    // Obtém o valor do pivô para o subvetor analisado. Nesse caso, o
    // pivô é sempre o valor do meio do subvetor analisado.
    int pivo_i = (max - min) / 2 + min;
    int pivo = vec[pivo_i];

    // Instancia um segundo vetor onde a partição ocorrerá. Ele já é
    // instanciado com tamanho para melhorar a performance.
    int *_vec = new int[max - min + 1];

    // Para cada valor do subvetor analisado, verifique se ele é maior
    // ou menor que o pivô, e coloque-o no fim ou começo do vetor
    // partição.
    int is = 0, ie = max - min;
    for(int i = min; i <= max; i++) {
        if(i == pivo_i) continue;

        if(vec[i] < pivo) {
            _vec[is] = vec[i];
            is++;
        } else {
            _vec[ie] = vec[i];
            ie--;
        }
    }

    // Insira o pivô no vetor partição.
    _vec[is] = pivo;

    // Troque o índice do pivô após a partição.
    pivo_i = is + min;

    // Copie o vetor partição ao vetor principal.
    for(int i = 0; i <= max - min; i++)
        vec[i + min] = _vec[i];

    // Deleta o vetor instanciado
    delete[] _vec;

    // Ordene as duas metades divididas pelo pivô.
    if(pivo_i != 0) quicksort(vec, min, pivo_i - 1);
    quicksort(vec, pivo_i + 1, max);
}


void quicksort(int *vec, int min, int max, int qs_limiar) {
    // Caso o subvetor analisado tenha tamanho 1 ou inválido, retorne.
    if(min >= max) return;

    // Caso o tamanho do subvetor analisado seja menor que o limiar,
    // use Insertion Sort (que é mais rápido em vetores pequenos).
    if(max - min <= qs_limiar) {
        insertion(vec, min, max);
        return;
    }

    // Obtém o valor do pivô para o subvetor analisado. Nesse caso, o
    // pivô é sempre o valor do meio do subvetor analisado.
    int pivo_i = (max - min) / 2 + min;
    int pivo = vec[pivo_i];

    // Instancia um segundo vetor onde a partição ocorrerá. Ele já é
    // instanciado com tamanho para melhorar a performance.
    int *_vec = new int[max - min + 1];

    // Para cada valor do subvetor analisado, verifique se ele é maior
    // ou menor que o pivô, e coloque-o no fim ou começo do vetor
    // partição.
    int is = 0, ie = max - min;
    for(int i = min; i <= max; i++) {
        if(i == pivo_i) continue;

        if(vec[i] < pivo) {
            _vec[is] = vec[i];
            is++;
        } else {
            _vec[ie] = vec[i];
            ie--;
        }
    }

    // Insira o pivô no vetor partição.
    _vec[is] = pivo;

    // Troque o índice do pivô após a partição.
    pivo_i = is + min;

    // Copie o vetor partição ao vetor principal.
    for(int i = 0; i <= max - min; i++)
        vec[i + min] = _vec[i];

    // Deleta o vetor instanciado
    delete[] _vec;

    // Ordene as duas metades divididas pelo pivô.
    if(pivo_i != 0) quicksort(vec, min, pivo_i - 1, qs_limiar);
    quicksort(vec, pivo_i + 1, max, qs_limiar);
}