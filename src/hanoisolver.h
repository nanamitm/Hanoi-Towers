#pragma once

#include <vector>

struct Move {
    int disk = 0;
    int from = 0;
    int to = 0;
};

class HanoiSolver
{
public:
    static std::vector<Move> solve(int diskCount);

private:
    static void solveRecursive(int count, int from, int to, int spare, std::vector<Move> &moves);
};
