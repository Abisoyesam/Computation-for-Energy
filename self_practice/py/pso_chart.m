% PSO in MATLAB with Visualization
clear; clc;

% Parameters
min_x = -4;
max_x = 2;
n_particles = 50;
max_iteration = 50;
w = 0.7;
c1 = 1.4;
c2 = 1.2;

% Function definition: f(x) = (x+1)^2 * sin(x)
f = @(x) (x+1).^2 .* sin(x);

% Initialize particle positions and velocities randomly in [min_x, max_x]
positions = min_x + (max_x - min_x) * rand(n_particles, 1);
velocities = min_x + (max_x - min_x) * rand(n_particles, 1);

% Initialize personal best positions and values
particle_best_positions = positions;
particle_best_values = inf(n_particles, 1);

% Initialize global best
global_best_value = inf;
global_best_position = positions(1);

figure;
for iter = 1:max_iteration
    % Evaluate fitness for each particle
    fitness = f(positions);
    
    % Update each particle's personal best
    for i = 1:n_particles
        if fitness(i) < particle_best_values(i)
            particle_best_values(i) = fitness(i);
            particle_best_positions(i) = positions(i);
        end
    end
    
    % Update global best
    [min_fitness, idx] = min(fitness);
    if min_fitness < global_best_value
        global_best_value = min_fitness;
        global_best_position = positions(idx);
    end
    
    % Plot function and current particle positions
    x = linspace(min_x, max_x, 400);
    y = f(x);
    plot(x, y, 'g-', 'LineWidth', 2); hold on;
    scatter(positions, f(positions), 50, 'r', 'filled'); hold off;
    title(['Iteration ' num2str(iter)]); xlabel('x'); ylabel('f(x)'); grid on;
    pause(0.1);
    
    % Update velocities and positions
    r1 = rand(n_particles, 1);
    r2 = rand(n_particles, 1);
    velocities = w .* velocities + c1 .* r1 .* (particle_best_positions - positions) + c2 .* r2 .* (global_best_position - positions);
    positions = positions + velocities;
    
    % Apply boundary conditions
    positions(positions > max_x) = max_x;
    positions(positions < min_x) = min_x;
end

disp(['Solution: ' num2str(global_best_position) ' with value: ' num2str(global_best_value)]);
