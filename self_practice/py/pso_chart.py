import numpy as np
import matplotlib.pyplot as plt
from random import random

def f(x):
    return (x + 1) * (x + 1) * np.sin(x)

class Particle:
    def __init__(self, min_x, max_x, dimensions=1):
        self.min_x = min_x
        self.max_x = max_x
        self.position = self.initialize(dimensions)
        self.velocity = self.initialize(dimensions)
        self.best_position = self.position
        self.best_value = 1e10

    def move(self):
        new_position = self.position + self.velocity
        new_position = np.where(new_position > self.max_x, self.max_x, new_position)
        new_position = np.where(new_position < self.min_x, self.min_x, new_position)
        self.position = new_position

    def initialize(self, x):
        return np.array([self.min_x + (self.max_x - self.min_x) * random() for _ in range(x)])

class ParticleSwarmOptimization:
    def __init__(self, min_x, max_x, n_particles=100, max_iteration=30, w=0.7, c1=1.4, c2=1.2):
        self.n_particles = n_particles
        self.max_iteration = max_iteration
        self.particles = [Particle(min_x, max_x) for _ in range(n_particles)]
        self.best_value = 1e10
        self.best_position = self.particles[0].position
        self.w = w
        self.c1 = c1
        self.c2 = c2

    def plot_function(self):
        x = np.linspace(self.particles[0].min_x, self.particles[0].max_x, 400)
        y = f(x)
        plt.plot(x, y, label="f(x)", color="green")
        plt.xlabel("x")
        plt.ylabel("f(x)")
        plt.title("Function Plot with Particle Positions")
        plt.legend()
        plt.grid()

    def plot_particles(self, iteration):
        positions = np.array([particle.position for particle in self.particles])
        plt.scatter(positions, f(positions), color="red", marker="o", label="Particles" if iteration == 0 else "")
        plt.pause(0.1)

    def run(self):
        plt.figure(figsize=(8, 6))
        self.plot_function()

        counter = 0
        while counter < self.max_iteration:
            plt.clf()
            self.plot_function()
            self.plot_particles(counter)
            self.move_particles()
            self.set_best()
            self.set_particle_best()
            counter += 1

        plt.show()
        print('Solution: %s with value: %s' % (self.best_position, self.best_value))

    def set_particle_best(self):
        for particle in self.particles:
            particle_fitness = f(particle.position)
            if particle.best_value > particle_fitness:
                particle.best_value = particle_fitness
                particle.best_position = particle.position

    def set_best(self):
        for particle in self.particles:
            particle_fitness = f(particle.position)
            if self.best_value > particle_fitness:
                self.best_value = particle_fitness
                self.best_position = particle.position

    def move_particles(self):
        for particle in self.particles:
            new_velocity = self.w * particle.velocity + self.c1 * random() * (particle.best_position - particle.position) + self.c2 * random() * (self.best_position - particle.position)
            particle.velocity = new_velocity
            particle.move()

if __name__ == '__main__':
    algorithm = ParticleSwarmOptimization(min_x=-4, max_x=2, n_particles=50, max_iteration=50)
    algorithm.run()
