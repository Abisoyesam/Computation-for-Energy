%%Rastrigin function in 2D, optimized using PSO
% PSO parameters
min_x = -5.12; 
max_x =  5.12;
n_particles = 20; % number of agents
max_iter = 100;
w = 0.7;   % inertia weight
c1 = 1.4;  % cognitive coefficient
c2 = 1.2;  % social coefficient

%% Rastrigin function in 2D:
% EVal = -3*m + sum_{j=1}^m( x_j^2 - 3*cos(2*pi*x_j ) ), for m=2
% => f(x1, x2) = -6 + x1^2 + x2^2 - 3*cos(2*pi*x1) - 3*cos(2*pi*x2)
rastrigin_2d = @(x) -6 + x(1)^2 + x(2)^2 ...
                    - 3*cos(2*pi*x(1)) - 3*cos(2*pi*x(2));

% Initialize positions and velocities in [min_x, max_x]
positions  = min_x + (max_x - min_x).*rand(n_particles, 2);
velocities = min_x + (max_x - min_x).*rand(n_particles, 2);

% Personal best
personal_best_positions = positions;
personal_best_values    = inf(n_particles, 1);

% Global best
global_best_value = inf;
global_best_position = [0, 0];

%% Pre-compute grid for contour
[X, Y] = meshgrid(linspace(min_x, max_x, 100), linspace(min_x, max_x, 100));
Z = zeros(size(X));
for i = 1:size(X,1)
    for j = 1:size(X,2)
        Z(i,j) = rastrigin_2d([X(i,j), Y(i,j)]);
    end
end

figure('Name','PSO Rastrigin 2D','NumberTitle','off');

%% Main PSO loop
for iter = 1:max_iter
    
    % Evaluate each particle
    for i = 1:n_particles
        fitness = rastrigin_2d(positions(i, :));
        
        % Update personal best
        if fitness < personal_best_values(i)
            personal_best_values(i) = fitness;
            personal_best_positions(i, :) = positions(i, :);
        end
        
        % Update global best
        if fitness < global_best_value
            global_best_value = fitness;
            global_best_position = positions(i, :);
        end
    end
    
    % Update velocities and positions
    for i = 1:n_particles
        r1 = rand();
        r2 = rand();
        velocities(i, :) = w*velocities(i, :) ...
            + c1*r1*(personal_best_positions(i, :) - positions(i, :)) ...
            + c2*r2*(global_best_position - positions(i, :));
        
        positions(i, :) = positions(i, :) + velocities(i, :);
        
        % Enforce boundary conditions
        for d = 1:2
            if positions(i, d) > max_x
                positions(i, d) = max_x;
            elseif positions(i, d) < min_x
                positions(i, d) = min_x;
            end
        end
    end
    
    % Visualization: clear axes, draw contour, then scatter swarm
    clf; % Clear current figure
    contourf(X, Y, Z, 50, 'LineColor','none'); hold on;
    plot(positions(:,1), positions(:,2), 'ro', 'MarkerFaceColor','r');
    title(['Iteration ' num2str(iter)]);
    xlabel('x_1'); ylabel('x_2');
    axis([min_x max_x min_x max_x]);
    colorbar;
    drawnow;
end

%% Print final result
fprintf('Best position found: [%.4f, %.4f]\n', global_best_position(1), global_best_position(2));
fprintf('Best value: %.4f\n', global_best_value);

