@prism(type='fragment', name='Fast3D Fragment Shader', version='1.0.0', description='Ported shader to prism', author='Emill & Prism Team')

#version 450 core

// should stay position
layout(location = @{get_vs_input_location("position", "RGBA32Float")}) in vec4 position;

@for(i in 0..2)
    @if(o_textures[i])
        layout(location = @{get_vs_input_location("aTexCoord" + to_string(i), "RG32Float")}) in vec2 aTexCoord@{i};
        layout(location = @{get_output_location()}) out vec2 vTexCoord@{i};
        @for(j in 0..2)
            @if(o_clamp[i][j])
                @if(j == 0)
                    layout(location = @{get_vs_input_location("aTexClampS" + to_string(i), "R32Float")}) in float aTexClampS@{i};
                    layout(location = @{get_output_location()}) out float vTexClampS@{i};
                @else
                    layout(location = @{get_vs_input_location("aTexClampT" + to_string(i), "R32Float")}) in float aTexClampT@{i};
                    layout(location = @{get_output_location()}) out float vTexClampT@{i};
                @end
            @end
        @end
    @end
@end

@if(o_fog)
    layout(location = @{get_vs_input_location("aFog", "RGBA32Float")}) in vec4 aFog;
    layout(location = @{get_output_location()}) out vec4 vFog;
@end

@if(o_grayscale)
    layout(location = @{get_vs_input_location("aGrayscaleColor", "RGBA32Float")}) in vec4 aGrayscaleColor;
    layout(location = @{get_output_location()}) out vec4 vGrayscaleColor;
@end

@for(i in 0..o_inputs)
    @if(o_alpha)
        layout(location = @{get_vs_input_location("aInput" + to_string(i+1), "RGBA32Float")}) in vec4 aInput@{i + 1};
        layout(location = @{get_output_location()}) out vec4 vInput@{i + 1};
    @else
        layout(location = @{get_vs_input_location("aInput" + to_string(i+1), "RGB32Float")}) in vec3 aInput@{i + 1};
        layout(location = @{get_output_location()}) out vec3 vInput@{i + 1};
    @end
@end

out gl_PerVertex {
    vec4 gl_Position;
};

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
    gl_Position = position;
}
