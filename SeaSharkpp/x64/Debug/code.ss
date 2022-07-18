using System
using OpenGL
using FileWriter

gl_Init()

shader = "Sea Shark"

gl_createShader(shader)
gl_initShader(shader, 800, 600)

vertShader = readFile("VertexShader.s")
fragShader = readFile("FragmentShader.s")

gl_compileShader(shader, vertShader, fragShader)

Vertices = [
	-1.0, -1.0, 0.0,
	1.0, -1.0, 0.0,
	-1.0, 1.0, 0.0,
	1.0, 1.0, 0.0
]

Indices = [
	0, 1, 2,
	2, 3, 1
]

gl_setVerts(shader, Vertices, Indices)

PlayerPos = [0.5, 0.8]
PlayerScale = [0.03, 0.03]
groundHeight = 0.1

widthHeightratio = 1.0

velY = 0

playerSpeed = 1

prevTime = gl_getTime()

while(!gl_shouldClose(shader))
{
	curTime = gl_getTime()
	deltaTime = curTime - prevTime
	prevTime = curTime

	gl_setmousePosition(400, 300)

	gl_setmouseVisibility(false)

	realSpeed = playerSpeed * deltaTime

	if(gl_isPressed("A"))
	{
		PlayerPos = set(PlayerPos, 0, get(PlayerPos, 0) - realSpeed)
	}
	if(gl_isPressed("D"))
	{
		PlayerPos = set(PlayerPos, 0, get(PlayerPos, 0) + realSpeed)
	}

	if(get(PlayerPos, 1) > groundHeight + get(PlayerScale, 1))
	{
		velY -= 0.1 * deltaTime
	}
	else
	{
		velY = 0
	}

	PlayerPos = set(PlayerPos, 1, get(PlayerPos, 1) + velY * deltaTime)

	size = gl_windowSize(shader)
	widthHeightratio = float(get(size, 0)) / get(size, 1)
	gl_uniform2f(shader, "windowSize", get(size, 0), get(size, 1))
	gl_uniform2f(shader, "PlayerPos", get(PlayerPos, 0), get(PlayerPos, 1) * widthHeightratio)
	gl_uniform2f(shader, "playerSize", get(PlayerScale, 0), get(PlayerScale, 1) * widthHeightratio)
	gl_uniform1f(shader, "GroundHeight", groundHeight * widthHeightratio)
	gl_Update(shader)

	if(gl_isPressed("ESCAPE"))
	{
		gl_Close(shader)
	}
}

gl_Terminate()