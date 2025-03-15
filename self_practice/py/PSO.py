from random import random
import numpy as np

def f(x):
    return (x+1)**2 * np.sin(x)

class Particle:

    def __init__(self, min_x, max_x, n_dims=1):
        # this is the interval we are dealing with [min_x, max_x]
        self.min_x = min_x
        self.max_x = max_x

        # this is the positon of a particle - dimension dependent variable >> 1 dimensions: (x) >> 2 dimensions: (x,y) cordinates
        self.position = self.initialize(n_dims)
        # velocity parameter of the particle
        self.velocity = self.initialize(n_dims)

        # track the best position (p_best)
        self.best_position = self.position
        # track the best value (f(p_best))
        self.best_value = 1e10

    # update the location of the particle
    def move(self):
        new_position = self.position + self.velocity
        # when updating the positions and velocities we have to consider the boundaries
        # upper bound: max_x - lower bound: min_x
        new_position = np.where(new_position > self.max_x, self.max_x, new_position)
        new_position = np.where(new_position < self.min_x, self.min_x, new_position)
        self.position = new_position

    # random positions for x in [min_x, max_x]
    def initialize(self, n):
        return np.array([self.min_x + (self.max_x - self.min_x)*random() for _ in range(n)])

class ParticleSwarmOptimization:
    # if c1=0 it means there is no individual actions - all the particles behave according to the global best position. 100% EXPLOITATION

    # if c2=0 it means the particles are totally independent of each other (no interaction between them and no information exchange between the particles). 100% EXPLORATION
    def __init__(self, min_x, max_x, n_particles=100, max_iteration=30, w=0.7, c1=1.4, c2=1.2):
        self.n_particles = n_particles
        self.max_x = max_x
        self.min_x = min_x
        self.max_iteration = max_iteration

        # swarm of particles
        self.particles = [Particle(min_x, max_x) for _ in range(n_particles)]
        self.best_position = self.particles[0].position
        self.best_value = 1e10

        # inertia weight (exploration and exploitation trade-off)
        self.w = w
        # cognitive parameter
        self.c1 = c1
        # social parameter
        self.c2 = c2

        # global best value and it is extremely high because we are looking for the minimum value
        # for max value we can set it to -1e10; extremely low value
        best_value = 1e10 
        best_position = self.particles[0].position

    def run(self):
        counter = 0

        while counter < self.max_iteration:
            counter += 1

            self.move_particles()
            self.set_best()
            self.set_particle_best()

        print("Solution: %s with value: %s" % (self.best_position, self.best_value))

    # particle related best values and positions 
    def set_particle_best(self):
        for particle in self.particles:
            particle_fitness = f(particle.position)

            if particle_fitness < particle.best_value:
                particle.best_value = particle_fitness
                particle.best_position = particle.position

    # update the global best value and position
    def set_best(self):
        for particle in self.particles:
            particle_fitness = f(particle.position)

            if particle_fitness < self.best_value:
                self.best_value = particle_fitness
                self.best_position = particle.position

    # update the position of the particles
    def move_particles(self):
        for particle in self.particles:
            new_velocity = self.w * particle.velocity + self.c1 * random() * (particle.best_position - particle.position) + self.c2 * random() * (self.best_position - particle.position)
            particle.velocity = new_velocity
            particle.move()

if __name__ == '__main__':
    algorithm = ParticleSwarmOptimization(min_x=-4, max_x=4)
    algorithm.run()