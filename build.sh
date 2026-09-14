g++ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework QuartzCore -framework OpenGL -L ./lib/macos -lraylib ./src/*.cpp -o app -I./include -I./src -std=c++17

if [ $? -ne 0 ]; then
  echo "BUILD FAILURE"
else
  echo "Build success!"
fi
