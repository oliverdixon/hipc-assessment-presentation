compute_t x_max = -INFINITY;
compute_t y_max = -INFINITY;

// Maximise our local X velocity
for (indexer_t i = h_interior.begin; i < h_interior.end; ++i)
    for (indexer_t j = v_interior.begin; j < v_interior.end; ++j)
        x_max = fmax(fabs(velocity_x[i][j]), x_max);

compute_t x_global_max = INT_MIN;
compute_t y_global_max = INT_MIN;
MPI_Request reqs[2];

// Reduce to the global maximum X velocity
MPI_Iallreduce(&x_max, &x_global_max, 1, MPI_COMPUTE, MPI_MAX, cartesian_comm, &reqs[0]);

// Maximise our local Y velocity
for (indexer_t i = h_interior.begin; i < h_interior.end; ++i)
    for (indexer_t j = v_interior.begin; j < v_interior.end; ++j)
        y_max = fmax(fabs(velocity_y[i][j]), y_max);

// Reduce to the global maximum Y velocity
MPI_Iallreduce(&y_max, &y_global_max, 1, MPI_COMPUTE, MPI_MAX, cartesian_comm, &reqs[1]);
MPI_Waitall(sizeof(reqs) / sizeof(*reqs), reqs, MPI_STATUSES_IGNORE);
