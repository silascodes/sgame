cmake_minimum_required(VERSION 3.6)
project(sgame)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic -std=c++11")

include_directories(src lib)

set(SOURCE_FILES lib/MicroPather/micropather.cpp src/main.cpp src/game/Game.cpp src/game/Game.h src/game/Map.cpp src/Log.cpp src/Log.h src/game/Map.h src/util/util.h src/util/util.cpp src/client/Client.h src/client/Client.cpp src/graphics/Renderer.h src/graphics/Renderer.cpp src/game/State.h src/game/State.cpp src/ui/UI.h src/ui/UI.cpp src/client/Input.cpp src/client/Input.h src/game/Sprite.h src/ui/Screen.h src/ui/Widget.h src/graphics/Graphics.h src/ui/Screen.cpp src/ui/Widget.cpp src/ui/widgets/Label.h src/ui/widgets/Button.h src/ui/widgets/ListBox.h src/ui/widgets/Textbox.h src/ui/widgets/Button.cpp src/ui/widgets/Label.cpp src/ui/widgets/ListBox.cpp src/ui/widgets/Textbox.cpp src/ui/widgets/Container.h src/ui/widgets/Container.cpp lib/LuaWrapper/luawrapper.h lib/LuaWrapper/luawrapperutil.h src/ui/Widget_LuaI.cpp src/ui/Screen_LuaI.cpp src/ui/widgets/Button_LuaI.cpp src/game/Entity.h src/game/Game_LuaI.cpp src/game/State_LuaI.cpp src/graphics/Texture.h src/graphics/Texture.cpp src/util/ISerializable.h src/util/Serializer.h src/util/Serializer.cpp src/util/Deserializer.h src/util/Deserializer.cpp src/game/Entity.cpp src/game/Entity_LuaI.cpp src/game/Sprite.cpp src/game/Sprite_LuaI.cpp src/util/Vector2.h src/graphics/Camera.h src/graphics/Camera.cpp src/util/Rect.h src/client/IInputListener.h src/client/MouseEvent.h src/client/KeyboardEvent.h src/client/Client_LuaI.cpp src/ui/widgets/Container_LuaI.cpp src/ui/widgets/ListBox_LuaI.cpp)
add_executable(sgame ${SOURCE_FILES})

target_link_libraries(sgame json-c SDL2 SDL2_image SDL2_ttf lua5.3 Box2D)