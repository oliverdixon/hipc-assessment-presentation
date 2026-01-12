const compute_t pd_factor = timestep_duration * resolution;

#pragma omp parallel default(none) shared(pd_factor)
{
    // X velocities
    #pragma omp for collapse(2) schedule(static) nowait
    for (indexer_t i = 1; i < extents.x - 2; ++i)
        for (indexer_t j = 1; j < extents.y - 1; ++j)
            if (flags[i][j] & CELL_FLUID && flags[i + 1][j] & CELL_FLUID)
                velocity_x[i][j] = tentative_velocity_x[i][j] -
                    (pressure[i + 1][j] - pressure[i][j]) * pd_factor;

    // Y velocities
    #pragma omp for collapse(2) schedule(static) nowait
    for (indexer_t i = 1; i < extents.x - 1; ++i)
        for (indexer_t j = 1; j < extents.y - 2; ++j)
            if (flags[i][j] & CELL_FLUID && flags[i][j + 1] & CELL_FLUID)
                velocity_y[i][j] = tentative_velocity_y[i][j] -
                    (pressure[i][j + 1] - pressure[i][j]) * pd_factor;
}

