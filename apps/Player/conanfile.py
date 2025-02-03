from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
import os

# Получаем путь к директории, в которой находится скрипт
name_s = os.path.dirname(os.path.abspath(__file__))

class MyLibConan(ConanFile):
    name = name_s
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"
    exports_sources = "CMakeLists.txt", "include/*", "src/*"

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = [name_s]
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.set_property("cmake_file_name", name_s)
        self.cpp_info.set_property("cmake_target_name", name_s + "::" + name_s)