gcc -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework QuartzCore -framework OpenGL -L ./lib/macos -lraylib ./src/*.c ./src/config/*.c ./src/ui/*.c ./src/core/*.c ./src/music/*.c ./src/osu/*.c ./src/utils/*.c ./external/*.c -o omp -I./include -I./src -std=c11

if [ $? -ne 0 ]; then
  echo "BUILD FAILURE"
else
  echo "Build success!"
fi
