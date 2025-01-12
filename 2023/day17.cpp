#include <string>
#include <vector>
#include <queue>
#include "days.hpp"
#include "status.hpp"
#include "util.hpp"

using std::string;

// node attributes:
// x and y
// direction (4 states, crucible cannot go backwards)
// moves left (10 states, when this is 0 crucible cannot go forwards)

class node {
    public:
    int x, y;
    char dir;
    char moves;
    unsigned short int cost;
    const node *parent;

    node() {}

    node(const node *parent): node(*parent) {
        this->parent = parent;
    }
};

template<>
struct std::less<node *> {
    bool operator() (const node *a, const node *b) const {
        return a->cost > b->cost;
    }
};

int day_17(const string &input, string *output, bool part2) {
    std::vector<string> map;
    split(input, "\n", &map);

    int shortest = -1;
    const node *final = nullptr;

    uint64_t *closed = (uint64_t *)calloc(map.size() * map[0].size(), sizeof(uint64_t));
    std::priority_queue<node *> open;

    node *first = new node();
    first->dir = 1;
    first->moves = part2 ? 10 : 3;
    open.push(first);

    node *second = new node();
    second->dir = 2;
    second->moves = part2 ? 10 : 3;
    open.push(second);

    while (!open.empty()) {
        node *next = open.top();
        open.pop();

        size_t idx = next->x * map.size() + next->y;
        uint64_t sidx = 1 << (next->moves * 4 + next->dir);
        if ((closed[idx] & sidx) != 0) {
            // std::cout << "already explored: " << next->x << ", " << next->y << " dir " << (int)next->dir << " moves " << (int)next->moves << std::endl;
            delete next;
            continue; // node already visited
        }
        // std::cout << "exploring: " << next->x << ", " << next->y << " dir " << (int)next->dir << " moves " << (int)next->moves << std::endl;
        closed[idx] |= sidx;

        if (next->y == map.size() - 1 && next->x == map[0].size() - 1 && next->moves <= 6) {
            shortest = next->cost;
            final = next;
            break; // reached the goal
        }

        if (next->moves <= 6) {
            node *left = new node(next);
            left->dir = (left->dir + 3) % 4;
            left->moves = part2 ? 9 : 2;
            move(left->dir, &left->x, &left->y);
            if (left->y >= 0 && left->x >= 0 && left->y < map.size() && left->x < map[0].size()) {
                left->cost += map[left->y][left->x] - '0';
                open.push(left);
            } else {
                delete left; // out of bounds
            }

            node *right = new node(next);
            right->dir = (right->dir + 1) % 4;
            right->moves = part2 ? 9 : 2;
            move(right->dir, &right->x, &right->y);
            if (right->y >= 0 && right->x >= 0 && right->y < map.size() && right->x < map[0].size()) {
                right->cost += map[right->y][right->x] - '0';
                open.push(right);
            } else {
                delete right; // out of bounds
            }
        }

        if (next->moves > 0) {
            node *straight = new node(next);
            straight->moves--;
            move(straight->dir, &straight->x, &straight->y);
            if (straight->y >= 0 && straight->x >= 0 && straight->y < map.size() && straight->x < map[0].size()) {
                straight->cost += map[straight->y][straight->x] - '0';
                open.push(straight);
            } else {
                delete straight; // out of bounds
            }
        }

        delete next;
    }
    while (!open.empty()) {
        delete open.top();
        open.pop();
    }
    free(closed);

    // while (final != nullptr) {
    //     map[final->y][final->x] = '.';
    //     final = final->parent;
    // }
    // for (int i = 0; i < map.size(); i++) {
    //     std::cout << map[i] << std::endl;;
    // }
    
    if (shortest == -1) {
        return ERR_NO_SOLUTION;
    }

    *output = std::to_string(shortest);
    return SUCCESS;
}
