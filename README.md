# sdl-projects

A collection of minor projects using the SDL3 library written mostly in C. The projects are
designed to have clean, well-written code that is easy for people to understand to learn about
SDL3, graphics, C, and other computer science concepts.

## contributing

Contributions to the projects are always welcome. If you find a problem in an existing project
or wish to submit a project of your own then feel free to make a PR and I will review it. Make
sure that your code follows the style naming conventions and style guide outlined by the `.clang-format` file and make sure the code *works* as intended. Make sure to comment your code
well, especially for functions. However, your code should be readable enough to not require
many. Names for important variables should be descriptive. Temporary or utility variables can
have less descript names. If you are adding a project of your own make sure that it integrates
with the build system.

## build

```
git clone https://github.com/Icosa-dev/sdl-projects.git
cd sdl-projects
mkdir build
cd build
cmake ..
make

# to run a program assuming you are in the build directory
./<subproject-name>/<subproject-name>
# for example "./snake/snake"
```
