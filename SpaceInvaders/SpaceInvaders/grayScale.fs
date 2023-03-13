#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
 
vec4 color = texture2D(texture1, TexCoord);
void main()
{
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114)); //formula for gray scale
    vec3 grayScale = vec3(gray);
    //gray scale
    FragColor = vec4(grayScale, color.a);

    
}