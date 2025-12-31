set_project("jitperm")
set_version("0.1.0")

set_languages("c++23")

add_rules("mode.debug", "mode.release")

target("jitperm")
    set_kind("binary")
    add_files("src/*.cpp")
