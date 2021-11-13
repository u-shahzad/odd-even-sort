#include <bits/stdc++.h> 
#include <iostream>
#include <chrono>
using namespace std; 
using namespace std::chrono;

vector<int> v;

void oddEvenSort(int n)
{ 
    bool isSorted = false; // Initially array is unsorted 
  
    while (!isSorted)
    {
        isSorted = true; 
  
        // ODD PHASE
        auto start_odd = high_resolution_clock::now();
        for (int i=1; i<=n-2; i=i+2)
        {
            if (v[i] > v[i+1])
            {
                swap(v[i], v[i+1]); 
                isSorted = false; 
            } 
        } 

        auto stop_odd = high_resolution_clock::now(); 
        auto duration_odd = duration_cast<nanoseconds>(stop_odd - start_odd); 
        cout << "Time taken by odd phase : " << duration_odd.count() << " nanoseconds" << endl;

        /*cout << "--> ";
        for (int i=0; i < n; i++) 
            cout << v[i] << " "; 
        cout << "\n\n"; */
  
        // EVEN PHASE 
        auto start_even = high_resolution_clock::now();
        for (int i=0; i<=n-2; i=i+2)
        {
            if (v[i] > v[i+1])
            {
                swap(v[i], v[i+1]); 
                isSorted = false; 
            } 
        }
        auto stop_even = high_resolution_clock::now(); 
        auto duration_even = duration_cast<nanoseconds>(stop_even - start_even); 
        cout << "Time taken by even phase: " << duration_even.count() << " nanoseconds" << endl; 

        /*cout << "--> ";
        for (int i=0; i < n; i++) 
            cout << v[i] << " "; 
        cout << "\n\n"; */
    } 
  
    return; 
} 

void printVector(int n)
{
    for (int i=0; i < n; i++) 
        cout << v[i] << " "; 
    cout << "\n"; 
} 

int main(int argc, char** argv)
{
    if (argc < 2) 
    {
        cerr << "Enter: nworkers \n";
        return -1;
    }

    int n = stoi(argv[1]); 

    srand((unsigned)time(0));
    for(int i=0; i<n; i++) 
        v.push_back((rand()%100)+1);

    cout << "\nGIVEN VECTOR --> "; 
    printVector(n);
    cout << endl;

    auto start = high_resolution_clock::now();

    oddEvenSort(n);

    auto stop = high_resolution_clock::now();

    cout << "\nSORTED VECTOR -->  "; 
    printVector(n);
 
    auto duration = duration_cast<microseconds>(stop - start); 
    cout << "\nTotal time: " << duration.count() << " microseconds" << endl;
    
    return (0); 
} 

