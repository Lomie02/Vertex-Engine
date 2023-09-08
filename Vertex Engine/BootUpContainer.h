#pragma once
enum TransparencySorting {
	BubbleSort = 0,
	Quick_Sort,
	Insertion_Sort,
};

struct BootUpContainer {
public:
	bool m_UseDefaultRenderer;
	bool m_EnableSingleSortForTension;
	bool m_AutoDeletePointers;
	TransparencySorting m_TransparentSortingAlgo = BubbleSort;
};