class Vector2D:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def __add__(self, vector2):
        return Vector2D(self.x + vector2.x, self.y + vector2.y)

vec1 = Vector2D(1,3)
vec2 = Vector2D(2,4)
ret = vec1 + vec2
print(ret.x)
print(ret.y)
