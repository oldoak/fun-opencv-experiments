
in vec3 vertexPosition;
uniform vec4 vertexColor;
out vec4 colorFromVshader;

void main()
{
    gl_Position = vec4(vertexPosition, 1.0);
    colorFromVshader = vertexColor;
}
