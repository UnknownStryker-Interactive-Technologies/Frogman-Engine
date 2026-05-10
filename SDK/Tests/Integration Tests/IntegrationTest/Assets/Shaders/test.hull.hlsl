struct VS_OUTPUT
{
    float4 position : POSITION;
};

struct HS_CONTROL_POINT_OUTPUT
{
    float4 position : POSITION;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float edges[3] : SV_TessFactor;
    float inside   : SV_InsideTessFactor;
};

HS_CONSTANT_DATA_OUTPUT hs_constants()
{
    HS_CONSTANT_DATA_OUTPUT o;
    o.edges[0] = o.edges[1] = o.edges[2] = 1;
    o.inside = 1;
    return o;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("hs_constants")]
HS_CONTROL_POINT_OUTPUT hs_main(InputPatch<VS_OUTPUT, 3> patch, uint i : SV_OutputControlPointID)
{
    HS_CONTROL_POINT_OUTPUT o;
    o.position = patch[i].position;
    return o;
}