#pragma once

#include "hanoisolver.h"

#include <array>
#include <vector>

class HanoiModel
{
public:
    void reset(int diskCount);
    bool next();
    bool previous();
    void goToStep(int step);

    int diskCount() const;
    int currentStep() const;
    int totalSteps() const;
    const std::vector<Move> &moves() const;
    const std::array<std::vector<int>, 3> &towers() const;
    Move currentMove() const;

private:
    void rebuildTowers();
    void applyMove(const Move &move);
    void undoMove(const Move &move);

    int m_diskCount = 10;
    int m_currentStep = 0;
    std::vector<Move> m_moves;
    std::array<std::vector<int>, 3> m_towers;
};
