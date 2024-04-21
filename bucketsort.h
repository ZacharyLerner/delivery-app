#include <vector>

class BucketSortInt {

private:
    int mode;

public:
    BucketSortInt(int mode);

    void insertion_sort_buckets(std::vector<int>& bucket);
    void print_buckets(std::vector<int> buckets[], int num_buckets);
    void bucket_sort(std::vector<int>& numbers, int num_buckets);
};