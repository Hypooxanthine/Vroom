# CMakeLists.txt in the 'tests' directory

cmake_minimum_required(VERSION 3.8)
# Le nom du projet pour les tests
project(VroomTests)

# Ajoutez le répertoire d'inclusion du projet principal
include_directories(${VROOM}/include)

# Liste des fichiers de test
set(TEST_SOURCES
    test_main.cpp  # un fichier contenant la fonction main() pour les tests
    test_application.cpp  # un exemple de fichier de test
)

# Créez un exécutable de test
add_executable(VroomTests ${TEST_SOURCES})

# Ajoutez les dépendances et les bibliothèques nécessaires
target_link_libraries(VroomTests
    Vroom  # le nom de la bibliothèque principale
    ${OPENGL_LIBRARY}
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
)

# Ajoutez la définition pour permettre d'inclure les fichiers d'en-tête du projet principal dans les tests
target_compile_definitions(VroomTests PUBLIC -D GLEW_STATIC)

# Ajoutez le test pour cet exécutable
add_test(NAME VroomTests COMMAND VroomTests)
