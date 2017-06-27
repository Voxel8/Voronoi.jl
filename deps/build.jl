#! /usr/bin/env julia

path = joinpath(dirname(@__FILE__), "..", "src")

@static if is_windows()
    import WinRPM
    info("Compiling with WinRPM gcc-c++")
    WinRPM.install("gcc-c++"; yes = true)
    WinRPM.install("gcc"; yes = true)
    WinRPM.install("headers"; yes = true)
    
    gpp = Pkg.dir("WinRPM","deps","usr","x86_64-w64-mingw32","sys-root","mingw","bin","g++")
    RPMbindir = Pkg.dir("WinRPM","deps","usr","x86_64-w64-mingw32","sys-root","mingw","bin")
    incdir = Pkg.dir("WinRPM","deps","usr","x86_64-w64-mingw32","sys-root","mingw","include")
    
    push!(Base.Libdl.DL_LOAD_PATH, RPMbindir)
    ENV["PATH"] = ENV["PATH"] * ";" * RPMbindir
    if success(`$gpp --version`)
        cd(path) do
            run(`$gpp -c -fPIC -std=c++11 voronoi.cpp -I. -I $incdir`)
            run(`$gpp -shared -o voronoi.dll voronoi.o`)
        end
    else
        error("Could not run gcc.")
    end
end

@static if is_unix()
    cd(path) do
        # Note: on Mac OS X, g++ is aliased to clang++.
        run(`g++ --version`)
        run(`g++ -c -fPIC -I. voronoi.cpp`)
        run(`g++ -shared -o voronoi.so voronoi.o`)
    end
end
