#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture(texture1, TexCoord);
    
    // Directional Light Setup
    vec3 lightDir = normalize(vec3(-0.5, 1.0, 0.3));
    
    // Diffuse lighting
    float diff = max(dot(Normal, lightDir), 0.0);
    
    // Ambient lighting
    float ambient = 0.4;
    
    // Final shading multiplier
    float lighting = ambient + (diff * 0.6);
    
    FragColor = vec4(texColor.rgb * lighting, texColor.a);
}
