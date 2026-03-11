import heapq
import sys

# A multiset with find_min operation.
#
# Elements will be sorted according to their natural ordering
# using the < comparison operator.
class SortedMultiset:

    def __init__(self):
        self.elem_heap   = []   # min-first element heap (single copy)
        self.elem_counts = {}   # element counters (0..)

    # Invariant:
    #   heap contents == dict keys

    def __bool__(self):
        self._delete_phantom_elements()
        return bool(self.elem_heap)

    # Deletes non-existing elements from the beginning of the heap
    def _delete_phantom_elements(self):
        while self.elem_heap:
            x = self.elem_heap[0]
            if 0 == self.elem_counts[x]:
                # x already deleted
                heapq.heappop(self.elem_heap)
                del self.elem_counts[x]
            else:
                break

    def _raise_if_empty(self):
        if not self.elem_heap:
            raise ValueError("collection is empty")

    def insert(self, x):
        if x in self.elem_counts:
            self.elem_counts[x] += 1
        else:
            heapq.heappush(self.elem_heap, x)
            self.elem_counts[x] = 1

    def count(self, x):
        return self.elem_counts.get(x,0)
    def contains(self, x):
        return 1 <= self.count(x)

    def find_min(self):
        self._delete_phantom_elements()
        self._raise_if_empty()
        return self.elem_heap[0]

    def delete_min(self):
        self._delete_phantom_elements()
        self._raise_if_empty()
        x = self.elem_heap[0]
        self.elem_counts[x] -= 1
        return x

    def delete(self, x):
        self._delete_phantom_elements()
        if (x in self.elem_counts) and (1 <= self.elem_counts[x]):
            self.elem_counts[x] -= 1
        else:
            raise KeyError(x)

buildings = SortedMultiset()    # currently open building heights (negative)
h_right   = []                  # min-heap of pairs (right end, height)
skyline   = []                  # computed skyline of pairs (x, height)

# Given a max building's right end, decreses the current skyline to the next
# available max height.
def skyline_drop_down(right):
    height = 0 - buildings.find_min() if buildings else 0

    if skyline[-1][0] < right:
        skyline.append((right, height))
        # print("skyline drop A: r={:4} h={:4}".format(right, height))
    else:
        skyline[-1] = skyline[-1][0], min(height, skyline[-1][1])
        # print("skyline drop U: r={:4} h={:4}".format(right, skyline[-1][1]))

# Adds a new building to skyline
def skyline_bump_up(left, height):
    if skyline and left == skyline[-1][0]:
        skyline[-1] = skyline[-1][0], max(height, skyline[-1][1])
        # print("skyline bump U: l={:4} h={:4}".format(left, skyline[-1][1]))
    elif not skyline or skyline[-1][1] < height:
        skyline.append((left, height))
        # print("skyline bump A: l={:4} h={:4}".format(left, height))

def close_buildings_up_to(x):
    while h_right and h_right[0][0] <= x:
        orr  = h_right[0][0]                        # old building's right edge
        oh   = h_right[0][1]                        # old building's height
        hmax = 0 - buildings.find_min()             # max height
        # print(f"orr={orr} oh={oh} hmax={hmax}")
        buildings.delete(-oh)                       # close the building

        if oh == hmax:                              # this was the max height
            if not buildings.contains(-oh):         # this building was the last one
                skyline_drop_down(orr)              # update the skyline

        heapq.heappop(h_right)

for input_line in sys.stdin:
    input_building = input_line.split()

    if input_building:
        nl = int(input_building[0])
        nh = int(input_building[1])
        nr = int(input_building[2])

        # Finalize buildings to the left of this new one
        close_buildings_up_to(nl)

        # add the new building
        buildings.insert(-nh)
        heapq.heappush(h_right, (nr,nh))
        skyline_bump_up(nl,nh)

# Finalize all buildings
close_buildings_up_to(sys.maxsize)

if skyline:
    skyline = [x for s2 in skyline for x in s2]
    print(" ".join(map(str,skyline)))

