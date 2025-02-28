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

    // 主圓
    float circle = smoothstep(u_Radius + pulse + u_Thickness, u_Radius + pulse, dist);

    // 邊緣漸變
    float alpha = circle * smoothstep(u_Radius + pulse + u_Thickness + u_Fade,
                                      u_Radius + pulse + u_Thickness,
                                      dist);

    // 最終顏色
    fragColor = vec4(u_Color.rgb, u_Color.a * alpha);
}