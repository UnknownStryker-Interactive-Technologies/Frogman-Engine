struct HS_CONTROL_POINT_OUTPUT
{
    float4 position : POSITION;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float edges[3] : SV_TessFactor;
    float inside   : SV_InsideTessFactor;
};

[domain("tri")]
float4 ds_main(HS_CONSTANT_DATA_OUTPUT patchConst, float3 loc : SV_DomainLocation, const OutputPatch<HS_CONTROL_POINT_OUTPUT, 3> patch) : SV_POSITION
{
    return patch[0].position;
}