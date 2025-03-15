import numpy as np
import matplotlib.pyplot as plt
from random import random
from mpl_toolkits.mplot3d import Axes3D  # Enables 3D plotting

# Rastrigin function in 3D:
# f(x1,x2,x3) = -9 + x1^2 + x2^2 + x3^2 - 3*cos(2*pi*x1) - 3*cos(2*pi*x2) - 3*cos(2*pi*x3)
def rastrigin_3d(x):
    return -9 + x[0]**2 + x[1]**2 + x[2]**2 \
           - 3*np.cos(2*np.pi*x[0]) - 3*np.cos(2*np.pi*x[1]) - 3*np.cos(2*np.pi*x[2])

class Particle:
    def __init__(self, min_x, max_x, dimensions=3):
        self.min_x = min_x
        self.max_x = max_x
        self.position = self._initialize(dimensions)
        self.velocity = self._initialize(dimensions)
        self.best_position = np.copy(self.position)
        self.best_value = float('inf')
    
    def _initialize(self, dim):
        return np.array([self.min_x + (self.max_x - self.min_x)*random() for _ in range(dim)])
    
    def move(self):
        new_position = self.position + self.velocity
        # Clamp to boundaries
        new_position = np.maximum(new_position, self.min_x)
        new_position = np.minimum(new_position, self.max_x)
        self.position = new_position

class ParticleSwarmOptimization:
    def __init__(self, min_x=-5.12, max_x=5.12, n_particles=20, max_iter=100, w=0.7, c1=1.4, c2=1.2):
        self.min_x = min_x
        self.max_x = max_x
        self.n_particles = n_particles
        self.max_iter = max_iter
        self.w = w
        self.c1 = c1
        self.c2 = c2
        self.particles = [Particle(min_x, max_x, 3) for _ in range(n_particles)]
        self.global_best_value = float('inf')
        self.global_best_position = np.zeros(3)
    
    def run(self):
        # Set up 3D visualization
        fig = plt.figure(figsize=(8, 6))
        ax = fig.add_subplot(111, projection='3d')
        plt.ion()  # Interactive mode
        
        for iter in range(self.max_iter):
            # Evaluate each particle
            for p in self.particles:
                fitness = rastrigin_3d(p.position)
                if fitness < p.best_value:
                    p.best_value = fitness
                    p.best_position = np.copy(p.position)
                if fitness < self.global_best_value:
                    self.global_best_value = fitness
                    self.global_best_position = np.copy(p.position)
            
            # Update velocities and positions
            for p in self.particles:
                r1 = random()
                r2 = random()
                p.velocity = (self.w * p.velocity +
                              self.c1 * r1 * (p.best_position - p.position) +
                              self.c2 * r2 * (self.global_best_position - p.position))
                p.move()
            
            # 3D Visualization
            ax.clear()
            # Plot current positions in blue
            positions = np.array([p.position for p in self.particles])
            ax.scatter(positions[:,0], positions[:,1], positions[:,2],
                       color='blue', marker='o', label='Current Position')
            # Plot personal best positions in red
            pbest = np.array([p.best_position for p in self.particles])
            ax.scatter(pbest[:,0], pbest[:,1], pbest[:,2],
                       color='red', marker='x', label='Personal Best')
            # Plot global best position in green
            ax.scatter(self.global_best_position[0], self.global_best_position[1], self.global_best_position[2],
                       color='green', marker='*', s=200, label='Global Best')
            
            ax.set_title(f"Iteration {iter+1}")
            ax.set_xlim(self.min_x, self.max_x)
            ax.set_ylim(self.min_x, self.max_x)
            ax.set_zlim(self.min_x, self.max_x)
            ax.set_xlabel('x1')
            ax.set_ylabel('x2')
            ax.set_zlabel('x3')
            ax.legend()
            plt.pause(0.1)
        
        plt.ioff()
        plt.show()
        print(f"Best position found: {self.global_best_position}")
        print(f"Best value: {self.global_best_value}")

if __name__ == '__main__':
    pso = ParticleSwarmOptimization()
    pso.run()
