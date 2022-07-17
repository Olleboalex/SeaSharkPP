#version 330 core

out vec4 FragColor;

uniform vec2 windowSize;

uniform vec2 PlayerPos;
uniform vec2 playerSize;

uniform float GroundHeight;

float Abs(float x)
{
	if(x < 0) return -x;
	return x;
}

void main()
{
	vec2 uv = gl_FragCoord.xy / windowSize;
	vec3 col = vec3(0, 0, 0);
	vec2 uvPlayer = PlayerPos * windowSize;
	vec2 uvSize = playerSize * windowSize;
	//uvSize.y *= windowSize.x / windowSize.y;

	if(gl_FragCoord.y <= GroundHeight * windowSize.y)
	{
		col = vec3(0, 0.7f, 0);
	}
	else if(Abs(uvPlayer.x - gl_FragCoord.x) <= uvSize.x && Abs(uvPlayer.y - gl_FragCoord.y) <= uvSize.y)
	{
		col = vec3(1, 1, 1);
	}

	FragColor = vec4(col, 0);
}