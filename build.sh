gcc -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework QuartzCore -framework OpenGL -L ./lib/macos -lraylib ./src/*.c ./external/*.c -o app -I./include -I./src -std=c11

if [ $? -ne 0 ]; then
  echo "BUILD FAILURE"
else
  echo "Build success!"
fi
