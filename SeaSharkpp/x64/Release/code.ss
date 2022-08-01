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

position = Player.pos

printline(Player.pos.x)