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

@unix_only begin
    cd("$(pkg_dir)/src") do
        run(`g++ -c -fPIC -std=c++11 -stdlib=libc++ -I. voronoi.cpp`)
        run(`g++ -shared -o voronoi.so voronoi.o`)
    end
end
