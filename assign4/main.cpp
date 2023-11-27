#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct DataEntry {
    std::string date;
    double value;
};

vector<DataEntry> readCSV(const string& filename);

void heapifyMax(vector<DataEntry> & arr, int n, int i);
void buildMaxHeap(vector<DataEntry> & arr);

void heapifyMin(vector<DataEntry> & arr, int n, int i);
void buildMinHeap(vector<DataEntry> & arr);

vector<DataEntry> getNMaxElements(vector<DataEntry> heap, int N);
vector<DataEntry> getNMinElements(std::vector<DataEntry> heap, int N);

double calculateAvgRate(vector<DataEntry> heap);
vector<DataEntry> calculateDifferenceM(vector<DataEntry> heap, double avg);
pair<int, int> maxSubsequenceSum(const vector<DataEntry>& nums);



int main() {
    vector<DataEntry> data = readCSV("/home/khalil/Desktop/courses/applied_data_structures/assign4/data.csv");

    vector<DataEntry> data_max_heap = data;
    buildMaxHeap(data_max_heap); //call by Reference

    vector<DataEntry> data_min_heap = data;
    buildMinHeap(data_min_heap);

    //Get max or min data values from the data set
    int N;

    cout<<"Could you enter the value of N (for max heap): ";
    cin>>N;
    vector<DataEntry> max_data = getNMaxElements(data_max_heap, N);

    cout<<"Could you enter the value of N (for min heap): ";
    cin>>N;
    vector<DataEntry> min_data = getNMinElements(data_min_heap, N);

    cout<<"Max values and dates"<<endl;
    for(int i = 0; i<max_data.size(); i++){
        cout<<max_data.at(i).date<< " --> "<< max_data.at(i).value<<"\n";
    }
    cout<<endl<<endl;
    cout<<"Min values and dates"<<endl;
    for(int i = 0; i<min_data.size(); i++){
        cout<<min_data.at(i).date<< " --> "<< min_data.at(i).value<<"\n";
    }

    cout<<endl<<endl;
    //start and end days of the contiguous period over which the sum of rate changes is maximum.
    double M = calculateAvgRate(data);
    vector<DataEntry> dataDiffM = calculateDifferenceM(data, M);

    auto [i, j] = maxSubsequenceSum(dataDiffM); //original data is passed here.
    cout<<"Start date of the contiguous period is : "<< dataDiffM.at(i).date<<endl;
    cout<<"End date of the contiguous period is : "<< dataDiffM.at(j).date<<endl;


    return 0;

}


//priority based on the value
void heapifyMax(vector<DataEntry>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].value > arr[largest].value) {
        largest = left;
    }

    if (right < n && arr[right].value > arr[largest].value) {
        largest = right;
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapifyMax(arr, n, largest);
    }
}

void buildMaxHeap(vector<DataEntry>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyMax(arr, n, i);
    }
}

//priority based on the value
void heapifyMin(vector<DataEntry>& arr, int n, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].value < arr[smallest].value) {
        smallest = left;
    }

    if (right < n && arr[right].value < arr[smallest].value) {
        smallest = right;
    }

    if (smallest != i) {
        swap(arr[i], arr[smallest]);
        heapifyMin(arr, n, smallest);
    }
}

void buildMinHeap(vector<DataEntry>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyMin(arr, n, i);
    }
}

//O(n)
pair<int, int> maxSubsequenceSum(const vector<DataEntry>& nums) {
    double max_so_far = nums[0].value;
    double max_ending_here = nums[0].value;
    int start = 0, end = 0, s = 0;

    for (int i = 1; i < nums.size(); i++) {
        if (nums[i].value > max_ending_here + nums[i].value) {
            max_ending_here = nums[i].value;
            s = i;
        } else {
            max_ending_here += nums[i].value;
        }

        if (max_ending_here > max_so_far) {
            max_so_far = max_ending_here;
            start = s;
            end = i;
        }
    }

    return {start, end}; // start and end index
}

vector<DataEntry> readCSV(const string& filename) {
    vector<DataEntry> data;
    ifstream file(filename);
    string line;

    // Check if the file is open
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // Read the header line
    if (file.good()) {
        std::getline(file, line); // Skip the first line containing headers
    }

    // Read data lines
    while (std::getline(file, line)) {
        std::istringstream s(line);
        std::string field;
        DataEntry entry;

        // Read date
        if (std::getline(s, field, ',')) {
            entry.date = field;
        }
        // Read value
        if (std::getline(s, field, ',')) {
            entry.value = std::stod(field);
        }

        data.push_back(entry);
    }

    file.close();
    return data;
}

vector<DataEntry> getNMaxElements(vector<DataEntry> heap, int N) {
    int heapSize = heap.size();
    vector<DataEntry> maxElements;

    for (int i = 0; i < N; ++i) {
        // Extract the root which is the max element
        maxElements.push_back(heap[0]);

        // Move the last element to root and reduce the size of heap
        heap[0] = heap[heapSize - 1];
        heapSize--;

        // Re-heapify the reduced heap
        heapifyMax(heap, heapSize, 0);
    }

    return maxElements;
}

vector<DataEntry> getNMinElements(std::vector<DataEntry> heap, int N) {
    int heapSize = heap.size();
    std::vector<DataEntry> minElements;

    for (int i = 0; i < N; ++i) {
        // Extract the root which is the min element
        minElements.push_back(heap[0]);

        // Move the last element to root and reduce the size of heap
        heap[0] = heap[heapSize - 1];
        heapSize--;

        // Re-heapify the reduced heap
        heapifyMin(heap, heapSize, 0);
    }

    return minElements;
}

double calculateAvgRate(vector<DataEntry> heap){
    double sum = 0;
    for(int i = 0; i<heap.size(); i++){sum+=heap.at(i).value;}
    return (sum/heap.size());
}

vector<DataEntry> calculateDifferenceM(vector<DataEntry> heap, double avg){
    vector<DataEntry> result = heap;
    for(int i = 0; i<heap.size(); i++){
        result.at(i).value =  result.at(i).value - avg;
    }
    return result;
}