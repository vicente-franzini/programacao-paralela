void merge(int *arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int *leftarr = new int[n1];
    int *rightarr = new int[n2];

    for(int i = 0; i < n1; i++)
        leftarr[i] = arr[l + i];

    for(int j = 0; j < n2; j++)
        rightarr[j] = arr[m + 1 + j];

    int i = 0, j = 0;
    int k = l;

    while(i < n1 && j < n2) {
        if(leftarr[i] <= rightarr[j]) {
            arr[k] = leftarr[i];
            i++;
        } else {
            arr[k] = rightarr[j];
            j++;
        }

        k++;
    }

    while(i < n1) {
        arr[k] = leftarr[i];
        i++;
        k++;
    }

    while(j < n2) {
        arr[k] = rightarr[j];
        j++;
        k++;
    }

    delete[] leftarr;
    delete[] rightarr;

}

void mergesort(int *arr, int l, int r) {
    if(l >= r) return;

    int m = l + (r - l) / 2;
    mergesort(arr, l, m);
    mergesort(arr, m + 1, r);
    merge(arr, l, m, r);
}