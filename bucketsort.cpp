#pragma once

#include <vector>
#include "bucketsort.h"

#include <iostream>
#include <algorithm>

/**
 * Constructor for a bucketsort object
 * @param mode:
 * mode == 0 == sort highest to lowest  DONE
 * mode == 1 == sort lowest to highest DONE
 */
BucketSortInt::BucketSortInt(int mode) {
    this->mode = mode;
}

/**
 * Insertion sort for invidual buckets
 * @param bucket
 */
void BucketSortInt::insertion_sort_buckets(std::vector<int>& bucket) {
    for (int i = 1; i < bucket.size(); i++) {
        int item = bucket[i];
        int j = i - 1;
        if (mode == 0) {
            while (j >= 0 && bucket[j] < item) { // Changed from bucket[j] > item
                bucket[j + 1] = bucket[j];
                j--;
            }
        } else {
            while (j >= 0 && bucket[j] > item) { // Changed from bucket[j] < item
                bucket[j + 1] = bucket[j];
                j--;
            }
        }
        bucket[j + 1] = item;
    }
}

/**
 * Display the buckets
 * Used primarily for testing purposes
 * @param buckets
 * @param num_buckets
 */
void BucketSortInt::print_buckets(std::vector<int> buckets[], int num_buckets) {
    for (int i = 0; i < num_buckets; i++) {
        std::cout << "Bucket " << i + 1 << ": ";
        for (int val : buckets[i]) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * Main bucket sort algorithm
 * @param orders
 * @param num_buckets
 */
void BucketSortInt::bucket_sort(std::vector<int>& numbers, int num_buckets) {
    std::vector<std::vector<int> > buckets(num_buckets);

    // Find the minimum and maximum values in the vector of numbers
    int minId = *std::min_element(numbers.begin(), numbers.end());
    int maxId = *std::max_element(numbers.begin(), numbers.end());

    // Distribute the numbers into buckets
    for (const auto& num : numbers) {
        int bucket_index = (num - minId) * num_buckets / (maxId - minId + 1);
        buckets[bucket_index].push_back(num);
    }

    // Clear numbers so we can fill it with sorted numbers
    numbers.clear();

    // Sort each bucket and merge the sorted buckets based on mode
    if (mode == 0) {
        for (int i = num_buckets - 1; i >= 0; i--) {
            insertion_sort_buckets(buckets[i]);
            numbers.insert(numbers.end(), buckets[i].begin(), buckets[i].end());
        }
    } else {
        for (auto& bucket : buckets) {
            insertion_sort_buckets(bucket);
            numbers.insert(numbers.end(), bucket.begin(), bucket.end());
        }
    }
}