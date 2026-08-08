#version 330 core
out vec4 FragColor;

in vec3 viewRay;

uniform vec3 horizonColor;
uniform vec3 zenithColor;

void main()
{
    vec3 dir = normalize(viewRay);
    
    // Smoothly mix based on the y (up) direction
    // pow() adds a nice curve so the horizon color stays lower
    float blend = clamp(dir.y, 0.0, 1.0);
    blend = pow(blend, 0.6); 
    
    FragColor = vec4(mix(horizonColor, zenithColor, blend), 1.0);
}
