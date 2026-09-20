/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
using System.Diagnostics;
using System.IO.Compression;
using Installer;




namespace Installer.Script
{
    static class Absl
    {
        public static readonly string Version = "20260107.1";
    }
    static class Boost
    {
        public static readonly string Version = "1.92.0";
        public static readonly string Url = "https://archives.boost.io/release/1.92.0/source/boost_1_92_0.zip";
        // public static readonly string DebugBuildB2Options = "architecture=x86 address-model=64 link=static runtime-link=static threading=multi variant=debug";
        // public static readonly string ReleaseBuildB2Options = "architecture=x86 address-model=64 link=static runtime-link=static threading=multi variant=release";
    }
    static class Cpptrace
    {
        public static readonly string Version = "1.0.4";
    }
    static class Glfw
    {
        public static readonly string Version = "3.4";
        public static readonly string Url = "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip";

    }
    static class Lz4
    {
        public static readonly string Version = "1.10.0";
    }
    static class ImGui
    {
        public static readonly string Version = "1.91.6";
    }
    static class JsonCpp
    {
        public static readonly string Version = "1.9.8";
    }
    static class FrogmanEngine
    {
        public static readonly string ThirdPartyFolderRelativePath = "SDK\\Third-Party\\Libraries";
    }




    public class Main : Installer.ScriptMain
    {
        
        string thirdPartyFolderPath = String.Empty;
        public override Queue<Script.Job> ScheduleJobs(Script.JobParameters parameters)
        {
            thirdPartyFolderPath = System.IO.Path.Combine(parameters.GdkInstallationPath, FrogmanEngine.ThirdPartyFolderRelativePath);
            EnqueueThirdPartyLibraryBuildJobs(parameters);
            EnqueueFrogmanGdkBuildJobs(parameters);
            return jobs;
        }

        private void EnqueueFrogmanGdkBuildJobs(JobParameters parameters)
        {
            Process process = new Process();
            process.StartInfo = new ProcessStartInfo
            {
                FileName = parameters.BuildBatchFileName,
                RedirectStandardOutput = false,
                UseShellExecute = true,
                CreateNoWindow = false
            };


            Script.Job feAudioBuildJob = new Script.Job
            {
                DisplayedMessage = $"Building Frogman Engine Audio...",
                Run = (Script.JobParameters parameters) =>
                {
                    Directory.SetCurrentDirectory(System.IO.Path.Combine(parameters.GdkInstallationPath, "SDK\\Audio\\CMake"));
                    process.Start();
                    process.WaitForExit();
                }
            }; jobs.Enqueue(feAudioBuildJob);


            Script.Job feCoreBuildJob = new Script.Job
            {
                DisplayedMessage = $"Building Frogman Engine Core...",
                Run = (Script.JobParameters parameters) =>
                {
                    Directory.SetCurrentDirectory(System.IO.Path.Combine(parameters.GdkInstallationPath, "SDK\\Core\\CMake"));
                    process.Start();
                    process.WaitForExit();
                }
            }; jobs.Enqueue(feCoreBuildJob);


            Script.Job feFrameworkBuildJob = new Script.Job
            {
                DisplayedMessage = $"Building Frogman Engine Framework...",
                Run = (Script.JobParameters parameters) =>
                {
                    Directory.SetCurrentDirectory(System.IO.Path.Combine(parameters.GdkInstallationPath, "SDK\\Framework\\CMake"));
                    process.Start();
                    process.WaitForExit();
                }
            }; jobs.Enqueue(feFrameworkBuildJob);


            Script.Job feRendererBuildJob = new Script.Job
            {
                DisplayedMessage = $"Building Frogman Engine Renderer...",
                Run = (Script.JobParameters parameters) =>
                {
                    Directory.SetCurrentDirectory(System.IO.Path.Combine(parameters.GdkInstallationPath, "SDK\\Renderer\\CMake"));
                    process.Start();
                    process.WaitForExit();
                }
            }; jobs.Enqueue(feRendererBuildJob);


            Script.Job feEngineBuildJob = new Script.Job
            {
                DisplayedMessage = $"Building Frogman Engine...",
                Run = (Script.JobParameters parameters) =>
                {
                    Directory.SetCurrentDirectory(System.IO.Path.Combine(parameters.GdkInstallationPath, "SDK\\Engine\\CMake"));
                    process.Start();
                    process.WaitForExit();
                }
            }; jobs.Enqueue(feEngineBuildJob);


            Script.Job frogmanHeaderToolBuildJob = new Script.Job
            {
                DisplayedMessage = $"Building Frogman Engine Header Tool...",
                Run = (Script.JobParameters parameters) =>
                {
                    Directory.SetCurrentDirectory(System.IO.Path.Combine(parameters.GdkInstallationPath, "SDK\\Header-Tool\\CMake"));
                    process.Start();
                    process.WaitForExit();
                }
            }; jobs.Enqueue(frogmanHeaderToolBuildJob);
        }

        private void EnqueueThirdPartyLibraryBuildJobs(JobParameters parameters)
        {
            Process process = new Process();
            process.StartInfo = new ProcessStartInfo
            {
                FileName = parameters.BuildBatchFileName,
                RedirectStandardOutput = false,
                UseShellExecute = true,
                CreateNoWindow = false
            };


            Script.Job abslBuildJob = new Script.Job
            {
                DisplayedMessage = $"Building the ABSL version {Absl.Version} ...",
                Run = (Script.JobParameters parameters) =>
                {
                    process.StartInfo.Arguments = String.Empty;
                    process.StartInfo.FileName = parameters.BuildBatchFileName;
                    string abslPath = System.IO.Path.Combine(thirdPartyFolderPath,
                                          $"abseil-cpp-{Absl.Version}");
                    Directory.SetCurrentDirectory(abslPath);
                    process.Start();
                    process.WaitForExit();
                }
            }; jobs.Enqueue(abslBuildJob);


            Script.Job boostDownloadJob = new Script.Job
            {
                DisplayedMessage = $"Downloading the Boost Libraries version {Boost.Version} ...",
                Run = (Script.JobParameters parameters) => DownloadBoost(parameters)
            }; jobs.Enqueue(boostDownloadJob);


            Script.Job cpptraceBuildJob = new Script.Job
            {
                DisplayedMessage = $"Building the Cpptrace version {Cpptrace.Version} ...",
                Run = (Script.JobParameters parameters) =>
                {
                    process.StartInfo.Arguments = String.Empty;
                    process.StartInfo.FileName = parameters.BuildBatchFileName;
                    string cpptracePath = System.IO.Path.Combine(thirdPartyFolderPath,
                                          $"cpptrace-{Cpptrace.Version}");
                    Directory.SetCurrentDirectory(cpptracePath);
                    process.Start();
                    process.WaitForExit();
                }
            }; jobs.Enqueue(cpptraceBuildJob);
            //Script.Job boostDebugBuildJob = new Script.Job
            //{
            //    DisplayedMessage = $"Building debug version of the Boost libraries with {Boost.DebugBuildB2Options} ...",
            //    Run = (Script.JobParameters parameters) =>
            //    {
            //        process.StartInfo.FileName = "b2.exe";
            //        process.StartInfo.Arguments = Boost.DebugBuildB2Options;
            //        process.StartInfo.Arguments += " ";
            //        switch (parameters.VisualStudioVersion)
            //        {
            //            case VisualStudioVersion.VisualStudio2022:
            //                process.StartInfo.Arguments += "toolset=msvc-14.3";
            //                break;

            //            case VisualStudioVersion.VisualStudio2026:
            //                process.StartInfo.Arguments += "toolset=msvc-14.5";
            //                break;
            //        }
            //        process.Start();
            //        process.WaitForExit();
            //    }
            //}; jobs.Enqueue(boostDebugBuildJob);

            //Script.Job boostReleaseBuildJob = new Script.Job
            //{
            //    DisplayedMessage = $"Building release version of the Boost libraries with {Boost.ReleaseBuildB2Options} ...",
            //    Run = (Script.JobParameters parameters) =>
            //    {
            //        process.StartInfo.FileName = "b2.exe";
            //        process.StartInfo.Arguments = Boost.ReleaseBuildB2Options;
            //        process.StartInfo.Arguments += " ";
            //        switch (parameters.VisualStudioVersion)
            //        {
            //            case VisualStudioVersion.VisualStudio2022:
            //                process.StartInfo.Arguments += "toolset=msvc-14.3";
            //                break;

            //            case VisualStudioVersion.VisualStudio2026:
            //                process.StartInfo.Arguments += "toolset=msvc-14.5";
            //                break;
            //        }
            //        process.Start();
            //        process.WaitForExit();
            //    }
            //}; jobs.Enqueue(boostReleaseBuildJob);


            Script.Job imGuiBuildJob = new Script.Job
            {
                DisplayedMessage = $"Building the ImGui version {ImGui.Version} ...",
                Run = (Script.JobParameters parameters) =>
                {
                    process.StartInfo.Arguments = String.Empty;
                    process.StartInfo.FileName = parameters.BuildBatchFileName;
                    string imguiPath = System.IO.Path.Combine(thirdPartyFolderPath,
                                          $"imgui-{ImGui.Version}");
                    Directory.SetCurrentDirectory(imguiPath);

                    process.Start();
                    process.WaitForExit();
                }
            }; jobs.Enqueue(imGuiBuildJob);


            Script.Job jsonCppBuildJob = new Script.Job
            {
                DisplayedMessage = $"Building the JsonCpp version {JsonCpp.Version} ...",
                Run = (Script.JobParameters parameters) =>
                {
                    process.StartInfo.Arguments = String.Empty;
                    process.StartInfo.FileName = parameters.BuildBatchFileName;
                    string jsonCppPath = System.IO.Path.Combine(thirdPartyFolderPath,
                                          $"jsoncpp-{JsonCpp.Version}");
                    Directory.SetCurrentDirectory(jsonCppPath);
                    process.Start();
                    process.WaitForExit();
                }
            }; jobs.Enqueue(jsonCppBuildJob);


            Script.Job glfwBuildJob = new Script.Job
            {
                DisplayedMessage = $"Building the GLFW version {Glfw.Version} ...",
                Run = (Script.JobParameters parameters) =>
                {
                    process.StartInfo.Arguments = String.Empty;
                    process.StartInfo.FileName = parameters.BuildBatchFileName;
                    string glfwPath = System.IO.Path.Combine(thirdPartyFolderPath,
                                          $"glfw-{Glfw.Version}");
                    Directory.SetCurrentDirectory(glfwPath);

                    process.Start();
                    process.WaitForExit();
                }
            }; jobs.Enqueue(glfwBuildJob);


            Script.Job lz4BuildJob = new Script.Job
            {
                DisplayedMessage = $"Building the LZ4 version {Lz4.Version} ...",
                Run = (Script.JobParameters parameters) =>
                {
                    process.StartInfo.Arguments = String.Empty;
                    process.StartInfo.FileName = parameters.BuildBatchFileName;
                    string lz4Path = System.IO.Path.Combine(thirdPartyFolderPath,
                                             $"lz4-{Lz4.Version}");
                    Directory.SetCurrentDirectory(Path.Combine(lz4Path, "build\\cmake"));

                    process.Start();
                    process.WaitForExit();
                }
            }; jobs.Enqueue(lz4BuildJob);
        }

        private void DownloadBoost(JobParameters parameters)
        {
            string boostFolderName = $"boost-{Boost.Version}";
            string boostZipFileName = boostFolderName + ".zip";
            string boostZipFilePath = System.IO.Path.Combine(thirdPartyFolderPath, boostZipFileName);
            DownloadFromWeb(Boost.Url, thirdPartyFolderPath, boostZipFileName);
            ZipFile.ExtractToDirectory(boostZipFilePath, thirdPartyFolderPath);
            File.Delete(boostZipFilePath);

            string underscored = boostFolderName.Replace('.', '_');
            underscored = underscored.Replace('-', '_');
            underscored = System.IO.Path.Combine(thirdPartyFolderPath, underscored);

            string boostFolderPath = System.IO.Path.Combine(thirdPartyFolderPath, boostFolderName);
            if (Directory.Exists(underscored))
            {
                Directory.Move(underscored, boostFolderPath);
            }

            //Directory.SetCurrentDirectory(boostFolderPath);
            //Process process = new Process();
            //process.StartInfo = new ProcessStartInfo
            //{
            //    FileName = "bootstrap.bat",
            //    RedirectStandardOutput = true,
            //    UseShellExecute = false,
            //    CreateNoWindow = true
            //};
            //process.Start();
            //EnqueueMessage(process.StandardOutput.ReadToEnd());
            //process.WaitForExit();
        }
    }
}
