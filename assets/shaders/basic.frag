#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform vec3 viewPos;
uniform vec3 fogColor;

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
    vec4 finalColor = vec4(texColor.rgb * lighting, texColor.a);
    
    // Fog calculation
    float distance = length(viewPos - FragPos);
    float fogStart = 30.0;
    float fogEnd = 80.0;
    
    // Linear fog factor
    float fogFactor = clamp((fogEnd - distance) / (fogEnd - fogStart), 0.0, 1.0);
    
    // Mix fog color with final color
    FragColor = mix(vec4(fogColor, 1.0), finalColor, fogFactor);
}
