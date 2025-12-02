from math import sqrt


def distance(p1, p2):
    """Euclidean distance."""
    return sqrt((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2)
