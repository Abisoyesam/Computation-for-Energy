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
    fitness(i,:) = rastrigin(x(i,:));
end

% personal best
p_best_pose = x;                % personal best position
p_best_val = fitness;           % personal best value

% global best
% the global best value is the minimum fitnes
[gbest_val, gbest_idx] = min(fitness);      % find the index of the minimum fitness
% the index of minimum fitness is the global best position
gbest_pose = x(gbest_idx,:);                 % global best position

%% Main PSO Loop