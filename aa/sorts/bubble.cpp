void bubblesort(int *vec, int min, int max) {
    for(int lim = max; lim >= min + 1; lim--) {
//      bool trocou = false;
        for(int i = 1; i <= lim; i++) {
            if(vec[i] > vec[i - 1]) continue;

            int tmp = vec[i];
            vec[i] = vec[i - 1];
            vec[i - 1] = tmp;
            
//          trocou = true;
        }

//      if(!trocou) return;
    }
}