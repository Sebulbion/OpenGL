#version 430 core
in vec3 a_position;
in vec3 a_Color;
//in vec2 a_texCord;

out VS_GS_VERTEX{
	out vec4 position;
	out vec3 color;
	out mat4 mvp;
} vs_out;

 uniform mat4 gScale;
 uniform mat4 gRotate;
 uniform mat4 gTranslate;
 uniform mat4 gView;
 uniform mat4 gPerspective;

void main(){
	gl_Position= gPerspective * gView * gTranslate * gRotate * gScale * vec4(a_position, 1.0f);
	vs_out.mvp = gPerspective * gView * gTranslate * gRotate * gScale;
	vs_out.color= a_Color;
	vs_out.position= gl_Position;
}