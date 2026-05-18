#include "hanoisolver.h"

std::vector<Move> HanoiSolver::solve(int diskCount)
{
    std::vector<Move> moves;
    if (diskCount <= 0) {
        return moves;
    }

    moves.reserve((1ULL << diskCount) - 1ULL);
    solveRecursive(diskCount, 0, 2, 1, moves);
    return moves;
}

void HanoiSolver::solveRecursive(int count, int from, int to, int spare, std::vector<Move> &moves)
{
    if (count == 0) {
        return;
    }

    solveRecursive(count - 1, from, spare, to, moves);
    moves.push_back({count, from, to});
    solveRecursive(count - 1, spare, to, from, moves);
}
