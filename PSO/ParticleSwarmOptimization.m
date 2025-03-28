clc; clear; close all;
%% User defined parameters
m = 2;                  % dimensionality (search space) for m = 2 or m = 3
n_particles = 10;       % numbers of particles (agents) set for 10 or 20
max_iter = 100;         % maximum iteration which is the stopping criteria

%% PSO hyperperameters
w = 0.7;                % inertia weight
c1 = 1.4;               % cognitive (personal) coefficient
c2 = 1.2;               % social coefficient

% for rastrigin problems, the search space boundaries is 
% commonly [-5.12 5.12]
min_x = -5.12;          % minimum search space boundary
max_x = 5.12;           % maximum search space boundary

%% Initialize the Swarm

% initialize the position with random value with [min_x max_x]
x = min_x + (max_x - min_x) .* rand(n_particles, m);

% initialize the velocity to zero
v = zeros(n_particles, m);

% initialize the fitness funtion
fitness = zeros(n_particles, 1);

for i = 1 : length(x)
    fitness(i) = rastrigin(x(i,:));
end

% personal best
p_best_pose = x;                % personal best position
p_best_val = fitness;           % personal best value

% global best
% the global best value is the minimum fitnes
[gbest_val, gbest_idx] = min(fitness);      % find the index of the minimum fitness
% the index of minimum fitness is the global best position
gbest_pose = x(gbest_idx,:);                 % global best position

%% Figure template
if m == 2
    figure('Name','PSO Rastrigin 2D','NumberTitle','off');
elseif m == 3
    figure('Name','PSO Rastrigin 3D','NumberTitle','off');
end

% figure template for precomputing grid for countor and surf
[X, Y] = meshgrid(linspace(min_x, max_x, 100), linspace(min_x, max_x, 100));
Z = zeros(size(X));
for i = 1:size(X,1)
    for j = 1:size(X,2)
        Z(i,j) = rastrigin([X(i,j), Y(i,j)]);
    end
end
%% Main PSO Loop
for iter = 1 : max_iter
    % update velocities and positions of the particles (agents)
    for i = 1 : n_particles
        % random coefficients for behavior ranging from 0 - 1
        r1 = rand(); r2 = rand();

        % velocity update
        v(i,:) = w*v(i,:) + c1*r1*(p_best_pose(i,:) - x(i,:)) + c2*r2*(gbest_pose - x(i,:));

        % update the position
        x(i,:) = x(i,:) + v(i,:);

        % boundary conditions to make sure position isn't 
        % beyound [min_x max_x] limit
        for d = 1:2
            if x(i, d) > max_x
                x(i, d) = max_x;
            elseif x(i, d) < min_x
                x(i, d) = min_x;
            end
        end
    end

    % evaluate fitness after updating positions
    for i = 1 : n_particles
        fitness(i) = rastrigin(x(i,:));

        % update personal best
        if fitness(i) < p_best_val(i)
            p_best_val(i) = fitness(i);
            p_best_pose(i,:) = x(i,:);
        end
    end

    % update global best
    [current_best, current_best_idx] = min(p_best_val);
    if current_best < gbest_val
        gbest_val = current_best;
        gbest_pose = p_best_pose(current_best_idx, :);
    end

    % display iteration info
    fprintf('Iteration %d  \t| Global Best Value = %f\n', iter, gbest_val);

    % plot the graph
    if m == 2
        clf;
        contourf(X,Y,Z,80, 'LineColor', 'none'); hold on;
        plot(p_best_pose(:,1), p_best_pose(:,2), 'ro', 'MarkerFaceColor','r');
        plot(gbest_pose(:,1), gbest_pose(:,2), 'ro', 'MarkerFaceColor','g');
        title(['Iteration ' num2str(iter)]);
        xlabel('x_1'); ylabel('x_2');
        axis([min_x max_x min_x max_x]);
        drawnow; % for the plot animation
    elseif (m == 3)
        clf; % clear figure
        surf(X, Y, Z, 'EdgeColor','none');
        colormap('jet');
        hold on;
        view(45, 50); % view angle
    
        % evaluate Rastrigin at each particle to show their 'height'
        zPos = arrayfun(@(row) rastrigin(p_best_pose(row,:)), 1:size(p_best_pose,1));
        scatter3(p_best_pose(:,1), p_best_pose(:,2), zPos, 40, 'r', 'filled');
        title(['Iteration ' num2str(iter)]);
        xlabel('x_1'); ylabel('x_2'); zlabel('Function Value');
        drawnow;
    end
end

%% final result
disp('==============================================');
disp('PSO optimization successfully done.');
fprintf('Best value found: %f\n', gbest_val);
if m == 2
    fprintf('Best position found: [%f, %f]\n', gbest_pose(1), gbest_pose(2));
elseif m == 3
    fprintf('Best position found: [%f, %f, %f]\n', gbest_pose(1), gbest_pose(2), gbest_pose(3));
end

