#! /usr/bin/env julia

pkg_dir = Pkg.dir("Voronoi")

@windows_only begin
    try
      run(`where cl.exe`)
    catch
      error("ERROR: Could not find cl.exe.")
    end

    path = "$(pkg_dir)\\src\\"
    run(`cl.exe /D_USRDLL /D_WINDLL /EHsc /Fo$(path) $(path)voronoi.cpp /MT /link /DLL /OUT:$(path)voronoi.dll`)
end
