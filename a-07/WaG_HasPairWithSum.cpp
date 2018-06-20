#include <unordered_set>
#include <vector>


bool HasPairWithSum(const std::vector<int> data, int sum) {
	std::unordered_set<int> comp; //complements
	for (int value : data) {
		if (comp.find(value) != comp.end)
			return true;
		comp.insert(sum - value);
	}
	return false;
}


template <class C, class V>
decltype(auto) summandsOf(C && summands, V && sum) {
	std::unordered_set<int> comp; //complements
// I kind of like the idea. But I think this is harder to parallelize.
//   -> one shared set leads to a lot of comm + locking...
// hmmm back to sorting?

// views/filter? ---> only (values <= sum)
// ----> use subranges
}