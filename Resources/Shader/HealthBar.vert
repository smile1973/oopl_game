#version 410 core

layout(location = 0) in vec2 a_Position;  // 頂點位置
layout(location = 1) in vec2 a_TexCoord;  // UV 坐標

out vec2 v_TexCoord;

uniform float u_Width;  // 血條寬度

void main() {
    // 設定血條寬度，這樣可以根據當前血量縮放血條
    gl_Position = vec4(a_Position.x * u_Width, a_Position.y, 0.0, 1.0);
    v_TexCoord = a_TexCoord;
}
