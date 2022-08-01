using System
using OpenGL
using FileWriter

struct vector
{
    x = 0
    y = 0
    z = 0
}

struct test
{
    pos = vector()
}

Player = test()

Player = Player.set("pos", Player.pos.set("x", 100))

printline(Player.pos.x)