mkdir -p Window.app/contents/macOS
clang -Wno-deprecated-declarations -o Window.app/contents/macOS/Window Window.m -framework Cocoa -framework AppKit -framework QuartzCore -framework OpenGL