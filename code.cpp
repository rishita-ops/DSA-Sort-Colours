#include<bits/stdc++.h>
using namespace std;
int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    int nums[300];
    cout << "Enter elements (0, 1 or 2 only): ";
    for (int i = 0; i < n; i++) cin >> nums[i];

    int low = 0, mid = 0, high = n - 1;

    while (mid <= high) {
        if (nums[mid] == 0) {
            swap(nums[low], nums[mid]);
            low++; mid++;
        } else if (nums[mid] == 1) {
            mid++;
        } else {
            swap(nums[mid], nums[high]);
            high--;
        }
    }

    cout << "Sorted: ";
    for (int i = 0; i < n; i++) cout << nums[i] << " ";
    cout << endl;

    return 0;
}
