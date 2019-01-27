
in vec2 textCoord;
uniform sampler2D text;
uniform vec4 color;
out vec4 fragmentColor;

void main() 
{
    fragmentColor = vec4(1, 1, 1, texture(text, textCoord).r) * color;
}