

void main()
{
	vec4 worldSpacePosition = u_Model * vec4(position, 1.0);
	vec4 viewSpacePosition = u_View * worldSpacePosition;

	gl_Position = u_Projection * viewSpacePosition;
}