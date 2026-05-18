#include "hanoimodel.h"

#include <algorithm>

void HanoiModel::reset(int diskCount)
{
    m_diskCount = std::clamp(diskCount, 1, 16);
    m_moves = HanoiSolver::solve(m_diskCount);
    m_currentStep = 0;
    rebuildTowers();
}

bool HanoiModel::next()
{
    if (m_currentStep >= totalSteps()) {
        return false;
    }

    applyMove(m_moves[static_cast<std::size_t>(m_currentStep)]);
    ++m_currentStep;
    return true;
}

bool HanoiModel::previous()
{
    if (m_currentStep <= 0) {
        return false;
    }

    --m_currentStep;
    undoMove(m_moves[static_cast<std::size_t>(m_currentStep)]);
    return true;
}

void HanoiModel::goToStep(int step)
{
    const int target = std::clamp(step, 0, totalSteps());
    if (target == m_currentStep) {
        return;
    }

    if (target < m_currentStep || target - m_currentStep > m_currentStep) {
        m_currentStep = 0;
        rebuildTowers();
    }

    while (m_currentStep < target) {
        next();
    }
}

int HanoiModel::diskCount() const
{
    return m_diskCount;
}

int HanoiModel::currentStep() const
{
    return m_currentStep;
}

int HanoiModel::totalSteps() const
{
    return static_cast<int>(m_moves.size());
}

const std::vector<Move> &HanoiModel::moves() const
{
    return m_moves;
}

const std::array<std::vector<int>, 3> &HanoiModel::towers() const
{
    return m_towers;
}

Move HanoiModel::currentMove() const
{
    if (m_currentStep <= 0 || m_currentStep > totalSteps()) {
        return {};
    }
    return m_moves[static_cast<std::size_t>(m_currentStep - 1)];
}

void HanoiModel::rebuildTowers()
{
    for (auto &tower : m_towers) {
        tower.clear();
    }

    m_towers[0].reserve(static_cast<std::size_t>(m_diskCount));
    for (int disk = m_diskCount; disk >= 1; --disk) {
        m_towers[0].push_back(disk);
    }
}

void HanoiModel::applyMove(const Move &move)
{
    auto &fromTower = m_towers[static_cast<std::size_t>(move.from)];
    auto &toTower = m_towers[static_cast<std::size_t>(move.to)];
    if (!fromTower.empty()) {
        toTower.push_back(fromTower.back());
        fromTower.pop_back();
    }
}

void HanoiModel::undoMove(const Move &move)
{
    auto &fromTower = m_towers[static_cast<std::size_t>(move.from)];
    auto &toTower = m_towers[static_cast<std::size_t>(move.to)];
    if (!toTower.empty()) {
        fromTower.push_back(toTower.back());
        toTower.pop_back();
    }
}
