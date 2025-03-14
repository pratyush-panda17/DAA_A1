import random

def generate_unique_2d_points(n, filename="points.txt"):
    points = set()
    
    while len(points) < n:
        x = random.randint(1, 500)
        y = random.randint(1, 500)
        points.add((x, y))  # Ensures uniqueness
    
    with open(filename, "w") as f:
        f.write(f"{n}\n")
        for x, y in points: 
            f.write(f"{x} {y}\n")


generate_unique_2d_points(1250)