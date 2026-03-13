Shader
{
    RenderType "Skybox"

	Pass
	{
        LightMode "Skybox"

		RenderState
		{
			DepthState (depthTest=true; depthWrite=true; depthFunc= LEQUAL)
		}

		Vertex
		{
			out vec3 worldNormal;

			uniform mat4 linda_SkyboxMatrix_P;

			void main()
			{
				worldNormal = aPosition;
				mat4 view = linda_Matrix_V;
				view[3].xyz = vec3(0.0);
				view[3].w = 1.0;
				gl_Position = linda_SkyboxMatrix_P * view * vec4(aPosition, 1.0);
				gl_Position = gl_Position.xyww;
			}
		}
		
		Fragment
		{
			in vec3 worldNormal;

			// 将方向转换为天顶角 theta (0~PI) 和方位角 phi (0~2PI)
			vec2 dirToAngles(vec3 dir) {
				float theta = acos(dir.y);                 // 与+Y轴的夹角
				float phi = atan(dir.z, dir.x);            // 方位角
				return vec2(theta, phi);
			}

			// 计算两个方向之间的夹角（弧度）
			float angleBetween(vec3 a, vec3 b) {
				float dotProduct = dot(a, b);
				return acos(clamp(dotProduct, -1.0, 1.0));
			}

			// 随机函数（用于星星）
			float hash(vec2 p) {
				return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
			}

			// 模拟 Rayleigh 散射导致的天空亮度（简单版）
			float rayleighPhase(float cosTheta) {
				return 0.75 * (1.0 + cosTheta * cosTheta);
			}

			// 模拟 Mie 散射（太阳周围的光晕）
			float miePhase(float cosTheta, float g) {
				float g2 = g * g;
				return (1.0 - g2) / (4.0 * PI * pow(1.0 + g2 - 2.0 * g * cosTheta, 1.5));
			}

			void main()
			{
                vec3 dir = normalize(worldNormal);           // 视线方向
                vec3 sunDir = normalize(-linda_DirLightDirections[0].xyz);          // 太阳方向

                // 基础颜色：天空与地面分离
                float sunHeight = sunDir.y;     // 太阳高度（-1~1）
                float dayFactor = clamp(sunHeight * 0.5 + 0.5, 0.0, 1.0);   // 太阳越低，黄昏越多

                // 白天颜色：天顶深蓝，地平线亮蓝/淡紫
                vec3 dayTop = vec3(0.2, 0.4, 0.9);      // 天顶
                vec3 dayHorizon = vec3(0.7, 0.8, 1.0);      // 地平线
                vec3 dayGround = vec3(0.46, 0.43, 0.38);
                // 黄昏颜色：暖色调
                vec3 twilightTop = vec3(0.4, 0.1, 0.1);     // 天顶偏紫红
                vec3 twilightHorizon = vec3(1.0, 0.6, 0.3); // 地平线橙黄
                vec3 twilightGround = dayGround * 0.3; // 地平线橙黄

                vec3 topColor = mix(twilightTop, dayTop, dayFactor);
                vec3 horizonColor = mix(twilightHorizon, dayHorizon, dayFactor);
                vec3 groundColor = mix(twilightGround, dayGround, dayFactor);

                float viewHeight = dir.y;
                float blend = mix(-viewHeight, 1.0 - viewHeight, step(0.0, viewHeight));
                blend = mix(pow(blend, 0.4), pow(blend, 1.5), step(0.0, viewHeight));
                blend = mix(smoothstep(0.2, 0.4, blend), blend, step(0.0, viewHeight));

                vec3 frontColor = mix(horizonColor, topColor, step(0.0, viewHeight));
                vec3 backColor = mix(groundColor, horizonColor, step(0.0, viewHeight));
                vec3 baseColor = mix(frontColor, backColor, blend);

                //vec3 baseColor;
                //if (viewHeight >= 0.0) {
                //    blend = 1.0 - viewHeight;
                //    blend = pow(blend, 1.5);
                //    baseColor = mix(topColor, horizonColor, blend);
                //} else {
                //    blend = -viewHeight;
                //    blend = pow(blend, 0.4);
                //    blend = smoothstep(0.2, 0.4, blend);
                //    baseColor = mix(horizonColor, groundColor, blend);
                //}

                // 太阳圆盘和光晕
                float sunAngle = angleBetween(dir, sunDir);
                float sunAngularRadius = 0.02;
                float sunCore = smoothstep(sunAngularRadius * 1.5, sunAngularRadius * 0.5, sunAngle);
                float cosTheta = dot(dir, sunDir);
                float mie = miePhase(cosTheta, 0.8);
                float glow = mie * 0.02;
                vec3 sunColor = mix(vec3(1.0, 0.5, 0.2), vec3(1.0, 0.9, 0.8), dayFactor);
                vec3 sunContribution = sunCore * sunColor + glow * sunColor;

                // 星空
                float starVisibility = clamp(-sunHeight * 2.0, 0.0, 1.0);
                vec3 stars = vec3(0.0);
                if (starVisibility > 0.01) {
                    vec2 angles = dirToAngles(dir);
                    float theta = angles.x;
                    float phi = angles.y;
                    float gridSize = 200.0;
                    vec2 gridPos = vec2(phi, theta) * gridSize;
                    vec2 cell = floor(gridPos);
                    vec2 sub = fract(gridPos);
                    float distToCenter = length(sub - 0.5);
                    float rand = hash(cell + floor(linda_Time.y * 0.1)); // 轻微闪烁
                    if (rand > 0.995 && distToCenter < 0.1) {
                        float brightness = rand * 0.8 + 0.2;
                        float starShape = smoothstep(0.1, 0.0, distToCenter);
                        stars = vec3(1.0) * brightness * starShape;
                    }
                }

                vec3 finalColor = baseColor + sunContribution;
                finalColor += stars * starVisibility;

                FragColor = vec4(finalColor, 1.0);
			}
		}
	}
}  
