gcc -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework QuartzCore -framework OpenGL -L ./lib/macos -lraylib ./tests/*.c ./src/*.c ./external/*.c -o omp_tests -I./include -I./src -std=c11

if [ $? -ne 0 ]; then
  echo "TEST FILES BUILD FAILURE"
else
  echo "Test files build success!"
fi
