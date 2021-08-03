mkdir -p Window1.app/contents/macOS
clang++ -Wno-deprecated-declarations -o Window1.app/contents/macOS/Window1 Window.mm -framework Cocoa -framework AppKit -framework QuartzCore -framework OpenGL