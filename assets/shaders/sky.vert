#version 330 core
layout (location = 0) in vec2 aPos;

out vec3 viewRay;

uniform mat4 invProjection;
uniform mat4 invView;

void main()
{
    // Render at the back of the depth buffer (Z = 0.9999)
    gl_Position = vec4(aPos, 0.9999, 1.0);

    // Unproject the clip space coordinate to world space ray direction
    vec4 clipPos = vec4(aPos, -1.0, 1.0);
    vec4 eyePos = invProjection * clipPos;
    eyePos = vec4(eyePos.x, eyePos.y, -1.0, 0.0);
    
    viewRay = vec3(invView * eyePos);
}
