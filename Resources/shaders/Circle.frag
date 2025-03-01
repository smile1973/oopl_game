#version 410 core

in vec2 v_TexCoord;

uniform float u_Radius;     // 圓的半徑 (0.0-0.5)
uniform float u_Thickness;  // 圓的邊緣寬度
uniform float u_Fade;       // 漸變效果的寬度
uniform vec4 u_Color;       // 圓的顏色
uniform float u_Time;       // 用於動畫效果

out vec4 fragColor;

void main()
{
    float dist = length(v_TexCoord);

    // 創建脈動效果
    float pulse = 0.05 * sin(u_Time * 3.0);

    // 計算實心圓
    float circle = 1.0 - smoothstep(u_Radius + pulse - u_Fade,
                                   u_Radius + pulse,
                                   dist);

    // 如果不在圓內，丟棄片段（保持背景透明）
    if (circle < 0.01) {
        discard;
    }

    // 增加圓邊緣效果
    float edge = smoothstep(u_Radius + pulse - u_Thickness - u_Fade,
                           u_Radius + pulse - u_Thickness,
                           dist);

    // 結合邊緣和內部
    float alpha = circle;

    // 邊緣效果增強
    vec3 color = mix(u_Color.rgb * 1.5, u_Color.rgb, edge);

    // 最終顏色
    fragColor = vec4(color, u_Color.a * alpha);
}