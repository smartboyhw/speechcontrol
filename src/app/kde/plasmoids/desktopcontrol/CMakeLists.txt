project(SCDesktopControlPlasmoid)

# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_subdirectory(data)
add_subdirectory(src)