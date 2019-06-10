fn main() {
    cpp_build::Config::new()
        .include("src/pch.h")
        .include("src/app.hh")
        .file("src/app.cc")
        .flag("-std:c++17")
        .flag("/EHsc")
        .build("src/lib.rs");
}
