#include <bits/stdc++.h> 
#include <chrono>
#include <iostream>
#include <vector>
#include <mutex>
#include <cstdlib>
#include <pthread.h> 
using namespace std; 
using namespace std::chrono;

vector<int> v;

void oddEvenSort(int n, int nw)
{
    int chunk = n / nw; // size of chunk
    int remainder = n % nw; 
    bool isSorted = false; // Initially array is unsorted

    vector<int> sorted(nw); // to check each chunck is sorted
    vector<int> border(nw); // to check border transposition
    vector<mutex> cnk_lock(nw); 

    int cnt = 0; // counting sorted chuncks
    mutex cnt_lock; // to protect shared cnt variable
    condition_variable cv; 

    vector<int> start_vec, end_vec;
    for (int i = 0; i < n; i += chunk) 
    {
        start_vec.push_back(i);
        if (remainder-- > 0) 
            i++;
        end_vec.push_back((i + chunk < n) ? (i + chunk) : (n - 1));
    }

    /* cout << "start_vec = ";
    for (int i = 0; i < chunk; i++) 
       cout << start_vec[i] << " "; 
    cout << "\n";

    cout << "end_vec = ";
    for (int i = 0; i < chunk; i++) 
       cout << end_vec[i] << " "; 
    cout << "\n"; */

    auto swapVec = [&](int tid) 
    {
        int start = start_vec[tid];
        int end = end_vec[tid];

        while (!isSorted) 
        {
            bool sorted_chunk = true; // if there is any transposition inside chunk --> (set false)
            cnk_lock[tid].lock();
            border[tid] = false;
            cnk_lock[tid].unlock();
            
            // ODD PHASE
            auto start_odd = high_resolution_clock::now();
            for (int i = start + 1; i < end; i += 2) 
            {
                // left side
                if (i == start && start != 0) 
                {
                    cnk_lock[tid].lock();
                    if (v[start] > v[start + 1]) 
                    {
                        swap(v[start], v[start + 1]);
                        sorted_chunk = false;
                        cnk_lock[tid - 1].lock();
                        border[tid - 1] = true;
                        if (sorted[tid - 1]) 
                        {
                            sorted[tid - 1] = false;
                            cnt_lock.lock();
                            --cnt;
                            cnt_lock.unlock();
                        }
                        cnk_lock[tid - 1].unlock();
                    }
                    cnk_lock[tid].unlock();
                    cout << "odd phase left code";
                }

                else 
                {
                    // right side
                    if (i == end - 1 && end != n - 1) 
                    {
                        cnk_lock[tid + 1].lock();
                        if (v[end] < v[end - 1]) 
                        {
                            swap(v[end], v[end - 1]);
                            sorted_chunk = false;
                            border[tid + 1] = true;
                            if (sorted[tid + 1]) 
                            {
                                sorted[tid + 1] = false;
                                cnt_lock.lock();
                                --cnt;
                                cnt_lock.unlock();
                            }
                        }
                        cnk_lock[tid + 1].unlock();
                    }

                    // internal
                    else 
                    {
                        if (v[i] > v[i + 1]) 
                        {
                            swap(v[i], v[i + 1]);
                            sorted_chunk = false;
                        }
                    }
                }
            }

            auto stop_odd = high_resolution_clock::now(); 
            auto duration_odd = duration_cast<nanoseconds>(stop_odd - start_odd);
            //cout << "Odd phase: " << duration_odd.count() << " nanoseconds" << " --- ";

            // EVEN PHASE
            auto start_even = high_resolution_clock::now();
            for (int i = start; i < end; i += 2) 
            {
                // left side
                if (i == start && start != 0) 
                {
                    cnk_lock[tid].lock();
                    if (v[start] > v[start + 1]) 
                    {
                        swap(v[start], v[start + 1]);
                        sorted_chunk = false;
                        cnk_lock[tid - 1].lock();
                        border[tid - 1] = true;
                        if (sorted[tid - 1]) 
                        {
                            sorted[tid - 1] = false;
                            cnt_lock.lock();
                            --cnt;
                            cnt_lock.unlock();
                        }
                        cnk_lock[tid - 1].unlock();
                    }
                    cnk_lock[tid].unlock();
                }

                else 
                {
                    // right side
                    if (i == end - 1 && end != n - 1) 
                    {
                        cnk_lock[tid + 1].lock();
                        if (v[end] < v[end - 1]) 
                        {
                            swap(v[end], v[end - 1]);
                            sorted_chunk = false;
                            border[tid + 1] = true;
                            if (sorted[tid + 1]) 
                            {
                                sorted[tid + 1] = false;
                                cnt_lock.lock();
                                --cnt;
                                cnt_lock.unlock();
                            }
                        }
                        cnk_lock[tid + 1].unlock();
                    }

                    // internal case
                    else 
                    {
                        if (v[i] > v[i + 1]) 
                        {
                            swap(v[i], v[i + 1]);
                            sorted_chunk = false;
                        }
                    }
                }
            }
            auto stop_even = high_resolution_clock::now(); 
            auto duration_even = duration_cast<nanoseconds>(stop_even - start_even);
            //cout << "Even phase: " << duration_even.count() << " nanoseconds" << endl;

            // set sorted thread
            if (sorted_chunk) 
            {
                cnk_lock[tid].lock();
                if (!border[tid] && !sorted[tid]) 
                {
                    sorted[tid] = true;
                    cnt_lock.lock();
                    ++cnt;
                    cnt_lock.unlock();
                    // send notification that the vector is sorted 
                    if (cnt == nw) 
                        cv.notify_one();
                }
                cnk_lock[tid].unlock();
            }
        }
    };

    auto start_t = high_resolution_clock::now();
    vector<thread*> tids(nw);
    for (int i = 0; i < nw; ++i)
	    tids[i] = new thread(swapVec, i);

    auto stop_t = high_resolution_clock::now(); 
    auto duration_t = duration_cast<nanoseconds>(stop_t - start_t);

    {
        unique_lock<mutex> lk(cnt_lock);
        cv.wait(lk, [&]{return cnt == nw;});
    }

    // terminate all threads
    isSorted = true; 
    
    for (int i = 0; i < nw; ++i) 
    {
        tids[i]->join();
        delete tids[i];
    }
    cout << "\nTime taken by Threads creation: " << duration_t.count() << " nanoseconds" << endl;
}   

void printVector(int n) 
{ 
   for (int i = 0; i < n; i++) 
       cout << v[i] << " "; 
   cout << "\n"; 
} 
 
int main(int argc, char** argv) 
{   
    if (argc < 4) 
    {
        cerr << "Enter: (vector-length) (nworkers) (seed)\n";
        return -1;
    }

    int n = stol(argv[1]);
	int nw = stol(argv[2]);
    int seed = stol(argv[3]);

    srand(seed);
    for(int i = 0; i < n; i++) 
        v.push_back((rand()%1000)+1);

    cout << "Given Array: ";
    printVector(n);

    auto start = high_resolution_clock::now();

    oddEvenSort(n, nw);

    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 

    cout << "\nSorted array is: "; 
    printVector(n);
    
    cout << "\nTime taken by function: " << duration.count() << " microseconds" << endl;

    return (0); 
} 