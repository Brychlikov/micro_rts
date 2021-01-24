import sys

import pygame
from pygame.locals import *

pygame.init()

fps = 60
fpsClock = pygame.time.Clock()

width, height = 1200, 700
screen = pygame.display.set_mode((width, height))


positions = [
    pygame.math.Vector2(100, 100),
    pygame.math.Vector2(200, 400),
    pygame.math.Vector2(150, 200),
    pygame.math.Vector2(350, 350),
    pygame.math.Vector2(500, 500),
    pygame.math.Vector2(670, 200),
    pygame.math.Vector2(800, 100)
]
# positions = [
#     pygame.math.Vector2(100, 400),
#     pygame.math.Vector2(200, 400),
#     pygame.math.Vector2(300, 400),
# ]

color = (255, 100, 100)

total_x = sum(p[0] for p in positions)
total_y = sum(p[1] for p in positions)
mean_coords = total_x / len(positions), total_y / len(positions)

closest_pos = min(positions, key=lambda t: (t[0] - mean_coords[0]) ** 2 + (t[1] - mean_coords[1]) ** 2)
offsets = [pygame.math.Vector2(p[0] - closest_pos[0], p[1] - closest_pos[1]) for p in positions]

# Game loop.
step = False
while True:
    screen.fill((0, 0, 0))

    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
        if event.type == KEYDOWN:
            if event.key == K_SPACE:
                step = True

    # Update.

    repel_multiplier = 200000.0
    attract_multiplier = 50.0
    if step:
        step = False
        for i, v1 in enumerate(offsets):
            delta = pygame.math.Vector2(0, 0)
            repel = pygame.math.Vector2(0, 0)
            for v2 in offsets:
                if v1 == v2:
                    continue
                r = v1 - v2
                direction = r / r.magnitude()
                repel += (direction * repel_multiplier) / r.magnitude_squared()
            if v1.magnitude() == 0:
                continue
            attract_dir = v1 / v1.magnitude()
            delta -= attract_dir * attract_multiplier
            print(delta, repel)
            delta += repel
            offsets[i] += delta
            positions[i] += delta




    # Draw.
    screen.fill((0, 0, 0))
    for x, y in positions:
        pygame.draw.rect(screen, color, pygame.Rect(x, y, 80, 80))

    pygame.draw.rect(screen, (100, 255, 100), pygame.Rect(*closest_pos, 80, 80))
    pygame.draw.circle(screen, (100, 100, 255), mean_coords, 10)

    pygame.display.flip()
    fpsClock.tick(fps)