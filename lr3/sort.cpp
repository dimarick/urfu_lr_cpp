#include <iostream>

using namespace std;

int main () {
    int array[] = {2, 5,  -8, 1, -4, 6, 3, -5, -9, 13, 0, 4, 9};
    int imin;
    int n = sizeof(array) / sizeof(*array);

    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }

    cout << endl;

    for (int i = 0; i < n - 1; i++) {
        imin = i;
        for (int j = i + 1; j < n; j++) {
            if (array[j] < array[imin]) {
                imin = j;
            }
        }

        int t = array[i];
        array[i] = array[imin];
        array[imin] = t;
    }

    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }

    cout << endl;
}