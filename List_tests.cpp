#include "List.hpp"
#include "unit_test_framework.hpp"

using namespace std;

// Add your test cases here

// ========== BASIC LIST TESTS ==========

TEST(test_default_constructor) {
    List<int> l;
    ASSERT_TRUE(l.empty());
    ASSERT_EQUAL(0, l.size());
}

TEST(test_push_back_single) {
    List<int> l;
    l.push_back(42);
    ASSERT_FALSE(l.empty());
    ASSERT_EQUAL(1, l.size());
    ASSERT_EQUAL(42, l.front());
    ASSERT_EQUAL(42, l.back());
}

TEST(test_push_front_single) {
    List<int> l;
    l.push_front(99);
    ASSERT_FALSE(l.empty());
    ASSERT_EQUAL(1, l.size());
    ASSERT_EQUAL(99, l.front());
    ASSERT_EQUAL(99, l.back());
}

TEST(test_push_back_multiple) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    ASSERT_EQUAL(3, l.size());
    ASSERT_EQUAL(1, l.front());
    ASSERT_EQUAL(3, l.back());
}

TEST(test_push_front_multiple) {
    List<int> l;
    l.push_front(3);
    l.push_front(2);
    l.push_front(1);
    ASSERT_EQUAL(3, l.size());
    ASSERT_EQUAL(1, l.front());
    ASSERT_EQUAL(3, l.back());
}

TEST(test_push_mixed) {
    List<int> l;
    l.push_back(2);
    l.push_front(1);
    l.push_back(3);
    ASSERT_EQUAL(3, l.size());
    ASSERT_EQUAL(1, l.front());
    ASSERT_EQUAL(3, l.back());
}

TEST(test_pop_back_single) {
    List<int> l;
    l.push_back(42);
    l.pop_back();
    ASSERT_TRUE(l.empty());
    ASSERT_EQUAL(0, l.size());
}

TEST(test_pop_front_single) {
    List<int> l;
    l.push_front(99);
    l.pop_front();
    ASSERT_TRUE(l.empty());
    ASSERT_EQUAL(0, l.size());
}

TEST(test_pop_back_multiple) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.pop_back();
    ASSERT_EQUAL(2, l.size());
    ASSERT_EQUAL(2, l.back());
    l.pop_back();
    ASSERT_EQUAL(1, l.size());
    ASSERT_EQUAL(1, l.back());
}

TEST(test_pop_front_multiple) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.pop_front();
    ASSERT_EQUAL(2, l.size());
    ASSERT_EQUAL(2, l.front());
    l.pop_front();
    ASSERT_EQUAL(1, l.size());
    ASSERT_EQUAL(3, l.front());
}

TEST(test_front_back_modification) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.front() = 10;
    l.back() = 20;
    ASSERT_EQUAL(10, l.front());
    ASSERT_EQUAL(20, l.back());
}

TEST(test_clear_empty) {
    List<int> l;
    l.clear();
    ASSERT_TRUE(l.empty());
    ASSERT_EQUAL(0, l.size());
}

TEST(test_clear_nonempty) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.clear();
    ASSERT_TRUE(l.empty());
    ASSERT_EQUAL(0, l.size());
}

TEST(test_copy_constructor_empty) {
    List<int> l1;
    List<int> l2(l1);
    ASSERT_TRUE(l2.empty());
    ASSERT_EQUAL(0, l2.size());
}

TEST(test_copy_constructor_nonempty) {
    List<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    List<int> l2(l1);
    ASSERT_EQUAL(3, l2.size());
    ASSERT_EQUAL(1, l2.front());
    ASSERT_EQUAL(3, l2.back());
    // Verify deep copy
    l1.front() = 99;
    ASSERT_EQUAL(1, l2.front());
}

TEST(test_assignment_operator_empty) {
    List<int> l1;
    List<int> l2;
    l2.push_back(1);
    l2 = l1;
    ASSERT_TRUE(l2.empty());
    ASSERT_EQUAL(0, l2.size());
}

TEST(test_assignment_operator_nonempty) {
    List<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    List<int> l2;
    l2 = l1;
    ASSERT_EQUAL(2, l2.size());
    ASSERT_EQUAL(1, l2.front());
    ASSERT_EQUAL(2, l2.back());
    // Verify deep copy
    l1.front() = 99;
    ASSERT_EQUAL(1, l2.front());
}

TEST(test_assignment_self) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    List<int> &ref = l;
    l = ref;
    ASSERT_EQUAL(2, l.size());
    ASSERT_EQUAL(1, l.front());
    ASSERT_EQUAL(2, l.back());
}

// ========== ITERATOR TESTS ==========

TEST(test_iterator_default_constructor) {
    List<int>::Iterator it;
    List<int>::Iterator it2;
    ASSERT_TRUE(it == it2);
}

TEST(test_begin_end_empty) {
    List<int> l;
    ASSERT_TRUE(l.begin() == l.end());
}

TEST(test_begin_end_nonempty) {
    List<int> l;
    l.push_back(1);
    ASSERT_FALSE(l.begin() == l.end());
}

TEST(test_iterator_dereference) {
    List<int> l;
    l.push_back(42);
    auto it = l.begin();
    ASSERT_EQUAL(42, *it);
}

TEST(test_iterator_arrow) {
    struct Point { int x, y; };
    List<Point> l;
    Point p = {3, 4};
    l.push_back(p);
    auto it = l.begin();
    ASSERT_EQUAL(3, it->x);
    ASSERT_EQUAL(4, it->y);
}

TEST(test_iterator_prefix_increment) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    auto it = l.begin();
    ++it;
    ASSERT_EQUAL(2, *it);
}

TEST(test_iterator_postfix_increment) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    auto it = l.begin();
    auto old_it = it++;
    ASSERT_EQUAL(1, *old_it);
    ASSERT_EQUAL(2, *it);
}

TEST(test_iterator_prefix_decrement) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    auto it = l.end();
    --it;
    ASSERT_EQUAL(2, *it);
}

TEST(test_iterator_postfix_decrement) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    auto it = l.end();
    auto old_it = it--;
    ASSERT_TRUE(old_it == l.end());
    ASSERT_EQUAL(2, *it);
}

TEST(test_iterator_equality) {
    List<int> l;
    l.push_back(1);
    auto it1 = l.begin();
    auto it2 = l.begin();
    ASSERT_TRUE(it1 == it2);
}

TEST(test_iterator_inequality) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    auto it1 = l.begin();
    auto it2 = l.begin();
    ++it2;
    ASSERT_TRUE(it1 != it2);
}

TEST(test_iterator_copy_constructor) {
    List<int> l;
    l.push_back(42);
    auto it1 = l.begin();
    auto it2(it1);
    ASSERT_TRUE(it1 == it2);
    ASSERT_EQUAL(42, *it2);
}

TEST(test_iterator_assignment) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    auto it1 = l.begin();
    auto it2 = l.end();
    it2 = it1;
    ASSERT_TRUE(it1 == it2);
}

TEST(test_iterator_traversal) {
    List<int> l;
    for (int i = 0; i < 5; ++i) l.push_back(i);
    int expected = 0;
    for (auto it = l.begin(); it != l.end(); ++it) {
        ASSERT_EQUAL(expected++, *it);
    }
}

TEST(test_iterator_modification) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    auto it = l.begin();
    ++it;
    *it = 42;
    ASSERT_EQUAL(42, *it);
    ASSERT_EQUAL(1, l.front());
    ASSERT_EQUAL(3, l.back());
}

TEST(test_range_based_for) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    int sum = 0;
    for (int val : l) {
        sum += val;
    }
    ASSERT_EQUAL(6, sum);
}

TEST(test_const_iterator) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    const List<int>& cl = l;
    auto it = cl.begin();
    ASSERT_EQUAL(1, *it);
    ++it;
    ASSERT_EQUAL(2, *it);
}

// ========== ERASE TESTS ==========

TEST(test_erase_only_element) {
    List<int> l;
    l.push_back(42);
    auto it = l.begin();
    auto next = l.erase(it);
    ASSERT_TRUE(l.empty());
    ASSERT_TRUE(next == l.end());
}

TEST(test_erase_first) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    auto it = l.erase(l.begin());
    ASSERT_EQUAL(2, l.size());
    ASSERT_EQUAL(2, l.front());
    ASSERT_EQUAL(2, *it);
}

TEST(test_erase_last) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    auto it = l.begin();
    ++it; ++it;
    auto next = l.erase(it);
    ASSERT_EQUAL(2, l.size());
    ASSERT_EQUAL(2, l.back());
    ASSERT_TRUE(next == l.end());
}

TEST(test_erase_middle) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    auto it = l.begin();
    ++it;
    auto next = l.erase(it);
    ASSERT_EQUAL(2, l.size());
    ASSERT_EQUAL(3, *next);
}

// ========== INSERT TESTS ==========

TEST(test_insert_at_begin) {
    List<int> l;
    l.push_back(2);
    l.push_back(3);
    auto it = l.insert(l.begin(), 1);
    ASSERT_EQUAL(3, l.size());
    ASSERT_EQUAL(1, l.front());
    ASSERT_EQUAL(1, *it);
}

TEST(test_insert_at_end) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    auto it = l.insert(l.end(), 3);
    ASSERT_EQUAL(3, l.size());
    ASSERT_EQUAL(3, l.back());
    ASSERT_EQUAL(3, *it);
}

TEST(test_insert_in_middle) {
    List<int> l;
    l.push_back(1);
    l.push_back(3);
    auto it = l.begin();
    ++it;
    auto new_it = l.insert(it, 2);
    ASSERT_EQUAL(3, l.size());
    ASSERT_EQUAL(2, *new_it);
}

TEST(test_insert_empty_list) {
    List<int> l;
    auto it = l.insert(l.begin(), 42);
    ASSERT_EQUAL(1, l.size());
    ASSERT_EQUAL(42, l.front());
    ASSERT_EQUAL(42, *it);
}

TEST_MAIN()
