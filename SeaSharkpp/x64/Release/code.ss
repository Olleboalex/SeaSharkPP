using System
using math
using OpenGL

gl_Init()

start = gl_getTime()

i = 0
while(i <= 180)
{
	printline(sin(ToRad(i)))
	i++
}
stop = gl_getTime()

printline(stop - start)

read()