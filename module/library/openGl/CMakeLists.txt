defineModule("library_openGl")

addModuleDependency("library_openGl" "library_gui")
addModuleDependency("library_openGl" "3rdParty_stb_image")
addDependency("library_openGl" OpenGL::GL)
addDependency("library_openGl" OpenGL::GLU)

defineExecutableForModule("library_openGl")