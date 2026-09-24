#pragma once

void insertion(int *vec, int min, int max) {
    for(int i = min + 1; i <= max; i++) {
        int atual = vec[i];
        int j = i - 1;

        while(j >= min && atual < vec[j]) {
            vec[j + 1] = vec[j];
            j -= 1;
        }

        vec[j + 1] = atual;
    }
}