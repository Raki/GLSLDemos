#version 460

out vec4 fragColor;
uniform vec2 resolution;


#define TRIANGLE_COUNT 30


// Computes the direction vector of a primary ray
// \param x, y Coordinates between 0 and 1 for the pixel that the ray
//      passes through (x, y)
// \param camera_position Position vector for the camera (o)
// \param left_bottom Position vector for the left bottom corner of
//      the image plane (c)
// \param right, up Direction vectors towards the right and top end
//      of the image plane (r, u)
// \return A direction vector of length 1 for a ray through the
//      specified pixel (d)
vec3 get_primary_ray_direction(float x, float y, vec3 camera_position, vec3 left_bottom, vec3 right, vec3 up) {

    vec3 p = left_bottom + (x*right) + (y*up);
    
    return normalize(p-camera_position);
                                                                                                                                                                                                                                                    vec3 image_plane_pos = left_bottom + x * right + y * up;     return normalize(image_plane_pos - camera_position);
}


// A triangle along with some shading parameters
struct triangle_t {
    // The positions of the three vertices (v_0, v_1, v_2)
    vec3 positions[3];
    // A vector of length 1, orthogonal to the triangle (n)
    vec3 normal;
    // The albedo of the triangle (i.e. the fraction of
    // red/green/blue light that gets reflected) (a)
    vec3 color;
    // The radiance emitted by the triangle (for light sources) (L_e)
    vec3 emission;
};


// Checks whether a ray intersects a triangle
// \param out_t The ray parameter at the intersection (if any) (t)
// \param origin The position at which the ray starts (o)
// \param direction The direction vector of the ray (d)
// \param tri The triangle for which to check an intersection
// \return true if there is an intersection, false otherwise
bool ray_triangle_intersection(out float out_t, vec3 origin, vec3 direction, triangle_t tri) {
        out_t = 0.0;
    vec3 e1 = tri.positions[1]-tri.positions[0];
    vec3 e2 = tri.positions[2]-tri.positions[0];
    mat3 mat = mat3(-direction,e1,e2);
    mat3 invMat = inverse(mat);
    //t, l1,l2
    vec3 res = invMat*(origin-tri.positions[0]);
    out_t = res.x;
    // TODO
    return (out_t>0.)&&(res.y>0.)&&(res.z>0.)&&(res.y+res.z<=1.);
    // TODO
    
}


// Checks whether a ray intersects any triangle of the mesh
// \param out_t The ray parameter at the closest intersection (if any) (t)
// \param out_tri The closest triangle that was intersected (if any)
// \param origin The position at which the ray starts (o)
// \param direction The direction vector of the ray (d)
// \return true if there is an intersection, false otherwise
bool ray_mesh_intersection(out float out_t, out triangle_t out_tri, vec3 origin, vec3 direction) {
    // Definition of the mesh geometry (exported from Blender)
    triangle_t tris[TRIANGLE_COUNT];
    tris[0].positions[0] = vec3(0.000000133, -0.559199989, 0.548799932); tris[0].positions[1] = vec3(0.555999935, -0.559199989, 0.000000040); tris[0].positions[2] = vec3(0.000000133, -0.559199989, 0.000000040); tris[1].positions[0] = vec3(0.313999921, -0.455999970, 0.329999954); tris[1].positions[1] = vec3(0.313999921, -0.455999970, 0.000000040); tris[1].positions[2] = vec3(0.472000152, -0.406000137, 0.000000040); tris[2].positions[0] = vec3(0.000000133, -0.559199989, 0.000000040); tris[2].positions[1] = vec3(0.555999935, -0.559199989, 0.000000040); tris[2].positions[2] = vec3(0.555999935, -0.000000119, 0.000000040); tris[3].positions[0] = vec3(0.264999926, -0.296000093, 0.329999954); tris[3].positions[1] = vec3(0.264999926, -0.296000093, 0.000000040); tris[3].positions[2] = vec3(0.313999921, -0.455999970, 0.000000040); tris[4].positions[0] = vec3(0.423000127, -0.246999890, 0.000000040); tris[4].positions[1] = vec3(0.472000152, -0.406000137, 0.329999954); tris[4].positions[2] = vec3(0.472000152, -0.406000137, 0.000000040); tris[5].positions[0] = vec3(0.264999926, -0.296000093, 0.329999954); tris[5].positions[1] = vec3(0.313999921, -0.455999970, 0.000000040); tris[5].positions[2] = vec3(0.313999921, -0.455999970, 0.329999954); tris[6].positions[0] = vec3(0.313999921, -0.455999970, 0.329999954); tris[6].positions[1] = vec3(0.472000152, -0.406000137, 0.000000040); tris[6].positions[2] = vec3(0.472000152, -0.406000137, 0.329999954); tris[7].positions[0] = vec3(0.240000039, -0.271999955, 0.165000007); tris[7].positions[1] = vec3(0.082000092, -0.225000143, 0.165000007); tris[7].positions[2] = vec3(0.082000092, -0.225000143, 0.000000040); tris[8].positions[0] = vec3(0.240000039, -0.271999955, 0.165000007); tris[8].positions[1] = vec3(0.082000092, -0.225000143, 0.000000040); tris[8].positions[2] = vec3(0.240000039, -0.271999955, 0.000000040); tris[9].positions[0] = vec3(0.290000081, -0.113999903, 0.000000040); tris[9].positions[1] = vec3(0.240000039, -0.271999955, 0.165000007); tris[9].positions[2] = vec3(0.240000039, -0.271999955, 0.000000040); tris[10].positions[0] = vec3(0.082000092, -0.225000143, 0.000000040); tris[10].positions[1] = vec3(0.130000070, -0.064999968, 0.165000007); tris[10].positions[2] = vec3(0.130000070, -0.064999968, 0.000000040); tris[11].positions[0] = vec3(0.082000092, -0.225000143, 0.000000040); tris[11].positions[1] = vec3(0.082000092, -0.225000143, 0.165000007); tris[11].positions[2] = vec3(0.130000070, -0.064999968, 0.165000007); tris[12].positions[0] = vec3(0.000000133, -0.559199989, 0.000000040); tris[12].positions[1] = vec3(0.555999935, -0.000000119, 0.000000040); tris[12].positions[2] = vec3(0.000000133, -0.000000119, 0.000000040); tris[13].positions[0] = vec3(0.130000070, -0.064999968, 0.000000040); tris[13].positions[1] = vec3(0.290000081, -0.114000171, 0.165000007); tris[13].positions[2] = vec3(0.290000081, -0.113999903, 0.000000040); tris[14].positions[0] = vec3(0.290000081, -0.113999903, 0.000000040); tris[14].positions[1] = vec3(0.290000081, -0.114000171, 0.165000007); tris[14].positions[2] = vec3(0.240000039, -0.271999955, 0.165000007); tris[15].positions[0] = vec3(0.130000070, -0.064999968, 0.000000040); tris[15].positions[1] = vec3(0.130000070, -0.064999968, 0.165000007); tris[15].positions[2] = vec3(0.290000081, -0.114000171, 0.165000007); tris[16].positions[0] = vec3(0.000000133, -0.559199989, 0.000000040); tris[16].positions[1] = vec3(0.000000133, -0.000000119, 0.000000040); tris[16].positions[2] = vec3(0.000000133, -0.000000119, 0.548799932); tris[17].positions[0] = vec3(0.130000070, -0.064999968, 0.165000007); tris[17].positions[1] = vec3(0.082000092, -0.225000143, 0.165000007); tris[17].positions[2] = vec3(0.240000039, -0.271999955, 0.165000007); tris[18].positions[0] = vec3(0.130000070, -0.064999968, 0.165000007); tris[18].positions[1] = vec3(0.240000039, -0.271999955, 0.165000007); tris[18].positions[2] = vec3(0.290000081, -0.114000171, 0.165000007); tris[19].positions[0] = vec3(0.423000127, -0.247000158, 0.329999954); tris[19].positions[1] = vec3(0.423000127, -0.246999890, 0.000000040); tris[19].positions[2] = vec3(0.264999926, -0.296000093, 0.000000040); tris[20].positions[0] = vec3(0.423000127, -0.247000158, 0.329999954); tris[20].positions[1] = vec3(0.264999926, -0.296000093, 0.000000040); tris[20].positions[2] = vec3(0.264999926, -0.296000093, 0.329999954); tris[21].positions[0] = vec3(0.423000127, -0.246999890, 0.000000040); tris[21].positions[1] = vec3(0.423000127, -0.247000158, 0.329999954); tris[21].positions[2] = vec3(0.472000152, -0.406000137, 0.329999954); tris[22].positions[0] = vec3(0.555999935, -0.000000119, 0.548799932); tris[22].positions[1] = vec3(0.555999935, -0.000000119, 0.000000040); tris[22].positions[2] = vec3(0.555999935, -0.559199989, 0.000000040); tris[23].positions[0] = vec3(0.000000133, -0.559199989, 0.000000040); tris[23].positions[1] = vec3(0.000000133, -0.000000119, 0.548799932); tris[23].positions[2] = vec3(0.000000133, -0.559199989, 0.548799932); tris[24].positions[0] = vec3(0.000000133, -0.000000119, 0.548799932); tris[24].positions[1] = vec3(0.555999935, -0.559199989, 0.548799932); tris[24].positions[2] = vec3(0.000000133, -0.559199989, 0.548799932); tris[25].positions[0] = vec3(0.000000133, -0.559199989, 0.548799932); tris[25].positions[1] = vec3(0.555999935, -0.559199989, 0.548799932); tris[25].positions[2] = vec3(0.555999935, -0.559199989, 0.000000040); tris[26].positions[0] = vec3(0.472000152, -0.406000137, 0.329999954); tris[26].positions[1] = vec3(0.264999926, -0.296000093, 0.329999954); tris[26].positions[2] = vec3(0.313999921, -0.455999970, 0.329999954); tris[27].positions[0] = vec3(0.555999935, -0.000000119, 0.548799932); tris[27].positions[1] = vec3(0.555999935, -0.559199989, 0.000000040); tris[27].positions[2] = vec3(0.555999935, -0.559199989, 0.548799932); tris[28].positions[0] = vec3(0.472000152, -0.406000137, 0.329999954); tris[28].positions[1] = vec3(0.423000127, -0.247000158, 0.329999954); tris[28].positions[2] = vec3(0.264999926, -0.296000093, 0.329999954); tris[29].positions[0] = vec3(0.000000133, -0.000000119, 0.548799932); tris[29].positions[1] = vec3(0.555999935, -0.000000119, 0.548799932); tris[29].positions[2] = vec3(0.555999935, -0.559199989, 0.548799932);
    tris[0].normal = vec3(0.0, 1.0, 0.0); tris[1].normal = vec3(0.301707575, -0.953400513, 0.0); tris[2].normal = vec3(0.0, 0.0, 1.0); tris[3].normal = vec3(-0.956165759, -0.292825958, -0.0); tris[4].normal = vec3(0.955649049, 0.294507888, 0.0); tris[5].normal = vec3(-0.956165759, -0.292825958, 0.0); tris[6].normal = vec3(0.301707575, -0.953400513, 0.0); tris[7].normal = vec3(-0.285119946, -0.958491845, 0.0); tris[8].normal = vec3(-0.285119946, -0.958491845, -0.0); tris[9].normal = vec3(0.953400053, -0.301709030, 0.0); tris[10].normal = vec3(-0.957826408, 0.287347476, 0.0); tris[11].normal = vec3(-0.957826408, 0.287347476, 0.0); tris[12].normal = vec3(0.0, 0.0, 1.0); tris[13].normal = vec3(0.292825408, 0.956165927, 0.000001554); tris[14].normal = vec3(0.953399906, -0.301709496, -0.000000490); tris[15].normal = vec3(0.292826874, 0.956165478, -0.0); tris[16].normal = vec3(1.0, 0.0, 0.0); tris[17].normal = vec3(0.0, 0.0, 1.0); tris[18].normal = vec3(0.0, 0.0, 1.0); tris[19].normal = vec3(-0.296209850, 0.955122885, 0.000000776); tris[20].normal = vec3(-0.296208371, 0.955123343, 0.0); tris[21].normal = vec3(0.955648909, 0.294508341, 0.000000239); tris[22].normal = vec3(-1.0, 0.0, -0.0); tris[23].normal = vec3(1.0, 0.0, 0.0); tris[24].normal = vec3(0.0, 0.0, -1.0); tris[25].normal = vec3(-0.0, 1.0, 0.0); tris[26].normal = vec3(0.0, 0.0, 1.0); tris[27].normal = vec3(-1.0, -0.0, 0.0); tris[28].normal = vec3(0.0, 0.0, 1.0); tris[29].normal = vec3(0.0, 0.0, -1.0);
    tris[0].color = vec3(0.874000013, 0.874000013, 0.875000000); tris[1].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[2].color = vec3(0.874000013, 0.874000013, 0.875000000); tris[3].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[4].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[5].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[6].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[7].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[8].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[9].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[10].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[11].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[12].color = vec3(0.874000013, 0.874000013, 0.875000000); tris[13].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[14].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[15].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[16].color = vec3(0.289999992, 0.663999975, 0.324999988); tris[17].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[18].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[19].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[20].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[21].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[22].color = vec3(0.730000019, 0.246000007, 0.250999987); tris[23].color = vec3(0.289999992, 0.663999975, 0.324999988); tris[24].color = vec3(0.0, 0.0, 0.0); tris[25].color = vec3(0.874000013, 0.874000013, 0.875000000); tris[26].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[27].color = vec3(0.730000019, 0.246000007, 0.250999987); tris[28].color = vec3(0.839039981, 0.839039981, 0.839999974); tris[29].color = vec3(0.0, 0.0, 0.0);
    tris[0].emission = vec3(0.0, 0.0, 0.0); tris[1].emission = vec3(0.0, 0.0, 0.0); tris[2].emission = vec3(0.0, 0.0, 0.0); tris[3].emission = vec3(0.0, 0.0, 0.0); tris[4].emission = vec3(0.0, 0.0, 0.0); tris[5].emission = vec3(0.0, 0.0, 0.0); tris[6].emission = vec3(0.0, 0.0, 0.0); tris[7].emission = vec3(0.0, 0.0, 0.0); tris[8].emission = vec3(0.0, 0.0, 0.0); tris[9].emission = vec3(0.0, 0.0, 0.0); tris[10].emission = vec3(0.0, 0.0, 0.0); tris[11].emission = vec3(0.0, 0.0, 0.0); tris[12].emission = vec3(0.0, 0.0, 0.0); tris[13].emission = vec3(0.0, 0.0, 0.0); tris[14].emission = vec3(0.0, 0.0, 0.0); tris[15].emission = vec3(0.0, 0.0, 0.0); tris[16].emission = vec3(0.0, 0.0, 0.0); tris[17].emission = vec3(0.0, 0.0, 0.0); tris[18].emission = vec3(0.0, 0.0, 0.0); tris[19].emission = vec3(0.0, 0.0, 0.0); tris[20].emission = vec3(0.0, 0.0, 0.0); tris[21].emission = vec3(0.0, 0.0, 0.0); tris[22].emission = vec3(0.0, 0.0, 0.0); tris[23].emission = vec3(0.0, 0.0, 0.0); tris[24].emission = vec3(3.0, 3.0, 3.0); tris[25].emission = vec3(0.0, 0.0, 0.0); tris[26].emission = vec3(0.0, 0.0, 0.0); tris[27].emission = vec3(0.0, 0.0, 0.0); tris[28].emission = vec3(0.0, 0.0, 0.0); tris[29].emission = vec3(3.0, 3.0, 3.0);
    // Find the nearest intersection across all triangles
    float infinity = 1.0e38;
    out_t = infinity;
    bool result = false;
    
    for(int i=0;i<TRIANGLE_COUNT;i++)
    {
        triangle_t tri = tris[i];
        float local_t;
        if(ray_triangle_intersection(local_t,origin,direction,tri))
        {
            if(local_t<out_t)
            {
                out_t = local_t;
                out_tri = tri;
                result = true;
            }
        }
    }
    
    return result;
}

void main()
{
    vec3 camera_position = vec3(0.278, 0.8, 0.2744);
    vec3 middle = camera_position - vec3(0.0, 0.8, 0.0);
    vec3 up = vec3(0.0, 0.0, 0.56);
    float aspect = resolution.x / resolution.y;
    vec3 right = aspect * vec3(-0.56, 0.0, 0.0);
    vec3 left_bottom = middle - 0.5 * right - 0.5 * up;
    // Compute the camera ray
    vec2 tex_coord = gl_FragCoord.xy/resolution;
    vec3 ray_direction = get_primary_ray_direction(
        tex_coord.x, tex_coord.y, camera_position, left_bottom, right, up);
    // Trace a ray and display the triangle color+emission
    triangle_t tri;
    float t;
    fragColor.rgb = vec3(0.0);
    if (ray_mesh_intersection(t, tri, camera_position, ray_direction))
        fragColor.rgb = tri.color + tri.emission;
    fragColor.a = 1.0;
}