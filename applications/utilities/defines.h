#ifndef MLAT_DEFINES_H
#define MLAT_DEFINES_H

#include <cstdint>

static const uint8_t k_towers_count = 5;
static const uint8_t k_equations_count = 10;
static const uint8_t k_max_iterations_count = 1;
static const uint8_t k_point_mod = 100;
static const uint8_t k_space_dim = 3;
static const double k_light_speed = 3e5f;

static const double k_sample_rate = 3e-2;
static const uint32_t k_duration_interval = 100;
static const uint32_t k_duration_overstatement = 0.05 * k_duration_interval;

static const double k_noise_dispersion = 1e-6;

//! Speed and height limits
static const double k_max_height = 10.f;
static const double k_min_z_speed = -0.01f;
static const double k_coefficient_deceleration = 1.1f;
static const double k_deceleration_speed = 1e-8f;

static const double k_max_speed = 250.f;
static const double k_max_acceleration = 9.5f;
static const double k_average_acceleration = 2.5f;

#endif
