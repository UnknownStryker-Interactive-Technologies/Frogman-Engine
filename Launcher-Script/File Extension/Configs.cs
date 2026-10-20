namespace Launcher;




[Serializable]
public class WorldPath
{
    public string? Name { get; set; } = null;
    public string? Path { get; set; } = null;
}


[Serializable]
public class GlobalResourceLookUpTable
{
    public List<WorldPath> WorldPaths { get; set; } = new();
}




[Serializable]
public class ProjectConfig
{
    public GlobalResourceLookUpTable GlobalResourceLookUpTable { get; set; } = new();

    public string? CompressionMethod { get; set; } = null;
    public string? DecompressionMethod { get; set; } = null;

    public string? EncryptionMethod { get; set; } = null;
    public string? DecryptionMethod { get; set; } = null;

    public uint FiberStackSize { get; set; } = 1048576; // 1 MiB
    public uint FibersPerThread { get; set; } = 3;

    public WindowConfig WindowConfig { get; set; } = new();
}

[Serializable]
public class WindowConfig
{
    public string Title { get; set; } = String.Empty;
    public string[] IconPaths { get; set; } = { "Assets\\Icon\\runtime icon.png" };
    public string[] RandomPlayIntroVideoPaths { get; set; } = { "Assets\\Video\\runtime splash video 1.mp4", "Assets\\Video\\runtime splash video 2.mp4" };
    public string[] SequentialPlayIntroVideoPaths { get; set; } = Array.Empty<string>();
    public string[] ShaderCompileSplashImagePaths { get; set; } = { "Assets\\Splash\\Shader Compile Splash.png", "Assets\\Splash\\Shader Compile Splash 2.png", "Assets\\Splash\\Shader Compile Splash 3.png" };
    public uint ShaderCompileSplashImageDurationInSeconds { get; set; } = 5; // 0 is infinite
    public uint SwapChainBufferCount { get; set; } = 3;
}


// .fmodule, .ao, .flevel, .fasset, .oplan