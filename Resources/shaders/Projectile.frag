#version 410 core

in vec2 v_TexCoord;

uniform float u_Radius;     // 圓的半徑 (0.0-0.5)
uniform vec4 u_Color;       // 圓的顏色
uniform float u_Time;       // 用於動畫效果

out vec4 fragColor;

void main()
{
    float dist = length(v_TexCoord);

    // 閃爍效果
    float glow = 1.0 + 0.2 * sin(u_Time * 10.0);

    // 主圓
    float circle = 1.0 - smoothstep(u_Radius * 0.8, u_Radius, dist);

    // 邊緣漸變
    float edge = smoothstep(u_Radius * 0.9, u_Radius, dist);

    // 添加尾跡效果
    vec2 direction = vec2(1.0, 0.0); // 假設向右飛行
    float trail = smoothstep(0.0, 0.5, dot(normalize(v_TexCoord), -direction) * 0.5 + 0.5) * (1.0 - dist / u_Radius);

    // 混合顏色
    vec3 color = mix(u_Color.rgb, u_Color.rgb * glow, edge);

    // 如果不在有效範圍，丟棄片段
    if (circle < 0.01) {
        discard;
    }

    // 最終顏色
    fragColor = vec4(color, u_Color.a * edge);
}