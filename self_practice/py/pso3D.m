%% PSO for 3D Rastrigin Function using PSO with 3D Visualization
% PSO parameters
min_x = -5.12; 
max_x =  5.12;
n_particles = 20;
max_iter = 100;
w = 0.7;   % inertia weight
c1 = 1.4;  % cognitive coefficient
c2 = 1.2;  % social coefficient

%% Rastrigin function in 3D:
% f(x1, x2, x3) = -9 + x1^2 + x2^2 + x3^2 - 3*cos(2*pi*x1) - 3*cos(2*pi*x2) - 3*cos(2*pi*x3)
rastrigin_3d = @(x) -9 + x(1)^2 + x(2)^2 + x(3)^2 ...
                    - 3*cos(2*pi*x(1)) - 3*cos(2*pi*x(2)) - 3*cos(2*pi*x(3));

%% Initialize positions and velocities (n_particles x 3)
positions  = min_x + (max_x - min_x).*rand(n_particles, 3);
velocities = min_x + (max_x - min_x).*rand(n_particles, 3);

% Personal best
personal_best_positions = positions;
personal_best_values    = inf(n_particles, 1);

% Global best
global_best_value = inf;
global_best_position = zeros(1,3);

%% Pre-compute grid for isosurface visualization (optional)
grid_points = 30;
[X, Y, Z] = meshgrid(linspace(min_x, max_x, grid_points), ...
                     linspace(min_x, max_x, grid_points), ...
                     linspace(min_x, max_x, grid_points));
F = zeros(size(X));
for i = 1:size(X,1)
    for j = 1:size(X,2)
        for k = 1:size(X,3)
            F(i,j,k) = rastrigin_3d([X(i,j,k), Y(i,j,k), Z(i,j,k)]);
        end
    end
end
% Choose an isovalue (here the mean of F)
iso_val = mean(F(:));

figure('Name','PSO Rastrigin 3D','NumberTitle','off');

%% Main PSO loop
for iter = 1:max_iter
    % Evaluate each particle
    for i = 1:n_particles
        fitness = rastrigin_3d(positions(i, :));
        
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
        velocities(i, :) = w*velocities(i, :) + ...
            c1*r1*(personal_best_positions(i, :) - positions(i, :)) + ...
            c2*r2*(global_best_position - positions(i, :));
        
        positions(i, :) = positions(i, :) + velocities(i, :);
        
        % Enforce boundary conditions
        for d = 1:3
            if positions(i, d) > max_x
                positions(i, d) = max_x;
            elseif positions(i, d) < min_x
                positions(i, d) = min_x;
            end
        end
    end
    
    % 3D Visualization
    clf; % Clear figure
    % Plot isosurface of the Rastrigin function (optional for context)
    p = patch(isosurface(X, Y, Z, F, iso_val));
    isonormals(X, Y, Z, F, p);
    p.FaceColor = [0.8 0.8 0.8];
    p.EdgeColor = 'none';
    alpha(0.3);
    hold on;
    
    % Plot current positions (blue circles)
    scatter3(positions(:,1), positions(:,2), positions(:,3), 50, 'b', 'filled');
    % Plot personal best positions (red crosses)
    scatter3(personal_best_positions(:,1), personal_best_positions(:,2), personal_best_positions(:,3), 50, 'r', 'x');
    % Plot global best position (green star)
    scatter3(global_best_position(1), global_best_position(2), global_best_position(3), 100, 'g', 'p', 'filled');
    
    title(['Iteration ' num2str(iter)]);
    xlabel('x_1'); ylabel('x_2'); zlabel('x_3');
    axis([min_x max_x min_x max_x min_x max_x]);
    grid on;
    view(3);
    drawnow;
end

%% Print final result
fprintf('Best position found: [%.4f, %.4f, %.4f]\n', global_best_position(1), global_best_position(2), global_best_position(3));
fprintf('Best value: %.4f\n', global_best_value);
