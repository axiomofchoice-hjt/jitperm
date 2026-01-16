set_project("jitperm")
set_version("0.1.0")

set_languages("c++23")

add_rules("mode.debug", "mode.release")

add_requires("asmjit")

target("jitperm")
    set_kind("binary")
    add_files("src/*.cpp")
    add_files("src/kernel/*.cpp")
    add_packages("asmjit")
    add_includedirs("src", {private = true})
    add_cxxflags("-Wall", "-Wextra", "-Wpedantic", "-Werror")
