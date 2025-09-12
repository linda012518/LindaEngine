Shader "MyShader/URP/P2_10"
{
    Properties {}
    SubShader
    {
        Tags
        {
            //�������棬��Shaderֻ���� URP ��Ⱦ����
            "RenderPipeline"="UniversalPipeline"
            //��Ⱦ����
            "RenderType"="Opaque"
            //��Ⱦ����
            "Queue"="Geometry"
        }
        Pass
        {
            Name "Universal Forward"
            Tags
            {
                // LightMode: <None>
            }

            Cull Back
            Blend One Zero
            ZTest LEqual
            ZWrite On
          
            HLSLPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            // Pragmas
            #pragma target 2.0
            
            // Includes
            #include "Packages/com.unity.render-pipelines.core/ShaderLibrary/Color.hlsl"
            #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Input.hlsl"

            //struct appdata
            //������ɫ��������
            struct Attributes
            {
                float3 positionOS : POSITION;
            };
            //struct v2f
            //ƬԪ��ɫ��������
            struct Varyings
            {
                float4 positionCS : SV_POSITION;
            };
            //v2f vert(Attributes v)
            //������ɫ��
            Varyings vert(Attributes v)
            {
                Varyings o = (Varyings)0;
                float3 positionWS = TransformObjectToWorld(v.positionOS);
                o.positionCS = TransformWorldToHClip(positionWS);
                return o;
            }
            //fixed4 frag(v2f i) : SV_TARGET
            //ƬԪ��ɫ��
            half4 frag(Varyings i) : SV_TARGET
            {
                half4 c;
                c.rgb = 0.5;
                c.a = 1;
                return c;
            }
            ENDHLSL
        }
    }

    FallBack "Hidden/Shader Graph/FallbackError"
}
