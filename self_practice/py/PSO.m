clear; clc; close all;

%% Problem Definition
func = @(x) rastrigin(x);        % cost function
m = 2;                        % no of variables or dimension
var_size = [1 m];             % matrix size of variables
max_x = 5.12; min_x = -5.12;  % upper and lower bound

%% Parameters of PSO
maxIter = 100;               % maximum number of iteration
n = 20;                      % no of particles
w = 1;                       % inertia coefficient
w_damp = 0.99;               % damping ratio of inertia coefficient
c1 = 1.5;                    % personal (cognitive) accl 
c2 = 1.5;                    % social accleration coeff

%% Initialization
% particle template
empty_particle.Position = [];
empty_particle.Velocity = [];
empty_particle.Cost = [];
empty_particle.Best.Position = [];
empty_particle.Best.Cost = [];

% create population array
particle = repmat(empty_particle, n, 1);

% initialize global best
GlobalBest.Cost = inf; % for min but -inf for maximization problem

% initialize population member
for i = 1:n
    % generate random solution
    particle(i).Position = unifrnd(min_x, max_x, var_size);

    % initialize  velocity
    particle(i).Velocity = zeros(var_size);

    % evaluation
    particle(i).Cost = func(particle(i).Position);

    % update the personal best
    particle(i).Best.Cost = particle(i).Cost;
    particle(i).Best.Position = particle(i).Position;

    % update global best
    if particle(i).Best.Cost < GlobalBest.Cost
        GlobalBest = particle(i).Best;
    end
end

% array to hold best cost value on each iteration
BestCost = zeros(maxIter, 1);

%% Main Loop of PSO
for it = 1:maxIter
    for i = 1:n
        % update velocity
        particle(i).Velocity = w*particle(i).Velocity + c1 * rand(var_size) .* (particle(i).Best.Position - particle(i).Position) + c2*rand(var_size).*(GlobalBest.Position - particle(i).Position);

        % update position
        particle(i).Position = particle(i).Position + particle(i).Velocity;

        % evaluation
        particle(i).Cost = func(particle(i).Position);

        % update personal best
        if particle(i).Cost < particle(i).Best.Cost
            particle(i).Best.Position = particle(i).Position;
            particle(i).Best.Cost = particle(i).Cost;

            % update global best
            if particle(i).Best.Cost < GlobalBest.Cost
                GlobalBest = particle(i).Best;
            end

        end
    end

    % store the best cost value
    BestCost(it) = GlobalBest.Cost;

    % display iteration information
    fprintf('Iteration %d : Best Cost = %f\n',it,BestCost(it));

    % damping inertia coefficient
    w = w * w_damp;
end

%% Results
figure;
plot(BestCost, 'LineWidth', 2);
xlabel('Iteration'); ylabel('Best Cost');
grid on;