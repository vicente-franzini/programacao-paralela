#include <bits/stdc++.h>

using namespace std;

int main() {
    int64_t values[512];
    for(int i = 0; i < 512; i++) cin >> values[i];

    std::sort(&values[0], &values[511]);

    long_double_t mean = 0, variance = 0;
    long_double_t ly = round((double) 512 * (1.0 - 0.1));
    long_double_t n = 0;

    for(int r = floor((double) 0 * (0.1 / 2.0)); r < ly; r++) { 
        n++;
        mean += (long_double_t) values[r] / 1000.0l;
    }

    mean /= n;
    n--;

    for(int r = 0; r < ly; r++) {
        variance += powl(((long_double_t) values[r] / 1000.0l) - mean, 2) / n;
    }

    long_double_t a = ((mean * mean) / (variance * variance));
    long_double_t b = (variance * variance) / mean;

    cout << a << ' ' << b << '\n';

    if(a > 1) {
        cout << ((a - 1) * b);
    } else {
        cout << mean << '\n';
        cout << 'b';
    }

    cout << endl;
    return 0;
}