#pragma once

#include <Eigen/Dense>

/// Amplitude magnitude threshold for display (values below ignored)
static constexpr double AMPLITUDE_DISPLAY_THRESHOLD = 1e-10;

/// Normalization tolerance for state verification
static constexpr double NORM_TOLERANCE = 1e-6;

/**
 * @brief Normalizes quantum state vector to unit norm
 * @param state State vector to normalize (modified in-place)
 * @throws std::runtime_error if normalization fails
 * 
 * Ensures ||state|| = 1.0 within numerical precision.
 */
void normalizeState(Eigen::VectorXcd& state);

/**
 * @brief Prints quantum state to stdout
 * @param state State vector to display
 * 
 * Displays amplitudes above AMPLITUDE_DISPLAY_THRESHOLD threshold.
 */
void printState(const Eigen::VectorXcd& state);
