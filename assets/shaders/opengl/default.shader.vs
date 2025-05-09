@prism(type='fragment', name='Fast3D Fragment Shader', version='1.0.0', description='Ported shader to prism', author='Emill & Prism Team')

@{GLSL_VERSION}

@{attr} vec4 aVtxPos;

@for(i in 0..2)
    @if(o_textures[i])
        @{attr} vec2 aTexCoord@{i};
        @{out} vec2 vTexCoord@{i};
        @{update_floats(2)}
        @for(j in 0..2)
            @if(o_clamp[i][j])
                @if(j == 0)
                    @{attr} float aTexClampS@{i};
                    @{out} float vTexClampS@{i};
                @else
                    @{attr} float aTexClampT@{i};
                    @{out} float vTexClampT@{i};
                @end
                @{update_floats(1)}
            @end
        @end
    @end
@end

@if(o_fog)
    @{attr} vec4 aFog;
    @{out} vec4 vFog;
    @{update_floats(4)}
@end

@if(o_grayscale)
    @{attr} vec4 aGrayscaleColor;
    @{out} vec4 vGrayscaleColor;
    @{update_floats(4)}
@end

@for(i in 0..o_inputs)
    @if(o_alpha)
        @{attr} vec4 aInput@{i + 1};
        @{out} vec4 vInput@{i + 1};
        @{update_floats(4)}
    @else
        @{attr} vec3 aInput@{i + 1};
        @{out} vec3 vInput@{i + 1};
        @{update_floats(3)}
    @end
@end

void main() {
     @for(i in 0..2)
        @if(o_textures[i])
            vTexCoord@{i} = aTexCoord@{i};
            @for(j in 0..2)
                @if(o_clamp[i][j])
                    @if(j == 0)
                        vTexClampS@{i} = aTexClampS@{i};
                    @else
                        vTexClampT@{i} = aTexClampT@{i};
                    @end
                @end
            @end
        @end
    @end
    @if(o_fog)
        vFog = aFog;
    @end
    @if(o_grayscale)
        vGrayscaleColor = aGrayscaleColor;
    @end
    @for(i in 0..o_inputs)
        vInput@{i + 1} = aInput@{i + 1};
    @end
    gl_Position = aVtxPos;
    @if(opengles)
        gl_Position.z *= 0.3f;
    @end
}